
***
# XROCK
The rockusb tools for rockchip SOC, support maskrom and loader mode for many chips.

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
xrock(v1.1.3) - https://github.com/xboot/xrock
usage:
    xrock version                                - Show chip version
    xrock hexdump <address> <length>             - Dumps memory region in hex
    xrock dump <address> <length>                - Binary memory dump to stdout
    xrock exec <address>                         - Call function address
    xrock read32 <address>                       - Read 32-bits value from device memory
    xrock write32 <address> <value>              - Write 32-bits value to device memory
    xrock read <address> <length> <file>         - Read memory to file
    xrock write <address> <file>                 - Write file to memory
    xrock reset                                  - Reset device using watchdog
    xrock sid                                    - Show sid information
    xrock jtag                                   - Enable jtag debug
    xrock ddr [type]                             - Initial ddr controller with optional type
    xrock spinor                                 - Detect spi nor flash
    xrock spinor read <address> <length> <file>  - Read spi nor flash to file
    xrock spinor write <address> <file>          - Write file to spi nor flash
    xrock spinand                                - Detect spi nand flash
    xrock spinand read <address> <length> <file> - Read spi nand flash to file
    xrock spinand write <address> <file>         - Write file to spi nand flash
```

## Links

* [The rockchip rkdeveloptool](https://github.com/rockchip-linux/rkdeveloptool)

## License

This library is free software; you can redistribute it and or modify it under the terms of the MIT license. See [MIT License](LICENSE) for details.

