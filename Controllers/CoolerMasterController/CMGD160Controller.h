/*-------------------------------------------------------------------*\
|  CMGD160Controller.cpp                                              |
|                                                                     |
|  Driver for Coolermaster Gaming Monitor                             |
|                                                                     |
|  morg (Morgan Guimard) 9/18/2023                                    |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RGBController.h"
#include <string>
#include <hidapi/hidapi.h>

#pragma once

#define CM_GD160_PACKET_LENGTH     65
#define CM_GD160_COLOR_DATA_LENGTH 577

enum
{
    CM_GD160_DIRECT_MODE         = 0xFF,
    CM_GD160_CUSTOM_MODE         = 0xFE,
    CM_GD160_SPECTRUM_MODE       = 0x00,
    CM_GD160_RELOAD_MODE         = 0x01,
    CM_GD160_RECOIL_MODE         = 0x02,
    CM_GD160_BREATHING_MODE      = 0x03,
    CM_GD160_REFILL_MODE         = 0x04,
    CM_GD160_OFF_MODE            = 0x06
};

enum
{
    CM_GD160_BRIGHTNESS_MAX      = 0xFF,
    CM_GD160_BRIGHTNESS_MIN      = 0x00,
    CM_GD160_SPEED_MAX           = 0x04,
    CM_GD160_SPEED_MIN           = 0x00,
};

class CMGD160Controller
{
public:
    CMGD160Controller(hid_device* dev_handle, const hid_device_info& info);
    ~CMGD160Controller();

    std::string     GetSerialString();
    std::string     GetDeviceLocation();
    void            SendDirect(const std::vector<RGBColor>& colors);
    void            SetMode(uint8_t mode_value, const RGBColor& color, uint8_t speed, uint8_t brightness);
    void            SetCustomMode(const std::vector<RGBColor>& colors, uint8_t brightnesss);
private:
    std::string     serial_number;
    std::string     location;
    hid_device*     dev;
    bool            software_mode_enabled = false;
    void            SetSoftwareModeEnabled(bool value);
};
