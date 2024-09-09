#ifndef __SRC_SCREENMANAGER_SCREENMANAGER_HPP__
#define __SRC_SCREENMANAGER_SCREENMANAGER_HPP__

#include <vector>

class Screen;

class ScreenManager
{
private:
    static ScreenManager* instance;

    // Private constructor to prevent instantiation
    ScreenManager() {}

    // Delete copy constructor and assignment operator
    ScreenManager(const ScreenManager&) = delete;
    ScreenManager& operator=(const ScreenManager&) = delete;

    std::vector<Screen*> screens{};
    std::vector<Screen*> screensToAdd{};
    std::vector<Screen*> screensToRemove{};
    bool clearAllScreens = false;

    void handlePendingTransactions();

public:
    // Static method to get the instance
    static ScreenManager& getInstance() 
    {
        if (instance == nullptr)
            instance = new ScreenManager();
        return *instance;
    }

    void pushScreen(Screen* screen);
    void popScreen();
    void setScreen(Screen* screen);
    Screen* getCurrentScreen();

    void init();
    void handleEvents();
    void update();
    void render();
    void dispose();
};

#endif // __SRC_SCREENMANAGER_SCREENMANAGER_HPP__




