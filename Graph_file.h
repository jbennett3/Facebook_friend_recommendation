#ifndef GRAPPHS_H_INCLUDED
#define GRAPPHS_H_INCLUDED
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
typedef struct
{
	int num_of_friends;
}Friends;
typedef struct {
	bool visited; // Add other fields if needed
}Data;

typedef struct {
	int target_vertex;
	int weight;
}Edge;

typedef struct {
	int vertex_id;
	Data *data;
	vector<Edge *>edge_list;
}Vertex;

class Graph {
private:
	unordered_map<int, Vertex *> vertex_list;
	int num_edges;
	void addEdge(int vs, int vt);
public:
	Graph() :num_edges(0) {};
	Graph(bool insertReverseEdge, ifstream& ifs); // Reads the graph from a text file of edges one per line.
	~Graph();
	int getNumVertices() { return vertex_list.size(); }
	int getNumEdges() { return num_edges; }
	void printGraph(); // For each vertex, print all the vertices it is connected to
	//void BFS(int vs); // Visits nodes breadth first
	//void DFS(int vs); // Visits nodes depth first
	void friends(int friends);
	vector<int>mutual_friends(vector<int> &friend1, vector<int> &friend2);
	vector<int>non_mutual(vector<int> &friend1,vector<int> &friend2);
	vector<int>sdd(int d);
	void printsfriends(vector<int>&mutual_friend_results);



};


#endif // GRAPPHS_H_INCLUDED
