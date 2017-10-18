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
            ratLabel = new QLabel("Rat ID");
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

    //alias assignment box
    portEditBox = new QGroupBox("Edit COM Port Labels");
        portEditLayout = new QGridLayout();
            currentPorts = new QLabel("Port");
        portEditLayout->addWidget(currentPorts,0,0,Qt::AlignCenter);
            newDescription = new QLabel("New Label");
        portEditLayout->addWidget(newDescription,0,1,Qt::AlignCenter);
            portDropdown = new QComboBox();
        portEditLayout->addWidget(portDropdown,1,0);
            newAlias = new QLineEdit();
        portEditLayout->addWidget(newAlias,1,1);
            addAlias_btn= new QPushButton("Add Label");
            addAlias_btn->setFocusPolicy(Qt::NoFocus);
        portEditLayout->addWidget(addAlias_btn,1,2);
            portnameLabel = new QLabel("\nCOM Port Labels");
        portEditLayout->addWidget(portnameLabel,2,0,2,3,Qt::AlignCenter);
            aliasWidget = new QListWidget();
            aliasWidget->setMinimumHeight(100);
        portEditLayout->addWidget(aliasWidget,4,0,1,3);
            rmvAlias_btn= new QPushButton("Remove Selected Label");
            rmvAlias_btn->setFocusPolicy(Qt::NoFocus);
        portEditLayout->addWidget(rmvAlias_btn,5,0,1,3);
    portEditBox->setLayout(portEditLayout);

    editLabelDialog = new QDialog();
        editLayout = new QGridLayout();
            newLabel = new QLabel("New Label:");
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
    editLabelDialog->setLayout(editLayout);
    editLabelDialog->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);//removes "?" from dialog box

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
    settingsLayout->addWidget(portEditBox);
    settingsLayout->addWidget(featuresBox);
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
    connect(addAlias_btn,SIGNAL(clicked()),this,SLOT(addAlias()));
    connect(newAlias,SIGNAL(returnPressed()),this,SLOT(addAlias()));
    connect(rmvAlias_btn,SIGNAL(clicked()),this,SLOT(removeAlias()));
    connect(aliasWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(editLabel()));
    connect(changeLabel_btn,SIGNAL(clicked()),this,SLOT(addAlias()));
    connect(cancelChange_btn,SIGNAL(clicked()),editLabelDialog,SLOT(close()));
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
        settings.setValue("portList",aliasStringList);
    settings.endGroup();

    settings.beginGroup("Features");
        settings.setValue("pythonEnabled",pythonCheckbox->isChecked());
        settings.setValue("showHistogram",histogramCheckbox->isChecked());
        settings.setValue("mcubeEnabled",mcubeCheckbox->isChecked());
    settings.endGroup();

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
    QListWidget *tempList = 0;
    QLineEdit *tempTxtbox = 0;
    QStringList *tempStringList = 0;
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

    //--Edit COM Port Labels--//
    //populate the combobox
    portDropdown->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
//        if(info.description()==usbDescription){
            list << info.portName();
            portDropdown->addItem(list.first(), list);
//        }
    }
    //GetaAlias stringlist from memory
    settings.beginGroup("sessionLists");
        aliasStringList = settings.value("portList").toStringList();
    settings.endGroup();
    // populate the listwidgets in the settings dialog with the items in the lists we got from memory
    aliasWidget->clear();
    for (int i = 0;i< aliasStringList.count(); i++){
        aliasWidget->addItem(aliasStringList.value(i));
    }

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

    qDebug()<<"about to show";
    this->exec();
    qDebug()<<"postion"<<this->pos();
}

void settingsDialog::addAlias()
{
    bool continueAdd = true;
    QString tempCOM;
    QString labelText;
    bool fromChangeLabel;
    if (sender()==changeLabel_btn){
        tempCOM = editLabelDialog->windowTitle();
        tempCOM = tempCOM.mid(tempCOM.indexOf(" "+usbTag)+1,tempCOM.indexOf(" label")-tempCOM.indexOf(" "+ usbTag)-1);
        labelText = editText->text();
        fromChangeLabel = true;
    }
    else{
        tempCOM = portDropdown->currentText();
        labelText = newAlias->text();
        fromChangeLabel = false;
    }
    if (!labelText.isEmpty()){ //if there's something in the textedit box...
        QStringList filter = aliasStringList.filter(tempCOM);
        if (!filter.isEmpty()){  //alias already exists
            QMessageBox aliasExists;
            QPushButton *connectButton = aliasExists.addButton(tr("Overwrite Label"), QMessageBox::YesRole);
            aliasExists.addButton(QMessageBox::Cancel);
            aliasExists.setText("A label for " + tempCOM + " already exists");
            aliasExists.setInformativeText("Would you like to overwrite it?");
            if (!fromChangeLabel){  //don't show popup if we got to addAlias from the changeLabel button
                aliasExists.exec();
            }
            if ((aliasExists.clickedButton() == connectButton) | fromChangeLabel) { //yes we want to overwrite it
                delete aliasWidget->item(aliasStringList.indexOf(filter[0]));
                aliasStringList.removeOne(filter[0]);
                editLabelDialog->close();
            }
            else{
                continueAdd = false;
            }
        }
        if (continueAdd){
            aliasStringList.append(labelText + " (" + tempCOM +")" );
            aliasStringList.sort();
            aliasWidget->addItem(labelText + " (" + tempCOM +")");
            aliasWidget->sortItems();
            newAlias->clear();
        }
       }
    else{
    }
}

void settingsDialog::removeAlias()
{
    if(aliasWidget->selectedItems().size()!=0){
        aliasStringList.removeOne(aliasWidget->currentItem()->text());
        delete aliasWidget->currentItem();
    }
}

void settingsDialog::editLabel(){
    //isolate the COMXX part of the port name
    QString COMbeingEdited = aliasWidget->currentItem()->text();
    editText->setText(COMbeingEdited.left(COMbeingEdited.indexOf(" (")));
    COMbeingEdited.remove(0,COMbeingEdited.indexOf("("+usbTag)+1);
    COMbeingEdited.remove(QChar (')'));
    editText->setFocus();
    editLabelDialog->setWindowTitle("Edit "+ COMbeingEdited + " label");
    editLabelDialog->exec();
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
