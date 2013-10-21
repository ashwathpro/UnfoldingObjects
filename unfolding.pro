#-------------------------------------------------
 #
 # Project created by QtCreator 2010-06-23T12:55:35
 #
 #-------------------------------------------------

 QT       += core gui

 TARGET = cube
 TEMPLATE = app
 SOURCES += main.cpp \
    window.cpp \
    mainwindow.cpp \
    extendedmesh.cpp \
    extendededge.cpp \
    extendedface.cpp \
    output.cpp \
    customdialog.cpp

     QT += opengl

     SOURCES += mainwidget.cpp \
        geometryengine.cpp

     HEADERS += \
         mainwidget.h \
         geometryengine.h \
    mainwindow.h \
    basemesh.h \
    extendedface.h \
    extendedmesh.h \
    extendededge.h \
    output.h \
    customdialog.h

     RESOURCES += \
         shaders.qrc \
         textures.qrc \
        application.qrc

     OTHER_FILES += \
         vshader.glsl \
         fshader.glsl


HEADERS += \
    window.h \
    vertex.h \
    edge.h \
    face.h
