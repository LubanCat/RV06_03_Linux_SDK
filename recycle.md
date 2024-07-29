# 野火rv1106 recycle box维护指南

## SDK操作

### 仓库拉取
仓库地址如下，分支是 **rv1106_recycle_box**

```
git@10.255.200.106:rockchip/test_code/rv1103_ipc/rv1106_ipc_linux_v1.7.0.git
```

拉取方法
```
git clone git@10.255.200.106:rockchip/test_code/rv1103_ipc/rv1106_ipc_linux_v1.7.0.git -b rv1106_recycle_box
```

### 文档查看
RK的文档会包含很多详细的介绍

位置在``SDK/docs``目录下

```
docs/
├── en
│   ├── audio
│   ├── bsp
│   ├── ipc
│   ├── isp
│   ├── media
│   └── security
└── zh
    ├── audio
    ├── bsp
    ├── ipc
    ├── isp
    ├── iva
    ├── media
    ├── Rockchip_User_Guide_Bug_System_CN.pdf
    ├── Rockchip_User_Guide_SDK_Application_And_Synchronization_CN.pdf
    └── security
```

具体什么内容可以自行点开查看

### 编译前准备

环境安装，建议ubuntu18.04以上
```
sudo apt-get install repo git ssh make gcc \
gcc-multilib g++-multilib module-assistant \
expect g++ gawk texinfo libssl-dev \
bison flex fakeroot cmake unzip gperf autoconf \
device-tree-compiler libncurses5-dev
```

注意：若编译遇到报错，可以视报错信息，安装对应的软件包

### rv1106 recycle box 编译

```
./build.sh lunch
```

可以选择很多的版本，目前我们主要有128M和256M两个版本，对应的是spi-nand的大小。目前我们主要编译256M的版本

```
----------------------------------------------------------------
2. BoardConfig_IPC/BoardConfig-SPI_NAND-NONE-RV1106_RECYCLE_128M-IPC.mk
                             boot medium(启动介质): SPI_NAND
                          power solution(电源方案): NONE
                        hardware version(硬件版本): RV1106_RECYCLE_128M
                             application(应用场景): IPC
----------------------------------------------------------------

----------------------------------------------------------------
3. BoardConfig_IPC/BoardConfig-SPI_NAND-NONE-RV1106_RECYCLE_256M-IPC.mk
                             boot medium(启动介质): SPI_NAND
                          power solution(电源方案): NONE
                        hardware version(硬件版本): RV1106_RECYCLE_256M
                             application(应用场景): IPC
----------------------------------------------------------------
```

选好之后就可以一键编译``./build.sh``或者使用部分编译

部分编译的具体操作可以查看仓库里的 **readme_cn.txt** 或者查看下面的链接

```
http://gitlab.ebf.local/rockchip/test_code/rv1103_ipc/rv1106_ipc_linux_v1.7.0/-/blob/rv1106_recycle_box/project/readme_cn.txt
```

## 项目代码

### 编译

1. 直接在SDK的主目录下运行``./build.sh``，速度：慢   
注意:这个会比较耗费时间
2. 直接在SDK的主目录下运行``./build.sh app``，速度：中  
注意:运行这个前需要全编译一下
3. 直接切换到程序的目录下 ``cd project/app/lvgl_app`` 然后执行``./build.sh``，速度：快


项目的代码位置在``SDK/project/app/lvgl_app``一般操作只需改app即可，注意：优先修改lvgl_app里的内容

```
lvgl_app/
├── build.sh
├── cmake
├── CMakeLists.txt
├── custom
├── files
├── generated
├── include
├── lib
├── lv_app
├── lv_drivers
├── lvgl
├── lvgl_app.sh
├── lv_port
├── main.c
├── Makefile
├── network.sh
├── out
├── output
└── recycle
```

