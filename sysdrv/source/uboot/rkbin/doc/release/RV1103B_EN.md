# RV1103B Release Note

## rv1103b_ddr_924MHz{_tb}_v1.07.bin

| Date       | File                              | Build commit | Severity  |
| ---------- | :-------------------------------- | ------------ | --------- |
| 2026-06-18 | rv1103b_ddr_924MHz{_tb}_v1.07.bin | 2bf646c6e6   | important |

### New

1. Add non-KGD package support.
2. Add support for 8-bit bus width detection.

------

## rv1103b_tee_ta_v1.05.bin

| Date       | File                     | Build commit | Severity  |
| ---------- | :----------------------- | ------------ | --------- |
| 2026-04-29 | rv1103b_tee_ta_v1.05.bin | 5f256dc1a    | important |

### New

1. OAEP decode compatible with MGF1 = SHA1.
2. Optimize sleep and wake-up.

### Fixed

| Index | Severity  | Update                                   | Issue description                                            | Issue source |
| ----- | --------- | ---------------------------------------- | ------------------------------------------------------------ | ------------ |
| 1     | important | Fix the low-probability data abort issue | The reboot of the device during secure storage may trigger it with a low probability | -            |

------

## rv1103b_spl_v1.01.bin

| Date       | File                  | Build commit | Severity  |
| ---------- | :----------------------- | ----------- | -------- |
| 2026-03-02 | rv1103b_spl_v1.01.bin | a27a89c5fa1   | important     |

### Fixed

| Index | Severity  | Update                                  | Issue description                   | Issue source |
| ----- | --------- | --------------------------------------- | ----------------------------------- | ------------ |
| 1     | important  | Avoid premature decompression termination by decom. | SPL hw decompression of uboot failed. | -            |

------

## rv1103b_usbplug_auto_merge_v1.11.bin

| Date       | File                               | Build commit                                   | Severity |
| ---------- | :--------------------------------- | ---------------------------------------------- | -------- |
| 2026-02-10 | rv1103b_usbplug_auto_merge_v1.11.bin | 3bb9568b5 | moderate |

### New

1. Enable SFDP.

------

## rv1103b_ddr_924MHz{_tb}_v1.06.bin

| Date       | File                              | Build commit | Severity  |
| ---------- | :-------------------------------- | ------------ | --------- |
| 2025-10-10 | rv1103b_ddr_924MHz{_tb}_v1.06.bin | b971cce143   | important |

### New

1. Weak pull up UART0M0 RX to prevent it from being switched to JTAG.
2. Add rzq calibration function.

### Fixed

| Index | Severity  | Update                               | Issue description                                            | Issue source |
| ----- | --------- | ------------------------------------ | ------------------------------------------------------------ | ------------ |
| 1     | important | Fixing DDR initialization hang issue | Board with high PLL power supply ripple exhibits intermittent boot hang | -            |
| 2     | important | Fixing gate training abnormal issue  | Intermittent training result anomaly with boot failure       | -            |

------

## rv1103b_usbplug_spinand_cont_v1.10.bin

| Date       | File                     | Build commit | Severity  |
| ---------- | :----------------------- | ------------ | --------- |
| 2025-07-28 | rv1103b_usbplug_spinand_cont_v1.10.bin | d86aef4ef0d3    | moderate |

### New

1. Support mxic and gigadevice spinand continues read mode.

------

## rv1103b_tee_ta_v1.04.bin

| Date       | File                     | Build commit | Severity  |
| ---------- | :----------------------- | ------------ | --------- |
| 2025-07-04 | rv1103b_tee_ta_v1.04.bin | 76f8f9d23    | important |

### New

1. Support User TA to use OEM OTP KEY for encryption and decryption calculations.

### Fixed

| Index | Severity  | Update                          | Issue description                  | Issue source |
| ----- | --------- | ------------------------------- | ---------------------------------- | ------------ |
| 1     | important | Merge official security patches | Address potential security risks   | -            |
| 2     | important | Reduce shared memory size       | Compatible with 64M memory devices | -            |

------

## rv1103b_tee_ta_v1.03.bin

| Date       | File                     | Build commit | Severity  |
| ---------- | :----------------------- | ------------ | --------- |
| 2024-12-17 | rv1103b_tee_ta_v1.03.bin | 6b118d039    | important |

### New

1. Supports hardware crypto and true random number generator TRNG.

------

## rv1103b_tee_ta_v1.02.bin

| Date       | File                     | Build commit | Severity  |
| ---------- | :----------------------- | ------------ | --------- |
| 2024-11-01 | rv1103b_tee_ta_v1.02.bin | 9f2aca7d1    | important |

### Fixed

