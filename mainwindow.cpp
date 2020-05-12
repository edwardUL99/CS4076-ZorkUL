#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "boost.h"
#include "potion.h"
#include <iostream>
#include <QKeyEvent>
using namespace std;

const map<int, int> MainWindow::levelRanges = {{1, 20},
                                               {2, 40},
                                               {3, 60},
                                               {4, 70}};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);

    QString welcome = "Welcome to ZorkUL!\n\nYou can travel from room to room using the given direction buttons\n\nUse the inventory to select your weapon/apply potions or boosts\n\nObserve the exits for each room...or else!\n\nFind the emerald and get unlimited stamina for 10 seconds!\n\nKill the enemies and you win!";
    ui->textEdit->append(welcome);

    game = new ZorkUL("Hero");

    //Create the window to be used for viewing help information
    help = new HelpDialog(parent, game->printHelp());
    inventory = new InventoryWindow(game, this);

    enemiesKilled = false;
    inBattle = false;

    ui->health->setRange(0, 100);
    ui->stamina->setRange(0, 100);
    ui->mobileHealth->setRange(0, 100);
    ui->mobileStamina->setRange(0, 100);
    ui->staticHealth->setRange(0, 100);
    ui->staticStamina->setRange(0, 100);
    ui->xpBar->setRange(0, levelRanges.at(1));

    ui->level->display(1);

    staminaTimer = new QTimer(this); //create a timer to restore stamina everytime it times out
    staminaTimer->setInterval(10000);
    healthTimer = new QTimer(this); //create a timer to restore health everytime it times out
    healthTimer->setInterval(15000);
    attackTimer = new QTimer(this);
    attackTimer->setInterval(3000);
    emeraldTimer = new ItemTimer(game->getItem("Emerald"), this, &MainWindow::on_ItemTimeOut); //create a timer to remove emerald from player after it times out
    emeraldTimer->setInterval(10000);
    emeraldTimer->setSingleShot(true);

    //Connect the signals to their slots
    connect(&game->getPlayer(), SIGNAL(gameWon()), this, SLOT(on_gameWon()));
    connect(&game->getPlayer(), SIGNAL(gameLost()), this, SLOT(on_gameLost()));
    connect(staminaTimer, SIGNAL(timeout()), this, SLOT(on_StaminaTimeOut()));
    connect(healthTimer, SIGNAL(timeout()), this, SLOT(on_HealthTimeOut()));
    connect(attackTimer, SIGNAL(timeout()), this, SLOT(on_AttackTimeOut()));

    visitRoom("A");
}

MainWindow::~MainWindow()
{
    delete staminaTimer;
    delete healthTimer;
    delete emeraldTimer;
    delete attackTimer;

    for (auto it = potionTimers.begin(); it != potionTimers.end(); it++) {
        delete it->second;
    }

    for (auto it = lightTimers.begin(); it != lightTimers.end(); it++) {
        delete it->second;
    }

    delete game;
    delete ui;
    delete help;
    delete inventory;
}

void MainWindow::addText(string str) {
    ui->textEdit->append(str.c_str());
}

void MainWindow::reset() {
    hideEnemyBars(false, MOBILE_ENEMY_HEALTH, MOBILE_ENEMY_STAMINA);
    hideEnemyBars(false, STATIC_ENEMY_HEALTH, STATIC_ENEMY_STAMINA);
    ui->textEdit->clear();
    setHealth(100, PLAYER_HEALTH);
    setHealth(100, MOBILE_ENEMY_HEALTH);
    setHealth(100, STATIC_ENEMY_HEALTH);
    setStamina(100, PLAYER_STAMINA);
    setStamina(100, STATIC_ENEMY_STAMINA);
    setStamina(100, MOBILE_ENEMY_STAMINA);
    setPoints(0);
    setExperience(0);
    ui->level->display(1);
    game->reset();
    inventory->reset();

    stopTimers();
    potionTimers.clear();
    lightTimers.clear();

    QString welcome = "Welcome to ZorkUL!\n\nYou can travel from room to room using the given direction buttons\n\nUse the inventory to select your weapon/apply potions or boosts\n\nObserve the exits for each room...or else!\n\nFind the emerald and get unlimited stamina for 10 seconds!\n\nKill the enemies and you win!";
    ui->textEdit->append(welcome);
    visitRoom("A");
}

