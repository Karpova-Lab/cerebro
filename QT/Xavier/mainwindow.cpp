/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MIT License

Copyright (c) 2015-2016 Andy S. Lustig

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
#include "dropbutton.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Xavier");
    qApp->setStyleSheet("QListWidget::item:selected { background-color: gold; color: black}");

    //Menu bar
    fileMenu = menuBar()->addMenu("File");
        toggleDebug = new QAction(this);
        toggleDebug->setText("Enter Debug/Calibrate Mode");
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

    aboutDialog = new QMessageBox();
        aboutDialog->setWindowTitle("About");
        aboutDialog->setText("Version:\t1.21.0\nUpdated:\t10/06/2016");
        aboutDialog->setStandardButtons(QMessageBox::Close);

        //Experimental setup
        equipmentBox = new QGroupBox();
            equipmentLayout = new QGridLayout();
                rig_lbl = new QLabel("Rig #");
            equipmentLayout->addWidget(rig_lbl,0,1,Qt::AlignCenter);
                rat_lbl = new QLabel("Rat ID");
            equipmentLayout->addWidget(rat_lbl,0,2,Qt::AlignCenter);
                cerebro_lbl = new QLabel("Cerebro #");
            equipmentLayout->addWidget(cerebro_lbl,0,3,Qt::AlignCenter);
                rigSelect = new QListWidget();
            equipmentLayout->addWidget(rigSelect,1,1,2,1,Qt::AlignTop);
                ratSelect = new QListWidget();
            equipmentLayout->addWidget(ratSelect,1,2,2,1,Qt::AlignTop);
                cerebroSelect = new QListWidget();
            equipmentLayout->addWidget(cerebroSelect,1,3,2,1,Qt::AlignTop);
                connectBS_label = new QLabel("Base Station Serial Port");
            equipmentLayout->addWidget(connectBS_label,0,5,1,3,Qt::AlignCenter);
                refresh_btn = new QPushButton("Rescan");
            equipmentLayout->addWidget(refresh_btn,1,5);
                serialPortList = new QComboBox();
                serialPortList->setMinimumWidth(150);
            equipmentLayout->addWidget(serialPortList,1,6,1,2);
                debugCheckbox = new QCheckBox("Debug/Calibrate Mode");
            equipmentLayout->addWidget(debugCheckbox,3,1,1,2);
                connect_btn = new QPushButton("Start Session");
                connect_btn->setCheckable(true);
//                connect_btn->setStyleSheet("background-color: green; color:white");
            equipmentLayout->addWidget(connect_btn,3,3,1,5);
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
                baseMonitor = new QPlainTextEdit();
                baseMonitor->setMinimumWidth(275);
                baseMonitor->setMinimumHeight(300);
            serialMonitorLayout->addWidget(baseMonitor,1,0,1,6);
                eeprom_btn = new QPushButton();
                eeprom_btn->setText("(While the rat is still in the rig)\nClick to End Session");
            serialMonitorLayout->addWidget(eeprom_btn,2,0,1,6);
        baseBox->setLayout(serialMonitorLayout);

        //Pulse parameter adjustment box
        adjustBox = new QGroupBox("Cerebro's Waveform Parameters");
            adjustmentLayout = new QGridLayout();
                startDelay_spn = new QSpinBox();
                startDelay_spn->setRange(0,65535);
                startDelay_spn->setSingleStep(10);
                startDelay_spn->setAlignment(Qt::AlignCenter);
            adjustmentLayout->addWidget(startDelay_spn,0,1);
                startDelay_lbl = new QLabel("Start Delay");
                startDelay_lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            adjustmentLayout->addWidget(startDelay_lbl,0,0);
                singleShot = new QRadioButton("Single Shot");
                singleShot->setChecked(true);
            adjustmentLayout->addWidget(singleShot,1,0,Qt::AlignRight);
                pulseTrain = new QRadioButton("Pulse Train");
            adjustmentLayout->addWidget(pulseTrain,1,1);
                onTime_lbl = new QLabel("On Time");
                onTime_lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            adjustmentLayout->addWidget(onTime_lbl,2,0);
                onTime_spn = new QSpinBox();
                onTime_spn->setRange(0,65535);
                onTime_spn->setSingleStep(50);
                onTime_spn->setSuffix(" ms");
                onTime_spn->setAlignment(Qt::AlignCenter);
            adjustmentLayout->addWidget(onTime_spn,2,1);
                offTime_lbl = new QLabel("Off Time");
                offTime_lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                offTime_lbl->setVisible(false);
            adjustmentLayout->addWidget(offTime_lbl,3,0);
                offTime_spn = new QSpinBox();
                offTime_spn->setRange(0,65535);
                offTime_spn->setSingleStep(50);
                offTime_spn->setSuffix(" ms");
                offTime_spn->setAlignment(Qt::AlignCenter);
                offTime_spn->setVisible(false);
            adjustmentLayout->addWidget(offTime_spn,3,1);
                trainDescription_lbl  = new QLabel("");
                trainDescription_lbl->setVisible(false);
                trainDescription_lbl->setAlignment(Qt::AlignRight);
            adjustmentLayout->addWidget(trainDescription_lbl,4,0,1,2);
                trainDuration_lbl = new QLabel("Train Duration");
                trainDuration_lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                trainDuration_lbl->setVisible(false);
            adjustmentLayout->addWidget(trainDuration_lbl,5,0);
                trainDuration_spn = new QSpinBox();
                trainDuration_spn->setRange(0,65535);
                trainDuration_spn->setSingleStep(250);
                trainDuration_spn->setSuffix(" ms");
                trainDuration_spn->setAlignment(Qt::AlignCenter);
                trainDuration_spn->setVisible(false);
            adjustmentLayout->addWidget(trainDuration_spn,5,1);
                fade_checkbox = new QCheckBox("Ramp Down");
            adjustmentLayout->addWidget(fade_checkbox,6,0,Qt::AlignRight);
                fade_spn = new QSpinBox();
                fade_spn->setRange(100,65535);
                fade_spn->setSingleStep(100);
                fade_spn->setSuffix(" ms");
                fade_spn->setAlignment(Qt::AlignCenter);
                fade_spn->setVisible(false);
            adjustmentLayout->addWidget(fade_spn,6,1);
                sendSettings_btn = new QPushButton("Send New Parameters");
                sendSettings_btn->setAutoDefault(true);
            adjustmentLayout->addWidget(sendSettings_btn,7,0,1,2);
                last_settings = new QLabel();
                last_settings->setText("No settings sent yet");
            adjustmentLayout->addWidget(last_settings,8,0,1,2,Qt::AlignTop);
        adjustBox->setLayout(adjustmentLayout);
        adjustBox->setMinimumWidth(235);

        //Base station Settings
        baseSettingsBox = new QGroupBox("Base Station Control");
            baseSettingLayout = new QGridLayout();
                filterLabel = new QLabel("Command Filter");
                filterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            baseSettingLayout->addWidget(filterLabel,0,0);
                baseFilter_spn = new QSpinBox();
                baseFilter_spn->setRange(0,65535);
                baseFilter_spn->setSingleStep(250);
                baseFilter_spn->setSuffix(" ms");
                baseFilter_spn->setAlignment(Qt::AlignCenter);
            baseSettingLayout->addWidget(baseFilter_spn,0,1);
                filter_btn = new QPushButton("Update Filter");
            baseSettingLayout->addWidget(filter_btn,1,0,1,2);
                lamp_btn = new QPushButton("Toggle IR Lamp");
