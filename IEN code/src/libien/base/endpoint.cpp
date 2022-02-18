#include "../common/common.h"
#include "./endpoint.h"

namespace ien {
	namespace endpoint {

		static const char* ipVersionCharsUnknown = "v?";
		static const char* ipVersionChars4 = "v4";
		static const char* ipVersionChars6 = "v6";

		static const char* protocolCharsUnknown = "unk";
		static const char* protocolCharsTcp = "tcp";
		static const char* protocolCharsUdp = "udp";

		uint32_t Endpoint::fromString(const char* str) {
			memset(this, 0, sizeof(Endpoint));
			int offset = 0;
			if (!memcmp(str, ipVersionChars4, 2)) {
				this->flag = this->flag | ipVersion4;
			} 
			else if (!memcmp(str, ipVersionChars6, 2)) {
				this->flag = this->flag | ipVersion6;
			}
			offset += 2;
			if (!memcmp(str + offset, protocolCharsTcp, 3)) {
				this->flag = this->flag | protocolTcp;
			} 
			else if (!memcmp(str + offset, protocolCharsUdp, 3)) {
				this->flag = this->flag | protocolUdp;
			}
			offset += 3;

			if (this->flag & ipVersion4) {
				int u8offset = 0;
				char u8[4];
				char split[4] = { '.', '.', '.', ':' };
				for (int ix = 0; ix < 4; ++ix) {
					memset(u8, 0, sizeof(u8));
					u8offset = 0;

					while (true) {
						char c = *(str + offset + u8offset);
						if (c != split[ix]) {
							u8[u8offset] = c;
							++u8offset;
							if (u8offset > 3) {
								return ICN_RESULT_INVALID_FORMAT;
							}
						} 
						else {
							break;
						}
					}
					if (!u8offset) {
						return ICN_RESULT_INVALID_FORMAT;
					}
					this->address[ix] = atoi(u8);
					offset += u8offset + 1;
					this->port = atoi(str + offset);
				}
			}
			else if (this->flag & ipVersion6) {
				if (str[offset] != '[') {
					return ICN_RESULT_INVALID_FORMAT;
				}
				++offset;


				int zeroStart = -1;
				int parts = 0;
				uint16_t address[8] = { 0, };
				do {
					uint8_t u16[4];

					if (str[offset] == ':') {
						++offset;
						// must start with ::
						if (str[offset] != ':')
						{
							return ICN_RESULT_INVALID_FORMAT;
						}
					}
					// explore parts like :abcd: or :abc] or ::
					while (true) {
						char c = str[offset];
						if (c == ':') {
							if (zeroStart == -1) {
								zeroStart = parts;
								offset += 1;
								continue;
							}
							else {
								// shouldn't contain multi ::
								return ICN_RESULT_INVALID_FORMAT;
							}
						}
						else if (c == ']') {
							offset += 1;
							// got end ]
							break;
						}
						int u16offset = 0;
						for (; u16offset < 4; ++u16offset) {
							c = str[offset + u16offset];
							if (isalpha(c)) {
								u16[u16offset] = 10 + (uint8_t)(c - (isupper(c) ? 'A' : 'a'));
							}
							else if (isalnum(c)) {
								u16[u16offset] = c - '0';
							}
							else {
								break;
							}
						}
						offset += u16offset;
						c = str[offset];
						if (c == ':'
							|| c == ']') {
							if (u16offset == 0) {
								if (zeroStart != -1) {
									// shouldn't contain multi ::
									return ICN_RESULT_INVALID_FORMAT;
								}
								zeroStart = parts;
								continue;
							}

							for (int ix = 0; ix < u16offset; ++ix) {
								*(address + parts) += (u16[ix] << (4 * (u16offset - 1 - ix)));
							}
							++offset;
							++parts;
							if (c == ']') {
								break;
							}
							if (parts == 8) {
								// more than 8 parts
								return ICN_RESULT_INVALID_FORMAT;
							}
						}
						else {
							return ICN_RESULT_INVALID_FORMAT;
						}
					}
				} while (false);

				if (zeroStart == -1) {
					if (parts < 8) {
						// less than 8 parts
						return ICN_RESULT_INVALID_FORMAT;
					}
					for (int ix = 0; ix < 8; ++ix) {
						// change sort 
						this->addressV6[ix << 1] = *((uint8_t*)(address + ix) + 1);
						this->addressV6[(ix << 1) + 1] = *((uint8_t*)(address + ix));
					}
				}
				else {
					if (parts == 8) {
						// more than 8 parts
						return ICN_RESULT_INVALID_FORMAT;
					}

					memset(this->addressV6, 0, 16);
					int iy = 0;
					int ix = 0;
					for (; ix < zeroStart; ++ix, ++iy) {
						this->addressV6[ix << 1] = *((uint8_t*)(address + iy) + 1);
						this->addressV6[(ix << 1) + 1] = *((uint8_t*)(address + iy));
					}
					for (ix = zeroStart + 8 - parts; ix < 8; ++ix, ++iy) {
						this->addressV6[ix << 1] = *((uint8_t*)(address + iy) + 1);
						this->addressV6[(ix << 1) + 1] = *((uint8_t*)(address + iy));
					}
				}

				if (str[offset] != ':') {
					return ICN_RESULT_INVALID_FORMAT;
				}
				++offset;
				this->port = ::atoi(str + offset);
			}

			return ICN_RESULT_SUCCESS;
		}

