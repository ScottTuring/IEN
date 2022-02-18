/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
               IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/

### **IEN****协议设计的基本概念**

**机制**

下面先介绍一些协议运行中会用到的基础机制。

**基本数据结构**

u16,u32,u64等常见基础类型不做描述，统一使用主机字节序。 u48也是协议会使用的基础类型。在很多地方，我们用uint48而不是uint64来取代uint32，已解决一些大小不够的问题。同时uint48也能在一些常见的脚本语言里获得正确的支持而不需要专门的扩展。

Array

Array8 表示头部有一个u8长度的，成员类型为u8的数组（最大长度为255）。
 Array16 表示头部有一个u16长度的，成员类型为u16的数组，(数组最大长度为65535)。

EndPoint

代表一个可以通信的端口。其形式为协议簇@主机地址@端口。比如

TCPv4@192.168.100.1@8000

UDPv6@fe80::5d57:63dd:d4c3:37fc@8000

Timestamp

32bits。时间戳，但与UNIX时间戳不同，要求精确到ms。可以简单的通过把标准的UNIX 时间戳左移10bits空出空间。

ObjectBody

ObjectBody的编码是NON网络定义中的一部分，由于要对Named Object进行签名，所以在指定编码方式后，必须有一个确定的编码结果，而不是像json/xml那么自由。

**IEN Package****的编码结构**

**IEN** Package在逻辑上支持两种编码方式：加密/明文。在实现上有UDP Base(datagram base)和TCP Base(Stream base)两种。
 组合一下，一共有四种编码流程。理解分类方式后能更好的理解其背后的设计逻辑。

1在UDP非加密环境下，结构为

{//Raw Box 

  maigcNum : u16,//固定值0x8000

  Package[0]{

​    totallen : u16,//包长度

​    cmdtype : u8, //包类型

​    cmdflags : u16,//用来指示字段是否存在

​    body //根据cmdtype不同而类型不同       

  },

Package[1]{

  totallen : u16,

  cmdtype : u8, 

​    cmdflags : u16,//用来指示字段是否存在

​    body //根据cmdtype不同而类型不同       

  },

  ... //可以有多个Package，基于包合并机制编码

}

在UDP加密环境下，结构为

{//普通加密Box 

MixHash : u64,

  {//下面的数据基于MixHash关联的Key进行对称加密

   Package[0]{

  totallen : u16,

  cmdtype : u8, 

  cmdflags : u16,

  body //根据cmdtype不同而类型不同       

​    },

Package[1]{

  totallen : u16,

  cmdtype : u8, 

  cmdflags : u16,

  body //根据cmdtype不同而类型不同       

​    },

​    ... //可以有多个Package，基于包合并机制编码

  }

}

 

{//带交换密钥的加密Box 

  crypto_key : u256, //使用RN的Public Key加密后的crypto_key

  key_hash  : u64, //Key hash(mix hash),RN可以在解密crypto_key后进行校验

  {//下面数据全部用crypto_key加密

​    ExchangeKeyPackage {//这是一个特殊的Package,没有cmdtype,也不能向前引用

​      totalLen : u16,//总长度

​      cmdflags : u16,//用来指示字段是否存在

​      seq : u32,//用来过滤旧包。

​      seqAndKeySign:u128,//string(seq)+(aesKey)的LN签名，证明这个包确实是由fromPeerID发出的

​      fromPeerid:u256,//和老的PeerID最好是兼容的。这里不用填写toPeerID,因为public key的存在这是不言自明的。

​      peerInfo:Option<PeerDesc>,//如果LN确信RN知道自己的PublicKey,那么就可以不填写。

​      sendTime:u64,//精确到ms

​    },

Package[1]{

totallen : u16,

cmdtype : u8, 

cmdflags : u16,

​      body //根据cmdtype不同而类型不同       

​    },

​    ... //可以有多个Package，基于包合并机制编码

  }

}

TCP环境下的Package结构

TCP模式下的情况相对比较复杂，首先IEN Stack中TCP连接成功后，并不能立刻确定该TCP连接的用途，需要通过TCP FirstPackage来明确其用途。这时存在所谓的TCP FirstPackage编码。如果TCP的连接用途是TCP Tunnel(发送IEN Package的管道)，那么会有和UDP接近的编码方式

TCP FirstPackage

//明文First Tcp Box

{

  magicNum : u16,///0x8000 | 协议版本号。版本号区间在[01-255] 该字段只有在FistPackage中才会填写

  {

​    boxsize : u16,//First Box的大小。

​    Package[0]{

   totallen : u16,

   cmdtype : u8, 

   cmdflags : u16,

   body //根据cmdtype不同而类型不同  

   },

Package[1]{

​    totallen : u16,

​    cmdtype : u8, 

​    cmdflags : u16,

​    body //根据cmdtype不同而类型不同  

​    }, 

  },

 

  //根据TCP连接的用途为box 或 stream data.

  ...

}

 

//普通密文First Tcp Box

{

  MixHash : u64,//可以通过MixHash得到后续的crypto key。

  {

​    boxsize : u16,//使用crypto key进行弱加密的box长度。

​    {//box payload，使用crypto key进行加密。

​        Package[0]{

​        totallen : u16,

​        cmdtype : u8, 

​        cmdflags : u16,

​        body //根据cmdtype不同而类型不同  

​      },

Package[1]{

​     totallen : u16,

​     cmdtype : u8, 

​     cmdflags : u16,

​     body //根据cmdtype不同而类型不同  

​      }, 

​      ...

​    }

  }

 

  //box 或 stream data.

  ...

}

 

//带交换密钥的密文First Tcp Box

{

  crypto_key : u256, //使用RN的Public Key加密后的crypto_key

  key_hash  : u64, //Key hash(mix hash),RN可以在解密crypto_key后进行校验

  {

​    boxsize : u16,//使用crypto key进行弱加密的box长度。

​    {//下面数据全部用crypto_key加密

​      ExchangeKeyPackage {//这是一个特殊的Package,没有cmdtype,也不能向前引用

​        totalLen : u16,//总长度

​        cmdflags : u16,//用来指示字段是否存在

​        seq : u32,//用来过滤旧包。

​        seqAndKeySign:u128,//string(seq)+(aesKey)的LN签名，证明这个包确实是由fromPeerID发出的

​        fromPeerid:u256,//和老的PeerID最好是兼容的。这里不用填写toPeerID,因为public key的存在这是不言自明的。

​        peerInfo:Option<PeerDesc>,//如果LN确信RN知道自己的PublicKey,那么就可以不填写。

​        sendTime:u64,//精确到ms

​      },

Package[1]{

​    totallen : u16,

​    cmdtype : u8, 

​    cmdflags : u16,

​    body //根据cmdtype不同而类型不同  

​    }, 

​      ...

​    }

  }

 

  //box 或 stream data.

  ...

}

