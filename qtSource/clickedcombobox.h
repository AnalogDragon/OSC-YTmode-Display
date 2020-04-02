#ifndef CLICKEDCOMBOBOX_H
#define CLICKEDCOMBOBOX_H

#include <QComboBox>
#include <QMouseEvent>

class clickedCombobox : public QComboBox
{

    Q_OBJECT
public:
    explicit clickedCombobox(QWidget *parent);
    ~clickedCombobox();

protected:
    void mousePressEvent(QMouseEvent *event);

signals:
    void clicked(bool);
};

#endif // CLICKEDCOMBOBOX_H
