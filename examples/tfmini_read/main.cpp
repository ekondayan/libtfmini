/*
 *  _____ _____          _       _
 * |_   _|  ___| __ ___ (_)_ __ (_)
 *   | | | |_ | '_ ` _ \| | '_ \| |
 *   | | |  _|| | | | | | | | | | |
 *   |_| |_|  |_| |_| |_|_|_| |_|_|

 * C++17 header only, driver library for reading TFmini ToF LIDAR sensors
 * written in modern C++
 *
 * Version: 0.1.0
 * URL: https://github.com/ekondayan/libtfmini.git
 *
 * Copyright (c) 2019-2020 Emil Kondayan
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <iomanip>

#include <QFile>
#include <QTextStream>
#include <QElapsedTimer>

#include "bsp_tfmini.h"

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    int reading = 0;

    tf.reset();
    tf.setDistanceUnit(tfmini::UNIT_MM);
    tf.setDistanceMode(tfmini::DISTANCE_LONG);
    tf.setDetectionPattern(tfmini::DETECTION_AUTO);

    QFile file("output.csv");
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        std::cerr<<"Can not open CSV file for writing: "<<file.errorString().toLocal8Bit().constData()<<std::endl;
        return -1;
    }

    QTextStream csv_stream(&file);
    csv_stream.setFieldAlignment(QTextStream::AlignLeft);
    csv_stream.setRealNumberNotation(QTextStream::FixedNotation);
    csv_stream.setRealNumberPrecision(2);
    csv_stream<<"Reading,"<<
                "Measurement"<<endl;

    while(true)
    {
        tfmini::Measurement measure;
        if(tf.readMeasure(&measure))
        {
            float dist = measure.reading;

            std::cout<<std::fixed << std::setprecision(0)<<
                       "Num:"<<++reading<<
                       "\tD:"<<dist<<
                       std::endl;

            csv_stream<<reading<<","<<
                        dist<<
                        endl;
        }
    }

    file.flush();
    file.close();

    return 0;
}
