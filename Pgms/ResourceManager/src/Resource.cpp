#include <iostream>
#include <fstream>

#include "..\inc\resource.h"

Resource::Resource(const char* rname) : isUsable(true) {
	int len = strlen(rname);
	name = new char[len+1];
	strcpy(name, rname);
}

Resource::Resource(const Resource& resource) {
	int len = strlen(resource.name);
	name = new char[len+1];
	strcpy(name, resource.name);
	isUsable = resource.isUsable;
}

Resource::~Resource() {
	delete [] name;
	dependsOn.clear();
	dependsOnMe.clear();
}

const char* const Resource::Name() const {
	return name;
}

void Resource::AddDependency(Resource* const resource) {
	//push the element onto the list
	dependsOn.push_back(resource);
	resource->AddDependsOnMe(this);
}

void Resource::AddDependsOnMe(Resource* const resource) {
	//push the element onto the list
	dependsOnMe.push_back(resource);
}

void Resource::RemoveDependency(Resource* const resource) {
	//remove the element from the list
	isUsable = false;

#ifdef LIST
	dependsOn.remove(resource);
#else
	//int size = dependsOn.size();
	//dependsOn.remove(resource);
#endif //LIST
}

void Resource::InformDeleted() {
	//Inform those who has dependency on this about the deletion
#ifdef LIST
	list<Resource*>::iterator itBegin = const_cast<Resource*>(this)->dependsOnMe.begin();
	list<Resource*>::iterator itEnd = const_cast<Resource*>(this)->dependsOnMe.end();

	for(;itBegin != itEnd; itBegin++) {
		(*itBegin)->RemoveDependency(this);
	}
#else
	int size = dependsOnMe.size();
	for(int i=0;i<size;i++) {
		dependsOnMe[i]->RemoveDependency(this);
	}
#endif //LIST
}

Resource& Resource::operator=(const Resource& res) {
	if( this != &res) {
		delete name;
		int len = strlen(res.name);
		name = new char[len];
		strcpy(name, res.name);
	}
	return *this;
}

inline const char* const Resource::IsUsable() const{
	if(isUsable)
		return "Yes";
	else
		return "No";
}

inline void Resource::Display() const {
	cout<<name<<" ";
}

void Resource::DisplayInfo() const {
	//Display the name of the resource
	cout<<endl<<"Name: ";
	Display();

	cout<<"-- Depends on : ";
	//Display the dependency list

#ifdef LIST
	list<Resource*>::iterator itBegin = const_cast<Resource*>(this)->dependsOn.begin();
	list<Resource*>::iterator itEnd = const_cast<Resource*>(this)->dependsOn.end();

	for(;itBegin != itEnd; itBegin++) {
		(*itBegin)->Display();
	}
#else
	int size = dependsOn.size();
	for(int i=0;i<size;i++) {
		dependsOn[i]->Display();
	}
#endif //LIST
	cout<<"-- Usable : "<<IsUsable();
}

void Resource::StoreInfo(ofstream& configFile) const {
	//Write on to file, only if it depends on some resources
	if(dependsOn.size()>0) {
		//first write the name of the resource
		configFile<<name;

#ifdef LIST
		//write all the dependent node
		list<Resource*>::iterator itBegin = const_cast<Resource*>(this)->dependsOn.begin();
		list<Resource*>::iterator itEnd = const_cast<Resource*>(this)->dependsOn.end();

		for(;itBegin != itEnd; itBegin++) {
			configFile<<" "<<(*itBegin)->Name();
		}
#else
		int size = dependsOn.size();
		for(int i=0;i<size;i++) {
			configFile<<" "<<dependsOn[i]->Name();
		}
#endif //LIST
		configFile<<endl;
	}
}

Graph::~Graph() {
	for(int i=0;i<count;i++) {
		delete graph[i];
	}
}

Graph* Graph::ConstructGraph(const char* fileName) {
	//create graph
	Graph* graph = new Graph;

	//Open config file in read mode
	ifstream configFile(fileName);
	char line[100];

	while(configFile.good()){
		//Read the first node
		configFile.getline(line, 100, ' ');

		if(configFile.good()) {
			//construct the resource
			Resource* resource = graph->GetResource(line);

			//read rest of the dependenct nodes from the file
			configFile.getline(line, 100);
			graph->AddNode(resource, line);
		}
	}

	configFile.close();
	return graph;
}

void Graph::AddNodeToGraph(const char* nodeDetails) {
	if(nodeDetails) {
		string details(nodeDetails);
		int endIndex = 0;
		//Read the first node
		endIndex = details.find(' ', 0);

		if(endIndex == string::npos) {
			endIndex = details.length();
		}

		//construct temporary name
		int len = endIndex+1;
		char* resName = new char[len];
		const char* ptr = details.c_str();
		strncpy(resName, ptr, len);
		resName[len-1] = '\0';

		//Check for the availability
		Resource* resource = GetResource(resName);
		//Free the temporary memory
		delete [] resName;

		//create the dependency list
		AddNode(resource, nodeDetails+endIndex);
	}
}


void Graph::AddNode(Resource* res, const char* dependencyList) {

	//Fetch each resource on which this resource is dependent on
	string dList(dependencyList);
	int startIndex = 0;
	int endIndex = 0;
	bool whileCond = true;

	do {
		endIndex = dList.find(' ', startIndex);
		if(endIndex == string::npos) {
			endIndex = dList.length();
			whileCond = false;
		}
		//Construct temporary name
		int len = endIndex-startIndex+1;
		char* resName = new char[len];
		const char* ptr = dList.c_str();
		ptr = ptr + startIndex;
		strncpy(resName, ptr, len);
		resName[len-1] = '\0';

		//Check for the availability
		Resource* resource = GetResource(resName);
		//Free the temporary memory
		delete [] resName;
		res->AddDependency(resource);

		startIndex = endIndex + 1;
	}while(whileCond);
}

Resource* Graph::GetResource(const char* name, bool createFlag) {
	//Check for the availability
	int freeSlot = -1;
	foundIndex = -1;
	Resource* resource = NULL;
	bool found = false;
	for(int i=0; i<count; i++) 	{
		if(graph[i]){
			if( strcmp(name, graph[i]->Name()) == 0){
				found = true;
				resource = graph[i];
				foundIndex = i;
				break;
			}
		}
		else {
			//we have got a free slot now.
			freeSlot = i;
		}
	}

	if(!found && createFlag) {
		//Its not yet added to the graph, so construct the resource
		resource = new Resource(name);

		if(freeSlot != -1) {
			//there is a free slot, re-use it, without incrementing count
			i = freeSlot;
		}
		else {
			count++;
		}
		//Add this resource to list of resources
		graph[i] = resource;
	}
	return resource;
}

void Graph::DeleteNode(const char* nodeName) {
	//Check for the availability of the node
	Resource* resource = GetResource(nodeName, false);
	if(resource) {
		//Inform those who has dependency on this about the deletion
		resource->InformDeleted();
		delete resource;
		//Set the entry as free
		graph[foundIndex] = NULL;
	}
	else {
		cout<<nodeName<<" does not present in the Graph"<<endl;
	}
}

void Graph::DisplayGraph() const {
	for(int i=0;i<count;i++) {
		if(graph[i])
			graph[i]->DisplayInfo();
	}
}

void Graph::StoreGraph(const char* fileName) const {
	//Open config file in write mode
	ofstream configFile(fileName);

	if(configFile.good()) {
		for(int i=0;i<count;i++) {
			if(graph[i])
				graph[i]->StoreInfo(configFile);
		}
	}
	configFile.close();
}
