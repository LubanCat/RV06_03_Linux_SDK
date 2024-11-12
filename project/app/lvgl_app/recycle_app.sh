#!/bin/bash

mkdir -p /root

# shell 读取一个json文件，判断“attribute”，如果是“server”设置APP_NAME="recycle_app" ，如果是“client”设置APP_NAME="recycle_app_client"

# 读取json文件
json_file="/userdata/device.json"

attribute=$(jq -r '.attribute' "$json_file")
# 判断attribute的值并设置APP_NAME
if [ "$attribute" == "server" ]; then
    APP_NAME="recycle_app"
elif [ "$attribute" == "client" ]; then
    APP_NAME="recycle_app_client"
else
    echo "未知的attribute值: $attribute"
    exit 1
fi

# recycle_app 的路径
APP_PATH="/oem/usr/bin/"${APP_NAME}

# recycle_app 更新包的路径
UPDATE_PATH="/root/"${APP_NAME}

# 守护进程日志文件
LOG_FILE="/var/log/"${APP_NAME}"_monitor.log"

# 确保脚本使用绝对路径
cd "$(dirname "$0")"

# 启动 recycle_app 的函数
start_recycle_app() {
    echo "$(date +'%Y-%m-%d %H:%M:%S') - 检查 recycle_app 权限" >> "$LOG_FILE"
    
    # 检查是否有执行权限
    if [ ! -x "$APP_PATH" ]; then
        echo "$(date +'%Y-%m-%d %H:%M:%S') - recycle_app 没有执行权限，正在修改权限" >> "$LOG_FILE"
        chmod +x "$APP_PATH"
        
        # 再次检查是否成功修改权限
        if [ ! -x "$APP_PATH" ]; then
            echo "$(date +'%Y-%m-%d %H:%M:%S') - 修改权限失败" >> "$LOG_FILE"
            exit 1
        fi
    fi
    
    echo "$(date +'%Y-%m-%d %H:%M:%S') - 启动 recycle_app" >> "$LOG_FILE"
    # $APP_PATH &
    # $APP_PATH >> "$LOG_FILE" 2>&1
    $APP_PATH >> /dev/ttyFIQ0 2>&1
    APP_PID=$!
    echo "$(date +'%Y-%m-%d %H:%M:%S') - recycle_app 启动成功，PID: $APP_PID" >> "$LOG_FILE"
}

# 检测 recycle_app 是否运行的函数
check_recycle_app() {
    if ! kill -0 $APP_PID 2>/dev/null; then
        echo "$(date +'%Y-%m-%d %H:%M:%S') - recycle_app 已退出，重新启动..." >> "$LOG_FILE"
        start_recycle_app
    fi
}

# 检测 recycle_app 是否有更新版本
check_recycle_update() {
    if [ -e $UPDATE_PATH ]; then
        echo "$(date +'%Y-%m-%d %H:%M:%S') - recycle_app 正在更新..." >> "$LOG_FILE"
        mv $UPDATE_PATH $APP_PATH
    fi
}

# 将脚本置于后台运行
if [ "$1" != "daemon" ]; then
    nohup "$0" daemon > /dev/null 2>&1 &
    exit 0
fi

# 主循环
check_recycle_update
start_recycle_app
while true; do
    check_recycle_update
    check_recycle_app
    sleep 10
done
