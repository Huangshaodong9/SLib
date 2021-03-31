#include "SSingleCoreApplication.h"

#include  "SLocalPeer.h"

SSingleCoreApplication::SSingleCoreApplication(int& argc, char** argv)
    : QCoreApplication(argc, argv),
      m_localPeer(new SLocalPeer("", this))
{
    connect(m_localPeer, SIGNAL(messageReceived(const QString&)), SIGNAL(messageReceived(const QString&)));
}

SSingleCoreApplication::SSingleCoreApplication(const QString& appId, int &argc, char **argv)
    : QCoreApplication(argc, argv),
      m_localPeer(new SLocalPeer(appId, this))
{
    connect(m_localPeer, SIGNAL(messageReceived(cosnt QString&)), this, SIGNAL(messageReceived(cosnt QString&)));
}

bool SSingleCoreApplication::isRunning()
{
    return m_localPeer->isRunning();
}

bool SSingleCoreApplication::sendMessage(const QString& message, int timeout)
{
    return m_localPeer->sendMessage(message, timeout);
}

QString SSingleCoreApplication::getApplicationId() const
{
    return m_localPeer->getApplicationId();
}
