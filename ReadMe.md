# Bare-Metal AVR Development Environment (Windows)

This guide explains how to set up a pure command-line development environment for AVR microcontrollers (like the ATmega328P) on Windows, without relying on heavy IDEs like Microchip Studio or Arduino. 

By the end of this setup, you will be able to compile C code and flash it directly to your MCU using a standard `Makefile`.

## Prerequisites
To compile and build projects from the command line, you need two pieces of software:
1. **avr-gcc**: The compiler toolchain that turns C code into AVR machine code.
2. **make**: A build automation tool that reads the `Makefile` and executes the compilation steps.

---

## Step 1: Download the AVR Toolchain
1. Download the official **AVR 8-bit GNU Toolchain** for Windows from Microchip:
   [Microchip GCC Compilers Download Page](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers)
2. Extract the downloaded `.zip` file to a permanent location on your PC. A good practice is to place it directly in your `C:\` drive (e.g., `C:\avr8-gnu-toolchain-win32_x86_64`).

## Step 2: Download GNU Make
The official toolchain does not include `make` for Windows. We need to add it manually.
1. Download the latest Windows release of **xPack GNU Make** (look for the `.zip` file ending in `win32-x64.zip`):
   [xPack GNU Make Releases](https://github.com/xpack-dev-tools/windows-build-tools-xpack/releases)
2. Extract the downloaded file.
3. Open the extracted folder and navigate into the `bin` directory.
4. Copy the `make.exe` file.
5. Paste `make.exe` directly into the `bin` folder of your AVR toolchain from Step 1 (e.g., `C:\avr8-gnu-toolchain-win32_x86_64\bin\`).

## Step 3: Add the Toolchain to your Windows PATH
To run `avr-gcc` and `make` from any command prompt window, Windows needs to know where these tools are located.

1. Open the Windows Start Menu and search for **Environment Variables**.
2. Select **Edit the system environment variables**.
3. In the System Properties window, click the **Environment Variables...** button.
4. Under the "System variables" (or "User variables") section, scroll down and select the **Path** variable, then click **Edit...**.
5. Click **New** and paste the exact path to the `bin` folder of your AVR toolchain. 
   *(Example: `C:\avr8-gnu-toolchain-win32_x86_64\bin`)*
6. Click **OK** on all three windows to save your changes.

## Step 4: Verify the Setup
Open a **new** Command Prompt or PowerShell window (it must be new so it loads the updated PATH) and run the following commands:

```cmd
avr-gcc --version
