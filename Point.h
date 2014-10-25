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

#include <QPointF>
#include <QString>
#include <QFont>
#include <QFontMetrics>

#ifndef POINT_H
#define POINT_H

/*
    @namespace CONSTANTS
    @brief A namespace to store all constant data and settings for the program.
    @details This namespace is used in Point.h, TextBox.h, GraphicsScene.h, and main.cpp to
    to store the constant fields used when bulding the gui. It allows easy access and modification
    of the constant fields.
*/
namespace CONSTANTS
{
    //Title variables
    const QString TITLE("Welcome to Particles on a Board!");
    const QString INFO("Click a point on the axis to insert a particle. It's interactive!");
    const int PADDING_TITLE = 25;

    //Canvas settings
    const int CANVAS_WIDTH = 640;
    const int CANVAS_HEIGHT = 240;
    const int WINDOW_WIDTH = CANVAS_WIDTH+30;
    const int WINDOW_HEIGHT = CANVAS_HEIGHT+30;

    //Axis Settings
    const int AXIS_MIN = -9;
    const int AXIS_MAX = 10;
    const int MARKER_INTERVAL = 15;
    const int LABEL_SPACER = 14;
    const int TOTAL_MARKERS = 20;
    const int AXIS_TAIL = MARKER_INTERVAL/2;
    const int PADDING_TOP = (2*CANVAS_HEIGHT)/3;
    const int PADDING_LEFT = (CANVAS_WIDTH-(MARKER_INTERVAL*TOTAL_MARKERS))/2;

    //Custom Label Settings
    const int LABEL_HEIGHT = 20;
    const int LABEL_WIDTH = 100;
    const int LABEL_HEIGHT_MARGIN = 10;

    //Tools
    const QFont DEFAULT_FONT;
    const QFontMetrics METRICS(DEFAULT_FONT);

    //Mouse Detection Settings
    const int DETECTION_WIDTH = 10;
    const int DETECTION_HEIGHT = 10;
}

/*
    @class Point
    @brief The basic data object class.
    @detail This class is used to store each point's location on the canvas and data value.
*/
class Point
{
    public:
        //Constructors
        Point();
        Point(int, int);

        //Functions
        bool isEmpty();
        bool inRegion(QPointF);
        void updateData(QString);

        //Accessors
        QPointF getCenter(){ return *center; }
        QString getData(){ return *data; }

    private:
        QString* data;
        QPointF* center;
};

#endif // POINT_H
