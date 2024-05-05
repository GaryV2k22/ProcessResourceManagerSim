#pragma once

#include <vector>
#include <queue>


class RL {

private:
    std::vector<std::queue<int>> levels; // Array of queues for each priority level

public:
    // Constructor that initializes the levels vector with n queues
    RL(int n) : levels(n) {}

    void insert(int priority, int pid);
    int removeHighestPriorityProcess();
    int getHighestPriorityProcess();
    void setLevels(int n);
    int getLevels();

};
