#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <random>
#include <functional> 
#include <chrono>
#include <algorithm>
#include <deque>
using std::vector;
using std::cin;

std::random_device rd;
std::mt19937 gen(rd());

const long float alpha = 0.85;
const long float convergence = 0.00001;




int main()
{

	int N;
	std::cout << "How much node in the graph" << std::endl;
	std::cin >> N;
	auto start = std::chrono::high_resolution_clock::now();
	
	std::deque<std::deque<int> > in_degree(N, std::deque<int>());
	std::deque<std::deque<int> > out_degree(N, std::deque<int>());
	
	std::uniform_int_distribution<int> rand_Link(N - 1, 3 * N);
	std::function<int()> f = std::bind(rand_Link, std::ref(gen));

	std::uniform_int_distribution<int> rand_Node(0, N - 1);
	std::function<int()> f2 = std::bind(rand_Node, std::ref(gen));

	int M = f();
	int M;
	int i(0), j(0);
	cin >> M;
	while (i < M)
	{
		int a, b;
		a = f2();
		b = f2();
		a--; b--;
		in_degree.at(b).push_back(a);
		out_degree.at(a).push_back(b);
		i++;
	}

	std::map<int, bool> outgoing_link_null;
	for (int i(0); i < N; i++)
	{
		if (out_degree.at(i).size() == 0)
			outgoing_link_null.insert(std::pair<int, bool>(i, true));
	}

	long float coeff2 = (1 - alpha) / N;

	i = 0;
	std::deque<long float> PR(N, 1 / static_cast<long float> (N)); // The final PageRank
	std::deque<long float> buffer(N, 0); 
	long float res(0);
	bool stop = false;
	int nombre_iteration = 0;

	while (!stop)
	{
		nombre_iteration++;
		j = 0;
		long float sum2 = 0;
		long float sum3 = 0;
		auto end = outgoing_link_null.end();
		i = 0;
		std::for_each(PR.begin(), PR.end(), [&sum2, &end, &i, &outgoing_link_null, &sum3](const auto& n) {
			sum2 += n;
			if (outgoing_link_null.find(i) != end)
				sum3 += n;
			i++;
		});
		for (i = 0; i < N; i++)
		{
			res = 0;
			std::for_each(in_degree.at(i).begin(), in_degree.at(i).end(), [&res, &PR, &out_degree](const auto& x) {
				res += 0.85*(1 / static_cast<long float>(out_degree.at(x).size()))* PR.at(x);
			});

			res += (alpha / static_cast<long float>(N))*sum3;
			res += sum2*coeff2;
			buffer.at(i) = res;
		}

		i = 0;
		for (const auto& x : buffer)
		{
			if (std::abs(x - PR.at(i++)) > convergence)
			{
				PR = buffer;
				break;
			}
			if (i == N)
				stop = true;
		}
	}
	i = 0;
	auto fin = std::chrono::high_resolution_clock::now();
	std::cout << "Time in millisecond :" << std::chrono::duration_cast<std::chrono::milliseconds>(fin - start).count() << std::endl;
	std::cout << "Number of iterations :" << nombre_iteration << std::endl;
	for (const auto & x : buffer)
	{
		std::cout << "PageRank " << i++ << "  : " << x << std::endl;
	}
	return 0;
}

