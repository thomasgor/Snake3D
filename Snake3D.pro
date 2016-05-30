#-------------------------------------------------
#
# Project created by QtCreator 2016-05-30T15:40:18
#
#-------------------------------------------------

QT       += core gui opengl

unix: LIBS += -lassimp # Windows can't deal with assimp as a system library for some reason
win32: LIBS += -LC:/assimp/code/ -lassimp
win32: INCLUDEPATH += C:/assimp/include
win32: DEPENDPATH += C:/assimp/include

LIBS += -lopengl32
LIBS += -lfreeglut
LIBS += -lglut32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Snake3D
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myglwidget.cpp \
    schlange.cpp \
    modelloader.cpp

HEADERS  += mainwindow.h \
    myglwidget.h \
    schlange.h \
    modelloader.h

FORMS    += mainwindow.ui

RESOURCES += \
    shader.qrc \
    textures.qrc
