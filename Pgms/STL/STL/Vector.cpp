#include <iostream>
#include <vector>

using namespace std;

void display_vector(const vector<int>& my_vector) {
    cout<<"my_vector.size():     "<<my_vector.size()<<endl;
    cout<<"my_vector.capacity(): "<<my_vector.capacity()<<endl;
    cout<<"Vector Contains:      "<<endl;
    vector<int>::const_iterator cii;
    for(cii=my_vector.begin(); cii!=my_vector.end(); cii++) {
        cout << *cii << endl;
    }
    cout<<endl;
}

void add_some_elements(vector<int>& my_vector, int size) {

    for(int i=0; i<size; i++) {
#if _CRASHES
        //This crashes as we have not set the size for the vector yet.
        //But this works if ever we had initialized vector as
        //vector<int> my_vector(10);
        //or called
        //my_vector.resize(10);

        my_vector.at(i)=rand();
        //or
        //my_vector[i]=rand();
#endif
        //push_back changes the size
        my_vector.push_back(rand());
    }
}

void modify_some_elements(vector<int>& my_vector) {

    vector<int>::iterator pos = my_vector.begin();
    pos = my_vector.insert(pos, 333);
    pos++;
    *pos = 100;
    pos++;
    *pos = 200;
    pos +=2;
    //insert Changes the size and returns the new pos as the return value, 
    //so unless we capture the return, pos is not valid anymore
    my_vector.insert(pos, 333);
#if _CRASHES
    //after insert, pos changes (reference to pos is not valid anymore), so assigning onto it Crashes
    //So, capture the return of insert, if you need the pos
    *pos = 10000;
    pos++;
    *pos = 400;
    my_vector.insert(pos, 7777);
#endif
    pos = my_vector.begin();
    pos += 8;
    my_vector.insert(pos, 3333);
    my_vector.push_back(rand());
}
    
void remove_some_elements(vector<int>& my_vector, int count) {

    for(int i=0; i<count; i++) {
        //pop_back changes the size
        my_vector.pop_back();
    }
    //OR
    //my_vector.erase(my_vector.end()-count+1, my_vector.end());
}

void remove_some_elements(vector<int>& my_vector, int count, int from) {

    //adjust the count if there are less elements from from_position to end of list
    if(my_vector.size() <= (from + count)) {
        count = my_vector.size() - from - 1;
    }
    my_vector.erase(my_vector.begin() + from, my_vector.begin() + from + count);
}

void copy_vectors(vector<int>& my_vector, int size) {

    vector<int> my_copy_vector;
    add_some_elements(my_copy_vector, size);

    //Inserting my_copy_vector, after 4th element of my_vector
    my_vector.insert(my_vector.begin()+5, my_copy_vector.begin(), my_copy_vector.end());

    //and erase contents of my_copy_vector
    my_copy_vector.erase(my_copy_vector.begin(), my_copy_vector.end());
}

void some_vector_examples() {
    vector<int> my_vector;

    display_vector(my_vector);
    add_some_elements(my_vector, 10);
    display_vector(my_vector);

    my_vector.resize(15, 999);
    display_vector(my_vector);

    modify_some_elements(my_vector);
    display_vector(my_vector);

    //remove 5 elements from the back/end
    remove_some_elements(my_vector, 5);
    display_vector(my_vector);

    //copy from another vector of size 7
    copy_vectors(my_vector, 7);
    display_vector(my_vector);

    //remove 6 elements from 8th position
    remove_some_elements(my_vector, 6, 8);
    display_vector(my_vector);
}
