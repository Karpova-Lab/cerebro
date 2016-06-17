#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Calibration Utilty");
//    fileMenu = menuBar()->addMenu("File");
//        gotoSettings = new QAction(this);
//        gotoSettings->setText("Settings...");
//    fileMenu->addAction(gotoSettings);
//    goMenu = menuBar()->addMenu("Go To");
//        openDir = new QAction(this);
//        openDir->setText("Default Save Directory");
//    goMenu->addAction(openDir);

    chooseBox = new QGroupBox();
        chooseLayout = new QGridLayout();

            wantedLevel = new QSlider(Qt::Horizontal);
//            wantedLevel->setMinimum(0);
            wantedLevel->setRange(0,15);
            wantedLevel->setValue(4);
            wantedLevel->setTickPosition(QSlider::TicksBelow);
        chooseLayout->addWidget(wantedLevel,1,0);
            slideLabel = new QLabel("Target Power: "+QString::number(wantedLevel->value())+ " mW");
        chooseLayout->addWidget(slideLabel,0,0,Qt::AlignCenter);
            showGraph = new QCheckBox("Show Graphs");
        chooseLayout->addWidget(showGraph,0,1,2,1);
            pickFile_btn = new QPushButton("Select Power Meter\ndata file");
            pickFile_btn->setCheckable(true);
        chooseLayout->addWidget(pickFile_btn,0,2,2,1);
            fileLabel = new QLabel();
        chooseLayout->addWidget(fileLabel,2,0,1,3,Qt::AlignCenter);
            codeTextBox = new QPlainTextEdit();
            codeTextBox->setReadOnly(true);
            codeTextBox->setMinimumHeight(130);
            codeTextBox->setMinimumWidth(450);
        chooseLayout->addWidget(codeTextBox,3,0,1,3);
    chooseBox->setLayout(chooseLayout);



    mainLayout = new QGridLayout();
    mainLayout->addWidget(chooseBox,0,0);
    this->resize(0,0);
    this->setFixedSize(this->size());
    QWidget *window = new QWidget();
    window->setLayout(mainLayout);
    setCentralWidget(window);

    connect(pickFile_btn,SIGNAL(clicked()),this,SLOT(chooseFile()));
    connect(wantedLevel,SIGNAL(sliderMoved(int)),this,SLOT(slideValueUpdate(int)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::slideValueUpdate(int newVal){
    wantedLevel->setValue(newVal);
    slideLabel->setText("Target Power: "+QString::number(wantedLevel->value())+ " mW");
}


void MainWindow::chooseFile(){
    fileLabel->setText("");
    codeTextBox->clear();
    QString calibrateDataPath = QFileDialog::getOpenFileName(this,tr("Set Default Directory"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),tr("(*.txt)"));
    if(!calibrateDataPath.isEmpty()){
        // Run python script to summarize data from base station and cerebro logs
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
        alert.setWindowTitle("Results are in!");
        alert.setText("The calibration vector has\nbeen copied to your clipboard");
        alert.exec();
        fileLabel->setText("<h3>"+calibrateDataPath+"</h3>");
    //    if (errorString.isEmpty()){
    //        alert.setWindowTitle("Session Summary");
    //        alert.setText(resultString);
    //        alert.exec();
    //    }
    //    else{
//            alert.setWindowTitle("Error");
//            alert.setText(errorString);
//            alert.exec();
//            QFile file3(tempPath + "/" + ratNumber + "/" + saveTime.toString("yyyyMMMdd_hhmm")+"_errorText.txt");
//            file3.open(QIODevice::WriteOnly | QIODevice::Text);
//            QTextStream out3(&file3);
//            out3 << errorString;
    //    }
    }
}
