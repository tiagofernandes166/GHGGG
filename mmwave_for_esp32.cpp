#include "mmwave_for_esp32.h"

// Adjust buffer size for ESP32
#define BUFFER_SIZE 256

// Constructor with one serial port
Seeed_HSP24::Seeed_HSP24(HardwareSerial &serial)
    : _serial(&serial),
    _debugSerial(nullptr),
    bufferIndex(0),
    receiveStartTime(0),
    isInATMode(0),
    bufferIndex_hsp24(0)
{
}

// Constructor with two serial ports
Seeed_HSP24::Seeed_HSP24(HardwareSerial &serial, HardwareSerial &debugSerial)
    : _serial(&serial),
    _debugSerial(&debugSerial),
    bufferIndex(0),
    receiveStartTime(0),
    isInATMode(0),
    bufferIndex_hsp24(0)
{
}

// Enter AT mode function
int Seeed_HSP24::enterATMode()
{
    _serial->print("+++");

    unsigned long startTime = millis();

    while (true)
    {
        while (_serial->available())
        {
            char receivedChar = _serial->read();
            buffer[bufferIndex] = receivedChar;
            bufferIndex++;
            receiveStartTime = millis();
        }

        if (bufferIndex > 0 && (bufferIndex >= BUFFER_SIZE || millis() - receiveStartTime >= RECEIVE_TIMEOUT))
        {
            if (this->bufferIndex == 1 && buffer[0] == 'a')
            {
                _serial->print('a');
            }
            else if (buffer[0] == 'O' && buffer[1] == 'K')
            {
                if (_debugSerial != nullptr)
                {
                    _debugSerial->println("Enter AT Mode Success!");
                }

                isInATMode = 1;
                memset(this->buffer, 0, BUFFER_SIZE);
                this->bufferIndex = 0;
                return 1;
            }
            else
            {
                memset(this->buffer, 0, BUFFER_SIZE);
                this->bufferIndex = 0;
                return 0;
            }
            memset(this->buffer, 0, BUFFER_SIZE);
            this->bufferIndex = 0;
        }

        if (millis() - startTime > 5000)
        {
            return 0;
        }
    }

    return 0;
}

// Exit AT mode function
int Seeed_HSP24::exitATMode()
{
    _serial->println("at+reconn=1");

    unsigned long startTime = millis();

    while (true)
    {
        while (_serial->available())
        {
            char receivedChar = _serial->read();
            buffer[bufferIndex] = receivedChar;
            bufferIndex++;
            receiveStartTime = millis();
        }

        if (bufferIndex > 0 && (bufferIndex >= BUFFER_SIZE || millis() - receiveStartTime >= RECEIVE_TIMEOUT))
        {
            char *found = strstr(this->buffer, "ok");
            if (found != NULL)
            {
                if (_debugSerial != nullptr)
                {
                    _debugSerial->println("Setting Success!");
                }

                memset(this->buffer, 0, BUFFER_SIZE);
                this->bufferIndex = 0;
                return 1;
            }
            else
            {
                if (_debugSerial != nullptr)
                {
                    for (int i = 0; i < this->bufferIndex; i++)
                    {
                        _debugSerial->print(buffer[i]);
                    }
                }

                memset(this->buffer, 0, BUFFER_SIZE);
                this->bufferIndex = 0;
                return 0;
            }
        }

        if (millis() - startTime > 3000)
        {
            return 0;
        }
    }
}

// Check buffer function
int Seeed_HSP24::checkBuffer()
{
    unsigned long startTime = millis();

    while (true)
    {
        while (_serial->available())
        {
            char receivedChar = _serial->read();
            buffer[bufferIndex] = receivedChar;
            bufferIndex++;
            receiveStartTime = millis();
        }

        if (bufferIndex > 0 && (bufferIndex >= BUFFER_SIZE || millis() - receiveStartTime >= RECEIVE_TIMEOUT))
        {
            char *found = strstr(this->buffer, "ok");
            if (found != NULL)
            {
                if (_debugSerial != nullptr)
                {
                    _debugSerial->println("Setting Success!");
                }

                memset(this->buffer, 0, BUFFER_SIZE);
                this->bufferIndex = 0;
                return 1;
            }
            else
            {
                if (_debugSerial != nullptr)
                {
                    for (int i = 0; i < this->bufferIndex; i++)
                    {
                        _debugSerial->print(buffer[i]);
                    }
                }

                memset(this->buffer, 0, BUFFER_SIZE);
                this->bufferIndex = 0;
                return 0;
            }
        }

        if (millis() - startTime > 3000)
        {
            return 0;
        }
    }
}

// Send AT command with exit function
int Seeed_HSP24::sendATCommandWithExit(String command)
{
    _serial->println(command);
    int ret = checkBuffer();
    exitATMode();
    return ret;
}

// Send AT command function
int Seeed_HSP24::sendATCommand(String command)
{
    _serial->println(command);
    int ret = checkBuffer();
    return ret;
}

// Get version function
int Seeed_HSP24::getVer()
{
    enterATMode();
    return sendATCommandWithExit("at+ver=?");
}

