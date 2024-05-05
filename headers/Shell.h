#pragma once

#include <string>
#include <iostream>
#include <sstream>


#include "Manager.h"


class Shell {
public:
    void run();
private:
    Manager os_manager; // Instance of the operating system manager
    void parseCommand(const std::string& command);
};