//            baseSettingLayout->addWidget(lamp_btn,2,0,1,2);
        baseSettingsBox->setLayout(baseSettingLayout);


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
                macroText = new QLineEdit();
                macroText->setFixedWidth(40);
            triggerLayout->addWidget(macroText,4,0,Qt::AlignRight);
                macro_btn = new QPushButton("Send Text");
            triggerLayout->addWidget(macro_btn,4,1);
        bugBox->setLayout(triggerLayout);

        chooseBox = new QGroupBox("Calibrate");
            chooseLayout = new QGridLayout();
                wantedLevel = new QSlider(Qt::Horizontal);
                wantedLevel->setRange(0,10);
                wantedLevel->setValue(4);
                wantedLevel->setTickPosition(QSlider::TicksBelow);
                wantedLevel->setMaximumWidth(150);
            chooseLayout->addWidget(wantedLevel,1,0,2,1);
                slideLabel = new QLabel("Target Power: "+QString::number(wantedLevel->value())+ " mW");
            chooseLayout->addWidget(slideLabel,0,0,Qt::AlignCenter);
                showGraph = new QCheckBox("Show Graphs");
                showGraph->setChecked(true);
            chooseLayout->addWidget(showGraph,0,1,Qt::AlignCenter);
                selectFile_btn = new DropButton;//("Select Power Meter\ndata file");
                selectFile_btn->setCheckable(true);
                selectFile_btn->setAcceptDrops(true);
                selectFile_btn->setCheckable(false);
//                selectFile_btn->setMinimumHeight(75);
            chooseLayout->addWidget(selectFile_btn,1,1,2,1);
                codeTextBox = new QPlainTextEdit();
//                QFont codefont;
//                codefont.setPointSize(5);
//                codeTextBox->setFont(codefont);
            chooseLayout->addWidget(codeTextBox,3,0,1,3);
            sendCal_btn = new QPushButton("Send Calibration Vector");
            chooseLayout->addWidget(sendCal_btn,4,0,1,3);
        chooseBox->setLayout(chooseLayout);


        QPixmap pictcha(":myFiles/headerpicc.png");
                picLabel = new QLabel();
                picLabel->setPixmap(pictcha);
                picLabel->setAlignment(Qt::AlignCenter);

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

    mainLayout = new QGridLayout();
