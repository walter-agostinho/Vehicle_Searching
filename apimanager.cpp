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

void ApiManager::GetBrands(const QString &vehicleType, ResponseCallback callback)
{
    this->request.setUrl(QUrl("https://fipe.parallelum.com.br/api/v2/"+vehicleType+"/brands"));
    this->request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    this->request.setRawHeader("X-Subscription-Token",
                               "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VySWQiOiIyM2E"
                               "3MDBhMS0wYmRmLTRjOGMtOWE3Ni03MmU5YzllYzE3NTkiLCJlbWFpbCI6IndhbHR"
                               "lci5hZ29zdGluaG9Ab3V0bG9vay5jb20iLCJpYXQiOjE3MTU2MjE5MDh9.xgLxE"
                               "WKQYuyYBpZes6I4DuQSgiA2rZ3dIu7Q-4wS7L8");

    this->reply = this->manager->get(this->request);

    connect(this->reply, &QNetworkReply::finished, this, [this, callback]{
        if (reply->error())
        {
            qDebug() << reply->errorString();
            return;
        }

        QByteArray answer = reply->readAll();
        qDebug() << answer;
        answer.replace("\\","");
        qDebug() << "DEPOIS = " << answer;
        QJsonDocument json = QJsonDocument::fromJson(answer);
        callback(json);
        this->reply->deleteLater();
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
