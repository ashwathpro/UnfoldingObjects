/****************************************************************************
 **
 ** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the QtCore module of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
 **     of its contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #ifndef MAINWIDGET_H
 #define MAINWIDGET_H

 #include "basemesh.h"
 #include "extendedmesh.h"
 #include <QtOpenGL/QGLWidget>
 #include <QtOpenGL/QGLFunctions>

 #include <QMatrix4x4>
 #include <QQuaternion>
 #include <QVector2D>

 class QBasicTimer;
 class QGLShaderProgram;

 class GeometryEngine;

 class MainWidget : public QGLWidget, protected QGLFunctions
 {
     Q_OBJECT
 public:
     explicit MainWidget(QWidget *parent = 0);
     virtual ~MainWidget();

     void Reset() {
         translateX = 0.0f;
         translateY = 0.0f;
         translateZ = -5.0f;
         rotation = QQuaternion(1.0, 0.0, 0.0, 0.0);
         baseMesh = NULL;
         triangleMesh = NULL;
         extendedMesh = NULL;
         isBaseMeshReady = false;
         isTriangleMeshReady = false;
         isFoldedMeshReady = false;
         isUnfoldedMeshReady = false;
     }

     bool SetBaseMesh(BaseMesh* mesh) {
        isBaseMeshReady = true;
        baseMesh = mesh;
        return true;
     }

     bool SetTriangleMesh(BaseMesh* mesh) {
        isTriangleMeshReady = true;
        triangleMesh = mesh;
        return true;
     }

     bool SetExtendedMesh(ExtendedMesh* mesh) {
        isFoldedMeshReady = true;
        isUnfoldedMeshReady = true;
        extendedMesh = mesh;
        return true;
     }

     void ShowOrignal(bool status) { isShowOriginal = status; }
     void ShowTriangle(bool status) { isShowTriangle = status; }
     void ShowFolded(bool status) { isShowFolded = status; }
     void ShowUnfolded(bool status) { isShowUnfolded = status; }

 signals:

 public slots:

 protected:
     void mousePressEvent(QMouseEvent *e);
     void mouseMoveEvent(QMouseEvent *e);
     void wheelEvent(QWheelEvent* e);

     void initializeGL();
     void resizeGL(int w, int h);
     void paintGL();

     void initShaders();
     void initTextures();

 private:
     QGLShaderProgram *program;
     GeometryEngine *geometries;

     GLuint texture;

     QMatrix4x4 projection;

     QVector2D mousePressPosition;
     QVector3D rotationAxis;  // Rotation axis is perpendicular to the mouse position difference
     qreal angularChange;  // Drag to rotate
     QQuaternion rotation;
     qreal translateX;  // Translate object in X, Y, Z directions
     qreal translateY;
     qreal translateZ;

     BaseMesh* baseMesh;  // NOT OWNED
     BaseMesh* triangleMesh; // NOT OWNED
     ExtendedMesh* extendedMesh; // NOT OWNED

     bool isBaseMeshReady;
     bool isTriangleMeshReady;
     bool isFoldedMeshReady;
     bool isUnfoldedMeshReady;

     bool isShowOriginal;
     bool isShowTriangle;
     bool isShowFolded;
     bool isShowUnfolded;
 };

 #endif // MAINWIDGET_H