//    mainLayout->addWidget(picLabel,0,0,1,3);
    mainLayout->addWidget(equipmentBox,0,0,1,3);
    mainLayout->addWidget(baseSettingsBox,1,0);
    mainLayout->addWidget(adjustBox,2,0,1,1);
    mainLayout->addWidget(chooseBox,3,0,1,1);
    mainLayout->addWidget(bugBox,4,0,1,1);
    mainLayout->addWidget(baseBox,1,1,4,2);
    mainLayout->setColumnStretch(2,1);
    mainLayout->addWidget(downloaderBox,0,3,5,1);
    mainLayout->setRowStretch(5,1);
    adjustBox->setEnabled(false);
    adjustBox->setPalette(Qt::gray);
    bugBox->setEnabled(false);
    bugBox->setPalette(Qt::gray);
    chooseBox->setEnabled(false);
    chooseBox->setPalette(Qt::gray);
    baseBox->setEnabled(false);
    baseSettingsBox->setEnabled(false);
    baseSettingsBox->setPalette(Qt::gray);
    this->setFixedSize(this->maximumSize());

    QWidget *window = new QWidget();
    window->setLayout(mainLayout);
    setCentralWidget(window);
    debugOn = false;

    settingsDialog = new QDialog();
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

        //Dropdown editor
        dropDownsBox = new QGroupBox("Edit Dropdown Lists");
            //1st listbox//
            dropDownsLayout = new QGridLayout();
                rigLabel = new QLabel("Rig #");
            dropDownsLayout->addWidget(rigLabel,0,0,1,2,Qt::AlignCenter);
                rigVals = new QListWidget();
            dropDownsLayout->addWidget(rigVals,2,0,1,2);
                add1_btn= new QPushButton("Add Rig #");
                add1_btn->setFocusPolicy(Qt::NoFocus);
            dropDownsLayout->addWidget(add1_btn,1,1);
                newItem1 = new QLineEdit();
            dropDownsLayout->addWidget(newItem1,1,0);
                rmv1_btn= new QPushButton("Remove Selected Rig #");
                rmv1_btn->setFocusPolicy(Qt::NoFocus);
            dropDownsLayout->addWidget(rmv1_btn,3,0,1,2);
            //2nd listbox//
                ratLabel = new QLabel("Rat ID");
            dropDownsLayout->addWidget(ratLabel,0,2,1,2,Qt::AlignCenter);
                ratVals = new QListWidget();
                ratVals->addItems(rigList);
            dropDownsLayout->addWidget(ratVals,2,2,1,2);
                add2_btn= new QPushButton("Add Rat ID");
                add2_btn->setFocusPolicy(Qt::NoFocus);
            dropDownsLayout->addWidget(add2_btn,1,3);
                newItem2 = new QLineEdit();
            dropDownsLayout->addWidget(newItem2,1,2);
                rmv2_btn= new QPushButton("Remove Selected Rat ID");
                rmv2_btn->setFocusPolicy(Qt::NoFocus);
            dropDownsLayout->addWidget(rmv2_btn,3,2,1,2);
            //3rd listbox//
                cerebroLabel = new QLabel("Cerebro #");
            dropDownsLayout->addWidget(cerebroLabel,0,4,1,2,Qt::AlignCenter);
                cerebroVals = new QListWidget();
                cerebroVals->addItems(rigList);
            dropDownsLayout->addWidget(cerebroVals,2,4,1,2);
                add3_btn= new QPushButton("Add Cerebro #");
                add3_btn->setFocusPolicy(Qt::NoFocus);
            dropDownsLayout->addWidget(add3_btn,1,5);
                newItem3 = new QLineEdit();
            dropDownsLayout->addWidget(newItem3,1,4);
                rmv3_btn= new QPushButton("Remove Selected Cerebro #");
                rmv3_btn->setFocusPolicy(Qt::NoFocus);
            dropDownsLayout->addWidget(rmv3_btn,3,4,1,2);
        dropDownsBox->setLayout(dropDownsLayout);

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
        editLabelDialog->setWindowFlags(settingsDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);//removes "?" from dialog box

        featuresBox = new QGroupBox("Other Settings");
            featuresLayout = new QGridLayout();
                pythonCheckbox = new QCheckBox("Enable Python Scripts");
                pythonCheckbox->setChecked(pythonEnabled);
            featuresLayout->addWidget(pythonCheckbox);
        featuresBox->setLayout(featuresLayout);

        settingsLayout = new QGridLayout();
        settingsLayout->addWidget(directoryBox);
        settingsLayout->addWidget(dropDownsBox);
        settingsLayout->addWidget(portEditBox);
        settingsLayout->addWidget(featuresBox);

    settingsDialog->setWindowTitle("Settings");
    settingsDialog->setWindowFlags(settingsDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);//removes "?" from dialog box
    settingsDialog->setMaximumWidth(0);
    settingsDialog->setMaximumHeight(300);
    settingsDialog->setLayout(settingsLayout);


#ifdef _WIN32   //--------Windows USB parse settings------------
    usbTag = "COM";
    usbDescription = "USB Serial Port";
#elif __APPLE__ //---------Mac USB parse settings---------------
    usbTag  = "cu.usbserial";
    usbDescription = "FT230X Basic UART";
