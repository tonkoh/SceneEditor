#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imgpanel.h"
#include <QFileDialog>
#include <QMessageBox>
#include "sceneconfdialog.h"
#include "imgsetupdialog.h"
#include <QLineEdit>
#include <QGraphicsSimpleTextItem>
#include <QVBoxLayout>
#include <QTextStream>
#include <QLabel>
#include <QWidget>
#include "objnameverifier.h"
#include <QXmlStreamWriter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    view = new QGraphicsView;
//    view->setBackgroundBrush(Qt::green);

    scene = 0;
    sceneBgRect = 0;
//    scene = new QGraphicsScene();
//    scene->setSceneRect(0,0,300,200);
//    scene->addRect(0,0,300,200,QPen(QColor("gray")),QBrush(QColor("gray")));
//    view->setScene(scene);

    confWidget = new QStackedWidget; //container for widgets used to set object's properties.

    //create widget applied to config widget when the scene has no focus on any objects.
    emptyPnl = new QWidget();
    QLabel *emptyLbl = new QLabel("Config Panel for objects");
    QVBoxLayout *vbl = new QVBoxLayout;
    vbl->addWidget(emptyLbl);
    emptyPnl->setLayout(vbl);
    confWidget->addWidget(emptyPnl);
    confWidget->setCurrentWidget(emptyPnl);

    //init config panels
    ObjNameVerifier * v = new ObjNameVerifier(&mapObjects);
    iPanel = new ImgPanel(v);
    confWidget->addWidget(iPanel);

    splitter = new QSplitter;
    splitter->addWidget(view);
    splitter->addWidget(confWidget);
    setCentralWidget(splitter);

    initMenu();
    initAction();
    setupConnection();
}

MainWindow::~MainWindow()
{
    mapObjects.clear();
    delete ui;
    delete scene;
    scene = 0;
    delete view;
    delete confWidget;
//    delete iPanel;
    delete splitter;

    delete actSave;
    delete actNewFile;
    delete actOpenFile;
    delete actCloseApp;
    delete actCrtImage; //action of creating Image
    delete actDelObj;
}

void MainWindow::initAction()
{
    actNewFile = menuFile->addAction(tr("New Scene"));
    actSave = menuFile->addAction(tr("Save Scene"));
    actOpenFile = menuFile->addAction(tr("Open File"));
    actCloseApp = menuFile->addAction(tr("Close App"));

    actCrtImage = menuCreate->addAction("Image");
    actDelObj = menuObject->addAction(tr("Delete Object"));
    actDelObj->setShortcut(tr("Delete"));

}

void MainWindow::initMenu()
{
    menuFile = menuBar()->addMenu(tr("&File"));
    menuObject = menuBar()->addMenu(tr("&Object"));
    menuCreate = menuObject->addMenu(tr("Create"));
}

void MainWindow::setupConnection()
{
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(closeApp()));

    connect(actNewFile, SIGNAL(triggered()), this, SLOT(createNewScene()));
    connect(actCrtImage, SIGNAL(triggered()), this, SLOT(addImage()));
    connect(actCloseApp, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(actDelObj,SIGNAL(triggered()),this,SLOT(delSelectedObject()));

    //connections for confing panels
    connect(iPanel, SIGNAL(nameChanged(QString,QString)), this, SLOT(onObjectNameChanged(QString,QString)));
}

bool MainWindow::hasObject(QString objName)
{
    return mapObjects.contains(objName);
}

void MainWindow::writeSceneOutToFile()
{
    QString fileName = curFileName;
    if(fileName.isEmpty()){
        fileName = QFileDialog::getSaveFileName(this);
        if (fileName.isEmpty())
            return;
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    out << "not implemented yet";

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    curFileName = fileName;
    statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::createNewScene()
{
    //If there is editing scene, show dialog to check if a user save it.
    if(scene){
        //save the existing file
        QMessageBox mbox(this);
        mbox.setText("Do you want to save the editing scene?");
        mbox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                                QMessageBox::Cancel);
        mbox.setDefaultButton(QMessageBox::Save);
        int ret = mbox.exec();
        if(ret == QMessageBox::Cancel)
            return;
        if(ret == QMessageBox::Save)
            writeSceneOutToFile();

        disconnect(scene,0,this,0);
        delete scene;
        scene = 0;
    }

    /*create new scene*/
    //show dialog to get necessary infomation to create a scene.
    SceneConfDialog sDialog(this);
    int ret = sDialog.exec();
    if(ret == QDialog::Accepted){
        if(sDialog._nameEdit->text().isEmpty()){
            sceneName.append("sceneA");
        }else{
            sceneName.clear();
            sceneName.append(sDialog._nameEdit->text());
        }

        numLayer = sDialog._layerSizeEdit->text().toInt();
        qreal sceneWidth = sDialog._widthEdit->text().toDouble();
        qreal sceneHeight = sDialog._heightEdit->text().toDouble();
        scene = new QGraphicsScene();
        scene->setSceneRect(0,0,sceneWidth,sceneHeight);
        scene->setBackgroundBrush(Qt::gray);
        if(sceneBgRect == 0){
            sceneBgRect = new QGraphicsRectItem(0,0,sceneWidth,sceneHeight);
            sceneBgRect->setPen(QPen(Qt::white));
            sceneBgRect->setBrush(QBrush(Qt::white));
        }else
            sceneBgRect->setRect(0,0,sceneWidth,sceneHeight);
        scene->addItem(sceneBgRect);

        view->setScene(scene);
        connect(scene,SIGNAL(selectionChanged()),this,SLOT(onSelectedItemChanged()));
    }
}

void MainWindow::closeScene()
{
    saveScene();
    if(scene != 0){
        disconnect(scene,0,this,0);

        //[todo] Add proc that disconnect all scene's slot from any signals.

        delete scene;
        scene = 0;
    }
}

void MainWindow::closeApp()
{
    saveScene(); // show a dialog to ask user if he want to save a editing scene.

    qApp->exit(0);

}

/******************************************************/
//save currently editing scene if there is its instance.
/******************************************************/

void MainWindow::saveScene()
{
    if(scene == 0)
        return;

    //save the existing file
    QMessageBox mbox;
    mbox.setText("Do you want to save the editing scene?");
    mbox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                            QMessageBox::Cancel);
    mbox.setDefaultButton(QMessageBox::Save);
    int ret = mbox.exec();
    if(ret == QMessageBox::Cancel)
        return;
    if(ret == QMessageBox::Save)
        writeSceneOutToFile();
}

