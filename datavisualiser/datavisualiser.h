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

#ifndef DATAVISUALISER_H
#define DATAVISUALISER_H

#include <QWidget>

//#include <QTimer> // temp
#include <QPushButton>
#include <QLayout>
#include <QSpinBox>
#include <QCheckBox>
#include "diagnostictools.h"

class QTimer;

namespace Ui {
class DataVisualiser;
}

namespace Diagnostictools {
namespace Internal {

class DataQueryEngine;

class DataVisualiser : public QWidget
{
    Q_OBJECT

public:
    explicit DataVisualiser(DataQueryEngine *dataQueryEngine, QWidget *parent = 0);
    ~DataVisualiser();
    QVBoxLayout   *layout;
    QVBoxLayout   *layout2;
    QVBoxLayout   *layout3;
    QLabel        *PaekMemory;
    QLabel        *PaekMemory2;
    QLabel        *peakMemory_2_1;
    QLabel        *peakDedicatedMemoryLabel;
    QSpinBox      *spinbox;
    QCheckBox     *CheckBox;
    QPushButton   *PushButton;
    Ui::DataVisualiser *ui;
    DataQueryEngine *m_dataQueryEngine;

signals:
    void update();

public slots:
    void startSession();
    void stopSession();

    void updateRamData(quint32 workingSetSize, quint32 pagefileUsage,
                       quint32 peakPagefileUsage, quint32 peakmamory);
    void updateCpuData(int percent);

private slots:
    //void testSlot();
    void onSessionTimerTimeout();
    void on_SpinBox_clicked();
    void on_stateChanged_clicked();
    void on_stateChanged_clickedRU();
    void on_pushButton_clicked();

private: // methods
    void setConnection();

private: // feilds

    QTimer *sessionTimer;
    qint64 secondsCount = 0;
};

} // namespace Internal
} // namespace DiagnosticTools

#endif // DATAVISUALISER_H
