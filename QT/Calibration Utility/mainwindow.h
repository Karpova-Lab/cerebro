#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
     QAction*           gotoSettings,*openDir;
     QMenu*             fileMenu,*goMenu;
     QGroupBox*         chooseBox;
     QPushButton*       pickFile_btn;
     QGridLayout*       chooseLayout,*mainLayout;
     QPlainTextEdit*    codeTextBox;
     QLabel*            fileLabel,*slideLabel;
     QCheckBox*         showGraph;
     QSlider*           wantedLevel;

private slots:
    void chooseFile();
    void slideValueUpdate(int newVal);
};

#endif // MAINWINDOW_H
