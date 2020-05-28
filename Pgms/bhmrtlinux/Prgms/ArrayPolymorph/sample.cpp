#include <stdio.h>

class base {
protected:
	int a;

public:
	base() {
	printf("base constructor.\n");
	}
	
	virtual ~base(){
    printf("base destructor.\n");
	}

	virtual void print(){
    printf("base print.\n");
	}

};

class sub : public base{
protected:
	float f;
public:
	sub() {
    printf("sub constructor.\n");
	}
	virtual ~sub(){
    printf("sub destructor.\n");
	}

	virtual void print(){
    printf("sub print.\n");
	}

};

void fun(base arr[], int size)
{
   for(int i = 0; i < size; i++)
      {
          printf("Call # %d\n", i+1);
          arr[i].print();	//the problamatic code
      }
}

int main ()
{
   base *b = new sub [2];
   b[0].print();
   fun(b, 2);
   delete []b;
   //delete [](sub*)b;
   getchar();
    return 0;
}

