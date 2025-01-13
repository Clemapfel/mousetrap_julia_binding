#include "../mousetrap_julia_binding.hpp"

// ### DROP DOWN

void implement_drop_down(jlcxx::Module& module)
{
    auto drop_down = module.add_type(DropDown)
        .constructor<>(USE_FINALIZERS)
        .add_type_method(DropDown, remove, !)
		.add_type_method(DropDown, empty, !)
        .add_type_method(DropDown, set_always_show_arrow, !)
        .add_type_method(DropDown, get_always_show_arrow)
        .add_type_method(DropDown, set_selected, !)
        .add_type_method(DropDown, get_selected)
        .add_type_method(DropDown, get_item_at)
    ;

    drop_down.method("push_back!", [](DropDown& drop_down, void* list_widget, void* label_widget, jl_value_t* task_in){

        auto* task = gc_protect(drop_down, task_in);
        return drop_down.push_back(*((Widget*) list_widget), *((Widget*) label_widget), [](DropDown& self, jl_value_t* task) {
            jl_safe_call("DropDown::item_selection_changed", task, jlcxx::box<DropDown&>(self));
        }, gc_protect(drop_down, task));
    });

    drop_down.method("push_front!", [](DropDown& drop_down, void* list_widget, void* label_widget, jl_value_t* task){
        return drop_down.push_front(*((Widget*) list_widget), *((Widget*) label_widget), [](DropDown& self, jl_value_t* task) {
            jl_safe_call("DropDown::item_selection_changed", task, jlcxx::box<DropDown&>(self));
        }, gc_protect(drop_down, task));
    });

    drop_down.method("insert!", [](DropDown& drop_down, uint64_t i, void* list_widget, void* label_widget, jl_value_t* task){
        return drop_down.insert(i, *((Widget*) list_widget), *((Widget*) label_widget), [](DropDown& self, jl_value_t* task) {
            jl_safe_call("DropDown::item_selection_changed", task, jlcxx::box<DropDown&>(self));
        }, gc_protect(drop_down, task));
    });

    add_widget_signals<DropDown>(drop_down, "DropDown");
}