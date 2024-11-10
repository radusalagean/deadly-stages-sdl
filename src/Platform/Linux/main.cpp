#include "../../Game.hpp"
#include "../Common/SaveDataHelper.hpp"

#include <iostream>
#include <unistd.h>
#include <libgen.h> // for dirname
#include <limits.h> // for PATH_MAX
#include <sys/stat.h>

#include "../../Core/Macros.hpp"

std::string resDirPathPrefix = "";
std::string userDirPathPrefix = std::string(getenv("HOME")) + "/DeadlyStagesRemix-UserData";

int setWorkingDirectory()
{
    // Buffer to store the path of the executable
    char executablePath[PATH_MAX];

    // Get the path of the executable
    if (readlink("/proc/self/exe", executablePath, sizeof(executablePath)) == -1) {
        std::cerr << "Error: Unable to get executable path\n";
        return 1;
    }

    // Get the directory containing the executable
    char* directoryPath = dirname(executablePath);

    // Change the working directory
    if (chdir(directoryPath) != 0) {
        std::cerr << "Error: Unable to change working directory\n";
        return 1;
    }

    // Now the working directory has been changed
    std::cout << "Working directory changed to: " << directoryPath << std::endl;
    
    return 0;
}

int createUserDataDirectory()
{
    std::string userDataDir = UPATH("");
    std::cout << "Creating user data directory: " << userDataDir << std::endl;
    return mkdir(userDataDir.c_str(), 0755);
}

int main(int argc, char* argv[])
{
    if (setWorkingDirectory() != 0)
        return 1;

    if (createUserDataDirectory() != 0)
    {
        std::cerr << "Warning: Unable to create user data directory" << std::endl;
    }

    Game::init();
    while (Game::isRunning)
    {
        Game::loop();
    }
    Game::dispose();

    return 0;
}