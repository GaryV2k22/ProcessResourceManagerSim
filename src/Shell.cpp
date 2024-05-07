#include "Shell.h"



void Shell::run() {
    std::string command;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, command) || command == "exit") {
            break;
        }
        parseCommand(command);
    }
}

void Shell::parseCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "cr") {
        int priority;
        if (!(iss >> priority)) {
            std::cout << "* Error: Priority must be specified\n";
        } else {
            int new_pid = os_manager.create(priority);
            if (new_pid >= 0) {
                std::cout << os_manager.getCurrentProcess() << " ";
            } else {
                std::cout << "-1 ";
            }
        }
    } else if (cmd == "de") {
        int pid;
        if (!(iss >> pid)) {
            std::cout << "* Error: Process ID must be specified\n";
        } else {
            if (os_manager.destroy(pid, os_manager.getCurrentProcess())) {
                std::cout << "* Process " << pid << " and its descendants destroyed\n";
            } else {
                std::cout << "-1 ";
            }
        }
    } else if (cmd == "rq") {
        int rid, units;
        if (!(iss >> rid >> units)) {
            std::cout << "* Error: Resource ID and units must be specified\n";
        } else {
            if (os_manager.request(os_manager.getCurrentProcess(), rid, units)) {
                std::cout << os_manager.getCurrentProcess() << " ";
            } else {
                std::cout << "-1 ";
            }
        }
    } else if (cmd == "rl") {
        int rid, units;
        if (!(iss >> rid >> units)) {
            std::cout << "* Error: Resource ID and units must be specified\n";
        } else {
            if (os_manager.release(os_manager.getCurrentProcess(), rid, units)) {
                std::cout << os_manager.getCurrentProcess() << " ";
            } else {
                std::cout << "-1 ";
            }
        }
    } else if (cmd == "to") {
        os_manager.timeout();
        std::cout << "* Timeout triggered\n";
    } else if (cmd == "in") {
        int n, u0, u1, u2, u3;
        if (!(iss >> n >> u0 >> u1 >> u2 >> u3)) {
            std::cout << "* Error: Initialization parameters must be specified\n";
        } else {
            if (os_manager.init(n, u0, u1, u2, u3)) {
                std::cout << os_manager.getCurrentProcess() << " ";
            } else {
                std::cout << "-1 ";
            }
        }
    } else if (cmd == "id") {
        os_manager.init_default();
        std::cout << "* System initialized to default state\n";
    } else {
        std::cout << "* Unknown command\n";
    }

    // Always display the current process after processing the command
    std::cout << "Current process ID: " << os_manager.getCurrentProcess() << std::endl;
}
