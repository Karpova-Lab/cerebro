/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MIT License

Copyright (c) 2015-2017 Andy S. Lustig

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
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Xavier");
    qApp->setStyleSheet("QListWidget::item:selected { background-color: gold; color: black}");
    //Menu bar
    fileMenu = menuBar()->addMenu("File");
        toggleDebug = new QAction(this);
        toggleDebug->setText("Enter Debug Mode");
        toggleDebug->setShortcut(QKeySequence(tr("Ctrl+D")));
    fileMenu->addAction(toggleDebug);
        gotoSettings = new QAction(this);
        gotoSettings->setText("Settings...");
        gotoSettings->setShortcut(QKeySequence(tr("Ctrl+,")));
    fileMenu->addAction(gotoSettings);
    goMenu = menuBar()->addMenu("Go To");
        openDir = new QAction(this);
        openDir->setText("Default Save Directory");
    goMenu->addAction(openDir);
        gotoApplocation = new QAction(this);
        gotoApplocation->setText("App install directory");
    goMenu->addAction(gotoApplocation);
    toolMenu = menuBar()->addMenu("Analyze");
        graphResults = new QAction(this);
        graphResults->setText("Create graph from session logs");
    toolMenu->addAction(graphResults);
    helpMenu = menuBar()->addMenu("Help");
        gotoDocs = new QAction(this);
        gotoDocs->setText("User Guide");
    helpMenu->addAction(gotoDocs);
        about = new QAction(this);
        about->setText("About...");
    helpMenu->addAction(about);

    /*Semantic Versioning
        MAJOR version when you make incompatible API changes,
        MINOR version when you add functionality in a backwards-compatible manner, and
        PATCH version when you make backwards-compatible bug fixes.
    */
    aboutDialog = new QMessageBox();
        aboutDialog->setWindowTitle("About");
        QString aboutString = "\t2.0.0\nUpdated:\t9/28/2017";
        aboutDialog->setText("Version:"+aboutString);
        aboutDialog->setStandardButtons(QMessageBox::Close);

    //Experimental setup
    equipmentBox = new QGroupBox("Session Setup");
        equipmentLayout = new QGridLayout();
            rig_lbl = new QLabel("Rig #");
        equipmentLayout->addWidget(rig_lbl,0,1,Qt::AlignCenter);
            rat_lbl = new QLabel("Rat ID");
        equipmentLayout->addWidget(rat_lbl,0,2,Qt::AlignCenter);
            rigSelect = new QListWidget();
        equipmentLayout->addWidget(rigSelect,1,1,4,1,Qt::AlignTop);
            ratSelect = new QListWidget();
        equipmentLayout->addWidget(ratSelect,1,2,4,1,Qt::AlignTop);
            connectBS_label = new QLabel("Base Station Serial Port");
        equipmentLayout->addWidget(connectBS_label,0,5,1,3,Qt::AlignCenter);
            refresh_btn = new QPushButton("Rescan");
        equipmentLayout->addWidget(refresh_btn,1,5);
            serialPortList = new QComboBox();
            serialPortList->setMinimumWidth(150);
        equipmentLayout->addWidget(serialPortList,1,6,1,2);
            debugCheckbox = new QCheckBox("Debug Mode");
        equipmentLayout->addWidget(debugCheckbox,2,5,1,3,Qt::AlignCenter);
            connect_btn = new QPushButton("Start Session");
            connect_btn->setCheckable(true);
        equipmentLayout->addWidget(connect_btn,3,5,1,3);
        equipmentLayout->setColumnStretch(0,1);
        equipmentLayout->setColumnStretch(8,1);
    equipmentBox->setLayout(equipmentLayout);

    //Base station monitor
    baseBox = new QGroupBox();
        serialMonitorLayout = new QGridLayout();
            serial_title = new QLabel("<h3><u>Base Station Monitor</u>:");
        serialMonitorLayout->addWidget(serial_title,0,0,1,4);
        clearBase_btn = new QPushButton();
            clearBase_btn->setText("Clear Monitor");
        serialMonitorLayout->addWidget(clearBase_btn,0,4,1,2,Qt::AlignRight);
            baseFilter_label = new QLabel("Filter Duration:");
        serialMonitorLayout->addWidget(baseFilter_label,1,0);
            baseMonitor = new QPlainTextEdit();
            baseMonitor->setMinimumWidth(500);
            baseMonitor->setMinimumHeight(500);
        serialMonitorLayout->addWidget(baseMonitor,2,0,1,6);
            eeprom_btn = new QPushButton();
            eeprom_btn->setText("Save Session");
        serialMonitorLayout->addWidget(eeprom_btn,3,0,1,6);
    baseBox->setLayout(serialMonitorLayout);
    baseBox->setEnabled(false);


    //Waveform Adjustment
    adjustBox = new QGroupBox("Cerebro's Parameters");
        adjustmentLayout = new QGridLayout();
            power_lbl = new QLabel("Power Level");
            power_lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        adjustmentLayout->addWidget(power_lbl,0,0);
            power_spn = new QSpinBox();
            power_spn->setRange(0,65535);
            power_spn->setSingleStep(50);
            power_spn->setAlignment(Qt::AlignCenter);
        adjustmentLayout->addWidget(power_spn,0,1);
            newPower_btn = new QPushButton("Send New Power Level");
        adjustmentLayout->addWidget(newPower_btn,1,0,1,2);
            singleShot = new QRadioButton("Single Shot");
            singleShot->setChecked(true);
        adjustmentLayout->addWidget(singleShot,2,0,Qt::AlignRight);
            pulseTrain = new QRadioButton("Pulse Train");
        adjustmentLayout->addWidget(pulseTrain,2,1);
        startDelay_lbl = new QLabel("Start Delay");
            startDelay_lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        adjustmentLayout->addWidget(startDelay_lbl,3,0);
            startDelay_spn = new QSpinBox();
            startDelay_spn->setRange(0,65535);
            startDelay_spn->setSingleStep(50);
            startDelay_spn->setSuffix(" ms");
            startDelay_spn->setAlignment(Qt::AlignCenter);
        adjustmentLayout->addWidget(startDelay_spn,3,1);
            onTime_lbl = new QLabel("On Time");
            onTime_lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        adjustmentLayout->addWidget(onTime_lbl,4,0);
            onTime_spn = new QSpinBox();
            onTime_spn->setRange(0,65535);
            onTime_spn->setSingleStep(50);
            onTime_spn->setSuffix(" ms");
            onTime_spn->setAlignment(Qt::AlignCenter);
        adjustmentLayout->addWidget(onTime_spn,4,1);
            offTime_lbl = new QLabel("Off Time");
            offTime_lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            offTime_lbl->setVisible(false);
        adjustmentLayout->addWidget(offTime_lbl,5,0);
            offTime_spn = new QSpinBox();
            offTime_spn->setRange(0,65535);
            offTime_spn->setSingleStep(50);
            offTime_spn->setSuffix(" ms");
            offTime_spn->setAlignment(Qt::AlignCenter);
            offTime_spn->setVisible(false);
        adjustmentLayout->addWidget(offTime_spn,5,1);
            trainDescription_lbl  = new QLabel("");
            trainDescription_lbl->setVisible(false);
            trainDescription_lbl->setAlignment(Qt::AlignRight);
        adjustmentLayout->addWidget(trainDescription_lbl,6,0,1,2);
            trainDuration_lbl = new QLabel("Train Duration");
            trainDuration_lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            trainDuration_lbl->setVisible(false);
        adjustmentLayout->addWidget(trainDuration_lbl,7,0);
            trainDuration_spn = new QSpinBox();
            trainDuration_spn->setRange(0,65535);
            trainDuration_spn->setSingleStep(250);
            trainDuration_spn->setSuffix(" ms");
            trainDuration_spn->setAlignment(Qt::AlignCenter);
            trainDuration_spn->setVisible(false);
        adjustmentLayout->addWidget(trainDuration_spn,7,1);
            fade_checkbox = new QCheckBox("Ramp Down");
        adjustmentLayout->addWidget(fade_checkbox,8,0,Qt::AlignRight);
            fade_spn = new QSpinBox();
            fade_spn->setRange(100,65535);
            fade_spn->setSingleStep(100);
            fade_spn->setSuffix(" ms");
            fade_spn->setAlignment(Qt::AlignCenter);
            fade_spn->setVisible(false);
        adjustmentLayout->addWidget(fade_spn,8,1);
            sendSettings_btn = new QPushButton("Send New Waveform Parameters");
            sendSettings_btn->setAutoDefault(true);
        adjustmentLayout->addWidget(sendSettings_btn,9,0,1,2);
            last_settings = new QLabel();
            last_settings->setText("No parameters sent yet");
        adjustmentLayout->addWidget(last_settings,10,0,1,2,Qt::AlignTop);
    adjustBox->setLayout(adjustmentLayout);
    adjustBox->setMinimumWidth(235);
    adjustBox->setEnabled(false);
    adjustBox->setPalette(Qt::gray);

    //Characterization
    charBox = new QGroupBox("Implant Construction Tools");
        charLayout = new QGridLayout();
            startDiode_btn = new QPushButton("Start Diode Characterization");
        charLayout->addWidget(startDiode_btn,0,0);
            startImplant_btn = new QPushButton("Start Implant Characterization");
        charLayout->addWidget(startImplant_btn,1,0);
            initialize_btn = new QPushButton("Send Power Values to Cerebro");
        charLayout->addWidget(initialize_btn,2,0);
    charBox->setLayout(charLayout);
    charBox->setEnabled(false);
    charBox->setPalette(Qt::gray);

    //Characterization Dialogs
    createFadeDialog = new QDialog();
        createVecLayout = new QGridLayout();
            slideLabel = new QLabel("Target Power (mW):");
        createVecLayout->addWidget(slideLabel,0,1,Qt::AlignRight);
            wantedLevel = new QLineEdit();
            wantedLevel->setMaximumWidth(50);
        createVecLayout->addWidget(wantedLevel,0,2);
            showGraph = new QCheckBox("Show Graphs");
            showGraph->setChecked(true);
        createVecLayout->addWidget(showGraph,1,0,1,4,Qt::AlignCenter);
            selectFile_btn = new DropButton(QString("Select Power Meter File"));//("Select Power Meter\ndata file");
            selectFile_btn->setCheckable(true);
            selectFile_btn->setAcceptDrops(true);
            selectFile_btn->setCheckable(false);
        createVecLayout->addWidget(selectFile_btn,2,0,1,4,Qt::AlignCenter);
    createFadeDialog->setWindowTitle("Create fade vector from file");
    createFadeDialog->setWindowFlags(createFadeDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);//removes "?" from dialog box
    createFadeDialog->setMinimumWidth(300);
    createFadeDialog->setMaximumWidth(300);
    createFadeDialog->setLayout(createVecLayout);

    sendFadeDialog = new QDialog();
        sendFadeLayout = new QGridLayout();
            createVecBtn = new QPushButton("Create fade vector from power meter file");
        sendFadeLayout->addWidget(createVecBtn,0,0,1,6);
            orLabel = new QLabel("or");
        sendFadeLayout->addWidget(orLabel,1,0,1,6,Qt::AlignCenter);
            codeTextBox = new QPlainTextEdit();
            codeTextBox->setMinimumHeight(350);
            codeTextBox->setPlaceholderText("Paste Fade Vector here");
    //        QFont codefont;
    //        codefont.setPointSize(5);
    //        codeTextBox->setFont(codefont);
        sendFadeLayout->addWidget(codeTextBox,2,0,1,6);
            cerebroNum_lbl = new QLabel("Cerebro #");
            cerebroNum_lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        sendFadeLayout->addWidget(cerebroNum_lbl,3,0);
            cerebroNum_edit = new QLineEdit;
            cerebroNum_edit->setFixedWidth(40);
        sendFadeLayout->addWidget(cerebroNum_edit,3,1);
            ldNum_lbl = new QLabel("Implant #");
            ldNum_lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        sendFadeLayout->addWidget(ldNum_lbl,4,0);
            ldNum_edit = new QLineEdit;
            ldNum_edit->setFixedWidth(40);
        sendFadeLayout->addWidget(ldNum_edit,4,1);
            sendCal_btn = new QPushButton("Send Values");
            sendCal_btn->setMinimumHeight(40);
        sendFadeLayout->addWidget(sendCal_btn,3,2,2,4);
    sendFadeDialog->setWindowTitle("Power Value Sender");
    sendFadeDialog->setWindowFlags(sendFadeDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);//removes "?" from dialog box
    sendFadeDialog->setMaximumWidth(0);
    sendFadeDialog->setMaximumHeight(0);
    sendFadeDialog->setModal(false);
    sendFadeDialog->setLayout(sendFadeLayout);

    //Triggering & debugging
    bugBox = new QGroupBox("Debug");
        triggerLayout = new QGridLayout();
            trigger_checkbox = new QCheckBox();
            trigger_checkbox->setText("Test Mode");
        triggerLayout->addWidget(trigger_checkbox,0,0,Qt::AlignRight);
            trials_spn = new QSpinBox();
            trials_spn->setMinimum(0);
            trials_spn->setMaximum(10000);
            trials_spn->setSingleStep(10);
            trials_spn->setSuffix(" test signals");
            trials_spn->setAlignment(Qt::AlignCenter);
            trials_spn->setValue(10);
            trials_spn->setEnabled(false);
        triggerLayout->addWidget(trials_spn,0,1,Qt::AlignLeft);
            trigger_btn = new QPushButton;
            trigger_btn->setText("Trigger");
            trigger_btn->setAutoDefault(true);
        triggerLayout->addWidget(trigger_btn,1,0);
            testProgress = new QProgressBar();
            testProgress->setValue(0);
            testProgress->setVisible(false);
        triggerLayout->addWidget(testProgress,2,0,1,2);
            abort_btn = new QPushButton("Stop");
        triggerLayout->addWidget(abort_btn,1,1);
            batteryStatus_btn = new QPushButton("Battery Status");
        triggerLayout->addWidget(batteryStatus_btn,3,0);
            getInfo_btn = new QPushButton("Get Cerebro Info");
        triggerLayout->addWidget(getInfo_btn,3,1);
            macroText = new QLineEdit();
