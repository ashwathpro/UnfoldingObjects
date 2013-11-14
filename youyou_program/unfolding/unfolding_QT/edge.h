#ifndef EDGE_H
#define EDGE_H

#include "vertex.h"
#include "face.h"
#include <QVector>


class Face;

class Edge {
public:
    Edge() { }
    virtual ~Edge() { }
    Edge(Vertex* inStart, Vertex* inEnd) {
        m_start = inStart;
        m_end = inEnd;
        m_center = CalculateCenter();
    }

    Vertex* GetStart() {return m_start;}
    Vertex* GetEnd() {return m_end;}

    virtual bool AddIncidentFace(Face* f) {
        m_faces.push_back(f);
        return true;
    }

    QVector <Face*> IncidentFaces() { return m_faces;}
    virtual Face* GetAnotherFace(Face* f) {
        for (int i=0; i<m_faces.size(); ++i)
            if (m_faces[i] != f)
                return m_faces[i];
        return NULL;
    }

    QVector3D GetCenter() { return m_center; }

    QVector3D CalculateCenter() {
        qreal x = (m_start->x() + m_end->x())/2.0;
        qreal y = (m_start->y() + m_end->y())/2.0;
        qreal z = (m_start->z() + m_end->z())/2.0;

        QVector3D center = QVector3D(x, y, z);
        return center;
    }

public:
    Vertex* m_start;
    Vertex* m_end;
    QVector3D m_center;

protected:
    QVector <Face*> m_faces;
};

#endif // EDGE_H
