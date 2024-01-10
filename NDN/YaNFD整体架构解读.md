# YaNFD整体架构解读

# 一.整体结构

**![image-20231107203907193](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231107203907193.png)**

从该目录下我们可以看到一共有cmd,core,dispath,executor,face,fw,mgmt,ndn

,table,util几个主要目录

## 1.cmd

![image-20231107204257901](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231107204257901.png)

这里是启动目录,yanfdui相较于yanfd多了一些ui,也就是一些可视化的功能

但yanfd中会多一些配置

![image-20231107204527607](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231107204527607.png)

![image-20231107204648151](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231107204648151.png)



## 2.core

![image-20231107204806145](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231107204806145.png)

名字虽然是core,但似乎并未包含太多主要的接口定义,

config是配置文件的读取,加载;errors是错误处理相关;  logger是启动/运行日志的处理;

runtime记录启动的时间戳,threads是处理线程退出信号的定义

## 3.executor

![image-20231107205715084](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231107205715084.png)

这里只有一个文件,包含YaNFD的初始化,启动,与停止操作

## 4.dispatch

![image-20231107205523033](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231107205523033.png)

这里主要是关于face和fw这两个模块的接口定义,以及一些简单的init,get等方法

## 5.face

face是网络接口的定义,类似于ip中的port

这里有一系列操作,包括设置和获取 face ID、获取 URI、获取范围和链接类型、获取 MTU 等,还有一个比较值得关注的SendPacket方法,会将数据包添加到发送队列中![image-20231107210437047](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231107210437047.png)

## 6.fw

fw应该是forward的缩写,是NFD内消息转发的功能模块



![image-20231107210317619](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231107210317619.png)

主要是将数据包和interst包添加到队列中以供后续处理

## 7.table/utils

这里主要是一些关于ndn中的三个重要要素即cd,fib,pit相关的定义与用到的一些算法的操作,如lru

![image-20231107211157236](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231107211157236.png)

utils中也主要是一些数据结构的操作,如堆和优先队列

## 8.ndn/mgmt

![image-20231107211802283](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231107211802283.png)

这一部分应该是逻辑处理的模块,包括数据包到来的处理,消息格式的处理,签名信息等等

# 二.启动流程:

​	YaNFD的程序入口在,其中YaNFDUI相较会多一个http server的功能

![image-20231120191815641](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120191815641.png)

进入后可以看到真正的Start函数

![image-20231120191942708](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120191942708.png)

![image-20231120192116236](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120192116236.png)

​	这里可以看到有三个主要goroutine的启动,分别是:

```
go nullFace.Run(nil)
```

```
go management.Run()
```

```
go fw.Threads[i].Run()
```

这里nullFace.Run是没有任何处理操作,似乎是用于测试所需

我们再来看看management

# 三.management

![image-20231120193334329](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120193334329.png)

​	首先这里有一个face.RegisterInternalTransport(),这表明这个thread是用于内部模块的通信使用

​	然后有一个对于是否开启本地hop的选择,如果开启就会将本机加入到路由表	

![image-20231120193645482](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120193645482.png)

![image-20231120193739338](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120193739338.png)

​	这里开启一个for循环,用于读取线程收到的数据,进入receive,我们可以看到它具体的实现![image-20231120193908555](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120193908555.png)

​	这里也是一个for死循环,一个case是从t.recvQueue中读取数据(当然其数据类型就是一个用于进程间通信的channel),然后将读取到的数据返回,一共是三个,一个是数据包,一个是pittoken(这个是到来的interest包的标识),还有一个inface,标识从那个连接收到的

​	后面又是一些判断,如果都通过的话,会进入到最后的处理

![image-20231120194849831](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120194849831.png)

​	从注释可以看到,这里会基于interest的name来处理,这里if是判断name对应的moudle是否存在,如果不存在则返回一个unknown的res包,如果通过则会调用handleIncomingInterest来处理到来的interest,这里根据不同的moudle会触发不同的处理,这里也可以看到一共有六个moudle

![image-20231120200707488](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120200707488.png)

​	关于各种module的处理这里就不再细看了

# 四.Foward thread

​	这里有一个thread的数量限制,run方法是每一个线程的具体处理逻辑![image-20231120201911583](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120201911583.png)

​	这里主要是按照到来的消息的不同类型来调用对应的处理函数,我们先关注Interest的处理

![image-20231120202205203](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120202205203.png)

​	这里可以看到发送t.pendingInterests的地方同样是thread下的QueueInterest

![image-20231120202822824](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120202822824.png)

而这个函数只会被link-servcie下的dispatchIncomingPacket函数调用

![image-20231120203007765](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120203007765.png)

![image-20231120203100889](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120203100889.png)

​	我们再逐层往上查找调用,可以看到是handleIncomingFrame在调用

![image-20231120203959563](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120203959563.png)

​	再往上继续查找,可以看到有几个transport的实现以及ndnlp-linkservcie有调用这个函数,但是这个函数方法的实现是在linkerservice类下

![image-20231120204044367](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120204044367.png)

​	关于transport就是传输层 udp/tcp的数据接收,这里可以简单看一下其中一个udp的调用

![image-20231120204918219](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120204918219.png)

​	readSize, err := t.conn.Read(recvBuf),这里是从建立好的网络连接中读取数据,然后处理数据帧,从这里倒着往回看就可以理清数据在代码中的流动

​	现在我们再来看看收到的Interest的处理

![image-20231120205610843](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120205610843.png)

​	这里首先检查到来的faceId是否为空,然后有一个HopLimitV的减少,这是interest最大转发次数的限制,然后获取PIT Token（如果存在）。

​	这里有一个是否Check if violates /localhost的注释,如果满足就会丢弃interest

![image-20231120210445293](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120210445293.png)

​	后面又有一系列的判断和逻辑处理,这里有一个判断是否在cs中已经有对应项的缓存,如果有的话就会直接返回了

![image-20231121140525586](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121140525586.png)

​	最后如果下一跳face不为空,就会选取一个fw thread来发送数据,即调用SendPacket函数

![image-20231120211525015](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231120211525015.png)

​	这里就又回到Face类的操作

# 五.Face

​	这里是一些Face的基础接口定义

![image-20231121142033891](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121142033891.png)

​	而关于face的实现类就是Linkservice,这个实现类扩充了很多接口,以及一些常量的定义

![image-20231121142150985](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121142150985.png)

​	我们首先看看SendPacket的定义,这里其实比较简单,就是将数据推送到一个发送队列

![image-20231121142018811](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121142018811.png)

​	![image-20231121142550291](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121142550291.png)

​	可以定位到对应的调用,是在ndnlp-linkservice的runSend方法

![image-20231121142621350](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121142621350.png)

​	这里可以看到调用了一个内部方法sendPacket,这里就会涉及到一些端口的状态检查,一些常量的维护

​	还有关于头部信息的开销去除

![image-20231121143216586](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121143216586.png)

​	最后根据字节流的大小来决定是否要分片,然后还有一系列的判断的逻辑处理

![image-20231121144544903](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121144544903.png)

![image-20231121144550588](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121144550588.png)

![image-20231121144555921](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121144555921.png)

​	这里是最后的数据帧发送,根据不同的连接方式,例如tcp/udp等,sendFrame的具体实现也不同

![image-20231121144641564](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121144641564.png)

![image-20231121144940807](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121144940807.png)

​	比如这里我们看一下在一个udp的传输中,sendFrame的实现

![image-20231121145040257](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20231121145040257.png)

​	其中最关键的是_, err := t.conn.Write(frame),将数据写入到对应连接的conn里,这将会调用一些系统函数,主要是一些关于net包的使用