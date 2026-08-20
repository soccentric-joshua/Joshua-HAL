/**
 * @file platform_joshua_sitara.h
 * @brief Joshua Sitara (AM335x/AM62x/AM64x) platform constants.
 *
 * This header centralizes platform paths and capability values used by HAL
 * modules. Deterministic/bit-precise I/O is expected to be delegated to
 * PRU-ICSS firmware and controlled from Linux via remoteproc + RPMsg.
 */

#pragma once

// CPU family envelope across AM335x/AM62x/AM64x.
static constexpr int    JOSHUA_SITARA_CPU_CORES        = 4;
static constexpr double JOSHUA_SITARA_CPU_MAX_FREQ_MHZ = 1400.0;
static constexpr double JOSHUA_SITARA_CPU_MIN_FREQ_MHZ = 300.0;
static constexpr double JOSHUA_SITARA_CPU_TEMP_MAX_C   = 105.0;

// GPU capabilities vary by SoM SKU; keep conservative defaults.
static constexpr int    JOSHUA_SITARA_GPU_SHADER_CORES = 4;
static constexpr double JOSHUA_SITARA_GPU_MAX_FREQ_MHZ = 800.0;
static constexpr double JOSHUA_SITARA_GPU_MIN_FREQ_MHZ = 300.0;
static constexpr double JOSHUA_SITARA_GPU_TEMP_MAX_C   = 85.0;
static constexpr const char* JOSHUA_SITARA_GPU_DEVFREQ_PATH = "/sys/class/devfreq/";
static constexpr const char* JOSHUA_SITARA_GPU_DEVICE_PATH  = "/sys/devices/platform/";
static constexpr const char* JOSHUA_SITARA_V3D_DEV_PATH     = "/dev/dri/card0";
static constexpr const char* JOSHUA_SITARA_V3D_RENDER_PATH  = "/dev/dri/renderD128";

// Memory capabilities differ by module SKU.
static constexpr long long JOSHUA_SITARA_MEM_8GB_BYTES = 8LL * 1024 * 1024 * 1024;
static constexpr long long JOSHUA_SITARA_MEM_4GB_BYTES = 4LL * 1024 * 1024 * 1024;
static constexpr double JOSHUA_SITARA_MEM_SPEED_MHZ    = 3200.0;
static constexpr double JOSHUA_SITARA_MEM_BW_GBS       = 12.8;
static constexpr int    JOSHUA_SITARA_MEM_BUS_BITS     = 64;

// PCIe availability depends on Sitara variant and carrier design.
static constexpr int    JOSHUA_SITARA_PCIE_GEN    = 2;
static constexpr int    JOSHUA_SITARA_PCIE_LANES  = 1;
static constexpr double JOSHUA_SITARA_PCIE_BW_GBS = 0.5;
static constexpr const char* JOSHUA_SITARA_PCIE_RC_PATH = "/sys/bus/pci/devices/0000:00:00.0";

// USB host controller path in TI Processor SDK Linux.
static constexpr const char* JOSHUA_SITARA_XHCI_DRIVER_PATH = "/sys/bus/platform/drivers/xhci-hcd";
static constexpr int JOSHUA_SITARA_USB3_PORT_COUNT = 1;
static constexpr int JOSHUA_SITARA_USB2_PORT_COUNT = 2;

// Ethernet interface defaults.
static constexpr const char* JOSHUA_SITARA_ETH_IFACE      = "eth0";
static constexpr int         JOSHUA_SITARA_ETH_SPEED_MBPS = 1000;

// Camera/video device defaults for V4L2 discovery.
static constexpr const char* JOSHUA_SITARA_CSI_DEVICE0 = "/dev/video0";
static constexpr const char* JOSHUA_SITARA_CSI_DEVICE1 = "/dev/video1";
static constexpr const char* JOSHUA_SITARA_MEDIA_CTL0  = "/dev/media0";
static constexpr const char* JOSHUA_SITARA_UNICAM_PATH = "/sys/bus/platform/devices";

// Display nodes.
static constexpr const char* JOSHUA_SITARA_DRM_CARD0 = "/dev/dri/card0";
static constexpr const char* JOSHUA_SITARA_DRM_CARD1 = "/dev/dri/card1";

// Audio interfaces.
static constexpr const char* JOSHUA_SITARA_I2S_DEVICE = "/sys/bus/platform/devices";
static constexpr const char* JOSHUA_SITARA_ALSA_CARDS = "/proc/asound/cards";

// Serial defaults for Sitara SDK Linux images.
static constexpr const char* JOSHUA_SITARA_DEBUG_UART = "/dev/ttyS2";
static constexpr const char* JOSHUA_SITARA_UART1      = "/dev/ttyS3";

// Thermal thresholds for industrial-grade operation.
static constexpr const char* JOSHUA_SITARA_THERMAL_CPU_ZONE   = "/sys/class/thermal/thermal_zone0";
static constexpr const char* JOSHUA_SITARA_THERMAL_SB_ZONE    = "/sys/class/thermal/thermal_zone1";
static constexpr double      JOSHUA_SITARA_THERMAL_THROTTLE_C = 95.0;
static constexpr double      JOSHUA_SITARA_THERMAL_SHUTDOWN_C = 105.0;

// Linux userspace GPIO through libgpiod.
static constexpr const char* JOSHUA_SITARA_GPIOCHIP_PATH = "/dev/gpiochip0";
static constexpr int         JOSHUA_SITARA_GPIO_PIN_COUNT = 128;

