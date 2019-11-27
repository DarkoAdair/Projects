#include "codemanager.h"

#include <QtCore>
#include <QDebug>
#include <QThread>
#include <QtConcurrent>

CodeManager::CodeManager(GameManager *gameEngine)
{
    //Object Initalize
    commandImpl = new CommandImpl();

    //RunningTimer Intialize
    runningTimer = new QTimer();
    connect(runningTimer, SIGNAL(timeout()), this, SLOT(onRunningProcess()));

    this->gameEngine = gameEngine;

    initalizeForDebugging();
}

CodeManager::~CodeManager()
{
    deinitalizeForDebugging();

    delete runningTimer;
    delete commandImpl;
}

void CodeManager::initalizeForDebugging()
{
    //Engine Setting
    engine = new QScriptEngine();

    QScriptValue gameEngineScript = engine->newQObject(gameEngine);
    engine->globalObject().setProperty("player", gameEngineScript);

    QScriptValue command = engine->newQObject(commandImpl);
    engine->globalObject().setProperty("command", command);

    //Debugger Initalize
    debugger = new ScriptDebugger(engine);
    connect(debugger, SIGNAL(signalLineChange(int)), this, SLOT(onLineNumberChanged(int)));

}

void CodeManager::deinitalizeForDebugging()
{
    disconnect(debugger, SIGNAL(signalLineChange(int)), this, SLOT(onLineNumberChanged(int)));

    if(debugger) delete debugger;
    debugger = nullptr;

    if(engine) delete engine;
    engine = nullptr;
}

void CodeManager::run(QString script, int delay)
{
    deinitalizeForDebugging();
    initalizeForDebugging();

    scriptRun(script);

    runningTimer->setInterval(delay);
    runningTimer->start();
}

void CodeManager::debug(QString script)
{
    deinitalizeForDebugging();
    initalizeForDebugging();

    scriptRun(script);
}

void CodeManager::scriptRun(QString script)
{
    qDebug() << "[CodeManager] Run :" << script;

    // Check script syntax
    QScriptSyntaxCheckResult syntaxResult = engine->checkSyntax(script);
    if (QScriptSyntaxCheckResult::Valid != syntaxResult.state()) {
        int line = syntaxResult.errorLineNumber();
        QString errorMessage = "Syntax error at line " + QString::number(line)  + " : " + syntaxResult.errorMessage();

        qDebug() << "[CodeManager] " << errorMessage;

        emit signalException(errorMessage);
        return;
    }

    this->script = script;

    // Start Debug mode.
    QMetaObject::invokeMethod(this, "onDebugProcess", Qt::QueuedConnection);
}

void CodeManager::onDebugProcess()
{
    debugger->breakAtNextStatement();
    QScriptValue result = engine->evaluate(script);

    if (engine->hasUncaughtException()) {
        int line = engine->uncaughtExceptionLineNumber();
        QString errorMessage = "Uncaught exception at line " + QString::number(line)  + " : " + result.toString();

        qDebug() << "[CodeManager] exception : " << errorMessage;
        emit signalException(errorMessage);
    }

    qDebug() << "[CodeMangaer] Running Finished.";
    emit signalFinish();

    runningTimer->stop();
}

void CodeManager::onLineNumberChanged(int currentLine)
{
    emit signalLineChanged(currentLine);
}

void CodeManager::onRunningProcess()
{
    moveNextLine();
}

void CodeManager::moveNextLine()
{
    this->debugger->moveNext();
}
