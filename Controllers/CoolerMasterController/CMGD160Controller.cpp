/*-------------------------------------------------------------------*\
|  CMGD160Controller.cpp                                              |
|                                                                     |
|  Driver for Coolermaster GD160 Desk                                 |
|                                                                     |
|  Eclipse (Logan Phillips)                                           |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "CMGD160Controller.h"

#include <cstring>

using namespace std::chrono_literals;

CMGD160Controller::CMGD160Controller(hid_device* dev_handle,
  const hid_device_info& info)
  {
  dev      = dev_handle;
  location = info.path;

  wchar_t serial_string[128];
  int ret = hid_get_serial_number_string(dev, serial_string, 128);

  if (ret != 0)
  {
    serial_number = "";
  }
  else
  {
    std::wstring return_wstring = serial_string;
    serial_number = std::string(return_wstring.begin(), return_wstring.end());
  }
}

CMGD160Controller::~CMGD160Controller()
{
  hid_close(dev);
}

std::string CMGD160Controller::GetDeviceLocation()
{
  return ("HID: " + location);
}

std::string CMGD160Controller::GetSerialString()
{
  return (serial_number);
}

void CMGD160Controller::SetMode(uint8_t mode_value, const RGBColor& color, uint8_t speed, uint8_t brightness)
{
  if (software_mode_enabled)
  {
    SetSoftwareModeEnabled(false);
  }

  for (unsigned int i = 0; i < 2; i++)
  {
    uint8_t usb_buf[CM_GD160_PACKET_LENGTH];
    memset(usb_buf, 0x00, CM_GD160_PACKET_LENGTH);

    usb_buf[1]  = 0x80;
    usb_buf[2]  = (mode_value == CM_GD160_OFF_MODE) ? 0x0F : 0x0B;
    usb_buf[3]  = 0x02;
    usb_buf[4]  = (i == 0) ? 0x01 : 0x02; //Sets both sides of desk to a specified mode
    usb_buf[5]  = mode_value;
    usb_buf[6]  = (mode_value == CM_GD160_OFF_MODE) ? 0x00 : 0x08;
    usb_buf[7]  = speed;
    usb_buf[8]  = brightness;
    usb_buf[9]  = RGBGetRValue(color);
    usb_buf[10] = RGBGetGValue(color);
    usb_buf[11] = RGBGetBValue(color);

    hid_write(dev, usb_buf, CM_GD160_PACKET_LENGTH);
  }
}
void CMGD160Controller::SetCustomMode(const std::vector <RGBColor>& colors, uint8_t brightnesss)
{
  if (software_mode_enabled)
  {
    SetSoftwareModeEnabled(false);
  }

  /*---------------------------------------------------------*\
  | Creates the color buffer                                  |
  \*---------------------------------------------------------*/
  uint8_t color_data[CM_GD160_COLOR_DATA_LENGTH];
  memset(color_data, 0x00, CM_GD160_COLOR_DATA_LENGTH);

  uint16_t offset = 0;

  for (const RGBColor & color: colors) {
    color_data[offset++] = RGBGetRValue(color);
    color_data[offset++] = RGBGetGValue(color);
    color_data[offset++] = RGBGetBValue(color);
  }

  /*---------------------------------------------------------*\
  | Sends 2 sets of 7 sequence packets,                      |
  \*---------------------------------------------------------*/

  //It doesn't even need the last 2 packets, why does it need them??
  //There's 41 bytes of color data in packet 5 and the 6th and 7th is just essentially trash bytes

  uint8_t usb_buf[CM_GD160_PACKET_LENGTH];

  for (unsigned int j = 0; j < 2; j++)
  {
    for (unsigned int i = 0; i < 7; i++)
    {
      memset(usb_buf, 0x00, CM_GD160_PACKET_LENGTH);

      usb_buf[1] = i < 6 ? i : 0x86;

      /*---------------------------------------------------------*\
      | First packet contains static data                         |
      \*---------------------------------------------------------*/
      if (i == 0)
      {
        usb_buf[2] = 0x10;
        usb_buf[3] = 0x02;
        usb_buf[4] = (j == 1) ? 0x02 : 0x01;
        usb_buf[5] = 0x80;
        usb_buf[6] = brightnesss;

        offset = (j == 1) ? 288 : 0;
        //Setting this back to offset 288 instead of padding packets makes this a lot more simple.

        memcpy(&usb_buf[7], &color_data[offset], CM_GD160_PACKET_LENGTH - 7);
        offset += CM_GD160_PACKET_LENGTH - 7;
      }
      else
      {
        memcpy(&usb_buf[2], &color_data[offset], CM_GD160_PACKET_LENGTH - 2);
        offset += (CM_GD160_PACKET_LENGTH - 2);
      }

      hid_write(dev, usb_buf, CM_GD160_PACKET_LENGTH);
    }
  }
}

