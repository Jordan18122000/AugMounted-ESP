#include "BLE_Hud_Service.hpp"

// LED use for state connection BLE
#define LED_BUILTIN 2

// Define UART service UUID & Charactéristique UUID
#define AUGMOUNTED_SERVICE_UUID "3496c184-9e6b-456e-9ac5-42bb7e8db1a6" // UART service UUID

#define USER_DATA_SERVICE_UUID "16c20be6-87de-492b-9ca9-c4802f73450f" // UART service UUID
#define CHARACTERISTIC_UUID_SPEED "e8884cc8-89ad-442e-8133-c744b6c723b5"
#define CHARACTERISTIC_UUID_ALTITUDE "2bb580f0-cc1d-4a3a-8ce3-2110f8fdf7f1"
#define CHARACTERISTIC_UUID_TIME "bf4eeeb0-d5ca-49c3-9c8f-64767dde3444"
#define CHARACTERISTIC_UUID_GPS "44ab84ab-3d7b-4c14-bf62-26d2dabf156f"
#define CHARACTERISTIC_UUID_PHONE_BATT "260a7202-ce80-42be-8b13-ebf10e433c57"
#define CHARACTERISTIC_UUID_WEATHER_TEMP "bbbcc252-1068-43c4-b4ad-af0a78c44d69"

#define DEVICE_DATA_SERVICE_UUID "11ad4651-4704-427f-9514-55bdef0f09c4" // UART service UUID
#define CHARACTERISTIC_UUID_SW "22b58c42-60d2-4af4-ad43-86967f8a0c2b"
#define CHARACTERISTIC_UUID_HW "3ad8d4c7-547c-4ff0-a987-e7ce65e9da94"
#define CHARACTERISTIC_UUID_BATTERY "68b77095-ca71-4e71-a31a-477cef200b0e"

BLE_Hud_Service::BLE_Hud_Service()
{
    // Initialization of BLE servers and boolean variables
    pServer = NULL;
    pServiceUserData = NULL;
    pServiceDataService = NULL;
    deviceConnected = false;
    deviceDisconnected = false;
}

class MyServerCallbacks : public BLEServerCallbacks, public BLE_Hud_Service
{
    // Using the class MyServerCallbacks to use the following functions
    // to know if the client is connected to the hud or not.

    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        Serial.println("Client Connect");
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
        Serial.println("Client Disconnect");
    }
};

void BLE_Hud_Service::initserviceUUID(void)
{
    pinMode(LED_BUILTIN, OUTPUT); // Specify that LED pin is output
    Serial.println("1- Please scan for BLE devices in the app");
    Serial.println("2- Connect to AugMounted");

    // Create the BLE Device
    BLEDevice::init("AugMounted Pro");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
}

