#ifndef VERTEX_H
#define VERTEX_H

#include "edge.h"
#include <QVector3D>
#include <QVector2D>
#include <QVector>

class Edge;

class Vertex : public QVector3D {
public:
    Vertex(qreal x, qreal y, qreal z) : QVector3D(x, y, z) { }
    QVector<Edge*> GetIncidentEdges() {
        return m_edges;
    }

    bool AddEdge(Edge* e) {
        m_edges.push_back(e);
        return true;
    }

    bool EraseEdge(Edge* e) {
        for (int i=0; i<m_edges.size(); ++i)
            if (m_edges[i] == e) {
                m_edges.erase(m_edges.begin()+i);
                return true;
            }
        return false;
    }

private:
    QVector<Edge*> m_edges;
};

#endif // VERTEX_H
