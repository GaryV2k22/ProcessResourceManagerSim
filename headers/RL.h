#pragma once

class RL {

private:
    std::vector<std::queue<int>> levels; // Array of queues for each priority level

public:
    void insert(int priority, int pid);
    int removeHighestPriorityProcess();

};
