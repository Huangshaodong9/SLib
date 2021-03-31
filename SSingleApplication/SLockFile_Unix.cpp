#include "SLockFile.h"

#include <fcntl.h>
#include <unistd.h>

#include <QDebug>

bool SLockFile::lock(LockMode mode, bool block)
{
    if( !isOpen() )
    {
        qWarning() << "SLockFile::lock(): file is not opened";
        return false;
    }

    if( mode == NoLock )
    {
        return unLock();
    }

    if( mode == m_lockMode )
    {
        return true;
    }

    if( m_lockMode == NoLock )
    {
        unLock();
    }

    struct flock fl;
    fl.l_type = (mode == ReadLock ? F_RDLCK : F_WRLCK);
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_whence = SEEK_SET;

    if( fcntl(handle(), block ? F_SETLKW : F_SETLK, &fl) == -1 )
    {
        qWarning("SLockFile::unLock(): fcntl: %s", strerror(errno));
        return false;
    }

    m_lockMode = mode;
    return true;
}

bool SLockFile::unLock()
{
    if( !isOpen() )
    {
        qWarning() << "SLockFile::unLock(): file is not opened";
        return false;
    }

    if( !isLock() )
    {
        return true;
    }

    struct flock fl;
    fl.l_type = F_UNLCK;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_whence = SEEK_SET;

    if( fcntl(handle(), F_SETLKW, &fl) == -1 )
    {
        qWarning("SLockFile::unLock(): fcntl: %s", strerror(errno));
        return false;
    }

    m_lockMode = NoLock;
    return true;
}
