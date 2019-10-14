#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include "Graph_file.h"
using namespace std;
int main()
{


	ifstream ifs("facebook_combined_list.txt");

	Graph g(false, ifs);

	cout << "Number of vertices " << g.getNumVertices() << endl;
	cout << "Number of edges " << g.getNumEdges() << endl << endl;
	cout << "Printing graph" << endl;
	//g.printGraph();
	int frinds;
	cout << "Enter person to find: " << endl;
	cin >> frinds;
	auto start = std::chrono::high_resolution_clock::now();

	g.friends(frinds);
	// cout << "Printing vertices in BFS order starting at vertex 0 " << endl;
	 //g.BFS(0);
	 //cout << "Printing vertices in DFS order starting at vertex 0 " << endl;
	 //g.DFS(0);
	auto finished = std::chrono::high_resolution_clock::now();

	chrono::duration<double> elapsed = finished - start;
	cout << "---------------------------" << endl;
	cout << "\nTime taken to execute Program: " << elapsed.count() * 1000 << "ms" << endl;
	cout << "---------------------------" << endl;
	g.~Graph();

	return 0;
}
