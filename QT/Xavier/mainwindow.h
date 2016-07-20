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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <qtimer.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QString                 location;
    QString                 saveName1;
    QString                 saveName2;
    bool                    isConnected;
    bool                    isConnected2;
    bool                    repeatOn;
    bool                    inTestloop;
    bool                    errorThrown;
    bool                    pythonEnabled;
    int                     testCount;
    bool                    first;
    bool                    debugOn;

    QAction*                gotoSettings,*openDir,*gotoApplocation,*gotoDocs,*about,*graphResults;
    QMenu*                  fileMenu,*viewMenu,*toolMenu,*goMenu,*helpMenu;
    QShortcut*              debugShortcut,*settingsShortcut;
    QMessageBox*            aboutDialog;
    QTimer*                 timer;

    QPushButton*            refresh_btn,*connect_btn,*startSession_btn;
    QPushButton*            refresh2_btn,*connect2_btn;
    QPushButton*            trigger_btn,*stop_btn,*abort_btn,*lamp_btn,*filter_btn,*macro_btn;;
    QPushButton*            eeprom_btn;
    QPushButton*            sendSettings_btn;
    QPushButton*            clearBase_btn;
    QPushButton*            clearDownload_btn;
    QPushButton*            changeSettings_btn;
    QPushButton*            saveMonitor_btn;
    QCheckBox*              trainOn_checkbox;
    QCheckBox*              fade_checkbox;
    QCheckBox*              trigger_checkbox;
    QComboBox*              serialPortList;
    QComboBox*              serialPortList2;
    QListWidget*            rigSelect,*ratSelect,*cerebroSelect;
    QStringList             rigList,ratList,cerebroList;
    QPlainTextEdit*         baseMonitor;
    QPlainTextEdit*         downloadMonitor;
    QLineEdit*              macroText;
    QSpinBox*               onTime_spn,*offTime_spn,*trainDuration_spn,*duration_spn,*power_spn,*fade_spn,*trials_spn,*baseFilter_spn;
    QRadioButton*           singleShot,*pulseTrain;
    QLabel*                 rig_lbl,*rat_lbl,*cerebro_lbl;
    QLabel*                 onTime_lbl,*offTime_lbl,*trainDescription_lbl,*trainDuration_lbl,*power_lbl,*fade_label,*filterLabel;
    QLabel*                 serial_title;
    QLabel*                 download_title;
    QLabel*                 last_settings;
    QLabel*                 picLabel,*singlePicLabel,*trainPicLabel;
    QLabel*                 connectBS_label;
    QLabel*                 connectLU_label,*instructions;

    QWidget*                window,*downloadWindow;
    QPushButton*            insertValues;
    QProgressBar*           testProgress;

    QGridLayout*            mainLayout;
    QGridLayout*            equipmentLayout;
    QGridLayout*            startscreenLayout;
    QGridLayout*            connectionLayout2;
    QGridLayout*            adjustmentLayout;
    QGridLayout*            triggerLayout;
    QGridLayout*            serialMonitorLayout;
    QGridLayout*            logLayout;
    QGridLayout*            triggTestLayout;
    QGridLayout*            baseSettingLayout;

    QGroupBox*              startscreenBox;
    QGroupBox*              equipmentBox;
    QGroupBox*              connectBox2;
    QGroupBox*              adjustBox;
    QGroupBox*              bugBox;
    QGroupBox*              baseBox;
    QGroupBox*              downloaderBox;
    QGroupBox*              baseSettingsBox;

    QSerialPort*            serial;
    QSerialPort*            serial2;

    QDialog*                settingsDialog;
    QGridLayout*            settingsLayout;

    QGroupBox*              directoryBox;
    QGridLayout*            directoryLayout;
    QLabel*                 directoryLabel;
    QPushButton*            changeDir_btn;

    QGroupBox*              dropDownsBox;
    QGridLayout*            dropDownsLayout;
    QLabel*                 rigLabel, *ratLabel, *cerebroLabel;
    QListWidget*            rigVals,*ratVals, *cerebroVals;
    QPushButton*            add1_btn,*add2_btn,*add3_btn;
    QLineEdit*              newItem1,*newItem2,*newItem3;
    QPushButton*            rmv1_btn,*rmv2_btn,*rmv3_btn;

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

    QGroupBox*              featuresBox;
    QGridLayout*            featuresLayout;
    QCheckBox*              pythonCheckbox;



private slots:
    void fillPortsInfo();
    void fillPortsInfo2();
    void connectPort();
    void connectDownloadPort();
    void sendTime();
    void readSerial();
    void readLog();
    void set();
    void sendTrigger();
    void abort();
    void EEPROM();
    void updateFilter();
    void lamp();
    void macro();
    void saveFile();
    void errorMsg();
    void triggerPushed();
    void triggerChecked();
    void trainChecked();
    void clearMonitor();
    void clearMonitor2();
    void setPath();
    void gotoDir();
    void gotoAppLocation();
    void trainDur();
    void showDebug();
    void fadeChecked();
    void openDocs();
    void removeItem();
    void addListItem();
    void refreshDrops();
    void openSettings();
    void addAlias();
    void removeAlias();
    void showDownloader();
    void getGraphs();
    void setDebug();
    void shrink();
    void editLabel();
};


#endif // MAINWINDOW_H
