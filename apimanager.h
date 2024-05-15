#ifndef APIMANAGER_H
#define APIMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>

class ApiManager : public QObject
{
    Q_OBJECT

public:
    ApiManager();
    using ResponseCallback = std::function<void(QJsonDocument)>;
    void GetBrands(const QString &vehicleType, ResponseCallback callback);

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
