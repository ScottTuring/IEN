/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
               IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/

设计目的**

**IEN(****智能生态网络)是由深圳市内容中心网络与区块链重点实验室（ICNlab）IEN课题小组设计，**该协议旨在为未来互联网提供一种全新的基础网络传输协议。相比TCP/IP协议，**IEN**提供了更丰富的基础语义。 
 IEN协议目前基于TCP/IP协议构建，但也允许未来工作在其它的基础链路协议之上。下面是**IEN**协议要解决的核心问题

 

**提供基于设备ID的点到点传输能力**

\1. 基于设备绑定的网络节点逻辑标识(DeviceId)来构建网络，而不是基于数据路由网络地址(IP地址)。

\2. DeviceId的生成是去中心化的。

\3. 设备到设备的数据传输是加密、可信的，且不依赖基于根证书体系的中心化证书系统。

\4. DeviceId的PoW设计，让IEN网络中的节点拥有了基础的信用等级，也让提供服务的节点拥有了防御DDoS攻击的基础能力，

\5. 可控两跳访问的设计，能尽量抹平系统中的流量特征，让提供服务的节点拥有了一定的物理隐秘能力

\6. 目前基于TCP/IP协议的实现，能在Super Node(SN)、Proxy Node(PN)的帮助下完成NAT穿透和链路优化，实现极高的节点连通性和更比TCP更好的传输性能。

\7. 系统可以有大量不特定的Super Node,Proxy Node的提供者，在激励系统的帮助下，整个网络能保持去中心的组织方式和较高的整体可用性

**实现NDN （Named Data Network)**

\13. 提供Named Data传输服务(Data Channel)，实现了一个可用的NDN。数据的需求方关注数据准确性而不关注数据来源。

\14. Data Channel的设计，为“中间人”加速，数据路由节点的“数据缓存”提供专门的支持。

**常见缩写**

LN : Local Node,通常是发起操作的一方。
 RN : Remote Node,通常是收到请求（类似服务）的一方。
 SN : Super Node 的缩写。
 PN : Proxy Node 的缩写。
 NON : Named Object Network的缩写。
 NDN : Named Data Network的缩写。

**主要概念解释**

**NON(named object network)**

NON是基于IEN实现的P2P网络的首要目标，定义了对象,特别是有权对象.再基于这些对象的语义关系构建有权对象语义网络 NON提供的新的结构化信息存储结构，和基于NON的应用开发理论，是系统最重要的理论基础。
 理解NON是理解系统整体设计的基础，请参看专门的文章，来建立对NON的完整了解。

**Groupid (gid):**

代表一组DeviceId.通常是IEN里广播操作的"目的"参数。目前除了SimpleGroup,系统里的各类Group还未完全稳定，需要在实际的应用开发过程中逐步完善。

**PoW****流程**

持有DeviceId的设备，应该在空闲时定期进行PoW运算。运算方法是：
 不断变换一个64byte 的 Nonce值，让Hash(Object.Desc + Sign(nonce) + nonce)的值小于一个指定的难度值. (给nonce签名的部分可以防止诞生专门的PoW计算服务，这个设计还未完全确定好)。需要的时候，可以展示这个符合难度规则的nonce值，以让Peerid满足服务提供者设定的基本条件。
 这个设计让系统能区分长期存在的善良节点，让IEN从原理上能解决P2P网络中的节点信用问题。

**Area** **区域:**

通用用来作为广播操作的目的地参数，和Group不同的是，Zone基本上没有Owner,而是类似“同城、同省的概念”。Area中包含一组Peer.
 Area可以分为逻辑Zone和物理Zone两大类。
 物理区域信息在Desc的区域编码(AreaCode)中，所以DeviceId的物理区域一旦确定就是不可修改的。
 每个DeviceId都只属于一个物理区域。
 逻辑Area通常和由物理网络提供，比如“同一个局域网”，“同一个IPv4网段”。
 每个DeviceId可以属于多个逻辑区域。

**Datagram** **报文：**

通常代表大小为1400字节的逻辑最小传输单元。 LN往RN投递一个Datagram,通常代表着一次低开销的快速通信。在实际实现中，报文可以通过分片的方法支持更大的大小（8KB?)，同时在底层协议的帮助下，也有一定的可靠性支持。 Datagram可用于一对一通信和一对多通信。

**IEN Package** **数据包(package):**

IEN协议中定义的逻辑数据包，IEN2.0协议设计文档主要就是定义这些package的详细格式。一个DataPackage的大小不能超过MTU（1460字节)。IEN Package面向的是IEN的实现，而Datagram是IEN提供的一个可供上层使用网络语义抽象。

**Broadcast** **广播：**

