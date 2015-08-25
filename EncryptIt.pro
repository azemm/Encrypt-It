#-------------------------------------------------
#
# Project created by QtCreator 2015-08-24T21:40:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EncryptIt
TEMPLATE = app

LIBS += -larchive -lboost_system -lboost_filesystem
INCLUDEPATH += "/usr/include/botan-1.10"
LIBS += "/usr/lib/libbotan-1.10.so.0"

SOURCES += main.cpp\
        mainwindow.cpp \
    encryption.cpp \
    zip.cpp \
    zipencrypt.cpp

HEADERS  += mainwindow.h \
    encryption.h \
    zip.h \
    zipencrypt.h

FORMS    += mainwindow.ui
