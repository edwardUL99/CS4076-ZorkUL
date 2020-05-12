#include "mainwindow.h"
#include <time.h>

#include <QApplication>

//add comments in the locations of these examples too

/* EXAMPLES OF ASSIGNMENT REQUIREMENTS:
 * 1. Destructors: see every class
 * 2. Inheritance (including virtual methods) and cascading constructors:
 *      StaminaBoost inherits from Boost which inherits from Item, cascading the constructors
 * 3. Templates: ObjectContainer is a templated container which adds to the standard vector in terms of returning random objects
 *      Item::cast uses templates to determine what to cast the item to
 * 4. Unary operator overloading: Weapon has an overloaded postfix decrement operator which reduces it's strength
 *      ObjectContainer has a unary cast operator to cast it to a vector
 * 5. Binary operator overloading: ObjectContainer has overloaded + operator to add an item and - operator to remove an item
 * 6. Friends: MainWindow is "friends" with InventoryWindow
 * 7. Virtual functions and polymorphism: Character has a virtual function long description which varies if the obect is a character or enemy
 * 8. Abstract classes and pure virtual functions: Item is an abstract class as it has a pure virtual function getLongDescription()
 *      Boost is an abstract class as it has a pure virtual function apply()
 * 9. Initializer list: seen after the constructors in the cpp files of Boost, Stamina/Point/Experience/Health boosts, Tool, Weapon
 * 10. Dynamic dispatch: calling Boost's apply method e.g in inventory window when apply button is clicked
 *     Static dispatch: calling any non-virtual function such as ItemManager's addItem method
 */

int main(int argc, char *argv[])
{
    srand(time(0));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle("ZorkUL");
    w.setFixedSize(w.size());
    return a.exec();
}
