#include <iostream>
#include <map>
#include <memory>

using namespace std;

class StringComp {
public:
  StringComp() { cout << "StringComp constructor" << this << endl; }
  ~StringComp() { cout << "StringComp destructor" << this << endl; }

  bool operator()(char const* argOne, char const* argTwo) const {
    return std::strcmp(argOne, argTwo) < 0;
   }
};

class Delete {
  void operator() (StringComp* p) {
    std::cout << "Calling delete for StringComp object... \n";
    delete p;
  }
};

typedef map<const char*, int, StringComp> Employee;

int* findFromMap(const Employee* mymap, const char* pattern) {
  int* retPtr = NULL;
  map<const char*, int, StringComp>::const_iterator it;
  it = mymap->find(pattern);
  if (it != mymap->end()) {
    retPtr = (int*)&(it->second);
  }

  return retPtr;
}

Employee* loadMap() {
  Employee* mymap = new Employee;
  if (mymap) {
    mymap->insert(std::pair<const char*, int>("LMN", 1024));
    mymap->insert(map<const char*, int>::value_type("XYZ", 1000));
    (*mymap)["ABC"] = 1067;
    (*mymap)["PQR"] = 2019;

    cout << "Map size: " << mymap->size() << endl;
    map<const char*, int, StringComp>::iterator begin;
    for(begin = mymap->begin(); begin != mymap->end(); ++begin) {
      cout << begin->first << ": " << begin->second << endl;
    }
  }

  return mymap;
}

int testMap() {
  Employee* emp = loadMap();
  if (emp) {
    int* idPtr = findFromMap(emp, "ABC");
    if (idPtr) {
      cout <<"ID: " << *idPtr << endl;
      *idPtr = 2689;
      idPtr = findFromMap(emp, "ABC");
      if (idPtr) {
        cout <<"ID: " << *idPtr << endl;
      }
    }
    delete emp;
  }
  return 0;
}

unique_ptr<StringComp> getUniquePtr() {
  cout << "getUniquePtr" << endl;
  unique_ptr<StringComp> ptr1 = make_unique<StringComp>();
  return ptr1;
}

int testUniquePtr() {
  cout << "testUniquePtr" << endl;
  unique_ptr<StringComp> ptr1 = make_unique<StringComp>();
  unique_ptr<StringComp> ptr2 = make_unique<StringComp>();
  unique_ptr<StringComp> ptr3 = getUniquePtr();
  unique_ptr<StringComp> ptr4;

  if (ptr4 == NULL) {
    cout << "its NULL" << endl;
  } else {
    cout << "its not NULL" << endl;
  }
  cout << "testUniquePtr after getUniquePtr" << endl;
  return 0;
}

int main() {
  testMap();
  testUniquePtr();
  return 0;
}

