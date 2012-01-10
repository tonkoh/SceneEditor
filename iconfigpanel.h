#ifndef ICONFIGPANEL_H
#define ICONFIGPANEL_H

#include <QWidget>
#include <QGraphicsItem>

class IConfigPanel : public QWidget
{
    Q_OBJECT
public:
    IConfigPanel(QWidget *parent=0):QWidget(parent){}
    //disconnects all target signals from all config panel slots and viceversa.
    virtual QGraphicsItem* getTarget() = 0;

public slots:
    virtual void disconnectTarget() = 0;
};

#endif // ICONFIGPANEL_H
