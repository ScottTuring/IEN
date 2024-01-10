#!/bin/bash

# 设置父目录路径
parent_directory="/root/hyperchain/flato"

# 定义服务器IP地址数组
server_addresses=("71.132.23.94" "69.234.248.132" "69.234.243.56" "161.189.215.135" "69.234.192.186" "52.83.187.131" "52.83.157.229" "52.83.246.81" "120.77.204.211" "120.79.47.170" "47.106.151.8")

#这里对自己运行的主机进行一个日志的搜索
# 循环处理每个节点
for ((node=1; node<=4; node++)); do
    echo "开始查找54.222.180.228上节点" $node

    # 设置节点日志文件路径
    node_log="${parent_directory}/node${node}/node${node}.log"
    #node_log="${parent_directory}/node${node}/namespaces/global/data/logs/hyperchain_*.log"
    # 获取节点广播 PrePrepare 日志的时间戳（秒.纳秒）
    broadcast_timestamp=$(tac "$node_log" | awk '/sending prePrepare/ {print $1, $2,$7,$8,$9,$10,$11,$12,$13,$14; exit}')
    if [ -n "$broadcast_timestamp" ]; then
    	echo "发送 PrePrepare 时间戳: $receive_timestamp"
    fi
    # 获取节点接收 PrePrepare 日志的时间戳（秒.纳秒）
    receive_timestamp=$(tac "$node_log" | awk '/received prePrepare/ {print  $1, $2,$7,$8,$9,$10,$11,$12,$13,$14; exit}')
    if [ -n "$receive_timestamp" ]; then
    	echo "接收 PrePrepare 时间戳: $receive_timestamp"
    fi
    # 获取节点广播 PrePrepare 日志的时间戳（秒.纳秒）
    #broadcast_timestamp=$(grep -a 'sending prePrepare' $node_log | tail -n 1 | awk '{print $1, $2}')

    # 获取节点接收 PrePrepare 日志的时间戳（秒.纳秒）
    #receive_timestamp=$(grep -a 'received prePrepare' $node_log | tail -n 1 | awk '{print $1, $2}')

    # 将时间戳和日志写入文本文件
    echo "$broadcast_timestamp" >> broadcast_logs.txt
    echo "$receive_timestamp" >> receive_logs.txt
done

node=4
# 循环处理每个服务器
for server_address in "${server_addresses[@]}"; do
    for ((i=1; i<=4; i++)); do
        # 设置节点日志文件路径
	((node++))
       
        node_log="${parent_directory}/node${node}/node${node}.log"

        #node_log="${parent_directory}/node${node}/namespaces/global/data/logs/hyperchain_*.log"

	echo "开始查找" $server_address "上节点" $node
	# 获取节点广播 PrePrepare 日志的时间戳（秒.纳秒）
	broadcast_timestamp=$(ssh $server_address "tac $node_log | awk '/sending prePrepare/ {print \$1, \$2,\$7,\$8,\$9,\$10,\$11,\$12,\$13,\$14; exit}'")
	if [ -n "$broadcast_timestamp" ]; then
            echo "发送 PrePrepare 时间戳: $receive_timestamp"
        fi
	# 获取节点接收 PrePrepare 日志的时间戳（秒.纳秒）
	receive_timestamp=$(ssh $server_address "tac $node_log | awk '/received prePrepare/ {print \$1, \$2,\$7,\$8,\$9,\$10,\$11,\$12,\$13,\$14; exit}'")
   	if [ -n "$receive_timestamp" ]; then
    	    echo "接收 PrePrepare 时间戳: $receive_timestamp"
        fi


        # 获取节点广播 PrePrepare 日志的时间戳（秒.纳秒）
        #broadcast_timestamp=$(ssh $server_address "grep -a 'sending prePrepare' $node_log | tail -n 1 | awk '{print \$1, \$2}'")

        # 获取节点接收 PrePrepare 日志的时间戳（秒.纳秒）
        #receive_timestamp=$(ssh $server_address "grep -a 'received prePrepare' $node_log | tail -n 1 | awk '{print \$1, \$2}'")
	
        # 将时间戳和日志写入文本文件
        echo "$broadcast_timestamp" >> broadcast_logs.txt
        echo "$receive_timestamp" >> receive_logs.txt
    done
done
server_address=54.222.128.216
for ((node=49; node<=50; node++)); do
    echo "开始查找54.222.128.216上节点" $node

    # 设置节点日志文件路径
    node_log="${parent_directory}/node${node}/node${node}.log"
    #node_log="${parent_directory}/node${node}/namespaces/global/data/logs/hyperchain_*.log"
    # 获取节点广播 PrePrepare 日志的时间戳（秒.纳秒）
    broadcast_timestamp=$(ssh $server_address "tac $node_log | awk '/sending prePrepare/ {print \$1, \$2,\$7,\$8,\$9,\$10,\$11,\$12,\$13,\$14; exit}'")
    if [ -n "$broadcast_timestamp" ]; then
        echo "发送 PrePrepare 时间戳: $receive_timestamp"
    fi
    # 获取节点接收 PrePrepare 日志的时间戳（秒.纳秒）
    receive_timestamp=$(ssh $server_address "tac $node_log | awk '/received prePrepare/ {print \$1, \$2,\$7,\$8,\$9,\$10,\$11,\$12,\$13,\$14; exit}'")
    if [ -n "$receive_timestamp" ]; then
    	echo "接收 PrePrepare 时间戳: $receive_timestamp"
    fi

    # 获取节点广播 PrePrepare 日志的时间戳（秒.纳秒）
    #broadcast_timestamp=$(ssh $server_address "grep -a 'sending prePrepare' $node_log | tail -n 1 | awk '{print \$1, \$2}'")

    # 获取节点接收 PrePrepare 日志的时间戳（秒.纳秒）
    #receive_timestamp=$(ssh $server_address "grep -a 'received prePrepare' $node_log | tail -n 1 | awk '{print \$1, \$2}'")

    # 将时间戳和日志写入文本文件
    echo "$broadcast_timestamp" >> broadcast_logs.txt
    echo "$receive_timestamp" >> receive_logs.txt
done
##最后两个节点

# 对记录的时间戳进行排序
sort -k2,2n broadcast_logs.txt -o broadcast_logs_sorted.txt
sort -k2,2n receive_logs.txt -o receive_logs_sorted.txt


# 删除临时文件
rm broadcast_logs.txt receive_logs.txt
