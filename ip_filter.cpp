#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
//#include <algorithm>
#include <tuple>

#include <chrono>

#include <range/v3/action/sort.hpp>
//#include <range/v3/view/filter.hpp>
#include <range/v3/algorithm/for_each.hpp>

using mainVector = std::vector<std::vector<std::string> >;
using sortVector = std::vector<std::vector<int> >;

enum octet {
    one = 0,
    two = 1,
    three = 2,
    four = 3,
    any = 4
};
using Condition = std::pair<int,octet>;


// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;         

    std::string::size_type start = 0;   
    std::string::size_type stop = str.find_first_of(d);     
    while(stop != std::string::npos)    
    {
        r.push_back(str.substr(start, stop - start)); 

        start = stop + 1;                   
        stop = str.find_first_of(d, start);     
    }

    r.push_back(str.substr(start));

    return r;
}

// Print function
void printVector (const sortVector* sort_ip)
{
    for(const auto& ip : *sort_ip) 
        {
            for(auto ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part)
            {
                if (ip_part != ip.cbegin())
                {
                    std::cout << ".";
                }
                std::cout << *ip_part;
            }
            std::cout << std::endl;
        }
}

// Reverse lexicographically sort
auto reverseSort(const mainVector& ip_pool)
{
    sortVector* ip = new sortVector;
    for (auto& i : ip_pool)
    {
        std::vector<int> temp;
        for (auto& z : i)
        {
            temp.push_back(atoi(z.c_str()));
        }
        ip->push_back(temp);
    }

    ranges::action::sort(*ip,[](const auto& l,const auto& r){
    return std::tie(l[0],l[1],l[2],l[3]) > std::tie(r[0],r[1],r[2],r[3]);
     });

    return ip;
}


// Search by condition and print
template<typename... Args>
void findAndPrint(const sortVector* sort_ip,Args&&... args)
{
    printVector(sort_ip);
}

template<typename T,typename... Args>
void findAndPrint(const sortVector* sort_ip,T&& t,Args&&... args)
{
    auto tmp = new sortVector();
    if(t.second == any){
        ranges::for_each(*sort_ip, [&tmp,&t](auto z){
        if(z[0]==t.first || z[1]==t.first || z[2]==t.first || z[3]==t.first) tmp->push_back(z);       
        }); 
    }
    else{
        ranges::for_each(*sort_ip, [&tmp,&t](auto z){
        if(z[t.second]==t.first) tmp->push_back(z);       
        });
    }

    findAndPrint(tmp,args...);
    delete tmp;
}


int main()
{
    // auto start_time = std::chrono::steady_clock::now();
    try
    {
        mainVector ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            auto v = split(line, '\t');       
            ip_pool.push_back(split(v.at(0), '.'));
        }

        // TODO reverse lexicographically sort
        auto sort_ip = reverseSort(ip_pool);
        // Print result
        printVector(sort_ip);


        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8
        // TODO filter by first byte and output
        // ip = filter(1)
        findAndPrint(sort_ip,Condition{1,octet::one});


        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)
        findAndPrint(sort_ip,Condition{46,octet::one},Condition{70,octet::two});

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)
        findAndPrint(sort_ip,Condition{46,octet::any});

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
        
        delete sort_ip;
        
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    // auto end_time = std::chrono::steady_clock::now();
    // auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    // std::cout << "Time chrono: " << elapsed_ns.count() << std::endl;

    return 0;
}