TCP Body(Box或stream data)

通过FirstPackage确定类型后，TCP连接上的后续数据的编码方式分下面几种

不加密stream

{//不加密stream,效率最高的一种。和TCP等价

  streamdata

}

加密stream

实现协议时，需要仔细考虑如何划分stream_box.

{//加密stream

  crypto_stream_box[0]{

​    boxsize:u16,//用当前crypto_key简单加密

​    {

​      payload //用当前crypto_key加密

​    }

  }，

   crypto_stream_box[1]{

​    boxsize:u16,//用当前crypto_key简单加密

​    {

​      payload //用当前crypto_key加密

​    }

  }

  ...

 

}

不加密TCP Tunnel

这里Box的作用是用来控制向前Package合并。不过在Box只有一个Package的情况下，会浪费2bytes的boxsize.

{

box[0]{

​    boxsize : u16,//Box的大小。

​    Package[0]{

​     totallen : u16,

​    cmdtype : u8, 

​    cmdflags : u16,

​      body //根据cmdtype不同而类型不同  

​    },

Package[1]{

   totallen : u16,

   cmdtype : u8, 

​    cmdflags : u16,

​      body //根据cmdtype不同而类型不同  

​    }, 

  },

box[1]{

​    boxsize : u16,//Box的大小。

Package[0]{

​    totallen : u16,

​    cmdtype : u8, 

​    cmdflags : u16,

​    body //根据cmdtype不同而类型不同  

​    },

Package[1]{

​    totallen : u16,

​    cmdtype : u8, 

​    cmdflags : u16,

​    body //根据cmdtype不同而类型不同  

​    }, 

  }

},

**加密Tunnel**

这里Box的作用除了是加解密边界，还是用来控制向前Package合并。不过在Box只有一个Package的情况下，会浪费2bytes的boxsize.

{

box[0]{

​    boxsize : u16,//用当前crypto_key简单加密

​    {//下面数据全部用crypto_key加密

Package[0]{

totallen : u16,

cmdtype : u8, 

cmdflags : u16,

​        body //根据cmdtype不同而类型不同  

​      },

Package[1]{

totallen : u16,

cmdtype : u8, 

cmdflags : u16,

​        body //根据cmdtype不同而类型不同  

​      }, 

​    }

  },

box[1]{

​    boxsize : u16,//用当前crypto_key简单加密

​    {//下面数据全部用crypto_key加密

Package[0]{

totallen : u16,

cmdtype : u8, 

cmdflags : u16,

​        body //根据cmdtype不同而类型不同  

​      },

Package[1]{

totallen : u16,

cmdtype : u8, 

cmdflags : u16,

​        body //根据cmdtype不同而类型不同  

​      }, 

​    }

  }

},

**包合并**

包合并时一种用于减少延迟的通用机制.这在P2P网络中大量的尝试性通信中很重要。按我们的分层设计，LN打算给RN投递一个DataGram（通过加密协议），传统不合并包的逻辑如下:
 LN->RN:EXCHANGE_KEY
 RN->LN:EXCHANGE_KEY_OK
 LN->RN:SYN_TUNNEL
 RN->LN:ACK_TUNNEL
 LN->RN:ACKACK_TUNNEL
 LN->RN:DATAGRAM
 RN:通知UserAPI 开始处理Datagram RN->LN:DATAGRAM_RESP

经过包合并逻辑后，实际通信如下.
 LN->RN:EXCHANGE_KEY with SYN_TUNNEL with Datagram
 RN->LN:ACK_TUNNEL,通知UserAPI 开始处理Datagram
 LN->RN:ACKACK_TUNNEL
 RN->LN:DATAGRAM_RESP

合并过程：
 EXCHANGE_KEY with SYN_TUNNEL with Datagram ，当Datagram不大时合并生效。此时如果SYN_TUNNEL、Datagram中有一个字段的值与EXCHANGE_KEY中的相等，包合并机制允许在编码的时候不编码该字段。最大允许32个包在一个Box中进行合并。
 在这个机制的约束下，我们对协议包的逻辑字段起名时要注意必须有同样的含义，下面是一些通用字段的名称（不断更新）

//TODO:

{ 

  sessionid : uint32;//相对对端（目标端）唯一的id即可。通常是由对端在ack中回复的toSessionID

  sendTime : uint32;//包的发送时间。精确到0.1ms。需要在现有的UNIX 秒级timestamp上略微改造。

  seq : uint32;//动作序列号，重发包的时候，seq号不会发生变化

  packageID : uint32; //包ID，重发包的时候，packageID会发生变化

  hopLimit : uint8; 

  ackSeq : uint32;

  ackPackageID : uint32;

  result: uint16;

  fromPID : uint512;//发送方的peerID

  fromVPort: uint16;

  toPID : uint512;

  toVPort : uint16;

  flags : uint16;

  wndSize : uint32;

}

**包字段选填**

而是使用FLAGS来标识选填字段的方法来进一步提升性能。FLAGS用来描述Body中定义的字段是否出现。这是一种为性能优化的编码方式，缺陷是一个Package不能有超过16个逻辑字段（还好这么复杂的包是非常少的）。针对量最大的SessioData包和DataPiece包，其FLAG可与多个字段对应。

**无协议特征的加密设计：**

所谓的无协议特征是指，如果存在中间人，无法通过UDP/TCP的Payload信息，观测出LN<->RN之间是否在运行IEN协议。IEN主要通过密码学和随机端口的方法，来支持无协议特征。无协议特征并不能阻止中间人观测发现LN<->RN之间存在流量。抹去协议的流量统计学特征，需要靠两跳网络来实现。
 IEN的加密通信的无协议特征设计，将为其之上的区块链之上提供更基础的匿名性保护。

**MixHash****机制**

在进入全加密传输状态后，为了尽量消除包特征而引入的一种算法。通过MixHash算法，我们让通过全加密协议发送的package很难观测到明显的特征。。下面详细介绍一下MixHash机制的具体流程。
 **未Mix的流程:**

\1. LN,RN通过秘钥交换，约定了共同的秘钥AesKey.

\2. LN往RN发送的包，有一个固定长度的头，这个头的数据为Hash(AesKey)。包的数据部分都用AesKey加密