// "visit" the room that is described by roomDescription, if not the name, the method "pulls" the room name out of it
void MainWindow::visitRoom(string roomDescription, bool teleported) {
    string roomName;
    if (roomDescription.size() == 1) roomName = roomDescription;
    else roomName = roomDescription.substr(7, 1);

    Room* r = game->getRoom(roomName);
    if (r != NULL) {
        if ((r->isLocked() && !teleported) && !game->getPlayer().hasKey()) {
            QString str;
            str += "Room ";
            str += r->getName().c_str();
            str += " is locked. You will need the key to enter\n";
            ui->textEdit->append(str);
        } else {
            inBattle = false;

            Character &character = game->getPlayer();
            QString str(r->longDescription(character.hasLight()).c_str());
            str += "\nCurrent Weapon: ";
            Weapon* weapon = character.getWeaponInHand();
            string weaponText = weapon == NULL ? "Punch":character.getWeaponInHand()->getShortDescription();
            if (weapon) {
                weaponText += ", Strength: " + to_string(weapon->getDamage());
            }
            str += weaponText.c_str();
            ui->textEdit->append(str);

            Enemy &staticEnemy = game->getEnemy(true);
            Enemy &mobileEnemy = game->getEnemy(false);

            bool staticInRoom = isEnemyInPlayerRoom(staticEnemy), mobileInRoom = isEnemyInPlayerRoom(mobileEnemy);

            if (staticInRoom || mobileInRoom) {
                if (staticInRoom && mobileInRoom) {
                   ui->textEdit->append("\nGRUE IS IN THE ROOM\nCENTAUR IS IN THE ROOM\n");
                } else if (staticInRoom) {
                    ui->textEdit->append("\nGRUE IS IN THE ROOM\n");
                } else {
                    ui->textEdit->append("\nCENTAUR IS IN THE ROOM\n");
                }
                attackTimer->start(); //start the attack cycle
            } else {
                ui->textEdit->append("No enemies in room\n");
            }

            addRoomItemsToComboBox();

            if (r->isDark() && !character.hasLight()) {
                QString msg;
                msg += "You cannot see in the dark, find a light source\n";
                ui->textEdit->append(msg);
            }

            character.setCurrentRoom(r);
        }
    }
}

void MainWindow::addRoomItemsToComboBox() {
    ui->comboBox->clear();
    Character &player = game->getPlayer();
    Room *room = player.getCurrentRoom();

    ui->comboBox->addItem("");
    if (!room->isDark() || player.hasLight()) {
        for (ObjectContainer<Item*>::iterator it = room->itemsIterator(true); it != room->itemsIterator(false); it++) {
            ui->comboBox->addItem((*it)->getShortDescription().c_str());
        }
    }
}

void MainWindow::setStamina(int stamina, StaminaBars staminaBar) {
    Character &character = game->getPlayer();
    if (stamina < 0) stamina = 0;
    if (stamina > 100) stamina = 100;
    if (staminaBar == PLAYER_STAMINA && !character.hasItem("Emerald")) {
        character.stamina = stamina;
        ui->stamina->setValue(character.stamina);
    } else if (staminaBar == MOBILE_ENEMY_STAMINA) {
        Enemy &mobileEnemy = game->getEnemy(false);
        mobileEnemy.stamina = stamina;
        ui->mobileStamina->setValue(mobileEnemy.stamina);
    } else {
        Enemy &staticEnemy = game->getEnemy(true);
        staticEnemy.stamina = stamina;
        ui->staticStamina->setValue(staticEnemy.stamina);
    }
}

