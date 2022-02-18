/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
               IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once
#include <vector>
#include <memory>
#include "../common/common.h"
#include "../base/base.h"

namespace ien {
    namespace protocol {
        const static size_t minPackageLength = 8;
        const static size_t minCryptoPackageLength = 16;
        const static uint8_t udpMagic[] = { 0x00, 0x80 };


        struct Package {
            uint8_t cmdtype;

            const static uint16_t flagDisableRef = 1 << 15;

            bool isSnPackage() const;
            bool isTcpPackage() const;

            virtual ~Package() {

            }

            static size_t minSize() {
                return 2;
            }

            static Package* create(int8_t cmdtype);
            virtual int copyFieldValue(const char* fieldName, void* pFieldValue) const = 0;
            virtual const void* fieldValue(const char* fieldName) const = 0;
            virtual int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const = 0;
            virtual int decode(BufferStream& bufferStream, const Package* pRefPackage) = 0;
        };
        
        struct ExchangeKey : Package {
            uint16_t cmdflags;
            uint32_t seq;
            uint8_t seqAndKeySign[crypto::signatureBytes];
            peer::Id fromPeerid;
            uint64_t sendTime;
            std::shared_ptr<peer::Info> peerInfo;

            const static uint8_t command = 0;

            struct Flags {
                const static uint16_t seq = (1 << 0);
                const static uint16_t seqAndKeySign = (1 << 1);
                const static uint16_t fromPeerid = (1 << 2);
                const static uint16_t peerInfo = (1 << 3);
                const static uint16_t sendTime = (1 << 4);
            };


            ExchangeKey() {
                this->cmdtype = ExchangeKey::command;
            }

            ~ExchangeKey() {

            }

            static size_t minSize() {
                return 94;
            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };
        
        struct SynTunnel : Package {
            uint16_t cmdflags;
            peer::Id fromPeerid;
            peer::Id toPeerid;
            peer::Id proxyPeerid;
            uint32_t seq;
            uint32_t fromContainerId;
            uint64_t sendTime;
            std::shared_ptr<peer::Info> proxyPeerInfo;
            std::shared_ptr<peer::Info> peerInfo;

            const static uint8_t command = 1;

            struct Flags {
                const static uint16_t fromPeerid = 1 << 0;
                const static uint16_t toPeerid = 1 << 1;
                const static uint16_t proxyPeerid = 1 << 2;
                const static uint16_t proxyPeerInfo = 1 << 3;
                const static uint16_t seq = 1 << 4;
                const static uint16_t fromContainerId = 1 << 5;
                const static uint16_t peerInfo = 1 << 6;
                const static uint16_t sendTime = 1 << 7;
            };

            SynTunnel() {
                this->cmdtype = command;
            }

            ~SynTunnel() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

        struct AckTunnel : Package {
            uint16_t cmdflags;
            uint64_t aesKeyHash;
            uint32_t seq;
            uint32_t fromContainerId;
            uint32_t toContainerId;
            uint8_t result;
            uint64_t sendTime;
            uint16_t mtu;
            std::shared_ptr<peer::Info> peerInfo;

            const static uint8_t command = 2;

            struct Flags {
                const static uint16_t aesKeyHash = 1 << 0;
                const static uint16_t seq = 1 << 1;
                const static uint16_t fromContainerId = 1 << 2;
                const static uint16_t toContainerId = 1 << 3;
                const static uint16_t result = 1 << 4;
                const static uint16_t peerInfo = 1 << 5;
                const static uint16_t sendTime = 1 << 6;
                const static uint16_t mtu = 1 << 7;
            };

            AckTunnel() {
                this->cmdtype = command;
            }

            ~AckTunnel() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

        struct AckAckTunnel : Package {
            uint16_t cmdflags;
            uint32_t seq;
            uint32_t toContainerId;

            const static uint8_t command = 3;

            struct Flags {
                const static uint16_t seq = 1 << 0;
                const static uint16_t toContainerId = 1 << 1;
            };

