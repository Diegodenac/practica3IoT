#ifndef SYSTEM_ALARM_H
#define SYSTEM_ALARM_H

#include "ClientManager.h"
#include <ArduinoJson.h>

class SystemAlarm
{
private:
    ClientManager* networkManager;
    unsigned char activeState;
    unsigned char buzzerState;
    unsigned char movementDetected;
    unsigned char movementDetectorPin;
    unsigned char buzzerPin;
    StaticJsonDocument<JSON_OBJECT_SIZE(4)> outputDoc;
    char outputBuffer[128];
public:
    SystemAlarm(ClientManager* manager, unsigned char attachDetectorPin, unsigned char attachBuzzerPin)
      : networkManager(manager),
        movementDetectorPin(attachDetectorPin),
        buzzerPin(attachBuzzerPin),
        activeState(0),
        buzzerState(0),
        movementDetected(0) {
    }
    void reportDetection(){
        outputDoc["state"]["reported"]["intruderDetected"] = movementDetected;
        serializeJson(outputDoc, outputBuffer);
        networkManager->publish(outputBuffer);
    }
    void reportActivation(){
        outputDoc["state"]["reported"]["alarmActive"] = activeState;
        serializeJson(outputDoc, outputBuffer);
        networkManager->publish(outputBuffer);
    }
    void reportBuzzerActivation(){
        outputDoc["state"]["reported"]["buzzerActive"] = buzzerState;
        serializeJson(outputDoc, outputBuffer);
        networkManager->publish(outputBuffer);
    }
    void setActivation(byte payloadAlarm){
        if(activeState != payloadAlarm){
            activeState = payloadAlarm;
            movementDetected = activeState ? movementDetected : 0;
            reportDetection();
            reportActivation();
            Serial.println(buzzerState);
        }
    }
    void setBuzzerActivation(byte payloadBuzzer){
        if(buzzerState != payloadBuzzer){
            buzzerState = payloadBuzzer;
            buzzerState ? digitalWrite(buzzerPin, HIGH) : digitalWrite(buzzerPin, LOW);
            Serial.println(buzzerState);
            reportBuzzerActivation();
        }
    }
    void readDetection(){
        if(activeState){
            if(!movementDetected){
                if(digitalRead(movementDetectorPin) == HIGH){
                    digitalWrite(buzzerPin, HIGH);
                    movementDetected = 1;
                    reportDetection();
                }
            }
            else{
              digitalWrite(buzzerPin, HIGH);
            }
        }
    }
};

#endif
