//below are headers for work around function that converts strings to integers
#include "Graph_file.h"
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


void Graph::addEdge(int vertex_source, int vertex_target)
{
	//look for the vertex source
	auto holder_of_vertext_source = vertex_list.find(vertex_source);
	//if the vertex source is in the hash table
	if (holder_of_vertext_source != vertex_list.end())
	{
		//look for the vertex target in the hashtable
		holder_of_vertext_source = vertex_list.find(vertex_target);
		Data *nData = new Data();
		nData->visited = false;
		Edge *nEdge = new Edge();
		nEdge->weight = 0;
		nEdge->target_vertex = vertex_target;
		vertex_list[vertex_source]->edge_list.push_back(nEdge);
		//see if target is in the hashtable
		if (holder_of_vertext_source == vertex_list.end())
		{
			Vertex *target = new Vertex();
			target->vertex_id = vertex_target;
			Data *tarData = new Data();
			tarData->visited = false;
			target->data = tarData;
			vertex_list[vertex_target] = target;
		}
	}
	else
	{ //if the start point (source is not in the table
		//use the iterator to find the target
		holder_of_vertext_source = vertex_list.find(vertex_target);
		Data *ndata = new Data();
		ndata->visited = false;
		Edge *nEdge = new Edge();
		nEdge->target_vertex = vertex_target;
		nEdge->weight = 0;
		Vertex *nVert = new Vertex();
		nVert->edge_list.push_back(nEdge);
		vertex_list[vertex_source] = nVert;
		//look for the target in the table
		if (holder_of_vertext_source == vertex_list.end())
		{
			Vertex*target = new Vertex();
			target->vertex_id = vertex_target;
			Data *tData = new Data();
			tData->visited = false;
			target->data = tData;
			vertex_list[vertex_target] = target;
		}
	}
}
Graph::Graph(bool insertReverseEdge, ifstream& ifs)
{
	//beginning of edge point(hold vertex source (vs)
	int starts;
	//ending of edge point (hold vertex target(vt)
	int ends;
	//check for spaces in text file to distinguish the start and stop
	string checker = " "; 

	for (string line_in_file; getline(ifs, line_in_file);)
	{//read text file and store the begin and end points of of the edges
		starts = work_around::stoi(line_in_file.substr(0, line_in_file.find(checker)));//start value (vs)
		ends = work_around::stoi(line_in_file.substr(line_in_file.find(checker) + 1, line_in_file.size()));//finish value (vt)
		addEdge(starts, ends);//add the edges
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



void Graph::friends(int friends_list)//print the contests of the edge list for vs
{
	auto startFriend = std::chrono::high_resolution_clock::now();
	vector<int>friend_list_vector;//friend list
	vector<int>mutual_friend_recommendation;
	map<int, int>map_of_mutual_friends;//holds friend index and number of mutual friends
	auto person_existance = vertex_list.find(friends_list);//if the person you are looking for does not exist

	if (person_existance == vertex_list.end())
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
			for (auto edge_iterator : it.second->edge_list)//look through edgelist and print end edge points
			{
				if (edge_iterator->target_vertex == friends_list)
				{
					friend_list_vector.push_back(it.first);//add friend to list (end point ,but no start point)
				}
			}
		}
		for (auto s : vertex_list[friends_list]->edge_list)//add friends from edge list to friend list
		{
			friend_list_vector.push_back(s->target_vertex);
		}

		
		for (unsigned int i = 0; i < friend_list_vector.size(); i++)//while looking through friend list
		{
			vector<int> friends_of_friends_container(iterater_friends_of_friends(friend_list_vector[i]));//go through friends of friends list
			vector<int>find_number_of_mutual_friends(mutual_friends(friend_list_vector, friends_of_friends_container));//find number of mutual friends
			map_of_mutual_friends.emplace(make_pair(find_number_of_mutual_friends.size(), friend_list_vector[i]));//make pair of friend with number of mutual friends
		}


		auto finishedFriend = std::chrono::high_resolution_clock::now();
		chrono::duration<double> elapsedFriend = finishedFriend - startFriend;

		//  cout << "time taken to execute User Info: " << elapsedFriend.count()*1000 << "ms" << endl;
		cout << "---------------" << endl;
		cout << "This person has:" << endl;
		cout << " - " << friend_list_vector.size() << " Friends." << endl;
		cout << " - " << map_of_mutual_friends.size() << " Close Friends." << endl;
		cout << "   time taken:" << elapsedFriend.count() * 1000 << "ms" << endl;
		cout << "---------------" << endl;

		unsigned int topN; //Holds the "n-top" number of friends the user wants to view.
		cout << "How many close friends do you want to see?: " << endl;
		cin >> topN;
		cout << "---Creating list of Top Friends:---";

		//TIMING "TOP N FRIENDS"

		auto startTop = std::chrono::high_resolution_clock::now();

		if (topN > map_of_mutual_friends.size())
		{
			cout << "\n--------------------\n";
			cout << "The user only has " << map_of_mutual_friends.size() << " top friends. Listing top " << map_of_mutual_friends.size() << " friends." << endl;
			cout << "--------------------\n";
			topN = map_of_mutual_friends.size();

		}
		unsigned int num_list = 0;
		std::cout << "Top " << topN << " friends of user: " << endl;

		std::multimap<int, int>::reverse_iterator rank_best_friends;//rank closest friends
		for (rank_best_friends = map_of_mutual_friends.rbegin(); rank_best_friends != map_of_mutual_friends.rend(); ++rank_best_friends)
		{//prints closest friends in descending order
			if (num_list < topN) {
				std::cout << "  #" << num_list + 1 << " - " << rank_best_friends->second << " with " << rank_best_friends->first << " mutual friends" << endl;
				num_list++;
			}
			

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
		for (rank_best_friends = map_of_mutual_friends.rbegin(); rank_best_friends != map_of_mutual_friends.rend(); ++rank_best_friends)
		{
			mutual_friend_recommendation.push_back(rank_best_friends->second);
		}

		vector<int> recommendThese;

		int random_num = (rand() % mutual_friend_recommendation.size()); // generate random number to pick random mutual friend from friend list.

		int random_friend = mutual_friend_recommendation[random_num]; //The actual user which got randomly selected

		//cout << "\nChose: " << random_friend << " as user to get suggestions from" << endl;

		vector<int>particularFriendList;

		for (auto particular_friend_iterator : vertex_list[random_friend]->edge_list)
		{
			particularFriendList.push_back(particular_friend_iterator->target_vertex);
		}

		//cout << "THIS SIZE: " << particularFriendList.size() << endl;

		recommendThese  = non_mutual(friend_list_vector, particularFriendList); //find non-mutual friends between the current person and their randomly selected friend
		
//TIMING END "FRIEND RECOMMENDATION LIST"
		auto finishedRecom = std::chrono::high_resolution_clock::now();
		chrono::duration<double> elapsedRecom = finishedRecom - startRecom;
		//cout << "Time taken to execute Find Suggestions Command: " << elapsedRecom.count()*1000 << "ms" << endl << endl;
		cout << elapsedRecom.count() * 1000 << "ms" << endl;
		cout << "-------------------------" << endl;
		//cout << "Size of difference" << recommendThese.size() << endl;
		cout << "User has " << recommendThese.size() << " friend suggestions. How many suggestions would you like to list? " << endl;

		unsigned int number_of_suggestions;
		cin >> number_of_suggestions;
		if (number_of_suggestions > recommendThese.size())
		{
			number_of_suggestions = recommendThese.size();
			cout << "Only " << recommendThese.size() << " are available." << endl;
			cout << "Printing " << recommendThese.size() << " recommendations." << endl;
		}
		cout << "----------Suggestions----------" << endl;
		for (unsigned int i = 0; i < number_of_suggestions; i++)
		{
			cout << "Recommendation #" << i + 1 << endl;
			cout << recommendThese[i] << endl;

		}
		cout << "---End of Suggestions---" << endl;
	}



}

vector<int>Graph::iterater_friends_of_friends(int friend_to_look_for)//function to help look through and create friends of friends list
{
	vector<int>friend_of_friend_vector;
	auto find_the_friend = vertex_list.find(friend_to_look_for);
	
	if (find_the_friend == vertex_list.end())
	{
		//cout<<"The person you are looking for does not exist"<<endl;
	}
	else
	{
		for (auto it : vertex_list)
		{

			for (auto ms : it.second->edge_list)//look throug edge list and print vt
			{
				if (ms->target_vertex == friend_to_look_for)
				{
					friend_of_friend_vector.push_back(it.first);
				}
			}
		}
		for (auto s : vertex_list[friend_to_look_for]->edge_list)
		{
			friend_of_friend_vector.push_back(s->target_vertex);
		}
		return friend_of_friend_vector;
	}
}

void Graph::print_friend(vector<int>&mutual_friend_results)//shows which person is mutual friend

{
	
	for (unsigned int i = 0; i < mutual_friend_results.size(); i++)
	{
		cout << mutual_friend_results[i] << " is a mutual friend " << endl;

	}
}
vector<int>Graph::mutual_friends(vector<int> &friend1, vector<int> &friend2)
{//function returns vector containing mutual friends
	vector<int>common_friends;
	std::sort(std::execution::par_unseq,friend1.begin(), friend1.end());
	std::sort(std::execution::par_unseq,friend2.begin(), friend2.end());
	std::set_intersection(friend1.begin(), friend1.end(), friend2.begin(), friend2.end(), back_inserter(common_friends));
	return common_friends;
}

vector<int>Graph::non_mutual(vector<int> &friend1, vector<int> &friend2)
{
	vector<int>uncommon_friends;
	std::sort(std::execution::par_unseq, friend1.begin(), friend1.end());
	std::sort(std::execution::par_unseq, friend2.begin(), friend2.end());
	set_difference(friend1.begin(), friend1.end(), friend2.begin(), friend2.end(), back_inserter(uncommon_friends));
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

