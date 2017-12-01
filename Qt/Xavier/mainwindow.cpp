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
        xavierVersion = "3.3.0";
        QString aboutString = "\t"+xavierVersion+"\nUpdated:\t12/1/2017";
        aboutDialog->setText("Version:"+aboutString);
        aboutDialog->setStandardButtons(QMessageBox::Close);

    //Experimental setup
    equipmentBox = new QGroupBox("Session Setup");
        equipmentLayout = new QGridLayout();
            rig_lbl = new QLabel("Rig #");
        equipmentLayout->addWidget(rig_lbl,0,1,Qt::AlignCenter);
            rat_lbl = new QLabel("Rat-implant: (LSet/RSet)");
        equipmentLayout->addWidget(rat_lbl,0,2,1,2,Qt::AlignCenter);
            rigSelect = new QListWidget();
        equipmentLayout->addWidget(rigSelect,1,1,4,1,Qt::AlignTop);
            ratSelect = new QListWidget();
            ratSelect->setMinimumWidth(150);
        equipmentLayout->addWidget(ratSelect,1,2,4,2,Qt::AlignTop|Qt::AlignHCenter);
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

    //Cerebro Status
    cerStatusBox = new QGroupBox("Cerebro Status");
        cerStatusLayout = new QGridLayout();
            serialNumber_lbl = new QLabel("Serial #\n");
            serialNumber_lbl->setAlignment(Qt::AlignCenter);
            serialNumber_lbl->setWordWrap(true);
        cerStatusLayout->addWidget(serialNumber_lbl,0,0);
            cerFirmware_lbl = new QLabel("Firmware\n");
            cerFirmware_lbl->setAlignment(Qt::AlignCenter);
            cerFirmware_lbl->setWordWrap(true);
        cerStatusLayout->addWidget(cerFirmware_lbl,0,1);
            Lset_lbl = new QLabel("Lset\n");
            Lset_lbl->setAlignment(Qt::AlignCenter);
            Lset_lbl->setWordWrap(true);
        cerStatusLayout->addWidget(Lset_lbl,0,2);
            Rset_lbl = new QLabel("Rset\n");
            Rset_lbl->setAlignment(Qt::AlignCenter);
            Rset_lbl->setWordWrap(true);
        cerStatusLayout->addWidget(Rset_lbl,0,3);
            cerDelay_lbl = new QLabel("Delay\n");
            cerDelay_lbl->setAlignment(Qt::AlignCenter);
            cerDelay_lbl->setWordWrap(true);
        cerStatusLayout->addWidget(cerDelay_lbl,0,4);
            cerOn_lbl = new QLabel("On\n");
            cerOn_lbl->setAlignment(Qt::AlignCenter);
            cerOn_lbl->setWordWrap(true);
        cerStatusLayout->addWidget(cerOn_lbl,0,5);
            cerOff_lbl = new QLabel("Off\n");
            cerOff_lbl->setAlignment(Qt::AlignCenter);
            cerOff_lbl->setWordWrap(true);
        cerStatusLayout->addWidget(cerOff_lbl,0,6);
            cerTrain_lbl = new QLabel("Train\n");
            cerTrain_lbl->setAlignment(Qt::AlignCenter);
            cerTrain_lbl->setWordWrap(true);
        cerStatusLayout->addWidget(cerTrain_lbl,0,7);
            cerRamp_lbl = new QLabel("Ramp\n");
            cerRamp_lbl->setAlignment(Qt::AlignCenter);
            cerRamp_lbl->setWordWrap(true);
        cerStatusLayout->addWidget(cerRamp_lbl,0,8);
            batteryIndicator = new QProgressBar();
            batteryIndicator->setOrientation(Qt::Horizontal);
            batteryIndicator->setAlignment(Qt::AlignHCenter);
            batteryIndicator->setMinimumWidth(30);
            batteryIndicator->setValue(0);
            batteryIndicator->setMinimum(0);
            batteryIndicator->setMaximum(100);
            batteryIndicator->setTextVisible(false);
            batteryIndicator->setFormat("Battery=%p%");
        cerStatusLayout->addWidget(batteryIndicator,1,1,1,8);
            battery_lbl = new QLabel(batteryIndicator->text());
        cerStatusLayout->addWidget(battery_lbl,1,0,1,1,Qt::AlignRight);
            getInfo_btn = new QPushButton("Check Wireless Connection / Update Battery Status");
        cerStatusLayout->addWidget(getInfo_btn,2,0,1,9,Qt::AlignCenter);
    cerStatusBox->setLayout(cerStatusLayout);
    cerStatusBox->setStyleSheet("");
    cerStatusBox->setEnabled(false);

    //Base station monitor
    baseBox = new QGroupBox("Base Station Monitor");
        serialMonitorLayout = new QGridLayout();
        clearBase_btn = new QPushButton();
            clearBase_btn->setText("Restart Session");
        serialMonitorLayout->addWidget(clearBase_btn,0,4,1,2,Qt::AlignRight);
            baseFilter_label = new QLabel("Filter Duration:");
        serialMonitorLayout->addWidget(baseFilter_label,0,0);
            baseMonitor = new QPlainTextEdit();
            baseMonitor->setMinimumHeight(380);
        serialMonitorLayout->addWidget(baseMonitor,1,0,1,6);
            saveMonitor_btn = new QPushButton();
            saveMonitor_btn->setText("Save Session");
            saveMonitor_btn->setMinimumHeight(40);
            saveMonitor_btn->setEnabled(true);
        serialMonitorLayout->addWidget(saveMonitor_btn,2,0,1,6);
    baseBox->setMinimumWidth(300);
    #ifdef __APPLE__
        baseMonitor->setMinimumWidth(325);
    #endif
    baseBox->setLayout(serialMonitorLayout);
    baseBox->setEnabled(false);


    //Waveform Adjustment
    adjustBox = new QGroupBox("Waveform Parameters");
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
            startDelay_checkbox = new QCheckBox("Start Delay");
        adjustmentLayout->addWidget(startDelay_checkbox,3,0,Qt::AlignRight);
            startDelay_spn = new QSpinBox();
            startDelay_spn->setRange(0,65535);
            startDelay_spn->setSingleStep(50);
            startDelay_spn->setSuffix(" ms");
            startDelay_spn->setAlignment(Qt::AlignCenter);
            startDelay_spn->setVisible(false);
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
            trainDuration_spn->setRange(0,9999999);
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
    adjustBox->setLayout(adjustmentLayout);
    adjustBox->setMinimumWidth(300);
    adjustBox->setEnabled(false);
    adjustBox->setPalette(Qt::gray);

    //Characterization
    charBox = new QGroupBox("Implant Construction Tools");
        charLayout = new QGridLayout();
            leftDiode_spn = new QSpinBox();
            leftDiode_spn->setRange(0,1023);
            leftDiode_spn->setAlignment(Qt::AlignCenter);
        charLayout->addWidget(leftDiode_spn,0,0,1,1);
            leftTest_btn = new QPushButton("Test\nLeft Diode");
        charLayout->addWidget(leftTest_btn,0,1,1,1);
            rightDiode_spn = new QSpinBox();
            rightDiode_spn->setRange(0,1023);
            rightDiode_spn->setAlignment(Qt::AlignCenter);
        charLayout->addWidget(rightDiode_spn,0,2,1,1);
            rightTest_btn = new QPushButton("Test\nRight Diode");
        charLayout->addWidget(rightTest_btn,0,3,1,1);
            setDiode_btn = new QPushButton("Set Both Laser Diode Powers");
        charLayout->addWidget(setDiode_btn,1,0,1,4);
            combinedTest_btn = new QPushButton("Combined Test");
        charLayout->addWidget(combinedTest_btn,2,0,1,4);
    charBox->setLayout(charLayout);
    charBox->setEnabled(false);
    charBox->setPalette(Qt::gray);

    //Triggering & debugging
    bugBox = new QGroupBox("Debug");
        triggerLayout = new QGridLayout();
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
            macroText = new QLineEdit();
