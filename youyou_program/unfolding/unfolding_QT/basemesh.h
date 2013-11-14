#ifndef MESH_H
#define MESH_H

#include "face.h"
#include <QVector>
#include <QMessageBox>

class BaseMesh{
public:
    BaseMesh() {}
    ~BaseMesh() { Clear(); }

    void Clear() {
        try {
        for (int i=0; i<m_edges.size(); ++i)
            delete m_edges[i];
        for (int i=0; i<m_faces.size(); ++i)
            delete m_faces[i];
        for (int i=0; i<m_vertices.size(); ++i)
            delete m_vertices[i];
        } catch(...) {
            QMessageBox::critical(NULL, "critical", "Invalid Pointer");
            return;
        }

        m_edges.clear();
        m_vertices.clear();
        m_faces.clear();
    }

    Vertex* AddVertex(qreal x, qreal y, qreal z) {
        try {
            Vertex* v = new Vertex(x, y, z);
            m_vertices.push_back(v);
            return v;
        } catch(...) {
            return NULL;
        }
    }

    Vertex* AddVertex(Vertex* v) {
        if (v == NULL) return NULL;
        return AddVertex(v->x(), v->y(), v->z());
    }

    virtual bool AddFace(QVector <Vertex*>& vertices) {
        if (vertices.size()<2) return false;
        QVector <Edge* > currentEdges;
        for (int i=0; i<vertices.size(); ++i) {
            int firstIndex = i;
            int lastIndex = (i == vertices.size()-1) ? 0 : i+1;
            Edge* current = EdgeExisting(vertices[firstIndex], vertices[lastIndex]);
            if (!current) {
                try {
                    Edge* edge = new Edge(vertices[firstIndex], vertices[lastIndex]);
                    m_edges.push_back(edge);
                    currentEdges.push_back(edge);
                    vertices[firstIndex]->GetIncidentEdges().push_back(edge);
                    vertices[lastIndex]->GetIncidentEdges().push_back(edge);

                } catch(...) {
                    return false;
                }
            } else
                currentEdges.push_back(current);
        }
        try {
            Face* face = new Face(vertices, currentEdges);
            m_faces.push_back(face);
        } catch(...) {
            return false;
        }
        return true;
    }

//    BaseMesh *Clone()
//    {
//        BaseMesh nb = new BaseMesh();
//        nb
//    }

    virtual QVector <Vertex* > GetVertices() { return m_vertices; }
    virtual QVector <Face* > GetFaces() { return m_faces; }
    virtual QVector <Edge* > GetEdges() { return m_edges; }

protected:
    virtual Edge* EdgeExisting(Vertex* s, Vertex* e) {
        for (int i=0; i<m_edges.size(); ++i) {
            if ((m_edges[i]->GetEnd() == e && m_edges[i]->GetStart() == s) ||
                (m_edges[i]->GetEnd() == s && m_edges[i]->GetStart() == e))
                return m_edges[i];
        }
        return NULL;
    }


    QVector <Vertex* > m_vertices;
    QVector <Edge* > m_edges;
    QVector <Face* > m_faces;
};

#endif // MESH_H
