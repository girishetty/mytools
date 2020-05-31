#include <iostream.h>
#include <stdlib.h>
#include <conio.h>

struct node
	{
	int data;
	node *next;
	};

class list
   {
private:
	node* first;
	node* temp;

public:
	list() : first(NULL), temp(NULL) { }
	void insert(int x);
	void modify();
	void display();
	};


void list::insert(int x) 
	{
	node *temp,*p1,*p;
	temp=new node;
	if(temp==NULL)
		cout<<"insufficient memory"<<endl;
	else
		{
		temp->data=x;
		temp->next=NULL;

		if(first==NULL)
			{
			first=temp;
			return;
			}

		p1=first;
		while(p1!=NULL)
			{ 
			p=p1;
			p1=p1->next;
			}
		p->next=temp;
		}
	}

void list::modify()
	{
	node *temp, *p1,*p2;

	temp=first;

	if(temp==NULL) 
		return;
	if(temp->next==NULL && temp->next->next==NULL)
		return;
	p1 =temp;
	p2 = temp->next;
	while(p1->next !=NULL && p1->next->next!=NULL)
		{
		temp=p2;
		p1->next=p2->next;
		p2->next=p1->next->next;
		p1->next->next=p2;

		p1=p2;
		p2=p1->next;
		}
	}

void list::display()
	{
	node *temp;
	temp=first;
	if(temp==NULL)
	cout<<"list empty"<<endl;
	else 
		{
		while(temp!=NULL)
			{
			cout<<temp->data;
			temp=temp->next;
			}
		}
	}


/*
int main() {
	const int MaxNode = 7;
	list l;

	for(int i=0;i<MaxNode; i++) {
		l.insert(i+1);
	}
	
	cout<<"List is :"<<endl;
	l.display();
	l.modify();
	cout<<endl<<"List is :"<<endl;
	l.display();
	cout<<endl;

	return 0;
}
*/