            AckAckTunnel() {
                this->cmdtype = command;
            }

            ~AckAckTunnel() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

        struct PingTunnel : Package {
            uint16_t cmdflags;
            uint32_t packageId;
            uint32_t toContainerId;
            uint64_t sendTime;
            uint64_t recvData;

            const static uint8_t command = 4;

            struct Flags {
                const static uint16_t packageId = 1 << 0;
                const static uint16_t sendTime = 1 << 1;
                const static uint16_t recvData = 1 << 2;
                const static uint16_t toContainerId = 1 << 3;
            };

            PingTunnel() {
                this->cmdtype = command;
            }

            ~PingTunnel() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

        struct PingTunnelResp : Package {
            uint16_t cmdflags;
            uint32_t ackPackageId;
            uint32_t toContainerId;
            uint64_t sendTime;
            uint64_t recvData;

            const static uint8_t command = 5;

            struct Flags {
                const static uint16_t ackPackageId = 1 << 0;
                const static uint16_t sendTime = 1 << 1;
                const static uint16_t recvData = 1 << 2;
                const static uint16_t toContainerId = 1 << 3;
            };

            PingTunnelResp() {
                this->cmdtype = command;
            }

            ~PingTunnelResp() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

        struct SnCall : Package {
            uint16_t cmdflags;
            uint32_t seq;
            peer::Id toPeerid;
            peer::Id fromPeerid;
            peer::Id snPeerid;
            std::vector<endpoint::Endpoint> reverseEndpointArray;
            std::shared_ptr<peer::Info> peerInfo;
            std::shared_ptr<Buffer> payload;

            const static uint8_t command = 0x20;

            struct Flags {
                const static uint16_t seq = 1 << 0;
                const static uint16_t toPeerid = 1 << 1;
                const static uint16_t fromPeerid = 1 << 2;
                const static uint16_t snPeerid = 1 << 3;
                const static uint16_t reverseEndpointArray = 1 << 4;
                const static uint16_t peerInfo = 1 << 5;
                const static uint16_t payload = 1 << 6;
                const static uint16_t alwaysCall = 1 << 14;
            };
            
            SnCall() {
                this->cmdtype = command;
            }

            ~SnCall() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

        struct SnCallResp : Package {
            uint16_t cmdflags;
            uint32_t seq;
            peer::Id snPeerid;
            uint8_t result;
            std::shared_ptr<peer::Info> toPeerInfo;

            const static uint8_t command = 0x21;

            struct Flags {
                const static uint16_t seq = 1 << 0;
                const static uint16_t snPeerid = 1 << 1;
                const static uint16_t result = 1 << 2;
                const static uint16_t toPeerInfo = 1 << 3;
            };
          
            SnCallResp() {
                this->cmdtype = command;
            }

            ~SnCallResp() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

        struct SnCalled : Package {
            uint16_t cmdflags;
            uint32_t seq;
            peer::Id toPeerid;
            peer::Id fromPeerid;
            peer::Id snPeerid;
            std::vector<endpoint::Endpoint> reverseEndpointArray;
            std::shared_ptr<peer::Info> peerInfo;
            std::shared_ptr<Buffer> payload;

            const static uint8_t command = 0x22;

            struct Flags {
                const static uint16_t seq = 1 << 0;
                const static uint16_t toPeerid = 1 << 1;
                const static uint16_t fromPeerid = 1 << 2;
                const static uint16_t snPeerid = 1 << 3;
                const static uint16_t reverseEndpointArray = 1 << 4;
                const static uint16_t peerInfo = 1 << 5;
                const static uint16_t payload = 1 << 6;
            };

            SnCalled() {
                this->cmdtype = command;
            }

            ~SnCalled() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

        struct SnCalledResp : Package {
            uint16_t cmdflags;
            uint32_t seq;
            uint8_t result;
            peer::Id snPeerid;

            const static uint8_t command = 0x23;

