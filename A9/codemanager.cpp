#include "codemanager.h"

#include <QDebug>

CodeManager::CodeManager()
{
    engine = new QJSEngine();
    commandImpl = new CommandImpl();


    QJSValue command = engine->newQObject(commandImpl);
    engine->globalObject().setProperty("command", command);
}

void CodeManager::run(QString test)
{
    qDebug() << "Run : " << test;

    QJSValue result = engine->evaluate(test);

    if (result.isError())
        qDebug()
                << "Uncaught exception at line"
                << result.property("lineNumber").toInt()
                << ":" << result.toString();
}
