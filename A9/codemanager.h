#ifndef CODEMANAGER_H
#define CODEMANAGER_H

#include <QAction>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include <QThread>

#include "commandimpl.h"
#include "scriptdebugger.h"
#include "debuggerthread.h"

class CodeManager
{
public:
    CodeManager();

    void run(QString test);

    QAction *actionNextLine;

    ScriptDebugger *testDebugger;
private:
    QScriptEngine *engine;
    QScriptEngineDebugger *debugger;
    CommandImpl *commandImpl;

    void startThread(QString test);
};



#endif // CODEMANAGER_H
