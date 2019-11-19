#ifndef CODEMANAGER_H
#define CODEMANAGER_H

#include <QJSEngine>

#include "commandimpl.h"

class CodeManager
{
public:
    CodeManager();

    void run(QString test);

private:
    QJSEngine *engine;
    CommandImpl *commandImpl;
};

#endif // CODEMANAGER_H
