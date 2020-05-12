#ifndef INVENTORYWINDOW_H
#define INVENTORYWINDOW_H

#include <QWidget>
#include <string>
#include "objectcontainer.h"
#include "item.h"
using namespace std;

namespace Ui {
class InventoryWindow;
}

class ZorkUL;
class MainWindow;

class InventoryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit InventoryWindow(ZorkUL *game, MainWindow *window, QWidget *parent = nullptr);
    // 1. Destructors
    ~InventoryWindow();
    void addUnlockedItem(Item* item);
    void removeUnlockedItem(Item* item);
    void removeLockedItem(Item* item);
    void removeItem(Item* item); //removes item from inventory window altogether, ie both locked and unlocked
    void update();
    void reset();

private slots:
    void on_drop_clicked();

    void on_use_clicked();

    void on_apply_clicked();

    void on_comboBox_activated(const QString &arg1);

private:
    Ui::InventoryWindow *ui;
    ZorkUL *game;
    MainWindow *window;
    ObjectContainer<Item*> itemsLocked;
    ObjectContainer<Item*> itemsUnlocked;
    Item* getItem(string itemName, bool remove); //if remove is true, it is removed from the itemsDisplayed vector
};

#endif // INVENTORYWINDOW_H
