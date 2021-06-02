#pragma once

#include <string>

enum NodeType {
  None,
  SelfClosing,
  LineClosing,
  Regular,
};

class XmlNode {
public:
  XmlNode(const std::string& nodeData) : mNodeData(nodeData) {}
  NodeType nodeType() const { return mNodeType; }
  bool normalize();
  const std::string& beginNode() const { return mBeginNode; }
  const std::string& endNode() const { return mEndNode; }

protected:
  void parse(const std::string& nodeData);

private:
  std::string mNodeData;
  std::string mBeginNode;
  std::string mEndNode;
  NodeType    mNodeType = None;
};

class NormalizeXml {
public:
  NormalizeXml(const std::string& xml) : mXml(xml) {}
  bool start(const std::string& outFile) const;

protected:
  NodeType readNextNode(size_t& position, std::string& nodeData) const;

private:
  const std::string mXml;
  std::string       mNormalizedXml;
};
