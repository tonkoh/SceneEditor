#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStackedWidget>
#include <QSplitter>
#include "image.h"
#include <QMap>
#include "iconfigpanel.h"

class ImgPanel;
class QWidget;
class QGraphicsRectItem;
class QXmlStreamWriter;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool hasObject(QString objName);

private:
    void initAction();
    void initMenu();
    void setupConnection();

    QPointF sceneTopLeft() const;
    void writeSceneData(QXmlStreamWriter &writer) const;
    void writeObjectsData(QXmlStreamWriter &writer) const;

public slots:
    void writeSceneOutToFile();
    void createNewScene();
    void closeScene();
    void closeApp();
    void saveScene();
    void disconnectConfPanel(IConfigPanel *confPanel);
    void delSelectedObject();

    void addImage();
    void onObjectNameChanged(QString newName, QString oldName);
    void onSelectedItemChanged();
    void resizeScene(qreal width, qreal height);

private:
    Ui::MainWindow *ui;

    QString             sceneName;
    QString             curFileName; //write scene data out to a file in xml format.
    int                 numLayer; //the number of a layer of a current scene
    QMap<QString, QGraphicsItem*> mapObjects;


    QGraphicsView       *view;
    QGraphicsScene      *scene;
    QGraphicsRectItem   *sceneBgRect; //Background rectangle for the scene.
    QStackedWidget      *confWidget; //container for widgets used to set object's properties.
    QSplitter           *splitter;

    //widgets for setting each GraphicsItem.
    QWidget             *emptyPnl;
    ImgPanel            *iPanel;

    //Actions
    QAction             *actSave;
    QAction             *actNewFile;
    QAction             *actOpenFile;
    QAction             *actCloseApp;
    QAction             *actCrtImage; //action of creating Image
    QAction             *actDelObj;

    //Menu
    QMenu               *menuFile;

    QMenu               *menuObject;
    QMenu               *menuCreate;
    QMenu               *menuDelete;


};

#endif // MAINWINDOW_H
