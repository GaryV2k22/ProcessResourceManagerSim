#pragma once

struct RCB {
    int rid; // Resource ID
    int inventory; // Total units of the resource
    int available; // Available units of the resource
    std::list<std::pair<int, int>> waitList; // List of pairs (process index, units requested)
};
