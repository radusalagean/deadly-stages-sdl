#include "../../Game.hpp"

#include "../../Platform/Common/SaveDataHelper.hpp"

#include <iostream>
#include <windows.h>
#include <shlobj.h>
#include <iostream>
#include <string>
#include <filesystem>

std::string resDirPathPrefix = "";
std::string userDirPathPrefix = "";

int setWorkingDirectory()
{
    // Buffer to store the path of the executable
    char executablePath[MAX_PATH];

    // Get the path of the executable
    DWORD pathLength = GetModuleFileName(NULL, executablePath, MAX_PATH);
    if (pathLength == 0) 
    {
        std::cerr << "Error: Unable to get executable path\n";
        return 1;
    }

    // Get the directory containing the executable
    char* lastBackslash = strrchr(executablePath, '\\');
    if (lastBackslash != NULL) 
    {
        *lastBackslash = '\0'; // Null-terminate the string at the last backslash
    }

    // Change the working directory
    if (!SetCurrentDirectory(executablePath)) 
    {
        std::cerr << "Error: Unable to change working directory\n";
        return 1;
    }

    // Now the working directory has been changed
    std::cout << "Working directory changed to: " << executablePath << std::endl;
    
    return 0;
}

std::string generateUserDirectoryPath() 
{
    char path[MAX_PATH];
    if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path))) 
    {
        std::cerr << "Failed to get the Local App Data path." << std::endl;
        return "";
    }
    return std::string(path) + "\\DeadlyStagesRemix";
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

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
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
            userDirPathPrefix += "\\";
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