//                macroText->setFixedWidth(40);
        triggerLayout->addWidget(macroText,4,0,Qt::AlignRight);
            macro_btn = new QPushButton("Send Text");
        triggerLayout->addWidget(macro_btn,4,1);
    bugBox->setLayout(triggerLayout);
    bugBox->setEnabled(false);
    bugBox->setPalette(Qt::gray);

    //Download Monitor
    downloaderBox = new QGroupBox();
        connectionLayout2 = new QGridLayout();
            connectLU_label = new QLabel("Downloader Serial Port");
        connectionLayout2->addWidget(connectLU_label,0,0,1,3,Qt::AlignCenter);
            refresh2_btn = new QPushButton("Rescan");
        connectionLayout2->addWidget(refresh2_btn,1,0);
            serialPortList2 = new QComboBox();
            serialPortList2->setMinimumWidth(150);
        connectionLayout2->addWidget(serialPortList2,1,1,1,2);
            connect2_btn = new QPushButton("Connect to Downloader");
            connect2_btn->setCheckable(true);
        connectionLayout2->addWidget(connect2_btn,2,0,1,3);
            download_title = new QLabel("<h3><u>Cerebro Monitor</u>:");
            download_title->setEnabled(false);
        connectionLayout2->addWidget(download_title,3,0,1,2);
            clearDownload_btn = new QPushButton("Clear Monitor");
            clearDownload_btn->setEnabled(false);
        connectionLayout2->addWidget(clearDownload_btn,3,2,Qt::AlignRight);
            downloadMonitor = new QPlainTextEdit();
            downloadMonitor->setMinimumWidth(275);
            downloadMonitor->setEnabled(false);
        connectionLayout2->addWidget(downloadMonitor,4,0,1,3);
        connectionLayout2->setRowStretch(4,1);
            saveMonitor_btn = new QPushButton();
            saveMonitor_btn->setText("Save Session");
            saveMonitor_btn->setMinimumWidth(125);
            saveMonitor_btn->setMinimumHeight(40);
            saveMonitor_btn->setEnabled(false);
        connectionLayout2->addWidget(saveMonitor_btn,5,0,1,3);
    downloaderBox->setLayout(connectionLayout2);


    //Add Elements to Main Layout
    mainLayout = new QGridLayout();
    mainLayout->addWidget(equipmentBox,0,0,1,3);
    mainLayout->addWidget(baseBox,1,1,4,2);
    mainLayout->addWidget(adjustBox,1,0);
    mainLayout->addWidget(charBox,2,0);
    mainLayout->addWidget(bugBox,3,0);
