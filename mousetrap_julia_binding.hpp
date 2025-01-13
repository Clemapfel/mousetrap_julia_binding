//
// Created by clem on 4/16/23.
//

#include <julia.h>
#include <mousetrap.hpp>
#include <jlcxx/jlcxx.hpp>

#include <string>
#include <array>
#include <vector>

using namespace mousetrap;

static inline jl_value_t* jl_box_string(const std::string& in)
{
    static auto* string = jl_get_global(jl_base_module, jl_symbol("string"));
    return jl_call1(string, (jl_value_t*) jl_symbol(in.c_str()));
}

static void jl_println(jl_value_t* value)
{
    static auto* println = jl_get_function(jl_base_module, "println");
    jl_call1(println, value);
}

template<typename... T>
static inline jl_value_t* jl_calln(jl_function_t* function, T... args)
{
    std::array<jl_value_t*, sizeof...(T)> wrapped = {args...};
    return jl_call(function, wrapped.data(), wrapped.size());
}

template<typename... T>
static inline jl_value_t* jl_safe_call(const char* scope, jl_function_t* function, T... args)
{
    static auto* safe_call = jl_eval_string("return Mousetrap.safe_call");
    std::array<jl_value_t*, sizeof...(T)+2> wrapped = {jl_box_string(scope), function, args...};
    return jl_call(safe_call, wrapped.data(), wrapped.size());
}

static inline jl_value_t* jl_get_property(jl_value_t* object, const char* property_name)
{
    static auto* get_property = jl_get_function(jl_base_module, "getproperty");
    return jl_call2(get_property, object, (jl_value_t*) jl_symbol(property_name));
}

template<typename... Ts>
static inline jl_value_t* jl_wrap(Ts... args)
{
    static auto* wrap = jl_eval_string("(args...) -> args");
    return jl_calln(wrap, args...);
}

// RAII-style safeguard, protects from the GC until the wrapper is deallocated

jl_value_t* gc_protect_aux(NativeObject host, jl_value_t* value);

template<typename Host_t, typename Value_t>
jl_value_t* gc_protect(Host_t& host, Value_t* value)
{
    return gc_protect_aux(G_OBJECT(host.get_internal()), value);
}

jl_value_t* box_vector2f(Vector2f in);
Vector2f unbox_vector2f(jl_value_t* in);

jl_value_t* box_vector2i(Vector2i in);
Vector2f unbox_vector2i(jl_value_t* in);

jl_value_t* box_vector3f(Vector3f in);
Vector3f unbox_vector3f(jl_value_t* in);

jl_value_t* box_vector4f(Vector4f in);
Vector4f unbox_vector4f(jl_value_t* in);

jl_value_t* box_rgba(RGBA in);
RGBA unbox_rgba(jl_value_t* in);

jl_value_t* box_hsva(HSVA in);
HSVA unbox_hsva(jl_value_t* in);

#define USE_FINALIZERS jlcxx::finalize_policy::yes

#define declare_is_subtype_of(A, B) template<> struct jlcxx::SuperType<A> { typedef B type; };
#define make_not_mirrored(Name) template<> struct jlcxx::IsMirroredType<Name> : std::false_type {};

#define add_type(Type) add_type<Type>(std::string("_") + #Type)
#define add_type_method(Type, id, ...) method(#id + std::string(#__VA_ARGS__), &Type::id)

