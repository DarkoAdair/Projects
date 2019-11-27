#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QDebug>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent, GameManager *_gameEngine)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    gameEngine = _gameEngine;
    ui->setupUi(this);

    QDateTime cd = QDateTime::currentDateTime();
    qsrand(cd.toTime_t());

    codeEditor = new CodeEditor(this);
    ui->codeEditlLayout->addWidget(codeEditor);

    completer = new QCompleter(this);
    completer->setModel(modelFromFile(":/command.txt"));
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    codeEditor->setCompleter(completer);

    codeEditor->appendPlainText("player.moveUp()\n");
    codeEditor->appendPlainText("player.moveRight()\n");
    codeEditor->appendPlainText("player.moveDown()\n");
    codeEditor->appendPlainText("player.moveRight()\n");

    ui->debugRightButton->setEnabled(false);

    QObject::connect(gameEngine, SIGNAL(movePlayer(int,int,bool,bool)),
                     this, SLOT(movePlayer(int,int,bool,bool)));
    QObject::connect(gameEngine, SIGNAL(updateLevelAndMap(int)),
                     this, SLOT(updateLevelAndMap(int)));
    QObject::connect(gameEngine, SIGNAL(resetSignal()),
                     this, SLOT(resetBoard()));
    QObject::connect(this, SIGNAL(signalGameOver()),
                     gameEngine, SLOT(checkLevelCompletionReset()));
    QObject::connect(gameEngine, SIGNAL(useKeySignal()), this, SLOT(usedKey()));
    QObject::connect(gameEngine, SIGNAL(useWeaponSignal()), this, SLOT(usedWeapon()));
    QObject::connect(gameEngine, SIGNAL(updateInventory(int, bool)), this, SLOT(updateInventory(int, bool)));
    QObject::connect(gameEngine, SIGNAL(deadSignal(int, int)), this, SLOT(onPlayerDead(int, int)));

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

    //set console
    ui->console->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);

//    std::array<QLabel*, 5> labels{ui->playField, ui->playerLabel,
//                ui->playerTopLabel, ui->level1Label, ui->finish1Label};
//    for(int i = 0; i < labels.size(); i++) {
//        QPixmap pixmap = labels[i]->pixmap()->copy();
//        pixmap = pixmap.scaled(labels[i]->width(), labels[i]->height(), Qt::KeepAspectRatio);
//        labels[i]->setPixmap(pixmap);
//    }
    ui->mapSection->setStyleSheet("QWidget {border-style: none;}");

    //Set gravity to 0
    b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
    world = new b2World(gravity);
    world->SetAllowSleeping(true);
    world->SetContinuousPhysics(true);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);

    b2EdgeShape groundEdge;
    b2FixtureDef boxShapeDef;
    boxShapeDef.shape = &groundEdge;

    physicsTimer = new QTimer(this);
    connect(physicsTimer, SIGNAL(timeout()), this, SLOT(onPhysicsUpdate()));
    physicsTimer->setInterval(1);
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
            xStep = 2 * xStep;
        }
        if(yOff != 0) {
            yStep = yOff/std::abs(yOff);
            yStep = 2 * yStep;
        }
    }

    // Move actual player sprite
    int x = ui->playerLabel->x() + xStep;
    int y = ui->playerLabel->y() + yStep;
    ui->playerLabel->setGeometry(x, y, ui->playerLabel->width(), ui->playerLabel->height());
    ui->playerTopLabel->setGeometry(x, y, ui->playerTopLabel->width(), ui->playerTopLabel->height());

    // Update player variable labels
    updateCoordinateLabels();

    // If the player is not in the right spot yet...
    if(ui->playerLabel->y()+ui->playerLabel->height()/3 != ui->playField->y()+ui->playerLabel->width()*targetY || ui->playerLabel->x() != ui->playField->x()+ui->playerLabel->width()*targetX) {
        QTimer::singleShot(5, this, SLOT(movePlayer()));
        return;
    }
    // Else move on to next target position
    else {
        QTimer::singleShot(100, codeManager, SLOT(onAnimationFinished()));

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
                xStep = 2 * xStep;
            }
            if(yOff != 0) {
                yStep = yOff/std::abs(yOff);
                yStep = 2 * yStep;
            }

            // A bit of a longer delay between separate commands
            QTimer::singleShot(100, this, SLOT(movePlayer()));
            //return;
        }
        else if(gameOver) {
            QTimer::singleShot(1000, this, SIGNAL(signalGameOver()));
        }
    }
}

void MainWindow::updateLevelAndMap(int level)
{
    QString levelString = "Level: ";
    levelString.append(QString::number(level));
    ui->levelLabel->setText(levelString);
}


void MainWindow::usedKey()
{

    // move labels to remove doorways
}

void MainWindow::usedWeapon()
{

    // move labels to remove enemys
}

void MainWindow::updateInventory(int pickup, bool status)
{
    QString item;
    switch(pickup)
    {
        case 0:
            item = "Holding Key: ";
            item.append(QVariant(status).toString());
            ui->keyLabel->setText(item);
            break;
        case 1:
            item = "Holding Weapon: ";
            item.append(QVariant(status).toString());
            ui->weaponLabel->setText(item);
            break;
        default:
            break;
    }
}