//    mainLayout->addWidget(downloaderBox,0,3,5,1);
//    mainLayout->setColumnStretch(2,1);
//    mainLayout->setRowStretch(5,1);

    QWidget *window = new QWidget();
    window->setLayout(mainLayout);
    setCentralWidget(window);


    settingsDlog = new settingsDialog();

#ifdef _WIN32   //--------Windows USB parse settings------------
    usbTag = "COM";
    usbDescription = "Adafruit Feather 32u4 (800C:00)";
#elif __APPLE__ //---------Mac USB parse settings---------------
    usbTag  = "cu.usbmodem";
    usbDescription = "Feather 32u4";
#elif __linux__ //---------Linux USB parse settings-------------
    usbTag  = "ttyUSB";
    usbDescription = "Feather 32u4";
#endif

    //program setup
    serial = new QSerialPort(this);
    serial2 = new QSerialPort(this);
    startingUp = true;
    baseConnected = false;
    receivedBaseInfo = false;
    downloadConnected = false;
    inTestloop = false;
    testCount = 0;
    timer = new QTimer(this);
    onTimeString = "";
    offTimeString = "";
    if(QSysInfo::WindowsVersion==48){ //If Windows XP
        onTimeString = " ms\t\t\nOn Time:\t\t";
        offTimeString = " ms\nOff Time:\t\t";
    }
    else{
        onTimeString = " ms\t\t\nOn Time:\t";
        offTimeString = " ms\nOff Time:\t";
    }
    applySettings();


    //menu functions
    connect(openDir,SIGNAL(triggered()),this,SLOT(gotoDir()));
    connect(gotoApplocation,SIGNAL(triggered()),this,SLOT(gotoAppLocation()));
    connect(graphResults,SIGNAL(triggered(bool)),this,SLOT(getGraphs()));
    connect(gotoDocs,SIGNAL(triggered()),this,SLOT(openDocs()));
    connect(about,SIGNAL(triggered(bool)),aboutDialog,SLOT(exec()));
    connect(toggleDebug,SIGNAL(triggered()),this,SLOT(setDebug()));
    connect(debugCheckbox,SIGNAL(clicked(bool)),this,SLOT(setDebug()));
    connect(gotoSettings,SIGNAL(triggered()),settingsDlog,SLOT(openSettings()));

    //Port Connections
    connect(settingsDlog,SIGNAL(dialogClosed()),this,SLOT(applySettings()));
    connect(refresh_btn,SIGNAL(clicked()),this,SLOT(fillBasestationPorts()));
    connect(refresh2_btn,SIGNAL(clicked()),this,SLOT(fillDownloaderPorts()));
    connect(connect_btn,SIGNAL(clicked()),this,SLOT(connectBasePort()));
    connect(connect2_btn,SIGNAL(clicked()),this,SLOT(connectDownloadPort()));

    //Monitors
    connect(serial,SIGNAL(readyRead()),this,SLOT(readSerial()));
    connect(serial2,SIGNAL(readyRead()),this,SLOT(readLog()));
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(errorMsg()));
    connect(clearBase_btn,SIGNAL(clicked()),this,SLOT(clearMonitor()));
    connect(clearDownload_btn,SIGNAL(clicked()),this,SLOT(clearMonitor2()));
    connect(saveMonitor_btn,SIGNAL(clicked()),this,SLOT(saveFile()));
    connect(eeprom_btn,SIGNAL(clicked()),this,SLOT(EEPROM()));

    //Cerebro Parameters
    connect(onTime_spn,SIGNAL(valueChanged(int)),this,SLOT(trainDur()));
    connect(offTime_spn,SIGNAL(valueChanged(int)),this,SLOT(trainDur()));
    connect(fade_checkbox,SIGNAL(clicked()),this,SLOT(fadeChecked()));
    connect(sendSettings_btn,SIGNAL(clicked()),this,SLOT(set()));
    connect(trigger_btn,SIGNAL(clicked()),this,SLOT(triggerPushed()));
    connect(trigger_checkbox,SIGNAL(clicked()),this,SLOT(triggerChecked()));
    connect(singleShot,SIGNAL(clicked()),this,SLOT(trainChecked()));
    connect(pulseTrain,SIGNAL(clicked()),this,SLOT(trainChecked()));
    connect(newPower_btn,SIGNAL(clicked(bool)),this,SLOT(sendNewPower()));

    //Debug Commands
    connect(macro_btn,SIGNAL(clicked()),this,SLOT(macro()));
    connect(macroText,SIGNAL(returnPressed()),this,SLOT(macro()));
    connect(abort_btn,SIGNAL(clicked()),this,SLOT(abort()));
    connect(batteryStatus_btn,SIGNAL(clicked(bool)),this,SLOT(getBatteryStatus()));
    connect(getInfo_btn,SIGNAL(clicked(bool)),this,SLOT(getInfo()));
    connect(timer, SIGNAL(timeout()), this, SLOT(sendTrigger()));

    //Calibration
    connect(selectFile_btn,SIGNAL(clicked()),this,SLOT(chooseFile()));
    connect(selectFile_btn, SIGNAL(dropped(const QMimeData*)),this, SLOT(useDropped(const QMimeData*)));
    connect(startImplant_btn,SIGNAL(clicked()),this,SLOT(sendImplantStart()));
    connect(startDiode_btn,SIGNAL(clicked()),this,SLOT(sendDiodeStart()));
    connect(sendCal_btn,SIGNAL(clicked()),this,SLOT(sendCalVector()));
    connect(initialize_btn,SIGNAL(clicked(bool)),sendFadeDialog,SLOT(show()));
    connect(createVecBtn,SIGNAL(clicked(bool)),createFadeDialog,SLOT(exec()));
}


