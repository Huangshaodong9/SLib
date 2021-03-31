#ifndef SSINGLECOREAPPLICATION_H
#define SSINGLECOREAPPLICATION_H

#include <QCoreApplication>

class SLocalPeer;
class SSingleCoreApplication : public QCoreApplication
{
    Q_OBJECT

signals:
    void messageReceived(const QString& message);

public:
    SSingleCoreApplication(int& argc, char** argv);
    SSingleCoreApplication(const QString& appId, int& argc, char** argv);

    bool isRunning();
    bool sendMessage(const QString& message, int timeout);
    QString getApplicationId() const;

private:
    SLocalPeer* m_localPeer;
};

#endif // SSINGLECOREAPPLICATION_H
