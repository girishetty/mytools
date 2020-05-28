#include <iostream>
#include <map>

using namespace std;

class StringComp {
public:
    bool operator()(char const* argOne, char const* argTwo) {
      return std::strcmp(argOne, argTwo) < 0;
   }
};

typedef map<char*, int, StringComp> Employee;

void some_map_examples() {
    Employee emp;
    emp.insert(std::pair<char*, int>("LMN", 1024));
    emp.insert(map<char*, int>::value_type("XYZ", 1000));
    emp["ABC"] = 1067;
    emp["PQR"] = 2019;

    cout << "Map size: " << emp.size() << endl;
    map<char*, int, StringComp>::iterator begin;
    for(begin = emp.begin(); begin != emp.end(); ++begin) {
        cout << (*begin).first << ": " << (*begin).second << endl;
        //cout << begin->first << ": " << begin->second << endl;
    }
}
