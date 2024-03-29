#include "../mousetrap_julia_binding.hpp"

// ### GRID VIEW

void implement_grid_view(jlcxx::Module& module)
{
    auto grid = module.add_type(GridView)
    .constructor<Orientation, SelectionMode>(USE_FINALIZERS)
    .method("push_back!", [](GridView& view, void* widget) -> void {
        view.push_back(*((Widget*) widget));
    })
    .method("push_front!", [](GridView& view, void* widget) -> void {
        view.push_front(*((Widget*) widget));
    })
    .method("insert!", [](GridView& view, uint64_t i, void* widget) -> void {
        view.insert(*((Widget*) widget), i);
    })
    .method("remove!", [](GridView& view, uint64_t index) -> void {
        view.remove(index);
    })
    .method("clear!", [](GridView& view) -> void {
        view.clear();
    })
    .method("find", [](GridView& view, void* widget) -> int {
        return view.find(*((Widget*) widget));
    })
    .add_type_method(GridView, get_n_items)
    .add_type_method(GridView, set_enable_rubberband_selection, !)
    .add_type_method(GridView, get_enable_rubberband_selection)
    .add_type_method(GridView, set_max_n_columns, !)
    .add_type_method(GridView, get_max_n_columns)
    .add_type_method(GridView, set_min_n_columns, !)
    .add_type_method(GridView, get_min_n_columns)
    .method("get_selection_model", [](GridView& view) -> void* {
        return view.get_selection_model()->get_internal();
    })
    .add_type_method(GridView, set_single_click_activate, !)
    .add_type_method(GridView, get_single_click_activate)
    .add_type_method(GridView, set_orientation, !)
    .add_type_method(GridView, get_orientation)
    ;

    add_widget_signals<GridView>(grid, "GridView");
    add_signal_activate_item<GridView>(grid, "GridView");
}