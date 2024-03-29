#include "../mousetrap_julia_binding.hpp"

// ### SEPARATOR

void implement_separator(jlcxx::Module& module)
{
    auto separator = module.add_type(Separator)
    .constructor<Orientation>(USE_FINALIZERS)
    .add_type_method(Separator, set_orientation, !)
    .add_type_method(Separator, get_orientation)
    ;

    add_widget_signals<Separator>(separator, "Separator");
}