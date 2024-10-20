#ifndef __SRC_CORE_OPTIONS_HPP__
#define __SRC_CORE_OPTIONS_HPP__

class Options
{
public:

    bool showFramerate = true;
    #ifdef SUPPORTS_AIM_ASSIST
    bool aimAssist = true;
    #endif
};

#endif // __SRC_CORE_OPTIONS_HPP__
