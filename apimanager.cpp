#include "apimanager.h"

ApiManager::ApiManager()
{
    this->manager = new QNetworkAccessManager();

    connect(manager, &QNetworkAccessManager::finished, this, &ApiManager::ManagerFinished);
}

void ApiManager::GetBrands(const QString vehicleType)
{
    this->request.setUrl(QUrl("https://fipe.parallelum.com.br/api/v2/"+vehicleType+"/brands"));
    this->manager->get(this->request);
}

QString ApiManager::GetAnswer()
{
    return this->answer;
}

void ApiManager::ManagerFinished(QNetworkReply *reply)
{
    if (reply->error())
    {
        qDebug() << reply->errorString();
        return;
    }

    this->answer = reply->readAll();
    qDebug() << answer;
}
