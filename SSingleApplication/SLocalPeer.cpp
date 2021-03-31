#include "SLocalPeer.h"

#include <unistd.h>

#include <QDir>
#include <QDataStream>
#include <QLocalSocket>
#include <QLocalServer>
#include <QCoreApplication>


const char* SLocalPeer::ACK = "ACK";

SLocalPeer::SLocalPeer(const QString& appId, QObject* parent)
    : QObject(parent),
      m_appId(appId),
      m_localServer(new QLocalServer(this))
{
    QString prefix = appId;
    if( m_appId.isEmpty() )
    {
        m_appId = QCoreApplication::applicationFilePath();
        prefix = m_appId.section("/", -1);
    }
    prefix.remove(QRegExp("[^a-zA-Z]"));
    prefix.truncate(6);

    m_socketName = "SSingleApplication-" + prefix + "-" + QString::number(::getuid(), 16);
    QString lockFileName = QDir(QDir::tempPath()).absolutePath() + "/" + m_socketName + "-LockFile";

    m_lockFile.setFileName(lockFileName);
    m_lockFile.open(QIODevice::ReadWrite);
}

bool SLocalPeer::isRunning()
{
    if( m_lockFile.isLock() )
    {
        return false;
    }

    if( !m_lockFile.lock(SLockFile::WriteLock, false) )
    {
        return true;
    }

    bool result = m_localServer->listen(m_socketName);
    if( !result && (m_localServer->serverError() == QAbstractSocket::AddressInUseError) )
    {
        QFile::remove(QDir::cleanPath(QDir::tempPath()) + "/" + m_socketName);
        result = m_localServer->listen(m_socketName);
    }

    if( result )
    {
        connect(m_localServer, SIGNAL(newConnection()), this, SLOT(onReceiveConnection()));
    }
    else
    {
        qWarning("SLocalPeer::isRunning() listen on local socket failed, %s", qUtf8Printable(m_localServer->errorString()));
    }

    return false;
}

bool SLocalPeer::sendMessage(const QString& message, int timeout)
{
    bool ret = false;

    if( !isRunning() )
    {
        return false;
    }

    QLocalSocket socket;

    // 尝试两次,防止单一实例进程刚启动情况
    for( int i=0; i<2; i++ )
    {
        socket.connectToServer(m_socketName);
        if( socket.waitForConnected(timeout/2) || (i == 1) )
        {
            ret = true;
            break;
        }

        int ms = 250;
#ifdef Q_OS_WIN
        Sleep(DWORD(ms));
#else
        struct timespec ts;
        ts.tv_sec = ms/1000;
        ts.tv_nsec = (ms%1000) * 1000 * 1000;

        nanosleep(&ts, NULL);
#endif
    }

    if( !ret )
    {
        return false;
    }

    QByteArray uMsg = message.toUtf8();

    QDataStream ds(&socket);
    ds.writeBytes(uMsg.constData(), uMsg.size());

    ret = socket.waitForBytesWritten(timeout);
    ret = ret && socket.waitForReadyRead(timeout);
    ret = ret && (socket.read(qstrlen(ACK)) == ACK);

    return ret;
}

QString SLocalPeer::getApplicationId()
{
    return m_appId;
}

void SLocalPeer::onReceiveConnection()
{
    QLocalSocket* socket = m_localServer->nextPendingConnection();

    if( socket == NULL )
    {
        return;
    }

    while( socket->bytesAvailable() < (qint64)sizeof(quint32) )
    {
        socket->waitForReadyRead();
    }

    QDataStream ds(socket);
    QByteArray uMsg;
    quint32 remaining = 0;

    ds >> remaining;
    uMsg.resize(remaining);

    int got = 0;
    char* uMsgBuf = uMsg.data();

    do
    {
        got = ds.readRawData(uMsgBuf, remaining);
        remaining -= got;
        uMsgBuf += got;
    } while(remaining && got >= 0 && socket->waitForReadyRead(2000));

    if( got == -1 )
    {
        qWarning("SLocalPeer::onReceiveConnection():  Message receive failed %s", qUtf8Printable(socket->errorString()));
        delete socket;
        return;
    }

    socket->write(ACK, qstrlen(ACK));
    socket->waitForBytesWritten();
    delete socket;

    qDebug() << QString::fromUtf8(uMsg);

    emit messageReceived(QString::fromUtf8(uMsg));
}
