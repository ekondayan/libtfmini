# <u>libtfmini</u>

---

# <u>Description</u>

C++17 header only, driver library providing low and high level access for TFmini ToF LIDAR sensors. The library is designed in such a way that it can work with single or multiple connected sensors. Supported firmware versions are **15x** and **16x**. The header only approach greatly simplifies the process of including it into other projects. 

The minimum C++ standard is [ISO/IEC 14882](https://en.wikipedia.org/wiki/ISO/IEC_14882) (C++17). 

The library is suitable from embedded devices to desktop computers and servers and anything in between as long as you have access to a C++17 compiler.

The project is hosted on GitHub [https://github.com/ekondayan/libtfmini](https://github.com/ekondayan/libtfmini).

# <u>Dependencies</u>

It is self contained and does not rely neither on the C++ Standard library nor any other external dependencie. The code is just plain C++17.

# <u>Usage</u>

## Implement `send` and `receive` functions

The library does not provide a low level communication layer, so it needs to be implemented by the user. This is easily done by implementing two global functions for sending and receiving data to/from the device.

```cpp
void send(uint8_t device_id, const uint8_t *buffer, int16_t len);
void receive(uint8_t device_id, uint8_t *buffer, int16_t len);
```

Those two methods do one simple task - to send a buffer of specified length and to receive a buffer of specified length. This design decision was made for two reasons:

- Communication layer is entirely project specific and it's implementation can differ from project to project. If you develop for a desktop you may want to use the native Linux serial port API or  you may want to use the more user friendly and polished API of QT. If you develop for a MCU you maw want to use the MCU's periphery directly or you may want to use the drivers provided by some RTOS. Those are just an examples of some use cases. The real world is much much more colorfull. 

- You may wish to have more control over the execution of `send` and `receive`.
  For example if you want to track the execution time or to count the bytes transfered by the two methods.

An example implementation of  using the QT's SerialPort library

```cpp
void send(uint8_t device_id, const uint8_t *buffer, int16_t len)
{
    if(!port.isOpen()) return;
    port.clearError();
    port.write((const char*)buffer, len);
    while(port.bytesToWrite() && port.waitForBytesWritten(100));
}
```

```cpp
void receive(uint8_t device_id, uint8_t *buffer, int16_t len)
{
    if(!port.isOpen()) return;
    port.clearError();
    int num_read = 0;
    do num_read += port.read((char*)(buffer + num_read), len - num_read);
    while(num_read != len && port.waitForReadyRead(100));
}
```

In this example `port` is an object of type `QSerialPort` but you can use any other library.

## Instantiate an object of type `tfmini::TFmini`

When instantiating an object of type  `tfmini::TFmini` pass pointers to `send` and `receive` functions that you have already implemented. 

```cpp
tfmini::TFmini tfmini(device_id, &send, &receive);
```

A good practice is to subclass `tfmini::TFmini`. 

When the an object wants to communicate with the device, it will pass the `device_id` to `send` and `receive`. That way `send` and `receive` will know which device shall be accessed. This is usefull when you have more than one device connected. If you plan to work with just one device, you can safely discard it in the `send` and `receive`.

# <u>Code Organization</u>

The API is organized in two layers: low level and high level.

## Low level API

Defined in `tfmini_comm.h`. It's main purpose is to guarantee the correct transceiving of raw packets from/to the device.

## High level API

Defined in `tfmini.h`. It exposes high level functions for controlling the device. It's purpose is to correctly format the commands and to pass them to the lower level API.

# <u>Examples</u>

In the examples section you can find example applications how to use the library.

# <u>Download</u>

You can download the project from GitHub using this command:

git clone https://github.com/ekondayan/libtfmini.git libtfmini

# <u>Install</u>

In order to use libtfmini, you just need to download and extract the header files into you project. In fact, the header files in the libtfmini subdirectory are the

only files required to compile programs with the library. The header files

are cross platform. It is not necessary to use CMake or install

anything, just include the header files into your project.

# <u>License</u>

Copyright (c) 2019-2020 Emil Kondayan

This Source Code Form is subject to the terms of the Mozilla Public

License, v. 2.0. If a copy of the MPL was not distributed with this

file, You can obtain one at http://mozilla.org/MPL/2.0/.

# <u>Now what?</u>

* if you are feeling nerdy, dig into the source code :)

# <u>Credits</u>

Author: Emil Kondayan

Owner: Emil Kondayan

Maintainer: Emil Kondayan

Contributors: Emil Kondayan
