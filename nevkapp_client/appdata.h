#ifndef APPDATA_H
#define APPDATA_H

#include <QString>
#include <QCoreApplication>

class AppData
{
public:
    AppData(const AppData&) = delete;

    static AppData& Get()
    {
        return appdata_inst;
    }

    static bool& Mode()
    {
        return Get().ModeI();
    }
    static QString SavedPagesPath(bool relative = true)
    {
        QString addon = "";
        if(relative)addon = QCoreApplication::applicationDirPath();
        return addon + Get().SavedPagesPathI();
    }
private:
    /*I-functions*/
    bool& ModeI(){return mode;}
    QString SavedPagesPathI(){return pagespath;}
    /*Instances*/
    AppData(){}
    static AppData appdata_inst;
    /*variables*/
    bool mode=true;
    QString pagespath = "/saved/";
};

#endif // APPDATA_H
