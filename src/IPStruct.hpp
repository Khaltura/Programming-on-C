#pragma once
#include <vector>
#include <memory>

class IPStruct
{
    private:
        std::unique_ptr<short[]> _IP;
    public:
        IPStruct();
        IPStruct(IPStruct&&); 
        IPStruct& operator=(IPStruct&&);
        IPStruct(const IPStruct&);
        IPStruct& operator = (const IPStruct&); 
        short& operator[](unsigned short)const;
        bool operator == (const IPStruct&) const;
};

bool operator<(const IPStruct& lIP, const IPStruct& rIP); 

std::ostream& operator<<(std::ostream& os, const IPStruct& IP);