向一个指定区域或Group广播内容。从内容的角度有两种主要形式：一种是Datagram广播，其网络语义与传统的UDP广播类似。通过这种方式广播的数字内容有最大长度限制。
 另一种是Chunk广播，是IEN支持的全新广播语义。如果对延迟的要求高，那么可以用于音视频多人会议等直播场景。如果对延迟的要求不高，那么可用于传统的群内文件分享场景。低延迟Chunk广播的具体实现方式将在应用开发中进行验证。会通过内置网络编码来提高性能，简化调度算法，降低广播源的带宽成本。
 进行高延迟Chunk广播的逻辑是先用报文广播广播数据头(Data Name)，当接受到数据头广播的节点判断自己需要这个数据的时候自己向广播发起方请求。
 广播的时候如果是基于D-DHT表，实际上是在进行一次点对点遍历发送。相对于网络层提供的原生广播(比如UDP广播）能力，性能难免有所下降，但会有更好的广域网能力。
 进行低延迟chunk广播时，可能需要先组网来保证一定的点对点分片转发可达性。

**Tunnel** **通道：**

连接两个节点的无方向、全双工逻辑信道（可以想象成一条虚拟的链路）。两个节点之间建立和保持Tunnel的逻辑属于IEN协议实现的核心策略。每个节点都可以根据自己的实际情况进行选择。
 两个节点之间允许有多条Tunnel.从IEN层的网络语言实现层(Session层）来看，应该更好的利用好已经存在的多条Tunnel.
 当两个Device之间不存在任何可用的Tunnel时，一个Peer想往另一个Peer发送IEN Package,就会触发Tunnel的Connector逻辑。Tunnel的Connector逻辑是IEN协议设计中最复杂的部分。

Session层实现利用Tunnel来发送**IEN**Pacakge,底层不提供任何额外的缓存和拥塞控制，逻辑语义基本等价于与UDP的sendto.
 另外，Session层对象的状态（比如Connection的状态）变化可以与tunnel的状态隔离。比如一个Tunnel发生了中断、重连的变化（也许重连后LN\RN的EndPoint发生了变化），但这个变化可以对Connection层是透明的。（这个特性在别的协议里常被称作连接迁移）。
 为了方便Session出了层开发，我们抽象了TunnelContainer.每个Peer对之间只有一个TunnelContainer，实际开发中我们多使用TunnelContainer的接口来进行Session层的实现。

**IEN****提供的应用层网络语义抽象**

**Connection** **连接（Stream）:**

与TCP Connection设计一样。提供了LN<->RN的全双工，顺序可靠传输。在Connection上传输大量数据时，会有拥塞控制机制。IEN允许在不同的Connection上根据其场景使用不同的拥塞控制算法。一般的，Connection用来传递逻辑控制命令而非Named Data数据。我们相信未来IEN网络中跑在Connection上的流量占比不会超过50%。

**投递Datagram :**

类似UDP的sendto,recvfrom，但能支持更大的Package。

**Boradcast :**

类似UDP在局域网的功能，但能在广域网工作。

**SuperNode SN:**

在带NAT 的P2P环境中，帮助peer与peer建立直接连接的tunnel/channel. Peer通过服务发现机制找到SN，并确定自己是否需要在该SN上注册。一个Peer可以在多个SN上注册，但会消耗额外的带宽成本。

DHT :

分布式哈希表，是系统实现去中心化的关键基础设施。分布式哈希表为在公网上进行广播提供了基础的支持，并在此基础上提供了一个免费的Global Object Cache. DHT Ping: 加入DHT的Peer之间用来进行定向的DHT信息交换。 DHT Peer Cache: 使用DHT的机制来保存PeerDesc,发现可用的SN,PN,进而实现IEN网络的“善意去中心化” DHT Stroage: 把DHT当成一个去中心化的K-V系统，保存任意值（传统DHT实现，可能会被DHT Object-Cache取代） DHT Object Cache: DHT是去中心化P2P系统的核心机制，但多年来改进甚少。我们期望在引入经济系统和带PoW的Deviceid后，可以进一步提高DHT机制的性能和可靠性（去中心化能力）、并防止恶意节点对DHT网络的污染。相关理论已经完成了最基础的核心设计，但DHT的实现细节，还需要在足够规模的实践测试中反复锤炼。详细的原理描述将有专门的文章介绍。

**Named Data Network (NDN):**

在NON网络中，有定义FileObj,DirObj,DiffObj。这些对象已经是符合NON规范的结构化对象。但在其具体定义中，可以包含或引用非结构化的二进制数据。我们把一个指定长度的数据块称作ChunkData或Chunk.Chunk的objid形式包含其 Hash和Length。FileObj,DirObj,DiffObj,ChunkData四个核心概念一起构成了。Named Data. 当device通过objid请求NamedData时，由于其拥有对NamedData进行校验的能力，所以可以不关注数据的来源。

**DataChanel (Channel):**

Peer与Peer之间建立的用来传输NamedData的通道，与Connection类似。但是为传输NamedData进行了专门的设计。

**DataSession :**

在NamedData上建立的一个具体的NamedData的传输会话。传输会话是逻辑意义上的，并无传输方向性。

**ChunkManager:**

协议里没有体现，属于实现部分。通常在Device上会运行一个DataMananger来与DataSession的接口进行互相设置。可以全局性（跨进程）的管理设备上的所有NamedData.

**ProxyNode****代理节点(Proxy)：**

LN如果无法和RN建立Tunnel或Channel,那么就可以通过P3提供的代理服务来建立Tunnel和Channel.如果P3支持Channel代理，那么当P1,问P2请求的数据P3拥有时，P3可以直接发送数据。一个ProxyNode还可以支持多种标准Proxy协议。

 

 

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