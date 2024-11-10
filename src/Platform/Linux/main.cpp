#include "../../Game.hpp"
#include "../Common/SaveDataHelper.hpp"

#include <iostream>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <sys/stat.h>
#include <filesystem>
#include "../../Core/Macros.hpp"

std::string resDirPathPrefix = "";
std::string userDirPathPrefix = "";

int setWorkingDirectory()
{
    // Buffer to store the path of the executable
    char executablePath[PATH_MAX];

    // Get the path of the executable
    if (readlink("/proc/self/exe", executablePath, sizeof(executablePath)) == -1) 
    {
        std::cerr << "Error: Unable to get executable path\n";
        return 1;
    }

    // Get the directory containing the executable
    char* directoryPath = dirname(executablePath);

    // Change the working directory
    if (chdir(directoryPath) != 0) 
    {
        std::cerr << "Error: Unable to change working directory\n";
        return 1;
    }

    // Now the working directory has been changed
    std::cout << "Working directory changed to: " << directoryPath << std::endl;
    
    return 0;
}

std::string generateUserDirectoryPath() 
{
    const char* home = getenv("HOME");
    if (!home) 
    {
        std::cerr << "Failed to get the HOME environment variable." << std::endl;
        return "";
    }
    return std::string(home) + "/.local/share/DeadlyStagesRemix/";
}

bool ensureDirectoryExists(const std::string& directoryPath) 
{
    try 
    {
        if (!std::filesystem::exists(directoryPath)) 
        {
            return std::filesystem::create_directories(directoryPath);
        }
        return true;
    } 
    catch (const std::filesystem::filesystem_error& e) 
    {
        std::cerr << "Error creating directory: " << e.what() << std::endl;
        return false;
    }
}

int main(int argc, char* argv[])
{
    if (setWorkingDirectory() != 0)
        return 1;

    userDirPathPrefix = generateUserDirectoryPath();
    if (userDirPathPrefix.empty()) 
    {
        std::cerr << "Failed to get the user directory path." << std::endl;
    }
    if (ensureDirectoryExists(userDirPathPrefix)) 
    {
        std::cout << "User directory is ready: " << userDirPathPrefix << std::endl;
    }
    else 
    {
        std::cerr << "Failed to create user directory: " << userDirPathPrefix << std::endl;
        userDirPathPrefix = "";
    }

    Game::init();
    while (Game::isRunning)
    {
        Game::loop();
    }
    Game::dispose();

    return 0;
}