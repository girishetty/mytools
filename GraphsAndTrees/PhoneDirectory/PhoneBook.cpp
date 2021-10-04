#include <iostream>
#include <string.h>
#include "Node.h"
#include "PhoneBook.h"

using std::cout;
using std::endl;

PhoneBook::PhoneBook() {
  //Root Node does not save any Entry as such
  Entry rootEntry("Root", nullptr);
  iRoot = new Node('\0', rootEntry);
}

PhoneBook::~PhoneBook() {
  CleanupPhoneBook(iRoot);
}

void PhoneBook::CleanupPhoneBook(Node* pNode) {
  if (pNode) {
    pNode->DeleteDepthNodes();
  }
}

Node* PhoneBook::FindNode(const char* key) const {
  Node* foundNode = nullptr;
  if (key && *key != '\0') {
    int length = strlen(key);
    char subkey;
    Node* currNode = iRoot;
    
    for (int index = 0; index < length; index++) {
      subkey = key[index];
      foundNode = currNode->GetChildNodeByKey(subkey);
      if (foundNode){
        //Navigate thru the path
        currNode = foundNode;
      } else {
        //there is not one, so return
        foundNode = nullptr;
        break;
      }
    }
  }
  return foundNode;
}

void PhoneBook::AddEntry(const char* pName, const char* pPhoneNum) {
  Entry phoneEntry(pName, pPhoneNum);
  AddEntry(phoneEntry);
}

void PhoneBook::AddEntry(const Entry& anEntry) {
  const char* pName = anEntry.Name();
  if (pName && (*pName != '\0')) {
    int length = strlen(pName);
    char key;
    Node* currNode = iRoot;
    Node* childNode = nullptr;
    Entry blankEntry(nullptr, nullptr);
    
    for (int index = 0; index < length; index++) {
      key = pName[index];
      childNode = currNode->GetChildNodeByKey(key);
      //if this is a new node, create one
      if (!childNode) {
        //If we have reached the last character, then its a terminal node
        if (index == length-1) {
          //So store the Entry
          childNode = new Node(key, anEntry);
          childNode->IsTerminal(true);
        } else {
          //Not a terminal node, so dont have to store anything, store some dummy value
          childNode = new Node(key, blankEntry);
        }

        //Add the Node to the currNode
        currNode->AddChild(childNode);
        currNode = childNode;
      } else {
        currNode = childNode;
      }
    }
  }
}

bool PhoneBook::Find(const char* key) const {
  bool found = false;

  //Find the Node with key as the key
  if (FindNode(key) != nullptr) {
    found = true;
  }

  return found;
}

void PhoneBook::ShowEntriesStartingFromKey(const char* key) const {
  Node* pNode = FindNode(key);
  cout << "Names starting with [" << key << "] are : " << endl;
  if (pNode) {
    pNode->DisplayDepthNodes();
  }
}
