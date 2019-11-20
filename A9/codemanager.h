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

    void run(QString test);

    QAction *actionNextLine;

private:
    QScriptEngine *engine;
    QScriptEngineDebugger *debugger;
    CommandImpl *commandImpl;
    GameManager *gameEngine;
};

#endif // CODEMANAGER_H
