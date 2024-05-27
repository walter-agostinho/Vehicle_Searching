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

// const QByteArray Token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VySWQiOiIyM2E"
//                          "3MDBhMS0wYmRmLTRjOGMtOWE3Ni03MmU5YzllYzE3NTkiLCJlbWFpbCI6IndhbHR"
//                          "lci5hZ29zdGluaG9Ab3V0bG9vay5jb20iLCJpYXQiOjE3MTU2MjE5MDh9.xgLxE"
//                          "WKQYuyYBpZes6I4DuQSgiA2rZ3dIu7Q-4wS7L8";

// const QString GOOGLE_SEARCH_API_KEY = "AIzaSyCgrzTn33eqwFfY7qtjz4CCab562IYF2dM";
// const QString ID_SEARCH_ENGINE = "a20665af4ceb54e8d";

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
