#include "codemanager.h"

#include <QDebug>

CodeManager::CodeManager(GameManager *gameEngine)
{
    engine = new QScriptEngine();
    debugger = new QScriptEngineDebugger();
    commandImpl = new CommandImpl();

    QScriptValue command = engine->newQObject(commandImpl);

    engine->globalObject().setProperty("command", command);
    QScriptValue gameEngineScript = engine->newQObject(gameEngine);
    engine->globalObject().setProperty("player", gameEngineScript);

}

bool CodeManager::run(QString test, QString* errorMessage)
{
    qDebug() << "Run : " << test;


    QScriptValue result = engine->evaluate(test);

    //throwing exception
    if (engine->hasUncaughtException()) {
        int line = engine->uncaughtExceptionLineNumber();
        errorMessage->append("Uncaught exception at line " + QString::number(line) + "->" + result.toString());
        return false;
    }

    return true;
}
