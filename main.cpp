/*
    @mainpage Particles on a Board GUI
    @author Tim Maytom
    @date 10/24/2014
    @section DESCRIPTION

    In this assignment I draw a Particle Board which is a horizontal axis.
    I allow the user to use mouse inputs to interactive with the Board, and place
    particles. Each particle can store a string of 10 chars. The user can switch
    between the particle input and output state by clicking on an empty particle or
    the label of a populated particle. They can also delete a particle by pressing the
    'X' in the output state of the label.
*/

#include <QApplication>

#include <QWidget>
#include <QHBoxLayout>
#include <QGraphicsView>
#include "GraphicsScene.h"

//Allow access to constant variables
using namespace CONSTANTS;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Create a window for the app and modify its settings
    QWidget* window = new QWidget;
    window->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    window->setWindowTitle("Particles on a Board");

    //Create a layout to hold the Widgets including the important GraphicsScene
    QHBoxLayout* layout = new QHBoxLayout;
    GraphicsScene* scene = new GraphicsScene();
    QGraphicsView* view = new QGraphicsView(scene);
    layout->addWidget(view);
    window->setLayout(layout);
    window->show();

    return a.exec();
}



