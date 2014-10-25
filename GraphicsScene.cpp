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

#include "GraphicsScene.h"
#include <QGraphicsScene>
#include <QString>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "Point.h"
#include "TextBox.h"
#include <vector>
#include <QKeyEvent>

//Include my constant variables
using namespace CONSTANTS;

/*
    Default Constructor for the GraphicsScene class.
    @detail This constructor calles the QGraphicsScene constructor, and sets the
    initial state to idle. It also calls the DrawIdle method to draw the idle
    evironment.
*/
GraphicsScene::GraphicsScene() : QGraphicsScene()
{
    DrawIdle(true);
    state = Idle;
}

/*
    Function to check if the event positon was on the axis.
    @detail This fuction checks the event position against the detection area for
    each of the Points on the axis. If it finds a match then it will set that point
    to be the current Point for other methods to use. Otherwise it will make sure
    that the current Point is null.
    @param QPointF position is the event position
    @return bool indicates if the event was in a relevant position
*/
bool GraphicsScene::InRegion(QPointF& position)
{
    for(auto& p : points)
    {
        //if in x and y detection zone
        if(p->inRegion(position))
        {
            selected_point = p;
            return true;
        }
    }
    //return false and reset selected_point if it wasn't in any of the zones
    selected_point = NULL;
    return false;
}

/*
    Function to check if the event positon was in the label.
    @detail This fuction checks the event position against the detection area for
    the label that is drawn during the Input and Output states.
    @param QPointF position is the event position
    @return bool indicates if the event was in a relevant position
*/
bool GraphicsScene::InLabelRegion(QPointF& position)
{
    if((position.rx() >= selected_point->getCenter().rx()-LABEL_WIDTH/2                    && position.rx() <= selected_point->getCenter().rx()+LABEL_WIDTH/2-2*METRICS.width("X"))
     &&(position.ry() >= selected_point->getCenter().ry()-LABEL_HEIGHT-LABEL_HEIGHT_MARGIN && position.ry() <= selected_point->getCenter().ry()-LABEL_HEIGHT_MARGIN))
        return true;
    return false;
}

/*
    Function to check if the event positon was in 'close' section of the label.
    @detail This fuction checks the event position against the detection area for
    the close part of the label.
    @param QPointF position is the event position
    @return bool indicates if the event was in a relevant position
*/
bool GraphicsScene::InCloseRegion(QPointF& position)
{
    if(   (position.rx() >= selected_point->getCenter().rx()+LABEL_WIDTH/2-2*METRICS.width("X") && position.rx() <= selected_point->getCenter().rx()+LABEL_WIDTH/2)
       && (position.ry() >= selected_point->getCenter().ry()-LABEL_HEIGHT-LABEL_HEIGHT_MARGIN   && position.ry() <= selected_point->getCenter().ry()-LABEL_HEIGHT_MARGIN))
        return true;
    return false;
}

/*
    Function to draw the basic features relevant for all states.
    @detail This fuction is responsible for beginning the drawing process for all
    the states. It draws the axis line, axis markers, and axis labels. It also has
    the ability to initialize the std::vector<Points*> when the program draws
    for the first time.
    @param bool init By default init will be false, assuming that it isn't the first call,
    but you set it to true when you call Draw in the constructor of GraphicsScene.
    @return QPainter* to allow additions to the current painter in other Draw fucntions
*/
QPainter* GraphicsScene::Draw(bool init=false)
{
    //Clear the scene if this isn't the initialization
    if(!init)
        clear();

    //Create a QPixmap to paint on
    canvas = new QPixmap(CANVAS_WIDTH,CANVAS_HEIGHT);
    QPainter* paint = new QPainter(canvas);

    //Initialize the label counter at the minimum value of the axis
    int name_counter = AXIS_MIN;

    //Painter setting for the default color of black
    paint->setPen(QColor(0,0,0));

    //Draw the axis line
    paint->drawLine(PADDING_LEFT,PADDING_TOP,PADDING_LEFT+MARKER_INTERVAL*TOTAL_MARKERS,PADDING_TOP);

    //If initializing you need to create all of the point objects
    if(init)
    {
        for(int i=0; i <TOTAL_MARKERS; i++)
        {
            //Draw the interval dashes on the axis
            paint->drawLine(PADDING_LEFT+AXIS_TAIL+MARKER_INTERVAL*i, PADDING_TOP, PADDING_LEFT+AXIS_TAIL+MARKER_INTERVAL*i, PADDING_TOP+3);
            //Draw the axis interval label
            QString label = QString::number(name_counter++);
            paint->drawText(PADDING_LEFT+AXIS_TAIL+MARKER_INTERVAL*i-METRICS.width(label)/2, PADDING_TOP+LABEL_SPACER, label);

            //Add point reference to the scenes std::vector passing in the Points location
            points.push_back(new Point(PADDING_LEFT+AXIS_TAIL+MARKER_INTERVAL*i, PADDING_TOP));
        }
    }
    else
    {
        //If this is a standard Draw call use the data stored in the points vector
        for(auto& p : points)
        {
            //Draw the interval dashes on the axis
            paint->drawLine(p->getCenter().rx(), p->getCenter().ry(), p->getCenter().rx(), p->getCenter().ry()+3);
            //Draw the axis interval label
            QString label = QString::number(name_counter++);
            paint->drawText(p->getCenter().rx()-METRICS.width(label)/2, p->getCenter().ry()+LABEL_SPACER, label);

            //Check if that Point already has data so we can indicate that with a point
            if(!p->isEmpty())
            {
                //Draw an ellipse that is filled
                paint->setBrush(Qt::SolidPattern);
                paint->drawEllipse(p->getCenter(), 3, 3);
            }
        }
    }
    //Return the paint pointer to allow additional draw calls by other functions
    return paint;
}

