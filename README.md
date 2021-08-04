
***
# XROCK
The low level tools for rockchip SOC with maskrom and loader mode support.

## How to build

### Linux platform

The xrock tools depends on the `libusb-1.0` library, you need to install `libusb-1.0-0-dev` before compile, for example in ubuntu:

```shell
sudo apt install libusb-1.0-0-dev
```

Then just type `make` at the root directory, you will see a binary program.

```shell
cd xrock
make
sudo make install
```

### Window platform

Install some build tools

```shell
sudo apt install mingw-w64
sudo apt install autoconf
sudo apt install libtool-bin
```

Download and install libusb

```shell
git clone https://github.com/libusb/libusb.git
cd libusb
./autogen.sh
./configure --host=i686-w64-mingw32 --prefix=/usr/i686-w64-mingw32/
make
sudo make install
```

Build xrock source code

```shell
cd xrock
CROSS=i686-w64-mingw32- make
```

For 64-bits windows, you can using `x86_64-w64-mingw32-` instead of `i686-w64-mingw32` above.


## Usage

```shell
xrock(v1.0.0) - https://github.com/xboot/xrock
usage:
    xrock maskrom <ddr> <usbplug>        - Initial chip using ddr and usbplug in maskrom mode
    xrock version                        - Show chip version
    xrock reset [maskrom]                - Reset device to normal or maskrom mode
    xrock hexdump <address> <length>     - Dumps memory region in hex
    xrock dump <address> <length>        - Binary memory dump to stdout
    xrock read <address> <length> <file> - Read memory to file
    xrock write <address> <file>         - Write file to memory
    xrock exec <address>                 - Call function address
```

## Tips

- The maskrom command can only used in maskrom mode, Before executing other commands, you must first execute the maskrom command

- The memory base address from `0`, **NOT**  sdram's physical address.

### RK3128

```shell
sudo xrock maskrom rk3128_ddr_300MHz_v2.12.bin rk3128_usbplug_v2.63.bin
sudo xrock version
```

### RK3288

```shell
sudo xrock maskrom rk3288_ddr_400MHz_v1.09.bin rk3288_usbplug_v2.58.bin
sudo xrock version
```

### RK3399

```shell
sudo xrock maskrom rk3399_ddr_800MHz_v1.25.bin rk3399_usbplug_v1.26.bin
sudo xrock version
```

### PX30

```shell
sudo xrock maskrom px30_ddr_333MHz_v1.16.bin px30_usbplug_v1.31.bin
sudo xrock version
```

## Links

* [The rockchip loader binaries](https://github.com/rockchip-linux/rkbin)
* [The rockchip rkdeveloptool](https://github.com/rockchip-linux/rkdeveloptool)

## License

This library is free software; you can redistribute it and or modify it under the terms of the MIT license. See [MIT License](LICENSE) for details.

