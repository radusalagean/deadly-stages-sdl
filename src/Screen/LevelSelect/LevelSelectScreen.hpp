#ifndef __SRC_SCREEN_LEVELSELECT_LEVELSELECTSCREEN_HPP__
#define __SRC_SCREEN_LEVELSELECT_LEVELSELECTSCREEN_HPP__

#include "../Screen.hpp"
#include "../../Core/Debouncer.hpp"
#include "../../Drawable/TextDrawable.hpp"
#include <functional>
#include <string>
#include <vector>

class LevelPreview;

class LevelSelectScreen : public Screen
{
private:

    enum NavigationAction
    {
        NAVIGATION_ACTION_PREVIOUS = 0,
        NAVIGATION_ACTION_NEXT
    };
    
    std::vector<LevelPreview*> levelPreviews{};
    TextDrawable levelNameTextDrawable = TextDrawable("");
    TextDrawable loadingTextDrawable = TextDrawable("Loading...", {255, 0, 0, 255});
    bool loadingIndicatorRendered = false;
    std::string pendingLevelLoadId = "";

    void layoutText();

    void loadLevelPreviews();
    void selectLevel(const std::string& id);
    Debouncer navigationDebouncer;
    int selectedIndex = 0;

    void previousLevel();
    void nextLevel();

public:
    LevelSelectScreen();
    ~LevelSelectScreen();
    
    void init();
    void handleEvents();
    void layoutPass();
    void update();
    void render();
};

#endif // __SRC_SCREEN_LEVELSELECT_LEVELSELECTSCREEN_HPP__