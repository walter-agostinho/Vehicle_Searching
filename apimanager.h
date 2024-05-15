#ifndef APIMANAGER_H
#define APIMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class ApiManager : public QObject
{
    Q_OBJECT

public:
    ApiManager();
    void GetBrands(const QString vehicleType);

public slots:
    void ManagerFinished(QNetworkReply *reply);
    void slotReadyRead();
    void finishReading();
    void slotError(QNetworkReply::NetworkError);
    void slotSslErrors(const QList<QSslError> &errors);

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QString answer;

    QString GetAnswer();


};

#endif // APIMANAGER_H