| Index | Severity  | Update                                                       | Issue description                                            | Issue source |
| ----- | --------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------ |
| 1     | important | check whether the rpmb key has been burned before changing security level | upgrading from weak security level to strong security level may result in rpmb key verification failure | -            |
| 2     | important | fixed RSA OAEP MGF1 algorithm                                | TA will report an error when using RSA algorithm OAEP MGF1 padding method | -            |

------

## rv1103b_hpmcu_wrap_v2.02.bin

| Date       | File                         | Build commit     | Severity  |
| ---------- | :--------------------------- | ---------------- | --------- |
| 2024-10-22 | rv1103b_hpmcu_wrap_v2.02.bin | rockit_ko:26f0ca4 | important |

### New

1. Support double channel wrap.

------

## rv1103b_ddr_924MHz{_tb}_v1.05.bin

| Date       | File                              | Build commit | Severity  |
| ---------- | :-------------------------------- | ------------ | --------- |
| 2024-10-14 | rv1103b_ddr_924MHz{_tb}_v1.05.bin | a0d2414c29   | important |

### Fixed

| Index | Severity  | Update                                          | Issue description | Issue source |
| ----- | --------- | ----------------------------------------------- | ----------------- | ------------ |
| 1     | important | Fix DDR3 924MHz probabilistic resume hang issue | -                 | -            |

------

## rv1103b_tee_ta_v1.01.bin

| Date       | File                     | Build commit | Severity  |
| ---------- | :----------------------- | ------------ | --------- |
| 2024-10-14 | rv1103b_tee_ta_v1.01.bin | 066b2fbeb    | important |

### New

1. Modify the TEE loading address to 62M.

------


## rv1103b_flash_acc_w25n01xx_v1.00.bin

| Date       | File                              | Build commit | Severity  |
| ---------- | :--------------------------------------- | ----------- | -------- |
| 2024-10-08 | rv1103b_flash_acc_w25n01xx_v1.00.bin        | c2c14bb7e419  | important     |

### New

1. Add w25n01jw rom accelerator.

------

## rv1103b_usbplug_auto_merge_v1.10.bin

| Date       | File                               | Build commit                                   | Severity |
| ---------- | :--------------------------------- | ---------------------------------------------- | -------- |
| 2024-09-29 | rv1103b_usbplug_auto_merge_v1.10.bin | f9ecba2b | moderate |

### New

1. Change cs1 to gpio2a6.

------

## rv1103b_usbplug_auto_merge_v1.00.bin

| Date       | File                               | Build commit                                   | Severity |
| ---------- | :--------------------------------- | ---------------------------------------------- | -------- |
| 2024-09-25 | rv1103b_usbplug_auto_merge_v1.00.bin | a87eca5 | moderate |

### New

1. Initial version.

------

## rv1103b_ddr_924MHz{_tb}_v1.04.bin

| Date       | File                              | Build commit | Severity  |
| ---------- | :-------------------------------- | ------------ | --------- |
| 2024-08-30 | rv1103b_ddr_924MHz{_tb}_v1.04.bin | ccb664bdcf   | important |

### New

1. Improve DDR stability.

------

## rv1103b_ddr_924MHz{_tb}_v1.03.bin

| Date       | File                              | Build commit | Severity  |
| ---------- | :-------------------------------- | ------------ | --------- |
| 2024-08-26 | rv1103b_ddr_924MHz{_tb}_v1.03.bin | b991ae72ff   | important |

### Fixed

| Index | Severity  | Update                                   | Issue description | Issue source |
| ----- | --------- | ---------------------------------------- | ----------------- | ------------ |
| 1     | important | Fix gate training timeout of DDR2 528MHz | -                 | -            |
| 2     | important | Fix isp mipi drop when 4M 60fps          | -                 | -            |
| 3     | important | Fix DDR setting for power-saving         | -                 | -            |

------

## rv1103b_tee_ta_v1.00.bin

| Date       | File                     | Build commit | Severity  |
| ---------- | :----------------------- | ------------ | --------- |
| 2024-08-20 | rv1103b_tee_ta_v1.00.bin | ed4478bc9    | important |

### New

1. Add OPTEE support.

------

## rv1103b_{ddr,spl,usbplug,hpmcu}_v1.00.bin

| Date       | File                               | Build commit                                   | Severity |
| ---------- | :--------------------------------- | ---------------------------------------------- | -------- |
| 2024-07-29 | rv1103b_{ddr,spl,usbplug,hpmcu}_v1.00.bin | ddr:1b742cd9d6#spl:3687236ab1c:usbplug:c53c564#rtt:3143c22c#hal:939ec3d5#battery_ipc:06ccc158 | moderate |

### New

1. Initial version.

------

