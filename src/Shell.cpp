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
            std::cout << "* process " << new_pid << " created\n";
        }
    } else if (cmd == "de") {
        int pid;
        if (!(iss >> pid)) {
            std::cout << "* Error: Process ID must be specified\n";
        } else {
            int count = os_manager.destroy(pid);
            std::cout << "* " << count << " process(es) destroyed\n";
        }
    } else if (cmd == "rq") {
        int rid, units;
        if (!(iss >> rid >> units)) {
            std::cout << "* Error: Resource ID and units must be specified\n";
        } else {
            bool success = os_manager.request(os_manager.getCurrentProcess(), rid, units);
            if (success) {
                std::cout << "* resource " << rid << " allocated\n";
            } else {
                std::cout << "* error\n";
            }
        }
    } else if (cmd == "rl") {
        int rid, units;
        if (!(iss >> rid >> units)) {
            std::cout << "* Error: Resource ID and units must be specified\n";
        } else {
            bool success = os_manager.release( os_manager.getCurrentProcess() ,rid, units);
            if (success) {
                std::cout << "* resource " << rid << " released\n";
            } else {
                std::cout << "* error\n";
            }
        }
    } else if (cmd == "to") {
        os_manager.timeout();
        std::cout << "* timeout\n";
    } else if (cmd == "in") {
        os_manager.init_default();
        std::cout << "* system initialized\n";
    } else {
        std::cout << "* Unknown command\n";
    }
}
