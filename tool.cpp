#include "tool.h"
#include <sstream>

                                                                       // 9. Initialiser list
Tool::Tool(string description, int value, int xpValue) : Item(description, value, xpValue) {
}

string Tool::getLongDescription() {
    stringstream ret;
    ret << "Tool: " << description << ", Value: " << value;
    return ret.str();
}