//                macroText->setFixedWidth(40);
        triggerLayout->addWidget(macroText,4,0,Qt::AlignRight);
            macro_btn = new QPushButton("Send Text");
        triggerLayout->addWidget(macro_btn,4,1);
            startDiode_btn = new QPushButton("Cerebro Monitor");
        triggerLayout->addWidget(startDiode_btn,6,0,1,2);
    bugBox->setLayout(triggerLayout);
    bugBox->setEnabled(false);
    bugBox->setPalette(Qt::gray);

    //Download Monitor
    downloaderDialog = new QDialog();
        connectionLayout2 = new QGridLayout();
            connectLU_label = new QLabel("Cerebro Serial Port");
        connectionLayout2->addWidget(connectLU_label,0,0,1,3,Qt::AlignCenter);
            refresh2_btn = new QPushButton("Rescan");
        connectionLayout2->addWidget(refresh2_btn,1,0);
            serialPortList2 = new QComboBox();
            serialPortList2->setMinimumWidth(150);
        connectionLayout2->addWidget(serialPortList2,1,1,1,2);
            connect2_btn = new QPushButton("Connect to Cerebro");
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
            downloadMonitor->setMinimumHeight(400);
            downloadMonitor->setEnabled(false);
        connectionLayout2->addWidget(downloadMonitor,4,0,1,3);
        connectionLayout2->setRowStretch(4,1);
    downloaderDialog->setLayout(connectionLayout2);

    //Session Start Dialog
    sessionStartDialog = new QDialog();
        sessionStartLayout = new QGridLayout();
            retry_btn = new QPushButton("Retry");
        sessionStartLayout->addWidget(retry_btn,0,0);
            sessionStartMonitor = new QPlainTextEdit();
        sessionStartLayout->addWidget(sessionStartMonitor,1,0);
            baseConnected_lbl = new QLabel("Base Station Connected \u2718");
            baseConnected_lbl->setAlignment(Qt::AlignCenter);
        sessionStartLayout->addWidget(baseConnected_lbl,2,0);
            cerebroConnected_lbl = new QLabel("Cerebro Wireless Connection \u2718");
            cerebroConnected_lbl->setAlignment(Qt::AlignCenter);
        sessionStartLayout->addWidget(cerebroConnected_lbl,3,0);
            implantSettingsMatch_lbl = new QLabel("Implant Settings Match \u2718");
            implantSettingsMatch_lbl->setAlignment(Qt::AlignCenter);
        sessionStartLayout->addWidget(implantSettingsMatch_lbl,4,0);
            startSession_btn = new QPushButton("Start Session");
            startSession_btn->setEnabled(true);
        sessionStartLayout->addWidget(startSession_btn,5,0);
        QFont codefont("Arial Unicode MS");
        baseConnected_lbl->setFont(codefont);
        cerebroConnected_lbl->setFont(codefont);
        implantSettingsMatch_lbl->setFont(codefont);
        baseConnected_lbl->setStyleSheet("color:red");
        cerebroConnected_lbl->setStyleSheet("color:red");
        implantSettingsMatch_lbl->setStyleSheet("color:red");
    sessionStartDialog->setLayout(sessionStartLayout);
    sessionStartDialog->setWindowTitle("Startup Sequence");
    sessionStartDialog->setWindowFlags(sessionStartDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);//removes "?" from dialog box

    //Add Elements to Main Layout
    mainLayout = new QGridLayout();
    mainLayout->addWidget(equipmentBox,0,0,1,3);
    mainLayout->addWidget(cerStatusBox,1,0,1,3);
    mainLayout->addWidget(baseBox,2,1,4,2);
    mainLayout->addWidget(adjustBox,2,0);
    mainLayout->addWidget(charBox,3,0);
    mainLayout->addWidget(bugBox,4,0);
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
    downloadConnected = false;
    inTestloop = false;
    testCount = 0;
    timer = new QTimer(this);
    onTimeString = "";
    offTimeString = "";
    sessionHasBegun = false;

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
    connect(gotoDocs,SIGNAL(triggered()),this,SLOT(openDocs()));
    connect(about,SIGNAL(triggered(bool)),aboutDialog,SLOT(exec()));
    connect(toggleDebug,SIGNAL(triggered()),this,SLOT(setDebug()));
    connect(debugCheckbox,SIGNAL(clicked(bool)),this,SLOT(setDebug()));
    connect(gotoSettings,SIGNAL(triggered()),settingsDlog,SLOT(openSettings()));

    //
    connect(startSession_btn,SIGNAL(clicked()),this,SLOT(startSession()));
    connect(sessionStartDialog,SIGNAL(rejected()),this,SLOT(connectBasePort()));
    connect(retry_btn,SIGNAL(clicked(bool)),this,SLOT(checkForBase()));

    //Port Connections
    connect(settingsDlog,SIGNAL(dialogClosed()),this,SLOT(applySettings()));
    connect(refresh_btn,SIGNAL(clicked()),this,SLOT(fillBasestationPorts()));
    connect(refresh2_btn,SIGNAL(clicked()),this,SLOT(fillDownloaderPorts()));
    connect(connect_btn,SIGNAL(clicked()),this,SLOT(connectBasePort()));
    connect(connect2_btn,SIGNAL(clicked()),this,SLOT(connectCerebroPort()));

    //Monitors
    connect(serial,SIGNAL(readyRead()),this,SLOT(readSerial()));
    connect(serial2,SIGNAL(readyRead()),this,SLOT(readLog()));
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(errorMsg()));
    connect(clearBase_btn,SIGNAL(clicked()),this,SLOT(checkForBase()));
    connect(clearDownload_btn,SIGNAL(clicked()),this,SLOT(clearMonitor2()));
    connect(saveMonitor_btn,SIGNAL(clicked()),this,SLOT(saveFile()));

    //Cerebro Parameters
    connect(onTime_spn,SIGNAL(valueChanged(int)),this,SLOT(trainDur()));
    connect(offTime_spn,SIGNAL(valueChanged(int)),this,SLOT(trainDur()));
    connect(startDelay_checkbox,SIGNAL(clicked()),this,SLOT(startDelayChecked()));
    connect(fade_checkbox,SIGNAL(clicked()),this,SLOT(fadeChecked()));
    connect(sendSettings_btn,SIGNAL(clicked()),this,SLOT(set()));
    connect(singleShot,SIGNAL(clicked()),this,SLOT(trainChecked()));
    connect(pulseTrain,SIGNAL(clicked()),this,SLOT(trainChecked()));

    //Characteriztion
    connect(leftTest_btn,SIGNAL(clicked()),this,SLOT(sendToDiode()));
    connect(leftDiode_spn,SIGNAL(editingFinished()),this,SLOT(sendToDiode()));
    connect(rightTest_btn,SIGNAL(clicked()),this,SLOT(sendToDiode()));
    connect(rightDiode_spn,SIGNAL(editingFinished()),this,SLOT(sendToDiode()));
    connect(setDiode_btn,SIGNAL(clicked()),this,SLOT(setDiodePower()));
    connect(combinedTest_btn,SIGNAL(clicked(bool)),this,SLOT(testCombined()));
    connect(startDiode_btn,SIGNAL(clicked()),downloaderDialog,SLOT(show()));

    //Debug Commands
    connect(macro_btn,SIGNAL(clicked()),this,SLOT(macro()));
    connect(macroText,SIGNAL(returnPressed()),this,SLOT(macro()));
    connect(abort_btn,SIGNAL(clicked()),this,SLOT(abort()));
    connect(getInfo_btn,SIGNAL(clicked(bool)),this,SLOT(getBatteryStatus()));
    connect(trigger_btn,SIGNAL(clicked()),this,SLOT(sendTrigger()));
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
//    toolMenu->setEnabled(pythonEnabled);
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
        connectCerebroPort(); //disconnect download port after settings change
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
            qDebug()<<"manufacturer"<<info.manufacturer();
            qDebug()<<info.portName();
            qDebug()<<info.productIdentifier();
            qDebug()<<info.serialNumber();
            if(info.manufacturer().contains("Cypress")){
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
    baseConnected_lbl->setText("Base Station Connection \u2718");
    baseConnected_lbl->setStyleSheet("color:red");
    cerebroConnected_lbl->setText("Cerebro Wireless Connection \u2718");
    cerebroConnected_lbl->setStyleSheet("color:red");
    implantSettingsMatch_lbl->setText("Implant Settings Match \u2718");
    implantSettingsMatch_lbl->setStyleSheet("color:red");
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
        baseBox->setEnabled(!baseConnected);
        cerStatusBox->setEnabled(!baseConnected);
        if(!baseConnected){ //connect to serial port
            //isolate the COMXX part of the port name
            QString tempPortName = serialPortList->currentText();
            tempPortName.remove(0,tempPortName.indexOf("("+usbTag)+1);
            tempPortName.remove(QChar (')'));
            //open serial connection
            qDebug()<<tempPortName;
            serial->setPortName(tempPortName);
            serial->setBaudRate(57600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            qDebug()<<"opening serial"<<serial->open(QIODevice::ReadWrite);
            qDebug()<<serial->errorString();
            connect_btn->setText("Disconnect");
            if(!debugOn){
                QStringList ratInfo = ratSelect->currentItem()->text().split(QRegExp("[:,\\-()\\s\\/]"),QString::SkipEmptyParts);
                qDebug()<<"rat info "<<ratInfo;
                ratNumber = ratInfo[0];
                titleLeftPower = ratInfo[2].toInt();
                titleRightPower = ratInfo[3].toInt();
                setWindowTitle("Rig " + rigSelect->currentItem()->text() + " Rat " + ratInfo[0] );}
            else{
                ratNumber = "9999";
                setWindowTitle("Debug Mode");
            }
            clearBase_btn->setEnabled(false);
            baseConnected = true;
            errorThrown = false;
            sessionStartMonitor->clear();
            if (!debugOn){
                QTimer::singleShot(750, this, SLOT(checkForBase()));
                sessionStartDialog->exec();
            }
            else{
                QTimer::singleShot(750, this, SLOT(clearMonitor()));
                sessionHasBegun=true;
            }
        }
        else{ //disconnect from serial port
            cerStatusBox->setStyleSheet("");
            QString time = "\r" + serialPortList->currentText() + " Disconnected - " + QDate::currentDate().toString() + " " + QTime::currentTime().toString()+ "\r---------------------------------------------------------\r";
            baseMonitor->textCursor().insertText(time);
            serial->close();
            setWindowTitle("Xavier");
            debugOn = false;
            showDebug();
            baseConnected = false;
            sessionHasBegun = false;
            clearMonitor();
            cerebroConnected_lbl->setText("Cerebro Wireless Connection \u2714");
            baseConnected_lbl->setText("Base Station Connected \u2718");
            cerebroConnected_lbl->setText("Cerebro Wireless Connection \u2718");
            implantSettingsMatch_lbl->setText("Implant Settings Match \u2718");
            baseConnected_lbl->setStyleSheet("color:green");
            cerebroConnected_lbl->setStyleSheet("color:green");
            implantSettingsMatch_lbl->setStyleSheet("color:green");
        }
    }
    connect_btn->setChecked(baseConnected);
}

