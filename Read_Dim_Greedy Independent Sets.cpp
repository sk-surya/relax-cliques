// Read_Dim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <fstream>
#include <string>
#include <set>
#include <vector>

using namespace std;

int vertex_count;
int edge_count;

struct node
{
	int value = 0;
	struct node *next = NULL;
};
typedef struct node NODE;

struct LList
{
	NODE *head = new NODE();
	struct node *tail;
};
typedef struct LList LLIST;

//int v_count;
LLIST *Adj = NULL;

class Graph {

private:

	bool** adjacencyMatrix;
	int vertexCount;
	bool init;

public:

	Graph(int vertexCount, bool init) {

		this->vertexCount = vertexCount;
		this->init = init;

		adjacencyMatrix = new bool*[vertexCount];

		for (int i = 0; i < vertexCount; i++) {

			adjacencyMatrix[i] = new bool[vertexCount];

			for (int j = 0; j < vertexCount; j++)

				adjacencyMatrix[i][j] = init;

		}

	}



	void addEdge(int i, int j) {

		if (i >= 0 && i < vertexCount && j >= 0 && j < vertexCount) {

			adjacencyMatrix[i][j] = true;

			adjacencyMatrix[j][i] = true;

		}

	}



	void removeEdge(int i, int j) {

		if (i >= 0 && i < vertexCount && j >= 0 && j < vertexCount) {

			adjacencyMatrix[i][j] = false;

			adjacencyMatrix[j][i] = false;

		}

	}



	bool isEdge(int i, int j) {

		return adjacencyMatrix[i][j];

	}

	void printG(void) {
		for (int i = 0; i < vertexCount; i++) {
			for (int j = 0; j < vertexCount; j++) {
				cout << adjacencyMatrix[i][j] << " ";
			}
			cout << endl;
		}
	}


	~Graph() {

		for (int i = 0; i < vertexCount; i++)

			delete[] adjacencyMatrix[i];

		delete[] adjacencyMatrix;

	}

};

void greedy_RMP(LLIST *Adj, const int& vertex_count, Graph& G);
void remove_neighbours(set<int>& mset, int i);
int get_min_degree_vertex(set<int>& mset, Graph& G);
void Check_Maximality(vector<set<int>>& mxl_set, Graph& G);
void Check_Maximality2(vector<set<int>>& mxl_set, Graph& G);

void addEdge(const int& v1, const int& v2)
{
	NODE *ptr = new NODE();
	ptr->value = v2;

	if (Adj[v1 - 1].head->value == 0) //value 0 means no neighbours yet
	{
		Adj[v1 - 1].head = ptr;
		Adj[v1 - 1].tail = Adj[v1 - 1].head;
	}
	else
	{
		Adj[v1 - 1].tail->next = ptr;
		Adj[v1 - 1].tail = ptr;
	}
}
//int v_count;
void printAdj(int v_count)
{
	//if zero is printed, it means no neighbours
	for (int i = 0; i < v_count; i++)
	{
		cout << "Adjacency list of " << i + 1 << " :" << endl;
		NODE *ptr = Adj[i].head;

		bool flag = 1;
		while (flag)
		{
			cout << ptr->value;
			if (ptr->next == NULL)
				flag = 0;
			else
			{
				ptr = ptr->next;
				cout << ",";
			}
		}
		cout << endl;
	}
}

struct Graph_L {
	int *V;
	LLIST *Nei;
};

set<int> all_V;
void greedy_RMP(LLIST *Adj, const int& vertex_count);
int main(void) 
{
	fstream file;
	string filepath, filename, name, keyword, word, a, b, e;
	int node_1, node_2;

	filepath = "F:\\Research\\Problem sets\\Max Clique\\";
	name = "C125.9.clq.txt";
	filename = filepath + name;
	if (name[0] == 'D')
		keyword = "edge";
	else
		keyword = "col";
	file.open(filename.c_str());

	while (file >> word && word != keyword);
	file >> word;
	vertex_count = stoi(word);
	file >> word;
	edge_count = stoi(word);
	cout << "No. of nodes: " << vertex_count << endl;
	cout << "No. of edges: " << edge_count << endl;

	bool graph_inverted = false;
	Graph Gm(vertex_count, graph_inverted);
	//Graph_L G;

	int *Vertices = new int[vertex_count];
	Adj = new LLIST[vertex_count];

	for (int i = 1; i <= edge_count; i++)
	{
		file >> e >> a >> b;
		node_1 = stoi(a) - 1;
		node_2 = stoi(b) - 1;
		Gm.addEdge(node_1, node_2);
		addEdge(node_1 + 1, node_2 + 1);
		addEdge(node_2 + 1, node_1 + 1);

	}

	for (int i = 0; i < vertex_count; i++)
	{
		all_V.insert(i);
	}
	greedy_RMP(Adj, vertex_count, Gm);
	//printAdj(vertex_count);
	//Gm.printG();
	//cout << "ex: " << Adj[0].head->next->next->next->next->next->next->next->next->next->next->value;
}


