#ifndef SSINGLEAPPLICATION_H
#define SSINGLEAPPLICATION_H

#include <QApplication>

class SLocalPeer;
class SSingleApplication : public QApplication
{
    Q_OBJECT

signals:
    void messageReceived(const QString& message);

public:
    SSingleApplication(int& argc, char** argv);
    SSingleApplication(const QString& appId, int& argc, char** argv);

    bool isRunning();
    bool sendMessage(const QString& message, int timeout);
    QString getApplicationId() const;
    void setActiveWindow(QWidget* window, bool activeOnMessage = true);
    QWidget* getActiveWindow() const;

public slots:
    void onActiveWindow();

private:
    SLocalPeer* m_localPeer;
    QWidget* m_activeWindow;
};

#endif // SSINGLEAPPLICATION_H
