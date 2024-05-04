#pragma once

struct PCB {
    int pid;
    int priority;
    int state;
    std::vector<std::pair<int, int>> resources; // Resource ID and units held
    std::vector<int> children;
    int parent;
};
