#ifndef __SRC_SCREENMANAGER_SCREENMANAGER_HPP__
#define __SRC_SCREENMANAGER_SCREENMANAGER_HPP__

#include <vector>

class Screen;

class ScreenManager
{
private:
    std::vector<Screen*> screens{};
    std::vector<Screen*> screensToAdd{};
    std::vector<Screen*> screensToRemove{};
    bool clearAllScreens = false;

    void handlePendingTransactions();

public:
    ~ScreenManager();

    void pushScreen(Screen* screen);
    void popScreen();
    void setScreen(Screen* screen);
    Screen* getCurrentScreen();

    void init();
    void handleEvents();
    void update();
    void render();

    void onRendererOutputSizeChanged();
};

#endif // __SRC_SCREENMANAGER_SCREENMANAGER_HPP__




