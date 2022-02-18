/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
Intelligent_Eco_Networking_IEN_II_A_Knowledge-Driven_Future_Internet_Infrastructure_for_Value-Oriented_Ecosystem
**************************************************/
#pragma once
#include "../common/common.h"
namespace ien {
	namespace endpoint {

		const static long protocolUnknown = (1 << 0);
		const static long protocolTcp = (1 << 1);
		const static long protocolUdp = (1 << 2);

		const static long ipVersionUnknown = (1 << 0);
		const static long ipVersion4 = (1 << 3);
		const static long ipVersion6 = (1 << 4);

		const static long flagStaticWan = (1 << 6);
		const static long flagSigned = (1 << 7);

		const static long maxChars = 53;

		struct Endpoint {
			uint8_t flag;
			uint8_t reserved;
			uint16_t port;
			union {
				uint8_t address[4];
				uint8_t addressV6[16];
			};


			uint32_t fromString(const char* str);
			void toString(char name[maxChars + 1]) const;
			int compare(const Endpoint* right, bool ignorePort) const;
			bool isSameIpVersion(const Endpoint* right) const;
			void copyFrom(const Endpoint* src);

			int toSockAddr(struct sockaddr* addr) const;
			int fromSockAddr(const struct sockaddr* addr, uint8_t protocol);
			int family() const;
			size_t sockaddrSize() const;
			
			int read(BufferStream& stream);
			uint32_t write(BufferStream& stream, size_t* pWriteBytes) const;
		};
	}
	
}
