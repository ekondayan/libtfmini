#ifndef TFMINI_DEFS_H
#define TFMINI_DEFS_H

namespace tfmini
{

    // Define the types used troughout the library
    typedef unsigned char uint8_t;
    typedef char int8_t;
    typedef unsigned short uint16_t;
    typedef short int16_t;

    // Structure to hold a measurement
    struct Measurement
    {
            uint16_t    reading         {0};        // Measured distance
            uint16_t    strength        {0};        // Strength of the beam
            bool        short_distance  {false};    // Distance mode
            bool        checksum        {false};
    };

    // Definition of the functions responsible for the low level send and receive
    typedef void (*send_t)(uint8_t device_id, const uint8_t *buffer, int16_t len);
    typedef void (*receive_t)(uint8_t device_id, uint8_t *buffer, int16_t len);

    // Configuration commands
    enum Command : uint8_t
    {
        CMD_OUTPUT_DATA_FORMAT = 0,
        CMD_OUTPUT_DATA_PERIOD,
        CMD_UNIT_OF_DISTANCE,
        CMD_DETECTION_PATTERN,
        CMD_DISTANCE_MODE,
        CMD_RANGE_LIMIT,
        CMD_SIGNAL_STRENGTH_LOW,
        CMD_SIGNAL_STRENGTH_HI,

        ADV_BAUD_RATE,
        ADV_TRIGGER_SOURCE,
        ADV_TRIGGER_EXTERNAL,
        ADV_RESET,

        ENTER_COMMAND_MODE,
        EXIT_COMMAND_MODE
    };

    enum OutputDataFormat : uint8_t
    {
        FORMAT_STANDARD = 0x01,
        FORMAT_PIXHAWK  = 0x04
    };

    enum DistanceUnit : uint8_t
    {
        UNIT_MM = 0x00,
        UNIT_CM = 0x01
    };

    enum DetectionPattern : uint8_t
    {
        DETECTION_AUTO = 0x00,
        DETECTION_FIX  = 0x01
    };

    enum DistanceMode : uint8_t
    {
        DISTANCE_SHORT_16X  = 0x00,
        DISTANCE_MIDDLE_16X = 0x03,
        DISTANCE_SHORT_15X  = 0x02,
        DISTANCE_LONG       = 0x07,
    };

    enum BaudRate : uint8_t
    {
        BAUD_9600   = 0x00,
        BAUD_14400  = 0x01,
        BAUD_19200  = 0x02,
        BAUD_38400  = 0x03,
        BAUD_56000  = 0x04,
        BAUD_57600  = 0x05,
        BAUD_115200 = 0x06,
        BAUD_128000 = 0x07,
        BAUD_230400 = 0x08,
        BAUD_256000 = 0x09,
        BAUD_460800 = 0x0A,
        BAUD_500000 = 0x0B,
        BAUD_512000 = 0x0C
    };

    enum TriggerSrc : uint8_t
    {
        TRIGGER_INT = 0x01,
        TRIGGER_EXT = 0x00
    };
}

#endif // TFMINI_DEFS_H