#include <gtest/gtest.h>
#include "IPParser.hpp"
#include "IPStruct.hpp"

TEST(TestIPParser, invalid_path)
{
    std::vector<IPStruct> testIP;
    ASSERT_THROW(IPParser::parseFile("error", testIP), std::runtime_error);
}

TEST(TestIPParser, test_txt_1)
{
    std::vector<IPStruct> testIP;
    std::vector<IPStruct> rightIP;
    IPParser::parseFile("unittests/IP/test1.txt", testIP);
    IPStruct temp; 
    bool flag = true; 
    temp[0] = 123; 
    temp[1] = 123;
    temp[2] = 123; 
    temp[3] = 123;
    rightIP.push_back(temp); 
    temp[0] = 91; 
    temp[1] = 64;
    temp[2] = 189; 
    temp[3] = 196; 
    rightIP.push_back(temp);
    temp[0] = 93; 
    temp[1] = 179;
    temp[2] = 90; 
    temp[3] = 82; 
    rightIP.push_back(temp); 
    for(int i = 0; i < 3; ++i)
    {
        if((rightIP[i] == testIP[i])) flag = false;
    }
    ASSERT_TRUE(flag);
}


