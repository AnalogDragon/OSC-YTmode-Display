#include "clickedcombobox.h"

clickedCombobox::clickedCombobox(QWidget *parent):QComboBox(parent)
{

}

clickedCombobox::~clickedCombobox()
{

}

void clickedCombobox::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked(true);
    }
    QComboBox::mousePressEvent(event);
}
