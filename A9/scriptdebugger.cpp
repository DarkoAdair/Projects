/****************************************************************************
 **
 ** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
 **     of its contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #include "scriptdebugger.h"
 #include "scriptbreakpointinfo.h"

 #include <QtScript/QScriptEngine>
 #include <QtScript/QScriptEngineAgent>
 #include <QtScript/QScriptContextInfo>
 #include <QtScript/QScriptValueIterator>
 #include <QtCore/QTextStream>
 #include <QtCore/QStack>
 #include <QDebug>
 #include <QApplication>
 #include <QThread>

 class ScriptDebuggerPrivate : public QScriptEngineAgent
 {
     Q_DECLARE_PUBLIC(ScriptDebugger)

 public:
     enum Mode {
         Run,
         StepInto
     };

     ScriptDebuggerPrivate(QScriptEngine *engine);
     ~ScriptDebuggerPrivate();

     // QScriptEngineAgent interface
     void scriptLoad(qint64 id, const QString &program,
                     const QString &fileName, int lineNumber);
     void scriptUnload(qint64 id);

     void positionChange(qint64 scriptId,
                         int lineNumber, int columnNumber);

     void functionEntry(qint64 scriptId);
     void functionExit(qint64 scriptId,
                       const QScriptValue &returnValue);

     void exceptionThrow(qint64 scriptId,
                         const QScriptValue &exception, bool hasHandler);

     void interactive();

     void setMode(Mode mode);
     Mode mode() const;

     int listLineNumber() const;
     void setListLineNumber(int lineNumber);

     Mode m_mode;

     ScriptDebugger *q_ptr;

     bool moveNext = false;
 };

 ScriptDebuggerPrivate::ScriptDebuggerPrivate(QScriptEngine *engine)
     : QScriptEngineAgent(engine), m_mode(Run)
 {
 }

 ScriptDebuggerPrivate::~ScriptDebuggerPrivate()
 {
 }

 void ScriptDebuggerPrivate::setMode(Mode mode)
 {
     m_mode = mode;
 }

 ScriptDebuggerPrivate::Mode ScriptDebuggerPrivate::mode() const
 {
     return m_mode;
 }

 void ScriptDebugger::moveNext()
 {
     d_ptr->moveNext = true;
 }

 void ScriptDebuggerPrivate::scriptLoad(qint64 id, const QString &program,
                                        const QString &fileName, int lineNumber)
 {
     qDebug() << "[ScriptLoad] ID : " << id << "/ Program : " << program << "/ Filename : " << fileName << "/ Linenumber : " << lineNumber;
 }

 void ScriptDebuggerPrivate::scriptUnload(qint64 id)
 {
     qDebug() << "[ScriptUnload] ID : " << id;
 }

 void ScriptDebuggerPrivate::functionEntry(qint64 scriptId)
 {
     qDebug() << "[Function Entry] ScriptId" << scriptId;
 }

 void ScriptDebuggerPrivate::functionExit(qint64 scriptId,
                                          const QScriptValue &returnValue)
 {
     qDebug() << "[Function Exit] ScriptId" << scriptId << " / retureValue : " << returnValue.toString();
 }

 void ScriptDebuggerPrivate::positionChange(qint64 scriptId,
                                            int lineNumber, int columnNumber)
 {
     qDebug() << "[Position Change] ScriptId" << scriptId << " / Linenumber : " << lineNumber << " / Columnnumber : " << columnNumber;

     bool enterInteractiveMode = false;

     //TODO - Breakpoint
     //if (m_bpManager->hasBreakpoints()) {
     //
     //}

     switch (mode()) {
     case Run:
         break;

     case StepInto:
         enterInteractiveMode = true;
         break;
     }

     if (enterInteractiveMode) { 
         while(!moveNext)
            QApplication::processEvents();

         moveNext = false;
     }
 }

 void ScriptDebuggerPrivate::exceptionThrow(qint64 scriptId,
                                            const QScriptValue &exception,
                                            bool hasHandler)
 {
     qDebug() << "[Exception Throw] ScriptId" << scriptId << " / exception : " << exception.toString() << " / hasHandler : " << hasHandler;

     if (!hasHandler) {
        //TODO - Exception Singal
     }
 }

 ScriptDebugger::ScriptDebugger(QScriptEngine *engine)
     : d_ptr(new ScriptDebuggerPrivate(engine))
 {
     d_ptr->q_ptr = this;
     engine->setAgent(d_ptr);
 }

 ScriptDebugger::ScriptDebugger(QScriptEngine *engine, ScriptDebuggerPrivate &dd)
     : d_ptr(&dd)
 {
     d_ptr->q_ptr = this;
     engine->setAgent(d_ptr);
 }

 ScriptDebugger::~ScriptDebugger()
 {
     delete d_ptr;
     d_ptr = nullptr;
 }

 void ScriptDebugger::breakAtNextStatement()
 {
     Q_D(ScriptDebugger);
     d->setMode(ScriptDebuggerPrivate::StepInto);
 }

 void ScriptDebugger::setBreakpoint(int lineNumber)
 {
     Q_D(ScriptDebugger);
     //TODO
 }