void MainWindow::updateCoordinateLabels(){
    QString xString = "x: ";
    xString.append(QString::number((ui->playerLabel->x()-ui->playField->x())/ui->playerLabel->width()));
    QString yString = "y: ";
    yString.append(QString::number((ui->playerLabel->y()+ui->playerLabel->height()/3-ui->playField->y())/ui->playerLabel->width()));
    ui->xLabel->setText(xString);
    ui->yLabel->setText(yString);
}

void MainWindow::on_goButton_clicked()
{
    resetBoard();
    ui->goButton->setEnabled(false);
    this->codeEditor->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    codeManager->run(codeEditor->toPlainText(), 1000);
}

void MainWindow::resetBoard() {
    targetX = 0;
    targetY = 0;
    xStep = 0;
    yStep = 0;
    gameEngine->resetPlayer();
    ui->goButton->setEnabled(true);
    ui->playerLabel->setGeometry(ui->playField->x()+gameEngine->getPlayerX()*ui->playerLabel->width(), ui->playField->y()-ui->playerLabel->height()/3+gameEngine->getPlayerY()*ui->playerLabel->width(), ui->playerLabel->width(), ui->playerLabel->height());
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
    updateCoordinateLabels();

    QPixmap pixmap = QPixmap(":/level_" + QString::number(gameEngine->getLevelCount()) + ".png");
    ui->level1Label->setPixmap(pixmap);
}

void MainWindow::on_debugButton_clicked()
{
    resetBoard();
    ui->goButton->setEnabled(false);
    this->codeEditor->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);

    ui->debugRightButton->setEnabled(true);
    ui->debugButton->setEnabled(false);
    ui->debugStopButton->setEnabled(true);

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

    this->codeEditor->setTextInteractionFlags(Qt::TextInteractionFlag::TextEditorInteraction);

    ui->debugStopButton->setEnabled(false);

}

void MainWindow::onDebugLineChanged(int currentLine)
{
    qDebug() << "[Main] [onDebugLineChanged] Line : " << currentLine;

    codeEditor->lineHighlighter(currentLine);

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
    ui->goButton->setEnabled(true);
    qDebug() << "[Main] [onRunningFinish] Finish Debugging";

    this->codeEditor->setTextInteractionFlags(Qt::TextInteractionFlag::TextEditorInteraction);

    ui->debugRightButton->setEnabled(false);
    ui->debugButton->setEnabled(true);

    ui->debugStopButton->setEnabled(false);

}

void MainWindow::onPhysicsUpdate()
{
    int velocityIterations = 8;
    int positionIterations = 3;

    // Simiulate
    world->Step(1, velocityIterations, positionIterations);

    // Update all objects.
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        int x = b->GetPosition().x;
        int y = b->GetPosition().y;

        int mapWidth = ui->mapSection->geometry().width();
        int mapHeight = ui->mapSection->geometry().height();

        if (b->GetUserData() != nullptr) {
            QLabel* spriteData = (QLabel *)b->GetUserData();

            //If it goes out of map, delete.
            if(x < -1 || y < -1 || mapWidth < x || mapHeight < y)
            {
                 world->DestroyBody(b);
                 delete spriteData;
            }
            else
            {
                spriteData->setGeometry(x, y, spriteData->width(), spriteData->height());
            }
        }
    }

    if(world->GetBodyCount() == 0)
        physicsTimer->stop();
}

void MainWindow::onPlayerDead(int deadPosX, int deadPosY)
{
    int posPlayerX = ui->playerLabel->geometry().x();
    int posPlayerY = ui->playerLabel->geometry().y();

    int posX = posPlayerX - (ui->playerLabel->geometry().width() / 2);
    int posY = posPlayerY - (ui->playerLabel->geometry().height() / 2);

    addBloodParticles(posX, posY, 100);
    physicsTimer->start();
}

void MainWindow::addBloodParticles(int deadPosX, int deadPosY, int amount)
{
    while(amount-- > 0)
    {
        QLabel* qSprite = new QLabel(this);
        qSprite->setGeometry(deadPosX, deadPosY, 16, 16);

        // Pick random blood particle
        int randomBlood = generateRandomNumber(1, 5);
        QPixmap pixmap = QPixmap(":/blood_" + QString::number(randomBlood) + ".png");
        pixmap = pixmap.scaled(qSprite->width(), qSprite->height(), Qt::KeepAspectRatio);
        qSprite->setPixmap(pixmap);
        qSprite->raise();

        // Set body position
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(deadPosX, deadPosY);
        bodyDef.userData = qSprite;
        b2Body* body = world->CreateBody(&bodyDef);

        // Set velocity
        int vX = generateRandomNumber(-10, 10);
        int vY = generateRandomNumber(-10, 10);

        body->SetLinearVelocity(b2Vec2(vX, vY));

        b2CircleShape circle;
        circle.m_radius = 0.55f;

        // Set fixture for object
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circle;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.2f;
        fixtureDef.restitution = 0.9f;
        body->CreateFixture(&fixtureDef);
    }
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

int MainWindow::generateRandomNumber(int low, int high)
{
<<<<<<< HEAD
=======

>>>>>>> ba07a7e076333fa780bec26262457ff0aa6a6720
    return qrand() % ((high + 1) - low) + low;
}