#define define_enum_in(module, Enum) \
    module.add_bits<Enum>(std::string("_") + #Enum, jl_int64_type); \
    module.method(std::string(#Enum) + "_to_int", [](Enum x) -> int64_t { return (int64_t) x; });

#define add_enum_value(Enum, PREFIX, VALUE) set_const(std::string(#PREFIX) + "_" + std::string(#VALUE), Enum::VALUE)

#define _DEFINE_ADD_SIGNAL_INVARIANT(snake_case) \
    method("disconnect_signal_" + std::string(#snake_case) + "!", [](T& instance) { \
        instance.disconnect_signal_##snake_case(); \
    }) \
    .method("set_signal_" + std::string(#snake_case) + "_blocked!", [](T& instance, bool b){ \
        instance.set_signal_##snake_case##_blocked(b); \
    }) \
    .method("get_signal_" + std::string(#snake_case) + "_blocked", [](T& instance) -> bool { \
        return instance.get_signal_##snake_case##_blocked(); \
    });

#define DEFINE_ADD_SIGNAL_ARG0(snake_case, return_t) \
template<typename T, typename Arg_t> \
void add_signal_##snake_case(Arg_t type, const std::string& name) \
{ \
    type.method("connect_signal_" + std::string(#snake_case) + "!", [name](T& instance, jl_function_t* task) \
    { \
        instance.connect_signal_##snake_case([name](T& instance, jl_function_t* task) -> return_t { \
            return jlcxx::unbox<return_t>( \
                jl_safe_call( \
                    (name + "::emit_signal_" + std::string(#snake_case)).c_str(), \
                    task,                                   \
                    jl_wrap(jlcxx::box<T&>(instance))\
                ) \
            ); \
        }, gc_protect(instance, task)); \
    }) \
    .method("emit_signal_" + std::string(#snake_case), [](T& instance) -> void { \
        instance.emit_signal_##snake_case(); \
    }); \
    type._DEFINE_ADD_SIGNAL_INVARIANT(snake_case)      \
}

#define DEFINE_ADD_SIGNAL_ARG1(snake_case, return_t, arg1_t, arg1_name) \
template<typename T, typename Arg_t> \
void add_signal_##snake_case(Arg_t type, const std::string& name) \
{ \
    type.method("connect_signal_" + std::string(#snake_case) + "!", [name](T& instance, jl_function_t* task) \
    { \
        instance.connect_signal_##snake_case([name](T& instance, arg1_t arg1_name, jl_function_t* task) -> return_t { \
            return jlcxx::unbox<return_t>( \
                jl_safe_call( \
                    (name + "::emit_signal_" + std::string(#snake_case)).c_str(), \
                    task, \
                    jl_wrap(jlcxx::box<T&>(instance), jlcxx::box<arg1_t>(arg1_name)) \
                ) \
            ); \
        }, gc_protect(instance, task)); \
    }) \
    .method("emit_signal_" + std::string(#snake_case), [](T& instance, arg1_t arg1_name) -> void { \
        instance.emit_signal_##snake_case(arg1_name); \
    }); \
    type._DEFINE_ADD_SIGNAL_INVARIANT(snake_case)      \
}

#define DEFINE_ADD_SIGNAL_ARG2(snake_case, return_t, arg1_t, arg1_name, arg2_t, arg2_name) \
template<typename T, typename Arg_t> \
void add_signal_##snake_case(Arg_t type, const std::string& name) \
{ \
    type.method("connect_signal_" + std::string(#snake_case) + "!", [name](T& instance, jl_function_t* task) \
    { \
        instance.connect_signal_##snake_case([name](T& instance, arg1_t arg1_name, arg2_t arg2_name, jl_function_t* task) -> return_t { \
            return jlcxx::unbox<return_t>( \
                jl_safe_call( \
                    (name + "::emit_signal_" + std::string(#snake_case)).c_str(), \
                    task, \
                    jl_wrap(jlcxx::box<T&>(instance), jlcxx::box<arg1_t>(arg1_name), jlcxx::box<arg2_t>(arg2_name)) \
                ) \
            ); \
        }, gc_protect(instance, task)); \
    }) \
    .method("emit_signal_" + std::string(#snake_case), [](T& instance, arg1_t arg1_name, arg2_t arg2_name) -> void { \
        instance.emit_signal_##snake_case(arg1_name, arg2_name); \
    }); \
    type._DEFINE_ADD_SIGNAL_INVARIANT(snake_case)      \
}

#define DEFINE_ADD_SIGNAL_ARG3(snake_case, return_t, arg1_t, arg1_name, arg2_t, arg2_name, arg3_t, arg3_name) \
template<typename T, typename Arg_t> \
void add_signal_##snake_case(Arg_t type, const std::string& name) \
{ \
    type.method("connect_signal_" + std::string(#snake_case) + "!", [name](T& instance, jl_function_t* task) \
    { \
        instance.connect_signal_##snake_case([name](T& instance, arg1_t arg1_name, arg2_t arg2_name, arg3_t arg3_name, jl_function_t* task) -> return_t { \
            return jlcxx::unbox<return_t>( \
                jl_safe_call( \
                    (name + "::emit_signal_" + std::string(#snake_case)).c_str(), \
                    task, \
                    jl_wrap(jlcxx::box<T&>(instance), jlcxx::box<arg1_t>(arg1_name), jlcxx::box<arg2_t>(arg2_name), jlcxx::box<arg3_t>(arg3_name)) \
                ) \
            ); \
        }, gc_protect(instance, task)); \
    }) \
    .method("emit_signal_" + std::string(#snake_case), [](T& instance, arg1_t arg1_name, arg2_t arg2_name, arg3_t arg3_name) -> void { \
        instance.emit_signal_##snake_case(arg1_name, arg2_name, arg3_name); \
    }); \
    type._DEFINE_ADD_SIGNAL_INVARIANT(snake_case)      \
}

#define DEFINE_ADD_SIGNAL_ARG0_VOID(snake_case) \
template<typename T, typename Arg_t> \
void add_signal_##snake_case(Arg_t type, const std::string& name) \
{ \
    type.method("connect_signal_" + std::string(#snake_case) + "!", [name](T& instance, jl_function_t* task) \
    { \
        instance.connect_signal_##snake_case([name](T& instance, jl_function_t* task) -> void { \
            /*return jlcxx::unbox<return_t>( */\
                jl_safe_call( \
                    (name + "::emit_signal_" + std::string(#snake_case)).c_str(), \
                    task,                                   \
                    jl_wrap(jlcxx::box<T&>(instance))\
                ) \
            /*)*/; \
        }, gc_protect(instance, task)); \
    }) \
    .method("emit_signal_" + std::string(#snake_case), [](T& instance) -> void { \
        instance.emit_signal_##snake_case(); \
    }); \
    type._DEFINE_ADD_SIGNAL_INVARIANT(snake_case)      \
}

#define DEFINE_ADD_SIGNAL_ARG1_VOID(snake_case,arg1_t, arg1_name) \
template<typename T, typename Arg_t> \
void add_signal_##snake_case(Arg_t type, const std::string& name) \
{ \
    type.method("connect_signal_" + std::string(#snake_case) + "!", [name](T& instance, jl_function_t* task) \
    { \
        instance.connect_signal_##snake_case([name](T& instance, arg1_t arg1_name, jl_function_t* task) -> void { \
            /*return jlcxx::unbox<return_t>(*/ \
                jl_safe_call( \
                    (name + "::emit_signal_" + std::string(#snake_case)).c_str(), \
                    task, \
                    jl_wrap(jlcxx::box<T&>(instance), jlcxx::box<arg1_t>(arg1_name)) \
                ) \
            /*)*/; \
        }, gc_protect(instance, task)); \
    }) \
    .method("emit_signal_" + std::string(#snake_case), [](T& instance, arg1_t arg1_name) -> void { \
        instance.emit_signal_##snake_case(arg1_name); \
    }); \
    type._DEFINE_ADD_SIGNAL_INVARIANT(snake_case)      \
}

#define DEFINE_ADD_SIGNAL_ARG2_VOID(snake_case, arg1_t, arg1_name, arg2_t, arg2_name) \
template<typename T, typename Arg_t> \
void add_signal_##snake_case(Arg_t type, const std::string& name) \
{ \
    type.method("connect_signal_" + std::string(#snake_case) + "!", [name](T& instance, jl_function_t* task) \
    { \
        instance.connect_signal_##snake_case([name](T& instance, arg1_t arg1_name, arg2_t arg2_name, jl_function_t* task) -> void { \
            /*return jlcxx::unbox<return_t>(*/ \
                jl_safe_call( \
                    (name + "::emit_signal_" + std::string(#snake_case)).c_str(), \
                    task, \
                    jl_wrap(jlcxx::box<T&>(instance), jlcxx::box<arg1_t>(arg1_name), jlcxx::box<arg2_t>(arg2_name)) \
                ) \
            /*)*/; \
        }, gc_protect(instance, task)); \
    }) \
    .method("emit_signal_" + std::string(#snake_case), [](T& instance, arg1_t arg1_name, arg2_t arg2_name) -> void { \
        instance.emit_signal_##snake_case(arg1_name, arg2_name); \
    }); \
    type._DEFINE_ADD_SIGNAL_INVARIANT(snake_case)      \
}

#define DEFINE_ADD_SIGNAL_ARG3_VOID(snake_case, arg1_t, arg1_name, arg2_t, arg2_name, arg3_t, arg3_name) \
template<typename T, typename Arg_t> \
void add_signal_##snake_case(Arg_t type, const std::string& name) \
{ \
    type.method("connect_signal_" + std::string(#snake_case) + "!", [name](T& instance, jl_function_t* task) \
    { \
        instance.connect_signal_##snake_case([name](T& instance, arg1_t arg1_name, arg2_t arg2_name, arg3_t arg3_name, jl_function_t* task) -> void { \
            /*return jlcxx::unbox<return_t>(*/ \
                jl_safe_call( \
                    (name + "::emit_signal_" + std::string(#snake_case)).c_str(), \
                    task, \
                    jl_wrap(jlcxx::box<T&>(instance), jlcxx::box<arg1_t>(arg1_name), jlcxx::box<arg2_t>(arg2_name), jlcxx::box<arg3_t>(arg3_name)) \
                ) \
            /*)*/; \
        }, gc_protect(instance, task)); \
    }) \
    .method("emit_signal_" + std::string(#snake_case), [](T& instance, arg1_t arg1_name, arg2_t arg2_name, arg3_t arg3_name) -> void { \
        instance.emit_signal_##snake_case(arg1_name, arg2_name, arg3_name); \
    }); \
    type._DEFINE_ADD_SIGNAL_INVARIANT(snake_case)      \
}

DEFINE_ADD_SIGNAL_ARG0_VOID(realize)
DEFINE_ADD_SIGNAL_ARG0_VOID(unrealize)
DEFINE_ADD_SIGNAL_ARG0_VOID(destroy)
DEFINE_ADD_SIGNAL_ARG0_VOID(hide)
DEFINE_ADD_SIGNAL_ARG0_VOID(show)
DEFINE_ADD_SIGNAL_ARG0_VOID(map)
DEFINE_ADD_SIGNAL_ARG0_VOID(unmap)

template<typename T, typename Arg_t>
void add_widget_signals(Arg_t& type, const std::string& name)
{
    add_signal_realize<T>(type, name);
    add_signal_unrealize<T>(type, name);
    add_signal_destroy<T>(type, name);
    add_signal_hide<T>(type, name);
    add_signal_show<T>(type, name);
    add_signal_map<T>(type, name);
    add_signal_unmap<T>(type, name);
}

DEFINE_ADD_SIGNAL_ARG0_VOID(activate)
DEFINE_ADD_SIGNAL_ARG0_VOID(startup)
DEFINE_ADD_SIGNAL_ARG0_VOID(shutdown)
DEFINE_ADD_SIGNAL_ARG0_VOID(update)
DEFINE_ADD_SIGNAL_ARG0_VOID(paint)
DEFINE_ADD_SIGNAL_ARG0_VOID(dismissed)
DEFINE_ADD_SIGNAL_ARG0_VOID(button_clicked)
DEFINE_ADD_SIGNAL_ARG0(close_request, WindowCloseRequestResult)
DEFINE_ADD_SIGNAL_ARG0_VOID(activate_default_widget)
DEFINE_ADD_SIGNAL_ARG0_VOID(activate_focused_widget)
DEFINE_ADD_SIGNAL_ARG0_VOID(clicked)
DEFINE_ADD_SIGNAL_ARG0_VOID(toggled)
DEFINE_ADD_SIGNAL_ARG0_VOID(text_changed)
// DEFINE_ADD_SIGNAL_ARG0_VOID(undo)
// DEFINE_ADD_SIGNAL_ARG0_VOID(redo)
DEFINE_ADD_SIGNAL_ARG2_VOID(selection_changed, gint, position, gint, n_items)
DEFINE_ADD_SIGNAL_ARG3(key_pressed, bool, guint, keyval, guint, keycode, ModifierState, modifier)
DEFINE_ADD_SIGNAL_ARG3_VOID(key_released, guint, keyval, guint, keycode, ModifierState, modifier)
DEFINE_ADD_SIGNAL_ARG1(modifiers_changed, bool, ModifierState, modifier)
DEFINE_ADD_SIGNAL_ARG2_VOID(motion_enter, double, x, double, y)
DEFINE_ADD_SIGNAL_ARG2_VOID(motion, double, x, double, y)
DEFINE_ADD_SIGNAL_ARG0_VOID(motion_leave)
DEFINE_ADD_SIGNAL_ARG3_VOID(click_pressed, gint, n_press, double, x, double, y)
DEFINE_ADD_SIGNAL_ARG3_VOID(click_released, gint, n_press, double, x, double, y)
DEFINE_ADD_SIGNAL_ARG0_VOID(click_stopped)
DEFINE_ADD_SIGNAL_ARG2_VOID(kinetic_scroll_decelerate, double, x_velocity, double, y_velocity)
DEFINE_ADD_SIGNAL_ARG0_VOID(scroll_begin)
DEFINE_ADD_SIGNAL_ARG2(scroll, bool, double, delta_x, double, delta_y)
DEFINE_ADD_SIGNAL_ARG0_VOID(scroll_end)
DEFINE_ADD_SIGNAL_ARG0_VOID(focus_gained)
DEFINE_ADD_SIGNAL_ARG0_VOID(focus_lost)
DEFINE_ADD_SIGNAL_ARG2_VOID(drag_begin, double, start_x, double, start_y)
DEFINE_ADD_SIGNAL_ARG2_VOID(drag, double, offset_x, double, offset_y)
DEFINE_ADD_SIGNAL_ARG2_VOID(drag_end, double, offset_x, double, offset_y)
DEFINE_ADD_SIGNAL_ARG1_VOID(scale_changed, double, scale)
DEFINE_ADD_SIGNAL_ARG2_VOID(rotation_changed, double, angle_absolute_radians, double, angle_delta_radians)
DEFINE_ADD_SIGNAL_ARG0_VOID(properties_changed)
DEFINE_ADD_SIGNAL_ARG0_VOID(value_changed)
DEFINE_ADD_SIGNAL_ARG2_VOID(resize, gint, width, gint, height)
DEFINE_ADD_SIGNAL_ARG2_VOID(page_added, void*, _, guint, page_index)
DEFINE_ADD_SIGNAL_ARG2_VOID(page_removed, void*, _, guint, page_index)
DEFINE_ADD_SIGNAL_ARG2_VOID(page_reordered, void*, _, guint, page_index)
DEFINE_ADD_SIGNAL_ARG2_VOID(page_selection_changed, void*, _, guint, page_index)
DEFINE_ADD_SIGNAL_ARG0_VOID(wrapped)
DEFINE_ADD_SIGNAL_ARG2_VOID(pressed, double, x, double, y)
DEFINE_ADD_SIGNAL_ARG0_VOID(press_cancelled)
DEFINE_ADD_SIGNAL_ARG2_VOID(pan, PanDirection, direction, double, offset)
DEFINE_ADD_SIGNAL_ARG2_VOID(swipe, double, x_velocity, double, y_velocity)
DEFINE_ADD_SIGNAL_ARG2_VOID(stylus_down, double, x, double, y)
DEFINE_ADD_SIGNAL_ARG2_VOID(stylus_up, double, x, double, y)
DEFINE_ADD_SIGNAL_ARG2_VOID(proximity, double, x, double, y)
DEFINE_ADD_SIGNAL_ARG2_VOID(scroll_child, ScrollType, scroll_type, bool, is_horizontal)
DEFINE_ADD_SIGNAL_ARG0_VOID(closed)
DEFINE_ADD_SIGNAL_ARG0_VOID(play)
DEFINE_ADD_SIGNAL_ARG0_VOID(stop)
DEFINE_ADD_SIGNAL_ARG3_VOID(items_changed, gint, position, gint, n_removed, gint, n_added)
DEFINE_ADD_SIGNAL_ARG1_VOID(revealed, void*, _)
DEFINE_ADD_SIGNAL_ARG1_VOID(switched, void*, _)
DEFINE_ADD_SIGNAL_ARG1_VOID(activated, void*, _)
DEFINE_ADD_SIGNAL_ARG1_VOID(activate_item, guint, index)

template<typename T, typename Arg_t>
void add_signal_render(Arg_t type, const std::string& name)
{
    type.method("connect_signal_render!", [name](T& instance, jl_function_t* task)
    {
        instance.connect_signal_render([name](T& instance, GdkGLContext* context, jl_function_t* task) -> bool {
            return jlcxx::unbox<bool>(jl_safe_call(
                (name + "::emit_signal_render").c_str(),
                task,
                jl_wrap(jlcxx::box<T&>(instance), jlcxx::box<void*>((void*) context))
            ));
        }, gc_protect(instance, task));
    })
    .method("emit_signal_render", [](T& instance, void* ptr) -> void {
        instance.emit_signal_render((GdkGLContext*) ptr);
    });
    type._DEFINE_ADD_SIGNAL_INVARIANT(render)
}

void implement_adjustment(jlcxx::Module& module);
void implement_action(jlcxx::Module& module);
void implement_action_bar(jlcxx::Module& module);
void implement_alignment(jlcxx::Module& module);
void implement_alert_dialog(jlcxx::Module& module);
void implement_application(jlcxx::Module& module);
void implement_animation(jlcxx::Module& module);
void implement_aspect_frame(jlcxx::Module& module);
void implement_blend_mode(jlcxx::Module& module);
void implement_box(jlcxx::Module& module);
void implement_button(jlcxx::Module& module);
void implement_center_box(jlcxx::Module& module);
void implement_check_button(jlcxx::Module& module);
void implement_clamp_frame(jlcxx::Module& module);
void implement_click_event_controller(jlcxx::Module& module);
void implement_clipboard(jlcxx::Module& module);
void implement_color(jlcxx::Module& module);
void implement_color_chooser(jlcxx::Module& module);
void implement_column_view(jlcxx::Module& module);
void implement_cursor_type(jlcxx::Module& module);
void implement_drag_event_controller(jlcxx::Module& module);
void implement_drop_down(jlcxx::Module& module);
void implement_entry(jlcxx::Module& module);
void implement_event_controller(jlcxx::Module& module);
void implement_expander(jlcxx::Module& module);
void implement_file_monitor(jlcxx::Module& module);
void implement_file_descriptor(jlcxx::Module& module);
void implement_file_system(jlcxx::Module& module);
void implement_file_chooser(jlcxx::Module& module);
void implement_fixed(jlcxx::Module& module);
void implement_flow_box(jlcxx::Module& module);
void implement_focus_event_controller(jlcxx::Module& module);
void implement_frame(jlcxx::Module& module);
void implement_frame_clock(jlcxx::Module& module);
void implement_gl_transform(jlcxx::Module& module);
void implement_grid(jlcxx::Module& module);
void implement_grid_view(jlcxx::Module& module);
void implement_header_bar(jlcxx::Module& module);
void implement_icon(jlcxx::Module& module);
void implement_image(jlcxx::Module& module);
void implement_image_display(jlcxx::Module& module);
void implement_justify_mode(jlcxx::Module& module);
void implement_key_codes(jlcxx::Module& module);
void implement_key_event_controller(jlcxx::Module& module);
void implement_key_file(jlcxx::Module& module);
void implement_label(jlcxx::Module& module);
void implement_level_bar(jlcxx::Module& module);
void implement_list_view(jlcxx::Module& module);
void implement_log(jlcxx::Module& module);
void implement_long_press_event_controller(jlcxx::Module& module);
void implement_gl_canvas(jlcxx::Module& module);
void implement_menu_model(jlcxx::Module& module);
void implement_menu_bar(jlcxx::Module& module);
void implement_motion_event_controller(jlcxx::Module& module);
void implement_notebook(jlcxx::Module& module);
void implement_orientation(jlcxx::Module& module);
void implement_overlay(jlcxx::Module& module);
void implement_pan_event_controller(jlcxx::Module& module);
void implement_paned(jlcxx::Module& module);
void implement_pinch_zoom_event_controller(jlcxx::Module& module);
void implement_popover(jlcxx::Module& module);
void implement_popover_button(jlcxx::Module& module);
void implement_popover_menu(jlcxx::Module& module);
void implement_popup_message(jlcxx::Module& module);
void implement_progress_bar(jlcxx::Module& module);
void implement_relative_position(jlcxx::Module& module);
void implement_render_area(jlcxx::Module& module);
void implement_render_task(jlcxx::Module& module);
void implement_revealer(jlcxx::Module& module);
void implement_rotate_event_controller(jlcxx::Module& module);
void implement_scale(jlcxx::Module& module);
void implement_scroll_event_controller(jlcxx::Module& module);
void implement_scrollbar(jlcxx::Module& module);
void implement_selection_model(jlcxx::Module& module);
void implement_separator(jlcxx::Module& module);
void implement_shader(jlcxx::Module& module);
void implement_shape(jlcxx::Module& module);
void implement_shortcut_event_controller(jlcxx::Module& module);
void implement_spin_button(jlcxx::Module& module);
void implement_spinner(jlcxx::Module& module);
void implement_stack(jlcxx::Module& module);
void implement_style_class(jlcxx::Module& module);
void implement_stylus_event_controller(jlcxx::Module& module);
void implement_swipe_event_controller(jlcxx::Module& module);
void implement_switch(jlcxx::Module& module);
void implement_text_view(jlcxx::Module& module);
void implement_theme(jlcxx::Module& module);
void implement_time(jlcxx::Module& module);
void implement_texture(jlcxx::Module& module);
void implement_toggle_button(jlcxx::Module& module);
void implement_transform_bin(jlcxx::Module& module);
void implement_viewport(jlcxx::Module& module);
void implement_widget(jlcxx::Module& module);
void implement_window(jlcxx::Module& module);

JLCXX_MODULE define_julia_module(jlcxx::Module& module);
