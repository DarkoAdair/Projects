#include "codemanager.h"

#include <QtCore>
#include <QDebug>
#include <QThread>
#include <QtConcurrent>

CodeManager::CodeManager()
{
    engine = new QScriptEngine();
    engine->setProcessEventsInterval(50);

    commandImpl = new CommandImpl();

    QScriptValue command = engine->newQObject(commandImpl);
    engine->globalObject().setProperty("command", command);
}

void CodeManager::run(QString script)
{
    qDebug() << "[CodeManager] Run :" << script;

    // Check script syntax
    QScriptSyntaxCheckResult syntaxResult = engine->checkSyntax(script);
    if (QScriptSyntaxCheckResult::Valid != syntaxResult.state()) {
       qDebug() << "[CodeManager] Syntax Error : " << syntaxResult.errorMessage();
       return;
    }

    this->script = script;

    // Start Debug mode.
    backgroundThread = new QThread(this);
    debugTimer = new QTimer(this);
    connect(debugTimer, SIGNAL(timeout()), this, SLOT(debugProcess()));

    debugTimer->moveToThread(backgroundThread);
    debugTimer->setSingleShot(true);
    debugTimer->start();
}

void CodeManager::debugProcess()
{
    debugger = new ScriptDebugger(engine);
    debugger->breakAtNextStatement();

    QScriptValue result = engine->evaluate(script);

    if (engine->hasUncaughtException()) {
        int line = engine->uncaughtExceptionLineNumber();
        QString errorMessage = "uncaught exception at line" + QString::number(line)  + ":" + result.toString();

        qDebug() << "[CodeManager] exception : " << errorMessage;
        emit onException(errorMessage);
    }

    emit onFinish();
}

void CodeManager::moveNextLine()
{
    this->debugger->moveNext();
}
