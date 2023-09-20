/**
 * @file
 */

#pragma once
#include "WindowMenuItems.hpp"
#include "WindowItemTempLabel.hpp"

class MI_HEAT_ENTIRE_BED : public WI_ICON_SWITCH_OFF_ON_t {
    constexpr static const char *const label = N_("Heat Entire Bed");

    bool init_index() const;

public:
    MI_HEAT_ENTIRE_BED()
        : WI_ICON_SWITCH_OFF_ON_t(init_index(), _(label), nullptr, is_enabled_t::yes, is_hidden_t::no) {}

protected:
    virtual void OnChange(size_t old_index) override;
};

class MI_INFO_MODULAR_BED_BOARD_TEMPERATURE : public WI_TEMP_LABEL_t {
    static constexpr const char *const label = N_("MBed Board Temp");

public:
    MI_INFO_MODULAR_BED_BOARD_TEMPERATURE();
};