# <u>libtfmini</u>

---

# <u>Description</u>

C++17 header only, driver library for reading TFmini ToF LIDAR sensors. The header only approach greatly simplifies the process of including it into other projects. The minimum C++ standard is [ISO/IEC 14882](https://en.wikipedia.org/wiki/ISO/IEC_14882) (C++17). 

The library is suitable from embedded devices to desktop computers and servers and anything in between as long as you have access to a C++17 compiler.

The project is hosted on GitHub [https://github.com/ekondayan/libtfmini](https://github.com/ekondayan/libtfmini).

# <u>Dependencies</u>

The library is self contained and does not use any external dependencies.

# <u>Usage</u>

## Override the GET and POST methods of `InterfaceComm`

The library does not provide a communication layer, so it needs to be implemented. This is easily done by implementing two global functions for sending and receiving data to/from the device.

```cpp
void send(uint8_t device_id, const uint8_t *buffer, int16_t len);
void receive(uint8_t device_id, uint8_t *buffer, int16_t len);
```

Those two methods do one simple task - to send a buffer of specified length and to receive a buffer of specified length. This design decision was made for two reasons:

- Communication layer is entirely project specific and it's implementation can differ from project to project.

- You may wish to have more control over the execution of `send` and `receive`.
  For example if you want to track the execution time or to count the bytes of the two methods.

Instantiate an object from `tfmini::TFmini` and pass pointers to `send` and `receive` that you have already implemented. When the object want to communicate with the device, it will pass the `device_id` to `send` and `receive`. That way `send` and `receive` will know which device shall be accessed. This is usefull when you have more than one device connected. If you plan to work with just one device, you can safely discard it in the `send` and `receive`. 

```cpp
tfmini::TFmini{device_id, &send, &receive}
```

# <u>Download</u>

You can download the project from GitHub using this command:

git clone https://github.com/ekondayan/libtfmini.git libtfmini

# <u>Install</u>

In order to use libtfmini, you just need to download and extractthe header files into you project. In fact, the header files in the libtfmini subdirectory are the

only files required to compile programs with the library. The header files

are cross platform. It is not necessary to use CMake or install

anything, just include the header files into your project.

# <u>License</u>

Copyright (c) 2019-2020 Emil Kondayan

This Source Code Form is subject to the terms of the Mozilla Public

License, v. 2.0. If a copy of the MPL was not distributed with this

file, You can obtain one at http://mozilla.org/MPL/2.0/.

# <u>Now what?</u>

* 

* if you are feeling nerdy, dig into the source code :)

# <u>Credits</u>

Author: Emil Kondayan

Owner: Emil Kondayan

Maintainer: Emil Kondayan

Contributors: Emil Kondayan
