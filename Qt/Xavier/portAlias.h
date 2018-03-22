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

#ifndef DIALOGS_H
#define DIALOGS_H

#include <QWidget>
#include <QtWidgets>
#include <QSerialPortInfo>

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QtSerialPort/QSerialPort>
#include <QFileDialog>
#include <QMap>
#include <qtimer.h>


class usbLabelDialog : public QGroupBox
{
Q_OBJECT
public:
    usbLabelDialog(QWidget *parent = 0);

    QGridLayout*            settingsLayout;

    QGroupBox*              portEditBox;
    QGridLayout*            portEditLayout;
    QLabel*                 portnameLabel, *aliasLabel,*currentPorts, *newDescription;
    QListWidget*            aliasWidget;
    QStringList             aliasStringList;
    QComboBox*              portDropdown;
    QPushButton*            addAlias_btn,*rmvAlias_btn;
    QLineEdit*              newAlias;

    QDialog*                editLabelDialog;
    QGroupBox*              buttonBox;
    QGridLayout*            editLayout,*buttonLayout;
    QLineEdit*              editText;
    QLabel*                 newLabel;
    QPushButton*            changeLabel_btn,*cancelChange_btn;

    QPushButton*            okButton;

    QMap<QString, QString>  portLabelMap;

    QString                 usbTag;
    QStringList             aliasList;
public slots:
    void editLabel();
    void addAlias();
    void saveChanges();
    void open_USB_Label_dialog();

signals:
    void dialogClosed();
};


#endif // DIALOGS_H
