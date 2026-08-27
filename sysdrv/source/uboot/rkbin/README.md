# rkbin Repository

The rkbin repository is mainly used to store files that may be used in the early boot stages on Rockchip platforms, including executable binaries, configuration files, tools, and more.

[TOC]

## File Types

Files in the rkbin repository are mainly divided into the following categories:

| Type | Description |
|------|-------------|
| **binary** | Boot-related binaries such as SPL/DDR/UsbPlug/BL31/BL32/OPTEE, usually built independently from a single repository |
| **mcu** | Binaries usually built jointly from multiple repositories |
| **syscfg** | Global configuration files on MOS platforms |
| **others** | Tools (`tools`), configuration files (`ini`), etc. |


## File Naming

The naming convention for binary files is as follows:

```
[platform]_[component]_[feature]_[version].[postfix]
```

**Naming examples:**

```
bin/rk35/rk3562_spl_v1.07.bin
bin/rk35/rk3562_ddr_1332MHz_D4_LP4_4x_eyescan_v1.09.bin
bin/rk35/rk3528_usbplug_v1.04.bin
bin/rk35/rk3562_bl31_v1.23.elf
bin/rk35/rk3562_bl32_v1.08.bin
```


## Submission Principles

1. **Source-first principle**: Patches in the source repository must be merged first. Then build the target file and submit it to rkbin. Unless there is a special case, submitting temporary or dirty versions to rkbin is prohibited.

   > Dirty version: a version built from local temporary changes that have not yet been committed with git commit.

2. **Traceability principle**: The commit message must be clear enough so that the current version can be **traced** back to the corresponding source repository.

3. **Version management principle**: The binary version must not be lower than v1.00. When updating, you must increment the version number and update the release document.

## Compliance Check

**Pre-submission check**: Run `./scripts/checkpatch.sh`.

> Note: `checkpatch.sh` checks the content of the first commit, so please commit locally before running the script.

## Merge Principle

When there is no -1, at least one review +1 from the patch owner or a reviewer is required before the patch can be merged.


## Commit Message Guidelines

This document defines commit message format requirements for patches of different file types. The goals are:

- Make the current version traceable to the source repository through the commit message
- Unify the commit style
- `Build from...` and `Update features...` are currently mandatory. If you are unsure what to put in `Update features`, it is recommended to use the same content as `Build from`.

> **Note**: This document only defines the basic guideline requirements. To improve traceability, submitters may make reasonable adjustments based on these rules.


### binary

Format template:

```
[platform]: [component]: Update version to [version]

Build from commit:
        <commit point in the source repository used to build the current binary file>

Update feature:
        <related commit records or textual description; choose one of the two>

Build command:  (optional)
        <build command>

Signed-off-by: Your Name <your.email@example.com>
Change-Id: Ixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```

Commit example:

```
rk3506: tee: Update version to v2.30

Build from commit:
        a56a36a8f29 rockchip: rk3506: Enable gpio group

Update feature:
        a56a36a8f29 rockchip: rk3506: Enable gpio group
        0b9994dc96c rockchip: common: Add gpio group support
        f866052154d rk3506: soc: enable tsadc_shut_m0

Build command:
        ./scripts/build_optee_os.sh rk3506

Signed-off-by: Joseph Chen <chenjh@rock-chips.com>
Change-Id: Ief074b1525cdab04160960fc05fc6995c1d9d5ab
```

Description:
- `[platform]: [component]: Update version to [version]` - **required**, title format
- `Build from commit` - **required**. Pay special attention: this commit refers to the source repository commit point used to build the current binary file.
- `Update feature` - **required**. Choose one of two forms: (1) provide related commit records, or (2) provide a textual description.
- `Build command` - optional. Provide the build command for reproduction.

---

### mcu

Format template:

```
[platform]: mcu: Update [mcu_name] version to [version]

Build from commit:
        <summary of build points from each repository, separated by #>

Build from [sub-repository 1] commit in [branch] branch:
        <build point of sub-repository 1>

Build from [sub-repository 2] commit in [branch] branch:
        <build point of sub-repository 2>

...

Signed-off-by: Your Name <your.email@example.com>
Change-Id: Ixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```

Commit example:

```
rv1106: mcu: Update rv1106_hpmcu_tb_sc450ai version to v1.91

Build from commit:
        rtt:4f71a2d#hal:8966acbf#battery_ipc:1497fb5

Build from rt-thread commit in master branch:
        4f71a2d: bsp: rockchip: board: add sc450ai boardcfg

Build from hal commit in master branch:
        8966acbf: rv1106-mcu: Optimize bss zero init time cost

Build from battery_ipc commit in master branch:
        1497fb5: fastae: 2.3.2-rc1

Signed-off-by: Lan Honglin <helin.lan@rock-chips.com>
Change-Id: I805514e21b953e20764a563176205edefff87d8a
```

Description:
- `[platform]: mcu: Update [mcu_name] version to [version]` - **required**, title format
- `Build from commit` - **required**, a summary of build points from each repository, separated by `#`
- `Build from [sub-repository name] commit in [branch name] branch` - **required**, list the build point of each sub-repository separately

