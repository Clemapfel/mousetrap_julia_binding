#include "../mousetrap_julia_binding.hpp"

// ### KEY EVENT CONTROLLER

void implement_key_event_controller(jlcxx::Module& module)
{
    module.add_bits<ModifierState>("_ModifierState");

    auto key = module.add_type(KeyEventController)
    .constructor<>(USE_FINALIZERS)
    .add_type_method(KeyEventController, should_shortcut_trigger_trigger)
    ;

    add_signal_key_pressed<KeyEventController>(key, "KeyEventController");
    add_signal_key_released<KeyEventController>(key, "KeyEventController");
    add_signal_modifiers_changed<KeyEventController>(key, "KeyEventController");

    module.method("shift_pressed", [](ModifierState state) -> bool {
        return KeyEventController::shift_pressed(state);
    });

    module.method("control_pressed", [](ModifierState state) -> bool {
        return KeyEventController::shift_pressed(state);
    });

    module.method("alt_pressed", [](ModifierState state) -> bool {
        return KeyEventController::shift_pressed(state);
    });

    module.method("mouset_button_01_pressed", [](ModifierState state) -> bool {
        return KeyEventController::shift_pressed(state);
    });

    module.method("mouset_button_02_pressed", [](ModifierState state) -> bool {
        return KeyEventController::shift_pressed(state);
    });
}