#endif

    //program setup
    serial = new QSerialPort(this);
    serial2 = new QSerialPort(this);
    first = true;
    refreshDrops();
    fillPortsInfo();
    fillPortsInfo2();
    isConnected = false;
    isConnected2 = false;
    inTestloop = false;
    testCount = 0;
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(sendTrigger()));
    connect(refresh_btn,SIGNAL(clicked()),this,SLOT(fillPortsInfo()));
    connect(refresh2_btn,SIGNAL(clicked()),this,SLOT(fillPortsInfo2()));
    connect(connect_btn,SIGNAL(clicked()),this,SLOT(connectPort()));
    connect(connect2_btn,SIGNAL(clicked()),this,SLOT(connectDownloadPort()));
    connect(serial,SIGNAL(readyRead()),this,SLOT(readSerial()));
    connect(serial2,SIGNAL(readyRead()),this,SLOT(readLog()));
    connect(sendSettings_btn,SIGNAL(clicked()),this,SLOT(set()));
    connect(trigger_btn,SIGNAL(clicked()),this,SLOT(triggerPushed()));
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(errorMsg()));
    connect(trigger_checkbox,SIGNAL(clicked()),this,SLOT(triggerChecked()));
    connect(singleShot,SIGNAL(clicked()),this,SLOT(trainChecked()));
    connect(pulseTrain,SIGNAL(clicked()),this,SLOT(trainChecked()));
    connect(clearBase_btn,SIGNAL(clicked()),this,SLOT(clearMonitor()));
    connect(clearDownload_btn,SIGNAL(clicked()),this,SLOT(clearMonitor2()));
    connect(saveMonitor_btn,SIGNAL(clicked()),this,SLOT(saveFile()));
    connect(gotoSettings,SIGNAL(triggered()),this,SLOT(openSettings()));
    connect(eeprom_btn,SIGNAL(clicked()),this,SLOT(EEPROM()));
    connect(macro_btn,SIGNAL(clicked()),this,SLOT(macro()));
    connect(macroText,SIGNAL(returnPressed()),this,SLOT(macro()));
    connect(lamp_btn,SIGNAL(clicked()),this,SLOT(lamp()));
    connect(abort_btn,SIGNAL(clicked()),this,SLOT(abort()));
    connect(openDir,SIGNAL(triggered()),this,SLOT(gotoDir()));
    connect(gotoApplocation,SIGNAL(triggered()),this,SLOT(gotoAppLocation()));
    connect(gotoDocs,SIGNAL(triggered()),this,SLOT(openDocs()));
    connect(onTime_spn,SIGNAL(valueChanged(int)),this,SLOT(trainDur()));
    connect(offTime_spn,SIGNAL(valueChanged(int)),this,SLOT(trainDur()));
    connect(fade_checkbox,SIGNAL(clicked()),this,SLOT(fadeChecked()));
    connect(add1_btn,SIGNAL(clicked()),this,SLOT(addListItem()));
    connect(add2_btn,SIGNAL(clicked()),this,SLOT(addListItem()));
    connect(add3_btn,SIGNAL(clicked()),this,SLOT(addListItem()));
    connect(newItem1,SIGNAL(returnPressed()),this,SLOT(addListItem()));
    connect(newItem2,SIGNAL(returnPressed()),this,SLOT(addListItem()));
    connect(newItem3,SIGNAL(returnPressed()),this,SLOT(addListItem()));
    connect(rmv1_btn,SIGNAL(clicked()),this,SLOT(removeItem()));
    connect(rmv2_btn,SIGNAL(clicked()),this,SLOT(removeItem()));
    connect(rmv3_btn,SIGNAL(clicked()),this,SLOT(removeItem()));
    connect(settingsDialog,SIGNAL(rejected()),this,SLOT(refreshDrops()));
    connect(changeDir_btn,SIGNAL(clicked()),this,SLOT(setPath()));
    connect(addAlias_btn,SIGNAL(clicked()),this,SLOT(addAlias()));
    connect(newAlias,SIGNAL(returnPressed()),this,SLOT(addAlias()));
    connect(rmvAlias_btn,SIGNAL(clicked()),this,SLOT(removeAlias()));
    connect(about,SIGNAL(triggered(bool)),aboutDialog,SLOT(exec()));
    connect(graphResults,SIGNAL(triggered(bool)),this,SLOT(getGraphs()));
    connect(aliasWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(editLabel()));
    connect(changeLabel_btn,SIGNAL(clicked()),this,SLOT(addAlias()));
    connect(cancelChange_btn,SIGNAL(clicked()),editLabelDialog,SLOT(close()));
    connect(filter_btn,SIGNAL(clicked()),this,SLOT(updateFilter()));
    connect(toggleDebug,SIGNAL(triggered()),this,SLOT(setDebug()));
    connect(sendCal_btn,SIGNAL(clicked()),this,SLOT(sendCalVector()));
    connect(debugCheckbox,SIGNAL(clicked(bool)),this,SLOT(setDebug()));

    //Calibration signal&slots
    connect(selectFile_btn,SIGNAL(clicked()),this,SLOT(chooseFile()));
    connect(selectFile_btn, SIGNAL(dropped(const QMimeData*)),this, SLOT(useDropped(const QMimeData*)));
    connect(wantedLevel,SIGNAL(sliderMoved(int)),this,SLOT(slideValueUpdate(int)));
    connect(about,SIGNAL(triggered(bool)),aboutDialog,SLOT(exec()));
    connect(gotoDocs,SIGNAL(triggered()),this,SLOT(openDocs()));



}
//--------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isConnected){
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


void MainWindow::fillPortsInfo()
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

void MainWindow::fillPortsInfo2()
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

