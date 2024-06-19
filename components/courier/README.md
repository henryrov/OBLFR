## Component
This components supports running NuttX on the D0 core
Include this in your M0 firmware to allow NuttX drivers to operate.

## Description
Several Peripherals of the BL808 only have their IRQ attached to the M0 core.
This component "forwards" all of these IRQs to the M0 core to allow NuttX to operate. 
