#include "MagicWindow/controller.h"

using namespace magicwindow;

/**
 * Constructor
 */
controller::controller(context & context) : ctx(context) {}

void controller::connect_draw_signal() {
  draw_connection = ctx.signals.draw.connect([this] {
    draw();
  });
}

void controller::connect_frame_signals() {
  connect_update_signal();
  connect_draw_signal();
}

void controller::connect_update_signal() {
  update_connection = ctx.signals.update.connect([this] {
    update();
  });
}

void controller::disconnect_draw_signal() {
  if (draw_connection.isConnected()){
    draw_connection.disconnect();
  }
}

void controller::disconnect_frame_signals() {
  disconnect_update_signal();
  disconnect_draw_signal();
}

void controller::disconnect_update_signal() {
  if (update_connection.isConnected()) {
    update_connection.disconnect();
  }
}
