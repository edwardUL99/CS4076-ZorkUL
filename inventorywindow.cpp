#include "inventorywindow.h"
#include "ui_inventorywindow.h"
#include "mainwindow.h"
#include "weapon.h"
#include "boost.h"
#include "potion.h"

InventoryWindow::InventoryWindow(ZorkUL *game, MainWindow *window, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InventoryWindow)
{
    ui->setupUi(this);
    ui->unlockedEdit->setReadOnly(true);
    ui->lockedEdit->setReadOnly(true);
    setWindowTitle("Inventory");
    setFixedSize(size());
    this->game = game;
    this->window = window;
    ui->use->setHidden(true);
    ui->apply->setHidden(true);

    ItemManager &itemManager = game->itemManager;
    for (auto it = itemManager.itemsIterator(true); it != itemManager.itemsIterator(false); it++) {
        if (itemManager.showInInventory(*it)) itemsLocked + *it;
    }

    update();
}

InventoryWindow::~InventoryWindow()
{
    delete ui;
}

void InventoryWindow::reset() {
    ui->unlockedEdit->clear();
    ItemManager &itemManager = game->itemManager;

    for (auto it = itemManager.itemsIterator(true); it != itemManager.itemsIterator(false); it++) {
        if (itemManager.showInInventory(*it)) itemsLocked + *it;
    }
    itemsUnlocked.clear();
}

void InventoryWindow::addUnlockedItem(Item *item) {
    if (!itemsUnlocked.contains(item) && game->itemManager.showInInventory(item)) {
        itemsUnlocked + item;
        this->removeLockedItem(item);
        this->update();
    }
}

void InventoryWindow::removeUnlockedItem(Item *item) {
    itemsUnlocked - item;;

    this->update();
}

void InventoryWindow::removeLockedItem(Item *item) {
    itemsLocked - item;

    this->update();
}

void InventoryWindow::removeItem(Item *item) {
    this->removeUnlockedItem(item);
    this->removeLockedItem(item);
}

void InventoryWindow::update() {
    ItemManager &manager = window->game->itemManager;
    ui->apply->setHidden(true);
    ui->use->setHidden(true);
    ui->unlockedEdit->clear();
    ui->lockedEdit->clear();
    ui->comboBox->clear();
    ui->comboBox->addItem("");

    for (ObjectContainer<Item*>::iterator pos = itemsLocked.begin(); pos != itemsLocked.end(); pos++) {
        if (manager.showInInventory(*pos)) {
            QString description(manager.getItemLevelDescription(*pos).c_str());
            description += "\n";
            ui->lockedEdit->append(description);
        }
    }

    for (ObjectContainer<Item*>::iterator pos = itemsUnlocked.begin(); pos != itemsUnlocked.end(); pos++) {
        if (manager.showInInventory(*pos)) {
            QString description(manager.getItemLevelDescription((*pos)).c_str());
            description += "\n";
            ui->unlockedEdit->append(description);
            ui->comboBox->addItem((*pos)->getShortDescription().c_str());
        }
    }

    ui->lockedEdit->moveCursor(QTextCursor::Start);
}

Item* InventoryWindow::getItem(string itemName, bool remove) {
    Item *item = NULL;
    for (ObjectContainer<Item*>::iterator it = itemsUnlocked.begin(); it != itemsUnlocked.end(); it++) {
        if ((*it)->getShortDescription() == itemName) {
            item = *it;
            if (remove) itemsUnlocked - item;
            return item;
        }
    }
    return item;
}

