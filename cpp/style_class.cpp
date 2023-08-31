//
// Created by clem on 8/27/23.
//

#include "../mousetrap_julia_binding.hpp"

namespace mousetrap
{
    void implement_style_class(jlcxx::Module& module)
    {
        auto style = module.add_type(StyleClass)
            .add_constructor(const std::string&)
            .add_type_method(StyleClass, get_name)
            .add_type_method(StyleClass, serialize)
            .method("set_property!", [](StyleClass& self, const std::string& target, const std::string& property, const std::string& value){
                self.set_property(target, property, value);
            })
            .method("get_property", [](const StyleClass& self, const std::string& property) -> std::string {
                return self.get_property(property);
            })
        ;

        module.method("style_manager_add_style_class!", [](const StyleClass& style){
            StyleManager::add_style_class(style);
        });
        module.method("remove_style_class!", [](const StyleClass& style){
            StyleManager::remove_style_class(style);
        });
        module.method("style_manager_get_style_class!", [](const std::string& name) -> StyleClass{
            return StyleManager::get_style_class(name);
        });
        module.method("style_manager_add_css!", [](const std::string& css){
            StyleManager::add_css(css);
        });
        module.method("style_manager_color_to_css_rgba", [](float r, float g, float b, float a){
            return StyleManager::color_to_css(RGBA(r, g, b, a));
        });
        module.method("style_manager_color_to_css_hsva", [](float h, float s, float v, float a){
           return StyleManager::color_to_css(HSVA(h, s, v, a));
        });
        module.method("style_manager_define_color_rgba!", [](const std::string& name, float r, float g, float b, float a){
            StyleManager::define_color(name, RGBA(r, g, b, a));
        });
        module.method("style_manager_define_color_hsva!", [](const std::string& name, float h, float s, float v, float a){
            StyleManager::define_color(name, RGBA(h, s, v, a));
        });
        
        #define EXPORT_CSS_PROPERTY(NAME) module.set_const(jl_box_string(std::string("STYLE_PROPERTY_") + #NAME, STYLE_PROPERTY_##NAME));
        EXPORT_CSS_PROPERTY(FOREGROUND_COLOR)
        EXPORT_CSS_PROPERTY(COLOR)
        EXPORT_CSS_PROPERTY(BACKGROUND_COLOR)
        EXPORT_CSS_PROPERTY(OPACITY)
        EXPORT_CSS_PROPERTY(FILTER)
        EXPORT_CSS_PROPERTY(FONT)
        EXPORT_CSS_PROPERTY(FONT_FAMILY)
        EXPORT_CSS_PROPERTY(FONT_VARIANT)
        EXPORT_CSS_PROPERTY(FONT_SIZE)
        EXPORT_CSS_PROPERTY(FONT_STYLE)
        EXPORT_CSS_PROPERTY(FONT_WEIGHT)
        EXPORT_CSS_PROPERTY(FONT_TRANSFORM)
        EXPORT_CSS_PROPERTY(CARET_COLOR)
        EXPORT_CSS_PROPERTY(TEXT_DECORATION)
        EXPORT_CSS_PROPERTY(TEXT_DECORATION_COLOR)
        EXPORT_CSS_PROPERTY(TEXT_DECORATION_STYLE)
        EXPORT_CSS_PROPERTY(TEXT_SHADOW)
        EXPORT_CSS_PROPERTY(ICON_SIZE)
        EXPORT_CSS_PROPERTY(TRANSFORM)
        EXPORT_CSS_PROPERTY(TRANSFORM_ORIGIN)
        EXPORT_CSS_PROPERTY(BORDER)
        EXPORT_CSS_PROPERTY(BORDER_STYLE)
        EXPORT_CSS_PROPERTY(BORDER_COLOR)
        EXPORT_CSS_PROPERTY(BORDER_WIDTH)
        EXPORT_CSS_PROPERTY(BORDER_RADIUS)
        EXPORT_CSS_PROPERTY(BORDER_SPACING)
        EXPORT_CSS_PROPERTY(OUTLINE)
        EXPORT_CSS_PROPERTY(OUTLINE_STYLE)
        EXPORT_CSS_PROPERTY(OUTLINE_COLOR)
        EXPORT_CSS_PROPERTY(OUTLINE_WIDTH)
        EXPORT_CSS_PROPERTY(BOX_SHADOW)
        EXPORT_CSS_PROPERTY(BACKGROUND_CLIP)
        EXPORT_CSS_PROPERTY(BACKGROUND_ORIGIN)
        EXPORT_CSS_PROPERTY(BACKGROUND_SIZE)
        EXPORT_CSS_PROPERTY(BACKGROUND_POSITION)
        EXPORT_CSS_PROPERTY(BACKGROUND_REPEAT)
        EXPORT_CSS_PROPERTY(TRANSITION)
        EXPORT_CSS_PROPERTY(TRANSITION_PROPERTY)
        EXPORT_CSS_PROPERTY(TRANSITION_DURATION)
        EXPORT_CSS_PROPERTY(TRANSITION_TIMING_FUNCTION)
        EXPORT_CSS_PROPERTY(TRANSITION_DELAY)
        EXPORT_CSS_PROPERTY(ANIMATION)
        EXPORT_CSS_PROPERTY(ANIMATION_NAME)
        EXPORT_CSS_PROPERTY(ANIMATION_DURATION)
        EXPORT_CSS_PROPERTY(ANIMATION_TIMING_FUNCTION)
        EXPORT_CSS_PROPERTY(ANIMATION_ITERATION_COUNT)
        EXPORT_CSS_PROPERTY(ANIMATION_DIRECTION)
        EXPORT_CSS_PROPERTY(ANIMATION_PLAY_STATE)
        EXPORT_CSS_PROPERTY(ANIMATION_DELAY)
        EXPORT_CSS_PROPERTY(ANIMATION_FILL_MODE)

        #define EXPORT_PRE_MADE_STYLE_CLASS_NAME(NAME) module.set_const(jl_box_string(std::string("STYLE_CLASS_") + #NAME, STYLE_CLASS_##NAME.get_name()));
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(SUGGESTED_ACTION);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(DESTRUCTIVE_ACTION);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(BUTTON_FLAT);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(BUTTON_RAISED);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(BUTTON_CIRCULAR);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(BUTTON_PILL);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(BOX_LINKED);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(TOOLBAR);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(DIMMED);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(FONT_HEADING_1);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(FONT_HEADING_2);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(FONT_HEADING_3);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(FONT_HEADING_4);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(FONT_LARGER);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(FONT_REGULAR);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(FONT_CAPTION_HEADING);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(FONT_CAPTION);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(FONT_MONOSPACE);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(FONT_NUMERIC);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(COLOR_ACCENT);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(COLOR_SUCCESS);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(COLOR_WARNING);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(COLOR_ERROR);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(ACTIVATABLE);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(CARD);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(LIST_VIEW_SIDEBAR);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(LIST_VIEW_RICH_LIST);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(LIST_VIEW_DATA_TABLE);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(LIST_VIEW_MENUBAR);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(ICON_DROPSHADOW);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(ICON_SMALL);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(CHECK_BUTTON_ROUND);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(ON_SCREEN_DISPLAY);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(BACKGROUND);
        EXPORT_PRE_MADE_STYLE_CLASS_NAME(VIEW);

        #define EXPORT_STYLE_TARGET(NAME) module.set_const(jl_box_string(std::string("STYLE_TARGET_") + #NAME, STYLE_TARGET_##NAME));
        EXPORT_STYLE_TARGET(SELF)
        EXPORT_STYLE_TARGET(ACTION_BAR)
        EXPORT_STYLE_TARGET(ACTION_BAR_BOX_START)
        EXPORT_STYLE_TARGET(ACTION_BAR_BOX_END)
        EXPORT_STYLE_TARGET(ASPECT_FRAME)
        EXPORT_STYLE_TARGET(BOX)
        EXPORT_STYLE_TARGET(BUTTON)
        EXPORT_STYLE_TARGET(BUTTON_PRESSED)
        EXPORT_STYLE_TARGET(CENTER_BOX)
        EXPORT_STYLE_TARGET(CHECK_BUTTON)
        EXPORT_STYLE_TARGET(CHECK_BUTTON_CHECK_MARK_ACTIVE)
        EXPORT_STYLE_TARGET(CHECK_BUTTON_CHECK_MARK_INCONSISTENT)
        EXPORT_STYLE_TARGET(CHECK_BUTTON_CHECK_MARK_INACTIVE)
        EXPORT_STYLE_TARGET(CLAMP_FRAME)
        EXPORT_STYLE_TARGET(COLUMN_VIEW)
        EXPORT_STYLE_TARGET(DROP_DOWN)
        EXPORT_STYLE_TARGET(ENTRY)
        EXPORT_STYLE_TARGET(ENTRY_TEXT)
        EXPORT_STYLE_TARGET(EXPANDER)
        EXPORT_STYLE_TARGET(EXPANDER_TITLE)
        EXPORT_STYLE_TARGET(EXPANDER_TITLE_ARROW)
        EXPORT_STYLE_TARGET(FLOW_BOX)
        EXPORT_STYLE_TARGET(FLOW_BOX_CHILD)
        EXPORT_STYLE_TARGET(FRAME)
        EXPORT_STYLE_TARGET(GRID)
        EXPORT_STYLE_TARGET(GRID_VIEW)
        EXPORT_STYLE_TARGET(GRID_VIEW_CHILDREN)
        EXPORT_STYLE_TARGET(GRID_VIEW_SELECTED)
        EXPORT_STYLE_TARGET(GRID_VIEW_NOT_SELECTED)
        EXPORT_STYLE_TARGET(HEADER_BAR)
        EXPORT_STYLE_TARGET(IMAGE_DISPLAY)
        EXPORT_STYLE_TARGET(LABEL)
        EXPORT_STYLE_TARGET(LEVEL_BAR)
        EXPORT_STYLE_TARGET(LEVEL_BAR_TROUGH)
        EXPORT_STYLE_TARGET(LEVEL_BAR_BLOCK_LOW)
        EXPORT_STYLE_TARGET(LEVEL_BAR_BLOCK_HIGH)
        EXPORT_STYLE_TARGET(LEVEL_BAR_BLOCK_FULL)
        EXPORT_STYLE_TARGET(LIST_VIEW)
        EXPORT_STYLE_TARGET(LIST_VIEW_CHILDREN)
        EXPORT_STYLE_TARGET(LIST_VIEW_SELECTED)
        EXPORT_STYLE_TARGET(LIST_VIEW_NOT_SELECTED)
        EXPORT_STYLE_TARGET(MENU_BAR)
        EXPORT_STYLE_TARGET(MENU_BAR_ITEM)
        EXPORT_STYLE_TARGET(MENU_BAR_DISABLED_ITEM)
        EXPORT_STYLE_TARGET(MENU_BAR_SELECTED_ITEM)
        EXPORT_STYLE_TARGET(NOTEBOOK)
        EXPORT_STYLE_TARGET(NOTEBOOK_TABS)
        EXPORT_STYLE_TARGET(NOTEBOOK_SELECTED_TAB)
        EXPORT_STYLE_TARGET(OVERLAY)
        EXPORT_STYLE_TARGET(PANED)
        EXPORT_STYLE_TARGET(PANED_HANDLE)
        EXPORT_STYLE_TARGET(PANED_START_CHILD)
        EXPORT_STYLE_TARGET(PANED_END_CHILD)
        EXPORT_STYLE_TARGET(POPOVER)
        EXPORT_STYLE_TARGET(POPOVER_ARROW)
        EXPORT_STYLE_TARGET(POPOVER_BUTTON)
        EXPORT_STYLE_TARGET(POPOVER_BUTTON_INDICATOR)
        EXPORT_STYLE_TARGET(POPOVER_MENU)
        EXPORT_STYLE_TARGET(POPOVER_MENU_ARROW)
        EXPORT_STYLE_TARGET(PROGRESS_BAR)
        EXPORT_STYLE_TARGET(PROGRESS_BAR_EMPTY)
        EXPORT_STYLE_TARGET(PROGRESS_BAR_FULL)
        EXPORT_STYLE_TARGET(PROGRESS_BAR_TROUGH)
        EXPORT_STYLE_TARGET(PROGRESS_BAR_TEXT)
        EXPORT_STYLE_TARGET(PROGRESS_BAR_DURING_PULSE)
        EXPORT_STYLE_TARGET(REVEALER)
        EXPORT_STYLE_TARGET(SCALE)
        EXPORT_STYLE_TARGET(SCALE_TROUGH)
        EXPORT_STYLE_TARGET(SCALE_TROUGH_FILL)
        EXPORT_STYLE_TARGET(SCALE_SLIDER)
        EXPORT_STYLE_TARGET(SCALE_VALUE_TEXT)
        EXPORT_STYLE_TARGET(SCROLLBAR)
        EXPORT_STYLE_TARGET(SCROLLBAR_TROUGH)
        EXPORT_STYLE_TARGET(SCOLLBAR_SLIDER)
        EXPORT_STYLE_TARGET(SEPARATOR)
        EXPORT_STYLE_TARGET(SPINBUTTON)
        EXPORT_STYLE_TARGET(SPINBUTTON_TEXT)
        EXPORT_STYLE_TARGET(SPINBUTTON_BUTTON_INCREASE)
        EXPORT_STYLE_TARGET(SPINBUTTON_BUTTON_DECREASE)
        EXPORT_STYLE_TARGET(SPINNER)
        EXPORT_STYLE_TARGET(STACK)
        EXPORT_STYLE_TARGET(STACK_SWITCHER)
        EXPORT_STYLE_TARGET(STACK_SIDEBAR)
        EXPORT_STYLE_TARGET(SWITCH)
        EXPORT_STYLE_TARGET(SWITCH_SLIDER)
        EXPORT_STYLE_TARGET(SWITCH_NOT_ACTIVE)
        EXPORT_STYLE_TARGET(SWITCH_ACTIVE)
        EXPORT_STYLE_TARGET(TEXTVIEW)
        EXPORT_STYLE_TARGET(TEXTVIEW_TEXT)
        EXPORT_STYLE_TARGET(TRANSFORM_BIN)
        EXPORT_STYLE_TARGET(TOGGLE_BUTTON)
        EXPORT_STYLE_TARGET(TOGGLE_BUTTON_ACTIVE)
        EXPORT_STYLE_TARGET(TOGGLE_BUTTON_NOT_ACTIVE)
        EXPORT_STYLE_TARGET(VIEWPORT)
        EXPORT_STYLE_TARGET(VIEWPORT_HORIZONTAL_SCROLLBAR)
        EXPORT_STYLE_TARGET(VIEWPORT_HORIZONTAL_SCROLLBAR_TROUGH)
        EXPORT_STYLE_TARGET(VIEWPORT_HORIZONTAL_SCOLLBAR_SLIDER)
        EXPORT_STYLE_TARGET(VIEWPORT_VERTICAL_SCROLLBAR)
        EXPORT_STYLE_TARGET(VIEWPORT_VERTICAL_SCROLLBAR_TROUGH)
        EXPORT_STYLE_TARGET(VIEWPORT_VERTICAL_SCOLLBAR_SLIDER)
        EXPORT_STYLE_TARGET(WINDOW)
        EXPORT_STYLE_TARGET(WIDGET)
        EXPORT_STYLE_TARGET(TEXT_ENTRY)
        EXPORT_STYLE_TARGET(TEXT_SELECTION)
    }
}
