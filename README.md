
***
# XROCK
The rockusb tools for rockchip SOC, support maskrom and loader mode for all chips.

## How to build

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

## Usage

```
xrock(v1.0.0) - https://github.com/xboot/xrock
usage:
    xrock maskrom <ddr> <usbplug>        - Initial chip using ddr and usbplug in maskrom mode
    xrock reset [maskrom]                - Reset device to normal or maskrom mode
    xrock hexdump <address> <length>     - Dumps memory region in hex
    xrock dump <address> <length>        - Binary memory dump to stdout
    xrock exec <address>                 - Call function address
    xrock read <address> <length> <file> - Read memory to file
    xrock write <address> <file>         - Write file to memory
```

## Links

* [The rockchip loader binaries](https://github.com/rockchip-linux/rkbin)
* [The rockchip rkdeveloptool](https://github.com/rockchip-linux/rkdeveloptool)

## License

This library is free software; you can redistribute it and or modify it under the terms of the MIT license. See [MIT License](LICENSE) for details.

