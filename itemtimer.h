#ifndef ITEMTIMER_H
#define ITEMTIMER_H
#include <QObject>
#include <string>
using namespace std;

class Item;
class QTimer;
class MainWindow;

struct ItemTimerUnit {
    Item *item;
    string timeOutMessage;
};

/**
 * @brief The ItemTimer class
 * This class encapsulates a QTimer which can be used to determine what happens to a certain item after the timer runs out
 * More convenient as it allows you define a single slot for the timeout of this and use it for many different objects.
 * Take for example, if you were to use a QTimer for timing a boost running out:
 *      You could connect the timeout signal to a on_BoostTimeOut() slot in the implementing class
 * Then you wanted to time out a weapon using a QTimer and connect the timeout signal for the separate QTimer for this weapon to a
 * on_WeaponTimeOut() slot.
 *
 * With this class, you could have a:
 *      on_ItemTimeOut(Item* item) method which deals with the timeout of all different objects and pass this method to this timer on creation
 * Offers more flexibility
 */
class ItemTimer : public QObject {
private:
    QTimer *timer;
    Item *item;
    MainWindow *ui;
    void (MainWindow::*timeOutFunction)(ItemTimerUnit);
    void onTimeOut();
    string timeOutMessage;

public:
    ItemTimer(Item* item, MainWindow *ui, void (MainWindow::*timeOutFunction)(ItemTimerUnit) = nullptr);
    //1. Destructors
    ~ItemTimer();
    void setTimeOutFunction(void (MainWindow::*timeOutFunction)(ItemTimerUnit));
    void start();
    void start(int milliseconds);
    void stop();
    void setInterval(int milliseconds);
    int getInterval() const;
    void setSingleShot(bool singleShot);
    void setTimeOutMessage(string timeOutMessage);
    bool isActive() const;
    string getTimeOutMessage() const;
    Item* getItem() const;
    void setItem(Item* item);
};

#endif // ITEMTIMER_H
