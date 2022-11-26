#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <future>

using namespace std;

template<typename Iterator, typename T>
Iterator parallel_min_element(Iterator first, Iterator last, T init)
{
	unsigned long const length = distance(first, last);
	unsigned long const max_chunk_size = 25;
    if (length <= max_chunk_size)
	{
        return min_element(first,last);
	}
    else
    {
		Iterator mid_point = first;
		advance(mid_point, length / 2);
		future<Iterator> first_half_res_future = async(parallel_min_element<Iterator, T>, first, mid_point, init);
		Iterator second_half_res = parallel_min_element(mid_point, last, init);
        Iterator first_half_res = first_half_res_future.get();
        if (*(first_half_res) < *(second_half_res)) return first_half_res;
        else return second_half_res;
	}
}


void parsssort(vector<int> &vec)
{
    int length = vec.size();
    auto it = vec.begin();
    while (it != vec.end())
    {
        auto min = parallel_min_element(it, vec.end(), 0);
        int index_min = distance(vec.begin(), min);
        int index_it = distance(vec.begin(), it);
        if (index_min > index_it)
        {
            int tmp = vec[index_it];
            vec[index_it] = vec[index_min];
            vec[index_min] = tmp;
        }
        it++;
    }
}

int main()
{
    vector<int> vec(100);

    mt19937 gen;
    uniform_int_distribution<int> dis(10, 100);
    auto rand_num([=]() mutable { return dis(gen); });
    generate(begin(vec), end(vec), rand_num);

    parsssort(vec);
    for (auto &n : vec)
    {
        cout << n << " ";
    }
    cout << endl;

    return 0;
}