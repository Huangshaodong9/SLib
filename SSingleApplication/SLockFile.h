#ifndef SLOCKFILE_H
#define SLOCKFILE_H

#include <QFile>

#ifdef Q_OS_WIN
#include <QVector>
#endif

class SLockFile : public QFile
{
public:
    enum LockMode
    {
        NoLock,
        ReadLock,
        WriteLock
    };

    SLockFile();
    SLockFile(QObject* parent);
    SLockFile(const QString& name);
    SLockFile(const QString& name, QObject* parent);

    bool open(OpenMode flags) Q_DECL_OVERRIDE;
    bool lock(LockMode mode, bool block = true);
    bool unLock();
    bool isLock() const;

private:
#ifdef Q_OS_WIN
    Qt::HANDLE m_readMutex;
    Qt::HANDLE m_writeMutex;
    QVector<Qt::HANDLE> m_readMutexVec;
    QString m_mutexName;

    Qt::HANDLE getMutexHandle(int index, bool doCreate);
    bool waitMutex(Qt::HANDLE mutex, bool doBlock);
#endif

    LockMode m_lockMode;

    void init();
};

#endif // SLOCKFILE_H
