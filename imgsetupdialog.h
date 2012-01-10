#ifndef IMGSETUPDIALOG_H
#define IMGSETUPDIALOG_H

#include <QDialog>

class QLineEdit;
class QLabel;
class QPushButton;
class QPixmap;
class ObjNameVerifier;

class ImgSetupDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ImgSetupDialog(ObjNameVerifier *v, QWidget *parent = 0);
    ~ImgSetupDialog();

signals:

private slots:
    void loadImageSrc();
    void onOkBtnClicked();
public:
    ObjNameVerifier *verifier;
    QPixmap   *image;
    QLineEdit *nameEdit;
    QLineEdit *layerEdit;
    //label used to show image
    QLabel *imgLabel;
    //ok button
    QPushButton *okBtn;
};

#endif // IMGSETUPDIALOG_H
