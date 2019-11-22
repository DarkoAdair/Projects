 #include "scriptbreakpointinfo.h"

 ScriptBreakpointManager::ScriptBreakpointManager()
 {
 }

 ScriptBreakpointManager::~ScriptBreakpointManager()
 {
 }

 bool ScriptBreakpointManager::hasBreakpoints() const
 {
     return !breakpoints.isEmpty();
 }

 int ScriptBreakpointManager::setBreakpoint(const QString &fileName, int lineNumber)
 {
     breakpoints.append(ScriptBreakpointInfo(fileName, lineNumber));
     return breakpoints.size() - 1;
 }

 int ScriptBreakpointManager::setBreakpoint(const QString &functionName, const QString &fileName)
 {
     breakpoints.append(ScriptBreakpointInfo(functionName, fileName));
     return breakpoints.size() - 1;
 }

 int ScriptBreakpointManager::setBreakpoint(const QScriptValue &function)
 {
     breakpoints.append(ScriptBreakpointInfo(function));
     return breakpoints.size() - 1;
 }

 void ScriptBreakpointManager::removeBreakpoint(int id)
 {
     if (id >= 0 && id < breakpoints.size())
         breakpoints[id] = ScriptBreakpointInfo();
 }

 int ScriptBreakpointManager::findBreakpoint(const QString &fileName, int lineNumber) const
 {
     for (int i = 0; i < breakpoints.size(); ++i) {
         const ScriptBreakpointInfo &brk = breakpoints.at(i);
         if (brk.type != ScriptBreakpointInfo::File)
             continue;
         if (brk.fileName == fileName && brk.lineNumber == lineNumber)
             return i;
     }
     return -1;
 }

 int ScriptBreakpointManager::findBreakpoint(const QString &functionName, const QString &fileName) const
 {
     for (int i = 0; i < breakpoints.size(); ++i) {
         const ScriptBreakpointInfo &brk = breakpoints.at(i);
         if (brk.type != ScriptBreakpointInfo::FunctionName)
             continue;
         if (brk.functionName == functionName && brk.fileName == fileName)
             return i;
     }
     return -1;
 }

 int ScriptBreakpointManager::findBreakpoint(const QScriptValue &function) const
 {
     for (int i = 0; i < breakpoints.size(); ++i) {
         const ScriptBreakpointInfo &brk = breakpoints.at(i);
         if (brk.type != ScriptBreakpointInfo::Function)
             continue;
         if (brk.function.strictlyEquals(function))
             return i;
     }
     return -1;
 }

 bool ScriptBreakpointManager::isBreakpointEnabled(int id) const
 {
     return breakpoints.value(id).enabled;
 }

 void ScriptBreakpointManager::setBreakpointEnabled(int id, bool enabled)
 {
     if (id >= 0 && id < breakpoints.size())
         breakpoints[id].enabled = enabled;
 }

 QString ScriptBreakpointManager::breakpointCondition(int id) const
 {
     return breakpoints.value(id).condition;
 }

 void ScriptBreakpointManager::setBreakpointCondition(int id, const QString &expression)
 {
     if (id >= 0 && id < breakpoints.size())
         breakpoints[id].condition = expression;
 }

 int ScriptBreakpointManager::breakpointIgnoreCount(int id) const
 {
     return breakpoints.value(id).ignoreCount;
 }

 void ScriptBreakpointManager::setBreakpointIgnoreCount(int id, int ignoreCount)
 {
     if (id >= 0 && id < breakpoints.size())
         breakpoints[id].ignoreCount = ignoreCount;
 }

 bool ScriptBreakpointManager::isBreakpointSingleShot(int id) const
 {
     return breakpoints.value(id).singleShot;
 }

 void ScriptBreakpointManager::setBreakpointSingleShot(int id, bool singleShot)
 {
     if (id >= 0 && id < breakpoints.size())
         breakpoints[id].singleShot = singleShot;
 }
