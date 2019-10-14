
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <queue>
#include "Graph_file.h"
#include <cstdlib>
#include <cerrno>
#include <string>
#include <stack>

#include <algorithm>
#include <utility>
#include <functional>

#include <map>
#include <chrono>
//below are headers for work around function that converts strings to integers
#include <cstdlib>
#include <cerrno>
#include <string>
#include <limits>
#include <stdexcept>
using namespace std;


namespace work_around//work around function to allow stoi
{
	int stoi(const std::string& stringss, std::size_t* position = 0, int base = 10)
	{
		const char* starts = stringss.c_str();
		char* finish = nullptr;
		long result = std::strtol(starts, &finish, base);

		if (errno == ERANGE || result > std::numeric_limits<int>::max() || result < std::numeric_limits<int>::min())
			throw std::out_of_range("stoi: out ofrange");

		if (finish == stringss.c_str())
			throw std::invalid_argument("stoi: invalid argument");

		if (position) *position = finish - starts;

		return result;
	}
}

using namespace std;
void Graph::addEdge(int vs, int vt)
{
	auto stf = vertex_list.find(vs);//look for the vertex source
	if (stf != vertex_list.end())//if the vertex source is in the hash table
	{
		stf = vertex_list.find(vt);//look for the vertex target in the hashtable
		Data *nData = new Data();
		nData->visited = false;
		Edge *nEdge = new Edge();
		nEdge->weight = 0;
		nEdge->target_vertex = vt;
		vertex_list[vs]->edge_list.push_back(nEdge);
		if (stf == vertex_list.end())//see if target is in the hashtable
		{
			Vertex *target = new Vertex();
			target->vertex_id = vt;
			Data *tarData = new Data();
			tarData->visited = false;
			target->data = tarData;
			vertex_list[vt] = target;
		}
	}
	else
	{ //if the start point (source is not in the table
		stf = vertex_list.find(vt);//use the iterator to find the target
		Data *ndata = new Data();
		ndata->visited = false;
		Edge *nEdge = new Edge();
		nEdge->target_vertex = vt;
		nEdge->weight = 0;
		Vertex *nVert = new Vertex();
		nVert->edge_list.push_back(nEdge);
		vertex_list[vs] = nVert;
		if (stf == vertex_list.end())//look for the target in the table
		{
			Vertex*target = new Vertex();
			target->vertex_id = vt;
			Data *tData = new Data();
			tData->visited = false;
			target->data = tData;
			vertex_list[vt] = target;
		}
	}
}
Graph::Graph(bool insertReverseEdge, ifstream& ifs)
{
	int starts;//vs
	int endss;//vt
	string checker = " "; //check for spaces in text file to distinguish the vs and vt

	for (string li; getline(ifs, li);)
	{//read text file and store the begin and end points of of the edges
		starts = work_around::stoi(li.substr(0, li.find(checker)));//start value (vs)
		endss = work_around::stoi(li.substr(li.find(checker) + 1, li.size()));//finish value (vt)
		addEdge(starts, endss);//add the edges
		num_edges++;//count number of edges

	}
}
Graph::~Graph()
{
	for (auto dvel : vertex_list)//transverse through vector list
	{
		for (auto& vectordestroy : dvel.second->edge_list)//destroy list
		{
			delete vectordestroy;
		}
		delete dvel.second->data;//delete data
		delete dvel.second;
	}

}



