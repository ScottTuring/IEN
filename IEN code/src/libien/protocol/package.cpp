#include <map>
#include "package.h"


namespace ien {
	namespace protocol {
		Package* Package::create(int8_t cmdtype) {
			switch (cmdtype) {
			case SynTunnel::command:
				return new SynTunnel();
			case AckTunnel::command:
				return new AckTunnel();
			case AckAckTunnel::command:
				return new AckAckTunnel();
			case PingTunnel::command:
				return new PingTunnel();
			case PingTunnelResp::command:
				return new PingTunnelResp();
			case SnCall::command:
				return new SnCall();
			case SnCallResp::command:
				return new SnCallResp();
			case SnCalled::command:
				return new SnCalled();
			case SnCalledResp::command:
				return new SnCalledResp();
			case SnPing::command:
				return new SnPing();
			case SnPingResp::command:
				return new SnPingResp();
			case Datagram::command:
				return new Datagram();
			case SessionData::command:
				return new SessionData();
			case TcpSynConnection::command:
				return new TcpSynConnection();
			case TcpAckConnection::command:
				return new TcpAckConnection();
			case TcpAckAckConnection::command:
				return new TcpAckAckConnection();
			default: 
				return NULL;
			}
		}

		bool Package::isSnPackage() const {
			return this->cmdtype >= SnCall::command && SnPingResp::command;
		}
		bool Package::isTcpPackage() const {
			return this->cmdtype >= TcpSynConnection::command && this->cmdtype <= TcpAckAckConnection::command;
		}

		int readEndpointArray(BufferStream& bufferStream, std::vector<endpoint::Endpoint>* out) {
			int r = 0;
			uint16_t arraySize;
			int totalRead = 0;
			r = bufferStream.readUInt16(&arraySize);
			if (r <= 0) {
				return 0;
			}
			totalRead += r;
			if (bufferStream.tail() < (size_t)(arraySize) * 6) {
				return 0;
			}
			for (int i = 0; i < arraySize; ++i)
			{
				endpoint::Endpoint ep;
				r = ep.read(bufferStream);

				if (r <= 0) {
					return 0;
				}
				totalRead += r;

				out->push_back(ep);
			}

			return totalRead;
		}

		int writeEndpointArray(BufferStream& bufferStream, const std::vector<endpoint::Endpoint>& in, size_t* pWriteBytes) {
			int r = 0;
			*pWriteBytes = 0;
			size_t writeBytes = 0;
			size_t totalWrite = 0;

			r = bufferStream.writeUInt16((uint16_t)in.size(), &writeBytes);
			if (r != ICN_RESULT_SUCCESS) {
				return r;
			}
			totalWrite += writeBytes;
			writeBytes = 0;

			for (int i = 0; i < in.size(); ++i) {
				r = in.at(i).write(bufferStream, &writeBytes);
				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWrite += writeBytes;
				writeBytes = 0;
			}
			*pWriteBytes = totalWrite;
			return ICN_RESULT_SUCCESS;
		}

		static const uint8_t defaultBytes16[16] = { 0 };
		static const uint8_t defaultBytes32[32] = { 0 };


		int ExchangeKey::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
			//assert(refPackage == NULL);
			size_t writeBytes = 0;
			size_t totalWriteBytes = 0;
			int r = 0;
			*pWriteBytes = 0;
			bool haveRef = false;
			bool isEqual = false;

			if (this == refPackage) {
				refPackage = NULL;
			}
			uint16_t cmdflags = this->cmdflags;
			if (cmdflags & flagDisableRef) {
				refPackage = NULL;
			}

			size_t headerPos = stream.pos();
			//totallen最后写 totallen,cmdflags
			stream.setPos(headerPos + 2 + 2);
			totalWriteBytes += 4;

			//写入数据
			r = stream.writeUInt32(this->seq, &writeBytes);
			if (r != ICN_RESULT_SUCCESS) {
				return r;
			}
			totalWriteBytes += writeBytes;
			writeBytes = 0;