void InventoryWindow::on_drop_clicked()
{
    Character &player = game->getPlayer();
    ItemManager &itemManager = game->itemManager;
    string itemText = ui->comboBox->currentText().toStdString();
    if (itemText != "") {
        Item *item = getItem(itemText, true);

        if (item != NULL) {
            itemManager.dropItem(item); //mark the item as dropped

            Weapon *weapon = Item::cast<Weapon*>(item);
            if (weapon) {
                if (weapon == player.getWeaponInHand()) {
                    player.setWeaponInHand(NULL);
                    window->updateWeapon("You are now fighting with: Punch");
                }
            }
            itemManager.spawnItem(item, player.getCurrentRoom());
            itemsLocked + item;

            string description = item->getShortDescription();
            if (description == "Key") {
                player.setKey(false);
            } else if (description == "Torch" || description == "Lantern") {
                window->lightTimers.at(description)->stop();
            }

            ui->comboBox->removeItem(ui->comboBox->currentIndex());
            player.removeItem(item);
            item->setDropped(true);
            window->visitRoom(player.getCurrentRoom()->getName());
            update();
        }
    } else {
        QString error("You must choose an item to drop in the drop-down box below\n");
        ui->unlockedEdit->append(error);
    }
    ui->comboBox->setCurrentIndex(0);
}

void InventoryWindow::on_use_clicked()
{
    Character &player = game->getPlayer();
    string weaponText = ui->comboBox->currentText().toStdString();
    Item *item = getItem(weaponText, false);

    if (item != NULL) {
        Weapon* weapon = Item::cast<Weapon*>(item);
        if (weapon) {
            if (!weapon->isAmmoItem()) {
                player.setWeaponInHand(weapon);
                QString success;
                success += "You can now fight with ";
                success += weapon->getShortDescription().c_str();
                success + " \n";
                ui->unlockedEdit->append(success);
                string weaponTxt = "You are now fighting with: " + weapon->getShortDescription();
                window->updateWeapon(weaponTxt);
            } else {
                QString str("You cannot select this as it is an ammo item\n"); //bug here was trying to select gun but it was saying
                ui->unlockedEdit->append(str);
            }
        } else {
            QString error("You can only select a Weapon item\n");
            ui->unlockedEdit->append(error);
        }
    } else {
        player.setWeaponInHand(NULL);
        window->updateWeapon("You are now fighting with: Punch");
    }
    ui->comboBox->setCurrentIndex(0);
    ui->use->hide();
}

void InventoryWindow::on_apply_clicked() {
    string boostText = ui->comboBox->currentText().toStdString();
    Item *item = getItem(boostText, true); // 10. Static Dispatch

    if (item != NULL) {
        Boost *boost = Item::cast<Boost*>(item);
        Potion *potion = Item::cast<Potion*>(item);

        if (boost) {
            boost->apply();  // 10. Dynamic dispatch
        } else {
            string description = potion->getShortDescription();
            if (potion->getType() == STAMINA_POTION) {
                Character &player = window->game->getPlayer();
                player.setMoveStaminaCost(player.getMoveStaminaCost() + potion->getPotionBonus());
                window->potionTimers.at(description)->start(potion->getTimeOutTime());
                window->addText(window->getTimedItemDescription(potion));
            } else if (potion->getType() == SPECIAL_EFFECT) {
                if (description == "Freeze Enemies") {
                    window->attackTimer->stop();
                    string frozen("The enemies have been frozen for 10 seconds!\n");
                    window->addText(frozen);
                } else if (description == "Weaken Enemies") {
                    string weakened("The enemies are 20% weaker for 10 seconds!\n");
                    window->addText(weakened);
                }
                window->potionTimers.at(description)->start(potion->getTimeOutTime());
            }
        }
    }

    Character &player = window->game->getPlayer();
    window->setPoints(player.getPoints());
    window->setStamina(player.getStamina(), window->PLAYER_STAMINA);
    window->setHealth(player.getHealth(), window->PLAYER_HEALTH);
    window->setExperience(player.getExperience());
    ui->comboBox->setCurrentIndex(0);
    update();
}

void InventoryWindow::on_comboBox_activated(const QString &arg1) {
    string text = arg1.toStdString();
    Item *item = getItem(text, false);

    Boost *boost = Item::cast<Boost*>(item);
    Potion *potion = Item::cast<Potion*>(item);
    Weapon *weapon = Item::cast<Weapon*>(item);

    if (boost || potion) {
        ui->apply->setHidden(false);
        ui->use->setHidden(true);
    } else if (weapon) {
        ui->apply->setHidden(true);
        ui->use->setHidden(false);
    } else {
        ui->apply->setHidden(true);
        ui->use->setHidden(true);
    }
}
