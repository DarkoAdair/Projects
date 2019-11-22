#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent, GameManager *gameEngine)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    codeEditor = new CodeEditor(this);
    ui->codeEditlLayout->addWidget(codeEditor);

    completer = new QCompleter(this);
    completer->setModel(modelFromFile(":/command.txt"));
    //completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    codeEditor->setCompleter(completer);

    codeEditor->appendPlainText("player.moveRight(1)\n");
    codeEditor->appendPlainText("player.moveDown(1)\n");
    codeEditor->appendPlainText("player.moveLeft(1)\n");
    codeEditor->appendPlainText("player.moveUp(1)\n");

    ui->debugLeftButton->setEnabled(false);
    ui->debugRightButton->setEnabled(false);

    QObject::connect(gameEngine, SIGNAL(movePlayer(int,int,bool)),
                     this, SLOT(movePlayer(int,int,bool)));


    codeManager = new CodeManager(gameEngine);
    connect(codeManager, SIGNAL(signalLineChanged(int)), this, SLOT(onDebugLineChanged(int)));
    connect(codeManager, SIGNAL(signalException(const QString)), this, SLOT(onDebugException(const QString)));
    connect(codeManager, SIGNAL(signalFinish()), this, SLOT(onRunningFinsih()));

    //set icon
    ui->debugButton->setIcon(QIcon (QPixmap (":/debug.png")));             //debugger
    ui->debugButton->setIconSize(QSize(33,33));
    ui->debugButton->setStyleSheet("background-color: rgba(255, 255, 255, 20);");
    ui->debugLeftButton->setIcon(QIcon (QPixmap (":/debugLeft.png")));             //debugLeft
    ui->debugLeftButton->setIconSize(QSize(33,33));
    ui->debugLeftButton->setStyleSheet("background-color: rgba(255, 255, 255, 20);");
    ui->debugRightButton->setIcon(QIcon (QPixmap (":/debugRight.png")));             //debugRight
    ui->debugRightButton->setIconSize(QSize(33,33));
    ui->debugRightButton->setStyleSheet("background-color: rgba(255, 255, 255, 20);");
    ui->debugStopButton->setIcon(QIcon (QPixmap (":/debugStop.png")));             //debugRight
    ui->debugStopButton->setIconSize(QSize(40,40));
    ui->debugStopButton->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    ui->debugStopButton->setIconSize(QSize(33,33));
    ui->debugStopButton->setStyleSheet("background-color: rgba(255, 255, 255, 20);");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::movePlayer(int _x, int _y, bool gameOver) {
    int x = ui->playField->x() + ui->playerLabel->width() * _x;
    int y = ui->playField->y() + ui->playerLabel->width() * _y - ui->playerLabel->height()/3;
    ui->playerLabel->setGeometry(x, y, ui->playerLabel->width(), ui->playerLabel->height());
    ui->playerTopLabel->setGeometry(x, y, ui->playerTopLabel->width(), ui->playerTopLabel->height());
    QString xString = "x: ";
    xString.append(QString::number(_x));
    QString yString = "y: ";
    yString.append(QString::number(_y));
    ui->xLabel->setText(xString);
    ui->yLabel->setText(yString);
}

void MainWindow::on_goButton_clicked()
{
    this->codeEditor->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    codeManager->run(codeEditor->toPlainText(), 1000);
}

void MainWindow::on_debugButton_clicked()
{
    this->codeEditor->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    ui->debugLeftButton->setEnabled(true);
    ui->debugRightButton->setEnabled(true);
    ui->debugButton->setEnabled(false);

    codeManager->debug(codeEditor->toPlainText());
}

void MainWindow::on_debugLeftButton_clicked()
{

}

void MainWindow::on_debugRightButton_clicked()
{
    codeManager->moveNextLine();
}

void MainWindow::on_debugStopButton_clicked()
{
    ui->debugLeftButton->setEnabled(false);
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

    ui->debugLeftButton->setEnabled(false);
    ui->debugRightButton->setEnabled(false);
    ui->debugButton->setEnabled(true);
}

void MainWindow::onRunningFinsih()
{
    qDebug() << "[Main] [onRunningFinish] Finish Debugging";

    this->codeEditor->setTextInteractionFlags(Qt::TextInteractionFlag::TextEditorInteraction);

    ui->debugLeftButton->setEnabled(false);
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