void MainWindow::setHealth(int health, HealthBars healthBar) {
    if (health < 0) {
        health = 0;
        emit game->getPlayer().gameLost();
    }

    if (health > 100) health = 100;
    if (healthBar == PLAYER_HEALTH) {
        Character &player = game->getPlayer();
        player.setHealth(health);
        ui->health->setValue(player.health);
    } else if (healthBar == MOBILE_ENEMY_HEALTH) {
        Character &mobileEnemy = game->getEnemy(false);
        mobileEnemy.health = health;
        ui->mobileHealth->setValue(mobileEnemy.health);
    } else {
        Character &staticEnemy = game->getEnemy(true);
        staticEnemy.health = health;
        ui->staticHealth->setValue(staticEnemy.health);
    }
}

void MainWindow::setPoints(int points) {
    this->game->getPlayer().setPoints(points);
    ui->points->display(points);
}

//finish testing this
void MainWindow::setExperience(int experience) {
    Character &player = this->game->getPlayer();
    int extraExperience = abs(experience - player.getExperience());
    player.setExperience(experience);

    bool levelUp = false;
    int level = player.getLevel();
    int levelRange = levelRanges.at(level);

    ui->experience->display(experience);

    if (experience >= levelRange) {
        experience = level < 4 ? abs(experience - levelRange): experience;

        ui->level->display(level);
        if (level < 4) {
            player.setLevel(level + 1);
            ui->xpBar->setRange(0, levelRange);
            ui->level->display(level + 1);
        }
        ui->xpBar->setValue(experience);
        levelUp = true;
    } else {
        ui->xpBar->setValue(experience);
    }

    if (extraExperience != 0) {
        QString str("+");
        str += to_string(extraExperience).c_str();
        str += " XP\n";
        ui->textEdit->append(str);
    }

    if (levelUp) {
        QString levelup;
        levelup += "Congratulations, you have levelled up!\n";
        ui->textEdit->append(levelup);
    }

    if (game->itemManager.spawnUnlockedItems()) {
        //levelled up and new Items spawned
        QString str("You have unlocked new items, go and find them\n");
        ui->textEdit->append(str);
        inventory->update(); //update the inventory to display the unlocked items
    }
}

void MainWindow::hideEnemyBars(bool hide, HealthBars healthBar, StaminaBars staminaBar) {
    if (healthBar == MOBILE_ENEMY_HEALTH && staminaBar == MOBILE_ENEMY_STAMINA) {
        ui->mobileHealth->setHidden(hide);
        ui->mobileStamina->setHidden(hide);
        ui->mobileHealthLabel->setHidden(hide);
        ui->mobileStaminaLabel->setHidden(hide);
    } else if (healthBar == STATIC_ENEMY_HEALTH && staminaBar == STATIC_ENEMY_STAMINA) {
        ui->staticHealth->setHidden(hide);
        ui->staticStamina->setHidden(hide);
        ui->staticHealthLabel->setHidden(hide);
        ui->staticStaminaLabel->setHidden(hide);
    }

    //move the textEdit around if both enemy's bars dis/appear
    if (healthBar != PLAYER_HEALTH && staminaBar != PLAYER_STAMINA) {
        if (hide && !enemiesKilled && ui->mobileHealth->isHidden() && ui->staticHealth->isHidden()) {
            enemiesKilled = true;
            const QRect &geometry = ui->textEdit->geometry();
            ui->textEdit->setGeometry(geometry.x(), geometry.y() - 40, ui->textEdit->width(), ui->textEdit->height() + 40);
            emit game->getPlayer().gameWon();
        } else if (!hide && enemiesKilled) {
            enemiesKilled = false;
            const QRect &geometry = ui->textEdit->geometry();
            ui->textEdit->setGeometry(geometry.x(), geometry.y() + 40, ui->textEdit->width(), ui->textEdit->height() - 40);
        }
    }
}

