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

 #include "mainwidget.h"

 #include "geometryengine.h"

 #include <QtOpenGL/QGLShaderProgram>

 #include <QMouseEvent>
 #include <QDebug>
 #include <QApplication>
 #include <math.h>
 #include <locale.h>
 #include <iostream>

 MainWidget::MainWidget(QWidget *parent) :
     QGLWidget(parent),
     program(new QGLShaderProgram),
     geometries(new GeometryEngine)
 {
     translateX = 0.0f;
     translateY = 0.0f;
     translateZ = -5.0f;
     isBaseMeshReady = false;
     isTriangleMeshReady = false;
     isUnfoldedMeshReady = false;
     isFoldedMeshReady = false;

     isShowOriginal = true;
     isShowTriangle = true;
     isShowFolded = true;
     isShowUnfolded = true;
 }

 MainWidget::~MainWidget()
 {
     delete program; program = 0;
     delete geometries; geometries = 0;

 //    deleteTexture(texture);
 }

 void MainWidget::mousePressEvent(QMouseEvent *e)
 {
     // Saving mouse press position
     mousePressPosition = QVector2D(e->pos());
 }

 void MainWidget::mouseMoveEvent(QMouseEvent *e)
 {

     // Mouse release position - mouse press position
     QVector2D diff = QVector2D(e->pos()) - mousePressPosition;

     // Rotation axis is perpendicular to the mouse position difference
     // vector
     QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

     // Accelerate angular speed relative to the length of the mouse sweep
     qreal acc = diff.length() / 4.0;

     // Calculate new rotation axis as weighted sum
     rotationAxis = (rotationAxis * angularChange + n * acc).normalized();

     // Change rotation angle
     angularChange = acc;

     if(e->buttons() & Qt::LeftButton)
     {
         if( QApplication::keyboardModifiers() == Qt::ShiftModifier ) {
             translateX += diff.x()/100;
             translateY -= diff.y()/100;
         }
         else if( QApplication::keyboardModifiers() == Qt::AltModifier ) {
            rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularChange) * rotation;
         }
         mousePressPosition = QVector2D(e->pos());
         // Update scene
         updateGL();
     }
 }

 void MainWidget::wheelEvent(QWheelEvent* e) {
     double numSteps = e->delta()/100.0f;
     translateZ += numSteps;
     updateGL();
 }

 void MainWidget::initializeGL()
 {
     initializeGLFunctions();

     qglClearColor(Qt::white);

     qDebug() << "Initializing shaders...";
     initShaders();

   //  qDebug() << "Initializing textures...";
   //  initTextures();

     // Enable depth buffer
     glEnable(GL_DEPTH_TEST);

     // Enable back face culling
    // glEnable(GL_CULL_FACE);

     qDebug() << "Initializing geometries...";
     geometries->init();

 }

 void MainWidget::initShaders()
 {
     // Overriding system locale until shaders are compiled
     setlocale(LC_NUMERIC, "C");

     // Compiling vertex shader
     if (!program->addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl"))
         close();

     // Compiling fragment shader
     if (!program->addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl"))
         close();

     // Linking shader pipeline
     if (!program->link())
         close();

     // Binding shader pipeline for use
     if (!program->bind())
         close();

     // Restore system locale
     setlocale(LC_ALL, "");
 }

 void MainWidget::initTextures()
 {
     // Loading cube.png to texture unit 0
     glActiveTexture(GL_TEXTURE0);
     glEnable(GL_TEXTURE_2D);
     texture = bindTexture(QImage(":/cube.png"));

     // Set nearest filtering mode for texture minification
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

     // Set bilinear filtering mode for texture magnification
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

     // Wrap texture coordinates by repeating
     // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 }

 void MainWidget::resizeGL(int w, int h)
 {
     // Set OpenGL viewport to cover whole widget
     glViewport(0, 0, w, h);

     // Calculate aspect ratio
     qreal aspect = (qreal)w / ((qreal)h?h:1);

     // Set near plane to 1.0, far plane to 7000.0, field of view 45 degrees
     const qreal zNear = 1.0, zFar = 7000.0, fov = 45.0;

     // Reset projection
     projection.setToIdentity();

     // Set perspective projection
     projection.perspective(fov, aspect, zNear, zFar);
 }

 void MainWidget::paintGL()
 {
     // Clear color and depth buffer
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     // Calculate model view transformation
     QMatrix4x4 matrix;
     matrix.translate(translateX, translateY, translateZ);
     matrix.rotate(rotation);

     // Set modelview-projection matrix
     program->setUniformValue("mvp_matrix", projection * matrix);

     // Using texture unit 0 which contains cube.png
    // program->setUniformValue("texture", 0);

     // Draw the base mesh
     if (isBaseMeshReady && isShowOriginal) {
         geometries->drawBaseMesh(baseMesh);
     }
     if (isTriangleMeshReady && isShowTriangle) {
         geometries->drawBaseMesh(triangleMesh);
     }
     if (isFoldedMeshReady && isShowFolded) {
         BaseMesh* baseFoldedMesh = extendedMesh->GetFoldedMesh();
         geometries->drawBaseMesh(baseFoldedMesh);
     }
     if (isUnfoldedMeshReady && isShowUnfolded) {
         BaseMesh* baseUnfoldedMesh = extendedMesh->GetUnFoldedMesh();
         geometries->drawBaseMesh(baseUnfoldedMesh);
     }

 }


























