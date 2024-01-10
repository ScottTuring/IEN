#!/bin/bash
    latest_timestamp=0
    earliest_timestamp=0
# 遍历所有ien_x.txt文件
for i in {1..80}; do
  file="ien1_$i.txt"
  
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

# 将最早和最晚时间转换回日期时间格式
#earliest_date=$(date -d "@$earliest_timestamp" "+%Y-%m-%d %H:%M:%S")
#latest_date=$(date -d "@$latest_timestamp" "+%Y-%m-%d %H:%M:%S")

# 将结果保存到time.txt文件
echo "Earlie Time:" $earliest_timestamp
echo "Latest Time:" $latest_timestamp

echo "Time information saved to time.txt"
