#include "../../Game.hpp"

#include "../../Platform/Common/SaveDataHelper.hpp"

#include <iostream>
#include <unistd.h>
#include <libgen.h> // for dirname
#include <limits.h> // for PATH_MAX
#include <mach-o/dyld.h>
#include <sys/stat.h>
#include "../../Core/Macros.hpp"

#ifdef NDEBUG
    std::string resDirPathPrefix = "Resources/";
#else
    std::string resDirPathPrefix = "";
#endif

std::string userDirPathPrefix = std::string(getenv("HOME")) + "/DeadlyStagesRemix-UserData/";

int setWorkingDirectory()
{
    uint32_t bufSize = PATH_MAX;
    
    // Buffer to store the path of the executable
    char executablePath[bufSize];

    // Get the path of the executable
    if (_NSGetExecutablePath(executablePath, &bufSize) != 0) {
        // Buffer size is too small.
        return 1;
    }

    // Get the directory containing the executable
    char* executableDirectory = dirname(executablePath);

    #ifdef NDEBUG
        // Get the parent directory
        char* expectedWorkingDirectory = dirname(executableDirectory);
    #else
        char* expectedWorkingDirectory = executableDirectory;
    #endif

    // Change the working directory
    if (chdir(expectedWorkingDirectory) != 0) {
        std::cerr << "Error: Unable to change working directory\n";
        return 1;
    }

    // Now the working directory has been changed
    std::cout << "Working directory changed to: " << expectedWorkingDirectory << std::endl;
    
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