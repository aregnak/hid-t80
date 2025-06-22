# Linux kernel module for Thrustmaster T80 (EXPERIMENTAL)

> **DISCLAIMER:** This is not an official Thrustmaster driver,
> be careful and read through the code of any kernel module you run!
> I am not responsable for any damaged devices as a result of running
> this code. Use at your own discretion.
> 
> The module is not completed! It is only partially
> functional! As of now, only steering, pedals, shifting, and face 
> buttons are functional. The rest of the buttons either partially
> work or not at all.

## Description

A linux kernel module for the Thrustmaster T80 game wheel. 

This module is meant to be a driver for the T80, though it is still
a work in progress. 

### Tested environments:
- Distro: Arch 
- Kernel: 6.14.10-arch1-1
- Desktop Environment: KDE (will all bloat)


### Games Tested:
Assetto Corsa with CM and CSP installed using [this](https://github.com/sihawido/assettocorsa-linux-setup) guide.

## Installation
For now, you need to manually build it to install it, and so far only 
a guide for Arch based distros is available.

### Dependencies
Install `linux-headers` and `joyutils` and upgrade the system.
Make sure `linux-headers` and `linux` are the same version
by upgrading the system.
```shell
sudo pacman -Syu linux-headers joyutils
```
If using linux LTS or another version, use
```shell
sudo pacman -Syu linux-lts-headers
```

### Building
The Makefile simplifies this task, and using it is very simple.
```shell
# Step 1:
make
```
#### To install:
```shell
# Step 2:
# Either temporarily load:
make load

# Or permanantly install:
make install
```
#### To uninstall:
```shell
# If loaded:
make unload

# If installed:
make uninstall
```

### Steam Settings
To use on steam, make sure to `disable steam input` for correct inputs. 

```
Right-click the game -> Properties -> Controller -> Override for GAME (Disable steam input)
```

## Status and Compatibility
### Game Tests

Assetto Corsa: While all main inputs needed for driving are mapped, handling feels weird
and noticably compared to Windows. Also, the steering wheel on idle may or 
may not be off center towards the left, as is the case for me.

### Oversteer Compatibility

Currently only steering and pedal degree are visible on Oversteer,
and the pedal mapping is off due to the `ABS` input given.

## Thank you!

This is made for fun because there is no native 
support for the Thrustmaster T80, so why not try. Any contribution 
will help!