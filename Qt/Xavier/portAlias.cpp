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

#include <QtGui>
#include "portAlias.h"
#include "QGridLayout"
#include <QtSerialPort/QSerialPortInfo>
#include <QFileDialog>
#include <QPaintDevice>

usbLabelDialog::usbLabelDialog(QWidget *parent)
    : QGroupBox(parent)
{
    #ifdef _WIN32   //--------Windows USB parse settings------------
        usbTag = "COM";
//        usbDescription = "USB Serial Port";
    #elif __APPLE__ //---------Mac USB parse settings---------------
        usbTag  = "cu.usbmodem";
//        usbDescription = "FT230X Basic UART";
    #endif

    //alias assignment box
    portEditLayout = new QGridLayout();
        aliasWidget = new QListWidget();
        aliasWidget->setMinimumHeight(150);
        aliasWidget->setMinimumWidth(300);
    portEditLayout->addWidget(aliasWidget,1,0);

    editLabelDialog = new QDialog();
        editLayout = new QGridLayout();
            newLabel = new QLabel("Rig # :");
        editLayout->addWidget(newLabel,0,0,Qt::AlignRight);
            editText = new QLineEdit();
        editLayout->addWidget(editText,0,1);
            buttonBox = new QGroupBox();
            buttonLayout = new QGridLayout();
                changeLabel_btn = new QPushButton("Change Label");
            buttonLayout->addWidget(changeLabel_btn,0,0);
                cancelChange_btn = new QPushButton("Cancel");
            buttonLayout->addWidget(cancelChange_btn,0,1);
            buttonBox->setLayout(buttonLayout);
        editLayout->addWidget(buttonBox,2,0,1,2);
    editLabelDialog->setMinimumWidth(300);
    editLabelDialog->setLayout(editLayout);
    editLabelDialog->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);//removes "?" from dialog box

    this->setLayout(portEditLayout);
    this->setTitle("Associate COM Port with Rig #");

    connect(aliasWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(editLabel()));
    connect(changeLabel_btn,SIGNAL(clicked()),this,SLOT(addAlias()));
    connect(cancelChange_btn,SIGNAL(clicked()),editLabelDialog,SLOT(close()));
}

void usbLabelDialog::open_USB_Label_dialog(){
    QSettings settings("Bobcat Engineering","CCS");
    settings.beginGroup("usbPorts");
    QStringList keys = settings.childKeys();
    foreach (QString key, keys) {
        portLabelMap[key] = settings.value(key).toString();
    }
    settings.endGroup();
    qDebug()<<"Keys and Values..."<<portLabelMap.keys()<<portLabelMap.values();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        if(info.manufacturer().contains("Cypress")){
            list << info.portName();
            if(portLabelMap.contains(info.portName())){
                aliasWidget->addItem("Rig " + portLabelMap.value(info.portName())+ " (" + info.portName() + ")");
                aliasList<<"Rig " + portLabelMap.value(info.portName())+ " (" + info.portName() + ")";
            }
            else{
                aliasWidget->addItem(info.portName());
            }
        }
    }
    aliasWidget->sortItems();
}

void usbLabelDialog::editLabel(){
    //isolate the COMXX part of the port name
    QString COMbeingEdited = aliasWidget->currentItem()->text();
    if(COMbeingEdited.contains("(")){
        editText->setText(COMbeingEdited.left(COMbeingEdited.indexOf(" (")).mid(4));
        COMbeingEdited.remove(0,COMbeingEdited.indexOf("("+usbTag)+1);
        COMbeingEdited.remove(QChar (')'));

    }
    else{
        editText->clear();
    }
    editText->setFocus();
    editLabelDialog->setWindowTitle("Edit "+ COMbeingEdited + " label");
    editLabelDialog->exec();
}

void usbLabelDialog::addAlias()
{
    QString tempCOM;
    QString labelText;
    tempCOM = editLabelDialog->windowTitle();
    tempCOM = tempCOM.mid(tempCOM.indexOf(" "+usbTag)+1,tempCOM.indexOf(" label")-tempCOM.indexOf(" "+ usbTag)-1);
    labelText = editText->text();
    portLabelMap[tempCOM] = labelText ;
    editLabelDialog->close();

    QString oldAlias = aliasWidget->currentItem()->text();
    QString newAlias = "Rig " + labelText + " (" + tempCOM +")";
    delete aliasWidget->currentItem();
    aliasList.removeOne(oldAlias);
    if (!labelText.isEmpty()){ //if there's something in the textedit box...
        aliasList<<newAlias;
        aliasWidget->addItem(newAlias);
    }
    else{
        QSettings settings("Bobcat Engineering","CCS");
        settings.beginGroup("usbPorts");
        settings.remove(tempCOM);
        settings.endGroup();
        portLabelMap.remove(tempCOM);
        aliasWidget->addItem(tempCOM);
    }
    aliasWidget->sortItems();

}


void usbLabelDialog::saveChanges(){
    //Update the Qsettings with the new values   
    QSettings settings("Bobcat Engineering","CCS");
    settings.beginGroup("usbPorts");
        QMap<QString, QString>::const_iterator i = portLabelMap.constBegin();
        while (i != portLabelMap.constEnd()) {
             settings.setValue(i.key(), i.value());
             ++i;
         }
        settings.setValue("portList",aliasList);
    settings.endGroup();
    aliasWidget->clear();
    aliasList.clear();
}