---

### syscfg

Format template:

```
[platform]: syscfg: [config_files]: [What's_your_update]

Build from [branch] commit:
        <build point in the source repository>

Update feature:
        <related commit records or textual description>

Build command:  (optional)
        <command used to generate the configuration file>

Signed-off-by: Your Name <your.email@example.com>
Change-Id: Ixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```

Commit example:

```
rk3576: syscfg: vehicle-evb20/evb21: Add system suspend support

Build from develop-6.1-mos commit:
        2583d4e4a31 rtc: rockchip mos: Reduce the timeout waiting time

Update feature:
        2583d4e4a31e rtc: rockchip mos: Reduce the timeout waiting time
        35253489b829 misc: vehicle: vehicle-notify: use mos_primary_req_mos_suspend
        f6854b3c8170 arm64: dts: rockchip: rk3576-mos: support mos suspend
        da217237af6c irqchip/gic: support mos suspend

Signed-off-by: Luo Wei <lw@rock-chips.com>
Change-Id: I0eb2c2b8362eaaa18b4e061b31036409a9663e40
```

Description:
- `[platform]: syscfg: [config_files]: [What's_your_update]` - **required**, title format
- `Build from [branch] commit` - **required**, the build point in the source repository
- `Update feature` - **required**, related commit records or textual description
- `Build command` - optional. Provide the command used to generate the configuration file.

---

### others

Format template:

```
tools: [component]: Update version to [version]

Update features:
        1. <update content 1>
        2. <update content 2>
        ...

<other related information, optional>

Signed-off-by: Your Name <your.email@example.com>
Change-Id: Ixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```

Commit example:

```
tools: ddrbin_tool: Update version to v1.28

Update features:
        1. support rk3538
        2. add cs driver strength support
        3. add clk compensate phase support

Signed-off-by: Zhihuan He <huan.he@rock-chips.com>
Change-Id: I0d35a045596666aee54be6e2cf7efd01788bcc58
```

Description:
- `tools: [component]: Update version to [version]` - **required**, title format
- `Update features` - **required**, update content list
- Other related information - optional

## Release Documents

### Content Requirements

When binary files are updated, please update both the Chinese and English documents under `doc/release/` accordingly. Refer to the existing documents for the specific format. Important notes:

- `File`: If multiple files are updated, `{}` can be used as a wildcard expression.

- `Build commit`: The build point in the source repository.

- `Importance`: critical > important > moderate.

  > Chinese version: 紧急 > 重要 > 普通。

- `New`: Use this when new features are updated, and describe them
  item by item in text form. Each item must have a sequence number,
  even if there is only one item.

  For example:

  ```
  1. Add ...
  2. Improve ...
  ```

- `Fixed`: Use this when fixing issues, and describe them item by item in table form.

- `Issue source`: Fill in as needed. It can be a Redmine ID or another textual description. If there is none, fill in `-`.

- Insert a horizontal separator at the end after the update is complete: `------`.

- Run the pre-submission check: `./scripts/checkpatch.sh`.

### **Auto Generation**

In addition to manually adding release document content, you can also modify `scripts/doc-template.txt` as needed, and then run the following command to automatically generate the corresponding content for `doc/release/*.md`.

```shell
./scripts/release-doc.sh -f scripts/doc-template.txt
```

> With this approach, you only need to focus on the document content itself, without worrying about the Markdown document structure or content format.

## Tools

### FIT

- mkimage: only used for packaging FIT non-secure firmware; does not support secure firmware packaging and signing.
- `uboot/`: tool directory used by the next-dev branch and v5 branch. The mkimage in this directory supports FIT non-secure firmware packaging and secure firmware packaging and signing.
- fit-repack.sh: used for re-signing FIT secure firmware.
- fit-sign.sh: used for standalone signing of FIT firmware.
- fit-unpack.sh: used for unpacking FIT firmware.
- dtc: used for generating FIT firmware.

### Firmware

- resource_tool: used for packaging and unpacking resource.img.
- boot_merger: used for packaging Loader.
- rk_sign_tool: used for signing Loader.
- trust_merger: used for packaging RK-format trust.img (deprecated).
- loaderimage: used for packaging RK-format uboot.img and trust.img (deprecated).
- mkkrnlimg: used for packaging RK-format kernel.img (deprecated).
- programmer_image_tool: used for converting update.img to an offline programmer image.
- `burner_image_kits/`: used for creating offline programmer images through the command line (programmer_image_tool is recommended).
- pcie_idb_config.sh: used for setting PCIe parameters of pcie_idb.img.

### DDR

- ddrbin_tool.py: used for modifying DDR bin configurations.
- ddrbin_param.txt
- ddrbin_tool_user_guide.txt

### Flashing

- rkdeveloptool: used for flashing firmware (deprecated).
- upgrade_tool: used for flashing firmware.

### Others

- gpt2env: used for converting parameter.txt to a GPT partition table recognized by U-Boot.
- bmp2gray16: used for image format conversion.
