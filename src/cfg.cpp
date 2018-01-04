#include "MagicWindow/cfg.h"
#include "MagicWindow/utils.h"

using namespace ci;
using namespace magicwindow;

const std::string cfg::window::DISPLAY_SPAN = "span";
const std::string cfg::window::DISPLAY_CUSTOM = "custom";
const std::string cfg::window::DISPLAY_GRID = "grid";

void cfg::initialize(JsonTree config) {
  magic_window = config.getChild("magic_window");
  app_scale = json::get<float>(magic_window, "scale", 1.0f);
  bezels = json::get<bool>(magic_window, "bezels", false);
  cursor = json::get<bool>(magic_window, "cursor", true);
  default_keys = json::get<bool>(magic_window, "default_keys", true);
  fullscreen = json::get<bool>(magic_window, "fullscreen", true);
  params = json::get<bool>(magic_window, "params", false);
  param_coords = vec2(
                      json::get<int>(magic_window, "params/coords/x", 0),
                      json::get<int>(magic_window, "params/coords/y", 0)
                      );
  
  windows = magic_window.getChild("windows");
  window_mode = json::get<std::string>(magic_window, "window_mode", cfg::window::DISPLAY_SPAN);
  
  
  if (config.hasChild("app")) app = config.getChild("app");
}