//--------------------------------------------------------------------------------------------------------//
MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (baseConnected){
        QMessageBox alert;
            alert.setWindowTitle("Exit");
            alert.setIcon(QMessageBox::Warning);
            alert.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
            alert.setDefaultButton(QMessageBox::Cancel);
            alert.setEscapeButton(QMessageBox::Cancel);
            alert.setText("Are you sure you want to exit without saving?");
        if (alert.exec() == QMessageBox::Cancel){
            event->ignore();
        }
        else{
            event->accept();
        }
    }
    else{
        event->accept();
    }
}

void MainWindow::applySettings()
{
    //get the Qstringlists from memory
    QSettings settings("Bobcat Engineering","CCS");
    settings.beginGroup("sessionLists");
        rigList = settings.value("rigList").toStringList();
        ratList = settings.value("ratList").toStringList();
    settings.endGroup();

    //populate the list widgets
    rigSelect->clear();
    ratSelect->clear();
    rigSelect->addItems(rigList);
    ratSelect->addItems(ratList);
    int itemHeight = 25;
    int itemWidth = 75;
    rigSelect->setMaximumSize(itemWidth,itemHeight*rigSelect->count());
    ratSelect->setMaximumSize(itemWidth,itemHeight*ratSelect->count());

    //
    settings.beginGroup("Features");
        pythonEnabled = settings.value("pythonEnabled").toBool();
        showHistogram = settings.value("showHistogram").toBool();
        mcubeEnabled = settings.value("mcubeEnabled").toBool();
    settings.endGroup();
    toolMenu->setEnabled(pythonEnabled);
    power_lbl->setVisible(mcubeEnabled);
    power_spn->setVisible(mcubeEnabled);
    newPower_btn->setVisible(mcubeEnabled);


    //Populate COM port Dropdowns
    settings.beginGroup("sessionLists");
        aliasStringList = settings.value("portList").toStringList();
    settings.endGroup();
    fillBasestationPorts();
    fillDownloaderPorts();

    debugOn = false;
    showDebug();
    if (downloadConnected){
        connectDownloadPort(); //disconnect download port after settings change
    }
}

void MainWindow::fillBasestationPorts()
{
    refresh_btn->setDown(1);
    serialPortList->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        if (!info.isBusy()){
            qDebug()<<info.description();
            if(info.description()==usbDescription){                
                if (!aliasStringList.filter(info.portName()).isEmpty()){
                    if (!aliasStringList.filter(info.portName())[0].contains("Downloader",Qt::CaseInsensitive)){
                        list << aliasStringList.filter(info.portName())[0];
                        serialPortList->addItem(list.first(), list);
                    }
                }
                else{
                    list << info.portName();
                    serialPortList->addItem(list.first(), list);
                }
            }
        }
    }
    refresh_btn->setDown(0);
}

void MainWindow::fillDownloaderPorts()
{
    refresh2_btn->setDown(1);
    serialPortList2->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        if (!info.isBusy()){
            if(info.description()==usbDescription){
                if (!aliasStringList.filter(info.portName()).isEmpty()){
                    if (!aliasStringList.filter(info.portName())[0].contains("Base",Qt::CaseInsensitive)){
                        list << aliasStringList.filter(info.portName())[0];
                        serialPortList2->addItem(list.first(), list);
                    }
                }
                else{
                    list << info.portName();
                    serialPortList2->addItem(list.first(), list);
                }
            }
        }
    }
    refresh2_btn->setDown(0);
}

void MainWindow::connectBasePort()
{
    if((rigSelect->selectedItems().size()==0) && !baseConnected && !debugOn ){ //didn't select rig
        QMessageBox alert;
        alert.setText("Please select a Rig # to continue");
        alert.setIcon(QMessageBox::Warning);
        alert.setWindowTitle("Missing Rig #");
        alert.exec();
        }
    else if((ratSelect->selectedItems().size()==0) && !baseConnected && !debugOn){ //didn't select rat
        QMessageBox alert;
        alert.setText("Please select a Rat ID to continue");
        alert.setIcon(QMessageBox::Warning);
        alert.setWindowTitle("Missing Rat ID");
        alert.exec();
    }
    else{ //  disable and enable components depending on whether we are connecting or disconnecting to the base station
        gotoSettings->setEnabled(baseConnected);
        toggleDebug->setEnabled(baseConnected);
        rig_lbl->setEnabled(baseConnected);
        rat_lbl->setEnabled(baseConnected);
        rigSelect->setEnabled(baseConnected);
        ratSelect->setEnabled(baseConnected);
        connectBS_label->setEnabled(baseConnected);
        serialPortList->setEnabled(baseConnected);
        refresh_btn->setEnabled(baseConnected);
        debugCheckbox->setEnabled(baseConnected);
        adjustBox->setEnabled(!baseConnected);
        charBox->setEnabled(!baseConnected && debugOn);
        bugBox->setEnabled(!baseConnected && debugOn);
//        baseSettingsBox->setEnabled(!baseConnected);
        baseBox->setEnabled(!baseConnected);
        saveMonitor_btn->setEnabled(downloadConnected && !baseConnected);
        if(!baseConnected){ //connect to serial port
            //isolate the COMXX part of the port name
            QString tempPortName = serialPortList->currentText();
            tempPortName.remove(0,tempPortName.indexOf("("+usbTag)+1);
            tempPortName.remove(QChar (')'));
            //open serial connection
            serial->setPortName(tempPortName);
            serial->setBaudRate(QSerialPort::Baud115200);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            serial->open(QIODevice::ReadWrite);
            connect_btn->setText("Disconnect");
//            connect_btn->setStyleSheet("background-color: grey; color:black");
            if(!debugOn){
                setWindowTitle("Rig " + rigSelect->currentItem()->text() + " Rat " + ratSelect->currentItem()->text() );}
            else{
                setWindowTitle("Debug Mode");
            }
//            QString rst = "R";
//            serial->write(rst.toLocal8Bit());
//            clearBase_btn->setEnabled(false);
//            connect_btn->setEnabled(false);
//            QTimer::singleShot(1000, this, SLOT(sendTime()));
            baseConnected = true;
            errorThrown = false;
        }
        else{ //disconnect from serial port
            QString time = "\r" + serialPortList->currentText() + " Disconnected - " + QDate::currentDate().toString() + " " + QTime::currentTime().toString()+ "\r---------------------------------------------------------\r";
            baseMonitor->textCursor().insertText(time);
            serial->close();
            // connect_btn->setStyleSheet("background-color: green; color:white");
            setWindowTitle("Xavier");
            debugOn = false;
            showDebug();
            baseConnected = false;
            receivedBaseInfo = false;
            baseFilter_label->setText("Filter Duration:");
        }
    }
    connect_btn->setChecked(baseConnected);
}

