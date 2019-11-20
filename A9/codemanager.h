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

    QAction *actionNextLine;
    QScriptEngine *engine;


    QScriptValue result="";
    int line = 0;

private:
    QScriptEngineDebugger *debugger;
    CommandImpl *commandImpl;


};

#endif // CODEMANAGER_H
