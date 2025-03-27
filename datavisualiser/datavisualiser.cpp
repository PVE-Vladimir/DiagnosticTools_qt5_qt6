/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/

#include "datavisualiser.h"
#include "../dataqueryengine/dataqueryengine.h"
#include "diagnostictools.h"

using namespace Diagnostictools::Internal;
#include "ui_datavisualiser.h"

#include <QTimer>
#include <QDebug>

/*
//Отключение всех qCritical, qFatal; Disabling all qCritical, qFatal
#include <QMessageLogger>

#define QT_NO_QDEBUG_MACRO while (false) QMessageLogger().noDebug
#define QT_NO_QCRITICAL_MACRO while (false) QMessageLogger().critical
#if defined(QT_NO_CRITICAL_OUTPUT)
#  undef qCritical
#  define qCritical QT_NO_QCRITICAL_MACRO
#endif

#define QT_NO_QCRITICAL_MACROO while (false) QMessageLogger().fatal
#if defined(QT_NO_FATAL_OUTPUT)
#  undef qFatal
#  define qFatal QT_NO_QCRITICAL_MACROO
#endif
*/

#define SESSION_TIMER_TICK 1000

namespace Diagnostictools {
namespace Internal {
//Отключение отладочной информации; Disabling debug information
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    Q_UNUSED(type);
    Q_UNUSED(context);
    Q_UNUSED(msg);
}

DataVisualiser::DataVisualiser(DataQueryEngine *dataQueryEngine, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataVisualiser),
    sessionTimer(new QTimer(this))
{
    qDebug() << "in constructor DataVisualiser";

    //Отключение отладочной информации; Disabling debug information
    qInstallMessageHandler(customMessageHandler);

    ui->setupUi(this);
    m_dataQueryEngine = dataQueryEngine;
    layout = new QVBoxLayout(ui->widget_2);
    spinbox = new QSpinBox(this);
    CheckBox = new QCheckBox("AutoMax", this);
    PushButton = new QPushButton(this);
    layout2 = new QVBoxLayout(ui->widget);
    layout3 = new QVBoxLayout(ui->widget_3);
    layout->setContentsMargins(0, 0, 0, 0);
    layout2->setContentsMargins(0, 0, 0, 0);
    layout3->setContentsMargins(0, 0, 0, 0);
    PaekMemory = new QLabel(this);
    PaekMemory2 = new QLabel(this);
    peakMemory_2_1 = new QLabel(this);
    peakDedicatedMemoryLabel = new QLabel(this);
    PaekMemory->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    peakMemory_2_1->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    PaekMemory2->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    peakDedicatedMemoryLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    QSizePolicy policy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    policy.setHorizontalStretch(1);
    policy.setVerticalStretch(1);
    PaekMemory->setSizePolicy(policy);
    peakMemory_2_1->setSizePolicy(policy);

    PaekMemory2->setSizePolicy(policy);
    peakDedicatedMemoryLabel->setSizePolicy(policy);
    qDebug() << ProjectExplorer::ProjectExplorerPlugin::instance()->allRunControls().count() << "ProjectExplorer::RunControl 000";
    spinbox->setRange(1,ProjectExplorer::ProjectExplorerPlugin::instance()->allRunControls().count());
    spinbox->setValue(Diagnostictools::Internal::DiagnostictoolsPlugin::valueold);
    if (Diagnostictools::Internal::DiagnostictoolsPlugin::valueCheckBox){CheckBox->setCheckState(Qt::Checked);}
    if (Diagnostictools::Internal::DiagnostictoolsPlugin::valueCheckBoxRU){
        ui->ru->setCheckState(Qt::Checked);
        ui->label_3->setText("Сеанс диагностики");
        ui->label->setText("Память RSS");
        ui->label->setToolTip ("RSS: Размер резидентной памяти в килобайтах (Resident Set Size)<br>"
                              " — это количество физической памяти, используемой процессом в данный момент. Это память, которая фактически загружена в оперативную память.");
        PaekMemory->setText("Пик");
        PaekMemory2->setText("Пик");
        PushButton->setText("Очистка");
        ui->label_2->setText("Выделенная память VSZ");
        ui->label_2->setToolTip ("VSZ: Размер виртуальной памяти процесса в килобайтах (Virtual Size)<br>"
                                " — это общее количество виртуальной памяти, используемой процессом, включая все его модули и библиотеки.");
        ui->label_4->setText("Процент загрузки CPU");
        Diagnostictools::Internal::DiagnostictoolsPlugin::Kilobyte=" Кб";
        Diagnostictools::Internal::DiagnostictoolsPlugin::Secynd=" с";
    } else {
        ui->label_3->setText("Diagnostic Session");
        ui->label->setText("RSS Memory");
        ui->label->setToolTip ("RSS: (Resident Set Size) in kilobytes is the amount of physical memory currently<br>"
                              "used by the process. This is the memory that is actually loaded into RAM.");
        PaekMemory->setText("Peak");
        PaekMemory2->setText("Peak");
        PushButton->setText("Cleanup");
        ui->label_2->setText("VSZ Memory Dedicated");
        ui->label_2->setToolTip ("VSZ: The process's virtual memory size in kilobytes (Virtual Size) is the<br>"
                                "total amount of virtual memory used by the process, including all its modules and libraries.");
        ui->label_4->setText("CPU Load Percentage");
        Diagnostictools::Internal::DiagnostictoolsPlugin::Kilobyte=" Kb";
        Diagnostictools::Internal::DiagnostictoolsPlugin::Secynd=" s";
    }
    QSpacerItem* spac = new QSpacerItem(0, 1000, QSizePolicy::Minimum, QSizePolicy::Expanding);
    setConnection();
    //ui->widget_2->setLayout(layout);
    layout->addWidget(spinbox);
    layout->addWidget(CheckBox);
    layout->addWidget(PushButton);
    layout->addSpacerItem(spac);

    layout2->addWidget(PaekMemory);
    layout2->addWidget(peakMemory_2_1);

    layout3->addWidget(PaekMemory2);
    layout3->addWidget(peakDedicatedMemoryLabel);
    //connect(spinbox, &QSpinBox::valueChanged, this, &DataVisualiser::on_SpinBox_clicked);
    connect(spinbox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DataVisualiser::on_SpinBox_clicked);
    connect(PushButton, &QPushButton::clicked, this, &DataVisualiser::on_pushButton_clicked);
    connect(CheckBox, &QCheckBox::stateChanged, this, &DataVisualiser::on_stateChanged_clicked);
    connect(ui->ru, &QCheckBox::stateChanged, this, &DataVisualiser::on_stateChanged_clickedRU);
    Diagnostictools::Internal::DiagnostictoolsPlugin::flagUi=true; 
}
//================================================================================
DataVisualiser::~DataVisualiser()
{
    qDebug() << "in destructor DataVisualiser";
    Diagnostictools::Internal::DiagnostictoolsPlugin::flagUi=false;
    Diagnostictools::Internal::DiagnostictoolsPlugin::flagUpdate=true;
    stopSession();
    delete ui;
}
//================================================================================
void DataVisualiser::startSession(){
    qDebug() << "DataVisualiser::startSession";
    sessionTimer->start(SESSION_TIMER_TICK);
    secondsCount = 0;
}
//================================================================================
void DataVisualiser::stopSession(){
    sessionTimer->stop();
    ui->timeOfSessionLabel->setNum(0);
    ui->memoryPlot->reset();
    ui->dedicatedMemoryPlot->reset();
    ui->cpuLoadPercentPlot->reset();
}
//================================================================================
void DataVisualiser::updateRamData(quint32 workingSetSize, quint32 pagefileUsage,
                                   quint32 peakPagefileUsage, quint32 peakmamory){
    // memory
    ui->memoryPlot->addValue(workingSetSize);
    ui->currentMemoryLabel->setText(QString::number((int)workingSetSize) + Diagnostictools::Internal::DiagnostictoolsPlugin::Kilobyte);
    // dedicated memory
    ui->dedicatedMemoryPlot->addValue(pagefileUsage);
    ui->currentDedicatedMemoryLabel->setText(QString::number((int)pagefileUsage) + Diagnostictools::Internal::DiagnostictoolsPlugin::Kilobyte);
    peakDedicatedMemoryLabel->setText(QString::number((int)peakPagefileUsage) + Diagnostictools::Internal::DiagnostictoolsPlugin::Kilobyte);
    peakMemory_2_1->setText(QString::number((int)peakmamory) + Diagnostictools::Internal::DiagnostictoolsPlugin::Kilobyte);
}
//================================================================================
void DataVisualiser::updateCpuData(int percent){
    //qDebug() << "DataVisualiser::updateCpuData percent" << percent;
    ui->cpuLoadPercentPlot->addValue(percent);
    ui->cpuLoadPercenLabel->setText(QString::number(percent) + " %");
}
//================================================================================
void DataVisualiser::onSessionTimerTimeout(){ // TODO: maybe lambda
    secondsCount ++;
    ui->timeOfSessionLabel->setText(QString::number(secondsCount) + Diagnostictools::Internal::DiagnostictoolsPlugin::Secynd);
}
//================================================================================
void DataVisualiser::setConnection(){
    connect(sessionTimer, &QTimer::timeout, this, &DataVisualiser::onSessionTimerTimeout);

    connect(m_dataQueryEngine, &DataQueryEngine::ramDataUpdated,
            this, &DataVisualiser::updateRamData);
    connect(m_dataQueryEngine, &DataQueryEngine::cpuDataUpdated,
            this, &DataVisualiser::updateCpuData);
    connect(m_dataQueryEngine, &DataQueryEngine::started,
            this, &DataVisualiser::startSession);
    connect(m_dataQueryEngine, &DataQueryEngine::stopped,
            this, &DataVisualiser::stopSession);
}
//================================================================================
void DataVisualiser::on_pushButton_clicked()
{
    Diagnostictools::Internal::DiagnostictoolsPlugin::flag=true;
    Diagnostictools::Internal::DiagnostictoolsPlugin::PeakVsize = 0;
    Diagnostictools::Internal::DiagnostictoolsPlugin::PeakRss = 0;
    emit update();
}
//================================================================================
void DataVisualiser::on_SpinBox_clicked()
{
    emit update();
}
//================================================================================
void DataVisualiser::on_stateChanged_clicked()
{
    if(CheckBox->checkState() ==  Qt::Checked)
    {
        Diagnostictools::Internal::DiagnostictoolsPlugin::valueCheckBox=true;
        spinbox->setValue(Diagnostictools::Internal::DiagnostictoolsPlugin::maxRunControls);
    } else {
        Diagnostictools::Internal::DiagnostictoolsPlugin::valueCheckBox=false;
    }
    qDebug() << "#######################" << CheckBox->checkState();;
}
//================================================================================
void DataVisualiser::on_stateChanged_clickedRU()
{
    if(ui->ru->checkState() ==  Qt::Checked)
    {
        Diagnostictools::Internal::DiagnostictoolsPlugin::valueCheckBoxRU=true;
        ui->label_3->setText("Сеанс диагностики");
        ui->label->setText("Память RSS");
        ui->label->setToolTip ("RSS: Размер резидентной памяти в килобайтах (Resident Set Size)<br>"
                              " — это количество физической памяти, используемой процессом в данный момент. Это память, которая фактически загружена в оперативную память.");
        PaekMemory->setText("Пик");
        PaekMemory2->setText("Пик");
        PushButton->setText("Очистка");
        ui->label_2->setText("Выделенная память VSZ");
        ui->label_2->setToolTip ("VSZ: Размер виртуальной памяти процесса в килобайтах (Virtual Size)<br>"
                                " — это общее количество виртуальной памяти, используемой процессом, включая все его модули и библиотеки.");
        ui->label_4->setText("Процент загрузки CPU");
        Diagnostictools::Internal::DiagnostictoolsPlugin::Kilobyte=" Кб";
        Diagnostictools::Internal::DiagnostictoolsPlugin::Secynd=" с";
    } else {
        Diagnostictools::Internal::DiagnostictoolsPlugin::valueCheckBoxRU=false;
        ui->label_3->setText("Diagnostic Session");
        ui->label->setText("RSS Memory");
        ui->label->setToolTip ("RSS: (Resident Set Size) in kilobytes is the amount of physical memory currently<br>"
                               "used by the process. This is the memory that is actually loaded into RAM.");
        PaekMemory->setText("Peak");
        PaekMemory2->setText("Peak");
        PushButton->setText("Cleanup");
        ui->label_2->setText("VSZ Memory Dedicated");
        ui->label_2->setToolTip ("VSZ: The process's virtual memory size in kilobytes (Virtual Size) is the<br>"
                                "total amount of virtual memory used by the process, including all its modules and libraries.");
        ui->label_4->setText("CPU Load Percentage");
        Diagnostictools::Internal::DiagnostictoolsPlugin::Kilobyte=" Kb";
        Diagnostictools::Internal::DiagnostictoolsPlugin::Secynd=" s";
    }
    qDebug() << "ui->ru ##############" << ui->ru->checkState();
}
} // namespace Internal
} // namespace DiagnosticTools
