# About XROCK

XROCK is a low-level tool for Rockchip SOCs. It supports both maskrom and loader modes, and can initialize chips, read/write memory, flash storage, and access OTP/SN over USB.

## Working modes

XROCK supports two device modes:

- **Maskrom mode**: the USB download mode of the BootROM at power-on. In this mode you must first upload the DDR init code and usbplug to bring the chip into loader mode before other commands work. The `maskrom`, `download` and `extra maskrom*` commands are only available in this mode.
- **Loader mode**: entered after usbplug / loader runs. It provides rich commands such as version query, memory access, flash operations, and OTP/SN/vendor storage access.

:::note
Before running loader-mode commands, you must initialize the chip with `maskrom` or `download` to enter loader mode.
:::

## Features

- Cross-platform: Linux, Windows and macOS;
- Built on libusb-1.0, no kernel driver required;
- Maskrom mode: upload DDR/usbplug, load loader, dump memory via debug UART, write/execute in memory;
- Loader mode: version/capability query, reset, memory read/write, execute;
- OTP read, serial number (SN) read/write, vendor storage read/write;
- Storage media query and switching;
- Flash detect, erase, read and write (multiple storage media).

## Entering maskrom mode

Generally: hold the board's Maskrom / Vol+ button (varies by board), power on or reset, and the host identifies a Rockchip device (USB Vendor ID `0x2207`).

## Links

- Source repository: [https://github.com/xboot/xrock](https://github.com/xboot/xrock)
- Rockchip loader binaries: [https://github.com/rockchip-linux/rkbin](https://github.com/rockchip-linux/rkbin)
- Official rkdeveloptool: [https://github.com/rockchip-linux/rkdeveloptool](https://github.com/rockchip-linux/rkdeveloptool)
