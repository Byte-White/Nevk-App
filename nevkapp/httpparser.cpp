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
        {
            auto s = i.split(" ");
            map["GET"]=s[1];
            if(s.length()==3)
            map["VERSION"] = s[2];
        }
        else if(i.startsWith("REQUEST"))
        {
            auto s = i.split(" ");
            if(s.length()==3)
            {
                map["REQUEST"]=s[1];
                map["VERSION"]=s[2];
            }
        }
        else if(i!="")
        {
            auto s = i.split(":");//split
            if(s.length()<2)continue;
            key_t name = s[0];
            value_t value = s[1];
            if(value[0]==(QString)" ")
            value.remove(0,1);
            map[name] = value;
        }
    }
}