void MainWindow::disconnectConfPanel(IConfigPanel *confPanel)
{
    if(confPanel == 0){
        //case disconnects all connections between the config panel
        //specified by @confPanel and the current target object.
        confPanel->disconnectTarget();
    }else{
        //case disconnects all connections of all config panels
    }
}

void MainWindow::delSelectedObject()
{
    //get currently selected scene items
    QList<QGraphicsItem*> items = scene->selectedItems();

    if(items.isEmpty())
        return;

    //remove item from scene and map of object.
    QGraphicsItem *item = items[0];
    IConfigPanel *cPanel = qobject_cast<IConfigPanel*>(confWidget->currentWidget());
    if(cPanel){
        cPanel->disconnectTarget();
        confWidget->setCurrentWidget(emptyPnl);
    }
    scene->removeItem(item);
    MyGraphicsObject *obj = qgraphicsitem_cast<MyGraphicsObject*>(item);
    if(obj)
        mapObjects.remove(obj->objectName());

    delete item;
}

void MainWindow::addImage()
{
    if(scene == 0){
        QMessageBox mb;
        mb.setText("Scene is required to add new object.");
        mb.exec();
        return;
    }

    ObjNameVerifier *v = new ObjNameVerifier(&mapObjects);
    ImgSetupDialog dialog(v , this);
    int ret = dialog.exec();
    if(ret != QDialog::Accepted)
        return;

    QString objName = dialog.nameEdit->text();
    if(mapObjects.contains(objName)==false){
        //case a new Image is to be created.
        Image *img = new Image(dialog.image);
        img->setZValue(dialog.layerEdit->text().toDouble());
        img->setObjectName(objName);
        mapObjects.insert(objName, img);
        QPointF sceneTL = sceneTopLeft();
        img->setPos(sceneTL.x(), sceneTL.y());
        img->setFlag(QGraphicsItem::ItemIsSelectable, true);
        img->setFlag(QGraphicsItem::ItemIsMovable, true);
        scene->clearSelection();
        img->setSelected(true);
        scene->addItem(img);
    }else{
        QMessageBox msgBox;
        msgBox.setText(objName+" is already registered as an object name.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
}

void MainWindow::onObjectNameChanged(QString newName, QString oldName)
{

    QGraphicsItem *item = mapObjects.value(oldName, 0);

    if(item){
        mapObjects.remove(oldName);
        mapObjects.insert(newName, item);
    }
}

void MainWindow::onSelectedItemChanged()
{
    //update a current config panel for the selected item.
    QList<QGraphicsItem *> items = scene->selectedItems();
    if(items.empty()){
        if(confWidget->currentIndex() != 0){
            confWidget->setCurrentWidget(emptyPnl);
        }
        return;
    }
    QGraphicsItem *selected = items[0];
    if(Image *img = qgraphicsitem_cast<Image *>(selected)){
        if(confWidget->currentIndex() != 1)
            confWidget->setCurrentWidget(iPanel);
        iPanel->setTargetImg(img);
        return;
    }
}

void MainWindow::resizeScene(qreal width, qreal height)
{
    scene->setSceneRect(0,0,width,height);
}

//Reusable and useful methods
QPointF MainWindow::sceneTopLeft() const
{
    QPointF sceneTopleft = view->mapToScene(0,0);
    if(sceneTopleft.x() < 0) sceneTopleft.setX(0);
    if(sceneTopleft.y() < 0) sceneTopleft.setY(0);

    return QPointF(sceneTopleft);
}

void MainWindow::writeSceneData(QXmlStreamWriter &writer) const
{
    writer.writeStartElement();

}

void MainWindow::writeObjectsData(QXmlStreamWriter &writer) const
{

}
