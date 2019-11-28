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

#include <iostream>

#include "bsp_tfmini.h"

BSP_TFmini tf[2]{{1},{2}};

namespace tfmini
{
    void send(tfmini::uint8_t device_id, const tfmini::uint8_t *buffer, tfmini::int16_t len)
    {
        if(!tf[device_id - 1].m_port.isOpen()) return;

        tf[device_id - 1].m_port.clearError();
        tf[device_id - 1].m_port.write((const char*)buffer, len);
        while(tf[device_id - 1].m_port.bytesToWrite() && tf[device_id - 1].m_port.waitForBytesWritten(100));
    }

    void receive(tfmini::uint8_t device_id, tfmini::uint8_t *buffer, tfmini::int16_t len)
    {
        if(!tf[device_id - 1].m_port.isOpen()) return;

        tf[device_id - 1].m_port.clearError();
        int num_read = 0;
        do num_read += tf[device_id - 1].m_port.read((char*)(buffer + num_read), len - num_read);
        while(num_read != len && tf[device_id - 1].m_port.waitForReadyRead(100));
    }
}

BSP_TFmini::BSP_TFmini(tfmini::uint8_t device_id):
    tfmini::TFmini(device_id, &tfmini::send, &tfmini::receive)
{
    m_port.setBaudRate(QSerialPort::Baud115200);
    m_port.setDataBits(QSerialPort::Data8);
    m_port.setStopBits(QSerialPort::OneStop);
    m_port.setParity(QSerialPort::NoParity);

    m_port.setReadBufferSize(500);
}

void BSP_TFmini::openPort(const QString &port)
{
    if(m_port.isOpen())
        m_port.close();

    m_port.setPortName(port);

    if(!m_port.open(QIODevice::ReadWrite))
        std::cout<<"Could not open the device:" <<
                   m_port.portName().toLocal8Bit().constData() <<
                   " ERROR:" <<
                   m_port.errorString().toLocal8Bit().constData();
}
