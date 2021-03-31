QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SLib
TEMPLATE = app

HEADERS += \
    SSingleApplication/SLockFile.h \
    SSingleApplication/SLocalPeer.h \
    SSingleApplication/SSingleApplication.h \
    SSingleApplication/SSingleCoreApplication.h

SOURCES += \
    SSingleApplication/SLockFile.cpp \
    SSingleApplication/SLocalPeer.cpp \
    SSingleApplication/SSingleApplication.cpp \
    SSingleApplication/SSingleCoreApplication.cpp \
    main.cpp

win32{
    SOURCES += \
        SSingleApplication/SLockFile_Win.cpp
}

unix{
    SOURCES += \
        SSingleApplication/SLockFile_Unix.cpp
}
