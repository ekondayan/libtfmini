/*
 *  _____ _____          _       _
 * |_   _|  ___| __ ___ (_)_ __ (_)
 *   | | | |_ | '_ ` _ \| | '_ \| |
 *   | | |  _|| | | | | | | | | | |
 *   |_| |_|  |_| |_| |_|_|_| |_|_|

 * C++17 header only, driver library for reading TFmini ToF LIDAR sensors
 * written in modern C++
 *
 * Version: 1.0.1
 * URL: https://github.com/ekondayan/libtfmini.git
 *
 * Copyright (c) 2019-2020 Emil Kondayan
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef BSP_TFMINI_H
#define BSP_TFMINI_H

#include <QSerialPort>

#include "../../src/tfmini.h"

namespace tfmini
{
    void send(uint8_t device_id, const uint8_t *buffer, int16_t len);
    void receive(uint8_t device_id, uint8_t *buffer, int16_t len);
}

class BSP_TFmini: public tfmini::TFmini
{
    public:
        explicit BSP_TFmini(const QString &port_name);

    private:
        QSerialPort m_port;

    private:
        friend void tfmini::send(uint8_t device_id, const uint8_t *buffer, int16_t len);
        friend void tfmini::receive(uint8_t device_id, uint8_t *buffer, int16_t len);
};

extern BSP_TFmini tf;

#endif // BSP_TFMINI_H
