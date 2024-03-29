#include "../mousetrap_julia_binding.hpp"

// ### REVEALER

void implement_revealer(jlcxx::Module& module)
{
    define_enum_in(module, RevealerTransitionType);
    module.add_enum_value(RevealerTransitionType, REVEALER_TRANSITION_TYPE, NONE);
    module.add_enum_value(RevealerTransitionType, REVEALER_TRANSITION_TYPE, CROSSFADE);
    module.add_enum_value(RevealerTransitionType, REVEALER_TRANSITION_TYPE, SLIDE_RIGHT);
    module.add_enum_value(RevealerTransitionType, REVEALER_TRANSITION_TYPE, SLIDE_LEFT);
    module.add_enum_value(RevealerTransitionType, REVEALER_TRANSITION_TYPE, SLIDE_DOWN);
    module.add_enum_value(RevealerTransitionType, REVEALER_TRANSITION_TYPE, SLIDE_UP);
    module.add_enum_value(RevealerTransitionType, REVEALER_TRANSITION_TYPE, SWING_RIGHT);
    module.add_enum_value(RevealerTransitionType, REVEALER_TRANSITION_TYPE, SWING_LEFT);
    module.add_enum_value(RevealerTransitionType, REVEALER_TRANSITION_TYPE, SWING_UP);
    module.add_enum_value(RevealerTransitionType, REVEALER_TRANSITION_TYPE, SWING_DOWN);

    auto revealer = module.add_type(Revealer)
    .constructor<RevealerTransitionType>(USE_FINALIZERS)
    .method("set_child!", [](Revealer& self, void* child) {
        self.set_child(*((Widget*) child));
    })
    .add_type_method(Revealer, remove_child, !)
    .add_type_method(Revealer, set_is_revealed, !)
    .add_type_method(Revealer, get_is_revealed)
    .add_type_method(Revealer, set_transition_type, !)
    .add_type_method(Revealer, get_transition_type)
    .method("set_transition_duration!", [](Revealer& self, double mys){
        self.set_transition_duration(microseconds(mys));
    })
    .method("get_transition_duration", [](Revealer& self) -> double {
        return self.get_transition_duration().as_microseconds();
    })
    ;

    add_widget_signals<Revealer>(revealer, "Revealer");
    add_signal_revealed<Revealer>(revealer, "Revealer");
}