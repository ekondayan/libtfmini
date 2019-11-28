/*
 *  _____ _____          _       _
 * |_   _|  ___| __ ___ (_)_ __ (_)
 *   | | | |_ | '_ ` _ \| | '_ \| |
 *   | | |  _|| | | | | | | | | | |
 *   |_| |_|  |_| |_| |_|_|_| |_|_|

 * C++17 header only, driver library for reading TFmini ToF LIDAR sensors
 * written in modern C++
 *
 * Version: 1.0.2
 * URL: https://github.com/ekondayan/libtfmini.git
 *
 * Copyright (c) 2019 Emil Kondayan
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
                return execCmd(m_command_list[CMD_OUTPUT_DATA_FORMAT]);
            }

            Comm::Status setOutputPeriod(const uint16_t period_ms)
            {
                if( (period_ms % 10) == 0)
                {
                    m_command_list[CMD_OUTPUT_DATA_PERIOD][4] = period_ms & 0x00FF;
                    m_command_list[CMD_OUTPUT_DATA_PERIOD][5] = (period_ms & 0xFF00)>>8;
                    return execCmd(m_command_list[CMD_OUTPUT_DATA_PERIOD]);
                }

                return STATUS_ERROR_PARAMETER;
            }

            Comm::Status setDistanceUnit(const DistanceUnit unit)
            {
                m_command_list[CMD_UNIT_OF_DISTANCE][6] = unit;
                return execCmd(m_command_list[CMD_UNIT_OF_DISTANCE]);
            }

            Comm::Status setDetectionPattern(const DetectionPattern pattern)
            {
                m_command_list[CMD_DETECTION_PATTERN][6] = pattern;
                return execCmd(m_command_list[CMD_DETECTION_PATTERN]);
            }

            Comm::Status setDistanceMode(const DistanceMode mode)
            {
                if(setDetectionPattern(DETECTION_FIX) != STATUS_SUCCESS)
                    return STATUS_ERROR_TRANSMISSION;

                m_command_list[CMD_DISTANCE_MODE][6] = mode;
                return execCmd(m_command_list[CMD_DISTANCE_MODE]);
            }

            Comm::Status setRangeLimit(const uint16_t range_mm)
            {
                if(range_mm >= 300 && range_mm <= 12000)
                {
                    m_command_list[CMD_RANGE_LIMIT][4] = range_mm & 0x00FF;
                    m_command_list[CMD_RANGE_LIMIT][5] = (range_mm & 0xFF00)>>8;
                    m_command_list[CMD_RANGE_LIMIT][6] = 0x01;
                    return execCmd(m_command_list[CMD_RANGE_LIMIT]);
                }
                else if(range_mm == 0)
                {
                    m_command_list[CMD_RANGE_LIMIT][4] = 0x00;
                    m_command_list[CMD_RANGE_LIMIT][5] = 0x00;
                    m_command_list[CMD_RANGE_LIMIT][6] = 0x00;
                    return execCmd(m_command_list[CMD_RANGE_LIMIT]);
                }

                return STATUS_ERROR_PARAMETER;
            }

            Comm::Status setSignalStrengthLow(const uint8_t low_threshold)
            {
                if(low_threshold <= 80)
                {
                    m_command_list[CMD_SIGNAL_STRENGTH_LOW][4] = low_threshold;
                    return execCmd(m_command_list[CMD_SIGNAL_STRENGTH_LOW]);
                }

                return STATUS_ERROR_PARAMETER;
            }

            Comm::Status setSignalStrengthHi(const uint16_t hi_threshold)
            {
                if(hi_threshold <= 3000)
                {
                    m_command_list[CMD_SIGNAL_STRENGTH_HI][4] = hi_threshold & 0x00FF;
                    m_command_list[CMD_SIGNAL_STRENGTH_HI][5] = (hi_threshold & 0xFF00)>>8;
                    return execCmd(m_command_list[CMD_SIGNAL_STRENGTH_HI]);
                }

                return STATUS_ERROR_PARAMETER;
            }

            Comm::Status setBaudRate(const BaudRate br)
            {
                m_command_list[ADV_BAUD_RATE][6] = br;
                return execCmd(m_command_list[ADV_BAUD_RATE]);
            }

            Comm::Status setTriggerSrc(const TriggerSrc trigger)
            {
                m_command_list[ADV_TRIGGER_SOURCE][6] = trigger;
                return execCmd(m_command_list[ADV_TRIGGER_SOURCE]);
            }

            Comm::Status triggerMeasurement()
            {
                return execCmd(m_command_list[ADV_TRIGGER_EXTERNAL]);
            }

            Comm::Status reset()
            {
                return execCmd(m_command_list[ADV_RESET]);
            }

        protected:
            Comm::Status execCmd(uint8_t *cmd)
            {
                if(cmd == nullptr)
                    return Comm::STATUS_ERROR_TRANSMISSION;

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
