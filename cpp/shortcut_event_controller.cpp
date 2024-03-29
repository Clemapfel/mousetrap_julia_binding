#include "../mousetrap_julia_binding.hpp"

// ### SHORTCUT EVENT CONTROLLER

void implement_shortcut_event_controller(jlcxx::Module& module)
{
    define_enum_in(module, ShortcutScope);
    module.add_enum_value(ShortcutScope, SHORTCUT_SCOPE, LOCAL);
    module.add_enum_value(ShortcutScope, SHORTCUT_SCOPE, MANAGED);
    module.add_enum_value(ShortcutScope, SHORTCUT_SCOPE, GLOBAL);

    module.add_type(ShortcutEventController)
    .constructor<>(USE_FINALIZERS)
    .add_type_method(ShortcutEventController, add_action, !)
    .add_type_method(ShortcutEventController, remove_action, !)
    .add_type_method(ShortcutEventController, set_scope, !)
    .add_type_method(ShortcutEventController, get_scope)
    ;
}