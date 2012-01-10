#include "sceneconfdialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QIntValidator>
#include <QDoubleValidator>

SceneConfDialog::SceneConfDialog(QWidget *parent) :
    QDialog(parent)
{
    QLabel *nameLbl = new QLabel(tr("Scene Name"));
    _nameEdit = new QLineEdit();
    _nameEdit->setInputMask("NNNNNNNNNNNNNNNN");

    QLabel *widthLbl = new QLabel(tr("Scene width"));
    _widthEdit = new QLineEdit();
    _widthEdit->setValidator(new QDoubleValidator(0.0, 10000.0, 1000, _widthEdit));
//    _widthEdit->setInputMask("9");
    _widthEdit->setText("400");

    QLabel *heightLbl = new QLabel(tr("Scene height"));
    _heightEdit = new QLineEdit();
    _heightEdit->setValidator(new QDoubleValidator(0.0,10000.0,1000,_heightEdit));
//    _heightEdit->setInputMask("9");
    _heightEdit->setText("300");

    QLabel *layerLbl = new QLabel(tr("Layer size"));
    _layerSizeEdit = new QLineEdit();
    _layerSizeEdit->setValidator(new QIntValidator(0,100,_layerSizeEdit));
    _layerSizeEdit->setText("1");
//    _layerSizeEdit->setInputMask("9");

    _okBtn = new QPushButton("OK");
    _cancelBtn = new QPushButton("Cancel");
    connect(_okBtn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(_cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));

    QGridLayout *gl = new QGridLayout;
    gl->addWidget(nameLbl, 0,0);
    gl->addWidget(_nameEdit,0,1);
    gl->addWidget(widthLbl,1,0);
    gl->addWidget(_widthEdit,1,1);
    gl->addWidget(heightLbl,2,0);
    gl->addWidget(_heightEdit,2,1);
    gl->addWidget(layerLbl,3,0);
    gl->addWidget(_layerSizeEdit,3,1);
    gl->addWidget(_okBtn,4,0);
    gl->addWidget(_cancelBtn,4,1);

    setLayout(gl);
}

SceneConfDialog::~SceneConfDialog()
{
    delete _nameEdit;
    delete _widthEdit;
    delete _heightEdit;
    delete _layerSizeEdit;

    delete _okBtn;
    delete _cancelBtn;
}
