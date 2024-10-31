#include <iostream> 
#include <algorithm>
#include "IPParser.hpp"
#include "IPStruct.hpp"

int main(int, char**)
{
    std::vector<IPStruct> IPList; 
    IPParser::parseFile("C:/Programming/Programming2/Programming_on_C--Lab2/src/ip_filter.tsv", IPList); 
    std::sort(IPList.rbegin(), IPList.rend());

    auto first_list = [](const IPStruct& ip){std::cout << ip << "\n";};
    auto second_list = [](const IPStruct& ip){if(ip[0] == 1) std::cout << ip << "\n";};
    auto third_list = [](const IPStruct& ip){if(ip[0] == 46 && ip[1] == 70) std::cout << ip << "\n";};
    auto fourth_list = [](const IPStruct& ip){if(ip[0] == 46 || ip[1] == 46 || ip[2] == 46 || ip[3] == 46) std::cout << ip << "\n";};
    std::for_each(IPList.begin(), IPList.end(), first_list);
    std::for_each(IPList.begin(), IPList.end(), second_list);
    std::for_each(IPList.begin(), IPList.end(), third_list);
    std::for_each(IPList.begin(), IPList.end(), fourth_list);
}




