#include "codemanager.h"

#include <QDebug>

CodeManager::CodeManager()
{
    engine = new QScriptEngine();
    debugger = new QScriptEngineDebugger();
    commandImpl = new CommandImpl();

    QScriptValue command = engine->newQObject(commandImpl);

    engine->globalObject().setProperty("command", command);

}

void CodeManager::run(QString test)
{
    qDebug() << "Run : " << test;

    // Set variable for user

    // Interrpt immidiatly
    result = engine->evaluate(test);


}

