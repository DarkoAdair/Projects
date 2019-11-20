#include "codemanager.h"

#include <QtCore>
#include <QDebug>
#include <QThread>
#include <QtConcurrent>

CodeManager::CodeManager()
{
    engine = new QScriptEngine();
    engine->setProcessEventsInterval(50);


    debugger = new QScriptEngineDebugger();
    commandImpl = new CommandImpl();

    QScriptValue command = engine->newQObject(commandImpl);
    engine->globalObject().setProperty("command", command);
}

void CodeManager::run(QString test)
{
    qDebug() << "Run : " << test;

    // check script syntax
    QScriptSyntaxCheckResult syntaxResult = engine->checkSyntax(test);
    if (QScriptSyntaxCheckResult::Valid != syntaxResult.state()) {
       qDebug() << "Syntax Error : " << syntaxResult.errorMessage();
       return;
    }

    DebuggerThread *thread = new DebuggerThread(engine, test);
    thread->run();
}

void CodeManager::startThread(QString test)
{

}
