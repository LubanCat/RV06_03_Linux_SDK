# rkbin 仓库

rkbin 仓库主要用于存放 Rockchip 平台上前级启动阶段可能用到的文件，包括：可执行二进制文件、配置文件、工具等。

[TOC]

## 文件类型

rkbin 仓库中的文件主要分为以下几类：

| 类型 | 说明 |
|------|------|
| **binary** | SPL/DDR/UsbPlug/BL31/BL32/OPTEE 等启动相关的二进制文件，通常由单仓库独立编译得到 |
| **mcu** | 通常由多仓库联合编译得到的二进制文件 |
| **syscfg** | MOS 平台上的全局配置文件 |
| **others** | 工具（tools）、配置文件（ini）等 |


## 文件命名

Binary 文件的命名规则如下：

```
[platform]_[component]_[feature]_[version].[postfix]
```

**命名示例：**

```
bin/rk35/rk3562_spl_v1.07.bin
bin/rk35/rk3562_ddr_1332MHz_D4_LP4_4x_eyescan_v1.09.bin
bin/rk35/rk3528_usbplug_v1.04.bin
bin/rk35/rk3562_bl31_v1.23.elf
bin/rk35/rk3562_bl32_v1.08.bin
```


## 提交原则

1. **源码优先原则**：源码仓库的补丁必须先合并，再编译出目标文件提交到 rkbin，非特殊情况禁止提交临时或 dirty 版本到 rkbin

   > dirty版本：本地有临时改动还未git commit 就进行代码编译而得到的版本。

2. **可溯源性原则**：必须有足够清晰的 commit message，让当前版本能够在对应的源码仓库中被**溯源**

3. **版本管理原则**：binary 版本不低于 v1.00，更新时必须：提升版本号、更新 release 文档

## 合规检查

**提交前检查**：执行 `./scripts/checkpatch.sh`

> 注意：checkpatch.sh是对第一个commit的内容进行检查，所以执行脚本前请先在本地commit。

## 合并原则

没有任何 -1 的情况下至少一个 review +1 来自补丁 owner 或 reviewer，补丁才可能被合并。


## 提交规范

本文档对不同类型的文件补丁做出 commit message 格式要求，目标是：

- 通过提交信息，当前版本能在源码仓库中被溯源
- 统一提交风格
- 目前 `Build from...` 和 `Update features...`是必填项，如果不知道`Update features`的内容应该填写什么，建议跟`Build from`内容保持一致即可。

> **注意**：本文档只是做出了基本的规范要求，提交者为了更准确溯源可以在此基础规则上自行做出合理调整。


### binary

格式模板：

```
[platform]: [component]: Update version to [version]

Build from commit:
        <源码仓库编译当前二进制文件时的提交点>

Update feature:
        <相关的 commit 记录或文字说明，二选一>

Build command:  (可选)
        <编译命令>

Signed-off-by: Your Name <your.email@example.com>
Change-Id: Ixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```

提交示例：

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

说明：
- `[platform]: [component]: Update version to [version]` - **强制要求**，标题格式
- `Build from commit` - **强制要求**，特别注意：这个 commit 指的是源码仓库编译当前二进制文件时的提交点
- `Update feature` - **强制要求**，形式二选一：（1）附上相关的 commit 记录或（2）文字说明
- `Build command` - 可选，提供编译命令以便复现

---

### mcu

格式模板：

```
[platform]: mcu: Update [mcu_name] version to [version]

Build from commit:
        <各仓库编译点的汇总，用 # 隔开>

Build from [子仓库1] commit in [branch] branch:
        <子仓库1的编译点>

Build from [子仓库2] commit in [branch] branch:
        <子仓库2的编译点>

...

Signed-off-by: Your Name <your.email@example.com>
Change-Id: Ixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```

提交示例：

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

说明：
- `[platform]: mcu: Update [mcu_name] version to [version]` - **强制要求**，标题格式
- `Build from commit` - **强制要求**，各仓库编译点的汇总，用 `#` 隔开
- `Build from [子仓库名] commit in [分支名] branch` - **强制要求**，每个子仓库的编译点单独列出

---

### syscfg

格式模板：