void Graph::friends(int vs)//print the contests of the edge list for vs
{
	auto startFriend = std::chrono::high_resolution_clock::now();
	vector<int>fr;//friend list
	vector<int>mut_rec;
	map<int, int>list_offriends;//holds friend index and number of mutual friends
	//vector<pair<int,int>>msust;
	//vector<int> frinddss_offr;
	auto ss = vertex_list.find(vs);//if person looking for dne
	if (ss == vertex_list.end())
	{
		cout << "The person you are looking for does not exist" << endl;
		auto finishedFriend = std::chrono::high_resolution_clock::now();
		chrono::duration<double> elapsedFriend = finishedFriend - startFriend;
		cout << "Time to execute" << elapsedFriend.count() * 1000 << "ms" << endl;
	}
	else//if person does exist
	{
		for (auto it : vertex_list)
		{
			for (auto ms : it.second->edge_list)//look throug edgelist and print vt
			{
				if (ms->target_vertex == vs)
				{
					fr.push_back(it.first);//add friend to list (vt,but no vs)
				}
			}
		}
		for (auto s : vertex_list[vs]->edge_list)//add friends from edge list to friend list
		{
			fr.push_back(s->target_vertex);
		}
		for (unsigned int i = 0; i < fr.size(); i++)//while looking through friend list
		{
			vector<int> mlp(sdd(fr[i]));//gi through friends of friends list
			vector<int>fse(mutual_friends(fr, mlp));//find number of mutual friends
			list_offriends.emplace(make_pair(fse.size(), fr[i]));//make pair of friend with number of mutual friends
		}
		auto finishedFriend = std::chrono::high_resolution_clock::now();
		chrono::duration<double> elapsedFriend = finishedFriend - startFriend;
		//  cout << "time taken to execute User Info: " << elapsedFriend.count()*1000 << "ms" << endl;
		cout << "---------------" << endl;
		cout << "This person has:" << endl;
		cout << " - " << fr.size() << " Friends." << endl;
		cout << " - " << list_offriends.size() << " Close Friends." << endl;
		cout << "   time taken:" << elapsedFriend.count() * 1000 << "ms" << endl;
		cout << "---------------" << endl;

		unsigned int topN; //Holds the "n-top" number of friends the user wants to view.
		cout << "How many top friends do you want to see?: " << endl;
		cin >> topN;
		cout << "---Creating list of Top Friends:---";

		//TIMING "TOP N FRIENDS"

		auto startTop = std::chrono::high_resolution_clock::now();

		if (topN > list_offriends.size())
		{
			cout << "\n--------------------\n";
			cout << "The user only has " << list_offriends.size() << " top friends. Listing top " << list_offriends.size() << " friends." << endl;
			cout << "--------------------\n";
			topN = list_offriends.size();

		}
		unsigned int num_list = 0;
		std::cout << "Top " << topN << " friends of user: " << endl;

		std::multimap<int, int>::reverse_iterator rit;//rank closest friends
		for (rit = list_offriends.rbegin(); rit != list_offriends.rend(); ++rit)
		{//prints closest friends in descending order
			if (num_list < topN) {
				std::cout << "  #" << num_list + 1 << " - " << rit->second << " with " << rit->first << " mutual friends" << endl;
				num_list++;
			}
			else {}

		}
		auto finishedTop = std::chrono::high_resolution_clock::now();
		chrono::duration<double> elapsedTop = finishedTop - startTop;
		cout << "  time taken:" << elapsedTop.count() * 1000 << "ms" << endl;
		cout << "---------------------------" << endl;
		//TIMING END "TOP N FRIENDS"
					//cout << "Time taken to execute Top Friends command: " << elapsed.count()*1000 << "ms" << endl << endl;
					//cout << "Num of mutual friends: " << list_offriends.size() << endl;

		//TIMING "FRIEND RECOMMENDATION LIST"
		cout << "Creating Friend Recommendation List: t=";
		auto startRecom = std::chrono::high_resolution_clock::now();
		for (rit = list_offriends.rbegin(); rit != list_offriends.rend(); ++rit)
		{
			mut_rec.push_back(rit->second);
		}

		vector<int> recommendThese;

		int random_num = (rand() % mut_rec.size()); // generate random number to pick random mutual friend from friend list.
		int random_friend = mut_rec[random_num]; //The actual user which got randomly selected

		//cout << "\nChose: " << random_friend << " as user to get suggestions from" << endl;

		vector<int>particularFriendList;

		for (auto s : vertex_list[random_friend]->edge_list)
		{
			particularFriendList.push_back(s->target_vertex);
		}

		//cout << "THIS SIZE: " << particularFriendList.size() << endl;

		recommendThese = non_mutual(fr, particularFriendList); //find non-mutual friends between the current person and their randomly selected friend

//TIMING END "FRIEND RECOMMENDATION LIST"
		auto finishedRecom = std::chrono::high_resolution_clock::now();
		chrono::duration<double> elapsedRecom = finishedRecom - startRecom;
		//cout << "Time taken to execute Find Suggestions Command: " << elapsedRecom.count()*1000 << "ms" << endl << endl;
		cout << elapsedRecom.count() * 1000 << "ms" << endl;
		cout << "-------------------------" << endl;
		//cout << "Size of difference" << recommendThese.size() << endl;
		cout << "User has " << recommendThese.size() << " friend suggestions. How many suggestions would you like to list? " << endl;
		unsigned int num_suggest;
		cin >> num_suggest;
		if (num_suggest > recommendThese.size())
		{
			num_suggest = recommendThese.size();
			cout << "Only " << recommendThese.size() << " are available." << endl;
			cout << "Printing " << recommendThese.size() << " recommendations." << endl;
		}
		cout << "----------Suggestions----------" << endl;
		for (int i = 0; i < num_suggest; i++)
		{
			cout << "Recommendation #" << i + 1 << endl;
			cout << recommendThese[i] << endl;

		}
		cout << "---End of Suggestions---" << endl;
	}



}

vector<int>Graph::sdd(int d)//function to help look through and create friends of friends list
{
	vector<int>r;
	auto ss = vertex_list.find(d);
	if (ss == vertex_list.end())
	{
		//cout<<"The person you are looking for does not exist"<<endl;
	}
	else
	{
		for (auto it : vertex_list)
		{

			for (auto ms : it.second->edge_list)//look throug edgelist and print vt
			{
				if (ms->target_vertex == d)
				{
					r.push_back(it.first);
				}
			}
		}
		for (auto s : vertex_list[d]->edge_list)
		{
			r.push_back(s->target_vertex);
		}
		return r;
	}
}
void Graph::printsfriends(vector<int>&vsl)//shows which person is mutual friend
{
	for (unsigned int i = 0; i < vsl.size(); i++)
	{
		cout << vsl[i] << " is a mutual friend " << endl;

	}
}
vector<int>Graph::mutual_friends(vector<int> &sr, vector<int> &sm)
{//function returns vector containing mutual friends
	vector<int>common_friends;
	sort(sr.begin(), sr.end());
	sort(sm.begin(), sm.end());
	set_intersection(sr.begin(), sr.end(), sm.begin(), sm.end(), back_inserter(common_friends));
	return common_friends;
}

vector<int>Graph::non_mutual(vector<int> &sr, vector<int> &sm)
{
	vector<int>uncommon_friends;
	sort(sr.begin(), sr.end());
	sort(sm.begin(), sm.end());
	set_difference(sr.begin(), sr.end(), sm.begin(), sm.end(), back_inserter(uncommon_friends));
	return uncommon_friends;
}


void Graph::printGraph()
{
	for (auto it : vertex_list)
	{
		cout << it.first << ": ";//transverse vector list print key value vs

		for (auto ms : it.second->edge_list)//look throug edgelist and print vt
		{
			cout << " " << ms->target_vertex << " ";
		}

		cout << endl;
	}
}

