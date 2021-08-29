QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    confirmclose.cpp \
    dffarchive.cpp \
    imgarchiver.cpp \
    imgcanvas.cpp \
    main.cpp \
    mainwindow.cpp \
    menu.cpp \
    txdarchive.cpp

HEADERS += \
    confirmclose.h \
    dffarchive.h \
    imgarchiver.h \
    imgcanvas.h \
    mainwindow.h \
    menu.h \
    txdarchive.h

FORMS += \
    confirmclose.ui \
    mainwindow.ui

TRANSLATIONS += \
    GnomeSATools_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
