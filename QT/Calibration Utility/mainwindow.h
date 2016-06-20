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
     QAction*           gotoDocs,*about;
     QMessageBox*       aboutDialog;
     QMenu*             fileMenu,*helpMenu;
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
    void openDocs();
};

#endif // MAINWINDOW_H
