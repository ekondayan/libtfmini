#ifndef TFMINI_H
#define TFMINI_H

#include "tfmini_comm.h"

namespace tfmini
{
    class TFmini: public Comm
    {
        protected:
            TFmini(const TFmini &&) = delete;
            TFmini&operator=(const TFmini &) = delete;

            TFmini(uint8_t device_id, send_t send, receive_t receive):
                Comm{device_id, send, receive}
            {
            }

        public:

            Comm::Status setOutputDataFormat(const OutputDataFormat format)
            {
                m_command_list[CMD_OUTPUT_DATA_FORMAT][6] = format;
                return runCmd(m_command_list[CMD_OUTPUT_DATA_FORMAT]);
            }

            Comm::Status setOutputPeriod(const uint16_t period_ms)
            {
                if( (period_ms % 10) == 0)
                {
                    m_command_list[CMD_OUTPUT_DATA_PERIOD][4] = period_ms & 0x00FF;
                    m_command_list[CMD_OUTPUT_DATA_PERIOD][5] = (period_ms & 0xFF00)>>8;
                }

                return runCmd(m_command_list[CMD_OUTPUT_DATA_PERIOD]);
            }

            Comm::Status setDistanceUnit(const DistanceUnit unit)
            {
                m_command_list[CMD_UNIT_OF_DISTANCE][6] = unit;
                return runCmd(m_command_list[CMD_UNIT_OF_DISTANCE]);
            }

            Comm::Status setDetectionPattern(const DetectionPattern pattern)
            {
                m_command_list[CMD_DETECTION_PATTERN][6] = pattern;
                return runCmd(m_command_list[CMD_DETECTION_PATTERN]);
            }

            Comm::Status setDistanceMode(const DistanceMode mode)
            {

                if(setDetectionPattern(DETECTION_FIX) != STATUS_SUCCESS) return STATUS_ERROR_TRANSMISSION;

                m_command_list[CMD_DISTANCE_MODE][6] = mode;
                return runCmd(m_command_list[CMD_DISTANCE_MODE]);
            }

            Comm::Status setRangeLimit(const uint16_t range_mm)
            {
                if(range_mm >= 300 && range_mm <= 12000)
                {
                    m_command_list[CMD_RANGE_LIMIT][4] = range_mm & 0x00FF;
                    m_command_list[CMD_RANGE_LIMIT][5] = (range_mm & 0xFF00)>>8;
                    m_command_list[CMD_RANGE_LIMIT][6] = 0x01;
                }
                else if(range_mm == 0)
                {
                    m_command_list[CMD_RANGE_LIMIT][4] = 0x00;
                    m_command_list[CMD_RANGE_LIMIT][5] = 0x00;
                    m_command_list[CMD_RANGE_LIMIT][6] = 0x00;
                }

                return runCmd(m_command_list[CMD_RANGE_LIMIT]);
            }

            Comm::Status setSignalStrengthLow(const uint8_t low_threshold)
            {
                if(low_threshold <= 80) m_command_list[CMD_SIGNAL_STRENGTH_LOW][4] = low_threshold;
                return runCmd(m_command_list[CMD_SIGNAL_STRENGTH_LOW]);
            }

            Comm::Status setSignalStrengthHi(const uint16_t hi_threshold)
            {
                if(hi_threshold <= 3000)
                {
                    m_command_list[CMD_SIGNAL_STRENGTH_HI][4] = hi_threshold & 0x00FF;
                    m_command_list[CMD_SIGNAL_STRENGTH_HI][5] = (hi_threshold & 0xFF00)>>8;
                }
                return runCmd(m_command_list[CMD_SIGNAL_STRENGTH_HI]);
            }

            Comm::Status setBaudRate(const BaudRate br)
            {
                m_command_list[ADV_BAUD_RATE][6] = br;
                return runCmd(m_command_list[ADV_BAUD_RATE]);
            }

            Comm::Status setTriggerSrc(const TriggerSrc trigger)
            {
                m_command_list[ADV_TRIGGER_SOURCE][6] = trigger;
                return runCmd(m_command_list[ADV_TRIGGER_SOURCE]);
            }

            Comm::Status triggerMeasurement()
            {
                return runCmd(m_command_list[ADV_TRIGGER_EXTERNAL]);
            }

            Comm::Status reset()
            {
                return runCmd(m_command_list[ADV_RESET]);
            }

        private:
            Comm::Status runCmd(uint8_t *cmd)
            {
                for(int i = 0; i < 3; ++i)
                {
                    if(sendCommand(m_command_list[ENTER_COMMAND_MODE]) == STATUS_SUCCESS)
                    {
                        Comm::Status status = sendCommand(cmd);

                        if(  cmd == m_command_list[ADV_BAUD_RATE] || cmd == m_command_list[ADV_TRIGGER_EXTERNAL] ||
                             cmd == m_command_list[ADV_RESET]     || cmd == m_command_list[ADV_TRIGGER_SOURCE])
                            return Comm::STATUS_SUCCESS;

                        sendCommand(m_command_list[EXIT_COMMAND_MODE]);
                        return status;
                    }
                }

                return Comm::STATUS_ERROR_TRANSMISSION;
            }

        private:
            static inline uint8_t m_command_list[][8]
            {
                {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x01, 0x06},
                {0x42, 0x57, 0x02, 0x00, 0x0A, 0x00, 0x00, 0x07},
                {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x01, 0x1A},
                {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x00, 0x14},
                {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x00, 0x11},
                {0x42, 0x57, 0x02, 0x00, 0xE0, 0x2E, 0x01, 0x19},
                {0x42, 0x57, 0x02, 0x00, 0x14, 0x00, 0x00, 0x20},
                {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x00, 0x21},

                {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x00, 0x08},
                {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x00, 0x40},
                {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x00, 0x41},
                {0x42, 0x57, 0x02, 0x00, 0xFF, 0xFF, 0xFF, 0xFF},

                {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x01, 0x02},
                {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02}
            };
    };

}

#endif // TFMINI_H
