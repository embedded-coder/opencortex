# opencortex

## 1. What is it?

This project is aimed to create an open-source multi-platform architecture for arm cortex based microcontrollers.

currently, we support platform

- fk103m4 (stm32f103vet6)

we support jtag interface

- cmsis-dap

This project is organized by makefiles, using GCC tool chain arm-none-eabi. Flash update and debug operation are all integrated in makefiles.

## 2. How to use it?

### 2.1 Preparation
For Ubuntu, we need to prepare:
- arm-none-eabi-gcc
- arm-none-eabi-gdb
- openocd
- make
- minicom

### 2.2 Build

First, we select one application

> $ cd demo/hello-world

Compile projects

> $ make

### 2.3 Flash

> $ make flash

### 2.4 Debug

> $ make debug

