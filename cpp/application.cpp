#include "../mousetrap_julia_binding.hpp"

// ### APPLICATION

void implement_application(jlcxx::Module& module)
{
    auto application = module.add_type(Application)
    .constructor<const std::string&, bool>(USE_FINALIZERS)
    .add_type_method(Application, run, !)
    .add_type_method(Application, quit, !)
    .add_type_method(Application, hold, !)
    .add_type_method(Application, release, !)
    .add_type_method(Application, mark_as_busy, !)
    .add_type_method(Application, unmark_as_busy, !)
    .add_type_method(Application, get_is_marked_as_busy)
    .add_type_method(Application, get_is_holding)
    .add_type_method(Application, get_id)
    .add_type_method(Application, add_action, !)
    .add_type_method(Application, remove_action, !)
    .add_type_method(Application, has_action)
    .add_type_method(Application, get_action)
    .add_type_method(Application, get_current_theme)
    .add_type_method(Application, set_current_theme, !)
    ;

    add_signal_activate<Application>(application, "Application");
    add_signal_shutdown<Application>(application, "Application");
}