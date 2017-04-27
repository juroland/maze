QT += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

TARGET = maze

TEMPLATE = app

SOURCES += main.cpp \
    model/algorithm.cpp \
    model/evaluation.cpp \
    model/maze.cpp \
    model/mazegenerator.cpp \
    model/mazesolver.cpp \
    doorview.cpp \
    evaluationdialog.cpp \
    evaluationtablemodel.cpp \
    mainwindow.cpp \
    mazecontroller.cpp \
    mazescene.cpp \
    mazesolverview.cpp \
    mazeview.cpp \
    newmazedialog.cpp \
    random.cpp \
    roomview.cpp \
    solvemazedialog.cpp

# Thfollowing define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

FORMS += \
    mainwindow.ui \
    newmazedialog.ui \
    solvemazedialog.ui \
    evaluationdialog.ui

HEADERS += \
    model/algorithm.h \
    model/evaluation.h \
    model/maze.h \
    model/mazegenerator.h \
    model/mazesolver.h \
    doorview.h \
    evaluationdialog.h \
    evaluationtablemodel.h \
    mainwindow.h \
    mazecontroller.h \
    mazescene.h \
    mazesolverview.h \
    mazeview.h \
    newmazedialog.h \
    random.h \
    roomview.h \
    solvemazedialog.h \
    utility.h
