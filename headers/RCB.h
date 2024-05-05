#pragma once

#include <list>

struct RCB {
    int rid; // Resource ID
    int inventory; // Total units of the resource
    int available; // Available units of the resource
    //waitList.first is the process index, waitList.second is the units requested
    std::list<std::pair<int, int>> waitList; // List of pairs (process index, units requested)

    // Default constructor with -1 as default values
    RCB() : rid(-1), inventory(-1), available(-1) {}

    // Parameterized constructor
    RCB(int rid, int inventory) : rid(rid), inventory(inventory), available(inventory) {}
};
