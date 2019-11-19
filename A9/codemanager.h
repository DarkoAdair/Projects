#ifndef CODEMANAGER_H
#define CODEMANAGER_H

#include <QAction>
#include <QScriptEngine>
#include <QScriptEngineDebugger>

#include "commandimpl.h"

class CodeManager
{
public:
    CodeManager();

    void run(QString test);

private:
    QScriptEngine *engine;
    QScriptEngineDebugger *debugger;
    CommandImpl *commandImpl;
};

#endif // CODEMANAGER_H
