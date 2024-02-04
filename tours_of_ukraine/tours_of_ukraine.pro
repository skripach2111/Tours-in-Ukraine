QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addeditclientsdialog.cpp \
    addedittouristsitesdialog.cpp \
    addedittoursdialog.cpp \
    addeditvauchersdialog.cpp \
    clientsfiltermodel.cpp \
    clientsmodel.cpp \
    databasemodule.cpp \
    imageintoursmodel.cpp \
    main.cpp \
    mainwindow.cpp \
    selecttourssitesdialog.cpp \
    touristsitesfiltermodel.cpp \
    touristsitesintourmodel.cpp \
    touristsitesmodel.cpp \
    toursfiltermodel.cpp \
    toursmodel.cpp \
    vauchersfiltermodel.cpp \
    vauchersmodel.cpp

HEADERS += \
    addeditclientsdialog.h \
    addedittouristsitesdialog.h \
    addedittoursdialog.h \
    addeditvauchersdialog.h \
    clientsfiltermodel.h \
    clientsmodel.h \
    databasemodule.h \
    imageintoursmodel.h \
    mainwindow.h \
    selecttourssitesdialog.h \
    touristsitesfiltermodel.h \
    touristsitesintourmodel.h \
    touristsitesmodel.h \
    toursfiltermodel.h \
    toursmodel.h \
    vauchersfiltermodel.h \
    vauchersmodel.h

FORMS += \
    addeditclientsdialog.ui \
    addedittouristsitesdialog.ui \
    addedittoursdialog.ui \
    addeditvauchersdialog.ui \
    mainwindow.ui \
    selecttourssitesdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
