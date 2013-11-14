#ifndef FACE_H
#define FACE_H

#include "edge.h"
#include <QVector>
#include <QVector3D>
#include <assert.h>

class Face {
public:
    Face() {}
    virtual ~Face() {}

    Face(QVector <Vertex* > inVertices, QVector <Edge* >& inEdges) {
        m_edges = inEdges;
        m_vertices = inVertices;
        for (int i=0; i<inEdges.size(); ++i)
            inEdges[i]->AddIncidentFace(this);
        CalculateCenter();
        CaluclateNorm();
    }

    QVector <Vertex* >& GetVertices() { return m_vertices;}
    QVector <Edge* >& GetEdges() { return Face::m_edges; }
    QVector3D GetCenter() { return m_center; }

protected:
    void CalculateCenter() {
        int size = m_vertices.size();
        qreal x = 0.0;
        qreal y = 0.0;
        qreal z = 0.0;
        for (int i=0; i<size; ++i) {
            x += m_vertices[i]->x();
            y += m_vertices[i]->y();
            z += m_vertices[i]->z();
        }
        x = x/size;
        y = y/size;
        z = z/size;
        m_center = QVector3D(x, y, z);
    }

    void CaluclateNorm() {
        m_norm = QVector3D(0, 0, 0);
        assert(m_vertices.size() >= 3);
        QVector3D U = *m_vertices[1] - *m_vertices[0];
        QVector3D V = *m_vertices[2] - *m_vertices[1];

        m_norm = U.crossProduct(U, V);
        m_norm.normalize();
    }

public:
    Face *m_copyIndicator;
protected:
    QVector <Vertex*> m_vertices;
    QVector <Edge*> m_edges;
    QVector3D m_norm;
    QVector3D m_center;

};

#endif // FACE_H
