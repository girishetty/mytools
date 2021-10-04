#include <iostream>
#include <fstream>
#include <sstream>
#include "normalizer.h"

using std::cout;

/*
 * Normalized node (Opening and closing in 2 different lines):
 * <Node xyz>
 * </Node>
 *
 * Normalize following cases:
 *
 * Case#1 (Opening and closing in same line):
 * <Node xyz> </Node>
 *
 * Case#2 (Opening and closing in the same node):
 * <Node  xyz/>
 *
 */
bool XmlNode::normalize() {
  bool bSuccess = false;
  auto beginNodeIndex = mNodeData.find('<');
  if (beginNodeIndex != std::string::npos) {
    auto endNodeNameIndex = mNodeData.find(' ');
    if (endNodeNameIndex != std::string::npos) {
      auto nodeName = mNodeData.substr(beginNodeIndex + 1, endNodeNameIndex - beginNodeIndex);
      auto selfEndingNodeIndex = mNodeData.find("/>");
      if (selfEndingNodeIndex != std::string::npos) {
        // Case#2. So re-adjust begin node
        mBeginNode = mNodeData;
        mBeginNode.erase(selfEndingNodeIndex, 1);
        bSuccess = true;
        mNodeType = SelfClosing;
      } else {
        auto nodeEndIndex = mNodeData.find(">");
        if (selfEndingNodeIndex != std::string::npos) {
          // Case#1. Break it into begin node
          mBeginNode = mNodeData.substr(0, nodeEndIndex + 1);
          bSuccess = true;
          mNodeType = LineClosing;
        }
      }
      if (bSuccess) {
        // Construct the end node
        mEndNode = mNodeData.substr(0, beginNodeIndex + 1);
        mEndNode += "/";
        mEndNode += nodeName;
        mEndNode += ">";
      }
    }
  }
  return bSuccess;
}

NodeType NormalizeXml::readNextNode(size_t& position, std::string& nodeData) const {
  NodeType nodeType = None;
  size_t index = 0;
  nodeData.clear();

  auto beginCommentNodeIndex = mXml.find("<?", position);
  auto beginClosingNodeIndex = mXml.find("</", position);
  auto beginNodeIndex = mXml.find('<', position);
  auto notCommentNode = mXml.find_first_of("<\r\n", position, beginCommentNodeIndex - position);
  auto notClosingNode = mXml.find_first_of("<\r\n", position, beginClosingNodeIndex - position);
  if (beginCommentNodeIndex != std::string::npos && !notCommentNode) {
    // handle comment nodes - mark it as regular node
    auto endCommentNodeIndex = mXml.find("?>", beginCommentNodeIndex + 2);
    if (endCommentNodeIndex != std::string::npos) {
      nodeData = mXml.substr(position, endCommentNodeIndex + 2 - position);
      position = endCommentNodeIndex + 2;
      nodeType = Regular;
    } else {
      cout << "ERROR: No end for Comment Node!!" << std::endl;
    }
  } else if (beginClosingNodeIndex != std::string::npos && !notClosingNode) {
    // handle closing nodes - mark it as regular node
    auto endClosingNodeIndex = mXml.find('>', beginClosingNodeIndex + 2);
    if (endClosingNodeIndex != std::string::npos) {
      nodeData = mXml.substr(position, endClosingNodeIndex + 1 - position);
      position = endClosingNodeIndex + 1;
      nodeType = Regular;
    } else {
      cout << "ERROR: No end for Closing Node!!" << std::endl;
    }
  } else if (beginNodeIndex != std::string::npos) {
    auto endNodeNameIndex = mXml.find(' ', beginNodeIndex + 1);
    if (endNodeNameIndex != std::string::npos) {
      auto nodeName = mXml.substr(beginNodeIndex + 1, endNodeNameIndex - beginNodeIndex);
      auto nodeEndIndex = mXml.find('>', endNodeNameIndex + 1);
      cout << "NodeName: " << nodeName << "nodeEndIndex: " << nodeEndIndex << ":" << mXml[nodeEndIndex - 1] << std::endl;
      if (nodeEndIndex != std::string::npos) {
        if (mXml[nodeEndIndex - 1] == '/') {
          // Self ending node
          cout << "Self ending node" << std::endl;
          nodeType = SelfClosing;
        } else {
          // See if there is an immediate end node for this
          auto endNodeIndex = mXml.find("</", nodeEndIndex + 1);
          cout << "endNodeIndex: " << endNodeIndex << std::endl;
          if (endNodeIndex != std::string::npos) {
            // See if there is any newline in between.
            // If so - Regular node, else line closing node
            auto found = mXml.find_first_of("\r\n", nodeEndIndex + 1, endNodeIndex - nodeEndIndex);
            if (found == std::string::npos) {
              // Line ending node
              nodeEndIndex = mXml.find('>', position + endNodeIndex + 1);
              nodeType = LineClosing;
            } else {
              // Regular node - nothing to be done
              nodeType = Regular;
            }
          }
        }
        // construct the node and move the position
        nodeData = mXml.substr(position, nodeEndIndex + 1 - position);
        position = nodeEndIndex + 1;
      } else {
        cout << "ERROR: No End for " << nodeName << " Node!!" << std::endl;
      }
    }
  }

  cout << nodeData << std::endl;
  size_t size = mXml.size();
  if (position < size) {
    if (mXml[position] == '\n') {
      position++;
      if (position < size) {
        if (mXml[position] == '\r') position++;
      }
    } else if (mXml[position] == '\r') {
      position++;
      if (position < size) {
        if (mXml[position] == '\n') position++;
      }
    }
  }

  return nodeType;
}

bool NormalizeXml::start(const std::string& outFile) const {
  bool   bSuccess = false;
  size_t index = 0;
  size_t size = mXml.size();
  std::string nodeData;
  size_t count = 0;

  std::ofstream normalizedFile(outFile, std::ios::binary);

  while (index < size) {
    NodeType nodeType = readNextNode(index, nodeData);
    if (nodeType == SelfClosing || nodeType == LineClosing) {
      XmlNode node(nodeData);
      bSuccess = node.normalize();
      if (bSuccess) {
        normalizedFile << node.beginNode() << '\n' << node.endNode() << '\n';
      } else {
        cout << "Failed to normalize the Node: " << nodeType << std::endl;
      }
    } else if (nodeType == Regular) {
      normalizedFile << nodeData << '\n';
      bSuccess = true;
    } else if (nodeType == None) {
      cout << "Couldn't read any node" << std::endl;
      bSuccess = false;
    }
    count++;
    if (count == 10) break;
  }

  return bSuccess;
}
