#include "MagicWindow/Config.h"
#include "MagicWindow/Utils.h"
#include "MagicWindow/WindowConfig.h"

using namespace ci;
using namespace magicwindow;

void Config::initialize(JsonTree config) {
    appNode = config.getChild("app");
    appScale = json::get<float>(config, "app/scale", 1.0f);
    defaultKeyHandlersEnabled = json::get<bool>(config, "app/default_keys", true);
    if (config.hasChild("extras")) extras = config.getChild("extras");
    fullscreen = json::get<bool>(config, "app/fullscreen", true);
    paramsWindowCoords = vec2(
        json::get<int>(config, "app/params_coords/x", 0),
        json::get<int>(config, "app/params_coords/y", 0)
    );
    showCursor = json::get<bool>(config, "app/cursor", true);
    showParams = json::get<bool>(config, "app/params", false);
    windowConfig = appNode.getChild("windows");
    windowMode = json::get<std::string>(config, "app/window_mode", WindowConfig::DISPLAY_SPAN);
}
