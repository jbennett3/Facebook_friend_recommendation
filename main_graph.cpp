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
	
	int friends;
	cout << "Enter person to find (The people are represented by integers so only enter in an integer value ): " << endl;
	cin >> friends;
	auto start = std::chrono::high_resolution_clock::now();
	g.friends(friends);
	
	auto finished = std::chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = finished - start;//time it took to generate the  graph using facebook text file//
	cout << "---------------------------" << endl;
	cout << "\nTime taken to execute Program: " << elapsed.count() * 1000 << "ms" << endl;
	cout << "---------------------------" << endl;
	cin.get();
	cin.get();
	return 0;
}