            struct Flags {
                const static uint16_t seq = 1 << 0;
                const static uint16_t result = 1 << 1;
                const static uint16_t snPeerid = 1 << 2;
            };

            SnCalledResp() {
                this->cmdtype = command;
            }

            ~SnCalledResp() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

        struct SnPing : Package {
            uint16_t cmdflags;
            uint32_t seq;
            peer::Id fromPeerid;
            peer::Id snPeerid;
            std::shared_ptr<peer::Info> peerInfo;

            const static uint8_t command = 0x24;

            struct Flags {
                const static uint16_t seq = 1 << 0;
                const static uint16_t fromPeerid = 1 << 1;
                const static uint16_t snPeerid = 1 << 2;
                const static uint16_t peerInfo = 1 << 3;
            };

            SnPing() {
                this->cmdtype = command;
            }

            ~SnPing() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

        struct SnPingResp : Package {
            uint16_t cmdflags;
            uint32_t seq;
            peer::Id snPeerid;
            uint8_t result;
            std::vector<endpoint::Endpoint> endpointArray;
            std::shared_ptr<peer::Info> peerInfo;

            const static uint8_t command = 0x25;

            struct Flags {
                const static uint16_t seq = 1 << 0;
                const static uint16_t snPeerid = 1 << 1;
                const static uint16_t result = 1 << 2;
                const static uint16_t peerInfo = 1 << 3;
                const static uint16_t endpointArray = 1 << 4;
            };

            SnPingResp() {
                this->cmdtype = command;
            }

            ~SnPingResp() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

        struct Datagram : Package {
            uint16_t cmdflags;
            uint32_t seq;
            uint32_t destZone;
            uint8_t hopLimit;
            uint16_t piece;
            uint32_t ackSeq;
            uint64_t sendTime;
            peer::Id authorPeerid;
            uint8_t dataSign[crypto::signatureTinyBytes];
            uint8_t innerCmdType;
            std::shared_ptr<peer::Info> authorPeerInfo;
            std::shared_ptr<Buffer> data;

            const static uint8_t command = 0x30;

            struct Flags {
                const static uint16_t seq = 1 << 0;
                const static uint16_t destZone = 1 << 1;
                const static uint16_t hopLimit = 1 << 2;
                const static uint16_t piece = 1 << 3;
                const static uint16_t ackSeq = 1 << 4;
                const static uint16_t sendTime = 1 << 5;
                const static uint16_t authorPeerid = 1 << 6;
                const static uint16_t authorPeerInfo = 1 << 7;
                const static uint16_t dataSign = 1 << 8;
                const static uint16_t innerCmdType = 1 << 9;
                const static uint16_t data = 1 << 10;
            };

            Datagram() {
                this->cmdtype = command;
            }

            ~Datagram() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

        struct SessionDataSynPart {
            uint32_t synSeq;
            uint8_t synControl;//用来控制后续的连接策略，比如强制要求反连
            uint8_t ccType;
            uint16_t toVPort;
            uint32_t fromSessionId;//自己本地的id,在握手的时候使用
        };

        struct SessionDataPackageIdPart {
            uint32_t packageId;//当ACK_PACKAGEID打开时，是ackPackageID
            uint64_t totalRecv;//48 bits长度
        };

        //为了提高SessionData包的encode/decode速度，将会使用一些特别的技巧。
        struct SessionData : Package {
            uint16_t cmdflags;
            uint32_t sessionId;//面向对端的sessionID.必填(对端的id，在最开始SYN的时候可能为0,ACK(SYN_FLAG和ACK_FLAG）的时候可以为0也可以为fromSessionID，有ack后就是fromSessionId了）
            uint64_t streamPos;//当前payload数据的起始位置，默认从0开始。必填。48bits;
            uint64_t ackStreamPos; //48bits;
            uint32_t toSessionId;//当SYN_FLAG和ACK_FLAG都打开时，该字段存在..ACK的时候发送给对方的
            uint32_t recvSpeedlimit;
            uint64_t sendTime;
            uint16_t payloadLength;
            uint8_t* payload;