// PRU-ICSS remoteproc + RPMsg plumbing.
static constexpr int         JOSHUA_SITARA_PRU_REMOTEPROC_ID = 0;
static constexpr const char* JOSHUA_SITARA_PRU_REMOTEPROC_BASE = "/sys/class/remoteproc";
static constexpr const char* JOSHUA_SITARA_PRU_RPMSG_DEVICE = "/dev/rpmsg_pru30";
static constexpr const char* JOSHUA_SITARA_PRU_FIRMWARE_DEFAULT = "am62x-pru0-fw";

// Storage paths.
static constexpr const char* JOSHUA_SITARA_MMCBLK_PATH = "/dev/mmcblk0";
static constexpr const char* JOSHUA_SITARA_NVME_PATH   = "/dev/nvme0n1";

// PMIC/power supply interface.
static constexpr const char* JOSHUA_SITARA_POWER_SUPPLY_PATH = "/sys/class/power_supply";

// I2C controller buses on the Sitara AM62x (I2C0/I2C1/I2C2).
static constexpr const char* JOSHUA_SITARA_I2C_DEV1  = "/dev/i2c-1";
static constexpr const char* JOSHUA_SITARA_I2C_DEV2  = "/dev/i2c-2";
static constexpr const char* JOSHUA_SITARA_I2C_SYSFS = "/sys/bus/i2c/devices";
static constexpr int         JOSHUA_SITARA_I2C_MAX_FREQ_HZ = 400000; // Fast-mode

// SPI controller.
static constexpr const char* JOSHUA_SITARA_SPI_DEV0 = "/dev/spidev0.0";
static constexpr const char* JOSHUA_SITARA_SPI_DEV1 = "/dev/spidev0.1";
static constexpr int         JOSHUA_SITARA_SPI_MAX_HZ = 48000000;

// CAN interface.
static constexpr const char* JOSHUA_SITARA_CAN_IFACE = "can0";
static constexpr int         JOSHUA_SITARA_CAN_BITRATE = 500000;

// RS-485 (half-duplex serial) defaults.
static constexpr const char* JOSHUA_SITARA_RS485_UART = "/dev/ttyS4";
static constexpr int         JOSHUA_SITARA_RS485_BAUD = 115200;

// Analog input (ADC) via the TI IIO subsystem.
static constexpr const char* JOSHUA_SITARA_ADC_BASE = "/sys/bus/iio/devices";
static constexpr const char* JOSHUA_SITARA_ADC_CH0_RAW = "/sys/bus/iio/devices/iio:device0/in_voltage0_raw";
static constexpr int         JOSHUA_SITARA_ADC_BITS = 12; // AM62x ADC resolution

// PWM outputs via the Linux PWM subsystem.
static constexpr const char* JOSHUA_SITARA_PWM_BASE = "/sys/class/pwm/pwmchip0";
static constexpr int         JOSHUA_SITARA_PWM_CHANNELS = 4;

// Storage: SD card / eMMC / NVMe (NVMe already defined above).
static constexpr const char* JOSHUA_SITARA_SD_DEV      = "/dev/mmcblk1";
static constexpr const char* JOSHUA_SITARA_EMMC_DEV    = "/dev/mmcblk0";
static constexpr const char* JOSHUA_SITARA_NVME_SYSFS  = "/sys/class/nvme";
static constexpr const char* JOSHUA_SITARA_MMC_SYSFS   = "/sys/class/mmc_host";

// Real-time clock.
static constexpr const char* JOSHUA_SITARA_RTC_DEV  = "/dev/rtc0";
static constexpr const char* JOSHUA_SITARA_RTC_SYSFS = "/sys/class/rtc/rtc0";

// Battery / power-supply monitor.
static constexpr const char* JOSHUA_SITARA_BATTERY_SYSFS = "/sys/class/power_supply/BAT0";
static constexpr const char* JOSHUA_SITARA_BATTERY_VOLTAGE = "/sys/class/power_supply/BAT0/voltage_now";

// Bluetooth radio (BlueZ).
static constexpr const char* JOSHUA_SITARA_BT_HCI = "hci0";
static constexpr const char* JOSHUA_SITARA_BT_SYSFS = "/sys/class/bluetooth/hci0";

// MIPI CSI-2 camera + MIPI DSI display interfaces.
static constexpr const char* JOSHUA_SITARA_CSI_DEVICE2 = "/dev/video2";
static constexpr const char* JOSHUA_SITARA_CSI_MEDIA1  = "/dev/media1";
static constexpr const char* JOSHUA_SITARA_DSI_DEV     = "/dev/fb1";
static constexpr const char* JOSHUA_SITARA_DSI_PANEL   = "/sys/class/display";

// IMU/accelerometer (TI IIO or PRU-assisted I2C sensor bus).
static constexpr const char* JOSHUA_SITARA_ACCEL_BASE = "/sys/bus/iio/devices";
static constexpr const char* JOSHUA_SITARA_ACCEL_ACCEL_X = "/sys/bus/iio/devices/iio:device0/in_accel_x_raw";
static constexpr int         JOSHUA_SITARA_ACCEL_RANGE_G = 8;

// Optional external AI accelerators.
static constexpr const char* JOSHUA_SITARA_HAILO8_PCIE_VENDOR = "1e60";
static constexpr const char* JOSHUA_SITARA_CORAL_USB_VENDOR   = "18d1";

// Platform identification.
static constexpr const char* PLATFORM_NAME      = "Joshua Sitara Platform";
static constexpr const char* PLATFORM_SOC       = "Sitara AM335x/AM62x/AM64x";
static constexpr const char* PLATFORM_IOBRIDGE  = "PRU-ICSS";
static constexpr const char* JOSHUA_SITARA_DT_MODEL_PATH = "/proc/device-tree/model";
