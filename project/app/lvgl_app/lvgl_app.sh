#!/bin/bash

# lvgl_app 的路径
APP_PATH="/oem/usr/bin/lvgl_app"

# 守护进程日志文件
LOG_FILE="/var/log/lvgl_app_monitor.log"

# 确保脚本使用绝对路径
cd "$(dirname "$0")"

# 启动 lvgl_app 的函数
start_lvgl_app() {
    echo "$(date +'%Y-%m-%d %H:%M:%S') - 启动 lvgl_app" >> "$LOG_FILE"
    $APP_PATH &
    APP_PID=$!
}

# 检测 lvgl_app 是否运行的函数
check_lvgl_app() {
    if ! kill -0 $APP_PID 2>/dev/null; then
        echo "$(date +'%Y-%m-%d %H:%M:%S') - lvgl_app 已退出，重新启动..." >> "$LOG_FILE"
        start_lvgl_app
    fi
}

# 将脚本置于后台运行
if [ "$1" != "daemon" ]; then
    nohup "$0" daemon > /dev/null 2>&1 &
    exit 0
fi

# 主循环
start_lvgl_app
while true; do
    check_lvgl_app
    sleep 10
done