void MainWindow::connectCerebroPort()
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
    if (serial->isOpen()){
        startTime = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm");
        QString startup =  QString("\n~~~~~~~~~New Session~~~~~~~~~\n"
                                   "Xavier Version,%1"
                                   "\nRig,%2"
                                   "\nRat,%3"
                                   "\nStart Time,%4").arg(xavierVersion,rigSelect->currentItem()->text(),ratNumber,startTime);
        baseMonitor->textCursor().insertText(startup);
        checkForBase();
        connect_btn->setEnabled(true);
        clearBase_btn->setEnabled(true);
    }
}

void MainWindow::readSerial()
{
    if(sessionHasBegun){
        baseMonitor->moveCursor(QTextCursor::End);
    }
    else{
        sessionStartMonitor->moveCursor(QTextCursor::End);
    }
    while(!serial->atEnd()){
        baseBuffer += serial->readAll();
    }
    int dataStart = baseBuffer.indexOf("*");
    int dataEnd = baseBuffer.indexOf("&");
    qDebug()<<"buffer,datastart,dataend"<<baseBuffer<<dataStart<<dataEnd;

    if (dataStart!=-1 && dataEnd!=-1){
        QString dataString = baseBuffer.mid(dataStart+1,dataEnd-dataStart-1);
        baseBuffer.remove(dataStart,dataEnd+1-dataStart);
        qDebug()<<"data removed, new buffer: "<<baseBuffer;
        QStringList dataFromBaseMsg = dataString.split("~");
        qDebug()<<"data = "<<dataFromBaseMsg;
        qDebug()<<"length "<<dataFromBaseMsg.length();
        cerStatusBox->setStyleSheet("");
        if (dataFromBaseMsg[0] == "Cerebro Info"){
            cerebroConnected_lbl->setText("Cerebro Wireless Connection \u2714");
            cerebroConnected_lbl->setStyleSheet("color:green");
            cerFirmware_lbl->setText("Firmware\n"+dataFromBaseMsg[1]);
            serialNumber_lbl->setText("Serial #\n"+dataFromBaseMsg[2]);
        }
        else if (dataFromBaseMsg[0] == "BaseOn"){
            baseConnected_lbl->setText("Base Station Connection \u2714");
            baseConnected_lbl->setStyleSheet("color:green");
        }
        else if (dataFromBaseMsg[0] == "Diode Powers"){
            Lset_lbl->setText("Lset\n"+dataFromBaseMsg[1]);leftDiode_spn->setValue(dataFromBaseMsg[1].toInt());
            Rset_lbl->setText("Rset\n"+dataFromBaseMsg[2]);rightDiode_spn->setValue(dataFromBaseMsg[2].toInt());
            if (!debugOn){
                if ((dataFromBaseMsg[1].toInt() != titleLeftPower) || (dataFromBaseMsg[2].toInt() != titleRightPower)){
                    QTimer::singleShot(500, this, SLOT(matchPowers()));
                }
                else{
                    implantSettingsMatch_lbl->setText("Implant Settings Match \u2714");
                    implantSettingsMatch_lbl->setStyleSheet("color:green");
                }
            }
        }
        else if (dataFromBaseMsg[0] == "Waveform"){
            cerDelay_lbl->setText("Delay\n"+dataFromBaseMsg[1]); startDelay_spn->setValue(dataFromBaseMsg[1].toInt());
            if (dataFromBaseMsg[1].toInt()){
                startDelay_checkbox->setChecked(true);
                startDelay_spn->setValue(dataFromBaseMsg[1].toInt());
            }
            else{
                startDelay_checkbox->setChecked(false);
            }
            cerOn_lbl->setText("On\n"+dataFromBaseMsg[2]); onTime_spn->setValue(dataFromBaseMsg[2].toInt());
            cerOff_lbl->setText("Off\n"+dataFromBaseMsg[3]); offTime_spn->setValue(dataFromBaseMsg[3].toInt());
            cerTrain_lbl->setText("Train\n"+dataFromBaseMsg[4]); int trainDurationData = dataFromBaseMsg[4].toInt();
            if(trainDurationData){
                pulseTrain->setChecked(true);
                trainChecked();
            }
            else{
                singleShot->setChecked(true);
                trainChecked();
            }
            trainDuration_spn->setValue(trainDurationData);
            cerRamp_lbl->setText("Ramp\n"+ dataFromBaseMsg[5]);
            if (dataFromBaseMsg[5].toInt()){
                fade_checkbox->setChecked(true);
                fade_spn->setValue(dataFromBaseMsg[5].toInt());
            }
            else{
                fade_checkbox->setChecked(false);
            }
            startDelayChecked();
            fadeChecked();
            updateFilter();
        }
        else if (dataFromBaseMsg[0] == "Battery"){
            qDebug()<<dataFromBaseMsg[1];
            cerStatusBox->setStyleSheet("");
            batteryIndicator->setValue(dataFromBaseMsg[1].toInt());
            battery_lbl->setText(batteryIndicator->text());
        }
        else if (dataFromBaseMsg[0] == 'X'){
            cerStatusBox->setStyleSheet("color:#ed0b0b");
        }
    }
    int newLineIndex = baseBuffer.lastIndexOf("\n");
    if (newLineIndex>-1 ){ //if we come the end of the line reaches the buffer, print the buffer to the monitor
        qDebug()<<"newline Index"<<newLineIndex<<baseBuffer.mid(0,newLineIndex+1)<<"||"<<baseBuffer.mid(newLineIndex+1,baseBuffer.length()-newLineIndex-1);
        if(sessionHasBegun){
            baseMonitor->insertPlainText(baseBuffer.mid(0,newLineIndex+1));
        }
        else{
            sessionStartMonitor->insertPlainText(baseBuffer.mid(0,newLineIndex+1));
        }
        baseBuffer = baseBuffer.mid(newLineIndex+1,baseBuffer.length()-newLineIndex-1);
    }
    baseMonitor->ensureCursorVisible();
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
            QStringList dataFromBaseMsg = buffer.mid(andIndex+1,starIndex).split('~');
        }
    }
    downloadMonitor->moveCursor(QTextCursor::End);
    downloadMonitor->ensureCursorVisible();
}

