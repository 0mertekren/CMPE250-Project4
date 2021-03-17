/*
 * */
#include <iostream>
#include <vector>
#include <cstdint>
#include <stdlib.h>
#include <fstream>
#include <cstdint>
#include <queue>
#include <stack>
#include <list>
#include <chrono>
#include <bits/stdc++.h>


using namespace std;
using namespace std::chrono;

/*Prototypes of Functions*/
bool spfa_early_terminate();
bool detect_cycle();
void input_parse();
bool update_residual_graph(long int reversing_vertex);
//Global

ofstream out_file;
ifstream file;

long int num_of_test_cases;
long int num_of_cables;
long int num_of_vertices;
long int start_vertex;
long int end_vertex;
long int current_vertex;

vector<queue<long int>> edges_queues;
vector<std::pair<long int, long int>> *adj;

long int *dis;
long int *pre;
bool *in_queue;
long int max_flow=0;
bool *visited;
bool *on_stack;

int main(int argc, char const *argv[])
{
	//auto start = chrono::high_resolution_clock::now();
	ofstream out_file;
	file.open(argv[1]);
	out_file.open(argv[2]);
	int i,j;
	if (file.is_open())
	{
		file >> num_of_test_cases;

		for(i=0; i<num_of_test_cases; i++)
		{
			input_parse();
			while(1)
			{
				if(spfa_early_terminate() == true)
				{
					if(update_residual_graph(current_vertex) == false)
						break;

				}
				else
				{
					if(update_residual_graph(end_vertex) == false)
						break;
				}
			}
			max_flow=0;
			for(j=1; j<= num_of_cables; j++)
			{
				max_flow += adj[num_of_cables+j][0].second;
			}
			out_file << max_flow << endl;
		}
	}
		//auto stop = high_resolution_clock::now();
		//auto duration = duration_cast<seconds>(stop - start);
		//cout << duration.count() << endl;
		delete dis;
		delete pre;
		delete in_queue;

		delete visited;
		delete on_stack;
	return 0;
}

/*
 * Here, the adj matrix is updated with residual graph
 */
bool update_residual_graph(long int reversing_vertex)
{
	if(pre[reversing_vertex] == -1) //no path found
		return false;

	long int latter_vertex = reversing_vertex;
	long int preceding_vertex = pre[latter_vertex];

	while(preceding_vertex != -1)
	{
		vector<pair<long int, long int>>::iterator it;
		for(it = adj[preceding_vertex].begin(); it != adj[preceding_vertex].end(); it++)
		{
			long int dist = (*it).second;
			long int adj_vertex = (*it).first;
			if(adj_vertex == latter_vertex)
			{
				adj[latter_vertex].emplace_back(preceding_vertex, -(dist));
				adj[preceding_vertex].erase(it);
				long int temp_vertex = preceding_vertex;
				preceding_vertex = pre[temp_vertex];
				latter_vertex = temp_vertex;
				break;
			}
		}
	}
	return true;

}

void input_parse()
{

	long int vertex, out_degree, successor;
	uint8_t* evenness;
	long int temp;

	file >> num_of_cables;
	num_of_vertices = num_of_cables*2 + 2;
	start_vertex = 0;
	end_vertex = num_of_vertices-1;
	adj = new std::vector<std::pair<long int, long int>>[num_of_vertices];
	dis = new long int[num_of_vertices];
	pre = new long int[num_of_vertices];
	in_queue = new bool[num_of_vertices];
	visited = new bool[num_of_vertices];
	on_stack = new bool[num_of_vertices];

	for (long int i = 1; i <= num_of_cables; i++)
	{
		adj[0].emplace_back(i, 0);
		adj[i+num_of_cables].emplace_back(num_of_vertices-1, 0);
		for(long int j = 1; j<= num_of_cables; j++)
		{
			file >> temp;
			adj[i].emplace_back(j + num_of_cables, -(temp));
		}
	}

	return;
}
/**
 * Below code is taken and slightly modified from KonaeAkira's blog
 * Function reference for spfa early terminate and detect cycle :
 * https://konaeakira.github.io/posts/using-the-shortest-path-faster-algorithm-to-find-negative-cycles.html
 */
bool spfa_early_terminate()
{
	std::fill(dis, dis + num_of_vertices, LONG_MAX);
	dis[0] = 0;
	std::fill(pre, pre + num_of_vertices, -1);
	std::fill(in_queue, in_queue + num_of_vertices, true);
	std::queue<int> queue;
	for (long int i = 0; i < num_of_vertices; ++i)
		queue.push(i);


	int iter = 0;
	while (!queue.empty())
	{
		int u = queue.front();
		queue.pop();
		in_queue[u] = false;
		for (auto [v, w] : adj[u])
			if (dis[u] + w < dis[v])
			{
				current_vertex = u;
				pre[v] = u;
				dis[v] = dis[u] + w;
				if (++iter == num_of_vertices)
				{
					iter = 0;
					if (detect_cycle())
					{
						//handle_negative_cycle();
						return true;
					}
				}
				if (!in_queue[v])
				{
					queue.push(v);
					in_queue[v] = true;
				}
			}
	}
	if (detect_cycle())
	{
		//handle_negative_cycle();
		return true;
	}
	return false;
}

bool detect_cycle()
{
	std::vector<int> vec;
	std::fill(on_stack, on_stack + num_of_vertices, false);
	for (int i = 0; i < num_of_vertices; ++i)
		if (!visited[i])
		{
			for (int j = i; j != -1; j = pre[j])
				if (!visited[j])
				{
					visited[j] = true;
					vec.push_back(j);
					on_stack[j] = true;
				}
				else
				{
					if (on_stack[j])
						return true;
					break;
				}
			for (int j : vec)
				on_stack[j] = false;
			vec.clear();
		}
	return false;
}
