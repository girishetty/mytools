#ifndef RESOURCE_H
#define RESOURCE_H

//MACRO which enable use of list (else vector will be used)
#define LIST

#ifdef LIST
#include <list>
using std::list;
#else
#include <vector>
using std::vector;
#endif //LIST

using namespace std;

#define MAX_NODES 100

class Resource {

public:
	Resource(const char* rname);
	Resource(const Resource& );
	~Resource();
	void AddDependency(Resource* const resource);
	void AddDependsOnMe(Resource* const resource);
	void RemoveDependency(Resource* const resource);
	void InformDeleted();
	inline const char* const Name() const;
	inline void Display() const;
	void DisplayInfo() const;
	void StoreInfo(ofstream& configFile) const;
	Resource& operator=(const Resource& res);
	inline const char* const IsUsable() const;
private:
	char* name;
	bool isUsable;
#ifdef LIST
	list<Resource*> dependsOn;
	list<Resource*> dependsOnMe;
#else
	vector<Resource*> dependsOn;
	vector<Resource*> dependsOnMe;
#endif //LIST
};


class Graph {
public:
	static Graph* ConstructGraph(const char* fileName);
	~Graph();

	void AddNode(const char* nodeDetails);
	void AddNodeToGraph(const char* nodeDetails);
	void DeleteNode(const char* nodeName);
	void DisplayGraph() const;
	void StoreGraph(const char* fileName) const;

private:
	Graph() : count (0) {
	}

	Graph(const Graph& graph) : count (graph.count) {
	}

	void AddNode(Resource*, const char* dependencyList);
	Resource* GetResource(const char* name, bool createFlag = true);
	
	Resource* graph[MAX_NODES];
	int count;
	int foundIndex;
};

#endif //RESOURCE_H

