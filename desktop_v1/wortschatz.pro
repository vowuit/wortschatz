#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T15:31:28
#
#-------------------------------------------------

QT       += core gui sql widgets quick

TARGET = wortschatz
TEMPLATE = app

SOURCES += \
    carditemmodel.cpp \
    emptytrainermodel.cpp \
    main.cpp \
    mainwindow.cpp \
    sqlitetrainermodel.cpp \
    trainer.cpp \
    trainermodel.cpp \
    wortschatztablemodel.cpp \
    wortschatztableview.cpp \
    wortschatzdatabase.cpp \
    wortschatzdriverplugin.cpp \
    wortschatzdrivercreatorbase.cpp \
    wortschatzdriver.cpp \
    translationssqlmodel.cpp \
    translationscachedmodel.cpp

HEADERS  += \
    carditemmodel.h \
    emptytrainermodel.h \
    mainwindow.h \
    sqlitetrainermodel.h \
    trainer.h \
    trainereditor.h \
    trainermodel.h \
    wortschatztablemodel.h \
    wortschatztableview.h \
    wortschatzdatabase.h \
    wortschatzdriverplugin.h \
    wortschatzdrivercreatorbase.h \
    wortschatzdriver.h \
    translationssqlmodel.h \
    translationscachedmodel.h

RESOURCES += \
    resources.qrc

unix|win32: LIBS += -lsqlite3

DISTFILES += \
    wortschatz.json
