#ifndef APIMANAGER_H
#define APIMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QSettings>
#include <QFile>
#include <openssl/rsa.h>

class ApiManager : public QObject
{
    Q_OBJECT

public:
    ApiManager();
    using ResponseCallback = std::function<void(QJsonDocument)>;
    using ImageResponseCallback = std::function<void(QByteArray)>;
    void GetBrands(const QString &vehicleType, const QString &monthReference, ResponseCallback callback);

    void GetModels(const QString &vehicleType, const QString &brandId, const QString &monthReference,
                   ResponseCallback callback);

    void GetYearsByModel(const QString &vehicleType, const QString &brandId, const QString &modelId,
                         const QString &monthReference, ResponseCallback callback);

    void GetFipeInfo(const QString &vehicleType, const QString &brandId, const QString &modelId,
                     const QString &monthReference, const QString &yearId, ResponseCallback callback);

    void GetModelPriceHistory(const QString &vehicleType, const QString &fipeCode, const QString &yearId,
                              const QString &monthReference, ResponseCallback callback);

    void GetMonthReferences(ResponseCallback callback);

    void GetCarImageLinks(const QString &search , ResponseCallback callback);

    void GetCarImage(const QString &imageLink, ImageResponseCallback callback);

public slots:
    void ManagerFinished(QNetworkReply *reply);
    void slotReadyRead();
    void finishReading();
    void slotError(QNetworkReply::NetworkError);
    void slotSslErrors(const QList<QSslError> &errors);

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    void LoadSettings();
    void SetJsonCallback(QNetworkReply *reply, ResponseCallback callback);
    void SetByteArrayCallback(QNetworkReply *reply, ImageResponseCallback callback);
    void SetMonthReferenceParameter(const QString &monthReference, QUrl &url);
    void SetCustomSearchParameters(const QString &search, QUrl &url);

    QByteArray TOKEN;

    QString GOOGLE_SEARCH_API_KEY;
    QString ID_SEARCH_ENGINE;
};

#endif // APIMANAGER_H
