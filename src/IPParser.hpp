#pragma once
#include <string>
#include <vector>
#include <fstream>

class IPParser
{
    public:
        template <typename IPStruct>
        static void parseFile(const std::string& filePath, std::vector<IPStruct>& IPBuffer);

};

template <typename IPStruct>
void IPParser::parseFile(const std::string& filePath, std::vector<IPStruct>& IPBuffer)
{
    std::vector<IPStruct> tempBuffer;
    std::ifstream IPFile(filePath); 
    std::string line;
    std::string IPNum;
    int i = 0;
    if(!IPFile.is_open()) 
        throw std::runtime_error("Couldn't open this file");
    while(std::getline(IPFile, line))
    {
        IPStruct ip; 
        IPNum = ""; 
        for(const auto& c: line)
        {
            if(std::isdigit(c))
                IPNum += c; 
            else if( c == '.')
            {
                ip[i] = std::stoi(IPNum);
                IPNum = "";
                i++;
            }
            else
            {
                ip[i] = std::stoi(IPNum);
                break;
            }
        }
        i = 0;
        tempBuffer.push_back(std::move(ip));
    }
    IPBuffer = std::move(tempBuffer);
}





