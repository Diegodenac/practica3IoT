#include "SystemAlarm.h"
#include "ClientManager.h"
#include <ArduinoJson.h>

// Replace with your network credentials
const char* WIFI_SSID = "CANEDO";
const char* WIFI_PASS = "dgm140897";

// Replace with your MQTT broker details
const char* MQTT_BROKER = "a2gyx6o3r72s2g-ats.iot.us-east-1.amazonaws.com";
const int MQTT_PORT = 8883;
const char* CLIENT_ID = "ESP-32";

const char AMAZON_ROOT_CA1[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

const char CERTIFICATE[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAMPzSP2dQBpaFxCt4ee6ihlHfesvMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDExMDYyMTQ1
MTZaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDGk3MbZ2L3YMZ/fEgu
qPxZW4NpppvLMK0Q88MGmKvd2li1zOjaJzhL4J4ICy09mjgkTT1ALc0hq88HjXqa
G+JD2LL0uilTGsvl0HnqNGjFzwgwLMVlU+5c6DM/iBA6kgJflPMlXJ+RNcpRrUdK
zL8iHZcbLCsykiMErS+B8hXvG56TnWfXEp6edCTqlUbZpwTU9v8eJjwnbUub4hah
xExlx+JoRAHEuM+ceHvdSKZRl+XX4yEhTHJnmCW42WWOCMd6ihiBBhRG1JrZCnWh
xjvZYSurLur88qO8/C4H2UR/l8qdeTObbWDC2ut6nz0JN3w5WjqNnDh5k5suIjsy
nRZbAgMBAAGjYDBeMB8GA1UdIwQYMBaAFEiWkhZf/y4h0+KoJul1fCi3WnLAMB0G
A1UdDgQWBBR++/vgjnJwNwOhVXDw08yWsdN4GTAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAJVFIW0zdKu5qCvLzvKvEigjj
DnxZPfTBOAgqkFX95h+jkOHwkeQ+rGa8RU/23LWPEoT5XniYlVmrHJsbVUK6KLCx
ImVxRoozTU6iUEZsTVtmuay0ft72zppVE04vj+hjrlbZ+1jv7J/m0ru/vGlYlHtd
Kyadz0ffuJUZtyxA5oJs0zpAWK+1LwbqNcw9Pe0HzNKyS5LuhXh0L4Qzu9cYfeBk
Qwn+BlqZrL/oyiOp4mwmoPSUo42SxQCJbE4itFL6MaB+EulFsn48Yn4lrrP7ltzf
6lCJd9oFjmNUFVkDrWz55FrxHQzuxzYj3tVfEDmqQ876SwP1Ub29YGS2k6jubw==
-----END CERTIFICATE-----
)KEY";

const char PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAxpNzG2di92DGf3xILqj8WVuDaaabyzCtEPPDBpir3dpYtczo
2ic4S+CeCAstPZo4JE09QC3NIavPB416mhviQ9iy9LopUxrL5dB56jRoxc8IMCzF
ZVPuXOgzP4gQOpICX5TzJVyfkTXKUa1HSsy/Ih2XGywrMpIjBK0vgfIV7xuek51n
1xKennQk6pVG2acE1Pb/HiY8J21Lm+IWocRMZcfiaEQBxLjPnHh73UimUZfl1+Mh
IUxyZ5gluNlljgjHeooYgQYURtSa2Qp1ocY72WErqy7q/PKjvPwuB9lEf5fKnXkz
m21gwtrrep89CTd8OVo6jZw4eZObLiI7Mp0WWwIDAQABAoIBAASTRSWufiNBAZHS
ZT9d+WJ9r0tx4UkPOA2Z8DZosh0MeNrHe9365VsqM7nvt/0hXoXHmc+uvnk2IjEE
W3zNg6sYhRcKawVVKrmC0ixJRNi56nBht7Kb+nP9jaC7G9SdpJxbLDcCKVMJMD4t
heT9TrIUiHTB0cT6hQAIlX2i/EpkCCDFfHOWuq8QLfNB03lesSEIpdQLzxoi/B3J
TYRo/DD6ws3VObrleu3D5/MR36lJoM3Z64pvZXOMbkDJWM+wIOcta8M6yVawb5Q4
sqsNScBwHKl6zVm6BardJLtwCOgAZJrgaTZltA6anooB5Zn8hzWcj+PXr/jNyRCI
faPN2MECgYEA5twRjqHihLXvtaYg6fcGtQcX38SPk/+FMUHOdH3uLebMQuTdOVTx
vdeHALIVycRHSPMPYJmN8kOWFifBYGK6CtxDXpECbQ0Tu2Z84zqiaTtdp/66wufo
sYurUaZLTQsAolv9jgezJZe4fwuL45xBrUTEe9gKYoqaz6Lhzsg54tECgYEA3DNf
j8kpnNNZM1jWOx1B9wN7oMIVnVCUpetMYGiVQRZjE7bvgN8gnUGUn5+H2ElIHtjW
OolH2COwYLzm7D3MF/JPuM9exHmJjvnLz3p5eM8z0LZgsEzAzx4wt6mlpLcq4+MM
anD8nMT1eN+S6DHwhQOK+qgUEjSF3yf6i49u+WsCgYBIzgZLcQo6yBW8t5ORtXHM
DVyeGSMejWq3ygUN/9jpaOvp/Ls3pW5HbH0K3x637sp8Wwryb/qv+FiQzCDUAM6j
NiuY2ypksccTRpMEJi2p+irjHBZR+UkNFPsGWOdOeLXOfixMamk45cwtZLrNIS4l
Mpa0Z8F+qi+AY+HKkKKQwQKBgHirRnD7Ieyo/TbgNfYLIH0G0ZbVBRdCc1NBUId8
iF40sHK7SyBqJetHZvTabY0fC4Cr/ynw1Qgi9qLsofdQHin5RarXKO2uVK2fQgse
6LZFhfWYHABA72ARAU2EkBtlrQFZCRLDfAtE++gTTOQlidK6kEywEt+vHLWfqPMV
1RClAoGBAKXPKASt8nV27SZZHb0AYJEwcJN9dhIOZfQAWOFC+4pnbbpWgzrSLqtb
6gldgIra00Q5h1pd0AotC/z9tq3K0J2AaPYVZN4BUgyLQzDqRhYoMxyiimlKcSVN
hMz+UEezZ5BgzbmZPOu3gAJTBrB/lvQNoreDe/0E++OxCKBlvMzV
-----END RSA PRIVATE KEY-----
)KEY";

