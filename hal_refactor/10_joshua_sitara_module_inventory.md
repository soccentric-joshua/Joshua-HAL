# Joshua Sitara HAL Module Inventory and Retarget Map This inventory was built from all module and test directories under src/ and tests/.
The public HAL module APIs are preserved; only backend/platform bindings are retargeted. ## Backend Classes - ARM userspace backend: TI Processor SDK Linux / Yocto
- Deterministic I/O backend: PRU-ICSS firmware via remoteproc + RPMsg
- Standard peripheral backend: libgpiod + Linux device nodes (I2C/SPI/PWM/UART) ## Module-to-Joshua Mapping | Source module | Joshua target module | Backend mapping |
|---|---|---|
| audio_interfaces | audio_interfaces | ALSA/I2S on Sitara + PRU timing assist |
| camera_interfaces_lib | camera_interfaces_lib | V4L2/media graph on Sitara |
| camera_module3_lib | camera_module3_lib | Camera stack on Sitara carrier/custom board |
| clock_and_timing_lib | clock_and_timing_lib | PRU cycle timer + Linux PTP/clocksource |
| communication_interface_lib | communication_interface_lib | SPI/I2C/UART + RPMsg transport |
| cpu_lib | cpu_lib | Sitara cpufreq/thermal metrics |
| debug_and_development_interfaces | debug_and_development_interfaces | TI SDK debugfs/procfs tooling |
| deep_learning_accelerator_lib | deep_learning_accelerator_lib | PCIe/USB accelerator discovery on Sitara |
| display_interfaces | display_interfaces | DRM/KMS on Sitara |
| environmental_lib | environmental_lib | thermal/power sensors on Sitara |
| ethernet_lib | ethernet_lib | CPSW/eth PHY + PTP |
| ethernet_switch_lib | ethernet_switch_lib | DSA/ethtool/sysfs switch checks |
| external_connector_lib | external_connector_lib | carrier connector continuity + pinmux checks |
| functional_safety_lib | functional_safety_lib | watchdog/reset/safety monitors on Sitara |
| gnss_lib | gnss_lib | UART/I2C/SPI GNSS interfaces on Sitara |
| gpio_lib | gpio_lib | libgpiod for standard GPIO + PRU deterministic GPIO |
| gpu_lib | gpu_lib | DRM/devfreq metrics where available |
| imu_lib | imu_lib | I2C/SPI IMU access |
| mcu_interface_lib | mcu_interface_lib | MCU links + PRU remoteproc/RPMsg management |
| pcie_lib | pcie_lib | Sitara PCIe root-port checks |
| poe_hat_lib | poe_hat_lib | PoE rail/fan/network checks on carrier |
| power_management_lib | power_management_lib | PMIC/rail/idle/suspend checks |
| pru_icss_lib | pru_icss_lib | NEW: remoteproc/RPMsg/fault recovery manager |
| reliability_lib | reliability_lib | burn-in/thermal/stability proxies |
| security_features_lib | security_features_lib | secure boot/crypto modules on Sitara |
| sense_hat_lib | sense_hat_lib | optional accessory stack on Sitara GPIO/I2C |
| serializer_deserializer_lib | serializer_deserializer_lib | high-speed link checks with PRU assist |
| software_interface_lib | software_interface_lib | kernel/modules/services framework checks |
| storage_lib | storage_lib | MMC/NVMe/filesystem checks |
| system_integration_lib | system_integration_lib | cross-module integration orchestration |
| system_memory_lib | system_memory_lib | memory bandwidth/capacity checks |
| thermal_management_lib | thermal_management_lib | thermal zones/fan/policy checks |
| usb_lib | usb_lib | xHCI/device role tests |
| video_encode_decode_lib | video_encode_decode_lib | V4L2 codec path checks |
| vision_accelerator_lib | vision_accelerator_lib | optional accelerator integration |
| watchdog_and_reset_lib | watchdog_and_reset_lib | watchdog and reset workflows | ## Test Inventory Mapping All existing test directories are preserved and mapped one-to-one: - tests/audio_interfaces -> Joshua audio backend
- tests/camera_interfaces -> Joshua camera backend
- tests/camera_module3 -> Joshua camera-module backend
- tests/clock_and_timing -> PRU-assisted timing backend
- tests/communication_interface -> Sitara comms backend
- tests/cpu -> Sitara CPU backend
- tests/debug_and_development_interfaces -> Sitara debug backend
- tests/deep_learning_accelerator -> Sitara accelerator backend
- tests/display_interfaces -> Sitara display backend
- tests/environmental -> Sitara environmental backend
- tests/ethernet -> Sitara ethernet backend
- tests/ethernet_switch -> Sitara switch backend
- tests/external_connector -> Sitara connector backend
- tests/functional_safety -> Sitara safety backend
- tests/gnss -> Sitara GNSS backend
- tests/gpio -> libgpiod + PRU GPIO backend
- tests/gpu -> Sitara GPU backend
- tests/imu -> Sitara IMU backend
- tests/mcu_interface -> PRU remoteproc/RPMsg + MCU backend
- tests/pcie -> Sitara PCIe backend
- tests/poe_hat -> Sitara PoE backend
- tests/power_management -> Sitara power backend
- tests/reliability -> Sitara reliability backend
- tests/security_features -> Sitara security backend
- tests/sense_hat -> Sitara accessory backend
- tests/serializer_deserializer -> Sitara serdes backend
- tests/software_interface -> Sitara software-interface backend
- tests/storage -> Sitara storage backend
- tests/system_integration -> Sitara integration backend
- tests/system_memory -> Sitara memory backend
- tests/thermal_management -> Sitara thermal backend
- tests/usb -> Sitara USB backend
- tests/video_encode_decode -> Sitara video backend
- tests/vision_accelerator -> Sitara vision backend
- tests/watchdog_and_reset -> Sitara watchdog/reset backend ## PRU Contract Summary - Control plane: remoteproc state/firmware nodes
- Data plane: RPMsg endpoint /dev/rpmsg_pru30
- Deterministic services: GPIO edge capture, toggle timing, PWM generation
- Recovery: stop/start cycle and RPMsg health probe
