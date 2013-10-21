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

 #include "geometryengine.h"

 GeometryEngine::GeometryEngine()
 {
 }

 GeometryEngine::~GeometryEngine()
 {
 }

 void GeometryEngine::init()
 {
     initializeGLFunctions();
     glEnable(GL_COLOR_MATERIAL);	// color material
     glLineWidth(2.0);
     glPointSize(8.0);

 }


void GeometryEngine::drawBaseMesh(BaseMesh* mesh)
{
    if (mesh == NULL) return;
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    QVector <Face*> faces = mesh->GetFaces();
    for (int i=0; i<faces.size(); ++i)  {
        QVector <Vertex* > vertices = faces[i]->GetVertices();
        glColor4f(0.0, 0.0, 0.0, 1.0);
        glBegin(GL_LINE_LOOP);
        for (int j=0; j<vertices.size(); ++j)
            glVertex3f(vertices[j]->x(), vertices[j]->y(), vertices[j]->z());
        glEnd();

        glColor4f(1.0, 1.0, 1.0, 1.0);
        glBegin(GL_POLYGON);
        for (int j=0; j<vertices.size(); ++j)
            glVertex3f(vertices[j]->x(), vertices[j]->y(), vertices[j]->z());
        glEnd();

    }
}


