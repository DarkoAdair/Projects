#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>


MainWindow::MainWindow(QWidget *parent, GameManager *gameEngine)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    codeEditor = new CodeEditor(this);
    ui->setupUi(this);
    ui->codeEditlLayout->addWidget(codeEditor);

//    codeEditor->appendPlainText("command.print(\"1\")\n");
//    codeEditor->appendPlainText("command.print(\"1\")\n");

    ui->debugLeftButton->setEnabled(false);
    ui->debugRightButton->setEnabled(false);

    QObject::connect(gameEngine, SIGNAL(movePlayer(int,int,bool)),
                     this, SLOT(movePlayer(int,int,bool)));


    codeManager = new CodeManager(gameEngine);

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

    connect(codeManager, SIGNAL(signalLineChanged(int)), this, SLOT(onDebugLineChanged(int)));
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

    QString errorMessage;
    bool result = codeManager->run(codeEditor->toPlainText(), &errorMessage);

    // throwing excetpoin
    if (!result) {
        codeManager->line = codeManager->engine->uncaughtExceptionLineNumber();

void MainWindow::on_goButton_clicked()
{
    codeManager->run(codeEditor->toPlainText(), 1000);
}

void MainWindow::on_debugButton_clicked()
{
    ui->debugLeftButton->setEnabled(true);
    ui->debugRightButton->setEnabled(true);
    ui->debugButton->setEnabled(false);
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
    qDebug() << "DEBUG : " << currentLine;
}