const char* UPDATE_TOPIC = "$aws/things/intruder_alarm/shadow/update";              // publish
const char* UPDATE_DELTA_TOPIC = "$aws/things/intruder_alarm/shadow/update/delta";  // subscribe

// Pin definitions
const int MOVEMENT_DETECTOR_PIN = 14;
const int BUZZER_PIN = 12;

//objetct instance
ClientManager* networkManager = new ClientManager(WIFI_SSID, WIFI_PASS, MQTT_BROKER, MQTT_PORT, CLIENT_ID, UPDATE_TOPIC, UPDATE_DELTA_TOPIC);
SystemAlarm systemAlarm(networkManager, MOVEMENT_DETECTOR_PIN, BUZZER_PIN);
// Callback function to handle messages received from the subscribed topic
byte payloadAlarm = 0;
byte payloadBuzzer = 0;
StaticJsonDocument<JSON_OBJECT_SIZE(64)> inputDoc;

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) message += String((char) payload[i]);
  Serial.println("Message from topic " + String(topic) + ":" + message);
  DeserializationError err = deserializeJson(inputDoc, payload);
  if (!err) {
    if (String(topic) == UPDATE_DELTA_TOPIC) {
      if (inputDoc["state"].containsKey("alarmActive")) {
        systemAlarm.setActivation(inputDoc["state"]["alarmActive"].as<int8_t>());
      }
      if (inputDoc["state"].containsKey("buzzerActive")) {
        systemAlarm.setBuzzerActivation(inputDoc["state"]["buzzerActive"].as<int8_t>());
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(MOVEMENT_DETECTOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  networkManager->setupWiFi();
  networkManager->setUpCredentials(AMAZON_ROOT_CA1, CERTIFICATE, PRIVATE_KEY);
  networkManager->setMqttConnection(callback);
}

void loop() {
  if (!networkManager->isConeected()) {
    networkManager->reconnect();
    systemAlarm.reportActivation();
    systemAlarm.reportBuzzerActivation();
    systemAlarm.reportDetection();
  }
  systemAlarm.readDetection();
  networkManager->listenMessages();
}
