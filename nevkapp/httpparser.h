#ifndef HTTPPARSER_H
#define HTTPPARSER_H
#include <QMap>
#include <QString>
#include <QStringList>

class HTTPParser
{
    using key_t = QString;
    using value_t = QString;
public:
    QMap<key_t,value_t> map;
public:
    HTTPParser(QStringList list);
    value_t operator[](key_t index);
private:
    void MapInstructions(QStringList& list);
};

#endif // HTTPPARSER_H
