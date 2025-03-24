## STM32F4 CMSIS Integration Guide

This guide walks you through the process of downloading and integrating the CMSIS (Common Microcontroller Software Interface Standard) files for the STM32F4 series microcontrollers.

CMSIS files contain the definitions of all the registers and their respective bits, making it easier to manage and configure peripherals without manually defining each register. This simplifies the development process, particularly for embedded systems, by providing a standardized interface to access hardware registers and manage peripherals.

### Step 1: Download STM32CubeF4 Package (Latest Version: 1.28.0)

1. Open your browser and go to [STMicroelectronics Official Website](https://www.st.com/content/st_com/en.html).
2. In the search bar, type **STM32CubeF4** to locate the package for the STM32F4 microcontroller series.
3. Download the latest version of STM32Cube MCU Package for STM32F4 series.
*  Ensure it's not a patch version!
4. Once the download is complete, unzip the package. Inside, you will find several subfolders, including the `Drivers` folder.

```bash
unzip en.stm32cubef4-v1-28-0.zip
```

### Step 2: Organize CMSIS Files

1. Inside your project workspace, create a new folder named `chip_headers`.
2. Within the `chip_headers` folder, create another folder named `CMSIS`.
3. Now, navigate to `Drivers/CMSIS` in the extracted STM32CubeF4 package.
4. Copy the entire `Include` folder from `Drivers/CMSIS` and paste it into the `chip_headers/CMSIS` directory you just created.
5. Next, copy the `Device` folder from `Drivers/CMSIS` into `chip_headers/CMSIS`.
6. Finally, clean up the `Device` folder:
* Navigate to `chip_headers/CMSIS/Device/ST/STM32F4xx`.
* Delete all files and folders, except the `Include` folder. This ensures you keep only the necessary header files for your specific microcontroller.
7. After setting up the project, copy the `chip_headers` folder and paste it into the STM32 project directory.

### Step 3: Work with CMSIS Files

The next task involves adding the paths of the subfolders within the `chip_headers` folder to the project’s include paths:

1. Open STM32CubeIDE, right-click on your project, and select Properties.
2. Once the `Properties` window opens, expand the `C/C++ General` options.
3. Select `Paths and Symbols`.
4. Under the `Includes` tab, click `Add...` to add a new directory.
5. Enter `${ProjDirPath}\chip_headers\CMSIS\Include` to add the `Include` folder located in our `CMSIS` folder, and then click `OK` to save.
6. Click `Add...` again to add another directory.
7. Enter `${ProjDirPath}\chip_headers\CMSIS\Device\ST\STM32F4xx\Include` to add the `Include` folder located in the `STM32F4xx` subdirectory and click `OK` to save.

Before closing the project properties dialog box, you need to specify the exact version of the STM32F4 microcontroller you are using. This ensures that the appropriate header file for your specific microcontroller is enabled in your project. The NUCLEO-F411 development board has the STM32F411 microcontroller. To configure your project for the STM32F411 microcontroller:

1. Click on the `# Symbols` tab.
2. Click `Add...` to add a new symbol.
3. In the `Add Symbol` dialog box, enter `STM32F411xE` in the `Name` field and then click `OK`.
4. Click `Apply and Close` to save all changes and close the `Project Properties` window.

Instead of Step 3, you can define your specific microcontroller and the necessary path flags in the project's Makefile.

Now, you should have the CMSIS headers properly set up in your project workspace, ready for use. Write your updated code, build the project in the IDE or with the Make build system, and run it on the development board.

If you want to use the Make build system to compile and build the program, and load it into the microcontroller, run the following command in the terminal:

```bash
make final
```

```bash
make flash
```

---

> **Note:**  
> Ensure that all the above commands are executed in the directory where the project's `Makefile` is located.

---