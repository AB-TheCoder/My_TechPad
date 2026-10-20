# techpad


*Techpad is my version of macropad i built under hackpad challange stardance.it has 12keys,knob,OLed screen,RGB,XIAO,RP2040 and it is build to give a clean,smart and aesthetic look for people on a mission.*

* Keyboard Maintainer: [Aarav Bhatt](https://github.com/AB-TheCoder)
* Hardware Supported: *TechPad PCB v1.0, Seeed XIAO RP2040*
* Hardware Availability: *Hack Club Hackpad kit*

Make example for this keyboard (after setting up your build environment):

    how to flash: hold BOOT, plug in, drag the .uf2 onto the RPI-RP2 drive

Flashing example for this keyboard:

    qmk compile -kb techpad -km default

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (2,2) in the matrix (usually the BOTTOM RIGHT left key or Escape) and plug in the keyboard
* **Physical reset button**: Briefly press the button on the back of the PCB - some may have pads you must short instead
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
