#ifndef APIMANAGER_H
#define APIMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>

class ApiManager
{
public:
    ApiManager();

private:
    QNetworkAccessManager manager;
};

#endif // APIMANAGER_H
