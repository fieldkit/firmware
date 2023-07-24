#include "display/debug_module_view.h"

namespace fk {

MenuScreen *create_debug_module_menu(ModulePosition bay, GlobalState const *gs, MenuOption *back_option, Pool &pool) {
    auto attached = gs->dynamic.attached();
    if (attached == nullptr) {
        return nullptr;
    }

    for (auto &attached_module : attached->modules()) {
        if (attached_module.position() == bay) {
            return attached_module.debug_menu(&pool);
        }
    }

    return nullptr;
}

} // namespace fk
