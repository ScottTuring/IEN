#!/bin/bash
    latest_timestamp=0
    earliest_timestamp=0

if [ "$#" -eq 3 ]; then
  # 使用命令行参数作为 suffix 的值
  suffix=$2
else
  echo "Usage: $0 <suffix>"
  exit 1
fi


# 遍历所有ien_x.txt文件
for i in {1..80}; do
  file="ip$suffix_$i.txt"

  # 提取每个文件的第一行时间
  first_line=$(head -n 1 "$file" | tr -d '\n')
  # 将时间转换为时间戳
  #timestamp=$(date -d $first_line "+%s%N")

  # 如果是第一个文件，初始化最早和最晚时间
  if [ "$i" -eq 1 ]; then
    earliest_timestamp=$first_line
    latest_timestamp=$first_line
  else
    # 更新最早和最晚时间
    if [ "$first_line" \< "$earliest_timestamp" ]; then
      earliest_timestamp=$first_line
    fi
    if [ "$first_line" \> "$latest_timestamp" ]; then
      latest_timestamp=$first_line
    fi
  fi
done
# 将结果保存到time.txt文件
echo "Earlie Time:" $earliest_timestamp
echo "Latest Time:" $latest_timestamp

echo "Time information saved to time.txt"