void MainWindow::connectPort()
{
    gotoSettings->setEnabled(isConnected);
    toggleDebug->setEnabled(isConnected);
    if((rigSelect->selectedItems().size()==0) & !isConnected){
        QMessageBox alert;
        alert.setText("Please select a Rig # to continue");
        alert.setIcon(QMessageBox::Warning);
        alert.setWindowTitle("Missing Rig #");
        alert.exec();
        connect_btn->setChecked(false);
        }
    else if((ratSelect->selectedItems().size()==0) & !isConnected){
        QMessageBox alert;
        alert.setText("Please select a Rat ID to continue");
        alert.setIcon(QMessageBox::Warning);
        alert.setWindowTitle("Missing Rat ID");
        alert.exec();
        connect_btn->setChecked(false);
    }
    else if((cerebroSelect->selectedItems().size()==0) & !isConnected){
        QMessageBox alert;
        alert.setText("Please select a Cerebro # to continue");
        alert.setIcon(QMessageBox::Warning);
        alert.setWindowTitle("Missing Cerebro #");
        alert.exec();
        connect_btn->setChecked(false);
    }
    else{
        rig_lbl->setEnabled(isConnected);
        rat_lbl->setEnabled(isConnected);
        cerebro_lbl->setEnabled(isConnected);
        rigSelect->setEnabled(isConnected);
        ratSelect->setEnabled(isConnected);
        cerebroSelect->setEnabled(isConnected);
        connectBS_label->setEnabled(isConnected);
        serialPortList->setEnabled(isConnected);
        refresh_btn->setEnabled(isConnected);
        debugCheckbox->setEnabled(isConnected);
        adjustBox->setEnabled(!isConnected);
        baseSettingsBox->setEnabled(!isConnected);
        bugBox->setEnabled(!isConnected && debugOn);
        chooseBox->setEnabled(!isConnected && debugOn);
        baseBox->setEnabled(!isConnected);
        connect_btn->setChecked(!isConnected);
        if(!isConnected){
            //isolate the COMXX part of the port name
            QString tempPortName = serialPortList->currentText();
            tempPortName.remove(0,tempPortName.indexOf("("+usbTag)+1);
            tempPortName.remove(QChar (')'));
            //open serial connection
            serial->setPortName(tempPortName);
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            serial->open(QIODevice::ReadWrite);
            connect_btn->setText("Disconnect");
//            connect_btn->setStyleSheet("background-color: grey; color:black");
            setWindowTitle("Rig " + rigSelect->currentItem()->text() + " Rat " + ratSelect->currentItem()->text() );
            QString rst = "R";
            serial->write(rst.toLocal8Bit());
            clearBase_btn->setEnabled(false);
            QTimer::singleShot(1000, this, SLOT(sendTime()));
            isConnected = 1;
            errorThrown = false;
        }
        else{
            if (inTestloop){
                triggerPushed();
            }
            QString time = "\r" + serialPortList->currentText() + " Disconnected - " + QDate::currentDate().toString() + " " + QTime::currentTime().toString()+ "\r---------------------------------------------------------\r";
            baseMonitor->textCursor().insertText(time);
            serial->close();
            if (debugOn){
                connect_btn->setText("Start Session (Debug/Calibrate Mode)");
            }
            else{
                connect_btn->setText("Start Session");
            }
//            connect_btn->setStyleSheet("background-color: green; color:white");
            setWindowTitle("Xavier");
            isConnected = 0;
        }
    }
}

void MainWindow::connectDownloadPort()
{
    QString tempPortName = serialPortList2->currentText();
    if (tempPortName.isEmpty() & !isConnected2){
        QMessageBox noSerial;
        noSerial.setText("Make sure a Downloader Cord is connected \nand click ''Rescan''' to see available Serial Ports.");
        noSerial.setIcon(QMessageBox::Warning);
        noSerial.setWindowTitle("No Available Serial Ports");
        noSerial.exec();
        connect2_btn->setChecked(false);
    }
    else{
        connectLU_label->setEnabled(isConnected2);
        serialPortList2->setEnabled(isConnected2);
        refresh2_btn->setEnabled(isConnected2);
        downloadMonitor->setEnabled(!isConnected2);
        download_title->setEnabled(!isConnected2);
        clearDownload_btn->setEnabled(!isConnected2);
        saveMonitor_btn->setEnabled(!isConnected2);
        if(!isConnected2){//if it's not connected, we'll connect it
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
            isConnected2 = true;
        }
        else{// if it is connected, we'll disconnect
            serial2->close();
            connect2_btn->setText("Connect To Downloader");
            isConnected2 = false;
            fillPortsInfo2();
        }
    }
}

void MainWindow::sendTime()
{
    QString time = QDate::currentDate().toString() + " " + QTime::currentTime().toString();
    QString startup = QString("%1,%2,%3,%4,").arg(time,rigSelect->currentItem()->text(),ratSelect->currentItem()->text(),cerebroSelect->currentItem()->text());
    serial->write(startup.toLocal8Bit());
    clearBase_btn->setEnabled(true);
}

void MainWindow::readSerial()
{
    QByteArray received;
    baseMonitor->moveCursor(QTextCursor::End);
    baseMonitor->insertPlainText(serial->readAll());
    baseMonitor->ensureCursorVisible();
}
void MainWindow::readLog()
{
    QByteArray received2;
    downloadMonitor->moveCursor(QTextCursor::End);
//    QString buffer = serial2->read(2);
//    startDelay_spn->setValue(buffer.toInt());
    downloadMonitor->insertPlainText(serial2->readAll());
    downloadMonitor->ensureCursorVisible();
}

