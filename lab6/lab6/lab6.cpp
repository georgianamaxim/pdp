// lab6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <vector>
#include <chrono>
#include <cstddef>
#include <functional>
#include <condition_variable>
#include <thread>
#include <list>
#include <ctime>
#include <atomic>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <memory>

class Graph {
private:
	int n;
	std::unordered_map<int, std::unique_ptr<std::unordered_set<int>>> edges;
public:
	Graph(int n) {
		this->n = n;
		for (int i = 0; i < n; i++) {
			edges[i] = std::make_unique<std::unordered_set<int>>();
		}
	}

	void add_edge(int start, int end) {
		edges[start]->insert(end);
	}

	std::unordered_set<int>* get_neighbours(int node) {
		return this->edges[node].get();
	}

	bool is_neighbour(int node, int neighbour) {
		return this->edges[node]->find(neighbour) != this->edges[node]->end();
	}

	void printGraph() {
		for (auto& pair : edges) {
			std::cout << pair.first << " [";
			for (auto& neigh : (*pair.second)) {
				std::cout << neigh << ' ';
			}
			std::cout << "]\n";
		}
		std::cout << '\n';
	}
};

class ThreadPool
{
public:
	explicit ThreadPool(size_t nrThreads): m_end(false), m_liveThreads(nrThreads)
	{
		m_threads.reserve(nrThreads);
		for (size_t i = 0; i < nrThreads; ++i)
		{
			m_threads.emplace_back([this]() { this->run(); });
		}
	}

	~ThreadPool()
	{
		close();
		for (std::thread &t : m_threads)
		{
			t.join();
		}
	}

	void close()
	{
		std::unique_lock<std::mutex> lck(m_mutex);
		m_end = true;
		m_cond.notify_all();
		while (m_liveThreads > 0)
		{
			m_condEnd.wait(lck);
		}
	}

	void enqueue(const std::function<void()> &func)
	{
		std::unique_lock<std::mutex> lck(m_mutex);
		m_queue.push_back(func);
		m_cond.notify_one();
	}

private:
	void run()
	{
		while (true)
		{
			std::function<void()> toExec;
			{
				std::unique_lock<std::mutex> lck(m_mutex);
				while (m_queue.empty() && !m_end)
				{
					m_cond.wait(lck);
				}
				if (m_queue.empty())
				{
					--m_liveThreads;
					if (0 == m_liveThreads)
					{
						m_condEnd.notify_all();
					}
					return;
				}
				toExec = std::move(m_queue.front());
				m_queue.pop_front();
			}
			toExec();
		}
	}

	std::mutex m_mutex;
	std::condition_variable m_cond;
	std::condition_variable m_condEnd;
	std::list<std::function<void()>> m_queue;
	bool m_end;
	size_t m_liveThreads;
	std::vector<std::thread> m_threads;
};

float generate_random() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX); 
}

std::shared_ptr<std::vector<std::vector<int>>> generate_permutations(int n) {
	auto result = std::make_shared<std::vector<std::vector<int>>>();
	std::vector<int> permutation;
	for (int i = 0; i < n; i++) {
		permutation[i] = i;
	}
	result->push_back(permutation);
	while (next_permutation(permutation.begin(), permutation.end())) {
		result->push_back(permutation);
	}
	std::cout << "PERM : " << result;
	return result;
}

std::shared_ptr<Graph> generate_graph() {
	auto graph = std::make_shared<Graph>(5);
	//ex of hamiltonian graph
// cycle will be :3 -> 1 -> 2 -> 4 -> 5 -> 3
	std::vector<int> aux;
	 // empty vector for node 0
	aux.push_back(2);
	graph->add_edge(1, 2); // [2] for node 1 
	aux = std::vector<int>(0);
	aux.push_back(4);
	graph->add_edge(2,4); // [4] for node 2 
	aux = std::vector<int>(0);
	aux.push_back(1);
	graph->add_edge(3,1); // [1] for node 3 
	aux = std::vector<int>(0);
	aux.push_back(5);
	graph->add_edge(4,5); // [5] for node 4
	aux = std::vector<int>(0);
	aux.push_back(3);
	graph->add_edge(4,3); // [3] for node 4


	////ex of non-hamiltonian graph
	//std::vector<int> aux;
	//graph.push_back(aux); // empty vector for node 0
	//aux.push_back(2);
	//graph.push_back(aux); // [2] for node 1 
	//aux = std::vector<int>(0);
	//aux.push_back(1);
	//graph.push_back(aux); // [1] for node 3 
	//aux = std::vector<int>(0);
	//aux.push_back(5);
	//graph.push_back(aux); // [5] for node 4
	return graph;
}

void show_path(const std::vector<int> &permutation) {
	std::cout << "hamiltonian path: ";
	for (auto& x : permutation) {
		std::cout << x << ' ';
	}
	std::cout << '\n';
}

void hamiltonian_task(
	const std::vector<int>& vertices,
	const std::shared_ptr<Graph>& graph,
	std::atomic<bool> &foundFlag
) {
	int n = vertices.size();
	for (int i = 0; i < n - 1; i++) {
		if (!graph->is_neighbour(vertices[i], vertices[i + 1])) {
			return;
		}
	}
	if (!graph->is_neighbour(vertices[n - 1], vertices[0])) {
		return;
	}

	std::cout << "hamiltonian path: ";
	for (auto& v : vertices) {
		std::cout << v << ' ';
	}
	std::cout << '\n';
	foundFlag = true;
}

void exec(std::shared_ptr<std::vector<std::vector<int>>> perms, std::shared_ptr<Graph> graph, int num_threads, std::atomic<bool> &foundFlag) {
	std::cout << "threads " << num_threads << ' ' << '\n';
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	auto thread_pool = std::make_shared<ThreadPool>(num_threads);
	for (int i = 0; i < perms->size(); i++) {
		if (foundFlag) break;
		thread_pool->enqueue([i, &perms, &graph, &foundFlag]() {
			hamiltonian_task((*perms)[i], graph, foundFlag);
		});
	}
	thread_pool->close();
}


int main()
{
	srand(0); 
	int n = 5;
	int threads = 2;
	auto vertices = generate_permutations(n);
	std::cout << "\n";
	std::cout << "nr of vertices " << vertices->size() << '\n';
	auto graph = generate_graph();

	graph->printGraph();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	std::atomic<bool> isHamiltonian(false);

	double time;
	auto start = std::chrono::system_clock::now();

	[&vertices, &graph, threads, &isHamiltonian]() {exec(vertices, graph, threads, isHamiltonian);};

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	time = elapsed_seconds.count();

	std::cout << "Hamiltonian " << (isHamiltonian ? " true " : " false ")
		<< "time " << time;
	return 0;
}

