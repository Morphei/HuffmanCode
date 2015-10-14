#include "graph.h"
#include "qdebug.h"
#include "QtAlgorithms"

Graph::Graph(std::vector<Vertex*>* array, QWidget *mWidget)
{
    rootVertexes = new std::vector<Vertex*>;
    vertexes = new std::vector<Vertex*>;
    rootVertexes = array;
    widget = mWidget;
    numOfResults = array->size();
}

void Graph::createTree()
{
//    node tempNode;

    int progressMax = rootVertexes->size()-1;
    QProgressDialog* progress = new QProgressDialog("Creating tree...", "Abort", 0, progressMax, widget);
    progress->setWindowTitle("Please wait...");
    progress->setWindowModality(Qt::WindowModal);

    int progressVal = 1;

//    qSort(rootVertexes->begin(), rootVertexes->end());


    progress->show();
    progress->raise();
    progress->activateWindow();

        while(rootVertexes->size() != 1)
        {
            progress->setValue(progressVal);


            if(progress->wasCanceled())
                break;

            Vertex *temp1 = new Vertex;
            Vertex *temp2 = new Vertex;
            Vertex *tempParent = new Vertex;
            temp1 = rootVertexes->at(rootVertexes->size()-1);
            rootVertexes->pop_back();
            temp2 = rootVertexes->at(rootVertexes->size()-1);
            rootVertexes->pop_back();

            tempParent->child1 = temp1;
            tempParent->child2 = temp2;
            tempParent->setValue(temp1->getValue() + temp2->getValue());
            tempParent->setName(temp1->getName() + temp2->getName());

            if(round(tempParent->getValue()*10)/10 == 1.0)
            {
                rootVertexes->push_back(tempParent);
                progress->setValue(progressMax);
                break;
            }

            std::vector<Vertex*>::iterator it;
            it = rootVertexes->begin();
            bool inserted = false;

                while (it != rootVertexes->end())
                {
                    if((*it)->getValue() <= tempParent->getValue())
                        {
                            rootVertexes->insert(it, tempParent);
                            inserted = true;
                            progress->setValue(progressMax);
                            break;
                        }
                    else
                            it++;
                }

                if(!inserted)
                    rootVertexes->insert(it, tempParent);

                progressVal++;
        }
        delete progress;
}

QMultiMap<float, Vertex*> Graph::codding()
{
    int progressMax = rootVertexes->size()-1;
    QProgressDialog* progress = new QProgressDialog("Codding...", "Abort", 0, progressMax, widget);
    progress->setWindowTitle("Please wait...");
    progress->setWindowModality(Qt::WindowModal);
    int progressVal = 0;

    Vertex* temp;
    temp = rootVertexes->at(0);
    temp->setCode("");
    progress->show();
    progress->raise();
    progress->activateWindow();
    enter(temp, &progressVal, progress);

    QMultiMap<float, Vertex*> map;
    for(int i = 0; i < vertexes->size(); i++)
    {
        map.insert(vertexes->at(i)->getValue(), vertexes->at(i));
    }

    delete progress;
    return map;
}

void Graph::enter(Vertex *parent,int *progressPtr, QProgressDialog* dialog)
{
    parent->child1->setCode(parent->getCode() + '1');
    parent->child2->setCode(parent->getCode() + '0');

    if(dialog->wasCanceled())
    {

    }
    else
    {
        if(parent->child1->isRoot())
        {
            vertexes->push_back(parent->child1);
            progressPtr++;
            dialog->setValue(*progressPtr);
        }
        else
            enter(parent->child1, progressPtr, dialog);


        if(parent->child2->isRoot())
        {
            vertexes->push_back(parent->child2);
            progressPtr++;
            dialog->setValue(*progressPtr);
        }
        else
            enter(parent->child2, progressPtr, dialog);
    }
}
