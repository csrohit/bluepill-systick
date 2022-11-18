# STM32 Programming with libopencm3

![Build Passing](https://img.shields.io/badge/build-passing-brightgreen) [![GPLv3 License](https://img.shields.io/badge/License-GPL%20v3-yellow.svg)](https://opensource.org/licenses/)
Blinking an led on STM32F1 Blue pill micro-controller with [libopencm3](https://github.com/libopencm3/libopencm3) is more simpler than any alternative for professional developes.
The *Hello World* of embedded world, blinking led is super easy to implement by using SysTick timer as a time base. The project is implemented using a library called libopencm3. In [this blog post](https://medium.com/@csrohit/stm32-blue-pill-using-libopencm3-882165a0d79f/), you will get to know about libopencm3 and how to use in our projects.

## libopencm3

The libopencm3 project aims to create an open-source firmware library for various ARM Cortex-M microcontrollers. The library supportes most subsystems on *STM32F1* and other mcu series. It uses `gcc-arm-none-eabi` as the toolchain and `make` as the build system. Project specific steps related to configuration and building are given in sections below.\
To read more about the library visiit [libopencm3 on GitHub](https://github.com/libopencm3/libopencm3).

## SysTick Timer

The processor has a 24 bit system timer, SysTick, that counts down from the reload value to zero, reloads and counts down on the subsequent clocks.\
System Tick Time (SysTick) generates interrupt requests on regular basis. This allows an os to perform context switching to support multitasking. For applications that do not require an OS, the SysTick can be used for time keeping, time measurement or as an interrupt source for tasks that need to be executed regularly.\
SysTick register can only be accessed using word access.

### Control flow

![Control flow diagram for SysTick timer](../docs/filled.png "SysTick timer - control flow diagram")

1. Program the reload value:\
   The reload value can be loaded by setting `STK_RVR` (*Reload Value Register*) register. This value is set to 1 less that the number of clock cycles needed for the interrupt as the timer counts both reload value as well as zero. e.g. If the SysTick interrupt is required every 100 clock pulses, set RELOAD to 99.
2. Clear current value:\
   This register can be accessed using `STK_CVR` (*Current Value Register*) variable. Bits *24:31* are reserved and 24 bit value can be read from bits *23:0*. Writing any value this register sets it to zero along with setting `STK_CSR_COUNTFLAG` to zero.
3. Configure SysTick and start:
   1. Select clock source-\
        Clock source can be set using `STK_CSR_CLKSOURCE` (*Clock source*) bit (2) of `STK_CSR` (*Control and Status Register*) register.\
        0 - AHB/8\
        1 - Processor Clock (AHB)
   2. Enable Tick interrupt-\
        To enable Tick interrupt set `STK_CSR_TICKINT` bit (2) of `STK_CSR` register.
   3. Start SysTick timer-\
        `STK_CSR_ENABLE` bit (0) of `STK_CSR` register enables the counter. When `STK_CSR` is set to 1, the counter loads the `STK_RVR` value to the `STK_CVR` register and then counts down. On reaching 0, it sets the `STK_CSR_COUNTFLAG` to 1 and optionally asserts the `SysTick` depending on the value of `STK_CSR_TICKINT`. It then loads the `STK_RVR` value again, and begins counting.

## Project Working

This project configures SysTick timer and uses it to generate time accurate delay for blinking an LED. The onboard LED connected to pin C13 blinks every second.

## Dependencies

* **make**\
    Make utility is required for configuring and building this project. You can install make on linux by running command:

    ```bash
    sudo apt install build-essential
    ```

* **gcc-arm-none-eabi toolchain**\
    ARM cross-platform toolchain is required to build applications for arm mcus. Toolchain can be installed by running following command:

    ```bash
    sudo apt install gcc-arm-none-eabi
    ```

* **openocd**\
    It is an Open On Circuit Debugging tool used to flash and debug arm micro controllers. You can install openocd on linux by running command:

   ```bash
   sudo apt install openocd -y
   ```

* **Cortex Debug extension**\
    This extension for VSCode is helpful for debugging the application on Blue Pill. The contents of registers as well as memory are visible in the context menu.
    Launch VS Code Quick Open (Ctrl+P), paste the following command, and press enter.

     ```bash
    ext install marus25.cortex-debug
    ```

* **libopencm3**\
    After cloning this repository, clone the libopencm3 submodule and build for the required micro-controllers.

   ```bash
   git submodules update --init
   pushd libopencm3
   make TARGETS="stm32/f1"
   popd
   ```

## Project Structure

* `src` directory contains all source files for the project
* `include` directory contains all header files for the project
* `libopencm3` directory contains source and header files for the library

### Source file description

* `stm32f1.ld`  - linker script specific to the blue pill board
* `src\main.cpp` - application code body
* `STM32F103.svd` - contains the description of the system contained in Arm Cortex-M processor-based microcontrollers, in particular, the memory mapped registers of peripherals.
* `Makefile` - main root level makefile for the project, which builds the binary

## Run Locally

Running the project is super easy. Just clone, build, and flash.

### Clone the project

1. Using https

    ```bash
    git clone https://github.com/csrohit/bluepill-systick.git
    cd bluepill-systick/opencm3
    git submodules update --init
    ```

2. Using ssh

    ```bash
    git clone git@github.com:csrohit/bluepill-systick.git
    cd bluepill-systick/opencm3
    git submodules update --init
    ```

## Configuration

All the configuration required for building this project is given below.

1. Build output directory
    In `Makefile`, output directory can be configured using variable `BUILD_DIR`.

2. Build type
    In `Makefile`, build type can be configured using variable`DEBUG`. Possible values are `Debug` and `Release`.

3. Binary name
    In `Makefile`, output directory can be configured using variable `TARGET`.
    ** update above info in `.vscode/launch.json` as well for debugging to work.

## Build

*Before building the project, remember to build the libopencm3 library for the specfic micro-controller, without it project build will fail.

```bash
pushd libopencm3
make TARGETS="stm32/f1"
popd
```

*Run following command in terminal to generate flashable binaries for blue pill board. Build files will be written to **Build Output Directory** as configured.

```bash
make all
```

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