void MainWindow::on_west_clicked() {
    Character &character = game->getPlayer();

    if (character.stamina >= character.moveStaminaCost && game->isPlayable()) {
        setStamina(character.stamina - character.moveStaminaCost, PLAYER_STAMINA);
        string g = game->go("west");

        ItemTimer *freeze = potionTimers["Freeze Enemies"];

        if (freeze) {
            if (!freeze->isActive()) {
                game->moveMobileEnemy();
            }
        } else {
            game->moveMobileEnemy();
        }

        if (g.compare("direction null") != 0) {
            attackTimer->stop(); //stop the attack cycle because you are moving away from the enemy(ies)
            visitRoom(g);
        } else {
            //you've hit a wall
            QString str = "You've crashed into a wall, you have lost health\n";
            ui->textEdit->append(str);
            setHealth(character.health - character.healthCost, PLAYER_HEALTH); //hitting a wall causes you to lose health
            if (!healthTimer->isActive()) healthTimer->start();
            character.checkStatus();
        }
    }
}

void MainWindow::on_east_clicked()
{
    Character &character = game->getPlayer();

    if (character.stamina >= character.moveStaminaCost && game->isPlayable()) {
        setStamina(character.stamina - character.moveStaminaCost, PLAYER_STAMINA);
        string g = game->go("east");

        ItemTimer *freeze = potionTimers["Freeze Enemies"];

        if (freeze) {
            if (!freeze->isActive()) {
                game->moveMobileEnemy();
            }
        } else {
            game->moveMobileEnemy();
        }

        if (g.compare("direction null") != 0) {
            attackTimer->stop(); //stop the attack cycle because you are moving away from the enemy(ies)
            visitRoom(g);
        } else {
            //you've hit a wall
            QString str = "You've crashed into a wall, you have lost health\n";
            ui->textEdit->append(str);
            setHealth(character.health - character.healthCost, PLAYER_HEALTH); //hitting a wall causes you to lose health
            if (!healthTimer->isActive()) healthTimer->start();
            character.checkStatus();
        }

        if (!staminaTimer->isActive()) staminaTimer->start(); //have the stamina increment every 10 seconds
    }
}

void MainWindow::on_north_clicked()
{
    Character &character = game->getPlayer();

    if (character.stamina >= character.moveStaminaCost && game->isPlayable()) {
        setStamina(character.stamina - character.moveStaminaCost, PLAYER_STAMINA);
        string g = game->go("north");

        ItemTimer *freeze = potionTimers["Freeze Enemies"];

        if (freeze) {
            if (!freeze->isActive()) {
                game->moveMobileEnemy();
            }
        } else {
            game->moveMobileEnemy();
        }

        if (g.compare("direction null") != 0) {
            attackTimer->stop(); //stop the attack cycle because you are moving away from the enemy(ies)
            visitRoom(g);
        } else {
            //you've hit a wall
            QString str = "You've crashed into a wall, you have lost health\n";
            ui->textEdit->append(str);
            setHealth(character.health - character.healthCost, PLAYER_HEALTH); //hitting a wall causes you to lose health
            if (!healthTimer->isActive()) healthTimer->start();
            character.checkStatus();
        }

        if (!staminaTimer->isActive()) staminaTimer->start(); //have the stamina increment every 10 seconds
    }
}

void MainWindow::on_south_clicked()
{
    Character &character = game->getPlayer();

    if (character.stamina >= character.moveStaminaCost && game->isPlayable()) {
        setStamina(character.stamina - character.moveStaminaCost, PLAYER_STAMINA);
        string g = game->go("south");

        ItemTimer *freeze = potionTimers["Freeze Enemies"];

        if (freeze) {
            if (!freeze->isActive()) {
                game->moveMobileEnemy();
            }
        } else {
            game->moveMobileEnemy();
        }

        if (g.compare("direction null") != 0) {
            attackTimer->stop(); //stop the attack cycle because you are moving away from the enemy(ies)
            visitRoom(g);
        } else {
            //you've hit a wall
            QString str = "You've crashed into a wall, you have lost health\n";
            ui->textEdit->append(str);
            setHealth(character.health - character.healthCost, PLAYER_HEALTH); //hitting a wall causes you to lose health
            if (!healthTimer->isActive()) healthTimer->start();
            character.checkStatus();
        }

        if (!staminaTimer->isActive()) staminaTimer->start(); //have the stamina increment every 10 seconds
    }
}

