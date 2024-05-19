#include "apimanager.h"

ApiManager::ApiManager()
{
    this->manager = new QNetworkAccessManager(this);

    //connect(this->manager, &QNetworkAccessManager::finished, this, &ApiManager::ManagerFinished);

    // QNetworkRequest req{QUrl(QString("https://api.github.com/users/%1").arg("walter-agostinho"))};
    // QNetworkReply *reply = this->manager->get(req);
    //QNetworkReply *reply = manager->get(QNetworkRequest(QUrl("http://qt-project.org")));

    // connect(reply, &QIODevice::readyRead, this, &ApiManager::slotReadyRead);
    // connect(reply, &QNetworkReply::finished, this, &ApiManager::finishReading);
    // connect(reply, &QNetworkReply::errorOccurred,
    //         this, &ApiManager::slotError);
    // connect(reply, &QNetworkReply::sslErrors,
    //         this, &ApiManager::slotSslErrors);
}

void ApiManager::GetBrands(const QString &vehicleType, const QString &monthReference, ResponseCallback callback)
{
    QUrl url = QUrl("https://fipe.parallelum.com.br/api/v2/"+vehicleType+"/brands");

    this->SetMonthReferenceParameter(monthReference, url);

    this->request.setUrl(url);
    this->request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    this->request.setRawHeader("X-Subscription-Token", Token);

    QNetworkReply *reply = this->manager->get(this->request);

    connect(reply, &QNetworkReply::finished, this, [reply, callback, this]{
        this->SetJsonCallback(reply, callback);
    });
}

void ApiManager::GetModels(const QString &vehicleType, const QString &brandId, const QString &monthReference,
                           ResponseCallback callback)
{
    QUrl url = QUrl("https://fipe.parallelum.com.br/api/v2/"+vehicleType+"/brands/"+brandId+"/models");

    this->SetMonthReferenceParameter(monthReference, url);

    this->request.setUrl(url);
    this->request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    this->request.setRawHeader("X-Subscription-Token", Token);

    QNetworkReply *reply = this->manager->get(this->request);

    connect(reply, &QNetworkReply::finished, this, [reply, callback, this]{
        this->SetJsonCallback(reply, callback);
    });
}

void ApiManager::GetYearsByModel(const QString &vehicleType, const QString &brandId, const QString &modelId,
                                 const QString &monthReference, ResponseCallback callback)
{
    QUrl url = QUrl("https://fipe.parallelum.com.br/api/v2/"+vehicleType+"/brands/"+brandId+"/models/"
                    +modelId+"/years");

    this->SetMonthReferenceParameter(monthReference, url);

    this->request.setUrl(url);
    this->request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    this->request.setRawHeader("X-Subscription-Token", Token);

    QNetworkReply *reply = this->manager->get(this->request);

    connect(reply, &QNetworkReply::finished, this, [reply, callback, this]{
        this->SetJsonCallback(reply, callback);
    });
}

void ApiManager::GetFipeInfo(const QString &vehicleType, const QString &brandId, const QString &modelId,
                             const QString &monthReference, const QString &yearId, ResponseCallback callback)
{
    QUrl url = QUrl("https://fipe.parallelum.com.br/api/v2/"+vehicleType+"/brands/"+brandId+"/models/"
                    +modelId+"/years/"+yearId);

    this->SetMonthReferenceParameter(monthReference, url);

    this->request.setUrl(url);
    this->request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    this->request.setRawHeader("X-Subscription-Token", Token);
    qDebug() << "REQUEST = " << url;
    QNetworkReply *reply = this->manager->get(this->request);

    connect(reply, &QNetworkReply::finished, this, [reply, callback, this]{
        this->SetJsonCallback(reply, callback);
    });
}

void ApiManager::GetMonthReferences(ResponseCallback callback)
{
    this->request.setUrl(QUrl("https://fipe.parallelum.com.br/api/v2/references"));
    this->request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    this->request.setRawHeader("X-Subscription-Token", Token);

    QNetworkReply *reply = this->manager->get(this->request);

    connect(reply, &QNetworkReply::finished, this, [reply, callback, this]{
        this->SetJsonCallback(reply, callback);
    });
}

void ApiManager::ManagerFinished(QNetworkReply *reply)
{
    qDebug() << "AQUI 5";
}

void ApiManager::slotReadyRead()
{
    qDebug() << "AQUI 1";
}

void ApiManager::finishReading()
{
    qDebug() << "AQUI 4";
}

void ApiManager::slotError(QNetworkReply::NetworkError)
{
    qDebug() << "AQUI 2";
}

void ApiManager::slotSslErrors(const QList<QSslError> &errors)
{
    qDebug() << "AQUI 3";
}

void ApiManager::SetJsonCallback(QNetworkReply *reply, ResponseCallback callback)
{
    if (reply->error())
    {
        qDebug() << reply->errorString();
        return;
    }

    QByteArray answer = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(answer);
    callback(json);
    reply->deleteLater();
}

void ApiManager::SetMonthReferenceParameter(const QString &monthReference, QUrl &url)
{
    if(!monthReference.isEmpty())
    {
        QUrlQuery query(url.query());
        query.addQueryItem("reference", monthReference);
        url.setQuery(query);
    }
}
