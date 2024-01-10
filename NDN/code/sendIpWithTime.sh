#!/bin/bash

output_file=""
temp_file="temp_file.txt"


# 检查是否提供了命令行参数
if [ "$#" -eq 1 ]; then
  # 使用命令行参数作为 trans_prefix 的值
  suffix=$1
  output_file="ip$suffix.txt"
else
  echo "Usage: $0 <integer>"
  exit 1
fi

# 获取当前时间戳
# timestamp=$(date "+%Y-%m-%d %H:%M:%S")

#output=$(curl -o "$temp_file" -s http://54.222.180.228:8000/getfile?filename=ien1.txt)
#echo "$output"
#echo "111"

#exit 1
# 使用 until 循环来轮询检查命令输出
#until curl http://54.222.180.228:8000/getfile?filename=ien1.txt > "$temp_file" 2>/dev/null; do
# until curl -o "$temp_file" -s http://54.222.180.228:8000/getfile?filename=ien1.txt; do
#until output=$(curl -o "$temp_file" -s "http://54.222.180.228:8000/getfile?filename=ien$suffix.txt") && [[ "${output:0:4}" != "File" ]]; do
#  echo "No output yet, waiting..."
#done
#'
while true; do
  output=$(curl -s "http://54.222.180.228:8000/getfile?filename=ien$suffix.txt")

  if [[ "${output:0:4}" == "File" ]]; then
    echo "尚未有输出，继续等待..."
  else
    #echo "文件回显: $output"

    # 保存回显到本地文件
    echo "$output" > "$temp_file"
    #echo "回显保存至 temp_file.txt"

    break  # 退出循环
  fi

  sleep 1  # 等待1秒后继续下一次循环
done

# 在文件最前面写入命令回显的时间戳
timestamp=$(date "+%s%N")
echo "$timestamp" > "$output_file"
echo "" >> "$temp_file"  # 添加一个空行以分隔时间戳和命令输出

# 将时间戳和命令输出追加到最终的输出文件
cat "$temp_file" >> "$output_file"

# 生成80份带数字后缀的文件
for i in {1..80}; do
  new_file="ip${1}_$i.txt"
  cp "$output_file" "$new_file"
  sed -i "1s/.*/$(date "+%s%N")/" "$new_file"
done

echo "Output found and saved to $output_file"

# 删除临时文件
rm "$temp_file"

