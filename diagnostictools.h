#ifndef DIAGNOSTICTOOLSPLUGIN_H
#define DIAGNOSTICTOOLSPLUGIN_H

#include "diagnostictools_global.h"

#include <extensionsystem/iplugin.h>
#include <projectexplorer/runconfiguration.h>
#include <projectexplorer/runcontrol.h>
#include <projectexplorer/projectexplorer.h>

#include <coreplugin/icore.h>
#include <projectexplorer/runcontrol.h>
#include <projectexplorer/projectexplorer.h>
#include <extensionsystem/iplugin.h>

#include <QPointer>

namespace Diagnostictools {
namespace Internal {

class DataQueryEngine;
class DiagnosticToolsOutputPane;
class DiagnosticToolsFactory;

class DiagnostictoolsPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Diagnostictools.json")

public:
    DiagnostictoolsPlugin();
    ~DiagnostictoolsPlugin() override;
    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;
    static bool flag;
    static bool flagUi;
    static bool flagCheckMax;
    static bool flagSpinBox;
    static bool flagUpdate;
    static bool valueCheckBox;
    static bool valueCheckBoxRU;
    static int  valueold;
    static int  maxRunControls;
    static int  flagRunControls;
    static double PeakVsize;
    static double PeakRss;
    static QString Kilobyte;
    static QString Secynd;

public slots:
    void updateRunControls();

private slots:
    void onRunControlStarted(ProjectExplorer::RunControl *rc);
    void onRunControlFinished(ProjectExplorer::RunControl *rc);
    void onApplicationHandleChanged();

signals:
    void valueChanged();

private:
    void setConnections();
    void triggerAction();
private: // feilds
    DataQueryEngine *m_dataQueryEngine;

    //DiagnosticToolsOutputPane *m_outputPane;
    DiagnosticToolsFactory *m_diagnosticToolsFactory;

    ProjectExplorer::RunControl* m_runControlPtr = nullptr;
    //QPointer<DiagnosticToolsFactory> m_diagnostictoolsFactory;
};

} // namespace Internal
} // namespace Diagnostictools

#endif // DIAGNOSTICTOOLSPLUGIN_H
