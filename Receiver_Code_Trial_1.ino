#include <Arduino.h>
#include <ArduinoJson.h>

static char recv_buf[512];
static int led = 2;

static int at_send_check_response(char *p_ack, int timeout_ms, const char *p_cmd, ...)
{
    int ch;
    int index = 0;
    unsigned long startMillis;
    va_list args;
    memset(recv_buf, 0, sizeof(recv_buf));
    va_start(args, p_cmd);
    Serial2.printf(p_cmd, args);
    Serial.printf(p_cmd, args);
    va_end(args);
    delay(200);
    startMillis = millis();

    if (p_ack == NULL)
        return 0;

    do
    {
        while (Serial2.available() > 0)
        {
            ch = Serial2.read();
            recv_buf[index++] = ch;
            Serial.print((char)ch);
            delay(2);
        }

        if (strstr(recv_buf, p_ack) != NULL)
            return 1;

    } while (millis() - startMillis < timeout_ms);
    Serial.println();
    return 0;
}

static void recv_prase(char *p_msg)
{
    if (p_msg == NULL)
    {
        Serial.println("Received null");
        return;
    }
    char *p_start = NULL;
    char data[128]; // To hold the received bytes as characters

    int bytes_len = 0;
    p_start = strstr(p_msg, "RX");
    if (p_start && (1 == sscanf(p_start, "RX \"%s\"", data)))
    {
        for (int i = 0; i < sizeof(data); i++)
        {
            if (data[i + 1] == 0)
            {
                bytes_len = i;
                break;
            }
        }

        // Convert the characters to a byteArray
        int message_len = bytes_len / 2 + 1;
        byte out[message_len];
        auto getNum = [](char c) { return c > '9' ? c - 'A' + 10 : c - '0'; };
        for (int x = 0, y = 0; x < bytes_len; ++x, ++y)
            out[y] = (getNum(data[x++]) << 4) + getNum(data[x]);
        out[message_len] = '\0';

        // Print the received bytes
        // for (int i = 0; i < message_len - 1; i++)s
        // {
        //     Serial.print(out[i], HEX);
        //     Serial.print("-");
        // }
        // Serial.println();
    }
}

void setup(void)
{
    Serial.begin(9600);
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX on GPIO16, TX on GPIO17
    Serial.print("Serial2 LOCAL TEST\r\n");
    at_send_check_response("+AT: OK", 1000, "AT\r\n");
    at_send_check_response("+MODE: TEST", 2000, "AT+MODE=TEST\r\n");
    at_send_check_response("+TEST: RXLRPKT", 3000, "AT+TEST=RXLRPKT\r\n"); //For HEX
    //at_send_check_response("+TEST: RXLRPKT", 3000, "AT+TEST=RXLRPKT\r\n"); //For String
    delay(200);
    digitalWrite(led, HIGH);
}

void loop(void)
{
    char cmd[128];
    // Transmit HEX Values
    sprintf(cmd, "");
    int ret = at_send_check_response("+TEST: RX", 4000, "");
    if (ret)
        recv_prase(recv_buf);
    else
        Serial.println("Receive failed!\r\n\r\n");
    delay(500);
}