void MainWindow::clearMonitor()
{
    baseMonitor->clear();
    baseFilter_label->setText("Filter Duration:");
    batteryIndicator->setValue(0);
    battery_lbl->setText(batteryIndicator->text());
    serialNumber_lbl->setText("Serial #\n");
    cerFirmware_lbl->setText("Firmware\n");
    Lset_lbl->setText("Lset\n");
    Rset_lbl->setText("Rset\n");
    cerDelay_lbl->setText("Delay\n");
    cerOn_lbl->setText("On\n");
    cerOff_lbl->setText("Off\n");
    cerTrain_lbl->setText("Train\n");
    cerRamp_lbl->setText("Ramp\n");
    QTimer::singleShot(1000, this, SLOT(sendTime()));
}

void MainWindow::clearMonitor2()
{
    downloadMonitor->clear();
}


void MainWindow::set()
{
    QString startDelay = "0";
    QString onTime = QString::number(onTime_spn->value());
    QString offTime = "0";
    QString trainDur = "0";
    QString fadeTime = "0";
    if(startDelay_checkbox->isChecked()){
        startDelay = QString::number(startDelay_spn->value());
    }
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
    QString msg = "W,"+ startDelay +","+ onTime +","+ offTime +"," + trainDur + "," + fadeTime+"\n";
    serial->write(msg.toLocal8Bit());
    qDebug()<<"message sent"<<msg;

}

