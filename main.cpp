#include <functional>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <chrono>
#include <random>

std::set <std::string> makeRandomWords(std::size_t N, std::size_t length)
{
    std::uniform_int_distribution<> letter(97, 122);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::set <std::string> words;

    for (std::string str(length, '_'); words.size() <= N; words.insert(str))
        for (auto& c : str)
            c = letter(e);

    return words;
}

template < typename T >
void hash_combine(std::size_t& seed, const T& value) noexcept
{
    seed ^= std::hash < T >()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template < typename T >
void hash_value(std::size_t& seed, const T& value) noexcept
{
    hash_combine(seed, value);
}

template < typename T, typename ... Types >
void hash_value(std::size_t& seed, const T& value, const Types & ... args) noexcept
{
    hash_combine(seed, value);
    hash_value(seed, args...);
}

template < typename ... Types >
std::size_t hash_value(const Types & ... args) noexcept
{
    std::size_t seed = 0;
    hash_value(seed, args...);
    return seed;
}

int main()
{
    std::vector<std::size_t> v(1000, 0);
    std::set<std::string> vi = makeRandomWords(1000, 10);
    std::vector<std::string> v1;
    std::vector<double> v2(1000, 0);

    std::default_random_engine dre;

    for (auto i = 0; i < std::size(v); ++i)
    {
        v[i] = i;
    }

    std::shuffle(std::begin(v), std::end(v), dre);

    v1.reserve(1000);

    for (auto& c : vi)
    {
        v1.push_back(c);
    }

    std::uniform_real_distribution<double> myrandom(0, 100);

    for (auto i = 0; i < std::size(v2); ++i)
    {
        v2[i] = myrandom(dre);
    }

    std::vector<std::size_t> hash_code(1000, 0);

    for (auto i = 0; i < std::size(hash_code); ++i)
    {
        hash_code[i] = (hash_value<std::string, std::size_t, double>(v1[i], v[i], v2[i]) % 64);
    }

    std::ofstream fout("3.3_4sem.csv");
    fout << "element, collision" << std::endl;
    std::set<std::size_t> set;
    std::size_t i;
    std::size_t num_of_collisions;

    for (auto N = 10; N < std::size(hash_code); N += 100)
    {
        for (auto i = 0; i < N; ++i)
        {
            set.insert(hash_code[i]);
        }
        num_of_collisions = N - std::size(set);
        fout << N << "," << num_of_collisions << std::endl;
    }

    return 0;
}