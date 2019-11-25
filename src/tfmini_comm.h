#ifndef TFMINI_COMM_H
#define TFMINI_COMM_H

#include "tfmini_defs.h"

namespace tfmini
{
    class Comm
    {
        public:
            enum Status
            {
                STATUS_SUCCESS = 0x01,
                STATUS_ERROR_INSTRUCTION = 0xFF,
                STATUS_ERROR_PARAMETER = 0x0F,
                STATUS_ERROR_TRANSMISSION = 0x02,
            };

        protected:
            Comm(const Comm &&) = delete;
            Comm &operator=(const Comm &) = delete;

            Comm(tfmini::uint8_t device_id, send_t send, receive_t receive):
                m_device_id{device_id},
                m_phy_send{send},
                m_phy_receive{receive}
            {

            }

            ~Comm()
            {
                m_phy_receive = nullptr;
                m_phy_send = nullptr;
            }

        public:
            Comm::Status sendCommand(const uint8_t *cmd)
            {
                if(!m_phy_receive || !m_phy_send) return Comm::STATUS_ERROR_TRANSMISSION;

                uint8_t search_magic_header_retry = m_max_search_bytes;

                m_phy_send(m_device_id, cmd, 8);

                while(--search_magic_header_retry)
                {
                    uint8_t byte = 0;
                    m_phy_receive(m_device_id, &byte, 1);
                    // If first byte is not magic then continue searching
                    if(byte != 0x42) continue;

                    byte = 0;
                    m_phy_receive(m_device_id, &byte, 1);
                    // If second byte is not magic then continue searching
                    if(byte != 0x57) continue;

                    byte = 0;
                    m_phy_receive(m_device_id, &byte, 1);
                    // If third byte is not 0x02 then continue searching
                    if(byte != 0x02) continue;

                    byte = 0;
                    m_phy_receive(m_device_id, &byte, 1);

                    switch(byte)
                    {
                        case Status::STATUS_SUCCESS: return STATUS_SUCCESS;
                        case STATUS_ERROR_INSTRUCTION: return STATUS_ERROR_INSTRUCTION;
                        case STATUS_ERROR_PARAMETER: return STATUS_ERROR_PARAMETER;
                    }

                    break;
                }

                return Comm::STATUS_ERROR_TRANSMISSION;
            }

            bool readMeasure(tfmini::Measurement *measure)
            {
                if(!m_phy_receive || !m_phy_send) return false;

                uint8_t search_magic_header_retry = m_max_search_bytes;

                while(--search_magic_header_retry)
                {
                    uint8_t byte = 0;
                    m_phy_receive(m_device_id, &byte, 1);
                    // If first byte is not magic then continue searching
                    if(byte!=0x59) continue;

                    byte = 0;
                    m_phy_receive(m_device_id, &byte, 1);
                    // If second byte is not magic then continue searching
                    if(byte!=0x59) continue;

                    uint8_t reading[7];
                    // Read the rest of the command
                    m_phy_receive(m_device_id, reading, 7);

                    if(reading[0] == 0xFF && reading[1] == 0xFF) measure->reading = 0xFFFF;
                    else measure->reading = reading[0]+reading[1]*256;
                    measure->checksum = uint8_t(0x59+0x59+reading[0]+reading[1]+reading[2]+reading[3]+reading[4]+reading[5]) == reading[6];
                    measure->strength = reading[2]+reading[3]*256;
                    switch (DistanceMode(reading[4]))
                    {
                        case DISTANCE_SHORT_15X:
                        case DISTANCE_SHORT_16X:
                            measure->short_distance = true;
                            break;
                        case DISTANCE_LONG:
                        case DISTANCE_MIDDLE_16X:
                            measure->short_distance = false;
                            break;
                    }

                    // Invalid command checksum or invalid distance measure
                    if(!measure->checksum || measure->reading == 0xFFFF) return false;

                    return true;
                }
                return false;
            }

            uint16_t getMaxSearchBytes() const
            {
                return m_max_search_bytes;
            }

            void setMaxSearchBytes(const uint16_t &value)
            {
                m_max_search_bytes = value;
            }

        protected:
            uint8_t     m_device_id;
            send_t      m_phy_send;
            receive_t   m_phy_receive;
            uint16_t    m_max_search_bytes{50};
    };
}
#endif // TFMINI_COMM_H
