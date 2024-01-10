#!/bin/bash

output_file=""
trans_prefix=""
temp_file="temp_output.txt"


# 检查是否提供了命令行参数
if [ "$#" -eq 1 ]; then
  # 使用命令行参数作为 trans_prefix 的值
  trans_prefix="/ien$1"
  output_file="ien$1.txt"
else
  echo "Usage: $0 <integer>"
  exit 1
fi

# 获取当前时间戳
timestamp=$(date  "+%s%N")

# 使用 until 循环来轮询检查命令输出
until ndnpeek -p "$trans_prefix" > "$temp_file" 2>/dev/null; do
  echo "No output yet, waiting..."
done

# 在文件最前面写入命令回显的时间戳
echo "$timestamp" > "$output_file"
echo "" >> "$temp_file"  # 添加一个空行以分隔时间戳和命令输出

# 将时间戳和命令输出追加到最终的输出文件
cat "$temp_file" >> "$output_file"

# 生成80份带数字后缀的文件
for i in {1..80}; do
	new_file="ien${1}_$i.txt" 
	cp "$output_file" "$new_file" 
	sed -i "1s/.*/$(date "+%s%N")/" "$new_file"  
done

echo "Output found and saved to $output_file"

# 删除临时文件
rm "$temp_file"