			r = stream.writeByteArray(this->seqAndKeySign, crypto::signatureBytes);
			if (r != ICN_RESULT_SUCCESS) {
				return r;
			}
			totalWriteBytes += crypto::signatureBytes;
			writeBytes = 0;

			//fromPeerid
			haveRef = false;
			isEqual = false;
			if (refPackage) {
				const char* fieldName = ExchangeKey::fieldName(Flags::fromPeerid);
				if (fieldName) {
					peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
					if (rParam) {
						haveRef = true;
						isEqual = this->fromPeerid.compare(rParam) == 0;
						if (isEqual) {
							cmdflags &= (~Flags::fromPeerid);
						}
						else {
							cmdflags |= (Flags::fromPeerid);
						}
					}
				}
			}

			if (!haveRef) {
				cmdflags |= (Flags::fromPeerid);
			}

			if (cmdflags & (Flags::fromPeerid)) {
				//write to buffer
				r = this->fromPeerid.write(stream, &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}


			//peerinfo
			haveRef = false;
			isEqual = false;
			if (refPackage) {
				const char* fieldName = ExchangeKey::fieldName(Flags::peerInfo);
				if (fieldName) {
					std::shared_ptr<peer::Info>* rParam = (std::shared_ptr<peer::Info>*)refPackage->fieldValue(fieldName);
					if (rParam) {
						haveRef = true;
						isEqual = rParam->get() == this->peerInfo.get();
						if (isEqual) {
							cmdflags &= (~Flags::peerInfo);
						}
						else {
							cmdflags |= (Flags::peerInfo);
						}
					}
				}
			}

			if (!haveRef) {
				cmdflags |= (Flags::peerInfo);
			}

			if (cmdflags & (Flags::peerInfo)) {
				//write to buffer
				r = peer::Info::write(stream, this->peerInfo, &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}


			r = stream.writeInt64(this->sendTime, &writeBytes);
			if (r != ICN_RESULT_SUCCESS) {
				return r;
			}
			totalWriteBytes += writeBytes;
			writeBytes = 0;

			size_t tailPos = stream.pos();
			stream.setPos(headerPos);
			stream.writeUInt16((uint16_t)totalWriteBytes, &writeBytes);
			stream.writeUInt16(cmdflags, &writeBytes);
			stream.setPos(tailPos);
			*pWriteBytes = totalWriteBytes;

			return ICN_RESULT_SUCCESS;
		}

		int ExchangeKey::decode(BufferStream& stream, const Package* pRefPackage) {
			int r = 0;
			size_t minsize = minSize();
			uint16_t totallen = 0;
			uint16_t cmdflags;
			int readlen = 0;
			
			if (totallen < minsize) {
				goto END;
			}
			readlen += r;

			r = stream.readUInt16(&(cmdflags));
			if (r <= 0) {
				goto END;
			}
			readlen += r;
			this->cmdflags = cmdflags;
			bool isDisableRef = cmdflags & flagDisableRef;
			if (isDisableRef) {
				pRefPackage = NULL;
			}

			r = stream.readUInt32(&(this->seq));
			if (r <= 0) {
				goto END;
			}
			readlen += r;

			r = stream.readByteArray(this->seqAndKeySign, crypto::signatureBytes);
			if (r <= 0) {
				goto END;
			}
			readlen += r;

			//fromPeerid
			bool haveField_fromPeerid = cmdflags & Flags::fromPeerid;
			if (haveField_fromPeerid) {
				r = this->fromPeerid.read(stream);
				if (r <= 0) {
					r = -1;
					goto END;
				}
				readlen += r;
			}
			else {
				if (pRefPackage) {
					r = pRefPackage->copyFieldValue(ExchangeKey::fieldName(Flags::fromPeerid), &(this->fromPeerid));
				}
			}


			//peerInfo
			bool haveField_peerInfo = cmdflags & Flags::peerInfo;
			if (haveField_peerInfo) {
				r = peer::Info::read(stream, &(this->peerInfo));
				if (r <= 0) {
					r = -1;
					goto END;
				}
				readlen += r;
			}
			else {
				if (pRefPackage) {
					r = pRefPackage->copyFieldValue(ExchangeKey::fieldName(Flags::peerInfo), &(this->peerInfo));
				}
			}

			r = stream.readUInt64(&(this->sendTime));
			if (r <= 0) {
				goto END;
			}
			readlen += r;

			if (totallen != readlen) {
				r = 0;
				goto END;
			}

		END:
			if (r <= 0)
			{
				return r;
			}

			return readlen;
		}

		int Datagram::copyFieldValue(const char* fieldName, void* pFieldValue) const {
			assert(false);
			return ICN_RESULT_NOT_FOUND;
		}

		const void* Datagram::fieldValue(const char* fieldName) const {
			assert(false);
			return NULL;
		}

		int Datagram::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
			size_t writeBytes = 0;
			bool isEqual = false;
			bool haveRef = false;
			size_t totalWriteBytes = 0;
			int r = 0;
			*pWriteBytes = 0;

			if (this == refPackage) {
				refPackage = NULL;
			}
			uint16_t cmdflags = this->cmdflags;
			if (cmdflags & flagDisableRef) {
				refPackage = NULL;
			}

			size_t headerPos = stream.pos();

			//write cmdtype,totallen,cmdflags
			stream.setPos(headerPos + 1 + 2 + 2);
			totalWriteBytes += 5;

			haveRef = false;
			isEqual = false;
			if (refPackage) {
				const char* fieldName = Datagram::fieldName(Flags::seq);
				if (fieldName) {
					uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
					if (rParam) {
						haveRef = true;
						isEqual = (this->seq == *rParam);
						if (isEqual) {
							cmdflags &= (~Flags::seq);
						}
						else {
							cmdflags |= (Flags::seq);
						}
					}
				}
			}

			if (!haveRef) {
				if (this->isDefaultField(Flags::seq)) {
					cmdflags &= (~Flags::seq);
				}
				else {
					cmdflags |= (Flags::seq);
				}
			}

			if (cmdflags & Flags::seq) {
				//write to buffer
				r = stream.writeUInt32((this->seq), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}
			//-------------------------------------------------------------

			haveRef = false;
			isEqual = false;
			if (refPackage) {
				const char* fieldName = Datagram::fieldName(Flags::destZone);
				if (fieldName) {
					uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
					if (rParam) {
						haveRef = true;
						isEqual = (this->destZone == *rParam);
						if (isEqual) {
							cmdflags &= (~Flags::destZone);
						}
						else {
							cmdflags |= (Flags::destZone);
						}
					}
				}
			}

			if (!haveRef) {
				if (this->isDefaultField(Flags::destZone)) {
					cmdflags &= (~Flags::destZone);
				}
				else {
					cmdflags |= (Flags::destZone);
				}
			}

			if (cmdflags & Flags::destZone) {
				//write to buffer
				r = stream.writeUInt32((this->destZone), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}
			//-------------------------------------------------------------

			haveRef = false;
			isEqual = false;
			if (refPackage) {
				const char* fieldName = Datagram::fieldName(Flags::hopLimit);
				if (fieldName) {
					uint8_t* rParam = (uint8_t*)refPackage->fieldValue(fieldName);
					if (rParam) {
						haveRef = true;
						isEqual = (this->hopLimit == *rParam);
						if (isEqual) {
							cmdflags &= (~Flags::hopLimit);
						}
						else {
							cmdflags |= (Flags::hopLimit);
						}
					}
				}
			}

			if (!haveRef) {
				if (this->isDefaultField(Flags::hopLimit)) {
					cmdflags &= (~Flags::hopLimit);
				}
				else {
					cmdflags |= (Flags::hopLimit);
				}
			}

			if (cmdflags & Flags::hopLimit) {
				//write to buffer
				r = stream.writeUInt8((this->hopLimit), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}
			//-------------------------------------------------------------

			haveRef = false;
			isEqual = false;
			if (refPackage) {
				const char* fieldName = Datagram::fieldName(Flags::piece);
				if (fieldName) {
					uint16_t* rParam = (uint16_t*)refPackage->fieldValue(fieldName);
					if (rParam) {
						haveRef = true;
						isEqual = (this->piece == *rParam);
						if (isEqual) {
							cmdflags &= (~Flags::piece);
						}
						else {
							cmdflags |= (Flags::piece);
						}
					}
				}
			}

			if (!haveRef) {
				if (this->isDefaultField(Flags::piece)) {
					cmdflags &= (~Flags::piece);
				}
				else {
					cmdflags |= (Flags::piece);
				}
			}

			if (cmdflags & Flags::piece) {
				//write to buffer
				r = stream.writeUInt16((this->piece), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}
			//-------------------------------------------------------------

			haveRef = false;
			isEqual = false;
			if (refPackage) {
				const char* fieldName = Datagram::fieldName(Flags::ackSeq);
				if (fieldName) {
					uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
					if (rParam) {
						haveRef = true;
						isEqual = (this->ackSeq == *rParam);
						if (isEqual) {
							cmdflags &= (~Flags::ackSeq);
						}
						else {
							cmdflags |= (Flags::ackSeq);
						}
					}
				}
			}

			if (!haveRef) {
				if (this->isDefaultField(Flags::ackSeq)) {
					cmdflags &= (~Flags::ackSeq);
				}
				else {
					cmdflags |= (Flags::ackSeq);
				}
			}

			if (cmdflags & Flags::ackSeq) {
				//write to buffer
				r = stream.writeUInt32((this->ackSeq), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}
			//-------------------------------------------------------------

			haveRef = false;
			isEqual = false;
			if (refPackage) {
				const char* fieldName = Datagram::fieldName(Flags::sendTime);
				if (fieldName) {
					uint64_t* rParam = (uint64_t*)refPackage->fieldValue(fieldName);
					if (rParam) {
						haveRef = true;
						isEqual = (this->sendTime == *rParam);
						if (isEqual) {
							cmdflags &= (~Flags::sendTime);
						}
						else {
							cmdflags |= (Flags::sendTime);
						}
					}
				}
			}

			if (!haveRef) {
				if (this->isDefaultField(Flags::sendTime)) {
					cmdflags &= (~Flags::sendTime);
				}
				else {
					cmdflags |= (Flags::sendTime);
				}
			}

			if (cmdflags & Flags::sendTime) {
				//write to buffer
				r = stream.writeUInt64((this->sendTime), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}
			//-------------------------------------------------------------

			haveRef = false;
			isEqual = false;
			if (refPackage) {
				const char* fieldName = Datagram::fieldName(Flags::authorPeerid);
				if (fieldName) {
					peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
					if (rParam) {
						haveRef = true;
						isEqual = (this->authorPeerid.compare(rParam) == 0);
						if (isEqual) {
							cmdflags &= (~Flags::authorPeerid);
						}
						else {
							cmdflags |= (Flags::authorPeerid);
						}
					}
				}
			}

			if (!haveRef) {
				if (this->isDefaultField(Flags::authorPeerid)) {
					cmdflags &= (~Flags::authorPeerid);
				}
				else {
					cmdflags |= (Flags::authorPeerid);
				}
			}

			if (cmdflags & Flags::authorPeerid) {
				//write to buffer
				r = this->authorPeerid.write(stream, &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}
			//-------------------------------------------------------------

			haveRef = false;
			isEqual = false;
			if (refPackage) {
				const char* fieldName = Datagram::fieldName(Flags::authorPeerInfo);
				if (fieldName) {
					std::shared_ptr<peer::Info>* rParam = (std::shared_ptr<peer::Info>*)refPackage->fieldValue(fieldName);
					if (rParam) {
						haveRef = true;
						isEqual = (this->authorPeerInfo.get() == (*rParam).get());
						if (isEqual) {
							cmdflags &= (~Flags::authorPeerInfo);
						}
						else {
							cmdflags |= (Flags::authorPeerInfo);
						}
					}
				}
			}

			if (!haveRef) {
				if (this->isDefaultField(Flags::authorPeerInfo)) {
					cmdflags &= (~Flags::authorPeerInfo);
				}
				else {
					cmdflags |= (Flags::authorPeerInfo);
				}
			}

			if (cmdflags & Flags::authorPeerInfo) {
				//write to buffer
				r = peer::Info::write(stream, (this->authorPeerInfo), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}
			//-------------------------------------------------------------


			if (cmdflags & Flags::dataSign) {
				//write to buffer
				r = stream.writeByteArray(this->dataSign, crypto::signatureTinyBytes);
				writeBytes = crypto::signatureTinyBytes;

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}
			//-------------------------------------------------------------

			haveRef = false;
			isEqual = false;
			if (refPackage) {
				const char* fieldName = Datagram::fieldName(Flags::innerCmdType);
				if (fieldName) {
					uint8_t* rParam = (uint8_t*)refPackage->fieldValue(fieldName);
					if (rParam) {
						haveRef = true;
						isEqual = (this->innerCmdType == *rParam);
						if (isEqual) {
							cmdflags &= (~Flags::innerCmdType);
						}
						else {
							cmdflags |= (Flags::innerCmdType);
						}
					}
				}
			}

			if (!haveRef) {
				if (this->isDefaultField(Flags::innerCmdType)) {
					cmdflags &= (~Flags::innerCmdType);
				}
				else {
					cmdflags |= (Flags::innerCmdType);
				}
			}

			if (cmdflags & Flags::innerCmdType) {
				//write to buffer
				r = stream.writeUInt8((this->innerCmdType), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}
			//-------------------------------------------------------------

			haveRef = false;
			isEqual = false;
			if (refPackage) {
				const char* fieldName = Datagram::fieldName(Flags::data);
				if (fieldName) {
					std::shared_ptr<Buffer>* rParam = (std::shared_ptr<Buffer>*)refPackage->fieldValue(fieldName);
					if (rParam) {
						haveRef = true;
						isEqual = false;
						if (isEqual) {
							cmdflags &= (~Flags::data);
						}
						else {
							cmdflags |= (Flags::data);
						}
					}
				}
			}

			if (!haveRef) {
				if (this->isDefaultField(Flags::data)) {
					cmdflags &= (~Flags::data);
				}
				else {
					cmdflags |= (Flags::data);
				}
			}

			if (cmdflags & Flags::data) {
				size_t datalen;
				uint8_t* pData = this->data->data(&datalen);
				r = stream.writeByteArray(pData, datalen);
				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += datalen;
			}
			//-------------------------------------------------------------

			size_t tailPos = stream.pos();
			stream.setPos(headerPos);
			stream.writeUInt16((uint16_t)totalWriteBytes, &writeBytes);
			stream.writeUInt8(this->cmdtype, &writeBytes);
			stream.writeUInt16(cmdflags, &writeBytes);
			stream.setPos(tailPos);
			*pWriteBytes = totalWriteBytes;

			return ICN_RESULT_SUCCESS;
		}

		int Datagram::decode(BufferStream& bufferStream, const Package* pRefPackage) {
			int r = 0;
			size_t minsize = minSize();
			uint16_t cmdflags;
			int readlen = 0;

			if (bufferStream.tail() < minsize) {
				goto END;
			}

			r = bufferStream.readUInt16(&(cmdflags));
			if (r <= 0) {
				goto END;
			}
			readlen += r;
			this->cmdflags = cmdflags;
			bool isDisableRef = cmdflags & flagDisableRef;
			if (isDisableRef) {
				pRefPackage = NULL;
			}

			{
				bool hasField = cmdflags & Flags::seq;
				if (hasField) {
					r = bufferStream.readUInt32(&this->seq);
					if (r <= 0) {
						r = -1;
						goto END;
					}
					readlen += r;
				}
				else {
					if (pRefPackage) {
						r = pRefPackage->copyFieldValue(Datagram::fieldName(Flags::seq), (void*)&(this->seq));
					}
				}
			}
			{
				bool hasField = cmdflags & Flags::destZone;
				if (hasField) {
					r = bufferStream.readUInt32(&this->destZone);
					if (r <= 0) {
						r = -1;
						goto END;
					}
					readlen += r;
				}
				else {
					if (pRefPackage) {
						r = pRefPackage->copyFieldValue(Datagram::fieldName(Flags::destZone), (void*)&(this->destZone));
					}
				}
			}
			{
				bool hasField = cmdflags & Flags::hopLimit;
				if (hasField) {
					r = bufferStream.readUInt8(&this->hopLimit);
					if (r <= 0) {
						r = -1;
						goto END;
					}
					readlen += r;
				}
				else {
					if (pRefPackage) {
						r = pRefPackage->copyFieldValue(Datagram::fieldName(Flags::hopLimit), (void*)&(this->hopLimit));
					}
				}
			}
			{
				bool hasField = cmdflags & Flags::piece;
				if (hasField) {
					r = bufferStream.readUInt16(&this->piece);
					if (r <= 0) {
						r = -1;
						goto END;
					}
					readlen += r;
				}
				else {
					if (pRefPackage) {
						r = pRefPackage->copyFieldValue(Datagram::fieldName(Flags::piece), (void*)&(this->piece));
					}
				}
			}
			{
				bool hasField = cmdflags & Flags::ackSeq;
				if (hasField) {
					r = bufferStream.readUInt32(&this->ackSeq);
					if (r <= 0) {
						r = -1;
						goto END;
					}
					readlen += r;
				}
				else {
					if (pRefPackage) {
						r = pRefPackage->copyFieldValue(Datagram::fieldName(Flags::ackSeq), (void*)&(this->ackSeq));
					}
				}
			}
			{
				bool hasField = cmdflags & Flags::sendTime;
				if (hasField) {
					r = bufferStream.readUInt64(&this->sendTime);
					if (r <= 0) {
						r = -1;
						goto END;
					}
					readlen += r;
				}
				else {
					if (pRefPackage) {
						r = pRefPackage->copyFieldValue(Datagram::fieldName(Flags::sendTime), (void*)&(this->sendTime));
					}
				}
			}
			{
				bool hasField = cmdflags & Flags::authorPeerid;
				if (hasField) {
					r = this->authorPeerid.read(bufferStream);
					if (r <= 0) {
						r = -1;
						goto END;
					}
					readlen += r;
				}
				else {
					if (pRefPackage) {
						r = pRefPackage->copyFieldValue(Datagram::fieldName(Flags::authorPeerid), (void*)&(this->authorPeerid));
					}
				}
			}
			{
				bool hasField = cmdflags & Flags::authorPeerInfo;
				if (hasField) {
					r = peer::Info::read(bufferStream, &this->authorPeerInfo);
					if (r <= 0) {
						r = -1;
						goto END;
					}
					readlen += r;
				}
				else {
					if (pRefPackage) {
						r = pRefPackage->copyFieldValue(Datagram::fieldName(Flags::authorPeerInfo), (void*)&(this->authorPeerInfo));
					}
				}
			}
			{
				bool hasField = cmdflags & Flags::dataSign;
				if (hasField) {
					r = bufferStream.readByteArray(this->dataSign, crypto::signatureTinyBytes);
					if (r <= 0) {
						r = -1;
						goto END;
					}
					readlen += r;
				}
				else {
					if (pRefPackage) {
						r = pRefPackage->copyFieldValue(Datagram::fieldName(Flags::dataSign), (void*)&(this->dataSign));
					}
				}
			}
			{
				bool hasField = cmdflags & Flags::innerCmdType;
				if (hasField) {
					r = bufferStream.readUInt8(&this->innerCmdType);
					if (r <= 0) {
						r = -1;
						goto END;
					}
					readlen += r;
				}
				else {
					if (pRefPackage) {
						r = pRefPackage->copyFieldValue(Datagram::fieldName(Flags::innerCmdType), (void*)&(this->innerCmdType));
					}
				}
			}
			{
				bool hasField = cmdflags & Flags::data;
				if (hasField) {
					uint16_t datalen = bufferStream.tail();//剩下的都是
					assert(datalen > 0);
					if (datalen <= 0) {
						r = -1;
						goto END;
					}

					this->data = Buffer::bind(bufferStream.buffer() + bufferStream.pos(), datalen);
					size_t thisPos = bufferStream.pos();
					bufferStream.setPos(thisPos + datalen);
					readlen += datalen;
				}
			}
			r = ICN_RESULT_SUCCESS;
		END:
			return r;
		}


		int SessionData::copyFieldValue(const char* fieldName, void* pFieldValue) const {
			assert(false);
			return ICN_RESULT_NOT_FOUND;
		}

		const void* SessionData::fieldValue(const char* fieldName) const {
			assert(false);
			return NULL;
		}

		int SessionData::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
			size_t writeBytes = 0;
			bool isEqual = false;
			bool haveRef = false;
			size_t totalWriteBytes = 0;
			int r = 0;
			*pWriteBytes = 0;

			if (this == refPackage) {
				refPackage = NULL;
			}

			uint16_t cmdflags = this->cmdflags;
			if (cmdflags & flagDisableRef) {
				refPackage = NULL;
			}

			size_t headerPos = stream.pos();

			//write cmdtype,totallen,cmdflags
			stream.setPos(headerPos + 1 + 2 + 2);
			totalWriteBytes += 5;

			r = stream.writeUInt32((this->sessionId), &writeBytes);

			if (r != ICN_RESULT_SUCCESS) {
				return r;
			}
			totalWriteBytes += writeBytes;
			writeBytes = 0;
			//-------------------------------------------------------------

			r = stream.writeUInt48(this->streamPos, &writeBytes);

			if (r != ICN_RESULT_SUCCESS) {
				return r;
			}
			totalWriteBytes += writeBytes;
			writeBytes = 0;
			//-------------------------------------------------------------


			if (this->synPart) {
				cmdflags |= (Flags::syn);

				//write synPart.synSeq
				r = stream.writeUInt32((this->synPart->synSeq), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;

				//write synPart.synControl
				r = stream.writeUInt8((this->synPart->synControl), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;

				//write to synPart.ccType
				r = stream.writeUInt8((this->synPart->ccType), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;

				//write to synPart.toVPort
				r = stream.writeUInt16((this->synPart->toVPort), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;

				//write to synPart.fromSessionId
				r = stream.writeUInt32((this->synPart->fromSessionId), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}
			else {
				cmdflags &= (~Flags::syn);
			}

			if (this->packageIDPart) {
				cmdflags |= (Flags::packageId);
				r = stream.writeUInt32((this->packageIDPart->packageId), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;


				r = stream.writeUInt48((this->packageIDPart->totalRecv), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}
			else {
				cmdflags &= (~Flags::packageId);
			}

			//ackStreamPos and sack array
			if (this->ackStreamPos != 0) {
				cmdflags |= Flags::ack;
			}
			if (cmdflags & Flags::ack) {
				//write to buffer
				r = stream.writeUInt48((this->ackStreamPos), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}

			//toSessionId
			if ((cmdflags & Flags::syn) && (cmdflags & Flags::ack)) {
				//write to buffer
				r = stream.writeUInt32((this->toSessionId), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}

			//sendtime
			if (this->sendTime == 0) {
				cmdflags &= (~Flags::sendTime);
			}
			else {
				cmdflags |= Flags::sendTime;
			}
			if (cmdflags & Flags::sendTime) {
				//write to buffer
				r = stream.writeUInt64((this->sendTime), &writeBytes);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += writeBytes;
				writeBytes = 0;
			}

			if (this->payload == NULL) {
				cmdflags &= (~Flags::payload);
			}
			else {
				cmdflags |= Flags::payload;
			}
			if (cmdflags & Flags::payload) {
				r = stream.writeByteArray(this->payload, this->payloadLength);
				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalWriteBytes += this->payloadLength;
			}

			size_t tailPos = stream.pos();
			stream.setPos(headerPos);
			stream.writeUInt16((uint16_t)totalWriteBytes, &writeBytes);
			stream.writeUInt8(this->cmdtype, &writeBytes);
			stream.writeUInt16(cmdflags, &writeBytes);
			stream.setPos(tailPos);
			*pWriteBytes = totalWriteBytes;

			return ICN_RESULT_SUCCESS;
		}


		int SessionData::decode(BufferStream& stream, const Package* pRefPackage) {
			int r = 0;
			size_t minsize = minSize();
			uint16_t cmdflags;
			int readlen = 0;

			if (stream.tail() < minsize) {
				goto END;
			}

			r = stream.readUInt16(&(cmdflags));
			if (r <= 0) {
				goto END;
			}
			readlen += r;
			this->cmdflags = cmdflags;

			//sessionId;
			r = stream.readUInt32(&(this->sessionId));
			if (r <= 0) {
				r = -1;
				goto END;
			}
			readlen += r;

			//streamPos;
			r = stream.readUInt48(&(this->streamPos));
			if (r <= 0) {
				r = -1;
				goto END;
			}
			readlen += r;

			bool haveField_synPart = cmdflags & Flags::syn;
			if (haveField_synPart) {
				if (this->synPart == NULL) {
					this->synPart = new SessionDataSynPart;
				}
				r = stream.readUInt32(&(this->synPart->synSeq));
				if (r <= 0) {
					r = -1;
					goto END;
				}
				readlen += r;

				r = stream.readUInt8(&(this->synPart->synControl));
				if (r <= 0) {
					r = -1;
					goto END;
				}
				readlen += r;

				r = stream.readUInt8(&(this->synPart->ccType));
				if (r <= 0) {
					r = -1;
					goto END;
				}
				readlen += r;

				r = stream.readUInt16(&(this->synPart->toVPort));
				if (r <= 0) {
					r = -1;
					goto END;
				}
				readlen += r;

				r = stream.readUInt32(&(this->synPart->fromSessionId));
				if (r <= 0) {
					r = -1;
					goto END;
				}
				readlen += r;
			}

			bool haveField_packageId = (cmdflags & Flags::packageId) || (cmdflags & Flags::ackPackageId);
			if (haveField_packageId) {
				if (this->packageIDPart == NULL) {
					this->packageIDPart = new SessionDataPackageIdPart;
				}
				r = stream.readUInt32(&(this->packageIDPart->packageId));
				if (r <= 0) {
					r = -1;
					goto END;
				}
				readlen += r;

				r = stream.readUInt48(&(this->packageIDPart->totalRecv));
				if (r <= 0) {
					r = -1;
					goto END;
				}
				readlen += r;
			}


			bool haveField_ackStreamPos = cmdflags & Flags::ack;
			if (haveField_ackStreamPos) {
				r = stream.readUInt48(&(this->ackStreamPos));
				if (r <= 0) {
					r = -1;
					goto END;
				}
				readlen += r;
			}

			
			bool haveField_toSessionId = (cmdflags & Flags::syn) && (cmdflags & Flags::ack);
			if (haveField_toSessionId) {
				r = stream.readUInt32(&(this->toSessionId));
				if (r <= 0) {
					r = -1;
					goto END;
				}
				readlen += r;
			}


			bool haveField_sendtime = cmdflags & Flags::sendTime;
			if (haveField_sendtime) {
				r = stream.readUInt64(&(this->sendTime));
				if (r <= 0) {
					r = -1;
					goto END;
				}
				readlen += r;
			}

			bool have_Payload = cmdflags & Flags::payload;
			if (have_Payload) {
				uint16_t datalen = stream.tail() - (uint16_t)readlen;//剩下的都是paylaod
				assert(datalen > 0);
				if (datalen <= 0) {
					r = -1;
					goto END;
				}


				this->payload = stream.buffer() + stream.pos();
				this->payloadLength = datalen;
				size_t thisPos = stream.pos();
				stream.setPos(thisPos + datalen);
				readlen += datalen;
			}

			r = ICN_RESULT_SUCCESS;
		END:
			return r;
		}


		#include "./package.imp.inl"
	}
}

