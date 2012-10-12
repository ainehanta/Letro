#!/bin/zsh
sudo avrdude -c dragon_isp -P usb -p m1284p -U flash:w:Letro.hex:i
