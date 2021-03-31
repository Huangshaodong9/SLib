#ifndef SLOCALPEER_H
#define SLOCALPEER_H

#include <QObject>

#include "SLockFile.h"

class QLocalServer;
class SLocalPeer : public QObject
{
    Q_OBJECT

signals:
    void messageReceived(const QString& message);

public:
    explicit SLocalPeer(const QString& appId, QObject* parent = NULL);

    bool isRunning();
    bool sendMessage(const QString& message, int timeout);
    QString getApplicationId();

private:
    QString m_appId;
    SLockFile m_lockFile;
    QLocalServer* m_localServer;
    QString m_socketName;

    static const char* ACK;

private slots:
    void onReceiveConnection();
};

#endif // SLOCALPEER_H
