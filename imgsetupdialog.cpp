#include "imgsetupdialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIntValidator>
#include <QGridLayout>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include "objnameverifier.h"

ImgSetupDialog::ImgSetupDialog(ObjNameVerifier *v, QWidget *parent) :
    QDialog(parent), verifier(v), image(0)
{
    QLabel *nameLbl = new QLabel("name");
    nameEdit = new QLineEdit;
    nameEdit->setMaxLength(20);
    nameEdit->setInputMask("AAAAAAAAAAAAAAAAAAAA");

    imgLabel = new QLabel("Image source");
    QPushButton *loadBtn = new QPushButton("Load Image");

    QLabel *layerLbl = new QLabel("Layer index");
    layerEdit = new QLineEdit();
//    layerEdit->setInputMask("00"); //If inputMask and validator are set together, input no work well.
    layerEdit->setValidator(new QIntValidator(0,20,layerEdit));

    okBtn = new QPushButton("Create");
    okBtn->setEnabled(false);
    QPushButton *cancelBtn = new QPushButton("Cancel");

    QGridLayout *gl = new QGridLayout;
    gl->addWidget(nameLbl, 0,0);
    gl->addWidget(nameEdit, 0,1);
    gl->addWidget(imgLabel, 1,0);
    gl->addWidget(loadBtn, 1,1);
    gl->addWidget(layerLbl, 2,0);
    gl->addWidget(layerEdit, 2,1);
    gl->addWidget(okBtn, 3,0);
    gl->addWidget(cancelBtn, 3,1);

    setLayout(gl);

    //connection
    connect(loadBtn, SIGNAL(clicked()), this, SLOT(loadImageSrc()));
    connect(okBtn, SIGNAL(clicked()), this, SLOT(onOkBtnClicked()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
}

ImgSetupDialog::~ImgSetupDialog()
{
    delete verifier;
    delete image;
    delete nameEdit;
    delete layerEdit;
    delete imgLabel;
    delete okBtn;
}

void ImgSetupDialog::loadImageSrc()
{
    QString fName;
    fName = QFileDialog::getOpenFileName(this,
                                            tr("Open Image"),
                                            "",
                                            tr("Image Files (*.png *.bmp)"));

    if(fName.isEmpty()==false){//case fName has valid full path to a img file
        if(image)
            delete image;
        image = new QPixmap(fName);
        imgLabel->setPixmap(*image);
        okBtn->setEnabled(true);
    }

}

//******************************************************************
//Check if all lineEdit and Image label has valid data.
//If doesn't This method shows error message dialog alert a user to
//missing information for creating a Image instance.
//otherwise, calls QDialog::accept().
//******************************************************************
void ImgSetupDialog::onOkBtnClicked()
{
    //If all forms are not filled, show dialog alerting a user to that.
    if(nameEdit->text().isEmpty() ||
       layerEdit->text().isEmpty() ||
       imgLabel->pixmap() == 0)
    {
        QMessageBox mb;
        mb.setText("You must fill all forms");
        mb.exec();
        return;
    }

    if(verifier->verify(nameEdit->text()) == false){
        QMessageBox mb;
        mb.setText(nameEdit->text()+" is already registered. try another one.");
        mb.exec();
        return;
    }

    this->accept();


}
