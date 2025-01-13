#include "../mousetrap_julia_binding.hpp"

// ### COLUMN VIEW

using ColumnViewColumn = ColumnView::Column;

void implement_column_view(jlcxx::Module& module)
{
    auto column = module.add_type(ColumnViewColumn)
    .constructor([](void* internal){
        return new ColumnView::Column(GTK_COLUMN_VIEW_COLUMN(internal));
    }, USE_FINALIZERS)
    .add_type_method(ColumnViewColumn, set_title, !)
    .add_type_method(ColumnViewColumn, get_title)
    .add_type_method(ColumnViewColumn, set_fixed_width, !)
    .add_type_method(ColumnViewColumn, get_fixed_width)
    .add_type_method(ColumnViewColumn, set_header_menu, !)
    .add_type_method(ColumnViewColumn, set_is_visible, !)
    .add_type_method(ColumnViewColumn, get_is_visible)
    .add_type_method(ColumnViewColumn, set_is_resizable, !)
    .add_type_method(ColumnViewColumn, get_is_resizable)
    ;

    auto column_view = module.add_type(ColumnView)
    .constructor<SelectionMode>(USE_FINALIZERS)
    .add_type_method(ColumnView, push_back_column, !)
    .add_type_method(ColumnView, push_front_column, !)
    .add_type_method(ColumnView, insert_column, !)
    .add_type_method(ColumnView, remove_column, !)
    .add_type_method(ColumnView, get_column_at)
    .add_type_method(ColumnView, get_column_with_title)
    .add_type_method(ColumnView, has_column_with_title)
	.add_type_method(ColumnView, remove_row, !)
    .method("set_widget_at!", [](ColumnView& view, ColumnView::Column& column, uint64_t row_i, void* widget){
        view.set_widget_at(column, row_i, *((Widget*) widget));
    })
    .add_type_method(ColumnView, set_enable_rubberband_selection, !)
    .add_type_method(ColumnView, get_enable_rubberband_selection)
    .add_type_method(ColumnView, set_show_row_separators, !)
    .add_type_method(ColumnView, get_show_row_separators)
    .add_type_method(ColumnView, set_show_column_separators, !)
    .add_type_method(ColumnView, get_show_column_separators)
    .add_type_method(ColumnView, set_single_click_activate, !)
    .add_type_method(ColumnView, get_single_click_activate)
    .add_type_method(ColumnView, get_n_rows)
    .add_type_method(ColumnView, get_n_columns)
    .method("get_selection_model", [](ColumnView& view) -> void* {
        return view.get_selection_model()->get_internal();
    })
    ;

    add_widget_signals<ColumnView>(column_view, "ColumnView");
    add_signal_activate<ColumnView>(column_view, "ColumnView");
}