void MainWindow::clearMonitor()
{
    baseMonitor->clear();
    if(isConnected){
        QString rst = "R";
        serial->write(rst.toLocal8Bit());
        clearBase_btn->setEnabled(false);
    }
    QTimer::singleShot(1000, this, SLOT(sendTime()));
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
        fadeTime = QString::number(fade_spn->value());
    }
    QString msg = startDelay +","+ onTime +","+ offTime +"," + trainDur + "," + fadeTime;
    if (singleShot->isChecked()){
        baseFilter_spn->setValue(startDelay_spn->value() + onTime_spn->value()+fade_spn->value() * fade_checkbox->isChecked());
    }
    else{
        baseFilter_spn->setValue(startDelay_spn->value()+ trainDuration_spn->value());
    }
    QMessageBox confirmUpdate;
        confirmUpdate.setWindowTitle("Confirm Parameter Change");
        confirmUpdate.setIcon(QMessageBox::Question);
        confirmUpdate.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
        confirmUpdate.setDefaultButton(QMessageBox::Cancel);
        confirmUpdate.setEscapeButton(QMessageBox::Cancel);
        confirmUpdate.setText("<b>Are you sure you want to send the following new parameters to Cerebro?</b>");
        confirmUpdate.setInformativeText("Start Delay:\t"+ startDelay+ " ms\nOn Time:\t" + onTime + " ms\nOff Time:\t" + offTime + " ms\nTrain Duration:\t" + trainDur + " ms\nFade Time:\t" + fadeTime + " ms");
    if (confirmUpdate.exec() == QMessageBox::Yes){
        qDebug()<<msg;
        serial->write(msg.toLocal8Bit());
        last_settings->setText("Last Settings Sent:\n" + startDelay  + ", " + onTime + ", " + offTime + ", " + trainDur + ", " + fadeTime );
        QTimer::singleShot(500, this, SLOT(updateFilter()));
        QMessageBox baseUpdated;
            baseUpdated.setWindowTitle("Base Station Updated");
            baseUpdated.setIcon(QMessageBox::Information);
            baseUpdated.setText("The command filter has automatically been updated to  <b>" + QString::number(baseFilter_spn->value()) + " ms</b> to match the settings sent to Cerebro");
        baseUpdated.exec();
    }
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
    //warn if there is no rat # to indicate which folder to save to
    QString ratNumber;
    if(debugOn){
        ratNumber = "9999";
    }
    else{
        ratNumber = ratSelect->currentItem()->text();
    }
    qDebug()<<debugOn;
    bool continueSaving = true;
    if(ratNumber==""){
        QMessageBox alert;
        alert.setText("Rat# is unknown, therefore this file can't be automatically saved to an existing rat folder.\nPlease select a folder you would like to save to.");
        alert.setIcon(QMessageBox::Warning);
        alert.setWindowTitle("Unknown Rat #");
        alert.exec();
    }
    //get the default save directory
    QSettings settings("Bobcat Engineering","CCS");
    settings.beginGroup("Saving");
    QString tempPath = settings.value("DefaultDir").toString();
    settings.endGroup();
    //if the default save directory hasn't been set yet, use the desktop as the save directory
    if(tempPath.isEmpty()){
        tempPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }
    QDateTime saveTime = saveTime.currentDateTime();
    //check if the rat# folder exists in the save directory, if not go ahead and make a new rat# folder
    if(!QDir(tempPath+ "/" + ratNumber).exists()){
        QMessageBox newFolder;
        QPushButton *connectButton = newFolder.addButton(tr("Yes, Create new folder"), QMessageBox::YesRole);
        newFolder.addButton(QMessageBox::Cancel);
        newFolder.setText("'" + ratNumber + "' folder does not currently exist in the '" + tempPath + "' directory. Would you like to create one?");
        newFolder.exec();
        if (newFolder.clickedButton() == connectButton) {
            // connect
            QDir().mkdir(tempPath+ "/" + ratNumber);
        }
        else{
            continueSaving = false;
        }
    }
    if(continueSaving){
        if (!ratNumber.isEmpty()){
            //save the base station log
            saveName1 = QFileDialog::getSaveFileName(this,
                tr("Save Base Station Log"), tempPath + "/" + ratNumber + "/" + saveTime.toString("yyyyMMMdd_hhmm")+"_base station",tr("(*.csv)") );
            qDebug()<<saveName1;
            if (!saveName1.isEmpty()){
                QFile file(saveName1);
                file.open(QIODevice::WriteOnly | QIODevice::Text);
                QTextStream out(&file);
                QString log = baseMonitor->toPlainText();
                out << log;
    //            file.waitForBytesWritten()
                file.close();
                //Disconnect the com port if connected.
                if(isConnected){
                    connectPort();
                    fillPortsInfo();
                }
                rigSelect->clearSelection();
                ratSelect->clearSelection();
                cerebroSelect->clearSelection();
                baseMonitor->clear();
                debugOn = false;
                showDebug();
            }
        }
        //save the cerebro log
        saveName2 = QFileDialog::getSaveFileName(this,
            tr("Save Cerebro Log"), tempPath + "/" + ratNumber + "/" + saveTime.toString("yyyyMMMdd_hhmm")+"_cerebroLog", tr("(*.csv)") );
        if (!saveName2.isEmpty()){
            QFile file2(saveName2);
            file2.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream out2(&file2);
            QString log2 = downloadMonitor->toPlainText();
            log2 = log2.mid(log2.indexOf("Ver,",Qt::CaseSensitive)); //remove any garbage that comes before the start of the log, indicated by "Ver,"
            out2 << log2;
            file2.close();
            downloadMonitor->clear();
            QTimer::singleShot(0,this,SLOT(shrink()));
            if(pythonEnabled){
                // Run python script to summarize data from base station and cerebro logs
                QProcess *process = new QProcess(this);
                QStringList pythonArgs;
                pythonArgs<<qApp->applicationDirPath()+"/python scripts/parseLogs.py"<<"\""+saveName1+"\""<<"\""+saveName2+"\""; //pass the two log file locations into the python script
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
                    QFile file3(tempPath + "/" + ratNumber + "/" + saveTime.toString("yyyyMMMdd_hhmm")+"_errorText.txt");
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
    qDebug()<<"serial error:"<<serial->error();
    if (((serial->error()==11)|(serial->error()==7)|(serial->error()==8)|(serial->error()==9)&& errorThrown==false)){                //http://doc.qt.io/qt-5/qserialport.html#SerialPortError-enum
        errorThrown = true;
        baseMonitor->textCursor().insertText("ERROR\r");
        connectPort();
        fillPortsInfo();
        QMessageBox alert;
        alert.setText("Serial Communication Error");
        alert.setIcon(QMessageBox::Warning);
        alert.setWindowTitle("Cerebro Control");
        alert.exec();
    }
}

void MainWindow::setPath(){
    QString savePath = QFileDialog::getExistingDirectory(this,tr("Set Default Directory"),QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    if (!savePath.isEmpty()){
        QSettings settings("Bobcat Engineering","CCS");
        settings.beginGroup("Saving");
        settings.setValue("DefaultDir",savePath);
        settings.endGroup();
        directoryLabel->setText("<b>Current Directory:</b><br>" + savePath);
    }
}

void MainWindow::updateFilter(){
    QString msg = "F,"+ QString::number(baseFilter_spn->value());
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
}

void MainWindow::lamp(){
    QString msg = "L";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
}
void MainWindow::EEPROM(){
    QString msg = "E";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
    showDownloader();
}
void MainWindow::abort(){
    QString msg = "S";
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
}
void MainWindow::macro(){
    QString msg = macroText->text();
    serial->write(msg.toLocal8Bit());
    qDebug()<<msg<<"sent";
}

void MainWindow::gotoDir(){
    //get the default save directory
    QSettings settings("Bobcat Engineering","CCS");
    settings.beginGroup("Saving");
    QString tempPath = settings.value("DefaultDir").toString();
    settings.endGroup();
    if(tempPath.isEmpty()){
        tempPath = QDir::toNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    }
    QDesktopServices::openUrl(QUrl::fromLocalFile(tempPath));
}

void MainWindow::gotoAppLocation(){
    QString tempPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
    QDesktopServices::openUrl(QUrl::fromLocalFile(tempPath));
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
    if (debugOn){
        rigSelect->setCurrentRow(0);
        ratSelect->setCurrentRow(0);
        cerebroSelect->setCurrentRow(0);
        connect_btn->setText("Start Session (Debug/Calibrate Mode)");
        toggleDebug->setText("Exit Debug/Calibrate Mode");
    }
    else{
        rigSelect->clearSelection();
        ratSelect->clearSelection();
        cerebroSelect->clearSelection();
        connect_btn->setText("Start Session");
        toggleDebug->setText("Enter Debug/Calibrate Mode");
    }
}

void MainWindow::fadeChecked(){
    fade_spn->setVisible(fade_checkbox->isChecked());
}

void MainWindow::openDocs(){
//    opens readthedocs.com documentation
    QUrl site = QUrl::fromEncoded( "http://cerebro.readthedocs.io/en/latest/Software/Xavier.html#xavier-user-guide");
    QDesktopServices::openUrl(site);
//    //opens local html file
//    QProcess *process = new QProcess(this);
//    process->start("explorer.exe", QStringList() << QDir::toNativeSeparators(qApp->applicationDirPath()+"/local docs/Software/Xavier.html"));

}

void MainWindow::removeItem()
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
    else if(sender()==rmv3_btn && cerebroVals->count()>0){
        if(cerebroVals->selectedItems().size()!=0){
            cerebroList.removeOne(cerebroVals->currentItem()->text());
            delete cerebroVals->currentItem();
        }
    }
}

void MainWindow::addListItem()
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
    else if(sender()== add3_btn || sender()== newItem3){
        tempList = cerebroVals;
        tempTxtbox = newItem3;
        tempStringList = &cerebroList;
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

void MainWindow::refreshDrops()
{
    if (first){ //starting up...
        //get the Qstringlists from memory
        QSettings settings("Bobcat Engineering","CCS");
        settings.beginGroup("Dropdowns");
        rigList = settings.value("rigList").toStringList();
        ratList = settings.value("ratList").toStringList();
        cerebroList = settings.value("cerebroList").toStringList();
        aliasStringList = settings.value("portList").toStringList();
        settings.endGroup();
        settings.beginGroup("Features");
        pythonEnabled = settings.value("pythonEnabled").toBool();
        settings.endGroup();
        toolMenu->setEnabled(pythonEnabled);
        pythonCheckbox->setChecked(pythonEnabled);
        first = false;
        // populate the listwidgets in the settings dialog with the items in the lists we got from memory
        for (int i = 0;i< rigList.count(); i++){
            rigVals->addItem(rigList.value(i));
        }
        for (int i = 0;i< ratList.count(); i++){
            ratVals->addItem(ratList.value(i));
        }
        for (int i = 0;i< cerebroList.count(); i++){
            cerebroVals->addItem(cerebroList.value(i));
        }
        //populate the combo boxes as well
        rigSelect->addItems(rigList);
        ratSelect->addItems(ratList);
        cerebroSelect->addItems(cerebroList);
    }
    else //exiting settings dialog
    {
        rigList.clear();                //clear the combo box list
        for (int i = 0;i< rigVals->count(); i++){ //fill the combo box list with items (as strings) from the settings dialog listwidget
            rigList << rigVals->item(i)->text();
        }
        rigSelect->clear();              //empty the combo box
        rigSelect->addItems(rigList);    //insert the updated combo box list

        ratList.clear();
        for (int i = 0;i< ratVals->count(); i++){
            ratList << ratVals->item(i)->text();
        }
        ratSelect->clear();
        ratSelect->addItems(ratList);

        cerebroList.clear();
        for (int i = 0;i< cerebroVals->count(); i++){
            cerebroList << cerebroVals->item(i)->text();
        }
        cerebroSelect->clear();
        cerebroSelect->addItems(cerebroList);

        //Update the Qsettings with the new values
        QSettings settings("Bobcat Engineering","CCS");
        settings.beginGroup("Dropdowns");
        settings.setValue("rigList",rigList);
        settings.setValue("ratList",ratList);
        settings.setValue("cerebroList",cerebroList);
        settings.setValue("portList",aliasStringList);
        settings.endGroup();
        settings.beginGroup("Features");
        settings.setValue("pythonEnabled",pythonCheckbox->isChecked());
        settings.endGroup();
        toolMenu->setEnabled(pythonCheckbox->isChecked());
        debugOn = false;
        if (isConnected2){
            connectDownloadPort(); //disconnect download port after settings change
        }
    }
    fillPortsInfo();
    fillPortsInfo2();
    int itemHeight = 25;
    int itemWidth = 75;
    rigSelect->setMaximumSize(itemWidth,itemHeight*rigSelect->count());
    ratSelect->setMaximumSize(itemWidth,itemHeight*ratSelect->count());
    cerebroSelect->setMaximumSize(itemWidth,itemHeight*cerebroSelect->count());
}

void MainWindow::openSettings()
{
    if(gotoSettings->isEnabled()){
        QSettings settings("Bobcat Engineering","CCS");
        settings.beginGroup("Dropdowns");
        aliasStringList = settings.value("portList").toStringList();
        settings.endGroup();
        portDropdown->clear();
        // populate the listwidgets in the settings dialog with the items in the lists we got from memory
        aliasWidget->clear();
        for (int i = 0;i< aliasStringList.count(); i++){
            aliasWidget->addItem(aliasStringList.value(i));
        }
        //populate the combobox
        static const QString blankString = QObject::tr("N/A");
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            QStringList list;
            if(info.description()==usbDescription){
                list << info.portName();
                portDropdown->addItem(list.first(), list);
            }
        }
        settingsDialog->exec();
    }
}

void MainWindow::addAlias()
{
    bool continueAdd = true;
    QString tempCOM;
    QString labelText;
    bool fromChangeLabel;
    if (sender()==changeLabel_btn){
        tempCOM = editLabelDialog->windowTitle();
        qDebug()<<tempCOM;
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

void MainWindow::removeAlias()
{
    if(aliasWidget->selectedItems().size()!=0){
        aliasStringList.removeOne(aliasWidget->currentItem()->text());
        delete aliasWidget->currentItem();
    }
}


void MainWindow::showDownloader()
{
    if (!(connect2_btn->isChecked())){ //prevents download port dropdown text from being cleared if already connected
        fillPortsInfo2();
    }
}

void MainWindow::getGraphs()
{
    //get the default save directory
    QSettings settings("Bobcat Engineering","CCS");
    settings.beginGroup("Saving");
    QString tempPath = settings.value("DefaultDir").toString();
    settings.endGroup();
    QString baseData = QFileDialog::getOpenFileName(this,tr("Select Base Station Log File"),tempPath,tr("(*station.csv)"));
    if (baseData!=""){
        //the cerebro data should be in the same folder, so we have the getOpenFileName dialog start in the the same folder
        QString betterPath = baseData.mid(tempPath.length(),baseData.indexOf("/",tempPath.length()+1)-tempPath.length())+"/";
        QString cerData = QFileDialog::getOpenFileName(this,tr("Select Base Station Log File"),betterPath,tr("(*cerebroLog.csv)"));
        if (cerData!=""){
            //Run python script for creating graph
            QProcess *process = new QProcess(this);
            QStringList pythonArgs;
            pythonArgs<<qApp->applicationDirPath()+"/python scripts/graph.py"<<"\""+baseData+"\""<<"\""+cerData+"\""; //pass the two log file locations into the python script
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
    if (!isConnected){
        debugOn = !debugOn;
        debugCheckbox->setChecked(debugOn);
        showDebug();
    }
}

void MainWindow::shrink(){
    adjustSize();
}

void MainWindow::editLabel(){
    //isolate the COMXX part of the port name
    QString COMbeingEdited = aliasWidget->currentItem()->text();
    editText->setText(COMbeingEdited.left(COMbeingEdited.indexOf(" (")));
    COMbeingEdited.remove(0,COMbeingEdited.indexOf("("+usbTag)+1);
    COMbeingEdited.remove(QChar (')'));
    editText->setFocus();
    editLabelDialog->setWindowTitle("Edit "+ COMbeingEdited + " label");
    editLabelDialog->exec();
}

void MainWindow::sendCalVector(){
    qDebug()<<"Send cal vector pressed";
    QString msg = "X";
    serial->write(msg.toLocal8Bit());
    QTimer::singleShot(500, this, SLOT(sendCalGroups()));

}

void MainWindow::sendCalGroups(){
    QString calibrationString = codeTextBox->toPlainText();
    QStringList calibrationGroups = calibrationString.split("\n",QString::SkipEmptyParts);
    qDebug()<<calibrationGroups.size();
    if(calibrationGroups.size()>0){
        serial->write(calibrationGroups.takeFirst().toLocal8Bit());
        codeTextBox->clear();
        for (int i = 0; i<calibrationGroups.size(); i++){
            codeTextBox->insertPlainText(calibrationGroups[i]+"\n");
        }
        QTimer::singleShot(475, this, SLOT(sendCalGroups()));
    }
}

/*``````````````````````Calibration functions`````````````````````*/
void MainWindow::slideValueUpdate(int newVal){
    wantedLevel->setValue(newVal);
    slideLabel->setText("Target Power: "+QString::number(wantedLevel->value())+ " mW");
}


void MainWindow::chooseFile(){
//    fileLabel->setText("");
    codeTextBox->clear();
    QString pathFromDialog = QFileDialog::getOpenFileName(this,tr("Select Power Meter Data"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),tr("(*.txt)"));
    qDebug()<<pathFromDialog;
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
    pythonArgs<<qApp->applicationDirPath()+"/python scripts/getCalibrationVec.py"<<"\""+calibrateDataPath+"\""<<QString::number(wantedLevel->value())<<QString::number(showGraph->isChecked()); //pass the calibration data into python script
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
}
