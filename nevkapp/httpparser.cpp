#include "httpparser.h"

HTTPParser::HTTPParser(QStringList list)
{
    MapInstructions(list);
}

HTTPParser::value_t HTTPParser::operator[](key_t index)
{
    value_t val = "";
    auto it = map.find(index);
    if(it != map.end())
    {
        val = it.value();
    }
    return val;
}

void HTTPParser::MapInstructions(QStringList &list)
{
    for(auto i:list)
    {
        if(i.startsWith("GET"))
            map["GET"]=i.split(" ")[1];
        else if(i.startsWith("REQUEST"))
            map["REQUEST"]=i.split(" ")[1];
        else if(i!="")
        {
            auto s = i.split(":");//split
            if(s.length()<2)continue;
            key_t name = s[0];
            value_t value = s[1];
            value.remove(0,1);
            map[name] = value;
        }
    }
}
