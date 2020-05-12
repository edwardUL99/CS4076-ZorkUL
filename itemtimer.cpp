#include "itemtimer.h"
#include "item.h"
#include <QTimer>
#include <QObject>

ItemTimer::ItemTimer(Item* item, MainWindow *ui, void (MainWindow::*timeOutFunction)(ItemTimerUnit)) {
    this->item = item;
    this->timeOutFunction = timeOutFunction;
    this->timer = new QTimer();
    this->ui = ui;
    this->timeOutMessage = "";
    connect(timer, &QTimer::timeout, this, &ItemTimer::onTimeOut);
}

ItemTimer::~ItemTimer() {
    this->timer->stop();
    delete timer;
}

void ItemTimer::setTimeOutFunction(void (MainWindow::*timeOutFunction)(ItemTimerUnit)) {
    this->timeOutFunction = timeOutFunction;
}

void ItemTimer::start() {
    this->start(timer->interval());
}

void ItemTimer::start(int milliseconds) {
    this->timer->start(milliseconds);
    this->item->setTimed(true);
}

void ItemTimer::stop() {
    this->timer->stop();
    this->item->setTimed(false);
}

void ItemTimer::setInterval(int milliseconds) {
    this->timer->setInterval(milliseconds);
}

int ItemTimer::getInterval() const {
    return this->timer->interval();
}

void ItemTimer::setSingleShot(bool singleShot) {
    this->timer->setSingleShot(singleShot);
}

void ItemTimer::onTimeOut() {
    ItemTimerUnit itemUnit = {item, timeOutMessage};
    this->item->setTimed(false);
    (ui->*timeOutFunction)(itemUnit);
}

bool ItemTimer::isActive() const {
    return this->timer->isActive();
}

void ItemTimer::setTimeOutMessage(string timeOutMessage) {
    this->timeOutMessage = timeOutMessage;
}

string ItemTimer::getTimeOutMessage() const {
    return this->timeOutMessage;
}

Item* ItemTimer::getItem() const {
    return item;
}

void ItemTimer::setItem(Item *item) {
    this->item = item;
}
