/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
               IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/

Package Decoder**

并不是一个传统的，可以逐个byte放入的parser.而是必须一口气塞入整个Package的decoder 这样可以降低一些实现复杂度。分包的工作由上层Parser完成。

**TCP FirstPackage Parser**

先读取

**TCP Parser**

\1.   如果是加密TCP Tunnel,那么每16个字节先解密，再丢给TCP Parser

\2.   读取2字节长度，然后读取包内容

\3.   将包内容塞给Package Decoder得到一个有效的Package

上述Decoder,Parser都是版本绑定的。在发布后实现时，要不断的创建类似 PackageDecoder_10，PackageDecoder_11这样的对象。已实现正确的向下兼容。

**AesUnBox****（udp）**

\1.   读取前64bit, 判断是否是合法的AesMixHash,得到AesKey

\2.   把剩下的数据用AesKey解密，UnBox成功

**加密传输方式**

我们有2个等级（带身份验证的明文取消了）

\1.   明文（不加密）

\2.   全加密从目前HTTPS大规模普及的角度考虑，系统默认使用最高的加密等级。

**协议特征消除**

从IP协议看到的IEN协议特征

\1.   ip1往ip2的udp port发送了大量的unknown数据如何定义大量：比平时多，ip2处于ip1不常通信的ip段

\2.   ip1往ip2发送的大量unknown数据包，有一个特点的特征（比如共同的数据头）

**Box** **封包:**

在通道上发送package时，需要根据通道的属性对package进行封包。封包逻辑本身是协议设计的一部分。Box的过程原则上不会影响Package原本包含的内容。

**Box Packages** **包合并:**

一个成功到达的P2P DataPackage是宝贵的。我们可以在这个宝贵的包中，通过Box Packages的方法多带一些包。这些包通过引用First Package的方法，可以省略自己与FirstPackage相同的字段，进而降低带宽占用和延迟。下面是一个典型的例子：

LN打算给RN投递一个DataGram（通过加密协议），不合并包的逻辑如下:

LN->RN:EXCHANGE_KEY 

RN->LN:EXCHANGE_KEY_OK 

LN->RN:SYN_TUNNEL 

RN->LN:ACK_TUNNEL 

LN->RN:ACKACK_TUNNEL 

LN->RN:DATAGRAM

RN:通知UserAPI 开始处理Datagram

RN->LN:DATAGRAM_RESP

经过包合并逻辑后，实际通信如下.

LN->RN:EXCHANGE_KEY with SYN_TUNNEL with Datagram  

RN->LN:ACK_TUNNEL,通知UserAPI 开始处理Datagram  

LN->RN:ACKACK_TUNNEL  

RN->LN:DATAGRAM_RESP  

合并过程：
 EXCHANGE_KEY with SYN_TUNNEL with Datagram ，当Datagram不大时合并生效。合并后的UDP包如下：

<FirstPackage header>

<FirstPackage Body>

<totallen,cmdType>

<SecondPackage Body> // 这里会去掉FirstPackage Body中已经有的字段 为了达到这个目的，所有协议里body定义的字段名同名时意义都是相同的

<totallen,cmdType>

<ThirdPackage Body> // 这里会去掉FirstPackage, Body中已经有的字段

 

 

**专利：**

\1.  美国专利（授权），2019年："NDN和IP融合网络内容控制方法和装置，以及存储介质（一种NDN和IP融合网络的内容管控方法、装置及存储介质）";

\2.  美国专利（授权），2019年："基于信息网络中概率树转发利益包的方法和装置（信息中心网络下基于概率树的兴趣包转发的方法）;

\3.  美国专利（授权），2019年："向网络发送目标数据并从网络获取目标数据的方法和装置";

\4.  中国专利（授权） 授权专利：基于网络传输的数据包的加密解密方法与终端，ZL201310688322.5，授权日： 2018年1月;

\5.  中文专利（授权） 授权专利：信息中心网络下兴趣包转发的方法和转发终端，ZL201410476456.5，授权：2017年7月。

\6.  中国专利（授权）授权专利：信息中心网络下的流媒体系统及其使用方法，ZL201410345816.8，授权：2017年6月。

\7.  中国专利（授权）授权专利：一种基于NDN网络架构的P2P文件传输方法，ZL201310339690.9，授权日：2016年5月。

\8.  中国专利（授权）授权专利：支持消费者移动的数据预取方法、接入基站和终端，ZL201410204197.0，授权日：2017年6月。

\9.  中国专利（授权）授权专利：TCP/IP协议与NDN协议之间的数据转换传输方法，ZL201310137990.9，授权日：2017年2月。

\10. 中国专利（正在处理中）："一种自组网络的分布式入侵检测方法和系统"，201810179320.6，申请日期，2018年3月8日【NDN区块链】

\11. 中国专利（处理中）： "一种CCN中基于区块链的密钥管理方法、装置及存储介质"，201810878557.3，申请日期，2018年8月3日【NDN Blockchain】

\12. 中国专利（处理中）："基于NDN的区块链同步方法和装置"，201710400368.0，申请日期，2017年5月31日【NDN区块链】

\13. 中国专利（处理中）："一种联盟区块链共识方法"，201810122889.9，申请日期，2018年2月【区块链】

\14. 中国专利（正在处理中）："一种区块链系统的测试系统和方法"，201711320172.7，申请日期，2017年12月12日【区块链】

\15. 中国专利（处理中）： "一种基于区块链的数字资产交易方法和系统"，201810247448.1，申请日期，2018年3月23日【区块链】

\16. 中国专利（处理中）： "一种活动社交网络中节点可信度的计算方法"，201710132298.5，申请日期，2017年3月8日【区块链】

 

 

**论文：**

1、A Peer-to-Peer File Sharing System over Named Data Networking. ICPADS 2016

2、BlockNDN_A_bitcoin_blockchain_decentralized_system_over_named_data_networking

3、Centaur_A_evolutionary_design_of_hybrid_NDN_IP_transport_architecture_for_streaming_application

4、NDN-Hippo_A_streaming_media_system_over_named_data_networking 

5、Intelligent Eco Networking IEN)_ Knowledge-Driven and Value-Oriented Future Internet Infrastructure

6、Intelligent_Eco_Networking_IEN_an_Advanced_Future_Internet_of_intelligence_for_Digital_Social_Economic_Ecosystem

7、Intelligent_Eco_Networking_IEN_II_A_Knowledge-Driven_Future_Internet_Infrastructure_for_Value-Oriented_Ecosystem