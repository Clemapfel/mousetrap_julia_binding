cmake_minimum_required(VERSION 3.18)

### PROJECT ###

project(mousetrap_julia_binding
    VERSION 0.2.0
    LANGUAGES CXX
)

set(LIBRARY_NAME mousetrap_julia_binding)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

### DEPENDENCIES ###

find_package(JlCxx REQUIRED)
find_package(mousetrap REQUIRED)

if(${JlCxx_VERSION} VERSION_LESS 0.11.0)
    message(WARNING "CxxWrap version < 0.11.0 detected, this may mean that Ubuntu machines are unable to use this library, cf. https://github.com/JuliaInterop/CxxWrap.jl/issues/372")
endif()

### LIBRARY ###

add_library(${LIBRARY_NAME} SHARED

    mousetrap_julia_binding.hpp
    cpp/action.cpp
    cpp/action_bar.cpp
    cpp/adjustment.cpp
    cpp/alert_dialog.cpp
    cpp/alignment.cpp
    cpp/animation.cpp
    cpp/application.cpp
    cpp/aspect_frame.cpp
    cpp/blend_mode.cpp
    cpp/box.cpp
    cpp/box_unbox.cpp
    cpp/button.cpp
    cpp/center_box.cpp
    cpp/check_button.cpp
    cpp/clamp_frame.cpp
    cpp/click_event_controller.cpp
    cpp/clipboard.cpp
    cpp/color.cpp
    cpp/color_chooser.cpp
    cpp/column_view.cpp
    cpp/cursor_type.cpp
    cpp/define_julia_module.cpp
    cpp/drag_event_controller.cpp
    cpp/drop_down.cpp
    cpp/entry.cpp
    cpp/event_controller.cpp
    cpp/expander.cpp
    cpp/file_chooser.cpp
    cpp/file_descriptor.cpp
    cpp/file_monitor.cpp
    cpp/file_system.cpp
    cpp/fixed.cpp
    cpp/flow_box.cpp
    cpp/focus_event_controller.cpp
    cpp/frame.cpp
    cpp/frame_clock.cpp
    cpp/gc_sentinel.cpp
    cpp/gl_transform.cpp
    cpp/grid.cpp
    cpp/grid_view.cpp
    cpp/header_bar.cpp
    cpp/icon.cpp
    cpp/image.cpp
    cpp/image_display.cpp
    cpp/orientation.cpp
    cpp/justify_mode.cpp
    cpp/key_codes.cpp
    cpp/key_event_controller.cpp
    cpp/key_file.cpp
    cpp/label.cpp
    cpp/level_bar.cpp
    cpp/list_view.cpp
    cpp/log.cpp
    cpp/long_press_event_controller.cpp
    cpp/menu_bar.cpp
    cpp/menu_model.cpp
    cpp/motion_event_controller.cpp
    cpp/notebook.cpp
    cpp/overlay.cpp
    cpp/pan_event_controller.cpp
    cpp/paned.cpp
    cpp/pinch_zoom_event_controller.cpp
    cpp/popover.cpp
    cpp/popover_button.cpp
    cpp/popover_menu.cpp
    cpp/popup_message.cpp
    cpp/progress_bar.cpp
    cpp/relative_position.cpp
    cpp/render_area.cpp
    cpp/render_task.cpp
    cpp/revealer.cpp
    cpp/rotate_event_controller.cpp
    cpp/scale.cpp
    cpp/scroll_event_controller.cpp
    cpp/scrollbar.cpp
    cpp/selection_model.cpp
    cpp/separator.cpp
    cpp/shader.cpp
    cpp/shape.cpp
    cpp/shortcut_event_controller.cpp
    cpp/spin_button.cpp
    cpp/spinner.cpp
    cpp/stack.cpp
    cpp/style_class.cpp
    cpp/stylus_event_controller.cpp
    cpp/swipe_event_controller.cpp
    cpp/switch.cpp
    cpp/text_view.cpp
    cpp/texture.cpp
    cpp/theme.cpp
    cpp/time.cpp
    cpp/toggle_button.cpp
    cpp/transform_bin.cpp
    cpp/viewport.cpp
    cpp/widget.cpp
    cpp/window.cpp
    cpp/gl_canvas.cpp
)

target_link_libraries(${LIBRARY_NAME} PUBLIC
    ${MOUSETRAP_LIBRARIES}
    JlCxx::cxxwrap_julia
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    "${MOUSETRAP_INCLUDE_DIRECTORIES}"
    "${Julia_INCLUDE_DIRS}"
)

target_compile_features(${LIBRARY_NAME} PUBLIC
    cxx_std_17
)

### INSTALL ###

install(TARGETS ${LIBRARY_NAME} DESTINATION "${CMAKE_INSTALL_LIBDIR}")