\3. RN收到包，与自己缓存的所有KeyHash进行对比，得到AesKey.从而知道该包来自LN

\4. RN用AesKey解密包体数据。此时如果中间人发现LN<->RN之间通信数据包，都有一个值相同的头，那么就可以判断LN<->RN在用IEN协议进行通信。

支持Mix的流程: 0. LN,RN都通过正确的方法校准了本地的系统时间

\1. LN,RN通过秘钥交换，约定了共同的秘钥AesKey.

\2. MixHash = Hash(AesKey..Time + d)。MixHash是AesKey和当前时间（最小粒度可以是10秒）的漂移(d)进行字符串拼接后的Hash.的漂移区间可以在一个随机范围MixTimeRange内选择[-5,+5],那么就有11个MixHash//TODO:可以优化，用2个字节的头来做自生成可以么？

\3. LN往RN发送的包，有一个固定长度的头，这个头的数据为MixHash。包的数据部分都用AesKey加密

\4. RN在本地通过定时器不断更新所有Key相关的MixHash.

\5. RN收到包，比较包头是否为本地存在的MixHash。推导得到AesKey.从而知道该包来自LN

\6. RN用AesKey解密包体数据。此时如果中间人对通信数据进行观测，会发现Package大部分时候的协议头都是不同的。

Mix流程的核心配置在于MixTimeRange.同时要求LN,RN的时间误差在接受范围之内。当MixTimeRange的区间取值范围越大，消耗的额外的存储空间越大，包头的多样性越强。
 如果MixTimeRange的取值区间为1，那么效果与未Mix相同。 //TODO：Mix算法可以考虑优化

**明文协议**

第一个bits用来标明该数据是明文协议还是加密协议。

IEN在专网（内网）中进行通信时，可以配置使用明文协议来提高性能，降低系统负载。IEN中的NDN传输和无确定目标广播协议，为了方便中间人加速，使用明文协议。 //TODO:仔细说明

**TCP****优先**

出于对网络全局性能、通过性、稳定性的考虑，我们在大部分场景下，默认使用TCP优先的原则。即LN，RN之间优先基于TCP协议进行通信。

 

**IEN****协议的分层结构**

分为3层。最底层为Tunnel，本层的协议设计主要关注“两个点的”可达性，并实现IEN设计中的网络传输语义：点对点传输和广播传输。中间层是 DHT，本层实现了IEN的去中心化组网逻辑（实现了去中心的Named Object Network) 上层为Session层，实现了IEN的网络应用语义：IENStream,Datagram,NamedData等。

**P2P Tunnel****的建立过程**

LN与RN建立P2P Tunnel的过程，实际上就是LN找到RN的一个可通信的<LocalEP,RemoteEP>对的过程。
 LN和RN之间，可能存在多个能通信的<LocalEP,RemoteEP>对。任何在NAT后面的节点，都需要通过SN才能知道自己的完整的list。
 SN Ping以及打洞的过程，是让RN的EndPoint对LN有效化的过程。

连接的逻辑过程:

\1. SYN，ACK命令都有发送方填写的timestamp

\2. LN通过自己所有的本地EndPoint往RN的所有已知EndPoint发送SYN(到发送端口收到ACK为止一直重发）

\3. LN如果知道RN在NAT后面，还需要往RN注册的SN发送Call。在Call中要包含自己最新的list

\4. RN收到Called命令后，往LN的所有已知EndPoint(通过called命令得到)发送ACK命令。

\5. RN在所有的udp onRecvFrom中，一旦收到合法的SYN_TUNNEL,都应回复ACK_TUNNEL命令

\6. LN收到多个ACK后，选出相对延迟最小（所以需要协议里有timestamp）的那个来源作为default remote EP.并把接收到这个ACK的端口设置为default local EP,

\7. 原理上在NAT后面的RN的多个interface都可以和SN保持Ping（不过NAT后面的节点一般很少能得知自己有多个WLAN interface),而且这个策略这会提高网络的运行成本。RN可以有一个稍微复杂一点的自我环境探测程序，用来检测自己的真实网络状况。

注意：第1步可能会组合爆炸同时发送多个SYN包，这里可以通过History里的统计数据可以进行一些策略性的调整。我们的链接策略分首次连接、快速重试、等逻辑阶段。可以针对这些不同的阶段定义不同的链接策略。
 连接过程通常描述的是在同一个协议族下的操作，如果EndPoint包含IPv4,IPv6，要考虑是否通过策略进行优先级调整（比如IPv6优先或IPv4优先)。 TODO:原理上是否允许，LN选择<EP1@LN，EP3@RN>,而RN选择<EP2@LN,EP4@RN>作为各自的default tunnel EP pair?

**本地EndPoint绑定与漫游友好**

在IEN协议栈的实现过程中，贯彻了Mobile First的原则。即认为协议栈创建时，绑定的设备当前EndPoint是会很容易改变的。比如说在手机上，经常在Wifi和移动数据网络中切换,从对端来说就是设备的IP地址在不断改变。
 由于P2P Tunnel的连接建立过程很关注IP Bind,所以我们大概率是不会简单的绑定0.0.0.0 地址的，而是需要精确的去绑定实际的IP地址，这要求我们有一个系统友好的探测程序，不断地向协议栈反馈当前有效的网络地址。
 当P2P Tunnel已经建立后，我们尽量在单方面发生漫游时，尝试保持住连接（注意BOX相关协议对这个特性的支持），同时也在解码时，几乎不依赖UDP Package的remote EP来进行包分发。
 在极端情况下，其实就是Tunnel并不关心remote EP.只要package能从parser中读取到，并近乎是合法的（加密包肯定是合法的），tunnel就会接受这个package，并更新当前的tunnel remote endpoint。

**Tunnel** **相关协议**

**EXCHANGE_KEY** **（UDP and TCP FirstPackage)**

由于交换密钥的成本问题，两个PeerID之间会约定一些可以使用的cryotp key.这个协议存在的目的就是在两个PeerID之间增加一个可用的crypto key。
 这个包任何一方都可以发起，发起方需要持有对端的PeerDesc.通常是LN<->RN通信的的第一个包。下面是ExchangeKeyPackage存在的两种形式

