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

#include "TextBox.h"
#include <QGraphicsTextItem>
#include <QTextCursor>
#include <QStyleOptionGraphicsItem>
#include <QKeyEvent>

/*
    Default Constructor for the TextBox class.
    @detail This constructor calls the QGraphicsTextItem constructor and then sets
    the object to be selectable, focusable, but no text interaction.
*/
TextBox::TextBox() : QGraphicsTextItem()
{
    setFlags(ItemIsSelectable | ItemIsFocusable);
    setTextInteractionFlags(Qt::NoTextInteraction);
}

/*
    Function to change whether the text can be editted.
    @detail This function allows you to enable/disable the TextBox's ability to
    be editted.
    @param bool edit tell the function if you want to edit(true) or not(false)
*/
void TextBox::SetTextInteraction(bool edit)
{
    //If the text isn't edittable and you want it to be
    if(edit && textInteractionFlags() == Qt::NoTextInteraction)
    {
        //Allow the text to be interacted with and focus on the text
        setTextInteractionFlags(Qt::TextEditorInteraction);
        setFocus(Qt::MouseFocusReason);
        setSelected(true);

        //By default select all of the text to allow for easy editting
        QTextCursor cursor = textCursor();
        cursor.select(QTextCursor::Document);
        setTextCursor(cursor);
    }
    //If the text is edittable and you don't want it to be
    else if(!edit && textInteractionFlags() == Qt::TextEditorInteraction)
    {
        //Disable text interaction, unselect whatever text is selected, and unfocus on the text
        setTextInteractionFlags(Qt::NoTextInteraction);
        QTextCursor cursor = this->textCursor();
        cursor.clearSelection();
        this->setTextCursor(cursor);
        clearFocus();
    }
}

/*
    Function to modify some of the default graphical style features of QGraphicsTextItem.
    @detail In a regular QGraphicsTextItem, it draws a dashed frame around the item when
    you are editting the text. This function modifies the style settings of the
    QGraphicsTextItem before painting it so that the ugly dashed frame isn't drawn.
    @param QPainter* p the QPainter drawing the item
    @param const QStyleOptionGraphicsItem* the style options for the paint call
    @param QWidget* the object being drawn on
*/
void TextBox::paint(QPainter* p, const QStyleOptionGraphicsItem* s, QWidget* w)
{
    //Create a local copy of the style options to edit, and then remove the selected
    //and hasfocus options.
    QStyleOptionGraphicsItem so(*s);
    so.state &= ~QStyle::State_Selected;
    so.state &= ~QStyle::State_HasFocus;
    //pass the modified variable and the two unmodified parameters into the
    //standard paint function
    QGraphicsTextItem::paint(p,&so,w);
}

/*
    Function to manage user input.
    @detail By default the TextBox doesn't control what is inputted, so it can overflow
    the surrounding label shape. To prevent this from happening, I have limited the size
    of the QString inputted to be 10 chars, since 10 of the largest chars in the default font
    will fill the TextBox's width. I also ignore the input if the user tries to input a return key
    or a tab key. This solution makes the TextBox behave like a single line item
    @param QEvent* the event to be check
    @return bool used to ignore the events
*/
bool TextBox::sceneEvent(QEvent* event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent* ke = static_cast<QKeyEvent*>(event);

        //Prevent input of \n and \t
        if(ke->key() == Qt::Key_Return)
            return true;
        if(ke->key() == Qt::Key_Tab)
            return true;

        //Prevent input of strings longer than 10 chars
        //Allow the use of backspace so that you can delete chars after hitting
        //the limit of 10
        if(toPlainText().size() >= 10 && ke->key() != Qt::Key_Backspace)
            return true;

    }
    //Return the sceneEvent to handle any other events that I don't want to ignore
    return QGraphicsTextItem::sceneEvent(event);
}
