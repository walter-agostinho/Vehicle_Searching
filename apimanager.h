#ifndef APIMANAGER_H
#define APIMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>

const QByteArray Token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VySWQiOiIyM2E"
                         "3MDBhMS0wYmRmLTRjOGMtOWE3Ni03MmU5YzllYzE3NTkiLCJlbWFpbCI6IndhbHR"
                         "lci5hZ29zdGluaG9Ab3V0bG9vay5jb20iLCJpYXQiOjE3MTU2MjE5MDh9.xgLxE"
                         "WKQYuyYBpZes6I4DuQSgiA2rZ3dIu7Q-4wS7L8";

class ApiManager : public QObject
{
    Q_OBJECT

public:
    ApiManager();
    using ResponseCallback = std::function<void(QJsonDocument)>;
    void GetBrands(const QString &vehicleType, ResponseCallback callback);
    void GetModels(const QString &vehicleType, const QString &brandId, ResponseCallback callback);

public slots:
    void ManagerFinished(QNetworkReply *reply);
    void slotReadyRead();
    void finishReading();
    void slotError(QNetworkReply::NetworkError);
    void slotSslErrors(const QList<QSslError> &errors);

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QNetworkRequest request;

    QString GetAnswer();


};

#endif // APIMANAGER_H
