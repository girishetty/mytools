#include <iostream>
#include <string>
#include "LinkedList.h"
#include "HashTable.h"

using std::string;

int main() {
  string s1("Sumathi");
  string s2("Vittal");
  string s3("Pramila");
  string s4("Harish");
  string s5("Soumya");
  string s6("Girish");
  string s7("Asha");
  string s8("Prathish");
  string s9("Ammi");
  string s0("Bhaktha");
  HashTable* pTable = HashTable::CreateHashTable();

  pTable->Insert(s1);
  pTable->Insert(s2);
  pTable->Insert(s3);
  pTable->Insert(s4);
  pTable->Insert(s5);
  pTable->Insert(s6);
  pTable->Insert(s7);
  pTable->Insert(s8);
  pTable->Insert(s9);
  pTable->Insert(s0);
  pTable->Print();
  pTable->Histogram();

  delete pTable;
  return 0;
}
