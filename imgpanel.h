#ifndef IMGPANEL_H
#define IMGPANEL_H

#include <QWidget>
#include "iconfigpanel.h"

class QLabel;
class QLineEdit;
class Image;
class QGraphicsItem;
class ObjNameVerifier;

class ImgPanel : public IConfigPanel
{
    Q_OBJECT
public:
    explicit ImgPanel(ObjNameVerifier const* v, QWidget *parent = 0);
    ~ImgPanel();
    Image* targetImg() const;
    virtual QGraphicsItem*  getTarget();

private:
    Image *target;
    ObjNameVerifier const* verifier;
    QString prevObjName;
    QLineEdit *nameEdit;
    QLineEdit *xposEdit;
    QLineEdit *yposEdit;
    QLineEdit *angleEdit;
    QLineEdit *scaleEdit;
    QLineEdit *layerEdit;
    QLineEdit *alphaEdit;


signals:
    void nameChanged(QString newName, QString oldName) const;
    void xEditChanged(qreal x);
    void yEditChanged(qreal y);
    void scaleEditChanged(qreal scale);
    void angleEditChanged(qreal angle);
    void alphaEditChanged(qreal alpha);
    void layerEditChanged(int layer);

public slots:
    virtual void disconnectTarget();
    void setTargetImg(Image *img);
    void onXEditFinished();
    void onYEditFinished();
    void onAngleEditFinished();
    void onScaleEditFinished();
    void onAlphaEditFinished();
    void onLayerEditFinished();

    void updateX();
    void updateY();
    void updateName();
    void updateAngle();
    void updateScale();
    void updateLayer();
    void updateAlpha();
    void sendNameChangedSIG();
};

#endif // IMGPANEL_H