```
[platform]: syscfg: [config_files]: [What's_your_update]

Build from [branch] commit:
        <源码仓库的编译点>

Update feature:
        <相关的 commit 记录或文字说明>

Build command:  (可选)
        <生成配置文件的命令>

Signed-off-by: Your Name <your.email@example.com>
Change-Id: Ixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```

提交示例：

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

说明：
- `[platform]: syscfg: [config_files]: [What's_your_update]` - **强制要求**，标题格式
- `Build from [branch] commit` - **强制要求**，源码仓库的编译点
- `Update feature` - **强制要求**，相关的 commit 记录或文字说明
- `Build command` - 可选，提供生成配置文件的命令

---

### others

格式模板：

```
tools: [component]: Update version to [version]

Update features:
        1. <更新内容1>
        2. <更新内容2>
        ...

<其他相关信息，可选>

Signed-off-by: Your Name <your.email@example.com>
Change-Id: Ixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```

提交示例：

```
tools: ddrbin_tool: Update version to v1.28

Update features:
        1. support rk3538
        2. add cs driver strength support
        3. add clk compensate phase support

Signed-off-by: Zhihuan He <huan.he@rock-chips.com>
Change-Id: I0d35a045596666aee54be6e2cf7efd01788bcc58
```

说明：
- `tools: [component]: Update version to [version]` - **强制要求**，标题格式
- `Update features` - **强制要求**，更新内容列表
- 其他相关信息 - 可选

## Release文档

### 内容要求

Binary文件更新时请同步更新`doc/release/`下的中、英文档，具体格式请参考现有文档。几点重要说明：

- `文件`：如果更新的是多个文件，可用`{}` 模糊表示。

- `编译 commit`：源码仓库的编译点。

- `重要程度`：紧急 > 重要 > 普通。

  > 英文版本：critical > important > moderate。

- `New`：有新的feature更新时启用，以文字形式逐项说明。每一项说明都必须有顺序编号（即使只有一项）。

  例如：

  ```
  1. 新增 ...
  2. 提升 ...
  ```

- `Fixed`：修复问题时启用，以表格形式逐项说明。

- `问题来源`：按需填写。可以是redmine的ID或其它文字说明；没有时填写为`-`。

- 更新完成后在末尾插入水平分隔符：`------`。

- 提交前执行检查： `./scripts/checkpatch.sh` 。

### **自动生成**

除了手动添加Release文档内容，用户还可以按需修改 `scripts/doc-template.txt` ，然后执行下面的命令自动生成对应的`doc/release/*.md`内容。

```shell
./scripts/release-doc.sh -f scripts/doc-template.txt
```

> 这种方式下用户只需要关注文档内容本身，不需要关心md文档和内容格式。

## Tools

### FIT

- mkimage：只用于 FIT 非安全固件打包，不支持安全固件打包签名。
- `uboot/`：next-dev 分支和 v5 分支使用的工具目录，其中 mkimage 支持 FIT 非安全固件打包和安全固件打包签名。
- fit-repack.sh：用于 FIT 安全固件重签名。
- fit-sign.sh：用于 FIT 固件独立签名。
- fit-unpack.sh：用于 FIT 固件解包。
- dtc：用于生成 FIT 固件。

### 固件

- resource_tool：用于 resource.img 打包和解包。
- boot_merger：用于 Loader 打包。
- rk_sign_tool：用于 Loader 签名。
- trust_merger：用于 RK 格式 trust.img 打包（已废弃）。
- loaderimage：用于 RK 格式 uboot.img、trust.img 打包（已废弃）。
- mkkrnlimg：用于 RK 格式 kernel.img 打包（已废弃）。
- programmer_image_tool：用于将 update.img 转换为离线烧录器镜像。
- `burner_image_kits/`：用于通过命令行制作离线烧录器镜像（推荐使用 programmer_image_tool）。
- pcie_idb_config.sh：用于设置 pcie_idb.img 的 PCIe 参数。

### DDR

- ddrbin_tool.py：用于修改 DDR bin 配置。
- ddrbin_param.txt
- ddrbin_tool_user_guide.txt

### 烧写

- rkdeveloptool：用于烧写固件（已废弃）。
- upgrade_tool：用于烧写固件。

### 其它

- gpt2env：用于将 parameter.txt 转换为 U-Boot 可识别的 GPT 分区表。
- bmp2gray16：用于图片格式转换。
