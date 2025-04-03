QT       += core gui
QT       += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Testy_ARX.cpp \
    Testy_PID.cpp \
    Testy_UAR.cpp \
    arx.cpp \
    arxwidget.cpp \
    generator.cpp \
    main.cpp \
    mainwindow.cpp \
    pid.cpp \
    symulacja.cpp

HEADERS += \
    arx.h \
    arxwidget.h \
    generator.h \
    glowne.h \
    mainwindow.h \
    pid.h \
    symulacja.h

FORMS += \
    arxwidget.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
