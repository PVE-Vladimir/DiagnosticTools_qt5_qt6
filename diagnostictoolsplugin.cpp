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

#include "diagnostictools.h"
#include "diagnostictoolsconstants.h"
#include "diagnostictoolsfactory.h"
#include "dataqueryengine/dataqueryengine.h"
// TODO: remove unnecessary
#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/ioutputpane.h>
#include <projectexplorer/buildtargetinfo.h>
#include <projectexplorer/target.h>
#include <cppeditor/projectinfo.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildtargetinfo.h>
#include <projectexplorer/runconfiguration.h>
#include <projectexplorer/session.h>
#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorer.h>
//#include <projectexplorer/runcontrol.h>
#include "extensionsystem/pluginmanager.h"
#include <extensionsystem/iplugin.h>
#include "datavisualiser/datavisualiser.h"

//
#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>
//
namespace Diagnostictools {
namespace Internal {

bool DiagnostictoolsPlugin::flag = false;
bool DiagnostictoolsPlugin::flagUi = false;
bool DiagnostictoolsPlugin::flagUpdate = true;
bool DiagnostictoolsPlugin::valueCheckBox = false;
bool DiagnostictoolsPlugin::valueCheckBoxRU = false;
int DiagnostictoolsPlugin::maxRunControls = 1;
int DiagnostictoolsPlugin::valueold = 1;
int DiagnostictoolsPlugin::flagRunControls = 10;
bool DiagnostictoolsPlugin::flagCheckMax = 1;
bool DiagnostictoolsPlugin::flagSpinBox = 1;
double DiagnostictoolsPlugin::PeakVsize = 0;
double DiagnostictoolsPlugin::PeakRss = 0;
QString DiagnostictoolsPlugin::Kilobyte= " Kb";
QString DiagnostictoolsPlugin::Secynd= " s";

DiagnostictoolsPlugin::DiagnostictoolsPlugin()
{
    //qDebug() << "in constructor DiagnosticToolsPlugin";
    // Create your members
    //connect(m_diagnosticToolsFactory->dataVisualiser, &DataVisualiser::update(), this, &DiagnostictoolsPlugin::updateRunControls());
    m_dataQueryEngine = new DataQueryEngine(this);
}
//================================================================================
DiagnostictoolsPlugin::~DiagnostictoolsPlugin()
{
    qDebug() << "DiagnosticToolsPlugin::~DiagnosticToolsPlugin";
    delete m_diagnosticToolsFactory;
    //qDebug() << "DiagnosticToolsPlugin::~DiagnosticToolsPlugin";
    // Unregister objects from the plugin manager's object pool
    // Delete members
}
//================================================================================
bool DiagnostictoolsPlugin::initialize(const QStringList &arguments, QString *errorString){

    qDebug() << "DiagnosticToolsPlugin::initialize";
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    // add DiagnosticToolsFactory
    m_diagnosticToolsFactory = new DiagnosticToolsFactory(m_dataQueryEngine);
    m_diagnosticToolsFactory->setDisplayName("DiagnosticTools");
    m_diagnosticToolsFactory->setPriority(1);
    //addAutoReleasedObject(m_diagnosticToolsFactory);
    //ExtensionSystem::PluginManager::addObject(m_diagnosticToolsFactory);
    //
    setConnections();
    //
    return true;
}
//================================================================================
void DiagnostictoolsPlugin::extensionsInitialized(){

    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}
//================================================================================
ExtensionSystem::IPlugin::ShutdownFlag DiagnostictoolsPlugin::aboutToShutdown(){

    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}
//================================================================================
void DiagnostictoolsPlugin::onRunControlStarted(ProjectExplorer::RunControl *rc){

    m_runControlPtr = rc;

    qDebug() << "DiagnosticToolsPlugin::onRunControlStarted";
    qDebug() << "PID:" << rc->applicationProcessHandle().pid();
    qDebug() <<"";
    connect(m_runControlPtr,
            &ProjectExplorer::RunControl::applicationProcessHandleChanged,
            this, &DiagnostictoolsPlugin::onApplicationHandleChanged);
}
////================================================================================
void DiagnostictoolsPlugin::onRunControlFinished(ProjectExplorer::RunControl *rc){

    Q_UNUSED(rc)
    qDebug() << "DiagnosticToolsPlugin::onRunControlFinished 11111111111111111111";
    qDebug() <<"";

    m_runControlPtr = nullptr;
    m_dataQueryEngine->stopDataQuery();
    //добавил тут
    updateRunControls();
}
//================================================================================
void DiagnostictoolsPlugin::onApplicationHandleChanged(){

    qDebug() << "DiagnosticToolsPlugin::onApplicationHandleChanged";

    // if (m_runControlPtr && m_runControlPtr->applicationProcessHandle().isValid())
    // {
        m_dataQueryEngine->setPid(m_runControlPtr->applicationProcessHandle().pid());
        m_dataQueryEngine->startDataQuery();
    // }
    // else{
    //     qDebug() << "Process handle is invalid";
    // }
    qDebug() << "";
}
//================================================================================
void DiagnostictoolsPlugin::setConnections() {
//     ProjectExplorer::ProjectExplorerPlugin* projectExplorerPlugin = ProjectExplorer::ProjectExplorerPlugin::instance();
//     QList<ProjectExplorer::RunControl *> runControls = projectExplorerPlugin->allRunControls();
//    qDebug() << runControls;
    //auto projectExplorerPlugin =
    // Инициализация текущих RunControl
    qDebug() << "STARTTTTTTTTTTTTTTTT";
    connect(m_diagnosticToolsFactory, SIGNAL(flagconnect()), this, SLOT(updateRunControls()));
    //updateRunControls(); // Чтобы инициализировать во время настройки

    connect(ProjectExplorer::ProjectExplorerPlugin::instance(),
            &ProjectExplorer::ProjectExplorerPlugin::runActionsUpdated,
          this, &DiagnostictoolsPlugin::updateRunControls);
//    connect(ProjectExplorer::ProjectExplorerPlugin::instance(),
//      &ProjectExplorer::ProjectExplorerPlugin::finishedInitialization,
//            this, &DiagnostictoolsPlugin::updateRunControls);
}


void DiagnostictoolsPlugin::updateRunControls() {
    QList<ProjectExplorer::RunControl *> runControls = ProjectExplorer::ProjectExplorerPlugin::instance()->allRunControls();
    qDebug() << runControls;
    maxRunControls = runControls.count();
    qDebug() << maxRunControls <<"текущий RunControl";
    //onRunControlStarted(runControls);
    if (!runControls.isEmpty()&&flagUi)
    {
        if(m_diagnosticToolsFactory->dataVisualiser->CheckBox->checkState() ==  Qt::Checked)
        {
            //valueCheckBox=true;
            m_diagnosticToolsFactory->dataVisualiser->spinbox->setValue(maxRunControls);
        } /*else {
            valueCheckBox=false;
        }*/
        // Передаем первый объект RunControl в метод onRunControlStarted
        int value = m_diagnosticToolsFactory->dataVisualiser->spinbox->value()-1;
        if (value+1>maxRunControls)
        {
            flagRunControls=0;
            value=maxRunControls-1;
            qDebug() << value <<"текущий value000000";
        }
        if (flagRunControls<8)
        {
            flagRunControls++;
            if(flagRunControls==7)
            {
                value=maxRunControls-1;
                m_diagnosticToolsFactory->dataVisualiser->spinbox->setValue(maxRunControls);
            }
        } else
        {
            flagRunControls=10;
        }
        qDebug() << value <<"value";
        valueold=value+1;
        qDebug() << valueold <<"valueold##########";
        onRunControlStarted(runControls[value]);
        onApplicationHandleChanged();
        if(flagUpdate)
        {
            flagUpdate=false;
            connect(m_diagnosticToolsFactory->dataVisualiser, SIGNAL(update()), this, SLOT(updateRunControls()));
        }
        m_diagnosticToolsFactory->dataVisualiser->spinbox->setMaximum(maxRunControls);
    } else
    {
        flagRunControls=10;
        qDebug() << "Нет доступных объектов RunControl.";
    }
    if (!runControls.isEmpty()&&flag&&flagUi)
    {
        int value = m_diagnosticToolsFactory->dataVisualiser->spinbox->value()-1;
        flag=false;
        onRunControlFinished(runControls[value]);
    }
}

} // namespace Internal
} // namespace DiagnosticTools
