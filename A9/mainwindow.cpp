#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent, GameManager *_gameEngine)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    gameEngine = _gameEngine;
    codeEditor = new CodeEditor(this);
    ui->setupUi(this);

    codeEditor = new CodeEditor(this);
    ui->codeEditlLayout->addWidget(codeEditor);

    completer = new QCompleter(this);
    completer->setModel(modelFromFile(":/command.txt"));
    //completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    codeEditor->setCompleter(completer);

   // codeEditor->appendPlainText("player.moveRight(1)\n");
   // codeEditor->appendPlainText("player.moveDown(1)\n");
   // codeEditor->appendPlainText("player.moveLeft(1)\n");
   // codeEditor->appendPlainText("player.moveUp(1)\n");


    ui->debugRightButton->setEnabled(false);

    QObject::connect(gameEngine, SIGNAL(movePlayer(int,int,bool,bool)),
                     this, SLOT(movePlayer(int,int,bool,bool)));

    QObject::connect(gameEngine, SIGNAL(updateLevelAndMap(int)), this, SLOT(updateLevelAndMap(int)));


    QObject::connect(this, SIGNAL(signalGameOver()), gameEngine, SLOT(checkLevelCompletionReset()));
    codeManager = new CodeManager(gameEngine);
    connect(codeManager, SIGNAL(signalLineChanged(int)), this, SLOT(onDebugLineChanged(int)));
    connect(codeManager, SIGNAL(signalException(const QString)), this, SLOT(onDebugException(const QString)));
    connect(codeManager, SIGNAL(signalFinish()), this, SLOT(onRunningFinsih()));

    //set icon
    ui->debugButton->setIcon(QIcon (QPixmap (":/debug.png")));             //debugger
    ui->debugButton->setIconSize(QSize(33,33));
    ui->debugButton->setStyleSheet("background-color: rgba(255, 255, 255, 20);");
    ui->debugRightButton->setIcon(QIcon (QPixmap (":/debugRight.png")));             //debugRight
    ui->debugRightButton->setIconSize(QSize(33,33));
    ui->debugRightButton->setStyleSheet("background-color: rgba(255, 255, 255, 20);");
    ui->debugStopButton->setIcon(QIcon (QPixmap (":/debugStop.png")));             //debugRight
    ui->debugStopButton->setIconSize(QSize(40,40));
    ui->debugStopButton->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->debugStopButton->setIconSize(QSize(33,33));
    ui->debugStopButton->setStyleSheet("background-color: rgba(255, 255, 255, 20);");
    std::array<QLabel*, 9> labels{ui->lavaLabel, ui->playField, ui->playerLabel,
                ui->playerTopLabel, ui->spikesLabel, ui->spikesLabel_2, ui->spikesLabel_3,
                ui->keyLabel, ui->lockLabel};
    for(int i = 0; i < labels.size(); i++) {
        QPixmap pixmap = labels[i]->pixmap()->copy();
        pixmap = pixmap.scaled(labels[i]->width(), labels[i]->height(), Qt::KeepAspectRatio);
        labels[i]->setPixmap(pixmap);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::movePlayer(int _x, int _y, bool mainCommand, bool _gameOver) {
    if(mainCommand) {
        gameOver = _gameOver;
        xTargets.push(_x);
        yTargets.push(_y);

        if(xTargets.size()>1) {
            return;
        }

        targetX = xTargets.front();
        targetY = yTargets.front();

        int xOff = xTargets.front()-((ui->playerLabel->x()-ui->playField->x())/ui->playerLabel->width());
        int yOff = yTargets.front()-((ui->playerLabel->y()+ui->playerLabel->height()/3-ui->playField->y())/ui->playerLabel->width());

        xStep = 0;
        yStep = 0;

        if(xOff != 0) {
            xStep = xOff/std::abs(xOff);
        }
        if(yOff != 0) {
            yStep = yOff/std::abs(yOff);
        }
    }

    // Move actual player sprite
    int x = ui->playerLabel->x() + xStep;
    int y = ui->playerLabel->y() + yStep;
    ui->playerLabel->setGeometry(x, y, ui->playerLabel->width(), ui->playerLabel->height());
    ui->playerTopLabel->setGeometry(x, y, ui->playerTopLabel->width(), ui->playerTopLabel->height());

    // Update player variable labels
    QString xString = "x: ";
    xString.append(QString::number((ui->playerLabel->x()-ui->playField->x())/ui->playerLabel->width()));
    QString yString = "y: ";
    yString.append(QString::number((ui->playerLabel->y()+ui->playerLabel->height()/3-ui->playField->y())/ui->playerLabel->width()));
    ui->xLabel->setText(xString);
    ui->yLabel->setText(yString);

    // If the player is not in the right spot yet...
    if(ui->playerLabel->y()+ui->playerLabel->height()/3 != ui->playField->y()+ui->playerLabel->width()*targetY || ui->playerLabel->x() != ui->playField->x()+ui->playerLabel->width()*targetX) {
        QTimer::singleShot(5, this, SLOT(movePlayer()));
        //return;
    }
    // Else move on to next target position
    else {
        int prevX = xTargets.front();
        int prevY = yTargets.front();

        xTargets.pop();
        yTargets.pop();

        if(!xTargets.empty()) {
            targetX = xTargets.front();
            targetY = yTargets.front();

            int xOff = xTargets.front()-prevX;
            int yOff = yTargets.front()-prevY;

            xStep = 0;
            yStep = 0;

            if(xOff != 0) {
                xStep = xOff/std::abs(xOff);
            }
            if(yOff != 0) {
                yStep = yOff/std::abs(yOff);
            }

            // A bit of a longer delay between separate commands
            QTimer::singleShot(100, this, SLOT(movePlayer()));
            //return;
        }
    }

    if(gameOver)
    {
        //do some physics animations if dead or if win
        //int reaction = gameEngine->getWhatsAtMove(_x, _y);

        // switch case 1 - 9 based on where player moved to


        QTimer::singleShot(1000, this, SIGNAL(signalGameOver()));
        //emit signalGameOver();


    }

void MainWindow::updateLevelAndMap(int level)
{
    QString levelString = "Level: ";
    levelString.append(QString::number(level));
    ui->levelLabel->setText(levelString);
}

void MainWindow::on_goButton_clicked()
{
    ui->playerLabel->setGeometry(ui->playField->x(), ui->playField->y()-ui->playerLabel->height()/3, ui->playerLabel->width(), ui->playerLabel->height());
    ui->playerTopLabel->setGeometry(ui->playerLabel->x(), ui->playerLabel->y(), ui->playerTopLabel->width(), ui->playerTopLabel->height());
    int numTargets = xTargets.size();
    for(int i = 0; i < numTargets; i++) {
        xTargets.pop();
        yTargets.pop();
    }
    targetX = 0;
    targetY = 0;
    xStep = 0;
    yStep = 0;
    this->codeEditor->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    codeManager->run(codeEditor->toPlainText(), 1000);

}

void MainWindow::on_debugButton_clicked()
{

    this->codeEditor->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);

    ui->debugRightButton->setEnabled(true);
    ui->debugButton->setEnabled(false);

    codeManager->debug(codeEditor->toPlainText());
}

void MainWindow::on_debugRightButton_clicked()
{
    codeManager->moveNextLine();
}

void MainWindow::on_debugStopButton_clicked()
{

    ui->debugRightButton->setEnabled(false);
    ui->debugButton->setEnabled(true);
    ui->debugStopButton->setFocus();
}

void MainWindow::onDebugLineChanged(int currentLine)
{
    qDebug() << "[Main] [onDebugLineChanged] Line : " << currentLine;

    //TODO - highlight code editor
}

void MainWindow::onDebugException(const QString errorMessage)
{
    qDebug() << "[Main] [onDebugException] " << errorMessage;

    ui->console->append(errorMessage);

    this->codeEditor->setTextInteractionFlags(Qt::TextInteractionFlag::TextEditorInteraction);

    ui->debugRightButton->setEnabled(false);
    ui->debugButton->setEnabled(true);
}

void MainWindow::onRunningFinsih()
{
    qDebug() << "[Main] [onRunningFinish] Finish Debugging";

    this->codeEditor->setTextInteractionFlags(Qt::TextInteractionFlag::TextEditorInteraction);

    ui->debugRightButton->setEnabled(false);
    ui->debugButton->setEnabled(true);
}

QAbstractItemModel *MainWindow::modelFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << line.trimmed();
    }

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words, completer);
}
