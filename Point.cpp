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
#include <QString>
#include <QPointF>

//Include my constant variables
using namespace CONSTANTS;

/*
    Default Constructor for the Point class.
    @detail This constructor just sets the data pointers to NULL.
*/
Point::Point()
{
    data = NULL;
    center = NULL;
}

/*
    Constructor for the Point class.
    @detail This is the constructor that I use to populate the std::vector<Points*>
    in the GraphicsScene object. It accepts the coordinates of the location where
    the Point is being drawn and initializes the QString as "".
    @param int x is the x coordinate of the Point on the GraphicsScene
    @param int y is the y coordinate of the Point on the GraphicsScene
*/
Point::Point(int x, int y)
{
    data = new QString("");
    center = new QPointF(x,y);
}

/*
    Function to check if the Point's data is empty.
    @detail I have set up the program so that a data value of "" is understood
    to be 'empty'.
    @return bool to indicate if the Point's data is empty
*/
bool Point::isEmpty()
{
    if(data->compare(QString("")) == 0){ return true; }
    return false;
}

/*
    Function to check if the event position is within the Point's bounds.
    @detail The Point calculates its bounds by using its position as a central
    point within a rectangle defined by the constants DETECTION_WIDTH and
    DETECTION_HEIGHT.
    @param QPointF position the inputted event position
    @return bool to indicate if the event position was within the Point's bounds
*/
bool Point::inRegion(QPointF position)
{
    if( (position.rx() >= center->rx()-DETECTION_WIDTH/2 && position.rx() <= center->rx()+DETECTION_WIDTH/2) &&
        (position.ry() >= center->ry()-DETECTION_HEIGHT/2 && position.ry() <= center->ry()+DETECTION_HEIGHT/2))
            return true;
    return false;
}

/*
    Function to change the Point's data
    @detail This fuction deletes the data that the current variable is pointing to
    and then generates a new QString object on the heap base off the given QString.
    @param QString s is the desired data value
*/
void Point::updateData(QString s)
{
    delete data;
    data = new QString(s);
}