void greedy_RMP(LLIST *Adj, const int& vertex_count, Graph& G)
{
	vector < set<int> > mxl_set(vertex_count);
	node *ptr;
	set<int> set_all;
	int x;

	for (int i = 0; i < vertex_count; i++)
	{
		//initialise set with all vertices
		set_all = all_V;

		//remove the vertex for which the set is defined (first residual graph)

		set_all.erase(i);
		mxl_set[i].insert(i);
		//remove all neighbours for the vertex for which the set is defined (trivial step)
		remove_neighbours(set_all, i);
		//get minimum degree vertex from the residual graph
		if (!set_all.empty())
			x = get_min_degree_vertex(set_all, G);
		while (!set_all.empty())
		{
			mxl_set[i].insert(x);
			set_all.erase(x);
			remove_neighbours(set_all, x);
			if (!set_all.empty())
				x = get_min_degree_vertex(set_all, G);
		}

		
		cout << endl;
		//int i = 0;
		cout << "Size of independent set containing vertex " << i + 1 << " : " << mxl_set[i].size() << endl;
		for (auto it : mxl_set[i])
			cout << it + 1 << "--";
		cout << endl;
		
	}
	Check_Maximality(mxl_set, G);
	Check_Maximality2(mxl_set, G);
}

void remove_neighbours(set<int>& mset, int i)
{
	node *ptr;
	ptr = Adj[i].head;
	while (ptr != NULL)
	{
		mset.erase(ptr->value-1);
		ptr = ptr->next;
	}
}

int get_min_degree_vertex(set<int>& mset, Graph& G)
{
	int deg = 0;
	int prev_deg = mset.size();;
	int x;

	for (auto it_1 : mset)
	{
		deg = 0;
		for (auto it_2 : mset)
		{
			deg += G.isEdge(it_1, it_2);
		}
		if (deg <= prev_deg)
		{
			x = it_1;
			prev_deg = deg;
		}
	}
	if (!(x > -1) && !(x < vertex_count))
		cout << "Uninitialised error";
	return x;
}	

void Check_Maximality(vector<set<int>>& mxl_set, Graph& G)
{
	cout << endl;
	cout << "vec size " << mxl_set.size() << endl;
	bool b = true;
	set<int> s_all, s1_all;

	for (int i = 0; i < mxl_set.size(); i++)
	{
		s_all = all_V;
		s1_all = s_all;
		for (auto it : mxl_set[i])
		{
			for (auto it2 : s_all)
			{
				if (G.isEdge(it, it2))
					s1_all.erase(it2);
			}
			s_all = s1_all;
		}
		if (s_all!=mxl_set[i])
		{
			b = false;
			break;
		}
	}

	cout << "Maximality check: " << b;
}

void Check_Maximality2(vector<set<int>>& mxl_set, Graph& G)
{
	cout << endl;
	cout << "vec size " << mxl_set.size() << endl;
	bool b = false;
	set<int> s_all, s1_all;

	for (int i = 0; i < mxl_set.size(); i++)
	{
		s_all = all_V;
		s1_all = s_all;

		for (auto it : mxl_set[i])
		{
			s_all.erase(it);
		}

		for (auto it : s_all)
		{
			b = false;
			//cout << b;
			for (auto it2 : mxl_set[i])
			{
				b = b || G.isEdge(it, it2);
			}
			//cout << b;
			//cout << endl;
			if (b == true)
				break;
		}
	}

	cout << "Maximality check 2: " << b;
}