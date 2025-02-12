# Water Screen Controller - firmware

This repository contains the firmware for the **Water Screen Controller**, developed using **MCUXpresso IDE v11.8.0_1165**.

## Prerequisites

Before importing and building the project, ensure that you have the following dependencies installed:

- **MCUXpresso IDE v11.8.0_1165**
- **SDK for LPCXpresso55S69 (SDK_2.x_LPCXpresso55s69 v2.14.0)**
- **A compatible LPCXpresso55S69 development board**

### **Downloading the Required SDK**
The required SDK for compiling and running the firmware can be downloaded from the NXP website. Log in and use the following link to access the SDK package:

[Download SDK_2.x_LPCXpresso55s69 v2.14.0](https://mcuxpresso.nxp.com/download/6e0178eca0f8a0fd8b1cdc90a3931d0c)

## Importing the Project

Once the repository is cloned, you can import the existing project into your **MCUXpresso** workspace by following these steps:

1. **Open MCUXpresso IDE**.
2. **Go to** `File` → `Import` → `General` → `Existing Projects into Workspace`.
3. **Browse to the cloned repository directory** and select the project.
4. Click `Finish` to complete the import.

## Configuring WiFi communication

Before building the project, you must configure the WiFi settings:

1. **Navigate to `source/config/`**.
2. **Clone the `wifi_cfg-example.h` and change its name to `wifi_cfg.h`**:
3. **Open `wifi_cfg.h` and update the WiFi configuration**:

## Building the Firmware

1. **Ensure that the required SDK has been installed in MCUXpresso**.
2. **Select the project in MCUXpresso IDE**.
3. Click on the **Build** button.

## Flashing and Debugging

To flash the firmware onto the LPCXpresso55S69 board:

1. **Connect the board via USB**.
2. **Ensure MCUXpresso recognizes the device**.
3. **Click the Debug button in MCUXpresso** or use the built-in flash tools.

## Notes
- Make sure that the WiFi credentials in `wifi_cfg.h` are correctly set before flashing the firmware.
- Verify that the correct version of the SDK is installed in MCUXpresso to avoid build errors.

---