void BLE_Hud_Service::createService(void)
{

    // Assignment of characteristics to a service uuid.
    // Creation of the characteristic according to the uuid key and permissions
    // on this characteristic.

    BLEService *pServiceUserData = pServer->createService(USER_DATA_SERVICE_UUID);
    pCharacteristicSpeed = pServiceUserData->createCharacteristic(
        CHARACTERISTIC_UUID_SPEED,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicSpeed->addDescriptor(new BLE2902());

    pCharacteristicAltitude = pServiceUserData->createCharacteristic(
        CHARACTERISTIC_UUID_ALTITUDE,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicAltitude->addDescriptor(new BLE2902());

    pCharacteristicTime = pServiceUserData->createCharacteristic(
        CHARACTERISTIC_UUID_TIME,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicTime->addDescriptor(new BLE2902());

    pCharacteristicGps = pServiceUserData->createCharacteristic(
        CHARACTERISTIC_UUID_GPS,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicGps->addDescriptor(new BLE2902());

    pCharacteristicPhoneBattery = pServiceUserData->createCharacteristic(
        CHARACTERISTIC_UUID_PHONE_BATT,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicPhoneBattery->addDescriptor(new BLE2902());

    pCharacteristicWeatherTemp = pServiceUserData->createCharacteristic(
        CHARACTERISTIC_UUID_WEATHER_TEMP,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicWeatherTemp->addDescriptor(new BLE2902());
    pServiceUserData->start();

    BLEService *pServiceDataService = pServer->createService(DEVICE_DATA_SERVICE_UUID);
    pCharacteristicSw = pServiceDataService->createCharacteristic(
        CHARACTERISTIC_UUID_SW,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicSw->addDescriptor(new BLE2902());

    pCharacteristicHw = pServiceDataService->createCharacteristic(
        CHARACTERISTIC_UUID_HW,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicHw->addDescriptor(new BLE2902());

    pCharacteristicBattery = pServiceDataService->createCharacteristic(
        CHARACTERISTIC_UUID_BATTERY,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE |
            BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristicBattery->addDescriptor(new BLE2902());
    // Launch of the service
    pServiceDataService->start();

    pServer->getAdvertising()->start();
    Serial.println("Waiting a client connection to notify...");
}

void BLE_Hud_Service::hudconnected(void)
{
    // if the user is logged in, the data is processed
    if (deviceConnected && !deviceDisconnected)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        deviceDisconnected = deviceConnected;

        std::string agmntd_speed = pCharacteristicSpeed->getValue();
        std::string agmntd_altitude = pCharacteristicAltitude->getValue();
        std::string agmntd_time = pCharacteristicTime->getValue();
        std::string agmntd_gps = pCharacteristicGps->getValue();
        std::string agmntd_phone_batt = pCharacteristicPhoneBattery->getValue();
        std::string agmntd_weather_temp = pCharacteristicWeatherTemp->getValue();

        agmntd_speed = pCharacteristicSpeed->getValue();
        agmntd_altitude = pCharacteristicAltitude->getValue();
        agmntd_time = pCharacteristicTime->getValue();
        agmntd_gps = pCharacteristicGps->getValue();
        agmntd_phone_batt = pCharacteristicPhoneBattery->getValue();
        agmntd_weather_temp = pCharacteristicWeatherTemp->getValue();

        // Value created and assigned by hud :
        // Soft version
        pCharacteristicSw->setValue("V1.0.0");
        // Hard version
        pCharacteristicHw->setValue("V1.0.0");
        // State battery
        pCharacteristicBattery->setValue("53");

        // Speed management
        if (agmntd_speed.length() > 0)
        {
            Serial.println("*********");
            Serial.print("Speed :");
            for (int i = 0; i < agmntd_speed.length(); i++)
            {
                Serial.print(agmntd_speed[i]);
            }

            Serial.println();
            Serial.println("*********");
        }
        // Altitude management
        if (agmntd_altitude.length() > 0)
        {
            Serial.println("*********");
            Serial.print("Altitude :");
            for (int i = 0; i < agmntd_altitude.length(); i++)
            {
                Serial.print(agmntd_altitude[i]);
            }

            Serial.println();
            Serial.println("*********");
        }
        // Time management
        if (agmntd_time.length() > 0)
        {
            Serial.println("*********");
            Serial.print("Time :");
            for (int i = 0; i < agmntd_time.length(); i++)
            {
                Serial.print(agmntd_time[i]);
            }

            Serial.println();
            Serial.println("*********");
        }

        // GPS management
        if (agmntd_gps.length() > 0)
        {
            Serial.println("*********");
            Serial.print("GPS :");
            for (int i = 0; i < agmntd_gps.length(); i++)
            {
                Serial.print(agmntd_gps[i]);
            }

            Serial.println();
            Serial.println("*********");
        }

        // Phone battery management
        if (agmntd_phone_batt.length() > 0)
        {
            Serial.println("*********");
            Serial.print("Phone Battery :");
            for (int i = 0; i < agmntd_phone_batt.length(); i++)
            {
                Serial.print(agmntd_phone_batt[i]);
            }

            Serial.println();
            Serial.println("*********");
        }
        // Weather temp management
        if (agmntd_weather_temp.length() > 0)
        {
            Serial.println("*********");
            Serial.print("Weather Temp :");
            for (int i = 0; i < agmntd_weather_temp.length(); i++)
            {
                Serial.print(agmntd_weather_temp[i]);
            }

            Serial.println();
            Serial.println("*********");
        }
    }

    // if the user is deconnected --> restart advertising
    if (!deviceConnected && deviceDisconnected)
    {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("AugMounted is deconnected"); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising();                 // restart advertising
        Serial.println("AugMounted restart advertising");
        deviceDisconnected = deviceConnected;
    }
}