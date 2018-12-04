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

#include <QtWidgets>

#include "settingsDialog.h"

settingsDialog::settingsDialog(QWidget *parent)
    : QDialog(parent)
{
    #ifdef _WIN32   //--------Windows USB parse settings------------
        usbTag = "COM";
        usbDescription = "USB Serial Port";
    #elif __APPLE__ //---------Mac USB parse settings---------------
        usbTag  = "cu.usbserial";
        usbDescription = "Feather 32u4";
    #elif __linux__ //---------Linux USB parse settings-------------
        usbTag  = "ttyUSB";
        usbDescription = "Feather 32u4";
    #endif
    //Save directory editor
    directoryBox = new QGroupBox("Edit Default Save Directory");
        directoryLayout = new QGridLayout();
            directoryLabel = new QLabel;
            QSettings settings("Bobcat Engineering","CCS");
            settings.beginGroup("Saving");
            directoryLabel->setText("<b>Current Directory:</b><br>" + settings.value("DefaultDir").toString());
            settings.endGroup();
        directoryLayout->addWidget(directoryLabel,0,0,1,3,Qt::AlignLeft);
            changeDir_btn = new QPushButton("Change Directory");
            changeDir_btn->setFocusPolicy(Qt::NoFocus);
        directoryLayout->addWidget(changeDir_btn,1,0,Qt::AlignLeft);
    directoryBox->setLayout(directoryLayout);

    //Session Setup Selectable lists
    sessionListsBox = new QGroupBox("Edit Session Setup Lists");
        //1st listbox//
        sessionListsLayout = new QGridLayout();
            rigLabel = new QLabel("Rig #");
        sessionListsLayout->addWidget(rigLabel,0,0,1,2,Qt::AlignCenter);
            rigVals = new QListWidget();
            rigVals->setMinimumHeight(150);
        sessionListsLayout->addWidget(rigVals,2,0,1,2);
            add1_btn= new QPushButton("Add Rig #");
            add1_btn->setFocusPolicy(Qt::NoFocus);
        sessionListsLayout->addWidget(add1_btn,1,1);
            newItem1 = new QLineEdit();
        sessionListsLayout->addWidget(newItem1,1,0);
            rmv1_btn= new QPushButton("Remove Selected Rig #");
            rmv1_btn->setFocusPolicy(Qt::NoFocus);
        sessionListsLayout->addWidget(rmv1_btn,3,0,1,2);
        //2nd listbox//
            ratLabel = new QLabel("Rat-implant: (LSet/RSet)");
        sessionListsLayout->addWidget(ratLabel,0,2,1,2,Qt::AlignCenter);
            ratVals = new QListWidget();
        sessionListsLayout->addWidget(ratVals,2,2,1,2);
            add2_btn= new QPushButton("Add Rat ID");
            add2_btn->setFocusPolicy(Qt::NoFocus);
        sessionListsLayout->addWidget(add2_btn,1,3);
            newItem2 = new QLineEdit();
        sessionListsLayout->addWidget(newItem2,1,2);
            rmv2_btn= new QPushButton("Remove Selected Rat ID");
            rmv2_btn->setFocusPolicy(Qt::NoFocus);
        sessionListsLayout->addWidget(rmv2_btn,3,2,1,2);
    sessionListsBox->setLayout(sessionListsLayout);

    featuresBox = new QGroupBox("Other Settings");
        featuresLayout = new QGridLayout();
            pythonCheckbox = new QCheckBox("Enable Python Scripts");
            pythonCheckbox->setChecked(pythonEnabled);
        featuresLayout->addWidget(pythonCheckbox);
            histogramCheckbox  = new QCheckBox("Show histogram of light levels at the end of the session");
            histogramCheckbox->setChecked(showHistogram);
        featuresLayout->addWidget(histogramCheckbox);
            mcubeCheckbox = new QCheckBox("Check this box if using Xavier with MCUBE (without photoresistor feedback)");
            mcubeCheckbox->setChecked(mcubeEnabled);
        featuresLayout->addWidget(mcubeCheckbox);
    featuresBox->setLayout(featuresLayout);

    okButton = new QPushButton("Save Settings");
    okButton->setFocusPolicy(Qt::NoFocus);

    settingsLayout = new QGridLayout();
    settingsLayout->addWidget(directoryBox);
    settingsLayout->addWidget(sessionListsBox);
//    settingsLayout->addWidget(portEditBox);
    usbLabelDlog = new usbLabelDialog();
    settingsLayout->addWidget(usbLabelDlog);
//    settingsLayout->addWidget(featuresBox);
    settingsLayout->addWidget(okButton);

    this->setWindowTitle("Settings");
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);//removes "?" from dialog box
    this->setMaximumWidth(0);
    this->setMaximumHeight(300);
    this->setLayout(settingsLayout);

    connect(add1_btn,SIGNAL(clicked()),this,SLOT(addListItem()));
    connect(add2_btn,SIGNAL(clicked()),this,SLOT(addListItem()));
    connect(newItem1,SIGNAL(returnPressed()),this,SLOT(addListItem()));
    connect(newItem2,SIGNAL(returnPressed()),this,SLOT(addListItem()));
    connect(rmv1_btn,SIGNAL(clicked()),this,SLOT(removeItem()));
    connect(rmv2_btn,SIGNAL(clicked()),this,SLOT(removeItem()));
    connect(this,SIGNAL(rejected()),this,SLOT(saveChanges()));
    connect(changeDir_btn,SIGNAL(clicked()),this,SLOT(setPath()));
    connect(okButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(pythonCheckbox,SIGNAL(clicked(bool)),histogramCheckbox,SLOT(setEnabled(bool)));

}

