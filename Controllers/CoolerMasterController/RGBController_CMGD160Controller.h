/*-------------------------------------------------------------------*\
|  RGBController_CMGD160Controller.cpp                              |
|                                                                     |
|  Driver for Coolermaster Gaming Monitor USB Controller              |
|                                                                     |
|  morg (Morgan Guimard) 9/18/2023                                    |
|                                                                     |
\*-------------------------------------------------------------------*/
#pragma once
#include "RGBController.h"
#include "CMGD160Controller.h"
#include <vector>

class RGBController_CMGD160Controller : public RGBController
{
public:
    RGBController_CMGD160Controller(CMGD160Controller* controller_ptr);
    ~RGBController_CMGD160Controller();

    void SetupZones();
    void ResizeZone(int zone, int new_size);
    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);
    void SetCustomMode();
    void DeviceUpdateMode();

private:
    CMGD160Controller*   controller;
};
