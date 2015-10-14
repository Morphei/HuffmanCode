#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vector"
#include "QDebug"
#include "calculator.h"
#include "QMessageBox"
#include "vertex.h"
#include <X11/Xlib.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    XInitThreads();
    ui->setupUi(this);
    ui->pushButton_2->setEnabled(false);
    ui->horizontalSlider->setEnabled(false);
    entropy = 0;
    entropyCount = false;
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::createVertexArray()
{
    std::vector<Vertex*>* vertex = new std::vector<Vertex*>;
    QString name, value, str;
    str = ui->textEdit->toPlainText();
    str.remove(' ');

    if(!str.isEmpty())
    {
    QStringList list;

    list = str.split(',');

    for(int i = 0; i < list.size(); i++)
    {
        Vertex *tempVertex = new Vertex;
        int j = 0;
        while (list.at(i).at(j) != '-') {
            name += list.at(i).at(j);
            j++;
            if(j >= list.size())
                break;
        }
        j++;
        while (j < list.at(i).size()) {
            value += list.at(i).at(j);
            j++;
        }

        tempVertex->setName(name);
        tempVertex->setValue(value.toFloat());
        tempVertex->setRoot();
        vertex->push_back(tempVertex);
        name.clear();
        value.clear();
    }
    arrayOfVertexes = vertex;
    }
    else
    {
        QMessageBox box;
        box.setText("Wrong input!");
        box.show();
    }

    if(!entropyCount)
    {
    for(int i = 0; i < arrayOfVertexes->size(); i++)
        {
            entropy += arrayOfVertexes->at(i)->getValue() * log2(arrayOfVertexes->at(i)->getValue());
        }
    entropy = -entropy;

    ui->Entropy->setText(QString::number(entropy));

    entropyCount = true;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    createVertexArray();
    std::vector<Vertex*> vertexes/* = new std::vector<Vertex*>*/;
    std::vector<Vertex*> tempArray1/* = new std::vector<Vertex*>*/;
    std::vector<Vertex*> tempArray2/* = new std::vector<Vertex*>*/;


    int rounds = ui->horizontalSlider->value();

    bool currentArray = false;
    tempArray1 = *arrayOfVertexes;

    int j = 0;
    for(int i = 1; i < rounds; i++)
    {
        if(!currentArray)
            tempArray2.clear();
        else
            tempArray1.clear();

        for(j = 0; j < arrayOfVertexes->size(); j++)
        {
            if(!currentArray)
            {
                for(int n = 0; n < tempArray1.size(); n++)
                {
                    Vertex* temp = new Vertex;
                    temp->setName(tempArray1.at(n)->getName() + arrayOfVertexes->at(j)->getName());
                    temp->setRoot();
                    temp->setValue(tempArray1.at(n)->getValue() * arrayOfVertexes->at(j)->getValue());
                    tempArray2.push_back(temp);
                }
            }
            else
            {
                for(int n = 0; n < tempArray2.size(); n++)
                {
                    Vertex* temp = new Vertex;
                    temp->setName(tempArray2.at(n)->getName() + arrayOfVertexes->at(j)->getName());
                    temp->setRoot();
                    temp->setValue(tempArray2.at(n)->getValue() * arrayOfVertexes->at(j)->getValue());
                    tempArray1.push_back(temp);
                }
            }
        }
        j=0;
        currentArray = !currentArray;
    }

    if(currentArray)
        vertexes = tempArray2;
    else vertexes = tempArray1;


    Calculator* calculator = new Calculator(&vertexes, ui->centralWidget);

    time.start();

    map = calculator->codding();
    encodingEnd();
}

void MainWindow::on_textEdit_selectionChanged()
{

}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    ui->label_6->setText(QString::number(position));
}

void MainWindow::on_pushButton_3_clicked()
{
    entropy = 0;
    entropyCount = false;
    QMessageBox* box = new QMessageBox;
    if(ui->textEdit->toPlainText().size() == 0)
    {
        box->setText("Enter something you wanna to encode");
        box->show();
    }
    else{
        createVertexArray();

        float summ = 0;
        for(int i = 0; i < arrayOfVertexes->size(); i++)
            {
                summ += arrayOfVertexes->at(i)->getValue();
            }

        if(round(summ*10)/10 != 1.0)
            {
                box->setText("Summ of numbers need to be 1, in your case it is " + QString::number(summ));
                box->show();
            }
        else if(arrayOfVertexes->size() == 1)
            {
                box->setText("You need at least 2 elements!");
                box->show();
            }
        else
            {
                ui->horizontalSlider->setMinimum(1);
                ui->horizontalSlider->setMaximum(10);
                ui->horizontalSlider->setValue(1);
                ui->label_2->setText(QString::number(ui->horizontalSlider->minimum()));
                ui->label_4->setText(QString::number(ui->horizontalSlider->maximum()));
                ui->label_6->setText(QString::number(ui->horizontalSlider->value()));

                ui->horizontalSlider->setEnabled(true);
                ui->pushButton_2->setEnabled(true);
        }
    }
    ui->textEdit_2->setFocus();
    ui->pushButton_3->setEnabled(false);
}

void MainWindow::on_textEdit_textChanged()
{
    ui->pushButton_3->setEnabled(true);
}

void MainWindow::encodingEnd()
{
    std::vector<Vertex*>* coddedVertexes = new std::vector<Vertex*>;

    QMultiMap<float, Vertex*>::iterator it;
    it = map.end() - 1;
    while (it != map.begin() - 1) {
        coddedVertexes->push_back((*it));
        it--;
    }


    QString text;
    float lenght = 0;
    for(int i = 0; i < coddedVertexes->size(); i++)
            {
                text += coddedVertexes->at(i)->getName() + "(" + QString::number(coddedVertexes->at(i)->getValue()) +
                        ")" + " = " + coddedVertexes->at(i)->getCode() + "\n";

                lenght += coddedVertexes->at(i)->getCode().size() * coddedVertexes->at(i)->getValue();
            }

    lenght = lenght/ui->horizontalSlider->value();
    ui->lenght->setText(QString::number(lenght));
    ui->textEdit_2->setText(text);
    ui->Redundancy->setText(QString::number(1.0-(entropy/lenght)));
    ui->statusBar->showMessage("Number of permutations: " + QString::number(coddedVertexes->size()) +
                               "          Elapsed time: " + QString::number((float)time.elapsed()/1000) + " seconds");


}

void MainWindow::progressChanged(int i)
{
    ui->statusBar->showMessage(QString::number(i));
}

void MainWindow::on_MainWindow_destroyed()
{

}