void MainWindow::on_teleport_clicked()
{
    Character &character = game->getPlayer();

    if (character.stamina >= character.teleportStaminaCost && game->isPlayable()) {
        setStamina(character.stamina - character.moveStaminaCost, PLAYER_STAMINA);
        string t = game->goToRandomRoom();

        ItemTimer *freeze = potionTimers["Freeze Enemies"];

        if (freeze) {
            if (!freeze->isActive()) {
                game->moveMobileEnemy();
            }
        } else {
            game->moveMobileEnemy();
        }

        attackTimer->stop(); //stop the attack cycle because you are moving away from the enemy(ies)
        visitRoom(t, true); //tell visitRoom you are teleporting into it, i.e you can "teleport" through a door even if locked

        if (!staminaTimer->isActive()) staminaTimer->start(); //have the stamina increment every 10 seconds
    }
}

void MainWindow::on_gameWon() {
    //game->setPlayable(false); //maybe best to allow the player to explore
    ui->textEdit->append("You have won the game, congratulations!\n");
    setPoints(game->getPlayer().getPoints() + 1000);
}

void MainWindow::on_gameLost() {
    stopTimers();
    game->setPlayable(false);
    ui->textEdit->append("You have lost the game, better luck next time!\n");
    ui->comboBox->clear();
}

void MainWindow::on_EnemyDeath(Enemy &enemy) {
    QString deathMessage = "";
    deathMessage += "You have killed ";
    deathMessage += enemy.getName().c_str();
    deathMessage += ", congratulations!\n";
    ui->textEdit->append(deathMessage);

    Character &player = game->getPlayer();

    if (enemy.getName() == "Grue") {
        setExperience(player.getExperience() + 15);
        setPoints(player.getPoints() + 100);
    } else {
        setExperience(player.getExperience() + 25);
        setPoints(player.getPoints() + 125);
    }

    enemy.setCurrentRoom(NULL); //remove them from the game
}

void MainWindow::on_WeaponDestroyed(Weapon *weapon) {
    QString destroyed;
    destroyed += "Weapon: ";
    destroyed += weapon->getShortDescription().c_str();
    destroyed += " destroyed\n";
    ui->textEdit->append(destroyed);

    Character &player = game->getPlayer();

    if (weapon == player.getWeaponInHand()) {
        player.setWeaponInHand(NULL);
    }

    inventory->removeItem(weapon);
    game->itemManager.removeItem(weapon);
}

//Used for handling what happens when the stamina timer times out
void MainWindow::on_StaminaTimeOut() {
    Character &character = game->getPlayer();
    if (game->isPlayable()) {
        if (character.stamina != 100) setStamina(character.stamina + 20, PLAYER_STAMINA);
    }
}

//Used for handling what happens when the health timer times out
void MainWindow::on_HealthTimeOut() {
    Character &character = game->getPlayer();
    if (game->isPlayable()) {
        if (character.health != 100) setHealth(character.health + character.healthCost, PLAYER_HEALTH);
    }
}

bool MainWindow::isEnemyInPlayerRoom(Enemy &enemy) {
    Room *currentRoom = game->getPlayer().getCurrentRoom();
    return enemy.getCurrentRoom() == currentRoom;
}

