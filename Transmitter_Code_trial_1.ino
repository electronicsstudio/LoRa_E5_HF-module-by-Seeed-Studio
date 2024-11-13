#include "Arduino.h"
#include "uRTCLib.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

// Initialize the RTC
uRTCLib rtc(0x68);

// Initialize SoftwareSerial for LoRa-E5 module
SoftwareSerial e5(7, 6); // RX, TX

static char recv_buf[512];
//static int led = 13;

static int at_send_check_response(const char *p_ack, int timeout_ms, const char *p_cmd, ...)
{
    int ch;
    int index = 0;
    int startMillis = millis();
    va_list args;
    memset(recv_buf, 0, sizeof(recv_buf));
    va_start(args, p_cmd);
    e5.print(p_cmd);
    Serial.print("Command sent: ");
    Serial.println(p_cmd);
    va_end(args);
    delay(200);

    if (p_ack == NULL)
        return 0;

    do {
        while (e5.available() > 0) {
            ch = e5.read();
            recv_buf[index++] = ch;
            Serial.print((char)ch);
            delay(2);
        }

        if (strstr(recv_buf, p_ack) != NULL)
            return 1;

    } while (millis() - startMillis < timeout_ms);

    Serial.println();
    Serial.print("Received buffer: ");
    Serial.println(recv_buf);
    return 0;
}


void setup()
{
    Serial.begin(9600);
    
    // Initialize I2C communication for RTC
    URTCLIB_WIRE.begin();

    // Initialize LoRa-E5 communication
    e5.begin(9600);
    Serial.println("E5 LOCAL TEST");

    // Sending initialization commands to the LoRa-E5 module
    if (at_send_check_response("+AT: OK", 1000, "AT\r\n"))
    {
        Serial.println("AT command OK");
    }
    else
    {
        Serial.println("AT command failed");
    }

    if (at_send_check_response("+MODE: TEST", 1000, "AT+MODE=TEST\r\n"))
    {
        Serial.println("Mode set to TEST");
    }
    else
    {
        Serial.println("Failed to set mode to TEST");
    }

    delay(200);
}

void loop()
{
    char cmd[128];
    
    // Refresh RTC data
    rtc.refresh();

    // Get the current time from the RTC
    int hour = rtc.hour();
    int minute = rtc.minute();
    int second = rtc.second();

    //Serial.print("Current RTC Time: ");
    Serial.print(hour);
    Serial.print(minute);
    Serial.println(second);

    // Format the time as a string
    //sprintf(cmd, "AT+TEST=TXLRPKT,\"%02d:%02d:%02d\"\r\n", hour, minute, second);

    sprintf(cmd, "AT+TEST=TXLRPKT,,\"%02d%02d%02d111C1301D38247C7735D15264\"\r\n", hour, minute, second); //For HEX
    //sprintf(cmd, "AT+TEST=TXLRSTR,\"%02d%02d%02d001,1301.38247,7735.15264\"\r\n", hour, minute, second); //For String
    
    // Transmit the formatted time
    int ret = at_send_check_response("+TEST: TXLRPKT,", 5000, cmd); //For HEX
    //int ret = at_send_check_response("+TEST: TXLRSTR", 5000, cmd); //For String

    if (ret)
        Serial.println("Sent");
    else
        Serial.println("Send failed!");

    //delay(500); // Add a delay between transmissions            
}



//001,1301.38247,7735.15264




