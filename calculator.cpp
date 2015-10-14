#include "calculator.h"


Calculator::Calculator(std::vector<Vertex*>* array, QWidget *mainWidget)
{
    arrayOfVertexes = new std::vector<Vertex*>;
    arrayOfVertexes = array;
    widget = mainWidget;
}

float Calculator::entropy()
{
    float entropy = 0;

    for(int i = 0; i < arrayOfVertexes->size(); i++)
        {
            entropy += arrayOfVertexes->at(i)->getValue() * log2(arrayOfVertexes->at(i)->getValue());
        }
    entropy = -entropy;

    return entropy;
}

QMultiMap<float, Vertex*> Calculator::codding()
{
    Graph graph(arrayOfVertexes, widget);
    graph.createTree();

    return graph.codding();
}
