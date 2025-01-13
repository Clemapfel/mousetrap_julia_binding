#include "../mousetrap_julia_binding.hpp"
#include <thread>

JLCXX_MODULE define_julia_module(jlcxx::Module& module)
{
    jl_eval_string("Main.eval(:(import Mousetrap))"); // inject into Main scope

    module.set_const("GTK_MAJOR_VERSION", jl_box_int32(GTK_MAJOR_VERSION));
    module.set_const("GTK_MINOR_VERSION", jl_box_int32(GTK_MINOR_VERSION));
    module.set_const("GLIB_MAJOR_VERSION", jl_box_int32(GLIB_MAJOR_VERSION));
    module.set_const("GLIB_MINOR_VERSION", jl_box_int32(GLIB_MINOR_VERSION));
    module.set_const("ADW_MAJOR_VERSION", jl_box_int32(ADW_MAJOR_VERSION));
    module.set_const("ADW_MINOR_VERSION", jl_box_int32(ADW_MINOR_VERSION));
    module.set_const("MOUSETRAP_ENABLE_OPENGL_COMPONENT", jl_box_uint8(MOUSETRAP_ENABLE_OPENGL_COMPONENT));

    implement_orientation(module);
    implement_time(module);
    implement_theme(module);

    implement_adjustment(module);
    implement_action(module);
    implement_alignment(module);
    implement_application(module);
    implement_window(module);

    implement_file_monitor(module);
    implement_file_descriptor(module);
    implement_file_system(module);

    implement_event_controller(module);
    implement_drag_event_controller(module);
    implement_click_event_controller(module);
    implement_focus_event_controller(module);
    implement_key_event_controller(module);
    implement_long_press_event_controller(module);
    implement_motion_event_controller(module);
    implement_pinch_zoom_event_controller(module);
    implement_rotate_event_controller(module);
    implement_scroll_event_controller(module);
    implement_shortcut_event_controller(module);
    implement_stylus_event_controller(module);
    implement_swipe_event_controller(module);
    implement_pan_event_controller(module);

    implement_color(module);
    implement_icon(module);
    implement_image(module);
    implement_cursor_type(module);

    implement_relative_position(module);
    implement_menu_model(module);
    implement_menu_bar(module);
    implement_popover_menu(module);
    implement_popover(module);
    implement_popover_button(module);
    implement_progress_bar(module);

    implement_aspect_frame(module);
    implement_clamp_frame(module);
    implement_blend_mode(module);
    implement_box(module);
    implement_flow_box(module);
    implement_button(module);
    implement_center_box(module);
    implement_check_button(module);
    implement_clipboard(module);

    implement_selection_model(module);
    implement_list_view(module);
    implement_grid_view(module);
    implement_column_view(module);
    implement_grid(module);

    implement_image_display(module);
    implement_drop_down(module);
    implement_entry(module);
    implement_expander(module);
    implement_file_chooser(module);
    implement_alert_dialog(module);
    implement_color_chooser(module);

    implement_fixed(module);
    implement_frame(module);
    implement_frame_clock(module);

    implement_header_bar(module);
    implement_key_file(module);

    implement_justify_mode(module);
    implement_label(module);
    implement_text_view(module);

    implement_level_bar(module);
    implement_log(module);

    implement_notebook(module);
    implement_overlay(module);
    implement_paned(module);

    implement_revealer(module);
    implement_action_bar(module);
    implement_scale(module);
    implement_scrollbar(module);
    implement_separator(module);
    implement_spin_button(module);
    implement_spinner(module);
    implement_switch(module);
    implement_toggle_button(module);

    implement_stack(module);
    implement_viewport(module);
    implement_transform_bin(module);
    implement_style_class(module);

    implement_widget(module);

    implement_gl_transform(module);
    implement_texture(module);
    implement_shader(module);
    implement_shape(module);
    implement_render_task(module);
    implement_render_area(module);

    implement_key_codes(module);

    implement_animation(module);
    implement_popup_message(module);

    implement_gl_canvas(module);

    module.method("_ref", [](void* ptr){
        return (void*) g_object_ref(G_OBJECT(ptr));
    });

    module.method("_unref", [](void* ptr){
        g_object_unref(G_OBJECT(ptr));
    });

    module.method("as_internal", [](void* ptr) -> void* {
        return ((SignalEmitter*) ptr)->get_internal();
    });

    module.method("as_gobject", [](void* ptr) -> void* {
       return ((SignalEmitter*) ptr)->operator GObject*();
    });

    mousetrap::detail::notify_if_gtk_uninitialized::message = R"(
    Attempting to construct a widget, but the GTK4 backend has not yet been initialized.

    A typical `.jl` file using mousetrap should look like this:
    ```julia
    using mousetrap
    main() do app::Application
        # all construction of widgets should happen here
    end
    ```
    You have most likely attempted to construct a widget outside of `main` while using mousetrap interactively.
    )";

    module.method("is_opengl_disabled", []() -> bool {
        #if MOUSETRAP_ENABLE_OPENGL_COMPONENT
            return mousetrap::detail::is_opengl_disabled();
        #else
            return true;
        #endif
    });

    adw_init();
    detail::mark_gtk_initialized();

    # if MOUSETRAP_ENABLE_OPENGL_COMPONENT
        detail::initialize_opengl();
    #endif
}