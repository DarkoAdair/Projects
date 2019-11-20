#ifndef CODEMANAGER_H
#define CODEMANAGER_H

#include <QAction>
#include <QScriptEngine>
#include <QScriptEngineDebugger>

#include "commandimpl.h"
#include "gamemanager.h"

class CodeManager
{
public:
    CodeManager(GameManager *gameEngine);

    bool run(QString test, QString* errorMessage);

    QAction *actionNextLine;
    QScriptEngine *engine;


    QScriptValue result;
    int line = 0;
    GameManager *gameEngine;
    QScriptValue gameEngineScript;

private:
    QScriptEngineDebugger *debugger;
    CommandImpl *commandImpl;

};

#endif // CODEMANAGER_H