{//带交换密钥的加密Box,UDP 

  crypto_key : u256, //使用RN的Public Key加密后的crypto_key

  key_hash  : u64, //Key hash(mix hash),RN可以在解密crypto_key后进行校验

  {//下面数据全部用crypto_key加密

​    ExchangeKeyPackage {//这是一个特殊的Package,没有cmdtype,也不能向前引用

​      totalLen : u16,//总长度

​      cmdflags : u16,//用来指示字段是否存在

​      seq : u32,//用来过滤旧包。

​      seqAndKeySign:u128,//string(seq)+(aesKey)的LN签名，证明这个包确实是由fromPeerID发出的

​      fromPeerid:u256,//和老的PeerID最好是兼容的。这里不用填写toPeerID,因为public key的存在这是不言自明的。

​      peerInfo:Option<PeerDesc>,//如果LN确信RN知道自己的PublicKey,那么就可以不填写。

​      sendTime:u64,//精确到ms

​    },

  }

}

//带交换密钥的密文First Tcp Box

{

  crypto_key : u256, //使用RN的Public Key加密后的crypto_key

  key_hash  : u64, //Key hash(mix hash),RN可以在解密crypto_key后进行校验

  {

​    boxsize : u16,//使用crypto key进行弱加密的box长度。

​    {//下面数据全部用crypto_key加密

​      ExchangeKeyPackage {//这是一个特殊的Package,没有cmdtype,也不能向前引用

​        totalLen : u16,//总长度

​        cmdflags : u16,//用来指示字段是否存在

​        seq : u32,//用来过滤旧包。

​        seqAndKeySign:u128,//string(seq)+(aesKey)的LN签名，证明这个包确实是由fromPeerID发出的

​        fromPeerid:u256,//和老的PeerID最好是兼容的。这里不用填写toPeerID,因为public key的存在这是不言自明的。

​        peerInfo:Option<PeerDesc>,//如果LN确信RN知道自己的PublicKey,那么就可以不填写。

​        sendTime:u64,//精确到ms

​      },

​      ...

​    }

  }

}

EXCHANGE_KEY是一个非常特殊的包，不会单独被发出，否则会有先有鸡（tunnel）还是现有蛋的问题(package),毕竟RN能回复EXCHANGE_KEY,通常也表示Tunnel建立成功了。

考虑LN,RN同时向对方发起EXCHANGE_KEY的行为，这里最简单的方法是我们允许LN<->RN之间有多个Key? IEN协议并没由设定crypto key的有效期。但在具体实现的时候，可以约定实现是保存的key的最大数量和最长缓存时间。