		void Endpoint::toString(char name[maxChars + 1]) const {
			memset(name, 0, maxChars + 1);
			size_t offset = 0;
			const char* ipv = ipVersionCharsUnknown;
			if (this->flag & ipVersion4) {
				ipv = ipVersionChars4;
			}
			else if (this->flag & ipVersion6) {
				ipv = ipVersionChars6;
			}

			const char* ptl = protocolCharsUnknown;
			if (this->flag & protocolTcp) {
				ptl = protocolCharsTcp;
			}
			else if (this->flag & protocolUdp) {
				ptl = protocolCharsUdp;
			}

			if (this->flag & ipVersion4) {
				sprintf(name, "%s%s%u.%u.%u.%u:%u",
					ipv, ptl,
					this->address[0],
					this->address[1],
					this->address[2],
					this->address[3],
					this->port);
			}
			else if (this->flag & ipVersion6) {
				uint16_t address[8];
				for (int8_t ix = 0; ix < 8; ++ix) {
					address[ix] = (((uint16_t)this->addressV6[ix << 1]) << 8) + this->addressV6[(ix << 1) + 1];
				}

				int8_t zeroStart = -1;
				int8_t zeroEnd = zeroStart;
				for (int8_t ix = 0; ix < 8; ++ix) {
					if (address[ix] == 0) {
						zeroEnd = zeroStart = ix;
						for (; ix < 8; ++ix) {
							if (address[ix] == 0) {
								++zeroEnd;
							}
							else {
								break;
							}
						}
						if (zeroEnd == zeroStart) {
							zeroStart = -1;
							continue;
						}
						else {
							break;
						}
					}
				}

				if (zeroStart == -1) {
					sprintf(name, "%s%s[%x:%x:%x:%x:%x:%x:%x:%x]:%u",
						ipv, ptl,
						address[0],
						address[1],
						address[2],
						address[3],
						address[4],
						address[5],
						address[6],
						address[7],
						this->port);
				}
				else {
					int ni = 0;
					ni += sprintf(name + ni, "%s%s[",
						ipv, ptl);
					int8_t ix = 0;
					if (zeroStart != 0) {
						for (; ix < zeroStart; ++ix) {
							ni += sprintf(name + ni, "%x:", address[ix]);
						}
					}
					else {
						ni += sprintf(name + ni, ":");
					}

					if (zeroEnd == 8) {
						ni += sprintf(name + ni, ":");
					}
					else {
						for (ix = zeroEnd; ix < 8; ++ix) {
							ni += sprintf(name + ni, ":%x", address[ix]);
						}
					}
					ni += sprintf(name + ni, "]:%u", this->port);
				}
			}
		}

		bool Endpoint::isSameIpVersion(const Endpoint* right) const {
			return (this->flag & ipVersion4
				&& right->flag & ipVersion4)
				|| (this->flag & ipVersion6
					&& right->flag & ipVersion6);
		}
		
		

		int Endpoint::compare(const Endpoint* right, bool ignorePort) const {

			const static long compareFlag = 
				ipVersion4
				| ipVersion6
				| ipVersionUnknown
				| protocolTcp
				| protocolUdp
				| protocolUnknown;

			uint8_t leftFlag = this->flag & compareFlag;
			uint8_t rightFlag = right->flag & compareFlag;
			if (leftFlag < rightFlag) {
				return -1;

			}
			if (leftFlag > rightFlag) {
				return 1;
			}
			int result = 0;
			if (leftFlag & ipVersion4) {
				result = memcmp(this->address, right->address, sizeof(this->address));
			}
			else {
				result = memcmp(this->addressV6, right->addressV6, sizeof(this->addressV6));
			}

			if (result != 0) {
				return result;
			}
			if (ignorePort) {
				return result;
			}
			if (this->port == right->port) {
				return 0;
			}
			return this->port > right->port ? 1 : -1;
		}

		void Endpoint::copyFrom(const Endpoint* src) {
			memcpy(this, src, sizeof(Endpoint));
		}

