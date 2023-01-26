#include "BLE_Hud_Service.hpp"

BLE_Hud_Service ble_com_hud = BLE_Hud_Service();

void setup()
{
  Serial.begin(115200);
  ble_com_hud.initserviceUUID();
  ble_com_hud.createService();
}

void loop()
{
  ble_com_hud.hudconnected();
  delay(500);
}