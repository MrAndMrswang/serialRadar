QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    radar.cpp \
    serialcom.cpp


HEADERS += \
    lidarpoint.h \
    mainwindow.h \
    radar.h \
    serialcom.h \
    ui_serialcom.h \

# for Lidar SDK
SOURCES += \
    sdk/sdk/src/arch/linux/net_serial.cpp \
    sdk/sdk/src/arch/linux/net_socket.cpp \
    sdk/sdk/src/arch/linux/timer.cpp \
    sdk/sdk/src/hal/thread.cpp \
    sdk/sdk/src/rplidar_driver.cpp

HEADERS += \
    sdk/sdk/include/rplidar.h \
    sdk/sdk/include/rplidar_cmd.h \
    sdk/sdk/include/rplidar_driver.h \
    sdk/sdk/include/rplidar_protocol.h \
    sdk/sdk/include/rptypes.h \
    sdk/sdk/src/arch/linux/arch_linux.h \
    sdk/sdk/src/arch/linux/net_serial.h \
    sdk/sdk/src/arch/linux/thread.hpp \
    sdk/sdk/src/arch/linux/timer.h \
    sdk/sdk/src/hal/abs_rxtx.h \
    sdk/sdk/src/hal/assert.h \
    sdk/sdk/src/hal/byteops.h \
    sdk/sdk/src/hal/event.h \
    sdk/sdk/src/hal/locker.h \
    sdk/sdk/src/hal/socket.h \
    sdk/sdk/src/hal/thread.h \
    sdk/sdk/src/hal/types.h \
    sdk/sdk/src/hal/util.h \
    sdk/sdk/src/rplidar_driver_impl.h \
    sdk/sdk/src/rplidar_driver_serial.h \
    sdk/sdk/src/rplidar_driver_TCP.h \
    sdk/sdk/src/sdkcommon.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
