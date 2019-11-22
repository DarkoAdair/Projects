#include "codemanager.h"

#include <QtCore>
#include <QDebug>
#include <QThread>
#include <QtConcurrent>

CodeManager::CodeManager()
{
    //initalize();
}

CodeManager::~CodeManager()
{
    deinitalize();
}

void CodeManager::initalize()
{
    //Object Initalize
    commandImpl = new CommandImpl();
    backgroundThread = new QThread(this);

    //DebugTimer Initalize
    debugTimer = new QTimer(this);
    connect(debugTimer, SIGNAL(timeout()), this, SLOT(onDebugProcess()));

    //RunningTimer Intialize
    runningTimer = new QTimer(this);
    connect(runningTimer, SIGNAL(timeout()), this, SLOT(onRunningProcess()));

    //Engine Setting
    engine = new QScriptEngine();

    QScriptValue command = engine->newQObject(commandImpl);
    engine->globalObject().setProperty("command", command);

    //Debugger Initalize
    debugger = new ScriptDebugger(engine);
    connect(debugger, SIGNAL(signalLineChange(int)), this, SLOT(onLineNumberChanged(int)));
    debugger->breakAtNextStatement();
}

void CodeManager::deinitalize()
{
    if(engine) delete engine;
    engine = nullptr;

    if(debugger) delete debugger;
    debugger = nullptr;

    if(commandImpl) delete commandImpl;
    commandImpl = nullptr;

    if(backgroundThread) delete backgroundThread;
    backgroundThread = nullptr;

    if(debugTimer) delete debugTimer;
    debugTimer = nullptr;

    if(runningTimer) delete runningTimer;
    runningTimer = nullptr;
}

void CodeManager::run(QString script, int delay)
{
    deinitalize();
    initalize();

    scriptRun(script);

    runningTimer->setInterval(delay);
    runningTimer->start();
}

void CodeManager::debug(QString script)
{
    deinitalize();
    initalize();

    scriptRun(script);
}

void CodeManager::scriptRun(QString script)
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
    debugTimer->moveToThread(backgroundThread);
    debugTimer->setSingleShot(true);
    debugTimer->start();
}

void CodeManager::onDebugProcess()
{
    QScriptValue result = engine->evaluate(script);

    if (engine->hasUncaughtException()) {
        int line = engine->uncaughtExceptionLineNumber();
        QString errorMessage = "uncaught exception at line" + QString::number(line)  + ":" + result.toString();

        qDebug() << "[CodeManager] exception : " << errorMessage;
        emit signalException(errorMessage);
    }

    emit signalFinish();
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
