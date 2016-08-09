#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class DropButton;

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
     QLabel*            fileLabel,*slideLabel,*dropLabel;
     QCheckBox*         showGraph;
     QSlider*           wantedLevel;
     DropButton*          selectFile_btn;

private slots:
    void chooseFile();
    void getCalVals(QString calibrateDataPath);
    void slideValueUpdate(int newVal);
    void openDocs();
    void useDropped(const QMimeData *mimeData);
};

#endif // MAINWINDOW_H
