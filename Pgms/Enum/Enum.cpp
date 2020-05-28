#include <iostream>
using namespace std;

enum Bool {
        False = 0,
        True = 1
};

enum Month {
        Jan = 1,
        Feb, Mar, Apr, May, Jun, July, Aug, Sep, Oct, Nov, Dec };

enum Int {
        One,
        Thousand = 1000
        };

int main(){
        cout<<"Size of bool is : "<<sizeof(bool)<<endl;
        cout<<"Size of Bool is : "<<sizeof(Bool)<<endl;
        cout<<"Size of Month is : "<<sizeof(Month)<<endl;
        cout<<"Size of Int is : "<<sizeof(Int)<<endl;

        Month month = Oct;
        //month = 100; //This is ERROR cant convert in to Month
        month = Month(100);

        return 0;
}
