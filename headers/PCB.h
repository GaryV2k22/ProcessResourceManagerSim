#pragma once

#include <vector>


struct PCB {
    int pid;
    int priority;
    int state; // 1 = ready, 0 = blocked, 2 = destroyed -1 = not initialized
    //resources.first is the resource ID, resources.second is the units held
    std::vector<int> resources; // Resource ID and units held
    std::vector<int> children;
    int parent;

    // Default constructor with -1 as default values
    PCB() : pid(-1), priority(-1), state(-1), parent(-1), resources({0,0,0,0}), children({}) {}

    // Parameterized constructor
    PCB(int pid, int priority, int state, int parent) : pid(pid), priority(priority), state(state), parent(parent), resources({0,0,0,0}), children({}) {}
};
