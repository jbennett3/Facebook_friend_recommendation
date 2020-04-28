#ifndef GRAPPHS_H_INCLUDED
#define GRAPPHS_H_INCLUDED
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <string>
#include <algorithm>
#include <utility>
#include <functional>
#include <map>
#include <chrono>
#include <limits>
#include <stdexcept>
#include<execution>
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
	std::vector<Edge *>edge_list;
}Vertex;

class Graph {
private:
	std::unordered_map<int, Vertex *> vertex_list;
	int num_edges;
	void addEdge(int vs, int vt);
public:
	Graph() :num_edges(0) {};
	Graph(bool insertReverseEdge, std::ifstream& ifs); // Reads the graph from a text file of edges one per line.
	~Graph();
	int getNumVertices() { return vertex_list.size(); }
	int getNumEdges() { return num_edges; }
	void printGraph(); // For each vertex, print all the vertices it is connected to
	void friends(int friends);
	std::vector<int>mutual_friends(std::vector<int> &friend1,std::vector<int> &friend2);
	std::vector<int>non_mutual(std::vector<int> &friend1,std::vector<int> &friend2);
	std::vector<int>iterater_friends_of_friends(int d);
	void print_friend(std::vector<int>&mutual_friend_results);
};


#endif // GRAPPHS_H_INCLUDED
