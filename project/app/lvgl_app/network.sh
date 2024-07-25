#!/bin/bash

# 检查eth0的连接状态
check_eth0_link() {
	sleep 0.1
    # 如果eth0没有物理连接
    if ip link show eth0 | grep -q "NO-CARRIER"; then
	    return 1  # eth0未连接
    fi
    return 0  # eth0已连接
}

# 检查eth0的IP获取情况
check_eth0_ip() {
    # 如果eth0没有获取到IP
    if  ifconfig eth0 | grep -q "inet "; then
        return 1  # eth0已获取IP
    fi
    return 0  # eth0未获取IP
}

# 设置路由，优先使用eth0，如果eth0不可用则使用eth1
set_route() {
    # 检查eth0是否可用
    if check_eth0_link; then
        # 尝试从路由表中获取eth0的网关
        gateway=$(ip route | grep default | grep eth0 | awk '{print $3}')
        
        if [ -z "$gateway" ]; then
            echo "eth0 does not have a gateway, obtaining IP via DHCP"
            # 如果没有网关，使用DHCP获取IP和网关
            udhcpc -i eth0
           
	        sleep 2
 
            # 再次检查eth0是否有IP
            if check_eth0_ip; then
                # 设置 metric 值
                ifmetric eth0 200
                ifmetric eth1 100
                return
            fi

            # 成功获取IP后更新网关
            gateway=$(ip route | grep default | grep eth0 | awk '{print $3}')
        fi

        if [ -n "$gateway" ]; then
            # 设置默认路由到eth0
            ifmetric eth0 100
            ifmetric eth1 200
            echo "Default route set to eth0 with gateway $gateway"
        else
            echo "No gateway found for eth0, using eth1"
            ifmetric eth0 200
            ifmetric eth1 100
        fi
    else
        # eth0不可用，切换到eth1
        ifconfig eth0 down 
        # 设置 metric 值
        ifmetric eth0 200
        ifmetric eth1 100
    fi
}

# 初始化变量以跟踪eth0的连接状态
eth0_connected=1

# 不断监控eth0的状态
while true; do
    # 检查eth0的当前状态
    check_eth0_link
    current_eth0_connected=$?
    # 如果连接状态发生变化
    if [ $eth0_connected -ne $current_eth0_connected ]; then
       	echo "eth0 link status change"
	    set_route
        eth0_connected=$current_eth0_connected
    fi

    # 每隔10秒钟检查一次
    sleep 3
done