ExchangeKey Resp: (//TODO) RN在处理EXCHANGE_KEY时，要注意识别已经交换过密钥的情况。如果是相同的cryptoAESKey，应该认为是重发包而予以忽略。当RN丢失自己的AesKey Cacheh后，LN可能并不知情，还是在用已经交换过密钥的Box继续同行。这时RN要及时发出“我不认识这个密钥”的回复，来帮助LN重新进入交换密钥的流程。正常情况下没有明确的回复，只需要按约定的密钥继续后续通信就可以了。EXCHANGE_KEY很少会作为独立的包发送的，通常都会with几个后续的逻辑包。
 错误的情况下去可以选
 \1. 不回复
 \2. 使用明文回复一个“我不知道你在说什么”包（这个版本不支持）

**SYN_TUNNEL (UDP & TCP FirstPackage) :**

在通过UDP方式发送时，进resend_queue,需要ACK_TUNNEL包来确认。

LN->RN: (下面描述只包含PackageBody部分)

{

  fromPeerid:PeerID,

  toPeerid:PeerID,

  proxyPID:Option<PeerID>,//可以选填这两项，告诉RN其实我是来自代理的

  proxyPeerInfo:Option<Desc>,

  seq:u32,

  fromContainerId:u32,

  peerDesc:Option<Desc>,

  sendTime:u64,//LN发送时的本地时间，精确到0.1ms 

  mtu:Option<u16>,//声明自己支持的最大mtu，这个值是基于TunnelContainer的。

  authDesc:Option<Desc>;//如果flags中指明了这是一个ProxyTunnel的握手协议，那么这个验证信息就是要填写的

}

SYN_TUNNEL,会是在建立TCP Tunnel时，在TCP Connection上发送的First Package //TODO:TCP部分需通过代码校对文档

**ACK_TUNNEL (UDP & TCP)**

**RN->LN:**

{

<aesKeyHash:u64>;//只在加密TCP中出现，则该字段明文传输标明身份 TODO:需要验证

  seq:u32;//与SYN包的Seq相同

  fromContainerId:u32,//说明是哪个SYN_TUNNEL的回复

  toContainerId:u32,//自生成

  result:u8,//是非0表示失败。错误代码TODO：

  peerDesc:Option<desc>,

  sendTime:u64,//RN回复时的本地时间，精确到0.1ms.

  mtu:Option<u16>,//可以回复自己支持的最大mtu，如果比SYN的mtu小，则回复

}

如果是TCP反连，那么这会是tunnel@TCP Connection上的First Package. //TODO:TCP部分需通过代码校对文档

**ACKACK_TUNNEL(UDP & TCP)**

{

  //cmd flags 在TCP下有一个RESET FLAG,用来让两端都回到初始状态。从而复用这个TCP Connection

seq:uuint32,

toContainerId:u32, 

}

**PING_TUNNEL(UDP)**

用来进行一些探测和保活的工作(特别是经过NAT建立的Tunnel).
 LN->RN(Req):

{

toContainerId:u32,

packageID:u32,//因为要做RTT探测，所以packageID(可以用来区分ep pair)

sendTime:u64,

recvData:u64,//LN在本tunnel上收到的来自RN的所有数据大小。//TODO 还有意义么?

}

RN->LN(Resp):

{

  toContainerId:u32,

  ackPackageID:u32,//因为要做RTT探测，所以packageID

  sendTime:u32,

  recvData:u64,

}

**Proxy****相关(TODO，需要Review设计)**

协议原生支持Proxy对未来系统的两条生态有重要的意义。鉴于移动互联网的发展，手机到手机的直连通常是不稳定和困难的，但如果手机通过POD进行互联，就会变得简单。
 此外，代理还能够帮助系统更好的磨平流量特征，或则帮助某些Peer隐藏自己的网络地址。实现模式和现在的主流的代理模式类似，分为正向代理和反向代理。

点到点代理需求

\1. LN和RN之间没有可用的链路,LN和Proxy,RN和Proxy都有可用的物理链路，则LN和RN可以通过LN->Proxy->RN的路径实现“点到点”通信。

\2. LN和RN之间有可用的链路，但速度不够快，LN和Proxy,RN和Proxy都有足够快的链路，则LN和RN可以通过LN->Rroxy->RN 的路径增加一条快速的链路。

\3. 上述场景中,LN、RN可以仅有一方或双方知道Proxy节点的存在。仅有一方知道的情况下，在另一方看来，这个链路和直接建立的点到点链路没有任何不同

\4. LN想和RN建立连接，但LN想要隐藏自己的身份(peerid)，让RN看起来是Proxy在和他建立连接。=>应用层协议

 

\5. RN不想通过自己的peerid提供服务，想通过Proxy的Peerid来提供服务。 =>应用层协议

\6. 上述任何需求下，LN,RN都不会把自己的私钥给Proxy

广播代理需求

\1. LN无法广播信息到指定组，需要通过Proxy节点来广播信息到指定组。在接收信息的人看来，这个信息是由LN广播的

\2. RN无法加入指定组，收不到广播信息。已经在组内的Proxy节点会将广播信息转发给RN.在LN看来，最终RN能收到该广播信息。

\3. LN不想以自己的身份广播信息，而是由Proxy来广播信息. 在收到信息的人看来，这个信息是由 Proxy发出的

\4. RN不想以自己的身份接收广播信息或加入组，而是由Proxy代劳。在组内看来， RN从来没加入过。RN能收到组里的广播信息。

\5. 上述任何需求下，LN,RN都不会把自己的私钥给Proxy

从上述需求分析来看，代理分“链路代理”和“身份代理” 两种。链路代理

LN->Proxy:正常建立Tunnel

LN->Proxy:BIND_RROXY_REQ(RN)

Proxy:Try Get RN PeerInfo

Proxy->LN:BIND_PROXY_RESP Bind RESULT,Return RN PeerInfo

LN->Proxy:BIND_PROXY_REQ(RN's SN)

Proxy->LN:BIND_PROXY_RESP Bind RESULT,Return RN's SN PeerInfo

 

LN->Proxy:PROXY_BOX_RN(EXCHANGE_KEY with SYN_TUNNEL with SYN_CONN).Proxy看不懂PROXY_BOX中的内容

LN->Proxy:PROXY_BOX_RNSN(EXCHANGE_KEY with SNCall(payload =EXCHANGE_KEY(RN) with SYN_TUNNEL with SYN_CONN ))

Proxy->RN:M*N send (EXCHANGE_KEY with SYN_TUNNEL with SYN_CONN) from LN

Proxy->RN'SN:Call payload=(EXCHANGE_KEY with SYN_TUNNEL with SYN_CONN) from LN

RN->Proxy:M*N send (ACK_TUNNEL with ACK_CONN)

Proxy->LN:PROXY_BOX_RN(ACK_TUNNEL with ACK_CONN)

LN->PROXY:PROXY_BOX_RN(ACKACK_TUNNEL with ACKACK_CONN)

PROXY->RN:(ACKACK_TUNNEL with ACKACK_CONN) from LN

考虑TCP_TUNNEL和TCP_CONNECTION

链路反向代理

RN->Proxy:正常建立tunnel

RN->Proxy:BIND_PROXY_REQ(self RN)

Proxy->RN: BIND_PROXY_RESP BindResult,Return EndPoint for RN

RN->Proxy:PROXY_BOX_RN(EXCHANGE_KEY with SNPing)

Proxy->SN:EXCHANGE_KEY with SNPing （从为RN分配的EndPoint发送）

SN->Proxy:SNPing Resp

Proxy->RN:PROXY_BOX_RN(SNPing Resp)

RN->Proxy:Update RN's PeerInfo(EndPoint)

LN->SN:Call RN

LN->Proxy:M*N send EXCHANGE_KEY with SYN_TUNNEL with SYN_CONN

Proxy:从给RN分配的EndPoint中读取到( EXCHANGE_KEY with SYN_TUNNEL with SYN_CONN),Proxy是无法识别出其中内容的。

Proxy->LN: PROXY_BOX(EXCHANGE_KEY with SYN_TUNNEL with SYN_CONN) //是否需要PROXY_BOX? 

SN->Proxy:Called from LN

Proxy:从给RN分配的EndPoint中读取到Called,Proxy是无法识别出Called的内容的。

Proxy->RN:PROXY_BOX(Called) //是否需要PROXY_BOX? 

RN->Proxy:PROXY_BOX(CalledResp)

Proxy->SN:CalledResp

RN->Proxy: M*N PROXY_BOX(ACK_TUNNEL with ACK_CONN)

Proxy->LN: M*N ACK_TUNNEL with ACK_CONN 

LN->Proxy: ACACK_TUNNEL with ACKACK_CONN

Proxy->RN: PROXY_BOX(ACACK_TUNNEL with ACKACK_CONN)

 

考虑TCP_TUNNEL和TCP_CONNECTION

身份代理: (Proxy一定能分析出RN与LN之间的通信内容么？)

LN->Proxy:正常建立Tunnel

LN->Proxy:BIND_RROXY_REQ(RN)

Proxy: Build Tunnel To RN

Proxy->LN: BIND_PROXY_RESP 

 

//不需要proxy tunnel?只要tunnel支持bind proxy就好了？节点可以选择和Proxy建立转发Tunnel,用来通过Proxy转发流量,这个Tunnel的建立还是使用标准的Tunnle相关协议（注意一些选填参数的填写),随后就建立一条新的ProxyTunnel。
 如果Proxy支持，TCPTunnel,TCPConnection也可通过代理使用。

ProxyTunnel建立后,LN把原本发送给RN的包，包在PROXY_BOX中发送给Proxy.由于peerid的长度，PROXY_BOX包中没有peerid字段，而是需要通过一个绑定操作来完成。在有Proxy参与的情况下，上述的LN给RN发送DataGram的流程改进如下：

LN->Proxy:EXCHANGE_KEY WITH SYN_TUNNEL(Proxy flag enable)

Proxy->LN:ACK_TUNNEL(Proxy flag enable)

LN->Proxy:ACKACK_TUNNEL(Proxy flag enable)

...

LN->Proxy:BIND_PROXY_REQ

Proxy->LN:BIND_PROXY_RESP

...

LN->Proxy:PROXY_BOX(EXCHANGE_KEY with SYN_TUNNEL with Datagram) 

Proxy->RN:EXCHANGE_KEY with SYN_TUNNEL with Datagram //这里PN实际上是把自己伪装成了LN,需要显示的说明一下么？

RN->Proxy:ACK_TUNNEL

Proxy->LN:PROXY_BOX(ACK_TUNNEL)

LN->Proxy:PROXY_BOX(ACKACK_TUNNEL)

Proxy->RN:ACKACK_TUNNEL  

RN->Proxy:DATAGRAM_RESP

**Proxy->LN:PROXY_BOX(DATAGRAM_RESP)** 

这是协议设计文档，只定义了当有Peer希望能成为代理时的标准行为。但为定义实现，也未定义LN的协议栈中有了一条 LN->PN的ProxyTunnel后，协议栈的工作会如何受到影响。这些都需要在实现架构里仔细讨论。阅读本端内容后，请认证思考ProxyTunnel与IP协议的二层代理的异同。

BIND_PROXY

这个绑定操作告诉代理节点，下面即将给这个目标peerID发送数据。Proxy可以根据自己的实现，决定要不要同意这个代理行为。一个ProxyTunnel上如果没有任何BIND操作，实际上是不能进行任何真正的代理转发工作的。 req:

{

  seq:uint32;

  bindOP:uint8//标示了是bind操作、是unbind操作、是getState操作

  bindPeerId:PeerID;

  <bindPeerInfo:Info>;//如果LN因为网络原因无法拿到目标PeerID的PeerInfo,可以不填写这个字段。

}

resp:

{

  seq:uint32;

  result:uint8;

  proxyID:uint32;//绑定成功后返回非0值。后面用到PROXY_BOX中，非常重要

  bindPeerInfo:Info;//绑定成功后填写，如果Proxy无法返回PeerInfo肯定是绑定失败了。

}

**PROXY_BOX**

{ //cmd flags 可以包含编码方式 proxyID:uint32//如果是发往Proxy的包，代表目的地，如果是来自 data:list;//编码后的原始数据。 } 那上面的例子里的一步来举个例子(LN与PN之间建立的是加密UDP ProxyTunnel)

相比之下，LN发给Proxy的包比Proxy发给RN的大了16字节。 TODO:最好罗列一下所有的Tunnel经过ProxyTunnel代理后上的典型包的样子。在上面的例子中，并没有详细阐述Proxy是如何和RN建立P2PTunnel的过程。这个细节实际上属于Proxy的实现，是可以持续优化的。这里提一点思路:

\1.  Proxy用自己的身份和RN建立了udp tunnel.从而获得了一个可以用的RN EndPoint.随后Proxy只需要用这个路径来转发LN发给Proxy的包就好了。当RN能直接收到一个TUNNEL_SYN包时，他的默认逻辑会直接回复ACK而忽略TUNNEL_SYN中的EndPoint List来进行尝试。（当然能隐藏这个列表也许能更好的处理重连逻辑）

\2.  Proxy用自己的身份和RN建立了TCP tunnel。如果这个TCP Tunnel是Proxy直连RN成功的，那么后续Proxy只需要建立好TCP连接，再等待LN的包进行转发就好了。

\3.  注意在全加密的情况下，如何LN和Proxy是UDP tunnel,Proxy和RN是TCP Tunnel，可能会无法进行直接转发。所以这里要求全对等(LN->Proxy与Proxy->RN的Tunnel类型必须是一致的）。全对等是好的！Tunnel代理和Connection代理都会要求路径的两边使用一样的链路。

\4.  Proxy用自己的身份和RN建立了TCP tunnel.但这个TCP Tunnel是反连成功的化，逻辑可能会有所复杂化。在RN看来，他只会反连Proxy而不会反连LN。这里在ACKACK包中加入了一个清除状态的行为，可以帮助RN复用这个TCP Conenction

\5.  ProxyChain：LN->Proxy1->Proxy2->RN.对LN来说，是看不到Proxy2的存在的。

**Proxy****对内置逻辑的支持**

\1. Proxy无法识别LN->RN的加密内容，所以没法进行任何逻辑操作。重发/ACK这些行为Proxy都不会做缓存。

\2. Proxy在可以识别LN->RN的明文内容的情况下，也不推荐做过多的操作。忠实的转发能让事情变简单。

**SN(SuperNode)**

SN是IEN网络长期稳定工作的重要节点，一个SN有如下功能： //TODO

\1. 帮助节点之间建立Tunnel

\2. 成为DHT表中的Known Node

\3. 在小型网络中进行"Quick Connect"

\4. Object Desc Cache.

每个节点在启动的时候，如果不确定自己是否在NAT后面，都可以找一个SN测试一下。
 所有不在NAT后面的节点，都可以成为SN。
 节点与SN进行通信时，需要和SN建立一条类似Tunnel的通道。这个通道与Tunnel并不完全相同(主要是策略为UDP优先)，常见逻辑如下：

RN->SN:

EXCHANGE_KEY with PING(如果RN是和SN首次通信需要EXCHANGE_KEY)

SN->RN:

SN_PING_RESP

RN->SN:

SN_PING

SN->RN:

SN_PING_RESP

 

LN->SN:

EXCHANGE_KEY with CAll(如果LN是和SN首次通信需要EXCHANGE_KEY，Payload中包含和RN交换密钥所需要的字段) 

SN->LN:

CALL_RESP

LN->RN:(多路尝试)

EXCHANGE_KEY_RN with SYN_TUNNEL with FirstPackage

SN->RN:

**CALLED**

RN->SN:

CALLED_RESP

RN->LN (多路尝试):

(ACK_TUNNEL with FirstPackage) or (Create TCP Crypto Tunnel with ACK_TUNNEL with FirstPackage)

明文流程会去掉上面所有的EXCHANGE_KEY。（TODO：我们几乎不在公网使用明文流程）

Node与SN之间的通信是UDP优先的，如果UDP通信不成，那么会尝试使用TCP。Node与SN之间的Tunnel方式并不影响LN，RN之间建立Tunnel的方式. LN与RN'S SN无法建立UDP通信:使用TCP发送CALL LN与LN'S SN无法建立UDP通信：使用TCP进行SN_PING,并在这个通道上获得PING_RESP和CALLED（但一般除非是自己UDP被封的原因，LN不会选择一个无法进行UDP通信的SN进行通信）。在这个前提下，LN与SN之间的TCP PING,只是帮助LN有机会进行反连。

SN发现与服务证明

从去中心化系统设计的角度来看，SN是一种特殊的Node，任何符合条件的节点都可以成为SN。

我们希望提供SN服务的节点能获得一定的收入，使用通用的“服务证明框架”思路，可以让IEN协议并不用与数字货币系统有太多的依赖，只是需要提供一套可验证的机制，证明“SN给用户A提供的服务，A是认可”的就好。

我们希望在经济利益的驱动下，系统中会存在更多的SN（以及ProxyNode），来增强整个IEN网络的通信可达性。

SN服务证明

A购买了SN的服务，在使用SN服务的过程中

1. 每次SNPing,SN都可以选择再PingResp中带上一个 "service report”,列出站在SN视角看到的为A的服务质量，包括时间段，在线时长，在不同的子时间段中处理Ping数量，处理Call->A请求的数量，处理A查询PeerInfo的数量
2. A如果认可该service report,则在下一个PingReq中带上Service report的签名
3. A如果觉得该service report有问题，但可以接受。可以在PingReq中带上签名的同时，也带上一份自己视角的Service     report (同时段)
4. A也可以主动发送Service Report(代表对服务的认可)
5. SN和A可以在其它的渠道进行服务购买的协商，并签订合约。当A不付钱的时候，SN就可以通过提交"A签名的Service Report"的方法，来获得保证金

缺点：作为服务的提供者SN，可以进行定向的错误构造（比如设计好在特点时间拦截特点人给A发送的Call消息），这可能会对A造成巨大的影响，但A无法区分这个是临时的服务故障还是

1. Call     B->SN->A ,CalledResp A->SN->B 这个环节里B可以在CallResp中期待A的签名，如果B能拿到A的CallResp签名，则证明SN正常工作
2.  

SN相关协议的设计

CALL:

注意LN可以不和RN所在的SN保持PING。这个包走的是LN到SN的Tunnel,所以加密方式采用与SN的约定。在必要的时候（LN完全不能使用UDP协议的情况下，也可以通过TCP方式发送Call) LN->SN:

Req:

{

  //cmd flag里包含一个ALWAYS_CALL_PEER的FLAG。默认情况下SN只把CALL命令发送给NAT后面的Peer以优化性能。

  seq:u32,

  snPeerid:PeerID,

  toPeerid:PeerID,//要call的对象

  fromPeerid:PeerID,

  reverseEndpointArray:Option<Array8<EndPoint>>,//TODO?

  peerInfo:Option<Desc>,

  sendtime:u64,

  payload:Array16<u8> //payload的数据会通过called包达到RN  

}

Resp:

{

  seq:u32,

  snPeerid:PeerID,

  result:uint8,//TODO:错误码定义

  toPeerInfo:Option<Desc>,//其中最重要的信息就是target的EndPointList

}

CALLED:

SN->RN:

Req:

{

  seq:u32，

  snPeerid:PeerID,

  toPeerid:PeerID,

  fromPeerid:PeerID,//谁来call我？

  reverseEndpointArray:Array8<EndPoint>,//重要，SN看到的LN EPList

  peerInfo:Desc,//TODO:这里应该是fromPeerInfo?

  callSeq:u32,

  callSendtime:u64, // call的sendtime

  payload:Array16<u8>,

}

Resp:

{

  seq:u32,

  snPeerid:PeerID,

  result:uint8,

}

**SN_PING (REG_SN):**

PING也相当于在SN上注册。IEN协议并未约定LN与SN之间进行PING的间隔。SN可以根据自己的负载情况要求LN减少PING的次数。（通过服务证明字段） LN->SN:

Req:

{

  seq:u32,

  snPeerid:PeerID,

  fromPeerid:Option<PeerID>,//如果和SN之间是加密通信，或则是TCP连接，则不需要fromPID

  peerDesc:Option<Desc>,//如果觉得没有改变，且离上次PING成功不久，不带也可以

  sendtime:u64,

  contractId:Option<ObjectId>,

  receipt:Option<(IENSnServiceReceipt,SIGNATURE_TYPE)>,//服务证明相关。这块的结构应该是要在协议里固定的

}

Resp:

{

  seq:u32,

  snPeerid:PeerID,

  result:u8,//TODO 补充错误码

  peerInfo:Option<Desc>,//SN的PeerInfo

  endpointArray:Array8<EndPoint>,//从SN角度看到的LN的EP(至返回变化部分即可),LN收到回复后使用合并的方法将EPListFromSN更新到自己的peerInfo中  

  receipt:Option<IENSnServiceReceipt>,如果该字段不为None, 则必须在下一次PING的时候提交证明。

}

**Session** **层协议**

**Connection (IENStream)**

建立逻辑上的P2PConnection是Session层最重要的基础抽象。特性和TCP一致：顺序可靠的字节流(Stream)。

连接的建立和传输过程是IEN的核心流程。从我们的底层设施上来看，真实的路径有一多。但对这些可能的路径进行理解，非常有助于对整体设计进行理解。

\1. LN与RN建立TCP直连

\2. LN与RN之间有UDP Tunnel,通过Tunnel上发送命令,RN反向和LN建立TCP连接

\3. LN与RN之间有TCP Tunnel,通过Tunnel上发送命令,RN反向和LN建立TCP连接

\4. LN通过SN Call RN. RN尝试与LN建立反向TCP连接，同时也在建立UDP Tunnel with Connnection.
 这个过程LN 在Call成功后不断的给RN发送EXCHANGE_KEY with SYN_TUNNEL with SYN_CONNECTION RN收到Call包后也会不断地给RN发送ACK_TUNNEL with ACK_CONNECTION (Call中包含了密钥交换，这里是可以用加密信道的) 同时RN也会尝试和LN建立TCP反连，一旦建立成功RN发送的的第一个包是 ACK_CONNECTION_TCPIMP

\5. 如果4的UDP Tunnel建立失败，TCP反连建立成功。RN会再次与 LN尝试建立一个TCP Tunnel.这个Tunnel的建立过程RN，LN角色互换。所以用标准的TCP Tunnel建立流程就好。

\6. 后续LN/RN之间再打算建立Connection，就可以走前面的流程了。

**CONNECTION** **协议定定义(SessionData)**

SessionData包是系统里跑的流量最大的包之一，在进行设计时做了优化。三次握手和结束的设计基本参考TCP.(SessionData包都是跑在UDP协议上的，目前我认为LN,RN之间能且仅能建立一条TCP的网络情况是不出现的) SYN,ACK,FIN,RESET这些包都是都是通过标志位来区分的

{

  sessionId:u32,//面向对端的sessionID.必填

  streamPos:u48,//可以从0开始

  //下面字段根据Flags选填

  {

synSeq:u32,

​     synControl:u8,//用来控制后续的连接策略，比如强制要求反连

​     ccType:u8,//期望的拥塞控制算法类型。

​     toVPort:u16,

​     fromSessionId:u32,  

  } synPart,//只有flags中包含了PACKAGE_SESSIONDATA_FLAG_SYN才会存在

  {

​    packageID:u32,//当ACK_PACKAGEID打开时，是ackPackageID

​    totalRecv:u64,

  } packageIDPart,//只有flag中包含了PACKAGEID或ACK_PACKAGEID才会存在

  ackStreamPos:Option<uint48>,

  toSessionId:Option<u32>,SYN_FLAG和ACK_FLAG都打开时(ACK)，该字段存在

  sackStreamRanges:Option<Array8<range>>,//sack的列表，可以在少量丢失数据的情况下更好的ack. TODO 目前实现未支持

  recvSpeedlimit:Option<u32>,//告知对端我方的最大接收速度

  sendtime:Option<u32>,//TODO:这里为什么用32位的时间?

  payload:Array<u8>//长度可以为0, 这里不带两字节头

}

下表说明了标志位和选填字段存在的关系。（目前用了10个标志位）

PACKAGE_SESSIONDATA_FLAG_PACKAGEID (1<<0)

packageIDPart存在，接收到该包后要立刻回复packageId ack.

 

PACKAGE_SESSIONDATA_FLAG_ACK_PACKAGEID (1<<1)

packageIDPart存在。

 

PACKAGE_SESSIONDATA_FLAG_SYN (1<<2)

SYN_CONN.synPart存在.

 

PACKAGE_SESSIONDATA_FLAG_ACK (1<<3) 

ackStreamPos存在。当SYN,ACK两个flag都存在时，toSessionId必须存在。

 

PACKAGE_SESSIONDATA_FLAG_SACK (1<<4) 

sackStreamRanges字段存在。用来进行高级ACK。TODO：目前实现未支持

 

PACKAGE_SESSIONDATA_FLAG_SPEEDLIMIT (1<<5)

speedlimit字段存在

 

PACKAGE_SESSIONDATA_FLAG_SENDTIME (1<<6)

sendtime字段存在

 

PACKAGE_SESSIONDATA_FLAG_PAYLOAD (1<<7)

有负载数据

 

PACKAGE_SESSIONDATA_FLAG_FIN (1<<8)

单端关闭

 

PACKAGE_SESSIONDATA_FLAG_RESET (1<<9)

强制关闭连接。

**在TCP_IMMP_TUNNEL运行的协议**

在必要的时候，EXCHANGE_KEY都可以作为第一个包。

TCP_SYN_CONNECTION (TCP FirstPackage)

LN使用TCP连接RN成功 LN->RN：

{

  seq:u32,

  result:u8,//TODO:错误码定义，为什么这里会有result?

  toVPort:u16,

  fromSessionId:u32,

  fromPeerid:Option<PeerID>,

  toPeerid:Option<PeerID>,

  proxyPID:Option<PeerID>,//可以选填这两项，告诉RN其实我是来自代理的

  proxyPeerInfo:Option<Desc>,

  peerInfo:Option<Desc>,//LN的PeerDesc,一般是明文协议填写

  reverseEndpointArray:Option<Array8<EndPoint>>,//TODO:需要明确意义。

  payload:Option<Array<u8>>,//FirstQ

  //<authInfo:info>;//如果flags中指明了这是一个ProxyTunnel的握手协议，那么这个验证信息就是要填写的

}

TCP_ACK_CONNECTION (TCP FirstPackage Resp or TCP FirstPackage)

有两种情况发送该Package： 1.是TCP_SYN_CONNECTION的Resp. RN->LN:TCP_ACK_CONNECTION (TCP FirstPackage Resp) 2.RN尝试反向建立到LN的TCP链接成功 RN->LN:TCP_ACK_CONNECTION (TCP FirstPackage)

{

  seq:u32,

  toSessionId:u32,

  result:u8,//TODO:错误码定义

  peerInfo:Option<Desc>;//明文协议可填

  payload:Option<Array<u8>>,//FirstA

}

TCP_ACKACK_CONNECTION

当RN尝试反向建立到LN的TCP链接成功后，发送了TCP_ACK_CONNECTION。LN需要回复TCP_ACKACK_CONNECTION确认。 LN->RN:TCP_ACKACK_CONNECTION(TCP FirstPackage Resp)

{

  //cmdflags:uint16;//在TCP下有一个RESET FLAG,用来让两端都回到初始状态。从而复用这个TCP Connection

  seq:u32,

  result:u8,//TODO:错误码定义

}

Connection核心逻辑简介

Connection被定义用来传输逻辑数据，等价于TCP。默认情况下，都是用加密方式进行传输。 TODO：主要介绍和TCP不同的部分

冗余带宽预测：

在常规加密传输数据的情况下，头部数据的大小为mixhash(8)+cmdlen(2)+cmdtype(1)+cmdflag(2) + sessionId(4) + streamPos(6) + ackStreamPos(6)，一共29字节。

拥塞控制支持：

从拥塞控制的机制上来说，主流实现有3种

window base的。要求协议支持ACK以及SACK机制

1. 延迟探测 base的。要求协议能低成本而准确的的探测当前链路上的延时

TODO：如果Conenction的底层是TCP Tunnel.拥塞控制机制还有效么?

**Datagram**

在一个Tunnel上发送报文。支持分片（最多8片) 广播报文也使用这个协议 TODO：这里故意没有设计VPort,跑跑看吧。如果DataGram需要接收方ack.那么需要在flags中打开需要重发标签。接收收到该DataGram后会发送一个带ackSeq的包。DataGram的ack粒度就是基于seq的。

**DATAGRAM** **协议**

DATAGRAM类似IP协议上的UDP协议是Session层暴露的偏底层的协议。比较直接的提供了点到点传输、广播语义。上层也可以基于Datagram协议开发自己的应用协议。

{

  seq:u32，

  from_vport:u16,//来源vport

  to_vport:u16,//目的vport

  destZone:Option<u32>,//TODO:Peerid的AreaCode相关。

  hopLimit:Option<u8>,//广播用，剩余跳数

  piece:Option<(u8,u8)>,//高位是分子，低位是分母。如果为None则表示该包不分片。

  ackPiece:Option<u32, (u8,u8)>, // <seq,(start_piece_index,end_piece_index)>

  sendTime:Option<u64>,

  authorPeerid:Option<Peerid>,//可选，DATAGRAM的创建PID

  authoPeerInfo:Option<Desc>,//可选，DATAGRAM的创建者PeerDesc

  dataSign:Option<TinySignBytes>,//data的签名,TODO:这个TINY SIGN要验证一下设计？

  innerCmdType:Option<u8>,//内部命令，TODO:该设计需要验证

  data:Array<u8>,//

}

 

 

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