void MainWindow::connectDownloadPort()
{
    QString tempPortName = serialPortList2->currentText();
    if (tempPortName.isEmpty() & !downloadConnected &!debugOn){
        QMessageBox noSerial;
        noSerial.setText("Make sure a Downloader Cord is connected \nand click ''Rescan''' to see available Serial Ports.");
        noSerial.setIcon(QMessageBox::Warning);
        noSerial.setWindowTitle("No Available Serial Ports");
        noSerial.exec();
    }
    else{
        connectLU_label->setEnabled(downloadConnected);
        serialPortList2->setEnabled(downloadConnected);
        refresh2_btn->setEnabled(downloadConnected);
        downloadMonitor->setEnabled(!downloadConnected);
        download_title->setEnabled(!downloadConnected);
        clearDownload_btn->setEnabled(!downloadConnected);
        saveMonitor_btn->setEnabled(!downloadConnected && baseConnected);
        if(!downloadConnected){//if it's not connected, we'll connect it
            //isolate the COMXX part of the port name
            tempPortName.remove(0,tempPortName.indexOf("("+usbTag)+1);
            tempPortName.remove(QChar (')'));
            //open serial connection
            serial2->setPortName(tempPortName);
            serial2->setBaudRate(QSerialPort::Baud115200);
            serial2->setDataBits(QSerialPort::Data8);
            serial2->setParity(QSerialPort::NoParity);
            serial2->setStopBits(QSerialPort::OneStop);
            serial2->setFlowControl(QSerialPort::NoFlowControl);
            serial2->open(QIODevice::ReadWrite);
            connect2_btn->setText("Disconnect");
            downloadConnected = true;
        }
        else{// if it is connected, we'll disconnect
            serial2->close();
            connect2_btn->setText("Connect To Downloader");
            downloadConnected = false;
            fillDownloaderPorts();
        }
    }
    connect2_btn->setChecked(downloadConnected);
}

void MainWindow::sendTime()
{
    startTime = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm");
    QString startup;
    if (!debugOn){
        startup = QString("%1,%2,%3,%4,").arg(startTime,rigSelect->currentItem()->text(),ratSelect->currentItem()->text());
    }
    else{
        startup = QString("%1,%2,%3,%4,").arg(startTime,"99","99","99");
    }
    serial->write(startup.toLocal8Bit());
    connect_btn->setEnabled(true);
    clearBase_btn->setEnabled(true);
}

void MainWindow::readSerial()
{
    baseMonitor->moveCursor(QTextCursor::End);
    baseMonitor->insertPlainText(serial->readAll());
    baseMonitor->ensureCursorVisible();
    if(!receivedBaseInfo){
        QString buffer = baseMonitor->toPlainText();
        if (buffer.count('\n')==1 && buffer.count('_')==4 && buffer.count(',')==5){
            receivedBaseInfo = true;
            QStringList baseInfo = buffer.trimmed().split(',');
            baseFilter_label->setText("Filter Duration: "+ baseInfo[5]+ " ms");
        }
        else if (buffer.count('\n')>1){
            receivedBaseInfo = true; //stop searching if we haven't found it after the first line
        }
    }
}
void MainWindow::readLog()
{
    downloadMonitor->moveCursor(QTextCursor::End);
    downloadMonitor->insertPlainText(serial2->readAll());
    downloadMonitor->moveCursor(QTextCursor::Start);
    if(downloadMonitor->find("*")){
        QString buffer = downloadMonitor->toPlainText();
        if (buffer.count('~')==9){
            int andIndex = buffer.indexOf("&");
            int starIndex = buffer.indexOf("*");
            downloadMonitor->clear();
            if(andIndex>0){
                downloadMonitor->insertPlainText(buffer.mid(0,andIndex-1));
            }
            QStringList onboardParams = buffer.mid(andIndex+1,starIndex).split('~');
            power_spn->setValue(onboardParams[3].toInt());
            startDelay_spn->setValue(onboardParams[4].toInt());
            onTime_spn->setValue(onboardParams[5].toInt());
            offTime_spn->setValue(onboardParams[6].toInt());
            if(onboardParams[7].toInt()){
                pulseTrain->setChecked(true);
                trainChecked();
            }
            else{
                singleShot->setChecked(true);
                trainChecked();
            }
            trainDuration_spn->setValue(onboardParams[7].toInt());
            if (onboardParams[8].toInt()){
                fade_checkbox->setChecked(true);
                fade_spn->setValue(onboardParams[8].toInt());
            }
            else{
                fade_checkbox->setChecked(false);
            }
            fadeChecked();
            QMessageBox showParams;
            showParams.setWindowTitle("Cerebro #" + onboardParams[1]  + " - LD #" + onboardParams[2]);
            showParams.setStandardButtons(QMessageBox::Ok);
            showParams.setText(
                    "Firmware:\t" + onboardParams[0] +
                    "\n\nPower Level:\t" + onboardParams[3] +
                    "\n\nStart Delay:\t"+ onboardParams[4]+
                    onTimeString + onboardParams[5] +
                    offTimeString + onboardParams[6] +
                    " ms\nTrain Duration:\t" + onboardParams[7] +
                    " ms\nFade Time:\t" + onboardParams[8] + " ms" );
            showParams.exec();
//            if(baseConnected){
//                QMessageBox filterMsg;
//                filterMsg.setWindowTitle("Update Base Station Command Filter");
//                filterMsg.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
//                filterMsg.setDefaultButton(QMessageBox::No);
//                filterMsg.setText("Would you like to update the Base Station Command Filter to match the parameters on this Cerebro?");
//                if(filterMsg.exec()==QMessageBox::Yes){
//                    updateFilter();
//                }
//            }
        }
    }
    downloadMonitor->moveCursor(QTextCursor::End);
    downloadMonitor->ensureCursorVisible();

}

