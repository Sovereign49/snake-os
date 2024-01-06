# snake-os

## Setup
The setup script requires the programs `sgdisk, mkfs.fat, kpartx`, 
check with your distros repos to ensure you have the required packages needed.

To set up the test image run `create_image.sh` to create and/or update the test.hdd image

## Running
To run the os, flash `test.hdd` to a usb via dd or install qemu and run `qemu.sh`

## Docs
The documentation for the UEFI Specification used in this project can be found here ()[https://uefi.org/specs/UEFI/2.10]