void CMGD160Controller::SendDirect(const std::vector < RGBColor > & colors)
{
  if (software_mode_enabled)
  {
    SetSoftwareModeEnabled(true);
  }

  /*---------------------------------------------------------*\
  | Creates the color buffer                                  |
  \*---------------------------------------------------------*/
  uint8_t color_data[CM_GD160_COLOR_DATA_LENGTH];
  memset(color_data, 0x00, CM_GD160_COLOR_DATA_LENGTH);

  uint16_t offset = 0;

  for (const RGBColor & color: colors) {
    color_data[offset++] = RGBGetRValue(color);
    color_data[offset++] = RGBGetGValue(color);
    color_data[offset++] = RGBGetBValue(color);
  }

  /*---------------------------------------------------------*\
  | Sends 2 sets the 7 sequence packets,                      |
  \*---------------------------------------------------------*/

  //It doesn't even need the last 2 packets, why does it need them??
  //There's 41 bytes of color in packet 5 and the 6th and 7th is just trash bytes

  uint8_t usb_buf[CM_GD160_PACKET_LENGTH];

  for (unsigned int j = 0; j < 2; j++)
  {
    for (unsigned int i = 0; i < 7; i++)
    {
      memset(usb_buf, 0x00, CM_GD160_PACKET_LENGTH);

      usb_buf[1] = i < 6 ? i : 0x86;

      /*---------------------------------------------------------*\
      | First packet contains static data                         |
      \*---------------------------------------------------------*/
      if (i == 0)
      {
        usb_buf[2] = 0x10;
        usb_buf[3] = 0x02;
        usb_buf[4] = (j == 1) ? 0x02 : 0x01;
        usb_buf[5] = 0x80;
        usb_buf[6] = 0xFF;

        offset = (j == 1) ? 288 : 0;
        //Setting this back to offset 288 instead of padding packets makes this a lot more simple.

        memcpy(&usb_buf[7], &color_data[offset], CM_GD160_PACKET_LENGTH - 7);
        offset += CM_GD160_PACKET_LENGTH - 7;
      }
      else
      {
        memcpy(&usb_buf[2], &color_data[offset], CM_GD160_PACKET_LENGTH - 2);
        offset += (CM_GD160_PACKET_LENGTH - 2);
      }

      hid_write(dev, usb_buf, CM_GD160_PACKET_LENGTH);
    }
  }
}

void CMGD160Controller::SetSoftwareModeEnabled(bool value)
{
  uint8_t usb_buf[CM_GD160_PACKET_LENGTH];
  memset(usb_buf, 0x00, CM_GD160_PACKET_LENGTH);

  usb_buf[1] = 0x80;
  usb_buf[2] = 0x0B;
  usb_buf[3] = 0x02;
  usb_buf[4] = 0x01;
  usb_buf[6] = value;

  hid_write(dev, usb_buf, CM_GD160_PACKET_LENGTH);

  software_mode_enabled = value;
}
