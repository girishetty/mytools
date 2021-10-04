#include <iostream>

#include "..\SetAssociativeCache\include\SetAssociativeCache.h"
using namespace std;

void PrintNow(const char* key, const char* val) {
  if(val) {
    cout<<"Cache Hit: Retrieved "<<key<<":"<<val<<endl;
  }
  else {
    cout<<"Cache Miss: Retrival of "<<key<<endl;
  }
}

int TestCaseLRUOne() {
  //Set up a 4-way set associative Cache with capacity of 128 items with LRU policy
  InitSetAssociativeCache(128, 4, LRU);
  cout<<"Cache Created"<<endl;

  char* val = nullptr;

  //Add 4 entries now
  AddToCache("1234",  "John");
  AddToCache("12345", "Mathew");
  AddToCache("12346", "Berry");
  AddToCache("12347", "Matt");

  //Retrieve all the entries in the same order
  RetrieveFromCache("1234", val);
  PrintNow("1234", val);
  RetrieveFromCache("12345", val);
  PrintNow("12345", val);
  RetrieveFromCache("12346", val);
  PrintNow("12346", val);
  RetrieveFromCache("12347", val);
  PrintNow("12347", val);
  
  //Add one more entry, this should replace item in "1234"
  AddToCache("12348", "Pat");
  RetrieveFromCache("12348", val);
  PrintNow("12348", val);

  //This should fail - as "12348" replaced "1234"
  RetrieveFromCache("1234", val);
  PrintNow("1234", val);
  
  //This should pass as "12346" should still be there
  RetrieveFromCache("12346", val);
  PrintNow("12346", val);

  //Now that we are done, free up the Cache.
  DestroyCache();
  cout<<"Cache Freed up"<<endl;
  return 0;
}

void TestCaseLRUTwo() {
  //Set up a 4-way set associative Cache with capacity of 128 items with LRU policy
  InitSetAssociativeCache(128, 4, LRU);
  cout<<"Cache Created"<<endl;
  char* val = nullptr;
  //Add 4 entries
  AddToCache("1234",  "John");
  AddToCache("12345", "Mathew");
  AddToCache("12346", "Berry");
  AddToCache("12347", "Matt");

  //Retrieve them all to see the order
  RetrieveFromCache("1234", val);
  PrintNow("1234", val);
  RetrieveFromCache("12345", val);
  PrintNow("12345", val);
  RetrieveFromCache("12346", val);
  PrintNow("12346", val);
  RetrieveFromCache("12347", val);
  PrintNow("12347", val);
  
  //Retrieve "1234" again to make it top of the list (MRU)
  //And this should pass
  RetrieveFromCache("1234", val);
  PrintNow("1234", val);

  //Add one more entry, and this should replace "12345" as its the LRU
  AddToCache("12348", "Pat");
  RetrieveFromCache("12348", val);
  PrintNow("12348", val);

  //This should pass
  RetrieveFromCache("1234", val);
  PrintNow("1234", val);

  //This should fail
  RetrieveFromCache("12345", val);
  PrintNow("12345", val);

  //Now that we are done, free up the Cache.
  DestroyCache();
  cout<<"Cache Freed up"<<endl;
}

int TestCaseMRU() {
  //Set up a 4-way set associative Cache with capacity of 128 items with MRU Policy
  InitSetAssociativeCache(128, 4, MRU);
  cout<<"Cache Created"<<endl;
  char* val = nullptr;
  //Add 4 entries now
  AddToCache("1234",  "John");
  AddToCache("12345", "Mathew");
  AddToCache("12346", "Berry");
  AddToCache("12347", "Matt");

  //Retrieve all the entries in the same order
  RetrieveFromCache("1234", val);
  PrintNow("1234", val);
  RetrieveFromCache("12345", val);
  PrintNow("12345", val);
  RetrieveFromCache("12346", val);
  PrintNow("12346", val);
  RetrieveFromCache("12347", val);
  PrintNow("12347", val);
  
  //Add one more entry, this should replace item in "12347"
  AddToCache("12348", "Pat");
  RetrieveFromCache("12348", val);
  PrintNow("12348", val);

  //This should fail - as "12348" replaced "12347"
  RetrieveFromCache("12347", val);
  PrintNow("12347", val);
  
  //This should pass as "12346" should still be there
  RetrieveFromCache("12346", val);
  PrintNow("12346", val);

  //Now that we are done, free up the Cache.
  DestroyCache();
  cout<<"Cache Freed up"<<endl;
  return 0;
}

int main() {
  TestCaseLRUOne();
  cout<<"======================"<<endl;
  TestCaseLRUTwo();
  cout<<"======================"<<endl;
  TestCaseMRU();
  return 0;
}
