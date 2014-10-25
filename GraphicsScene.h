/*
    @mainpage HW3
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

#include "Point.h"
#include "TextBox.h"
#include <QGraphicsScene>

#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

/*
    @class GraphicsScene
    @brief Custom QGraphicsScene class.
    @detail This class is used to create a custom QGraphicsScene object that handles
    the drawing, mouse detection events, and managing of Point objects.
*/
class GraphicsScene : public QGraphicsScene
{
    public:
        //Constructor
        GraphicsScene();

        //Detection Functions
        bool InRegion(QPointF&);
        bool InLabelRegion(QPointF&);
        bool InCloseRegion(QPointF&);

        //Drawing Functions
        QPainter* Draw(bool);
        void DrawIdle(bool);
        void DrawInput();
        void DrawOutput();

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event);

    private:
        //Sub-Drawing Functions
        void DrawPolygonLabel(QPainter*);
        void DrawTitle();
        void DrawTextBox(bool);
        void DrawClose();
        void DrawSave();

        //Event states
        enum State{ Idle, Input, Output };
        State state;

        //Array of the Point objects
        std::vector<Point*> points;

        //Current QGraphicsScene features
        QPixmap* canvas;
        Point* selected_point;
        TextBox* current_textBox;
};

#endif // GRAPHICSSCENE_H