/*
    Function to draw the Idle state.
    @detail This fuction just calls the finishing lines of Draw() and DrawTitle() to draw
    the axis and the title text.
    @param bool init the parameter to pass into Draw(bool)
*/
void GraphicsScene::DrawIdle(bool init=false)
{
    Draw(init);

    //Just add the canvas used in Draw() to the scene since I don't
    //add any additional features in the Idle state
    addPixmap(*canvas);

    //Draw the title text
    DrawTitle();
}

/*
    Function to draw the label.
    @detail This fuction draws the polygon shape label on the paint object generated
    in Draw().
    @param QPainter* paint is the returned QPainter from Draw()
*/
void GraphicsScene::DrawPolygonLabel(QPainter* paint)
{
    //Cache the selected_point x and y values to make the code easier to read
    int c_x = selected_point->getCenter().rx();
    int c_y = selected_point->getCenter().ry();

    //Create a polygon that will be the label shape
    QPolygonF label;
    label.append(QPointF(c_x-LABEL_WIDTH/2,c_y-LABEL_HEIGHT-LABEL_HEIGHT_MARGIN));
    label.append(QPointF(c_x+LABEL_WIDTH/2,c_y-LABEL_HEIGHT-LABEL_HEIGHT_MARGIN));
    label.append(QPointF(c_x+LABEL_WIDTH/2,c_y-LABEL_HEIGHT_MARGIN));
    label.append(QPointF(c_x+5,c_y-LABEL_HEIGHT_MARGIN));
    label.append(QPointF(c_x,c_y-LABEL_HEIGHT_MARGIN+5));
    label.append(QPointF(c_x-5,c_y-LABEL_HEIGHT_MARGIN));
    label.append(QPointF(c_x-LABEL_WIDTH/2,c_y-LABEL_HEIGHT_MARGIN));
    label.append(QPointF(c_x-LABEL_WIDTH/2,c_y-LABEL_HEIGHT-LABEL_HEIGHT_MARGIN));
    //Adjust the painter settings to change the fill color of the label polygon
    paint->setBrush(QBrush(QColor(255,255,255),Qt::SolidPattern));
    paint->setOpacity(0.8);
    //add the polygon
    paint->drawPolygon(label);
}

/*
    Function to draw the TextBox.
    @detail This fuction draws the TextBox that is positioned in the label.
    @param bool option to indicate whether the TextBox should be edittable.
*/
void GraphicsScene::DrawTextBox(bool option)
{
    //save the curretn TextBox
    current_textBox = new TextBox();
    //If edittable then set default text otherwise set saved text
    if(option)
        current_textBox->setPlainText("Edit me");
    else
        current_textBox->setPlainText(selected_point->getData());
    //Basic settings
    current_textBox->setTextWidth(LABEL_WIDTH-20);
    current_textBox->setPos((selected_point->getCenter().rx()-LABEL_WIDTH/2),(selected_point->getCenter().ry()-LABEL_HEIGHT-LABEL_HEIGHT_MARGIN));
    //Set whether you want the text to be edittable or not
    current_textBox->SetTextInteraction(option);
    //Add the TextBox to the scene
    addItem(current_textBox);
}

/*
    Function to draw the close icon.
    @detail This fuction draws the red 'X' that is used to delete the Point.
*/
void GraphicsScene::DrawClose()
{
    //Create a "Delete Button" using a red 'X'
    QGraphicsTextItem* close = new QGraphicsTextItem("X");
    //set color of the text to red
    close->setDefaultTextColor(QColor(247,40,12));
    //position the 'X' in the right side of the polygon label
    close->setPos(selected_point->getCenter().rx()+LABEL_WIDTH/2-2*METRICS.width("X"),selected_point->getCenter().ry()-LABEL_HEIGHT-LABEL_HEIGHT_MARGIN);
    addItem(close);
}

