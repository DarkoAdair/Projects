#ifndef CODEMANAGER_H
#define CODEMANAGER_H

#include <QObject>
#include <QAction>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include <QThread>
#include <QTimer>

#include "commandimpl.h"
#include "scriptdebugger.h"
#include "debuggerthread.h"

class CodeManager : public QObject
{
    Q_OBJECT

public:
    CodeManager();
    ~CodeManager();

    void run(QString script, int delay);
    void debug(QString script);
    void moveNextLine();

private:
    QScriptEngine *engine = nullptr;
    ScriptDebugger *debugger = nullptr;
    CommandImpl *commandImpl = nullptr;
    QThread *backgroundThread = nullptr;
    QTimer *debugTimer = nullptr;
    QTimer *runningTimer = nullptr;
    QString script;

    void initalize();
    void deinitalize();

    void scriptRun(QString script);

public slots:
    void onDebugProcess();
    void onRunningProcess();
    void onLineNumberChanged(int currentLine);

signals:
    void signalLineChanged(int currentLine);
    void signalException(const QString errorMessage);
    void signalFinish();
};



#endif // CODEMANAGER_H
