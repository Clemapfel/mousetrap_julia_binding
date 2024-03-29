#include "../mousetrap_julia_binding.hpp"


// ### SCALE

void implement_scale(jlcxx::Module& module)
{
    auto scale = module.add_type(Scale)
    .constructor<float, float, float, Orientation>(USE_FINALIZERS)
    .method("get_adjustment", [](Scale& self) -> void* {
        return self.get_adjustment().get_internal();
    })
    .add_type_method(Scale, get_lower)
    .add_type_method(Scale, get_upper)
    .add_type_method(Scale, get_value)
    .add_type_method(Scale, get_step_increment)
    .add_type_method(Scale, set_lower, !)
    .add_type_method(Scale, set_upper, !)
    .add_type_method(Scale, set_value, !)
    .add_type_method(Scale, set_step_increment, !)
    .add_type_method(Scale, set_should_draw_value, !)
    .add_type_method(Scale, get_should_draw_value)
    .add_type_method(Scale, set_has_origin, !)
    .add_type_method(Scale, get_has_origin)
    .add_type_method(Scale, add_mark, !)
    .add_type_method(Scale, clear_marks, !)
    .add_type_method(Scale, set_orientation, !)
    .add_type_method(Scale, get_orientation)
    ;

    add_widget_signals<Scale>(scale, "Scale");
    add_signal_value_changed<Scale>(scale, "Scale");
}