#ifndef __SRC_ADAPTER_OPTIONSADAPTER_HPP__
#define __SRC_ADAPTER_OPTIONSADAPTER_HPP__

#include <string>
#include "OptionsModel.hpp"

namespace OptionsAdapter
{
    extern OptionsModel extractOptionsFromGame();
    extern void applyOptionsFromModel(OptionsModel& options);

    extern std::string serializeOptions(OptionsModel& options);
    extern OptionsModel deserializeOptions(std::string& serializedOptions);
};

#endif // __SRC_ADAPTER_OPTIONSADAPTER_HPP__