void MainWindow::sendTrigger()
{
    QString msg = "T";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<" Sent";
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
    QString msg = "M";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<" Sent";
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
                tr("Save Base Station Log"), saveDirectoryPath + "/" + ratNumber + "/" + ratNumber+ "_" + startTime +"_baseLog",tr("(*.csv)") );
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
    }
}


void MainWindow::errorMsg()
{
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
    baseFilter_label->setText("Filter Duration: "+ QString::number(baseFilter) + " ms");
 }


void MainWindow::abort(){
    QString msg = "A\n";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
}
void MainWindow::macro(){
    QString msg = macroText->text()+"\n";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
}

void MainWindow::getBatteryStatus(){
    QString msg = "B\n";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
    cerStatusBox->setStyleSheet("color:#ed0b0b");                            
}

void MainWindow::checkForBase(){
    baseConnected_lbl->setText("Base Station Connection \u2718");
    baseConnected_lbl->setStyleSheet("color:red");
    cerebroConnected_lbl->setText("Cerebro Wireless Connection \u2718");
    cerebroConnected_lbl->setStyleSheet("color:red");
    implantSettingsMatch_lbl->setText("Implant Settings Match \u2718");
    implantSettingsMatch_lbl->setStyleSheet("color:red");
    QString msg = "N\n";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
}