            SessionDataSynPart* synPart;//只有flags中包含了SESSION_SYN才会存在
            SessionDataPackageIdPart* packageIDPart;//只有flag中包含了PACKAGEID或ACK_PACKAGEID才会存在

            const static uint8_t command = 0x40;

            struct Flags {
                const static uint16_t packageId = 1 << 0;
                const static uint16_t ackPackageId = 1 << 1;
                const static uint16_t syn = 1 << 2;
                const static uint16_t ack = 1 << 3;
                const static uint16_t sack = 1 << 4;
                const static uint16_t speedLimit = 1 << 5;
                const static uint16_t sendTime = 1 << 6;
                const static uint16_t payload = 1 << 7;
                const static uint16_t fin = 1 << 10;
                const static uint16_t finAck = 1 << 11;
                const static uint16_t reset = 1 << 12;
            };
           
            SessionData() {
                this->cmdtype = command;
            }

            ~SessionData() {
                if (this->synPart) {
                    delete this->synPart;
                }
                if (this->packageIDPart) {
                    delete this->packageIDPart;
                }
            }


            
            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
        };

        struct TcpSynConnection : Package {
            uint16_t cmdflags;
            uint32_t seq;
            uint8_t result;
            uint16_t toVPort;
            uint32_t fromSessionId;
            peer::Id fromPeerid;
            peer::Id toPeerid;
            peer::Id proxyPeerid;
            std::shared_ptr<peer::Info> peerInfo;
            std::vector<endpoint::Endpoint> reverseEndpointArray;
            std::shared_ptr<Buffer> payload;

            const static uint8_t command = 0x41;

            struct Flags {
                const static uint16_t seq = 1 << 0;
                const static uint16_t result = 1 << 1;
                const static uint16_t toVPort = 1 << 2;
                const static uint16_t fromSessionId = 1 << 3;
                const static uint16_t fromPeerid = 1 << 4;
                const static uint16_t toPeerid = 1 << 5;
                const static uint16_t proxyPeerid = 1 << 6;
                const static uint16_t peerInfo = 1 << 7;
                const static uint16_t reverseEndpointArray = 1 << 8;
                const static uint16_t payload = 1 << 9;
                const static uint16_t reverse = 1 << 10;
            };

            TcpSynConnection() {
                this->cmdtype = command;
            }

            ~TcpSynConnection() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

        struct TcpAckConnection : Package {
            uint16_t cmdflags;
            uint32_t seq;
            uint32_t toSessionId;
            uint8_t result;
            std::shared_ptr<peer::Info> peerInfo;
            std::shared_ptr<Buffer> payload;

            const static uint8_t command = 0x42;

            struct Flags {
                const static uint16_t seq = 1 << 0;
                const static uint16_t toSessionId = 1 << 1;
                const static uint16_t result = 1 << 2;
                const static uint16_t peerInfo = 1 << 3;
                const static uint16_t payload = 1 << 4;
            };

            TcpAckConnection() {
                this->cmdtype = command;
            }

            ~TcpAckConnection() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

        struct TcpAckAckConnection : Package {
            uint8_t cmdtype;
            uint16_t cmdflags;
            uint32_t seq;
            uint8_t result;

            const static uint8_t command = 0x43;
            
            struct Flags {
                const static uint16_t seq = 1 << 0;
                const static uint16_t result = 1 << 1;
            };

            TcpAckAckConnection() {
                this->cmdtype = command;
            }

            ~TcpAckAckConnection() {

            }

            int copyFieldValue(const char* fieldName, void* pFieldValue) const;
            const void* fieldValue(const char* fieldName) const;
            bool isDefaultField(uint16_t fieldFlags) const;
            int encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const;
            int decode(BufferStream& bufferStream, const Package* pRefPackage);
            static const char* fieldName(uint16_t fieldFlags);
        };

       
    }
}


