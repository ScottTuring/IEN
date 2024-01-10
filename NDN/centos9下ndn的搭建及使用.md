# centos9下 NDN的搭建/测试:

## ndn-cxx下载

ndn-tools的代码: 

​	git clone https://github.com/named-data/ndn-tools.git

ndn-cxx的代码下载:

​	git clone https://github.com/named-data/ndn-tools.git



![image-20231121212629797](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121212629797.png)



```
sudo dnf install gcc-c++ pkgconf-pkg-config python3 boost-devel openssl-devel sqlite-devel
```

然后直接进行build即可

![image-20231225230950338](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231225230950338.png)

在运行中![image-20231121211624970](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121211624970.png)

目前我有一个下载错误,是在运行 sudo apt install build-essential pkg-config python3-minimal libboost-all-dev libssl-dev libsqlite3-dev时间出现的

全系统搭建参考官方

https://github.com/named-data/ndn-cxx/blob/master/docs/INSTALL.rst

## ndn-tools搭建

然后是ndn-tools的构建

![image-20231122194342202](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231122194342202.png)

​	比较简单

## NFD安装

```
# Download NFD
git clone --recursive https://github.com/named-data/NFD.git
```

![image-20231225231258841](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231225231258841.png)	

​	sudo dnf install libpcap-devel systemd-devel

​	然后也是进行build

		./waf configure
		./waf
		sudo ./waf install
## 如果已将 ndn-cxx 和/或任何其他依赖项安装到非标准路径中

```
export PKG_CONFIG_PATH="/custom/lib/pkgconfig:$PKG_CONFIG_PATH"
```

注意这只会在当前会话中生效

​	如果要全局生效可以使用软连接的方式

在构建ndn服务的时候,jsoncpp的环境配置

export LD_LIBRARY_PATH=/root/ndn/jsoncpp/build/lib/:$LD_LIBRARY_PATH

## YaNFD

​	官方文档:https://github.com/named-data/YaNFD

![image-20231122194648880](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231122194648880.png)

![image-20231225231709847](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231225231709847.png)

go的安装:

​	wget https://golang.org/dl/go1.19.linux-amd64.tar.gz

最后启动:

![image-20231122194722720](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231122194722720.png)



## 关于centos9下使用jsoncpp

**jsoncpp**
—jsoncpp（项目目录）
—build

jsoncpp/jsoncpp 是通过git clone git://github.com/open-source-parsers/jsoncpp/拉取源码

jsoncpp/build 是在这个目录下进行编译

cd进入/build

运行 cmake ../jsoncpp, 这会在build/目录生成编译文件

然后make, make install编译

这里的安装不会安装到标准库

export LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH



可以在编译时通过指定库的路径和名称来链接 `jsoncpp` 库。以下是一些示例编译命令：

g++ main.cpp -o main -std=c++11 -lpthread -L/usr/local/lib64 -ljsoncpp

或者直接g++ main.cpp -o main -std=c++11 -lpthread -ljsoncpp

## nfdc运行中遇到的问题

sudo cp /root/ndn/ndn-cxx/build/libndn-cxx.so.0.8.1 /usr/lib
sudo ldconfig

nfdc: error while loading shared libraries: libndn-cxx.so.0.8.1: cannot open shared object file: No such file or directory解决这个标准库缺少的问题



## ndn通信所需的命令

​	创建face:

​		nfdc face create udp://<xxx>

​	客户端通向server的route:

​		nfdc route add /<prefix> nexthop <faceid>

​	server推送数据:

​		echo'HELLO WORLD'ndnpoke /localhost/demo/hello

​	client peek数据:

​		ndnpeek -p /localhost/demo/hello

## ndn的一些特性/缺点

​	1.同一个前缀的数据在网络中目前不会被更新,发请求会直接拿到原来缓存的数据,而不是server推送的新消息

​	2.ndnpoke会阻塞等待直到有peek来获取数据

​	3.目前在使用过程会遇到即使有poke也有阻塞3-4s的情况,原因尚不明确

## 测试过程中用过的一些命令

​	curl http://54.222.180.228:8000/getfile?filename=ien1.txt

带时间的curl命令:

​	curl -w "Total time: %{time_total} seconds\n" http://54.222.180.228:8000/getfile?filename=ien1.txt

解决c++库下载后不在标准搜索路径下的问题

​        export LD_LIBRARY_PATH=/root/ndn/jsoncpp/build/lib/:$LD_LIBRARY_PATH