void MainWindow::clearMonitor()
{
    baseMonitor->clear();
//    receivedBaseInfo = false;
//    baseFilter_label->setText("Filter Duration:");
//    if(baseConnected){
//        QString rst = "R";
//        serial->write(rst.toLocal8Bit());
//        clearBase_btn->setEnabled(false);
//    }
//    QTimer::singleShot(1000, this, SLOT(sendTime()));
}

void MainWindow::clearMonitor2()
{
    downloadMonitor->clear();
}


void MainWindow::set()
{
    QString startDelay = QString::number(startDelay_spn->value());
    QString onTime = QString::number(onTime_spn->value());
    QString offTime = "0";
    QString trainDur = "0";
    QString fadeTime = "0";
    if(pulseTrain->isChecked()){
        offTime = QString::number(offTime_spn->value());
        trainDur = QString::number(trainDuration_spn->value());
    }
    if(fade_checkbox->isChecked()){
        if(fade_spn->value()%100){// throw an error if the fadetime isn't a multiple of 100
            QMessageBox fadeError;
                fadeError.setWindowTitle("Parameter Error");
                fadeError.setIcon(QMessageBox::Critical);
                fadeError.setStandardButtons(QMessageBox::Ok);
                fadeError.setText("Ramp Down time must be a multiple of 100");
            fadeError.exec();
            return;
        }
        else{
            fadeTime = QString::number(fade_spn->value());
        }
    }
    QString msg = "W,"+ startDelay +","+ onTime +","+ offTime +"," + trainDur + "," + fadeTime;
    QMessageBox confirmUpdate;
        confirmUpdate.setWindowTitle("Confirm Parameter Change");
        confirmUpdate.setIcon(QMessageBox::Question);
        confirmUpdate.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
        confirmUpdate.setDefaultButton(QMessageBox::Cancel);
        confirmUpdate.setEscapeButton(QMessageBox::Cancel);
        confirmUpdate.setText("<b>Are you sure you want to send the following new parameters to Cerebro?</b>");
        confirmUpdate.setInformativeText(
                    "Start Delay:\t"+ startDelay+
                    onTimeString + onTime +
                    offTimeString + offTime +
                    " ms\nTrain Duration:\t" + trainDur +
                    " ms\nFade Time:\t" + fadeTime + " ms");
//                    "\n\nThe command filter will be automatically updated to " + QString::number(baseFilter_spn->value()) + " ms");
//    if (confirmUpdate.exec() == QMessageBox::Yes){
        serial->write(msg.toLocal8Bit());
        last_settings->setText("Last Parameters Sent:\n" + startDelay  + ", " + onTime + ", " + offTime + ", " + trainDur + ", " + fadeTime );
//        QTimer::singleShot(500, this, SLOT(updateFilter()));
//    }
}

void MainWindow::sendTrigger()
{
    QString msg = "T";
    if (inTestloop){
        testCount++;
        testProgress->setValue(testCount);
        if (testCount==trials_spn->value()){
            triggerPushed();
        }
    }
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<" Sent";
}

void MainWindow::triggerPushed()
{
    if(trigger_checkbox->isChecked()){
        bool flag = 0;
        if(!flag){
            inTestloop = !inTestloop;
            if(inTestloop){
                timer->start(2000);
                testProgress->setVisible(true);
                testProgress->setValue(0);
                testProgress->setMaximum(trials_spn->value());
                trigger_btn->setText("Stop Testing");
            }
            else{
                timer->stop();
                testProgress->setVisible(false);
                trigger_btn->setText("Begin Test");
                testCount = 0;
            }
            trigger_checkbox->setEnabled(!inTestloop);
            trials_spn->setEnabled(!inTestloop);
        }
    }
    else{
        sendTrigger();
    }
}

void MainWindow::triggerChecked()
{
    trials_spn->setEnabled(!trials_spn->isEnabled());
    if(!trigger_checkbox->isChecked()){
        trigger_btn->setText("Trigger");
    }
    else{
        trigger_btn->setText("Begin Test");
    }
}

void MainWindow::trainChecked()
{
    offTime_lbl->setVisible(pulseTrain->isChecked());
    offTime_spn->setVisible(pulseTrain->isChecked());
    trainDescription_lbl->setVisible(pulseTrain->isChecked());
    trainDuration_lbl->setVisible(pulseTrain->isChecked());
    trainDuration_spn->setVisible(pulseTrain->isChecked());
    trainDur();
    fade_checkbox->setVisible(singleShot->isChecked());
    if(pulseTrain->isChecked()){
        fade_spn->setVisible(false);
        fade_checkbox->setChecked(false);
    }


}

