#ifndef BLE_Hud_Service_H
#define BLE_Hud_Service_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

using namespace std;
static bool deviceConnected;

class BLE_Hud_Service
{
public:
    BLE_Hud_Service();
    void initserviceUUID(void);
    void createService(void);
    void hudconnected(void);
    bool deviceDisconnected;

private:
    BLEServer *pServer;
    BLEServer *pServiceUserData;
    BLEServer *pServiceDataService;
    // Service User data
    BLECharacteristic *pCharacteristic;
    BLECharacteristic *pCharacteristicSpeed;
    BLECharacteristic *pCharacteristicAltitude;
    BLECharacteristic *pCharacteristicTime;
    BLECharacteristic *pCharacteristicGps;
    BLECharacteristic *pCharacteristicPhoneBattery;
    BLECharacteristic *pCharacteristicWeatherTemp;
    // Service Data service
    BLECharacteristic *pCharacteristicSw;
    BLECharacteristic *pCharacteristicHw;
    BLECharacteristic *pCharacteristicBattery;
};

#endif