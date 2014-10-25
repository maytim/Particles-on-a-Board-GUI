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

#include <QGraphicsTextItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QEvent>

#ifndef TEXTBOX_H
#define TEXTBOX_H

/*
    @class TextBox
    @brief Custom QGraphicsTextItem class.
    @detail This class is used to create a custom QGraphicsTextItem object that
    displays the particle's data. It has additional features that enable/disable
    the ability to edit the QGraphicsTextItem, and also has some cosmetic modifications
    compared to a standard QGraphicsTextItem.
*/
class TextBox : public QGraphicsTextItem
{
    public:
        //Constructor
        TextBox();

        //Functions
        void SetTextInteraction(bool);
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
        bool sceneEvent(QEvent* event);
};

#endif // TEXTBOX_H
