#include "SSingleApplication.h"

#include <QWidget>

#include "SLocalPeer.h"

SSingleApplication::SSingleApplication(int& argc, char** argv)
    : QApplication(argc, argv),
      m_localPeer(new SLocalPeer("", this)),
      m_activeWindow(NULL)
{
    connect(m_localPeer, SIGNAL(messageReceived(const QString&)), SIGNAL(messageReceived(const QString&)));
}

SSingleApplication::SSingleApplication(const QString& appId, int& argc, char** argv)
    : QApplication(argc, argv),
      m_localPeer(new SLocalPeer(appId, this)),
      m_activeWindow(NULL)
{
    connect(m_localPeer, SIGNAL(messageReceived(const QString&)), SIGNAL(messageReceived(const QString&)));
}

bool SSingleApplication::isRunning()
{
    return m_localPeer->isRunning();
}

bool SSingleApplication::sendMessage(const QString& message, int timeout)
{
    return m_localPeer->sendMessage(message, timeout);
}

QString SSingleApplication::getApplicationId() const
{
    return m_localPeer->getApplicationId();
}

void SSingleApplication::setActiveWindow(QWidget* window, bool activeOnMessage)
{
    m_activeWindow = window;

    if( activeOnMessage )
    {
        connect(m_localPeer, SIGNAL(messageReceived(const QString&)), this, SLOT(onActiveWindow()));
    }
    else
    {
        disconnect(m_localPeer, SIGNAL(messageReceived(const QString&)), this, SLOT(onActiveWindow()));
    }
}

QWidget* SSingleApplication::getActiveWindow() const
{
    return m_activeWindow;
}

void SSingleApplication::onActiveWindow()
{
    if( m_activeWindow )
    {
        m_activeWindow->setWindowState(m_activeWindow->windowState() & ~Qt::WindowMinimized);
        m_activeWindow->raise();
        m_activeWindow->activateWindow();
    }
}
