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

#include <iostream>

#include "bsp_tfmini.h"

BSP_TFmini tf{"/dev/ttyUSB0"};

void tfmini::send(tfmini::uint8_t device_id, const tfmini::uint8_t *buffer, tfmini::int16_t len)
{
    Q_UNUSED(device_id)

    if(!tf.m_port.isOpen()) return;

    tf.m_port.write((const char*)buffer, len);
    while(tf.m_port.bytesToWrite() && tf.m_port.waitForBytesWritten(100));
}

void tfmini::receive(tfmini::uint8_t device_id, tfmini::uint8_t *buffer, tfmini::int16_t len)
{
    Q_UNUSED(device_id)

    if(!tf.m_port.isOpen()) return;

    int num_read = 0;
    do num_read += tf.m_port.read((char*)(buffer+num_read), len - num_read);
    while(num_read!=len && tf.m_port.waitForReadyRead(100));
}

BSP_TFmini::BSP_TFmini(const QString &port_name):
    tfmini::TFmini(0, &tfmini::send, &tfmini::receive),
    m_port(port_name)
{
    m_port.setBaudRate(QSerialPort::Baud115200);
    m_port.setDataBits(QSerialPort::Data8);
    m_port.setStopBits(QSerialPort::OneStop);
    m_port.setParity(QSerialPort::NoParity);

    if(!m_port.open(QIODevice::ReadWrite)) std::cout<<"Could not open the device:"<<m_port.portName().toLocal8Bit().constData()<<" ERROR:"<<m_port.errorString().toLocal8Bit().constData();
}
