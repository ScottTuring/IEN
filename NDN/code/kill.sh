#!/bin/bash

# 指定要杀死的端口范围
start_port=8000
end_port=8100

# 循环遍历指定范围的端口，逐个杀死对应的进程
for ((port = start_port; port <= end_port; port++)); do
    # 查找占用指定端口的进程ID
    pid=$(lsof -t -i:$port)

    # 如果找到了进程ID，则杀死该进程
    if [ -n "$pid" ]; then
        echo "Killing process on port $port with PID $pid"
        kill -9 "$pid"
    else
        echo "No process found on port $port"
    fi
done

