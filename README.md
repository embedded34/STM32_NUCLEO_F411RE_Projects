# STM32 Development Tools Setup for Linux Mint

This guide will walk you through setting up a complete development environment for STM32 microcontroller programming on a Linux Mint machine. The setup includes STM32CubeIDE, the GNU Arm Embedded Toolchain, OpenOCD, and Make. Follow these steps carefully to get started with STM32 development.

## Prerequisites

- Linux Mint (Any version)
- A terminal with sudo access

## Step 1: Install Required Packages

### 1.1. Update Your System

First, make sure your system is up to date. Open a terminal and run the following command:

```bash
sudo apt update -y && sudo apt full-upgrade -y && sudo apt-get dist-upgrade -y && sudo apt autoremove -y && sudo apt clean -y && sudo apt autoclean -y
```

### 1.2. Install Dependencies

To install the required packages, run:

```bash
sudo apt install build-essential
```

This will install:
- `build-essential` (includes Make, GCC, etc.)

## Step 2: Install STM32CubeIDE (Latest Version: 1.18.0)

STM32CubeIDE is the official IDE from STMicroelectronics for STM32 development. It integrates the GNU toolchain, OpenOCD, and the STM32CubeMX configuration tool.

### 2.1. Download STM32CubeIDE

1. Go to the [STM32CubeIDE download page](https://www.st.com/en/development-tools/stm32cubeide.html).
2. Download the Linux version of STM32CubeIDE (STM32CubeIDE Debian Linux Installer).
3. Extract the downloaded `.tar` archive.

### 2.2. Install STM32CubeIDE

Open a terminal in the directory where you extracted STM32CubeIDE and run the followings to make the file executable and to start the installation process:

```bash
chmod +x st-stm32cubeide_1.18.0_24413_20250227_1633_amd64.deb_bundle.sh
sudo ./st-stm32cubeide_1.18.0_24413_20250227_1633_amd64.deb_bundle.sh
```

This will install STM32CubeIDE on your system.

### 2.3. Launch STM32CubeIDE

After installation, you can start STM32CubeIDE from your applications menu.

## Step 3: Install GNU ARM Toolchain (Latest Version: 14.2.Rel1)

The GNU ARM toolchain is required for compiling STM32 code.

### 3.1. Download the GNU ARM Toolchain

1. Go to the official [ARM toolchain download page](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads).
2. Download the `arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz` archive for your platform (x86_64 Linux).

### 3.2. Extract the Toolchain Archive

Once the archive is downloaded, you need to extract it.

1. Open a terminal and navigate to the folder where the `arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz` file is located.
2. Extract the archive using the following command:

```bash
tar -xvf arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz
```

### 3.3. Move the Toolchain to a System Directory

Move the extracted directory to `/opt` for system-wide access:

```bash
sudo mv arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi /opt/
```

### 3.4. Add the Toolchain to the PATH

To make the toolchain accessible globally, you need to add it to your `PATH`.

1. Open your `.bashrc` in your home directory:

```bash
nano ~/.bashrc
```
2. Add the following line at the end of the file:

```bash
export PATH=$PATH:/opt/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin
```

3. Save the file and exit the editor (`CTRL + X`, then `Y`, then `Enter` for nano).

4. Reload the `.bashrc` file to apply the changes:

```bash
source ~/.bashrc
```

### 3.5. Verify Installation

To verify the installation, run the following command:

```bash
arm-none-eabi-gcc --version
```

You should see the version of the ARM GCC compiler printed in the terminal.

## Step 4: Install OpenOCD (Latest Version: 0.12.0-6)

OpenOCD (Open On-Chip Debugger) is used for programming and debugging STM32 devices over JTAG/SWD.

### 4.1. Download the OpenOCD Archive

1. Go to the official [OpenOCD download page](https://github.com/xpack-dev-tools/openocd-xpack/releases).
2. Download the `xpack-openocd-0.12.0-6-linux-x64.tar.gz` archive for your platform (x86_64 Linux).

### 4.2. Extract the OpenOCD Archive

Once the archive is downloaded, you need to extract it.

1. Open a terminal and navigate to the folder where the `xpack-openocd-0.12.0-6-linux-x64.tar.gz` file is located.
2. Extract the archive using the following command:

```bash
tar -xvzf xpack-openocd-0.12.0-6-linux-x64.tar.gz
```

### 4.3. Move OpenOCD to a System Directory

Move the extracted directory to `/opt` for system-wide access:

```bash
sudo mv xpack-openocd-0.12.0-6 /opt/
```

### 4.4. Add OpenOCD to the PATH

To make OpenOCD accessible from anywhere in the terminal, you need to add it to your `PATH`.

1. Open your `.bashrc` in your home directory:

```bash
nano ~/.bashrc
```
2. Add the following line at the end of the file:

```bash
export PATH=$PATH:/opt/xpack-openocd-0.12.0-6/bin
```

3. Save the file and exit the editor (`CTRL + X`, then `Y`, then `Enter` for nano).

4. Reload the `.bashrc` file to apply the changes:

```bash
source ~/.bashrc
```

### 4.5. Verify OpenOCD Installation

You can verify that OpenOCD is installed correctly by running:

```bash
openocd --version
```

This will display the OpenOCD version and confirm that it’s properly installed.

## Step 5: Install Make

Make is used to automate the build process in STM32 projects.

### 5.1. Install Make

If `make` is not already installed, you can install it with the following:

```bash
sudo apt install make
```

### 5.2. Verify Make Installation

Verify that `make` is installed by checking its version:

```bash
make --version
```
---

That's it! You're now ready to develop, build, and flash your STM32 projects on Linux Mint using STM32CubeIDE, the GNU ARM toolchain, OpenOCD, and Make. Happy coding!