void MainWindow::saveFile()
{
    QString ratNumber;
    if(debugOn){
        ratNumber = "9999";
    }
    else{
        ratNumber = ratSelect->currentItem()->text();
    }
    bool continueSaving = true;
    //get the default save directory
    QSettings settings("Bobcat Engineering","CCS");
    settings.beginGroup("Saving");
    QString saveDirectoryPath = settings.value("DefaultDir").toString();
    settings.endGroup();
    //if the default save directory hasn't been set yet, use the desktop as the save directory
    if(saveDirectoryPath.isEmpty()){
        saveDirectoryPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }
    QDateTime saveTime = saveTime.currentDateTime();
    //check if the rat# folder exists in the save directory, if not go ahead and make a new rat# folder
    if(!QDir(saveDirectoryPath+ "/" + ratNumber).exists()){
        QMessageBox newFolder;
        QPushButton *connectButton = newFolder.addButton(tr("Yes, Create new folder"), QMessageBox::YesRole);
        newFolder.addButton(QMessageBox::Cancel);
        newFolder.setText("'" + ratNumber + "' folder does not currently exist in the '" + saveDirectoryPath + "' directory. Would you like to create one?");
        newFolder.exec();
        if (newFolder.clickedButton() == connectButton) {
            // connect
            QDir().mkdir(saveDirectoryPath+ "/" + ratNumber);
        }
        else{
            continueSaving = false;
        }
    }
    if(continueSaving){
        if (!ratNumber.isEmpty()){
            //save the base station log
            saveName1 = QFileDialog::getSaveFileName(this,
                tr("Save Base Station Log"), saveDirectoryPath + "/" + ratNumber + "/" + startTime +"_base station",tr("(*.csv)") );
            qDebug()<<saveName1;
            if (!saveName1.isEmpty()){
                QFile file(saveName1);
                file.open(QIODevice::WriteOnly | QIODevice::Text);
                QTextStream out(&file);
                QString log = baseMonitor->toPlainText();
                out << log;
                file.close();
                //Disconnect the com port if connected.
                if(baseConnected){
                    connectBasePort();
                    fillBasestationPorts();
                }
                rigSelect->clearSelection();
                ratSelect->clearSelection();
                baseMonitor->clear();
                debugOn = false;
                showDebug();
            }
        }
        //save the cerebro log
        saveName2 = QFileDialog::getSaveFileName(this,
            tr("Save Cerebro Log"), saveDirectoryPath + "/" + ratNumber + "/" + startTime+"_cerebroLog", tr("(*.csv)") );
        if (!saveName2.isEmpty()){
            QFile file2(saveName2);
            file2.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream out2(&file2);
            QString log2 = downloadMonitor->toPlainText();
            log2 = log2.mid(log2.indexOf("Ver,",Qt::CaseSensitive)); //remove any garbage that comes before the start of the log, indicated by "Ver,"
            out2 << log2;
            file2.close();
            downloadMonitor->clear();
            if(pythonEnabled){
                // Run python script to summarize data from base station and cerebro logs
                QProcess *process = new QProcess(this);
                QString plotArg;
                if (showHistogram){
                    plotArg = "save and histogram";
                }
                else{
                    plotArg = "save only";
                }
                QStringList pythonArgs;
                pythonArgs<<qApp->applicationDirPath()+"/python scripts/parseLogs.py"<<saveName1<<saveName2<<plotArg; //pass the two log file locations into the python script
                process->start("python",pythonArgs);
                process->waitForFinished(-1);
                QString errorString = process->readAllStandardError();
                //Display summary in a popup message
                QMessageBox alert;
                if (errorString.isEmpty()){
                    alert.setWindowTitle("Session Summary");
                    alert.setText(process->readAll());
                    alert.exec();
                }
                else{
        //            alert.setWindowTitle("Error");
        //            alert.setText(errorString);
        //            alert.exec();
                    QFile file3(saveDirectoryPath + "/" + ratNumber + "/" + saveTime.toString("yyyy_MM_dd_hh_mm")+"_errorText.txt");
                    file3.open(QIODevice::WriteOnly | QIODevice::Text);
                    QTextStream out3(&file3);
                    out3 << errorString;
                }
            }
        }
    }
}


void MainWindow::errorMsg()
{
//    qDebug()<<"serial error:"<<serial->error();
    if (((serial->error()==11)|(serial->error()==7)|(serial->error()==8)|(serial->error()==9)&& errorThrown==false)){                //http://doc.qt.io/qt-5/qserialport.html#SerialPortError-enum
        errorThrown = true;
        baseMonitor->textCursor().insertText("ERROR\r");
        connectBasePort();
        fillBasestationPorts();
        QMessageBox alert;
        alert.setText("Serial Communication Error");
        alert.setIcon(QMessageBox::Warning);
        alert.setWindowTitle("Cerebro Control");
        alert.exec();
    }
}



void MainWindow::updateFilter(){
    if (singleShot->isChecked()){
        baseFilter = startDelay_spn->value() + onTime_spn->value();
    }
    else{
        baseFilter = startDelay_spn->value()+ trainDuration_spn->value();
    }
    QString msg = "F,"+ QString::number(baseFilter);
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
    QMessageBox baseUpdated;
        baseUpdated.setWindowTitle("Command Filter Updated");
        baseUpdated.setIcon(QMessageBox::Information);
        baseUpdated.setText("Base Station's command filter has automatically been updated to <b>" + QString::number(baseFilter) + " ms</b>");
    baseUpdated.exec();
    QTimer::singleShot(500, this, SLOT(clearMonitor()));
 }


void MainWindow::EEPROM(){
    QString msg = "E";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
    if (!(connect2_btn->isChecked())){ //prevents download port dropdown text from being cleared if already connected
        fillDownloaderPorts();
    }
}

void MainWindow::abort(){
    QString msg = "A";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
}
void MainWindow::macro(){
    QString msg = macroText->text();
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
}

void MainWindow::getBatteryStatus(){
    QString msg = "B";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
}

void MainWindow::getInfo(){
    QString msg = "I";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
}

void MainWindow::gotoDir(){
    //get the default save directory
    QSettings settings("Bobcat Engineering","CCS");
    settings.beginGroup("Saving");
    QString saveDirectoryPath = settings.value("DefaultDir").toString();
    settings.endGroup();
    if(saveDirectoryPath.isEmpty()){
        saveDirectoryPath = QDir::toNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    }
    QDesktopServices::openUrl(QUrl::fromLocalFile(saveDirectoryPath));
}

void MainWindow::gotoAppLocation(){
    QString saveDirectoryPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
    QDesktopServices::openUrl(QUrl::fromLocalFile(saveDirectoryPath));
}


void MainWindow::trainDur(){
    int total = onTime_spn->value()+offTime_spn->value();
    trainDuration_spn->setMinimum(total);
    if(total>0){
        QString freq =  QString::number(1000.0/total,'f',2);
        QString dutyCycle =  QString::number(onTime_spn->value()*100.0/total,'f',2);
        trainDescription_lbl->setText("(" + freq + " Hz, " + dutyCycle + "% duty cycle)");
    }
}

void MainWindow::showDebug(){
    debugCheckbox->setChecked(debugOn);
    if (debugOn){
        rigSelect->setEnabled(false);
        ratSelect->setEnabled(false);
        rigSelect->clearSelection();
        ratSelect->clearSelection();
        connect_btn->setText("Start Session (Debug Mode)");
        toggleDebug->setText("Exit Debug Mode");
    }
    else{
        rigSelect->setEnabled(true);
        ratSelect->setEnabled(true);
        connect_btn->setText("Start Session");
        toggleDebug->setText("Enter Debug Mode");
    }
}

void MainWindow::fadeChecked(){
    fade_spn->setVisible(fade_checkbox->isChecked());
}

void MainWindow::openDocs(){
//    opens readthedocs.com documentation
    QUrl site = QUrl::fromEncoded( "https://karpova-lab.github.io/cerebro/Software/Xavier.html");
    QDesktopServices::openUrl(site);
//    //opens local html file
//    QProcess *process = new QProcess(this);
//    process->start("explorer.exe", QStringList() << QDir::toNativeSeparators(qApp->applicationDirPath()+"/local docs/Software/Xavier.html"));
}