void MainWindow::attackPlayer() {
    Enemy &staticEnemy = game->getEnemy(true);
    Enemy &mobileEnemy = game->getEnemy(false);
    ItemTimer *freezeTimer = potionTimers["Freeze Enemies"];
    bool frozen = false;

    if (freezeTimer) {
        frozen = freezeTimer->isActive();
    }

    if (isEnemyInPlayerRoom(staticEnemy) && !frozen)
        game->attackPlayer(staticEnemy);

    if (isEnemyInPlayerRoom(mobileEnemy) && !frozen)
        game->attackPlayer(mobileEnemy);

    setHealth(game->getPlayer().getHealth(), PLAYER_HEALTH);
    if (!healthTimer->isActive()) healthTimer->start();
}

void MainWindow::on_AttackTimeOut() {
    if (!inBattle) {
        attackPlayer();

        Enemy &mobileEnemy = game->getEnemy(false);
        Enemy &staticEnemy = game->getEnemy(true);

        if (isEnemyInPlayerRoom(mobileEnemy))
            setStamina(mobileEnemy.getStamina() - mobileEnemy.getStaminaAttackCost(), MOBILE_ENEMY_STAMINA);

        if (isEnemyInPlayerRoom(staticEnemy))
            setStamina(staticEnemy.getStamina() - staticEnemy.getStaminaAttackCost(), STATIC_ENEMY_STAMINA);
    }
}

//Used for handling what happens when an item times out
void MainWindow::on_ItemTimeOut(ItemTimerUnit timerItem) {
    Item *item = timerItem.item;
    string message = timerItem.timeOutMessage;
    Character &character = game->getPlayer();
    character.removeItem(item);
    inventory->removeUnlockedItem(item);
    inventory->removeLockedItem(item);
    Potion *potion = Item::cast<Potion*>(item);

    string description = item->getShortDescription();
    bool light = description == "Lantern" || description == "Candle";

    if (potion) {
        if (potion->getType() == STAMINA_POTION) character.moveStaminaCost -= potion->getPotionBonus();
        if (potion->getType() == SPECIAL_EFFECT) {
            if (description == "Freeze Enemies") {
                if (isEnemyInPlayerRoom(game->getEnemy(true)) || isEnemyInPlayerRoom(game->getEnemy(false))) {
                    attackTimer->start();
                }
            }
        }
    }

    if (message != "") {
        QString msg;
        msg += message.c_str();
        msg += "\n";
        ui->textEdit->append(msg);
    }

    game->itemManager.removeItem(item, light); //if its a light source just remove from room to respawn
    if (light) game->itemManager.respawnItem(item);
}

void MainWindow::on_help_clicked() {
    help->show();
}

