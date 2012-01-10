#include "imgpanel.h"
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QGroupBox>
#include <QGridLayout>
#include "image.h"
#include "objnameverifier.h"
#include <QMessageBox>

ImgPanel::ImgPanel(ObjNameVerifier const* v, QWidget *parent) :
    IConfigPanel(parent), verifier(v)
{
//    QGroupBox *group = new QGroupBox(tr("Setting for Image Object"));

    target = 0;
    QLabel *nameLabel = new QLabel(tr("Name:"));
    nameEdit = new QLineEdit;
    nameEdit->setMaxLength(16);
    QLabel *xposLabel = new QLabel(tr("X"));
    QDoubleValidator dv(-(1<<12), 1<<12, 1000, this);
    xposEdit = new QLineEdit;
    xposEdit->setValidator(&dv);
    QLabel *yposLabel = new QLabel(tr("Y"));
    yposEdit = new QLineEdit;
    yposEdit->setValidator(&dv);

    QLabel *angleLabel= new QLabel(tr("Angle"));
    angleEdit = new QLineEdit;
    angleEdit->setValidator(new QDoubleValidator(0.0, 360.0, 100, this));

    QLabel *scaleLabel = new QLabel(tr("Scale"));
    scaleEdit = new QLineEdit;
    scaleEdit->setValidator(new QDoubleValidator(-1000.0, 1000.0, 100, this));

    QLabel *layerLabel = new QLabel(tr("Layer"));
    layerEdit = new QLineEdit;
    layerEdit->setValidator(new QIntValidator(0, 10, this));

    QLabel *alphaLable = new QLabel(tr("Alpha"));
    alphaEdit = new QLineEdit;
    alphaEdit->setValidator(new QDoubleValidator(0.0, 1.0, 1000, this));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameEdit, 0, 1);
    layout->addWidget(xposLabel, 1, 0);
    layout->addWidget(xposEdit, 1, 1);
    layout->addWidget(yposLabel, 2, 0);
    layout->addWidget(yposEdit, 2, 1);
    layout->addWidget(angleLabel, 3, 0);
    layout->addWidget(angleEdit, 3, 1);
    layout->addWidget(scaleLabel, 4, 0);
    layout->addWidget(scaleEdit, 4, 1);
    layout->addWidget(layerLabel, 5, 0);
    layout->addWidget(layerEdit, 5, 1);
    layout->addWidget(alphaLable, 6, 0);
    layout->addWidget(alphaEdit, 6, 1);
    setLayout(layout);

    connect(nameEdit, SIGNAL(editingFinished()), this, SLOT(sendNameChangedSIG()));
    connect(xposEdit, SIGNAL(editingFinished()), this, SLOT(onXEditFinished()));
    connect(yposEdit, SIGNAL(editingFinished()), this, SLOT(onYEditFinished()));
    connect(scaleEdit, SIGNAL(editingFinished()), this, SLOT(onScaleEditFinished()));
    connect(angleEdit, SIGNAL(editingFinished()), this, SLOT(onAngleEditFinished()));
    connect(alphaEdit, SIGNAL(editingFinished()), this, SLOT(onAlphaEditFinished()));
    connect(layerEdit, SIGNAL(editingFinished()), this, SLOT(onLayerEditFinished()));
}

ImgPanel::~ImgPanel()
{
    target = 0;
    delete verifier;
    delete nameEdit;
    delete xposEdit;
    delete yposEdit;
    delete angleEdit;
    delete scaleEdit;
    delete layerEdit;
    delete alphaEdit;
}

Image* ImgPanel::targetImg() const
{
    return target;
}

void ImgPanel::disconnectTarget()
{
    if(target){
        disconnect(target,0,this,0);
        disconnect(this,0,target,0);
        target = 0;
    }
}

QGraphicsItem*  ImgPanel::getTarget()
{
    return target;
}

void ImgPanel::setTargetImg(Image *img)
{
    if(img == 0 || target == img)
        return;

    if(target){
        //disconnect all slots of target from all signals of this.
        disconnect(this, 0, target, 0);
        disconnect(target, 0, this, 0);
    }

    target = img;

    updateX();
    updateY();
    updateName();
    updateAngle();
    updateScale();
    updateLayer();
    updateAlpha();

//    sendNameChangedSIG();

    //connection
    connect(target, SIGNAL(xChanged()), this, SLOT(updateX()));
    connect(target, SIGNAL(yChanged()), this, SLOT(updateY()));
    connect(target, SIGNAL(zChanged()), this, SLOT(updateLayer()));
    connect(target, SIGNAL(rotationChanged()), this, SLOT(updateAngle()));
    connect(target, SIGNAL(scaleChanged()), this, SLOT(updateScale()));
    connect(target, SIGNAL(opacityChanged()), this, SLOT(updateAlpha()));

    connect(this, SIGNAL(xEditChanged(qreal)), target, SLOT(setXPos(qreal)));
    connect(this, SIGNAL(yEditChanged(qreal)), target, SLOT(setYPos(qreal)));
    connect(this, SIGNAL(scaleEditChanged(qreal)), target, SLOT(setScaleVal(qreal)));
    connect(this, SIGNAL(angleEditChanged(qreal)), target, SLOT(setAngle(qreal)));
    connect(this, SIGNAL(alphaEditChanged(qreal)), target, SLOT(setAlpha(qreal)));
    connect(this, SIGNAL(layerEditChanged(int)), target, SLOT(setZVal(int)));
}


/*
 * SLOTs
 */
void ImgPanel::onXEditFinished()
{
    emit xEditChanged(xposEdit->text().toDouble());
}

void ImgPanel::onYEditFinished()
{
    emit yEditChanged(yposEdit->text().toDouble());
}

void ImgPanel::onAngleEditFinished()
{
    emit angleEditChanged(angleEdit->text().toDouble());
}

void ImgPanel::onScaleEditFinished()
{
    emit scaleEditChanged(scaleEdit->text().toDouble());
}

void ImgPanel::onAlphaEditFinished()
{
    emit alphaEditChanged(alphaEdit->text().toDouble());
}

void ImgPanel::onLayerEditFinished()
{
    emit layerEditChanged(layerEdit->text().toInt());
}

void ImgPanel::updateX()
{
    xposEdit->setText(QString::number(target->x()));
}

void ImgPanel::updateY()
{
    yposEdit->setText(QString::number(target->y()));
}

void ImgPanel::updateName()
{

    nameEdit->setText(target->objectName());
    prevObjName.clear();
    prevObjName.append(nameEdit->text());
}

void ImgPanel::updateAngle()
{
    angleEdit->setText(QString::number(target->rotation()));
}

void ImgPanel::updateScale()
{
    scaleEdit->setText(QString::number(target->scale()));
}

void ImgPanel::updateLayer()
{
    layerEdit->setText(QString::number(target->zValue()));
}

void ImgPanel::updateAlpha()
{
    alphaEdit->setText(QString::number(target->opacity()));
}

void ImgPanel::sendNameChangedSIG()
{
    if(nameEdit->text() == prevObjName) //case name is not changed.
        return;

    if(verifier->verify(nameEdit->text())){
        emit nameChanged(nameEdit->text(), prevObjName);
        prevObjName.clear();
        prevObjName.append(nameEdit->text());
    }else{
        nameEdit->setText(prevObjName);
        //dialog
        QMessageBox mb;
        mb.setText(nameEdit->text()+" is already registered. Try another.");
        mb.setIcon(QMessageBox::Warning);
        mb.exec();
    }
}
