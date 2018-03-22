/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MIT License

Copyright (c) 2015-2018 Andy S. Lustig

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QWidget>
#include <QtWidgets>
#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <portAlias.h>


class settingsDialog : public QDialog
{
Q_OBJECT

public:
    settingsDialog(QWidget *parent = 0);

//    QDialog*                settingsDialog;
    QGridLayout*            settingsLayout;

    QGroupBox*              directoryBox;
    QGridLayout*            directoryLayout;
    QLabel*                 directoryLabel;
    QPushButton*            changeDir_btn;

    QGroupBox*              sessionListsBox;
    QGridLayout*            sessionListsLayout;
    QLabel*                 rigLabel, *ratLabel;
    QListWidget*            rigVals,*ratVals;
    QPushButton*            add1_btn,*add2_btn;
    QLineEdit*              newItem1,*newItem2;
    QPushButton*            rmv1_btn,*rmv2_btn;

    QGroupBox*              featuresBox;
    QGridLayout*            featuresLayout;
    QCheckBox*              pythonCheckbox,*mcubeCheckbox,*histogramCheckbox;
    bool                    pythonEnabled,mcubeEnabled,showHistogram;
    QStringList             rigList,ratList;
    QPushButton*            okButton;

    usbLabelDialog*         usbLabelDlog;



private:
    QString                 usbTag,usbDescription;

public slots:
    void myexec();
    void setPath();
    void removeItem();
    void addListItem();
    void openSettings();
    void saveChanges();

signals:
    void dialogClosed();
};
#endif // TESTDIALOG_H