void MainWindow::processItem(Item *item) {
    Boost* boost = Item::cast<Boost*>(item);
    Potion* potion = Item::cast<Potion*>(item); // 3. Templates
    Weapon* weapon = Item::cast<Weapon*>(item);

    if (!boost && !potion) {
        Character &character = game->getPlayer();
        string description = item->getShortDescription();

        if (description == "Key") {
            character.setKey(true);
        } else if (description == "Emerald") {
            emeraldTimer->start();
        } else if (description == "Candle") {
            ItemTimer *torchTimer = new ItemTimer(item, this, &MainWindow::on_ItemTimeOut);
            string description = "Candle has timed out";
            torchTimer->setTimeOutMessage(description);
            torchTimer->setSingleShot(true);
            torchTimer->setInterval(10000);
            lightTimers["Candle"] = torchTimer;
            torchTimer->start();
        } else if (description == "Lantern") {
            ItemTimer *lanternTimer = new ItemTimer(item, this, &MainWindow::on_ItemTimeOut);
            string description = "Lantern has timed out";
            lanternTimer->setTimeOutMessage(description);
            lanternTimer->setSingleShot(true);
            lanternTimer->setInterval(10000);
            lightTimers["Lantern"] = lanternTimer;
            lanternTimer->start();
        }

        if (item->getXpValue() != 0 && !item->isDropped())
            setExperience(character.experience + item->getXpValue());

        QString timeout = getTimedItemDescription(item).c_str();
        timeout += "\n";
        ui->textEdit->append(timeout);

        ui->comboBox->removeItem(ui->comboBox->findText(description.c_str()));
        ui->comboBox->setCurrentIndex(0);

        if (description == "Lucky Charm") {
            ui->textEdit->append("You have picked up the lucky charm!");

            ItemManager &manager = game->itemManager;
            ObjectContainer<Item*> unlockedItems = manager.spawnedItems(false);
            if (unlockedItems.size() != 0 || (unlockedItems.size() != 1 && unlockedItems[0]->getShortDescription() != "Lucky Charm")) {
                Item* luckyItem = unlockedItems.getRandomObject();
                manager.respawnItem(luckyItem);

                QString text("The lucky charm spawned:\n");
                text += luckyItem->getLongDescription().c_str(); // 10. Dynamic Dispatch
                text += "\n";
                ui->textEdit->append(text);
                inventory->addUnlockedItem(luckyItem);
                character.addItem(luckyItem);
                ui->comboBox->removeItem(ui->comboBox->findText(luckyItem->getShortDescription().c_str()));
                processItem(luckyItem);
            } else {
                ui->textEdit->append("You have picked up every unlocked item, so the Lucky Charm could not find anything\n");
            }
        }
    }

    if (potion) {
        ItemTimer *timer = new ItemTimer(potion, this, &MainWindow::on_ItemTimeOut);
        string description = potion->getShortDescription();
        string message;
        if (potion->getType() != SPECIAL_EFFECT) {
            message = "Potion: " + description + " has timed out";
        } else {
            if (description == "Freeze Enemies") {
                message = "Enemies are no longer frozen";
            } else if (description == "Weaken Enemies") {
                message = "Enemies have full strength again";
            }
        }
        timer->setTimeOutMessage(message);
        timer->setSingleShot(true);
        potionTimers[description] = timer;
    }

    if (weapon && !weapon->isAmmoItem()) {
        game->player.setWeaponInHand(weapon);
        string text = "You are now fighting with " + weapon->getShortDescription();
        updateWeapon(text);
    }
}

//the game crashed when you picked up an item in J
void MainWindow::on_comboBox_activated(const QString &arg1) {
    Character &character = game->getPlayer();
    ItemManager &itemManager = game->itemManager;

    Room *r = game->getPlayer().getCurrentRoom();
    string objectName = arg1.toStdString();
    if (objectName == "") return;
    Item* item = r->getItem(objectName);
    string description = item->getLongDescription();
    itemManager.removeItem(item, true); //only remove item from room, not manager

    character.addItem(item);
    itemManager.pickUpItem(item); //mark the item as picked up

    processItem(item);

    inventory->addUnlockedItem(item);

    ui->comboBox->removeItem(ui->comboBox->findText(arg1));
    ui->comboBox->setCurrentIndex(0);

    if (item->getShortDescription() != "Lucky Charm") {
        QString text(description.c_str());
        if (item->isDropped()) text += "\nYou already collected this item before, so points and XP will not be added";
        text += "\n";
        ui->textEdit->append(text);
    }

    setHealth(character.health, PLAYER_HEALTH);
    setStamina(character.stamina, PLAYER_STAMINA);
    setPoints(character.points);
}

void MainWindow::on_inventory_clicked() {
    inventory->show();
}

void MainWindow::closeEvent(QCloseEvent*) {
    inventory->close();
    help->close();
}

void MainWindow::updateWeapon(string weapon) {
    QString txt(weapon.c_str());
    txt += "\n";
    ui->textEdit->append(txt);
}

void MainWindow::on_map_clicked()
{
    QString map(game->map().c_str());
    map += "\n";
    ui->textEdit->append(map);
}

