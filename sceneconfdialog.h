#ifndef SCENECONFDIALOG_H
#define SCENECONFDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class SceneConfDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SceneConfDialog(QWidget *parent = 0);
    ~SceneConfDialog();

signals:

public slots:

public:
    QLineEdit *_nameEdit;
    QLineEdit *_widthEdit;
    QLineEdit *_heightEdit;
    QLineEdit *_layerSizeEdit;

    QPushButton *_okBtn;
    QPushButton *_cancelBtn;

};

#endif // SCENECONFDIALOG_H
