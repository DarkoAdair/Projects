#ifndef CODEMANAGER_H
#define CODEMANAGER_H

#include <QObject>
#include <QAction>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include <QThread>
#include <QTimer>

#include "commandimpl.h"
#include "scriptdebugger.h"
#include "debuggerthread.h"

class CodeManager : public QObject
{
    Q_OBJECT

public:
    CodeManager();

    void run(QString test);
    void moveNextLine();

    QAction *actionNextLine;

    ScriptDebugger *testDebugger;

private:
    QScriptEngine *engine;
    ScriptDebugger *debugger;

    CommandImpl *commandImpl;

    QThread *backgroundThread;
    QTimer *debugTimer;
    QString script;

public slots:
    void debugProcess();

signals:
    void onException(const QString errorMessage);
    void onFinish();
};



#endif // CODEMANAGER_H