void settingsDialog::myexec(){
    this->exec();
}

void settingsDialog::saveChanges(){
    //Update the Qsettings with the new values
    QSettings settings("Bobcat Engineering","CCS");
    settings.beginGroup("sessionLists");
        settings.setValue("rigList",rigList);
        settings.setValue("ratList",ratList);
    settings.endGroup();

    settings.beginGroup("Features");
        settings.setValue("pythonEnabled",pythonCheckbox->isChecked());
        settings.setValue("showHistogram",histogramCheckbox->isChecked());
        settings.setValue("mcubeEnabled",mcubeCheckbox->isChecked());
    settings.endGroup();

    usbLabelDlog->saveChanges();

    emit dialogClosed();
}

void settingsDialog::removeItem()
{
    if(sender()==rmv1_btn && rigVals->count()>0){
        if(rigVals->selectedItems().size()!=0){
            rigList.removeOne(rigVals->currentItem()->text());
            delete rigVals->currentItem();
        }
    }
    else if(sender()==rmv2_btn && ratVals->count()>0){
        if(ratVals->selectedItems().size()!=0){
            ratList.removeOne(ratVals->currentItem()->text());
            delete ratVals->currentItem();
        }
    }
}

void settingsDialog::addListItem()
{
    QListWidget *tempList = nullptr;
    QLineEdit *tempTxtbox = nullptr;
    QStringList *tempStringList = nullptr;
    if(sender()== add1_btn || sender()== newItem1){
        tempList = rigVals;
        tempTxtbox = newItem1;
        tempStringList = &rigList;
    }
    else if(sender()== add2_btn || sender()== newItem2){
        tempList = ratVals;
        tempTxtbox = newItem2;
        tempStringList = &ratList;
    }
    QString temp = tempTxtbox->text();
    if (!tempStringList->contains(temp) && temp!=""){ //only adds string if it doesn't exist already and if the string isn't blank
        tempStringList->append(tempTxtbox->text());
        tempStringList->sort();
        tempList->addItem(tempTxtbox->text());
        tempTxtbox->clear();
        tempList->sortItems();
    }
}

void settingsDialog::openSettings()
{
    usbLabelDlog->open_USB_Label_dialog();

    //---Edit Session Setup Lists--//
    //get the Qstringlists from memory
    QSettings settings("Bobcat Engineering","CCS");
    settings.beginGroup("sessionLists");
        rigList = settings.value("rigList").toStringList();
        ratList = settings.value("ratList").toStringList();
    settings.endGroup();
    // populate the listwidgets in the settings dialog with the items in the lists we got from memory
    rigVals->clear();
    ratVals->clear();
    for (int i = 0;i< rigList.count(); i++){
        rigVals->addItem(rigList.value(i));
    }
    for (int i = 0;i< ratList.count(); i++){
        ratVals->addItem(ratList.value(i));
    }
    rigVals->clearSelection();
    ratVals->clearSelection();

    //--Other Settings--//
    settings.beginGroup("Features");
        pythonEnabled = settings.value("pythonEnabled").toBool();
        showHistogram = settings.value("showHistogram").toBool();
        mcubeEnabled = settings.value("mcubeEnabled").toBool();
    settings.endGroup();
    //set the checkboxes to match what we got from memory
    pythonCheckbox->setChecked(pythonEnabled);
    histogramCheckbox->setChecked(showHistogram);
    mcubeCheckbox->setChecked(mcubeEnabled);

    this->exec();
}


void settingsDialog::setPath(){
    QString savePath = QFileDialog::getExistingDirectory(this,tr("Set Default Directory"),QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    if (!savePath.isEmpty()){
        QSettings settings("Bobcat Engineering","CCS");
        settings.beginGroup("Saving");
        settings.setValue("DefaultDir",savePath);
        settings.endGroup();
        directoryLabel->setText("<b>Current Directory:</b><br>" + savePath);
    }
}
