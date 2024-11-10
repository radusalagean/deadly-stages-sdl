#include "../../Game.hpp"

#include "../../Platform/Common/SaveDataHelper.hpp"
#include <iostream>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <mach-o/dyld.h>
#include <sys/stat.h>
#include "../../Core/Macros.hpp"
#include <string>
#include <filesystem>
#include <cstdlib>

#ifdef NDEBUG
std::string resDirPathPrefix = "Resources/";
#else
std::string resDirPathPrefix = "";
#endif
std::string userDirPathPrefix = "";

int setWorkingDirectory()
{
    uint32_t bufSize = PATH_MAX;
    
    // Buffer to store the path of the executable
    char executablePath[bufSize];

    // Get the path of the executable
    if (_NSGetExecutablePath(executablePath, &bufSize) != 0) 
    {
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
    if (chdir(expectedWorkingDirectory) != 0) 
    {
        std::cerr << "Error: Unable to change working directory\n";
        return 1;
    }

    // Now the working directory has been changed
    std::cout << "Working directory changed to: " << expectedWorkingDirectory << std::endl;
    
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
    return std::string(home) + "/Library/Application Support/DeadlyStagesRemix";
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
    else
    {
        if (ensureDirectoryExists(userDirPathPrefix)) 
        {
            std::cout << "User directory is ready: " << userDirPathPrefix << std::endl;
            userDirPathPrefix += "/";
        } 
        else 
        {
            std::cerr << "Failed to create user directory: " << userDirPathPrefix << std::endl;
            userDirPathPrefix = "";
        }
    }

    Game::init();
    while (Game::isRunning)
    {
        Game::loop();
    }
    Game::dispose();

    return 0;
}