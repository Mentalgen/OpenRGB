/*-------------------------------------------------------------------*\
|  RGBController_CMGD160Controller.cpp                                |
|                                                                     |
|  Driver for Coolermaster GD160 Desk                                 |
|                                                                     |
|  Eclipse (Logan Phillips)                                           |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RGBController_CMGD160Controller.h"

#include <thread>
#include <chrono>

/*-------------------------------------------------------------------*\
    @name Coolermaster GD160 Desk
    @category LEDStrip
    @type USB
    @save :robot:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCoolerMasterDesk
    @comment
\*-------------------------------------------------------------------*/

RGBController_CMGD160Controller::RGBController_CMGD160Controller(CMGD160Controller* controller_ptr)
{
    controller                  = controller_ptr;
    name                        = "CoolerMaster LED Controller A1";
    vendor                      = "CoolerMaster";
    type                        = DEVICE_TYPE_LEDSTRIP;
    description                 = name;
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();
    version                     = "";

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = CM_GD160_DIRECT_MODE;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Spectrum;
    Spectrum.name               = "Spectrum cycle";
    Spectrum.value              = CM_GD160_SPECTRUM_MODE;
    Spectrum.flags              = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Spectrum.color_mode         = MODE_COLORS_NONE;
    Spectrum.speed_min          = CM_GD160_SPEED_MIN;
    Spectrum.speed_max          = CM_GD160_SPEED_MAX;
    Spectrum.speed              = CM_GD160_SPEED_MAX/2;
    Spectrum.brightness_min     = CM_GD160_BRIGHTNESS_MIN;
    Spectrum.brightness_max     = CM_GD160_BRIGHTNESS_MAX;
    Spectrum.brightness         = CM_GD160_BRIGHTNESS_MAX;
    modes.push_back(Spectrum);

    mode Reload;
    Reload.name                 = "Reload";
    Reload.value                = CM_GD160_RELOAD_MODE;
    Reload.flags                = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Reload.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Reload.colors_min           = 1;
    Reload.colors_max           = 1;
    Reload.colors.resize(1);
    Reload.speed_min            = CM_GD160_SPEED_MIN;
    Reload.speed_max            = CM_GD160_SPEED_MAX;
    Reload.speed                = CM_GD160_SPEED_MAX/2;
    Reload.brightness_min       = CM_GD160_BRIGHTNESS_MIN;
    Reload.brightness_max       = CM_GD160_BRIGHTNESS_MAX;
    Reload.brightness           = CM_GD160_BRIGHTNESS_MAX;
    modes.push_back(Reload);

    mode Recoil;
    Recoil.name                 = "Recoil";
    Recoil.value                = CM_GD160_RECOIL_MODE;
    Recoil.flags                = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Recoil.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Recoil.colors_min           = 1;
    Recoil.colors_max           = 1;
    Recoil.colors.resize(1);
    Recoil.speed_min            = CM_GD160_SPEED_MIN;
    Recoil.speed_max            = CM_GD160_SPEED_MAX;
    Recoil.speed                = CM_GD160_SPEED_MAX/2;
    Recoil.brightness_min       = CM_GD160_BRIGHTNESS_MIN;
    Recoil.brightness_max       = CM_GD160_BRIGHTNESS_MAX;
    Recoil.brightness           = CM_GD160_BRIGHTNESS_MAX;
    modes.push_back(Recoil);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = CM_GD160_BREATHING_MODE;
    Breathing.flags             = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.colors.resize(1);
    Breathing.speed_min         = CM_GD160_SPEED_MIN;
    Breathing.speed_max         = CM_GD160_SPEED_MAX;
    Breathing.speed             = CM_GD160_SPEED_MAX/2;
    Breathing.brightness_min    = CM_GD160_BRIGHTNESS_MIN;
    Breathing.brightness_max    = CM_GD160_BRIGHTNESS_MAX;
    Breathing.brightness        = CM_GD160_BRIGHTNESS_MAX;
    modes.push_back(Breathing);

    mode Refill;
    Refill.name                 = "Refill";
    Refill.value                = CM_GD160_REFILL_MODE;
    Refill.flags                = MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Refill.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Refill.colors_min           = 1;
    Refill.colors_max           = 1;
    Refill.colors.resize(1);
    Refill.speed_min            = CM_GD160_SPEED_MIN;
    Refill.speed_max            = CM_GD160_SPEED_MAX;
    Refill.speed                = CM_GD160_SPEED_MAX/2;
    Refill.brightness_min       = CM_GD160_BRIGHTNESS_MIN;
    Refill.brightness_max       = CM_GD160_BRIGHTNESS_MAX;
    Refill.brightness           = CM_GD160_BRIGHTNESS_MAX;
    modes.push_back(Refill);

    mode Custom;
    Custom.name                 = "Custom";
    Custom.value                = CM_GD160_CUSTOM_MODE;
    Custom.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Custom.color_mode           = MODE_COLORS_PER_LED;
    Custom.brightness_min       = CM_GD160_BRIGHTNESS_MIN;
    Custom.brightness_max       = CM_GD160_BRIGHTNESS_MAX;
    Custom.brightness           = CM_GD160_BRIGHTNESS_MAX;
    modes.push_back(Custom);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = CM_GD160_SPECTRUM_MODE;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_CMGD160Controller::~RGBController_CMGD160Controller()
{
    delete controller;
}

void RGBController_CMGD160Controller::SetupZones()
{
    zone z;

    z.name          = "Front Desk";
    z.type          = ZONE_TYPE_LINEAR;
    z.leds_min      = 96;
    z.leds_max      = 96;
    z.leds_count    = 96;
    z.matrix_map    = NULL;

    zones.push_back(z);

    for(unsigned int i = 0; i < 96; i++)
    {
        led l;
        l.name          = std::to_string(i + 1);
        l.value         = i;
        leds.push_back(l);
    }

    zone x;

    x.name          = "Back Desk";
    x.type          = ZONE_TYPE_LINEAR;
    x.leds_min      = 96;
    x.leds_max      = 96;
    x.leds_count    = 96;
    x.matrix_map    = NULL;

    zones.push_back(x);

    for(unsigned int i = 0; i < 96; i++)
    {
        led l;
        l.name          = std::to_string(i + 1);
        l.value         = i;
        leds.push_back(l);
    }

    SetupColors();
}

void RGBController_CMGD160Controller::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CMGD160Controller::DeviceUpdateLEDs()
{
    if(modes[active_mode].value == CM_GD160_DIRECT_MODE)
    {
        controller->SendDirect(colors);
    }
    else if(modes[active_mode].value == CM_GD160_CUSTOM_MODE)
    {
        controller->SetCustomMode(colors, modes[active_mode].brightness);
    }
}

void RGBController_CMGD160Controller::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMGD160Controller::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMGD160Controller::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_CMGD160Controller::DeviceUpdateMode()
{
    switch(modes[active_mode].value)
    {
        case CM_GD160_OFF_MODE:
        case CM_GD160_SPECTRUM_MODE:
            controller->SetMode(modes[active_mode].value, 0, modes[active_mode].speed, modes[active_mode].brightness);
            break;

        case CM_GD160_RELOAD_MODE:
        case CM_GD160_RECOIL_MODE:
        case CM_GD160_BREATHING_MODE:
        case CM_GD160_REFILL_MODE:
            controller->SetMode(modes[active_mode].value, modes[active_mode].colors[0], modes[active_mode].speed, modes[active_mode].brightness);
            break;

        case CM_GD160_CUSTOM_MODE:
            DeviceUpdateLEDs();
            break;
        default: break;
    }
}
