QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    can.cpp \
    cfgtcpsockitem.cpp \
    devicecfg.cpp \
    devicescfg.cpp \
    devicesrvcfg.cpp \
    di.cpp \
    do.cpp \
    filetransmit.cpp \
    main.cpp \
    dialog.cpp \
    net.cpp \
    serialport.cpp \
    syscfg.cpp \
    tcpsockitem.cpp \
    MD5.cpp

HEADERS += \
    DeviceCfgData.h \
    StatusInfo.h \
    can.h \
    cfgtcpsockitem.h \
    devicecfg.h \
    devicescfg.h \
    devicesrvcfg.h \
    di.h \
    dialog.h \
    do.h \
    filetransmit.h \
    net.h \
    serialport.h \
    syscfg.h \
    tcpsockitem.h \
    MD5.h

FORMS += \
    can.ui \
    devicecfg.ui \
    devicescfg.ui \
    devicesrvcfg.ui \
    di.ui \
    dialog.ui \
    do.ui \
    filetransmit.ui \
    net.ui \
    serialport.ui \
    syscfg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