/*
    Function to draw the save icon.
    @detail This fuction draws the green 'O' that can be used to save the Point.
*/
void GraphicsScene::DrawSave()
{
    //Create a "Save Button" using a green 'O'
    QGraphicsTextItem* save = new QGraphicsTextItem("O");
    //set color of the text to green
    save->setDefaultTextColor(QColor(52,201,26));
    //position the 'O' in the right side of the polygon label
    save->setPos(selected_point->getCenter().rx()+LABEL_WIDTH/2-2*METRICS.width("O"),selected_point->getCenter().ry()-LABEL_HEIGHT-LABEL_HEIGHT_MARGIN);
    addItem(save);
}

/*
    Function to draw the title text.
    @detail This fuction draws the welcome and help text.
*/
void GraphicsScene::DrawTitle()
{
    //Add the title text
    QGraphicsTextItem* title = new QGraphicsTextItem(TITLE);
    title->setPos((CANVAS_WIDTH-METRICS.width(TITLE))/2,PADDING_TITLE);
    addItem(title);

    //Add the info text
    QGraphicsTextItem* info = new QGraphicsTextItem(INFO);
    info->setPos((CANVAS_WIDTH-METRICS.width(INFO))/2,PADDING_TITLE+METRICS.height());
    addItem(info);
}

/*
    Function to draw the Input state.
    @detail This fuction calls Draw to set up the canvas and then adds the label,
    TextBox, and title text.
*/
void GraphicsScene::DrawInput()
{
    //First draw the axis and locally store the QPainter
    QPainter* temp_paint = Draw();

    //Draw the Label polygon
    DrawPolygonLabel(temp_paint);

    //add the canvas to the scene
    addPixmap(*canvas);

    //Create a TextBox item that will allow the user to edit the text stored at that point
    DrawTextBox(true);

    //Create a Save button
    DrawSave();

    //Add the title text
    DrawTitle();
}

/*
    Function to draw the Output state.
    @detail This fuction calls Draw to set up the canvas and then adds the label,
    TextBox, and title text.
*/
void GraphicsScene::DrawOutput()
{
    //First draw the axis and locally store the QPainter
    QPainter* temp_paint = Draw();

    //Draw the Label polygon
    DrawPolygonLabel(temp_paint);

    //add the canvas to the scene
    addPixmap(*canvas);

    //Create a TextBox item that won't allow the user to edit the text stored at that point
    DrawTextBox(false);

    //Create a "Delete Button" using a red 'X'
    DrawClose();

    //Add the title text
    DrawTitle();
}

/*
    Function overload of mousePressEvent to control the apps behavior and state.
    @detail This fuction uses the position of the mouse press and the current state
    to decide how to react.
    @param QGraphicsSceneMouseEvent* event the event
*/
void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event){
    switch(state)
    {
        case Idle:
            //If clicked in a Point's detection region on the axis
            if(InRegion(event->scenePos()))
            {
                //Check if that Point has any data
                if(selected_point->isEmpty())
                {
                    //If it is an empty point then change the state to Input
                    state = Input;
                    DrawInput();
                }
                else
                {
                    //If the point already has data then just show the Output state
                    state = Output;
                    DrawOutput();
                }
            }
            break;
        case Input:
            //If they click out of the input region then revert to the Idle state
            if(!InLabelRegion(event->scenePos()))
            {
                //Disable Editing
                current_textBox->SetTextInteraction(false);

                //save the text value in the Point
                selected_point->updateData(QString(current_textBox->toPlainText()));

                //Display Output
                state = Output;
                DrawOutput();
            }
            else{
                //If they click within the label region then double check that they are able to edit the text
                if(current_textBox->textInteractionFlags() == Qt::NoTextInteraction)
                    current_textBox->SetTextInteraction(true);
            }
            break;
        case Output:
            //If click event happens in current region change state to Input
            if(InLabelRegion(event->scenePos()))
            {
                state = Input;
                DrawInput();
            }
            //If click event happens in the close region
            else if(InCloseRegion(event->scenePos()))
            {
                //delete that points data if clicked in the close region of the label
                selected_point->updateData("");
                state = Idle;
                DrawIdle();
            }

            //Check if they have clicked on another point
            else if(InRegion(event->scenePos()))
            {
                //If an empty point then go to the Input state
                if(selected_point->isEmpty())
                {
                    state = Input;
                    DrawInput();
                }
                //If it's not empty then display the Output state
                else
                {
                    state = Output;
                    DrawOutput();
                }
            }

            //Otherwise revert to Idle
            else
            {
                state = Idle;
                DrawIdle();
            }
            break;
    }
}
