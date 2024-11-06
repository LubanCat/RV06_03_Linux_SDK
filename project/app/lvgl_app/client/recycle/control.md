# 串口通讯控制

## 协议

使用json格式传输内容

## 指令列表

### 投递门控制指令

```json
{
    "cmd": "PostDoorCtrl",
    "ctrl": "open",
    "rod_value":100
}
```

描述：投递门控制指令

"ctrl" :字符串  

* "open" 为打开投递门
* "close" 为关闭投递门参数。

"rod_value" :整形数据 0-100，投递杆的力度。

```json
{
    "cmd": "PostDoorCtrl",
    "retCode": 0
}
```

描述：投递门控制指令返回
"retCode" : 整形数据 

* 0为成功，
* 1为失败

### 外灯控制指令

```json
{
    "cmd": "OutLightCtrl",
    "ctrl": "open"
}
```

描述：外灯控制指令
"ctrl" :字符串  

* "open" 为打开外灯
* "close" 为关闭外灯参数。

```json
{
    "cmd": "OutLightCtrl",
    "retCode": 0
}
```

描述：外灯控制指令返回
"retCode" -> 整形数据 

* 0为成功
* 1为失败

### 内灯控制指令

```json
{
    "cmd": "InLightCtrl",
    "ctrl": "open"
}
```

描述：内灯控制指令
"ctrl" -> 字符串 

* 1为打开内灯
* 0为关闭内灯参数。

```json
{
    "cmd": "InLightCtrl",
    "retCode": 0
}
```

描述： 内灯控制指令返回
"retCode" -> 整形数据

* 0为成功
* 1为失败

### 红外控制指令

```json
{
    "cmd": "IRCtrl",
    "ctrl": "get_state"
}
```

描述：红外控制指令

```json
{
    "cmd": "IRCtrl",
    "state": 1,
    "retCode": 0
}
```

描述：红外控制指令返回
"state" -> 整形数据

*0为红外灯都灭
*1为下面的红外亮
*2为上面的红外亮
*3为都亮。

"retCode" -> 整形数据 

* 0为成功
* 1为失败

### 重量控制指令

```json
{
    "cmd": "WeightCtrl",
    "ctrl": "get_raw_data"
}
```

描述：重量控制指令

```json
{
    "cmd": "WeightCtrl",
    "adc0": 99999,
    "adc1": 99999,
    "retCode": 0
}
```

描述：重量控制指令返回
"adc0" -> 整形数据 0-16777215，表示重量传感器的ADC0值。
"adc1" -> 整形数据 0-16777215，表示重量传感器的ADC1值。
"retCode" -> 整形数据 0为成功，1为失败

### 清运门控制指令

```json
{
    "cmd": "CleanDoorCtrl",
    "ctrl": "open"
}
```

描述：清运门控制指令
"ctrl" -> 字符串

* "open" 为打开投递门

```json
{
    "cmd": "CleanDoorCtrl",
    "retCode": 0
}
```

----
----

```json
{
    "cmd": "GetCleanDoor",
    "ctrl": "get_state"
}
```

描述：清运门控制指令
"ctrl" -> 字符串

* "open" 为打开投递门

```json
{
    "cmd": "GetCleanDoor",
    "state": 1,
    "retCode": 0
}
```

描述：清运门控制指令返回
"state" -> 整形数据

* 0为门关闭
* 1为门打开。

"retCode" -> 整形数据 

* 0为成功
* 1为失败

### 更新控制指令

```json
{
    "cmd": "UpdateCtrl",
    "ctrl": "get_version"
}
```

描述：更新控制指令
"ctrl" -> 字符串

* "get_version" 获取固件版本号
* "update" 更新固件

```json
{
    "cmd": "UpdateCtrl",
    "version": "1.0",
    "retCode": 0
}
```

描述：更新控制指令返回
"version" -> 字符串 固件版本号

"retCode" -> 整形数据 

* 0为成功
* 1为失败

### 喇叭控制指令

```json
{
    "cmd": "SpeakerCtrl",
    "volume": 50
}
```

描述：喇叭控制指令
"ctrl" -> 字符串

"volume" -> 整形数据 音量 0-6

```json
{
    "cmd": "SpeakerCtrl",
    "retCode": 0
}
```

描述：喇叭控制指令返回
"retCode" -> 整形数据

* 0为成功
* 1为失败


### 文件传输指令

```json
{
    "cmd": "FileCtrl",
    "ctrl": "upload",
    "filename": "test.txt",
    "filesize": 1024,
    "filepath": "/root/test.txt",
    "md5": "1234567890",
}
```

描述：文件传输指令
"ctrl" -> 字符串

* "upload" 上传文件

"filename" -> 字符串 文件名

"filesize" -> 整形数据 文件大小

"filepath" -> 字符串 文件路径

"md5" -> 字符串 文件md5值

```json
{
    "cmd": "FileCtrl",
    "ctrl": "ready"
}
```

* ready
* resend
* success


```json
{
    "cmd": "FileCtrl",
    "retCode": 0
}
```

描述：文件传输指令返回
"retCode" -> 整形数据

* 0为成功
* 1为失败

### 机器控制指令

```json
{
    "cmd": "MachineCtrl",
    "ctrl": "reboot"
}
```

描述：机器控制指令
"ctrl" -> 字符串

* "reset" 重置设备

```json
{
    "cmd": "MachineCtrl",
    "retCode": 0
}
```

描述：重置控制指令返回
"retCode" -> 整形数据
