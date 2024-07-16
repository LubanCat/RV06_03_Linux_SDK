#!/bin/bash

# lvgl_app 的路径
APP_PATH="/oem/usr/bin/lvgl_app"

# lvgl_app 更新包的路径
UPDATE_PATH="/root/lvgl_app"

# 守护进程日志文件
LOG_FILE="/var/log/lvgl_app_monitor.log"

# 确保脚本使用绝对路径
cd "$(dirname "$0")"

# 启动 lvgl_app 的函数
start_lvgl_app() {
    echo "$(date +'%Y-%m-%d %H:%M:%S') - 检查 lvgl_app 权限" >> "$LOG_FILE"
    
    # 检查是否有执行权限
    if [ ! -x "$APP_PATH" ]; then
        echo "$(date +'%Y-%m-%d %H:%M:%S') - lvgl_app 没有执行权限，正在修改权限" >> "$LOG_FILE"
        chmod +x "$APP_PATH"
        
        # 再次检查是否成功修改权限
        if [ ! -x "$APP_PATH" ]; then
            echo "$(date +'%Y-%m-%d %H:%M:%S') - 修改权限失败" >> "$LOG_FILE"
            exit 1
        fi
    fi
    
    echo "$(date +'%Y-%m-%d %H:%M:%S') - 启动 lvgl_app" >> "$LOG_FILE"
    # $APP_PATH &
    # $APP_PATH >> "$LOG_FILE" 2>&1
    $APP_PATH >> /dev/ttyFIQ0 2>&1
    APP_PID=$!
    echo "$(date +'%Y-%m-%d %H:%M:%S') - lvgl_app 启动成功，PID: $APP_PID" >> "$LOG_FILE"
}

# 检测 lvgl_app 是否运行的函数
check_lvgl_app() {
    if ! kill -0 $APP_PID 2>/dev/null; then
        echo "$(date +'%Y-%m-%d %H:%M:%S') - lvgl_app 已退出，重新启动..." >> "$LOG_FILE"
        start_lvgl_app
    fi
}

# 检测 lvgl_app 是否有更新版本
check_lvgl_update() {
    if [ -e $UPDATE_PATH ]; then
        echo "$(date +'%Y-%m-%d %H:%M:%S') - lvgl_app 正在更新..." >> "$LOG_FILE"
        mv $UPDATE_PATH $APP_PATH
    fi
}

# 将脚本置于后台运行
if [ "$1" != "daemon" ]; then
    nohup "$0" daemon > /dev/null 2>&1 &
    exit 0
fi

# 主循环
check_lvgl_update
start_lvgl_app
while true; do
    check_lvgl_update
    check_lvgl_app
    sleep 10
done
