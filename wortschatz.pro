
QT       += core gui sql declarative

TARGET = wortschatz
TEMPLATE = app

maemo5 {
    QT += maemo5
    SOURCES += maemo5mainwindow.cpp
    HEADERS += maemo5mainwindow.h
} else {
    SOURCES += mainwindow.cpp
    HEADERS += mainwindow.h
}

maemo5 {
    # Installation path
    target.path = /opt/$${TARGET}/bin

    desktopfile.files = $${TARGET}.desktop
    desktopfile.path = /usr/share/applications/hildon
    icon.files = $${TARGET}64.png
    icon.path = /usr/share/icons/hicolor/64x64/apps

    INSTALLS += target desktopfile icon
}

SOURCES += \
    main.cpp \
    qtquick1applicationviewer.cpp

HEADERS  += \
    qtquick1applicationviewer.h

RESOURCES += \
    qml.qrc

DISTFILES += \
    main.qml