		int Endpoint::toSockAddr(struct sockaddr* addr) const {
			int ret = 0;
			if (this->flag & ipVersion4) {
				((struct sockaddr_in*)addr)->sin_family = AF_INET;
				memcpy((void*)(&(((struct sockaddr_in*)addr)->sin_addr)), (void*)(this->address), sizeof(this->address));
				((struct sockaddr_in*)addr)->sin_port = htons(this->port);
			}
			else if (this->flag & ipVersion6) {
				memset(addr, 0, sizeof(struct sockaddr_in6));
				((struct sockaddr_in6*)addr)->sin6_family = AF_INET6;
				memcpy((void*)(&(((struct sockaddr_in6*)addr)->sin6_addr)), (void*)(this->addressV6), sizeof(this->addressV6));
				((struct sockaddr_in6*)addr)->sin6_port = htons(this->port);
			}
			else {
				ret = ICN_RESULT_INVALID_PARAM;
			}

			return ret;
		}

		int Endpoint::fromSockAddr(const struct sockaddr* addr, uint8_t protocol) {
			int ret = 0;
			memset(this, 0, sizeof(Endpoint));
			if (addr->sa_family == AF_INET) {
				this->flag |= ipVersion4;
				memcpy((void*)(this->address), (void*)(&(((struct sockaddr_in*)addr)->sin_addr)), sizeof(((struct sockaddr_in*)addr)->sin_addr));
				this->port = ntohs(((struct sockaddr_in*)addr)->sin_port);
			}
			else if (addr->sa_family == AF_INET6) {
				this->flag |= ipVersion6;
				memcpy((void*)(this->addressV6), (void*)(&(((struct sockaddr_in6*)addr)->sin6_addr)), sizeof(((struct sockaddr_in6*)addr)->sin6_addr));
				this->port = ntohs(((struct sockaddr_in6*)addr)->sin6_port);
			}
			else {
				ret = ICN_RESULT_INVALID_PARAM;
			}

			this->flag |= (protocol & (protocolUnknown | protocolTcp | protocolUdp));

			return 0;
		}

		int Endpoint::family() const {
			int ret;
			if (this->flag & ipVersion4) {
				ret = AF_INET;
			}
			else if (this->flag & ipVersion6) {
				ret = AF_INET6;
			}
			else {
				ret = AF_UNSPEC;
			}

			return ret;
		}

		size_t Endpoint::sockaddrSize() const {
			size_t ret;
			if (this->flag & ipVersion4) {
				ret = sizeof(struct sockaddr_in);
			}
			else if (this->flag & ipVersion6) {
				ret = sizeof(struct sockaddr_in6);
			}
			else {
				ret = 0;
			}

			return ret;
		}


		int Endpoint::read(BufferStream& stream) {
			size_t startPos = stream.pos();
			int readSize = stream.readUInt8(&this->flag);
			if (readSize <= 0) {
				return readSize;
			}

			readSize = stream.readUInt16(&this->port);
			if (readSize <= 0) {
				return readSize;
			}

			if ((this->flag & ipVersion4) != 0) {
				readSize = stream.readByteArray(this->address, sizeof(this->address));
				if (readSize <= 0) {
					return readSize;
				}
			}
			else if ((this->flag & ipVersion6) != 0) {
				readSize = stream.readByteArray((uint8_t*)this->addressV6, sizeof(this->addressV6));
				if (readSize <= 0) {
					return readSize;
				}
			}
			return (int)(stream.pos() - startPos);
		}

		uint32_t Endpoint::write(BufferStream& stream, size_t* pWriteBytes) const {
			int r = 0;
			*pWriteBytes = 0;
			size_t writeBytes = 0;
			size_t totalWrite = 0;

			r = stream.writeUInt8(this->flag, &writeBytes);
			if (r != ICN_RESULT_SUCCESS) {
				return r;
			}
			totalWrite += writeBytes;
			writeBytes = 0;

			r = stream.writeUInt16(this->port, &writeBytes);
			if (r != ICN_RESULT_SUCCESS) {
				return r;
			}
			totalWrite += writeBytes;
			writeBytes = 0;

			if (this->flag & ipVersion4) {
				r = stream.writeByteArray(this->address, sizeof(this->address));
				writeBytes = sizeof(this->address);
			}
			else {
				r = stream.writeByteArray((uint8_t*)this->addressV6, sizeof(this->addressV6));
				writeBytes = sizeof(this->addressV6);
			}
			if (r != ICN_RESULT_SUCCESS) {
				return r;
			}
			totalWrite += writeBytes;
			writeBytes = 0;

			*pWriteBytes = writeBytes;
			return ICN_RESULT_SUCCESS;
		}
	}
}