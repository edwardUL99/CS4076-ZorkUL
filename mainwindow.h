#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ZorkUL.h"
#include "Character.h"
#include "helpdialog.h"
#include "inventorywindow.h"
#include "itemtimer.h"

#include <QMainWindow>
#include <map>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    // 1. Destructors
    ~MainWindow();

    enum HealthBars {
        PLAYER_HEALTH,
        MOBILE_ENEMY_HEALTH,
        STATIC_ENEMY_HEALTH
    };

    enum StaminaBars {
        PLAYER_STAMINA,
        MOBILE_ENEMY_STAMINA,
        STATIC_ENEMY_STAMINA
    };

    void reset();
    void setHealth(int health, HealthBars healthBar);
    void setStamina(int stamina, StaminaBars staminaBar);
    void setPoints(int points);
    void setExperience(int experience);
    void addText(string str);
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_west_clicked();

    void on_north_clicked();

    void on_south_clicked();

    void on_east_clicked();

    void on_teleport_clicked();

    void on_gameWon(); //Winning condition is if you visit every room at least once or if lucky, you pick up the emerald

    void on_gameLost(); //You lose if you lose all your health (you lose health by crashing into walls)

    void on_EnemyDeath(Enemy &enemy);

    void on_WeaponDestroyed(Weapon* weapon);

    void on_StaminaTimeOut();

    void on_HealthTimeOut();

    void on_AttackTimeOut();

    void on_ItemTimeOut(ItemTimerUnit timerItem);

    void on_help_clicked();

    void on_comboBox_activated(const QString &arg1);

    void on_inventory_clicked();

    void on_map_clicked();

    void on_attack_clicked();

    void on_restart_clicked();

private:
    const static map<int, int> levelRanges;

    Ui::MainWindow *ui;
    HelpDialog *help;
    InventoryWindow *inventory;
    ZorkUL *game;
    bool enemiesKilled;
    bool inBattle; //while inBattle is true, the timer shouldn't attack also. inBattle is initiated by pressing attack, and uninitiated when leaving the room
    QTimer *staminaTimer;
    QTimer *healthTimer;
    QTimer *attackTimer;
    ItemTimer *emeraldTimer;//QTimer *emeraldTimer;
    map<string, ItemTimer*> potionTimers;
    map<string, ItemTimer*> lightTimers;
    void visitRoom(string roomDescription, bool teleported = false); //teleported is true if you teleported into the room
    void addRoomItemsToComboBox();
    void hideEnemyBars(bool hide, HealthBars healthBar, StaminaBars staminaBar);
    void processItem(Item *item); //processes item i.e. checks if it is a boost/potion or regular item
    void updateWeapon(string weapon);
    void stopTimers();
    string getTimedItemDescription(Item *item); //returns a description stating how long the item will last for
    bool isEnemyInPlayerRoom(Enemy &enemy);
    void attackPlayer();
    // 6. Friends.
    friend class InventoryWindow;
};
#endif // MAINWINDOW_H