void MainWindow::stopTimers() {
    healthTimer->stop();
    emeraldTimer->stop();
    attackTimer->stop();

    for (map<string, ItemTimer*>::iterator it = potionTimers.begin(); it != potionTimers.end(); it++) {
        if (it->second) it->second->stop();
    }

    for (map<string, ItemTimer*>::iterator it = lightTimers.begin(); it != lightTimers.end(); it++) {
        if (it->second) it->second->stop();
    }
}

string MainWindow::getTimedItemDescription(Item *item) {
    int timeOutTime = 0;
    string description = item->getShortDescription();
    ItemTimer *timer = potionTimers[description];

    if (timer == NULL) {
        timer = lightTimers[description];
    }

    if (timer) {
        timeOutTime = timer->getInterval() / 1000;
    } else {
        Potion *potion = Item::cast<Potion*>(item);

        if (potion) {
            timeOutTime = potion->getTimeOutTime() / 1000;
        }
    }

    string ret = "Item " + item->getShortDescription();

    if (timeOutTime == 0) {
        ret += " does not time out";
    } else {
        ret += " will timeout in " + to_string(timeOutTime) + " seconds";
    }

    return ret;
}

void MainWindow::on_attack_clicked() {
    if (!game->isPlayable()) return;
    Character &player = game->getPlayer();
    Enemy &staticEnemy = game->getEnemy(true);
    Enemy &mobileEnemy = game->getEnemy(false);
    static bool curseApplied = false;
    inBattle = true;

    bool staticInRoom = isEnemyInPlayerRoom(staticEnemy);
    bool mobileInRoom = isEnemyInPlayerRoom(mobileEnemy);

    if (mobileInRoom || staticInRoom)
        ui->textEdit->append("BATTLE IN PROGRESS\n");

    string message = game->attack();

    if (message != "") {
        QString msg(message.c_str());
        msg += "\n";
        ui->textEdit->append(msg);

        addRoomItemsToComboBox(); //if the weapon was thrown out of player's hand show it in the combobox
    }

    attackPlayer();

    if (staticInRoom) {
        Potion *potion = staticEnemy.getPotion();

        if (potion && !curseApplied) {
            curseApplied = true;
            processItem(potion);
            player.setMoveStaminaCost(player.getMoveStaminaCost() + potion->getPotionBonus());
            ItemTimer *timer = potionTimers[potion->getShortDescription()];
            if (timer) {
                timer->start(potion->getTimeOutTime());
                timer->setTimeOutMessage("You are no longer cursed");
                QString cursed("Grue has cursed you for 10 seconds, making you use more stamina as you move\n");
                ui->textEdit->append(cursed);
            }
        }

        setHealth(staticEnemy.getHealth(), STATIC_ENEMY_HEALTH);
        setStamina(staticEnemy.getStamina(), STATIC_ENEMY_STAMINA);

        if (staticEnemy.getHealth() == 0) {
            hideEnemyBars(true, STATIC_ENEMY_HEALTH, STATIC_ENEMY_STAMINA);
            if (!mobileInRoom || mobileEnemy.getHealth() == 0) {
                attackTimer->stop();
            }

            on_EnemyDeath(staticEnemy);
        }
    }

    if (mobileInRoom) {
        setHealth(mobileEnemy.getHealth(), MOBILE_ENEMY_HEALTH);
        setStamina(mobileEnemy.getStamina(), MOBILE_ENEMY_STAMINA);

        if (mobileEnemy.getHealth() == 0) {
            hideEnemyBars(true, MOBILE_ENEMY_HEALTH, MOBILE_ENEMY_STAMINA);
            if (!staticInRoom || staticEnemy.getHealth() == 0) {
                attackTimer->stop();
            }

            on_EnemyDeath(mobileEnemy);
        }
    }

    Weapon *weapon = player.getWeaponInHand();

    if (weapon) {
        if (weapon->getDamage() == 0) {
            on_WeaponDestroyed(weapon);
        }
    }

    setStamina(player.getStamina(), PLAYER_STAMINA);
}

void MainWindow::on_restart_clicked() {
    this->reset();
}
