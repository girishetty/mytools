#include <iostream>
using namespace std;

#include "..\inc\resource.h"

inline char DisplayOptions() {
	char choice;
	cout<<endl<<"\n\nMake a choice"<<endl;
	cout<<"q --> for quiting the application"<<endl;
	cout<<"d --> for deleting a node from Graph"<<endl;
	cout<<"p --> for printing the Graph"<<endl;
	cout<<"s --> for storing the Graph"<<endl;
	cout<<"a --> for adding a node to the Graph"<<endl;
	cin>>choice;
	return choice;
}


int main() {
	//construct the graph using the config file
	Graph* graph = Graph::ConstructGraph("config.txt");
	//Display the Graph
	graph->DisplayGraph();

	char choice;
	char nodeName[20];
	char nodeDetails[100];

	do{
		choice = DisplayOptions();
		switch(choice) {
		case 'q' :
			break;
		case 'd' :
			cout<<"Enter Node Name to be deleted : ";
			cin>>nodeName;
			graph->DeleteNode(nodeName);
			break;
		case 'p':
			graph->DisplayGraph();
			break;
		case 's':
			graph->StoreGraph("config1.txt");
			break;
		case 'a':
			cout<<"Enter Node Name to be deleted : ";
			cin>>nodeDetails;
			graph->AddNodeToGraph(nodeDetails);
			break;
		default:
			cout<<"Invaild selection!!"<<endl;
		}
	}while(choice != 'q');

	graph->StoreGraph("config1.txt");
	graph->DisplayGraph();
	delete graph;
	return 0;
}
