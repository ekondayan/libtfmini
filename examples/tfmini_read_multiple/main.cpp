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
#include <iomanip>

#include <QFile>
#include <QTextStream>

#include "bsp_tfmini.h"

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    tf[0].openPort("/dev/ttyUSB0");
    tf[1].openPort("/dev/ttyUSB1");

    QFile file("output.csv");
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        std::cerr << "Can not open CSV file for writing: " << file.errorString().toLocal8Bit().constData()<<std::endl;
        return -1;
    }

    QTextStream csv_stream(&file);
    csv_stream.setFieldAlignment(QTextStream::AlignLeft);
    csv_stream.setRealNumberNotation(QTextStream::FixedNotation);
    csv_stream.setRealNumberPrecision(2);
    csv_stream<<"Reading,"<<
                "Measurement1,"<<
                "Measurement2"<<endl;

    for(int i = 0; i < tf_array_size; ++i)
    {
        tf[i].reset();
        tf[i].setDistanceUnit(tfmini::UNIT_MM);
        tf[i].setDetectionPattern(tfmini::DETECTION_AUTO);
    }

    unsigned int reading = 0;
    while(true)
    {
        csv_stream << reading << ",";
        std::cout<<std::fixed << std::setprecision(0)<<"Num:"<<reading;
        ++reading;

        tfmini::Measurement measure[tf_array_size];

        for(int i = 0; i < tf_array_size; ++i)
        {
            if(tf[i].readMeasure(&measure[i]))
            {
                std::cout<<"\tDistance" <<
                           i <<
                           ":" <<
                           float(measure[i].reading);

                csv_stream << float(measure[i].reading);
            }
            else
            {
                std::cout<<"\tDistance" <<
                           i <<
                           ":NULL";
                csv_stream << "NULL";
            }

            if(i < tf_array_size - 1)
                csv_stream << ",";
        }

        std::cout << std::endl;
        csv_stream << endl;
    }

    file.flush();
    file.close();

    return 0;
}