void MainWindow::getGraphs()
{
    //get the default save directory
    QSettings settings("Bobcat Engineering","CCS");
    settings.beginGroup("Saving");
    QString saveDirectoryPath = settings.value("DefaultDir").toString();
    settings.endGroup();
    QString baseData = QFileDialog::getOpenFileName(this,tr("Select Base Station Log File"),saveDirectoryPath,tr("(*station.csv)"));
    if (baseData!=""){
        //the cerebro data should be in the same folder, so we have the getOpenFileName dialog start in the the same folder
        QString betterPath = baseData.mid(saveDirectoryPath.length(),baseData.indexOf("/",saveDirectoryPath.length()+1)-saveDirectoryPath.length())+"/";
        QString cerData = QFileDialog::getOpenFileName(this,tr("Select Base Station Log File"),betterPath,tr("(*cerebroLog.csv)"));
        if (cerData!=""){
            //Run python script for creating graph
            QProcess *process = new QProcess(this);
            QString plotArg;
            if (debugOn){
                plotArg = "alignment and histogram and outcsv";
                debugOn = false;
                showDebug();
            }
            else{
                plotArg = "alignment and histogram";
            }
            QStringList pythonArgs;
            pythonArgs<<qApp->applicationDirPath()+"/python scripts/parseLogs.py"<<baseData<<cerData<<plotArg; //pass the two log file locations into the python script
            process->start("python",pythonArgs);
            process->waitForFinished(-1);
            QString errorString = process->readAllStandardError();
            QString printedString = process->readAll();
            //Display summary in a popup message
            QMessageBox alert;
//            QFont codefont;
//            codefont.setPointSize(6);
//            alert.setFont(codefont);
            if (!errorString.isEmpty()){
                alert.setWindowTitle("Python Error");
                alert.setText(errorString);
                alert.exec();
            }
            else{
                alert.setWindowTitle("Sesssion Summary");
                alert.setText(printedString);
                alert.exec();
            }
        }
    }
}

void MainWindow::setDebug()
{
    if (!baseConnected){
        debugOn = !debugOn;
        showDebug();
    }
}




void MainWindow::sendCalVector(){
    bool cerNumisInt,ldNumisInt;
    int cerNum = cerebroNum_edit->text().toInt(&cerNumisInt);
    int ldNum = ldNum_edit->text().toInt(&ldNumisInt);
    qDebug()<<!cerNumisInt<<!ldNumisInt<<(cerNum<0)<<(ldNum<0);
    if  (!cerNumisInt || !ldNumisInt || cerNum<0 || ldNum<0){
        QMessageBox calError;
        calError.setWindowTitle("Error");
        calError.setIcon(QMessageBox::Warning);
        calError.setStandardButtons(QMessageBox::Ok);
        calError.setText("Please fill in Cerebro# and LD# with postive integers");
        calError.exec();
    }
    else{
        QMessageBox confirmUpdate;
        confirmUpdate.setWindowTitle("Confirm Update");
        confirmUpdate.setIcon(QMessageBox::Question);
        confirmUpdate.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
        confirmUpdate.setDefaultButton(QMessageBox::Cancel);
        confirmUpdate.setEscapeButton(QMessageBox::Cancel);
        confirmUpdate.setText("Are you sure you want to update Cerebro #" + cerebroNum_edit->text() + "'s power level and corresponding fade vector?");
        if (confirmUpdate.exec() == QMessageBox::Yes){
            sendCal_btn->setEnabled(false);
            QString msg = "X";
            serial->write(msg.toLocal8Bit());
            QTimer::singleShot(500, this, SLOT(sendCalGroups()));
        }
    }
}

void MainWindow::sendCalGroups(){
    QString calibrationString = codeTextBox->toPlainText();
    QStringList calibrationGroups = calibrationString.split("\n",QString::SkipEmptyParts);
    if(calibrationGroups.size()>0){
        serial->write(calibrationGroups.takeFirst().toLocal8Bit());
        codeTextBox->clear();
        for (int i = 0; i<calibrationGroups.size(); i++){
            codeTextBox->insertPlainText(calibrationGroups[i]+"\n");
        }
        QTimer::singleShot(475, this, SLOT(sendCalGroups()));
    }
    else{
        isFirstTime = true;
        QTimer::singleShot(1000,this,SLOT(sendHardwareVals()));

    }
}

void MainWindow::sendHardwareVals(){
    if (isFirstTime){
        QString msg = "H";
        serial->write(msg.toLocal8Bit());
        QTimer::singleShot(500, this, SLOT(sendHardwareVals()));
        isFirstTime = false;
    }
    else{
        QString msg = cerebroNum_edit->text()+"," + ldNum_edit->text() + ",0,0,0";
        qDebug()<<msg<<"sent";
        serial->write(msg.toLocal8Bit());
        cerebroNum_edit->clear();
        ldNum_edit->clear();
        sendCal_btn->setEnabled(true);
    }
}

void MainWindow::sendNewPower(){
    isFirstTime = true;
    QTimer::singleShot(1000,this,SLOT(powerSending()));
}

void MainWindow::powerSending(){
    if (isFirstTime){
        QString msg = "P";
        serial->write(msg.toLocal8Bit());
        QTimer::singleShot(500, this, SLOT(powerSending()));
        isFirstTime = false;
    }
    else{
        QString msg = power_spn->text() + ",0,0,0,0";
        qDebug()<<msg<<"sent";
        serial->write(msg.toLocal8Bit());
    }
}

/*``````````````````````Calibration functions`````````````````````*/


void MainWindow::chooseFile(){
//    fileLabel->setText("");
    codeTextBox->clear();
    QString pathFromDialog = QFileDialog::getOpenFileName(this,tr("Select Power Meter Data"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),tr("(*.txt)"));
    if(!pathFromDialog.isEmpty()){
        // Run python script to summarize data from base station and cerebro logs
        getCalVals(pathFromDialog);
    }
    else{
        selectFile_btn->clearFocus();
    }
}


void MainWindow::useDropped(const QMimeData *mimeData)
{
//    fileLabel->setText("");
    codeTextBox->clear();
    QString dataPath =  mimeData->text().simplified();
    dataPath.remove("file:///");
    getCalVals(dataPath);
}

void MainWindow::getCalVals(QString calibrateDataPath ){
    QProcess *process = new QProcess(this);
    QStringList pythonArgs;
    pythonArgs<<qApp->applicationDirPath()+"/python scripts/getCalibrationVec.py"<<"\""+calibrateDataPath+"\""<<wantedLevel->text()<<QString::number(showGraph->isChecked()); //pass the calibration data into python script
    process->start("python",pythonArgs);
    process->waitForFinished(-1);
    QString errorString = process->readAllStandardError();
    QString resultString = process->readAll();
    codeTextBox->insertPlainText(resultString);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(resultString);
    QMessageBox alert;
    if (!errorString.isEmpty()){
        alert.setWindowTitle("Error Message from Python");
        alert.setText(errorString);
        alert.exec();
    }
    createFadeDialog->close();
}

void MainWindow::sendImplantStart()
{
    QString msg = "Z";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<" Sent";
}

void MainWindow::sendDiodeStart()
{
    QString msg = "K";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<" Sent";
}

//void MainWindow::getJSON(){
//    QString val;
//    QFile file;
//    file.setFileName(qApp->applicationDirPath()+"/test.json");
//    file.open(QIODevice::ReadOnly | QIODevice::Text);
//    val = file.readAll();
//    file.close();
//    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
//    QJsonObject topLevel = doc.object();
//    QJsonValue rigVallArray = topLevel.value(QString("rigVals"));
//    qDebug() <<rigVallArray.toArray()[0].toString()<< rigVallArray.toArray()[1].toString();
//}
