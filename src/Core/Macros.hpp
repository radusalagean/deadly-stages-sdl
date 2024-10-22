#ifndef __SRC_CORE_MACROS_HPP__
#define __SRC_CORE_MACROS_HPP__

#include "Constants.hpp"
#include "../Game.hpp"

#define USCALE(value) (value * Constants::UI_SCALE)
#define RPATH(filePath) (resDirPathPrefix + filePath)

#endif // __SRC_CORE_MACROS_HPP__
