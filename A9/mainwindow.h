#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <queue>

#include "codeeditor.h"
#include "codemanager.h"
#include "Box2D/Box2D.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int targetX = 0;
    int targetY = 0;
    int xStep = 0;
    int yStep = 0;
    int runNum = 0;
    int currentRun = 0;
    std::queue<int> xTargets;
    std::queue<int> yTargets;
    bool gameOver = false;
    QPixmap *fieldPixmap;

public:
    MainWindow(QWidget *parent = nullptr, GameManager *gameEngine = nullptr);
    ~MainWindow();
    void updateCoordinateLabels();

signals:
    void signalGameOver();

private slots:
    void resetBoard();

    void on_goButton_clicked();
    void on_debugButton_clicked();
    void on_debugRightButton_clicked();
    void on_debugStopButton_clicked();

    //GameEngine
    void movePlayer(int x = 0, int y = 0, bool mainCommand = false, bool gameOver = false);
    void updateLevelCount(int level);
    void usedKey();
    void usedWeapon();
    void updateInventory(int pickup, bool status);

    //CodeManager
    void onDebugLineChanged(int currentLine);
    void onDebugException(const QString errorMessage);
    void onRunningFinsih();

    //PhysicsEngine
    void onPhysicsUpdate();
    void onPlayerDead(int deadPosX, int deadPosY);
    void tutorial(int level);

private:
    Ui::MainWindow *ui;

    CodeEditor *codeEditor = nullptr;
    CodeManager *codeManager = nullptr;
    GameManager *gameEngine = nullptr;
    QCompleter *completer = nullptr;

    b2World* world = nullptr;
    QTimer* physicsTimer = nullptr;

    void addBloodParticles(int deadPosX, int deadPosY, int amount);
    int generateRandomNumber(int low, int high);

    QAbstractItemModel *modelFromFile(const QString& fileName);
};
#endif // MAINWINDOW_H
