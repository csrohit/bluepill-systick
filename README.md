# Blue Pill - SysTick programming

![Build Passing](https://img.shields.io/badge/build-passing-brightgreen) [![GPLv3 License](https://img.shields.io/badge/License-GPL%20v3-yellow.svg)](https://opensource.org/licenses/)

Configure the systick timer to generate periodic interrupts. Use this timer for generating accurate delay function.

## SysTick Timer

The processor has a 24 bit system timer, SysTick, that counts down from the reload value to zero, reloads and counts down on the subsequent clocks.\
System Tick Time (SysTick) generates interrupt requests on regular basis. This allows an os to perform context switching to support multitasking. For applications that do not require an OS, the SysTick can be used for time keeping, time measurement or as an interrupt source for tasks that need to be executed regularly.\
SysTick register can only be accessed using word access.


## Project Variant

STM32F1 Blue pill is a versatile board and can be programmed in various ways. I have created following variations of this project which differs in programming language, use of RTOS and framework. Clone the repository and follow steps mentioned in the respective variations for building and flashing.

1. [Baremetal](baremetal)
2. [libopencm3](opencm3)
3. [FreeRTOS](rtos)
4. Zephyr RTOS
5. Arduino
6. CPP Cmake
7. ST HAL
## Flash

1. Connect STlink to PC and blue pill board using swd headers.
2. Put blue pill board in programming mode.
3. Run following to flash board with binary.

```bash
make flash
```

## Output

Onboard led connected to Pin C13 can be observed to be blinking every second.

## Debug

Click in `Run and Debug` option in VsCode sidebar. Then launch `Cortex Debug` target.

Happy debugging....