// Get radar status function
Seeed_HSP24::RadarStatus Seeed_HSP24::getStatus()
{
    bufferIndex_hsp24 = 0;
    Seeed_HSP24::RadarStatus radarStatus;
    const int MIN_FRAME_LENGTH_BASE = 23;
    const int MIN_FRAME_LENGTH_ENGINEERING = 45;

    while (_serial->available() && bufferIndex_hsp24 < BUFFER_SIZE)
    {
        buffer_hsp24[bufferIndex_hsp24] = _serial->read();
        bufferIndex_hsp24++;

        int startIndex = findSequence(buffer_hsp24, bufferIndex_hsp24, frameStart, 4);
        int endIndex = findSequence(buffer_hsp24, bufferIndex_hsp24, frameEnd, 4);

        if ((endIndex + 4 - startIndex) == MIN_FRAME_LENGTH_BASE)
        {
            uint8_t tmp_buffer[(endIndex + 4) - startIndex];
            for (int i = startIndex; i < endIndex + 4; i++)
            {
                tmp_buffer[i] = buffer_hsp24[i];
            }

            int tmp_bufferSize = sizeof(tmp_buffer);

            int mode = tmp_buffer[6];
            radarStatus.radarMode = mode;
            if(mode != 2) return radarStatus;
            if(tmp_buffer[7] != 0xAA) return radarStatus;
            if(tmp_buffer[17] != 0x55) return radarStatus;
            if(tmp_buffer[18] != 0x00) return radarStatus;

            if (_debugSerial != nullptr)
            {
                _debugSerial->println("sensor status: " + String(tmp_buffer[8]));
            }
            status = static_cast<Seeed_HSP24::TargetStatus>(tmp_buffer[8]);
            radarStatus.targetStatus = static_cast<Seeed_HSP24::TargetStatus>(tmp_buffer[8]);

            int distance = tmp_buffer[15] | (tmp_buffer[16] << 8);
            if (_debugSerial != nullptr)
            {
                _debugSerial->println("distance: " + String(distance));
            }
            radarStatus.distance = distance;

            if (_debugSerial != nullptr)
            {
                _debugSerial->print("nature: ");
                for (int i = 0; i < sizeof(tmp_buffer); i++)
                {
                    if (tmp_buffer[i] < 0x10)
                        _debugSerial->print("0");
                    _debugSerial->print(tmp_buffer[i], HEX);
                    _debugSerial->print(" ");
                }
                _debugSerial->println();
            }

            int bytesToMove = bufferIndex_hsp24 - (endIndex + 4);
            for (int i = 0; i < bytesToMove; i++)
            {
                buffer_hsp24[i] = buffer_hsp24[endIndex + 4 + i];
            }
            bufferIndex_hsp24 = bytesToMove;
        }
        else if ((endIndex + 4 - startIndex) == MIN_FRAME_LENGTH_ENGINEERING)
        {
            uint8_t tmp_buffer[(endIndex + 4) - startIndex];
            for (int i = startIndex; i < endIndex + 4; i++)
            {
                tmp_buffer[i] = buffer_hsp24[i];
            }

            int tmp_bufferSize = sizeof(tmp_buffer);

            int mode = tmp_buffer[6];
            radarStatus.radarMode = mode;

            if(mode != 2) return radarStatus;
            if(tmp_buffer[7] != 0xAA) return radarStatus;
            if(tmp_buffer[17] != 0x55) return radarStatus;
            if(tmp_buffer[18] != 0x00) return radarStatus;

            if (_debugSerial != nullptr)
            {
                _debugSerial->println("sensor status: " + String(tmp_buffer[8]));
            }
            status = static_cast<Seeed_HSP24::TargetStatus>(tmp_buffer[8]);
            radarStatus.targetStatus = static_cast<Seeed_HSP24::TargetStatus>(tmp_buffer[8]);

            int distance = tmp_buffer[15] | (tmp_buffer[16] << 8);
            if (_debugSerial != nullptr)
            {
                _debugSerial->println("distance: " + String(distance));
            }
            radarStatus.distance = distance;

            if (_debugSerial != nullptr)
            {
                _debugSerial->print("nature: ");
                for (int i = 0; i < sizeof(tmp_buffer); i++)
                {
                    if (tmp_buffer[i] < 0x10)
                        _debugSerial->print("0");
                    _debugSerial->print(tmp_buffer[i], HEX);
                    _debugSerial->print(" ");
                }
                _debugSerial->println();
            }

            int bytesToMove = bufferIndex_hsp24 - (endIndex + 4);
            for (int i = 0; i < bytesToMove; i++)
            {
                buffer_hsp24[i] = buffer_hsp24[endIndex + 4 + i];
            }
            bufferIndex_hsp24 = bytesToMove;
        }
    }

    return radarStatus;
}

// Find sequence function
int Seeed_HSP24::findSequence(const uint8_t* data, int dataSize, const uint8_t* sequence, int sequenceSize)
{
    for (int i = 0; i <= dataSize - sequenceSize; i++)
    {
        bool match = true;
        for (int j = 0; j < sequenceSize; j++)
        {
            if (data[i + j] != sequence[j])
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            return i;
        }
    }
    return -1;
}
