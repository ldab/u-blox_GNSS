# u-blox_GNSS

u-blox GNSS, GPS receiver library minimalistic light weight for low power tracking application.

[![GitHub version](https://img.shields.io/github/release/ldab/u-blox_GNSS.svg)](https://github.com/ldab/u-blox_GNSS/releases/latest)
[![Build Status](https://travis-ci.org/ldab/u-blox_GNSS.svg?branch=master)](https://travis-ci.org/ldab/u-blox_GNSS)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://github.com/ldab/u-blox_GNSS/blob/master/LICENSE)

[![GitHub last commit](https://img.shields.io/github/last-commit/ldab/u-blox_GNSS.svg?style=social)](https://github.com/ldab/u-blox_GNSS)

![SAM-M8Q GNSS Antenna Module](https://www.u-blox.com/sites/default/files/styles/product_full/public/products/SAM-M8Q.png)

## TODO

- [x] Use Stream class to work with both Harware and Software serial
- [x] Implement better debug handler
- [ ] Update Keywords
- [x] Implement serial timeout
- [ ] Add AssistNow AID support

## How does it work?

When first powered the GNSS module automatically starts sending [NMEA](https://en.wikipedia.org/wiki/NMEA_0183) messages over the iterface, so the basic setup would be:
1. Disable NMEA and enable [UBX protocol](./extras/u-blox8-M8_ReceiverDescrProtSpec_(UBX-13003221)_Public.pdf);
2. Especify the Power Save Mode;
3. Save configuration, otherwise it will reset to default configuration after waking up;
4. Poll messages anytime you want;

You can also turn the GNSS module OFF by ```GNSS::off()``` function, and empty command will sleep forever until any activity on the UART interface.

When the ```GNSS::getCoodinates``` is called, it try to wake the module and retrive Navigation information, the default timeout is *60 sec* and the accuracy can be set as the last parameter of the function.

## Power Save Modes

* Continuous (default) mode for best GNSS performance vs. power consumption;
* 1, 2 or 4Hz cyclic tracking mode for aggressive power reduction*;
* ON/OFF interval mode;

We will use UBX-CFG-PMS (only supported in protocol versions 18+).

*Only 1Hz is enabled in this library.

## Current consumption of operating modes

Current comsumption is hard to estimate as it would vary significantly based on signal conditions as well as navigation rate but some indicative data is available as follows:

Mode        | TYP mA
------------|--------
Max         | 67
Acquisition*| 25
Continous   | 23
PSM 1Hz     | 9.5

*Current from start-up until the first fix.

## Troubleshooting

If you get in trouble or want to try different configurations you can use the [serial_bridge.ino](.examples\Serial_Bridge\serial_bridge.ino) example and control the module using u-blox [u-center](https://www.u-blox.com/en/product/u-center). This example simply bridge the serial USB over the UART connected to the u-blox module defined on the ```#define``` interfaces.

## Credits

Github Shields and Badges created with [Shields.io](https://github.com/badges/shields/)

u-blox official mBed [library](https://os.mbed.com/teams/ublox/code/gnss/)