void MainWindow::getInfo(){
    QString msg = "I\n";
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
        connect_btn->setText("Connect to Base Station (Debug Mode)");
        toggleDebug->setText("Exit Debug Mode");
        bugBox->setVisible(true);
        charBox->setVisible(true);
    }
    else{
        rigSelect->setEnabled(true);
        ratSelect->setEnabled(true);
        connect_btn->setText("Connect to Base Station");
        toggleDebug->setText("Enter Debug Mode");
        bugBox->setVisible(false);
        charBox->setVisible(false);
    }
}

void MainWindow::fadeChecked(){
    fade_spn->setVisible(fade_checkbox->isChecked());
}

void MainWindow::startDelayChecked(){
    startDelay_spn->setVisible(startDelay_checkbox->isChecked());
}

void MainWindow::openDocs(){
//    opens online documentation
    QUrl site = QUrl::fromEncoded( "https://karpova-lab.github.io/cerebro/Software/Xavier.html");
    QDesktopServices::openUrl(site);
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

void MainWindow::matchPowers(){
    sessionStartMonitor->textCursor().insertText("\nDiode parameter mismatch. Sending new diode values to Cerebro\n");
    QString msg = "D";
    QString leftPowerValue = QString::number(titleLeftPower);
    QString rightPowerValue = QString::number(titleRightPower);
    msg = msg + "," + leftPowerValue + "," + rightPowerValue + "\n";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<" Sent";
}

void MainWindow::sendToDiode()
{
    QString msg = "";
    QString value = "";
    if (sender()==leftTest_btn || (sender()==leftDiode_spn && leftDiode_spn->hasFocus())){
        msg = "l";
        value =  QString::number(leftDiode_spn->value());
    }
    else if (sender()==rightTest_btn || (sender()==rightDiode_spn && rightDiode_spn->hasFocus())){
        msg = "r";
        value =  QString::number(rightDiode_spn->value());
    }
    msg = msg + "," + value+"\n";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<" Sent";
}

void MainWindow::setDiodePower()
{
    QString msg = "D";
    QString leftPowerValue = QString::number(leftDiode_spn->value());
    qDebug()<<leftPowerValue<<" Sent";
    QString rightPowerValue = QString::number(rightDiode_spn->value());
    msg = msg + "," + leftPowerValue + "," + rightPowerValue + "\n";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<" Sent";
}

void MainWindow::testCombined()
{
    QString msg = "c\n";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<" Sent";
}

void MainWindow::startSession(){
    sessionStartDialog->accept();
    baseConnected_lbl->setText("Base Station Connection \u2718");
    baseConnected_lbl->setStyleSheet("color:red");
    cerebroConnected_lbl->setText("Cerebro Wireless Connection \u2718");
    cerebroConnected_lbl->setStyleSheet("color:red");
    implantSettingsMatch_lbl->setText("Implant Settings Match \u2718");
    implantSettingsMatch_lbl->setStyleSheet("color:red");
    sessionHasBegun=true;
    clearMonitor();
}
