#pragma once

#include "common.h"
#include "hal/display.h"
#include "display_views.h"
#include "state_ref.h"

namespace fk {

MenuScreen *create_debug_module_menu(ModulePosition bay, GlobalState const *gs, MenuOption *back_option, Pool &pool);

} // namespace fk
