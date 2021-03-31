#include "SLockFile.h"

#include <QDebug>

SLockFile::SLockFile() : QFile()
{
    init();
}

SLockFile::SLockFile(QObject* parent)
    : QFile(parent)
{
    init();
}

SLockFile::SLockFile(const QString& name)
    : QFile(name)
{
    init();
}

SLockFile::SLockFile(const QString& name, QObject* parent)
    : QFile(name, parent)
{
    init();
}

bool SLockFile::open(QIODevice::OpenMode flags)
{
    if( flags & QIODevice::Truncate )
    {
        qWarning("SLockFile::open(): Truncate mode not allowed");
        return false;
    }

    return QFile::open(flags);
}

bool SLockFile::isLock() const
{
    return m_lockMode != NoLock;
}

void SLockFile::init()
{
#ifdef Q_OS_WIN
    m_readMutex = Q_NULLPTR;
    m_writeMutex = Q_NULLPTR;
#endif
    m_lockMode = NoLock;
}