- ``build.sh``:编译脚本，可以在本目录下执行该命令编译代码
- ``custom``:存放lvgl的显示接口，程序与lvgl的通讯就是通过该接口通讯
- ``files``:存放与程序相关的媒体文件
- ``generated``:lvgl的界面代码，所有lvgl的显示代码都是存放在该位置上
- ``include``:存放第三方代码的头文件，如果要添加第三方头文件，可以添加到该地方
- ``lib``:存放第三方代码的库，如果要添加第三方库，可以添加到该地方
- ``lv_port``:存放lvgl的配置文件，三个配置文件，可以通过调整lv_port来配置lvgl
- ``lvgl``:lvgl的核心代码
- ``lv_drivers``:lvgl驱动的代码
- ``out``:程序编译出来的程序结果
- ``recycle``:存放着我们项目程序的核心代码
- ``lvgl_app.sh``:程序的守护进程
- ``main.c``:程序的主程序
- ``Makefile``:控制程序的编译
- ``network.sh``:4G网卡和以太网的自动切换脚本

``recycle`` 里放着我们的核心代码

```
lvgl_app/recycle/
├── capture.c
├── capture.h
├── hardware.c
├── hardware.h
├── module.c
├── module.h
├── rk_gpio.c
├── rk_gpio.h
├── rk_sysfs.c
├── rk_sysfs.h
├── server.c
└── server.h
```

- ``capture.x``:存放着摄像头相关的控制
- ``hardware.x``:板子外围设备的控制
- ``module.x``:4G模块的控制
- ``rk_gpio.x``:rockchip gpio的控制
- ``rk_sysfs.x``:sysfs的读写控制
- ``server.x``:主要程序的函数


## 硬件操作

### 串口连接

串口的波特率是``115200``,串口的连线方式是``Tx Rx Gnd`` ，靠近触摸排线的是``GND``

### 固件烧录

可以参照``docs/zh/ipc/Rockchip_Quick_Start_Linux_IPC_SDK_CN.pdf``

1. 安装瑞芯微驱动  
略
2. 安装烧录软件软件  
略
3. 电脑连接usb接口
4. 将板子上的绿色杜邦线断开
5. 按下板子上的boot按键
6. 接通12V的电源
7. 可以根据手册对板子进行固件烧录

### 网络调试接口
网址

```
https://api.recycling.maizucity.com/doc.html#/default/%E6%B5%8B%E8%AF%95%E6%8E%A5%E5%8F%A3/debug   

下发重启
{
  "sn": "00016000000002"
}

下发进入调试
{
  "sn": "00016000000002"
}

下发配置
{
"sn": "00016000000002",
"recyclePrice":"0.7",
"overweight":"100",
"heartBeatInterval":"180",
"turnOnLight":"2000",
"turnOffLight":"0800",
"lightTime":"2000-0800"
}
```

### 硬件测试
用到的硬件
- 电源:12V供电，建议用他们自己拿过来的电源
- usb摄像头:插法很重要，一共三个usb-a接口，摄像头插边上的两个，靠近sd卡槽的是外摄，另一个是内摄像头
- 红外:红外有两个接口，现在只要接一个，这个是设备是否溢出的检查
- 推杆:推杆是有adc的当他变长的时候会有电流变化，当出现被挡住的时候会有电流增大的情况，比阈值大的时候就会缩回去
- 内灯:一般是开内摄像头的时候去开这个灯
- 外灯:定时或者有人投递的时候开
- 称重：用来称重

### 运行逻辑

1. 上电会连接服务器
2. 连接服务器成功就会显示二维码  
注：在显示器没有成功的时候不能进调试界面
- 用微信扫描二维码后就可以进入投递衣服的页面，这时候语音播报开门的声音，内灯打开，内外摄像头进行拍照，然后捕捉usb摄像头的画面到预览窗口中，然后开始录像然后推杆收回，倒计时开始计时，直接点击关门就会关门，到了倒计时20秒之后，按键就会变成继续投递，点击继续投递后倒计时会重新开始。点击关门的时候就会存储录像，推杆变长，然后内外摄像头进行拍照，关门后上传照片，重新返回二维码的界面
- 下发进入调试可以进入调试页面，然后可以测试各种硬件
- 还有一个回收人员使用的小程序进行的回收模式，微信搜索``新翔环境运营端``,扫码使用了。
