#ifndef GRAPH_H
#define GRAPH_H

#include "vertex.h"
#include "qstatusbar.h"
#include "qprogressdialog.h"
#include <QTime>

class Graph
{
public:
    Graph(std::vector<Vertex *> *array, QWidget* mWidget);
    void createTree();
    QMultiMap<float, Vertex *> codding();

private:

    struct node {
        Vertex parent;
        Vertex children1, children2;
    };

    int numOfResults;

    int compar(const Vertex* first, const Vertex* second);

    bool lessThan(Vertex* first, Vertex* second)
    {
        return first->getValue() < second->getValue();
    }

    QWidget* widget;
    std::vector<Vertex*>* rootVertexes;
    std::vector<Vertex*>* vertexes;

    void enter(Vertex* parent, int *progressPtr, QProgressDialog* dialog);
};

#endif // GRAPH_H
