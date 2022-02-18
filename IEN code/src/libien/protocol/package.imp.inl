
struct FieldNames {

static const char* seq() { return "seq"; }
static const char* seqAndKeySign() { return "seqAndKeySign"; }
static const char* fromPeerid() { return "fromPeerid"; }
static const char* peerInfo() { return "peerInfo"; }
static const char* sendTime() { return "sendTime"; }
static const char* toPeerid() { return "toPeerid"; }
static const char* proxyPeerid() { return "proxyPeerid"; }
static const char* proxyPeerInfo() { return "proxyPeerInfo"; }
static const char* fromContainerId() { return "fromContainerId"; }
static const char* aesKeyHash() { return "aesKeyHash"; }
static const char* toContainerId() { return "toContainerId"; }
static const char* result() { return "result"; }
static const char* mtu() { return "mtu"; }
static const char* packageId() { return "packageId"; }
static const char* recvData() { return "recvData"; }
static const char* ackPackageId() { return "ackPackageId"; }
static const char* snPeerid() { return "snPeerid"; }
static const char* reverseEndpointArray() { return "reverseEndpointArray"; }
static const char* payload() { return "payload"; }
static const char* toPeerInfo() { return "toPeerInfo"; }
static const char* endpointArray() { return "endpointArray"; }
static const char* toVPort() { return "toVPort"; }
static const char* fromSessionId() { return "fromSessionId"; }
static const char* toSessionId() { return "toSessionId"; }
};

inline const char* ExchangeKey::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::seq:
			return FieldNames::seq();
		case Flags::seqAndKeySign:
			return FieldNames::seqAndKeySign();
		case Flags::fromPeerid:
			return FieldNames::fromPeerid();
		case Flags::peerInfo:
			return FieldNames::peerInfo();
		case Flags::sendTime:
			return FieldNames::sendTime();
		default:
			assert(0);
			return NULL;
	}
}
inline const char* SynTunnel::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::fromPeerid:
			return FieldNames::fromPeerid();
		case Flags::toPeerid:
			return FieldNames::toPeerid();
		case Flags::proxyPeerid:
			return FieldNames::proxyPeerid();
		case Flags::proxyPeerInfo:
			return FieldNames::proxyPeerInfo();
		case Flags::seq:
			return FieldNames::seq();
		case Flags::fromContainerId:
			return FieldNames::fromContainerId();
		case Flags::peerInfo:
			return FieldNames::peerInfo();
		case Flags::sendTime:
			return FieldNames::sendTime();
		default:
			assert(0);
			return NULL;
	}
}
inline const char* AckTunnel::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::aesKeyHash:
			return FieldNames::aesKeyHash();
		case Flags::seq:
			return FieldNames::seq();
		case Flags::fromContainerId:
			return FieldNames::fromContainerId();
		case Flags::toContainerId:
			return FieldNames::toContainerId();
		case Flags::result:
			return FieldNames::result();
		case Flags::peerInfo:
			return FieldNames::peerInfo();
		case Flags::sendTime:
			return FieldNames::sendTime();
		case Flags::mtu:
			return FieldNames::mtu();
		default:
			assert(0);
			return NULL;
	}
}
inline const char* AckAckTunnel::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::seq:
			return FieldNames::seq();
		case Flags::toContainerId:
			return FieldNames::toContainerId();
		default:
			assert(0);
			return NULL;
	}
}
inline const char* PingTunnel::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::toContainerId:
			return FieldNames::toContainerId();
		case Flags::packageId:
			return FieldNames::packageId();
		case Flags::sendTime:
			return FieldNames::sendTime();
		case Flags::recvData:
			return FieldNames::recvData();
		default:
			assert(0);
			return NULL;
	}
}
inline const char* PingTunnelResp::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::toContainerId:
			return FieldNames::toContainerId();
		case Flags::ackPackageId:
			return FieldNames::ackPackageId();
		case Flags::sendTime:
			return FieldNames::sendTime();
		case Flags::recvData:
			return FieldNames::recvData();
		default:
			assert(0);
			return NULL;
	}
}
inline const char* SnCall::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::seq:
			return FieldNames::seq();
		case Flags::snPeerid:
			return FieldNames::snPeerid();
		case Flags::toPeerid:
			return FieldNames::toPeerid();
		case Flags::fromPeerid:
			return FieldNames::fromPeerid();
		case Flags::reverseEndpointArray:
			return FieldNames::reverseEndpointArray();
		case Flags::peerInfo:
			return FieldNames::peerInfo();
		case Flags::payload:
			return FieldNames::payload();
		default:
			assert(0);
			return NULL;
	}
}
inline const char* SnCallResp::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::seq:
			return FieldNames::seq();
		case Flags::snPeerid:
			return FieldNames::snPeerid();
		case Flags::result:
			return FieldNames::result();
		case Flags::toPeerInfo:
			return FieldNames::toPeerInfo();
		default:
			assert(0);
			return NULL;
	}
}
inline const char* SnCalled::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::seq:
			return FieldNames::seq();
		case Flags::snPeerid:
			return FieldNames::snPeerid();
		case Flags::toPeerid:
			return FieldNames::toPeerid();
		case Flags::fromPeerid:
			return FieldNames::fromPeerid();
		case Flags::reverseEndpointArray:
			return FieldNames::reverseEndpointArray();
		case Flags::peerInfo:
			return FieldNames::peerInfo();
		case Flags::payload:
			return FieldNames::payload();
		default:
			assert(0);
			return NULL;
	}
}
inline const char* SnCalledResp::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::seq:
			return FieldNames::seq();
		case Flags::snPeerid:
			return FieldNames::snPeerid();
		case Flags::result:
			return FieldNames::result();
		default:
			assert(0);
			return NULL;
	}
}
inline const char* SnPing::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::seq:
			return FieldNames::seq();
		case Flags::snPeerid:
			return FieldNames::snPeerid();
		case Flags::fromPeerid:
			return FieldNames::fromPeerid();
		case Flags::peerInfo:
			return FieldNames::peerInfo();
		default:
			assert(0);
			return NULL;
	}
}
inline const char* SnPingResp::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::seq:
			return FieldNames::seq();
		case Flags::snPeerid:
			return FieldNames::snPeerid();
		case Flags::result:
			return FieldNames::result();
		case Flags::peerInfo:
			return FieldNames::peerInfo();
		case Flags::endpointArray:
			return FieldNames::endpointArray();
		default:
			assert(0);
			return NULL;
	}
}
inline const char* TcpSynConnection::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::seq:
			return FieldNames::seq();
		case Flags::result:
			return FieldNames::result();
		case Flags::toVPort:
			return FieldNames::toVPort();
		case Flags::fromSessionId:
			return FieldNames::fromSessionId();
		case Flags::fromPeerid:
			return FieldNames::fromPeerid();
		case Flags::toPeerid:
			return FieldNames::toPeerid();
		case Flags::proxyPeerid:
			return FieldNames::proxyPeerid();
		case Flags::peerInfo:
			return FieldNames::peerInfo();
		case Flags::reverseEndpointArray:
			return FieldNames::reverseEndpointArray();
		case Flags::payload:
			return FieldNames::payload();
		default:
			assert(0);
			return NULL;
	}
}
inline const char* TcpAckConnection::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::seq:
			return FieldNames::seq();
		case Flags::toSessionId:
			return FieldNames::toSessionId();
		case Flags::result:
			return FieldNames::result();
		case Flags::peerInfo:
			return FieldNames::peerInfo();
		case Flags::payload:
			return FieldNames::payload();
		default:
			assert(0);
			return NULL;
	}
}
inline const char* TcpAckAckConnection::fieldName(uint16_t fieldFlags) {
	switch (fieldFlags) {
		case Flags::seq:
			return FieldNames::seq();
		case Flags::result:
			return FieldNames::result();
		default:
			assert(0);
			return NULL;
	}
}

int ExchangeKey::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::seq()) {
		(*(uint32_t*)pFieldValue) = this->seq;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::seqAndKeySign()) {
		memcpy(pFieldValue,this->seqAndKeySign,crypto::signatureBytes);
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::fromPeerid()) {
		(*(peer::Id*)pFieldValue) = this->fromPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::peerInfo()) {
		(*(std::shared_ptr<peer::Info>*)pFieldValue) = this->peerInfo;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::sendTime()) {
		(*(uint64_t*)pFieldValue) = this->sendTime;
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}
int SynTunnel::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::fromPeerid()) {
		(*(peer::Id*)pFieldValue) = this->fromPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::toPeerid()) {
		(*(peer::Id*)pFieldValue) = this->toPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::proxyPeerid()) {
		(*(peer::Id*)pFieldValue) = this->proxyPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::proxyPeerInfo()) {
		(*(std::shared_ptr<peer::Info>*)pFieldValue) = this->proxyPeerInfo;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::seq()) {
		(*(uint32_t*)pFieldValue) = this->seq;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::fromContainerId()) {
		(*(uint32_t*)pFieldValue) = this->fromContainerId;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::peerInfo()) {
		(*(std::shared_ptr<peer::Info>*)pFieldValue) = this->peerInfo;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::sendTime()) {
		(*(uint64_t*)pFieldValue) = this->sendTime;
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}
int AckTunnel::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::aesKeyHash()) {
		(*(uint64_t*)pFieldValue) = this->aesKeyHash;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::seq()) {
		(*(uint32_t*)pFieldValue) = this->seq;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::fromContainerId()) {
		(*(uint32_t*)pFieldValue) = this->fromContainerId;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::toContainerId()) {
		(*(uint32_t*)pFieldValue) = this->toContainerId;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::result()) {
		(*(uint8_t*)pFieldValue) = this->result;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::peerInfo()) {
		(*(std::shared_ptr<peer::Info>*)pFieldValue) = this->peerInfo;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::sendTime()) {
		(*(uint64_t*)pFieldValue) = this->sendTime;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::mtu()) {
		(*(uint16_t*)pFieldValue) = this->mtu;
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}
int AckAckTunnel::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::seq()) {
		(*(uint32_t*)pFieldValue) = this->seq;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::toContainerId()) {
		(*(uint32_t*)pFieldValue) = this->toContainerId;
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}
int PingTunnel::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::toContainerId()) {
		(*(uint32_t*)pFieldValue) = this->toContainerId;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::packageId()) {
		(*(uint32_t*)pFieldValue) = this->packageId;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::sendTime()) {
		(*(uint64_t*)pFieldValue) = this->sendTime;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::recvData()) {
		(*(uint64_t*)pFieldValue) = this->recvData;
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}
int PingTunnelResp::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::toContainerId()) {
		(*(uint32_t*)pFieldValue) = this->toContainerId;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::ackPackageId()) {
		(*(uint32_t*)pFieldValue) = this->ackPackageId;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::sendTime()) {
		(*(uint64_t*)pFieldValue) = this->sendTime;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::recvData()) {
		(*(uint64_t*)pFieldValue) = this->recvData;
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}
int SnCall::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::seq()) {
		(*(uint32_t*)pFieldValue) = this->seq;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::snPeerid()) {
		(*(peer::Id*)pFieldValue) = this->snPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::toPeerid()) {
		(*(peer::Id*)pFieldValue) = this->toPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::fromPeerid()) {
		(*(peer::Id*)pFieldValue) = this->fromPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::reverseEndpointArray()) {
		*((std::vector<endpoint::Endpoint>*)pFieldValue) = this->reverseEndpointArray;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::peerInfo()) {
		(*(std::shared_ptr<peer::Info>*)pFieldValue) = this->peerInfo;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::payload()) {
		assert(0);
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}
int SnCallResp::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::seq()) {
		(*(uint32_t*)pFieldValue) = this->seq;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::snPeerid()) {
		(*(peer::Id*)pFieldValue) = this->snPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::result()) {
		(*(uint8_t*)pFieldValue) = this->result;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::toPeerInfo()) {
		(*(std::shared_ptr<peer::Info>*)pFieldValue) = this->toPeerInfo;
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}
int SnCalled::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::seq()) {
		(*(uint32_t*)pFieldValue) = this->seq;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::snPeerid()) {
		(*(peer::Id*)pFieldValue) = this->snPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::toPeerid()) {
		(*(peer::Id*)pFieldValue) = this->toPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::fromPeerid()) {
		(*(peer::Id*)pFieldValue) = this->fromPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::reverseEndpointArray()) {
		*((std::vector<endpoint::Endpoint>*)pFieldValue) = this->reverseEndpointArray;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::peerInfo()) {
		(*(std::shared_ptr<peer::Info>*)pFieldValue) = this->peerInfo;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::payload()) {
		assert(0);
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}
int SnCalledResp::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::seq()) {
		(*(uint32_t*)pFieldValue) = this->seq;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::snPeerid()) {
		(*(peer::Id*)pFieldValue) = this->snPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::result()) {
		(*(uint8_t*)pFieldValue) = this->result;
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}
int SnPing::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::seq()) {
		(*(uint32_t*)pFieldValue) = this->seq;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::snPeerid()) {
		(*(peer::Id*)pFieldValue) = this->snPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::fromPeerid()) {
		(*(peer::Id*)pFieldValue) = this->fromPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::peerInfo()) {
		(*(std::shared_ptr<peer::Info>*)pFieldValue) = this->peerInfo;
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}
int SnPingResp::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::seq()) {
		(*(uint32_t*)pFieldValue) = this->seq;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::snPeerid()) {
		(*(peer::Id*)pFieldValue) = this->snPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::result()) {
		(*(uint8_t*)pFieldValue) = this->result;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::peerInfo()) {
		(*(std::shared_ptr<peer::Info>*)pFieldValue) = this->peerInfo;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::endpointArray()) {
		*((std::vector<endpoint::Endpoint>*)pFieldValue) = this->endpointArray;
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}
int TcpSynConnection::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::seq()) {
		(*(uint32_t*)pFieldValue) = this->seq;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::result()) {
		(*(uint8_t*)pFieldValue) = this->result;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::toVPort()) {
		(*(uint16_t*)pFieldValue) = this->toVPort;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::fromSessionId()) {
		(*(uint32_t*)pFieldValue) = this->fromSessionId;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::fromPeerid()) {
		(*(peer::Id*)pFieldValue) = this->fromPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::toPeerid()) {
		(*(peer::Id*)pFieldValue) = this->toPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::proxyPeerid()) {
		(*(peer::Id*)pFieldValue) = this->proxyPeerid;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::peerInfo()) {
		(*(std::shared_ptr<peer::Info>*)pFieldValue) = this->peerInfo;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::reverseEndpointArray()) {
		*((std::vector<endpoint::Endpoint>*)pFieldValue) = this->reverseEndpointArray;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::payload()) {
		assert(0);
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}
int TcpAckConnection::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::seq()) {
		(*(uint32_t*)pFieldValue) = this->seq;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::toSessionId()) {
		(*(uint32_t*)pFieldValue) = this->toSessionId;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::result()) {
		(*(uint8_t*)pFieldValue) = this->result;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::peerInfo()) {
		(*(std::shared_ptr<peer::Info>*)pFieldValue) = this->peerInfo;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::payload()) {
		assert(0);
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}
int TcpAckAckConnection::copyFieldValue(const char* fieldName, void* pFieldValue) const {
	if (fieldName == FieldNames::seq()) {
		(*(uint32_t*)pFieldValue) = this->seq;
		return ICN_RESULT_SUCCESS;
	}
	if (fieldName == FieldNames::result()) {
		(*(uint8_t*)pFieldValue) = this->result;
		return ICN_RESULT_SUCCESS;
	}
	assert(0);
	return ICN_RESULT_NOT_FOUND;
}

int SynTunnel::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
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
		const char* fieldName = SynTunnel::fieldName(Flags::fromPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->fromPeerid.compare(rParam) ==0);
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
		if(this->isDefaultField(Flags::fromPeerid)) {
			cmdflags &= (~Flags::fromPeerid);
		}
		else {
			cmdflags |= (Flags::fromPeerid);
		}
	}

	if (cmdflags & Flags::fromPeerid) {
		//write to buffer
		r = this->fromPeerid.write(stream, &writeBytes);

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
		const char* fieldName = SynTunnel::fieldName(Flags::toPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->toPeerid.compare(rParam) ==0);
				if (isEqual) {
					cmdflags &= (~Flags::toPeerid);
				}
				else {
					cmdflags |= (Flags::toPeerid);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::toPeerid)) {
			cmdflags &= (~Flags::toPeerid);
		}
		else {
			cmdflags |= (Flags::toPeerid);
		}
	}

	if (cmdflags & Flags::toPeerid) {
		//write to buffer
		r = this->toPeerid.write(stream, &writeBytes);

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
		const char* fieldName = SynTunnel::fieldName(Flags::proxyPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->proxyPeerid.compare(rParam) ==0);
				if (isEqual) {
					cmdflags &= (~Flags::proxyPeerid);
				}
				else {
					cmdflags |= (Flags::proxyPeerid);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::proxyPeerid)) {
			cmdflags &= (~Flags::proxyPeerid);
		}
		else {
			cmdflags |= (Flags::proxyPeerid);
		}
	}

	if (cmdflags & Flags::proxyPeerid) {
		//write to buffer
		r = this->proxyPeerid.write(stream, &writeBytes);

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
		const char* fieldName = SynTunnel::fieldName(Flags::proxyPeerInfo);
		if (fieldName) {	
			std::shared_ptr<peer::Info>* rParam = (std::shared_ptr<peer::Info>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->proxyPeerInfo.get() == (*rParam).get());
				if (isEqual) {
					cmdflags &= (~Flags::proxyPeerInfo);
				}
				else {
					cmdflags |= (Flags::proxyPeerInfo);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::proxyPeerInfo)) {
			cmdflags &= (~Flags::proxyPeerInfo);
		}
		else {
			cmdflags |= (Flags::proxyPeerInfo);
		}
	}

	if (cmdflags & Flags::proxyPeerInfo) {
		//write to buffer
		r = peer::Info::write(stream, (this->proxyPeerInfo),&writeBytes);

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
		const char* fieldName = SynTunnel::fieldName(Flags::seq);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::seq)) {
			cmdflags &= (~Flags::seq);
		}
		else {
			cmdflags |= (Flags::seq);
		}
	}

	if (cmdflags & Flags::seq) {
		//write to buffer
		r = stream.writeUInt32((this->seq),&writeBytes);

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
		const char* fieldName = SynTunnel::fieldName(Flags::fromContainerId);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->fromContainerId == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::fromContainerId);
				}
				else {
					cmdflags |= (Flags::fromContainerId);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::fromContainerId)) {
			cmdflags &= (~Flags::fromContainerId);
		}
		else {
			cmdflags |= (Flags::fromContainerId);
		}
	}

	if (cmdflags & Flags::fromContainerId) {
		//write to buffer
		r = stream.writeUInt32((this->fromContainerId),&writeBytes);

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
		const char* fieldName = SynTunnel::fieldName(Flags::peerInfo);
		if (fieldName) {	
			std::shared_ptr<peer::Info>* rParam = (std::shared_ptr<peer::Info>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->peerInfo.get() == (*rParam).get());
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
		if(this->isDefaultField(Flags::peerInfo)) {
			cmdflags &= (~Flags::peerInfo);
		}
		else {
			cmdflags |= (Flags::peerInfo);
		}
	}

	if (cmdflags & Flags::peerInfo) {
		//write to buffer
		r = peer::Info::write(stream, (this->peerInfo),&writeBytes);

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
		const char* fieldName = SynTunnel::fieldName(Flags::sendTime);
		if (fieldName) {	
			uint64_t* rParam = (uint64_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::sendTime)) {
			cmdflags &= (~Flags::sendTime);
		}
		else {
			cmdflags |= (Flags::sendTime);
		}
	}

	if (cmdflags & Flags::sendTime) {
		//write to buffer
		r = stream.writeUInt64((this->sendTime),&writeBytes);

		if (r != ICN_RESULT_SUCCESS) {
			return r;
		}
		totalWriteBytes += writeBytes;
		writeBytes = 0;
	}
	//-------------------------------------------------------------
	
	size_t tailPos = stream.pos();
	stream.setPos(headerPos);
	stream.writeUInt16((uint16_t)totalWriteBytes,&writeBytes);
	stream.writeUInt8(this->cmdtype,&writeBytes);
	stream.writeUInt16(cmdflags,&writeBytes);
	stream.setPos(tailPos);
	*pWriteBytes = totalWriteBytes;
	
	return ICN_RESULT_SUCCESS;
}	

		
int AckTunnel::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
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
		const char* fieldName = AckTunnel::fieldName(Flags::aesKeyHash);
		if (fieldName) {	
			uint64_t* rParam = (uint64_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->aesKeyHash == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::aesKeyHash);
				}
				else {
					cmdflags |= (Flags::aesKeyHash);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::aesKeyHash)) {
			cmdflags &= (~Flags::aesKeyHash);
		}
		else {
			cmdflags |= (Flags::aesKeyHash);
		}
	}

	if (cmdflags & Flags::aesKeyHash) {
		//write to buffer
		r = stream.writeUInt64((this->aesKeyHash),&writeBytes);

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
		const char* fieldName = AckTunnel::fieldName(Flags::seq);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::seq)) {
			cmdflags &= (~Flags::seq);
		}
		else {
			cmdflags |= (Flags::seq);
		}
	}

	if (cmdflags & Flags::seq) {
		//write to buffer
		r = stream.writeUInt32((this->seq),&writeBytes);

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
		const char* fieldName = AckTunnel::fieldName(Flags::fromContainerId);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->fromContainerId == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::fromContainerId);
				}
				else {
					cmdflags |= (Flags::fromContainerId);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::fromContainerId)) {
			cmdflags &= (~Flags::fromContainerId);
		}
		else {
			cmdflags |= (Flags::fromContainerId);
		}
	}

	if (cmdflags & Flags::fromContainerId) {
		//write to buffer
		r = stream.writeUInt32((this->fromContainerId),&writeBytes);

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
		const char* fieldName = AckTunnel::fieldName(Flags::toContainerId);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->toContainerId == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::toContainerId);
				}
				else {
					cmdflags |= (Flags::toContainerId);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::toContainerId)) {
			cmdflags &= (~Flags::toContainerId);
		}
		else {
			cmdflags |= (Flags::toContainerId);
		}
	}

	if (cmdflags & Flags::toContainerId) {
		//write to buffer
		r = stream.writeUInt32((this->toContainerId),&writeBytes);

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
		const char* fieldName = AckTunnel::fieldName(Flags::result);
		if (fieldName) {	
			uint8_t* rParam = (uint8_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->result == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::result);
				}
				else {
					cmdflags |= (Flags::result);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::result)) {
			cmdflags &= (~Flags::result);
		}
		else {
			cmdflags |= (Flags::result);
		}
	}

	if (cmdflags & Flags::result) {
		//write to buffer
		r = stream.writeUInt8((this->result),&writeBytes);

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
		const char* fieldName = AckTunnel::fieldName(Flags::peerInfo);
		if (fieldName) {	
			std::shared_ptr<peer::Info>* rParam = (std::shared_ptr<peer::Info>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->peerInfo.get() == (*rParam).get());
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
		if(this->isDefaultField(Flags::peerInfo)) {
			cmdflags &= (~Flags::peerInfo);
		}
		else {
			cmdflags |= (Flags::peerInfo);
		}
	}

	if (cmdflags & Flags::peerInfo) {
		//write to buffer
		r = peer::Info::write(stream, (this->peerInfo),&writeBytes);

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
		const char* fieldName = AckTunnel::fieldName(Flags::sendTime);
		if (fieldName) {	
			uint64_t* rParam = (uint64_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::sendTime)) {
			cmdflags &= (~Flags::sendTime);
		}
		else {
			cmdflags |= (Flags::sendTime);
		}
	}

	if (cmdflags & Flags::sendTime) {
		//write to buffer
		r = stream.writeUInt64((this->sendTime),&writeBytes);

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
		const char* fieldName = AckTunnel::fieldName(Flags::mtu);
		if (fieldName) {	
			uint16_t* rParam = (uint16_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->mtu == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::mtu);
				}
				else {
					cmdflags |= (Flags::mtu);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::mtu)) {
			cmdflags &= (~Flags::mtu);
		}
		else {
			cmdflags |= (Flags::mtu);
		}
	}

	if (cmdflags & Flags::mtu) {
		//write to buffer
		r = stream.writeUInt16((this->mtu),&writeBytes);

		if (r != ICN_RESULT_SUCCESS) {
			return r;
		}
		totalWriteBytes += writeBytes;
		writeBytes = 0;
	}
	//-------------------------------------------------------------
	
	size_t tailPos = stream.pos();
	stream.setPos(headerPos);
	stream.writeUInt16((uint16_t)totalWriteBytes,&writeBytes);
	stream.writeUInt8(this->cmdtype,&writeBytes);
	stream.writeUInt16(cmdflags,&writeBytes);
	stream.setPos(tailPos);
	*pWriteBytes = totalWriteBytes;
	
	return ICN_RESULT_SUCCESS;
}	

		
int AckAckTunnel::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
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
		const char* fieldName = AckAckTunnel::fieldName(Flags::seq);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::seq)) {
			cmdflags &= (~Flags::seq);
		}
		else {
			cmdflags |= (Flags::seq);
		}
	}

	if (cmdflags & Flags::seq) {
		//write to buffer
		r = stream.writeUInt32((this->seq),&writeBytes);

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
		const char* fieldName = AckAckTunnel::fieldName(Flags::toContainerId);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->toContainerId == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::toContainerId);
				}
				else {
					cmdflags |= (Flags::toContainerId);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::toContainerId)) {
			cmdflags &= (~Flags::toContainerId);
		}
		else {
			cmdflags |= (Flags::toContainerId);
		}
	}

	if (cmdflags & Flags::toContainerId) {
		//write to buffer
		r = stream.writeUInt32((this->toContainerId),&writeBytes);

		if (r != ICN_RESULT_SUCCESS) {
			return r;
		}
		totalWriteBytes += writeBytes;
		writeBytes = 0;
	}
	//-------------------------------------------------------------
	
	size_t tailPos = stream.pos();
	stream.setPos(headerPos);
	stream.writeUInt16((uint16_t)totalWriteBytes,&writeBytes);
	stream.writeUInt8(this->cmdtype,&writeBytes);
	stream.writeUInt16(cmdflags,&writeBytes);
	stream.setPos(tailPos);
	*pWriteBytes = totalWriteBytes;
	
	return ICN_RESULT_SUCCESS;
}	

		
int PingTunnel::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
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
		const char* fieldName = PingTunnel::fieldName(Flags::toContainerId);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->toContainerId == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::toContainerId);
				}
				else {
					cmdflags |= (Flags::toContainerId);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::toContainerId)) {
			cmdflags &= (~Flags::toContainerId);
		}
		else {
			cmdflags |= (Flags::toContainerId);
		}
	}

	if (cmdflags & Flags::toContainerId) {
		//write to buffer
		r = stream.writeUInt32((this->toContainerId),&writeBytes);

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
		const char* fieldName = PingTunnel::fieldName(Flags::packageId);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->packageId == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::packageId);
				}
				else {
					cmdflags |= (Flags::packageId);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::packageId)) {
			cmdflags &= (~Flags::packageId);
		}
		else {
			cmdflags |= (Flags::packageId);
		}
	}

	if (cmdflags & Flags::packageId) {
		//write to buffer
		r = stream.writeUInt32((this->packageId),&writeBytes);

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
		const char* fieldName = PingTunnel::fieldName(Flags::sendTime);
		if (fieldName) {	
			uint64_t* rParam = (uint64_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::sendTime)) {
			cmdflags &= (~Flags::sendTime);
		}
		else {
			cmdflags |= (Flags::sendTime);
		}
	}

	if (cmdflags & Flags::sendTime) {
		//write to buffer
		r = stream.writeUInt64((this->sendTime),&writeBytes);

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
		const char* fieldName = PingTunnel::fieldName(Flags::recvData);
		if (fieldName) {	
			uint64_t* rParam = (uint64_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->recvData == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::recvData);
				}
				else {
					cmdflags |= (Flags::recvData);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::recvData)) {
			cmdflags &= (~Flags::recvData);
		}
		else {
			cmdflags |= (Flags::recvData);
		}
	}

	if (cmdflags & Flags::recvData) {
		//write to buffer
		r = stream.writeUInt64((this->recvData),&writeBytes);

		if (r != ICN_RESULT_SUCCESS) {
			return r;
		}
		totalWriteBytes += writeBytes;
		writeBytes = 0;
	}
	//-------------------------------------------------------------
	
	size_t tailPos = stream.pos();
	stream.setPos(headerPos);
	stream.writeUInt16((uint16_t)totalWriteBytes,&writeBytes);
	stream.writeUInt8(this->cmdtype,&writeBytes);
	stream.writeUInt16(cmdflags,&writeBytes);
	stream.setPos(tailPos);
	*pWriteBytes = totalWriteBytes;
	
	return ICN_RESULT_SUCCESS;
}	

		
int PingTunnelResp::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
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
		const char* fieldName = PingTunnelResp::fieldName(Flags::toContainerId);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->toContainerId == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::toContainerId);
				}
				else {
					cmdflags |= (Flags::toContainerId);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::toContainerId)) {
			cmdflags &= (~Flags::toContainerId);
		}
		else {
			cmdflags |= (Flags::toContainerId);
		}
	}

	if (cmdflags & Flags::toContainerId) {
		//write to buffer
		r = stream.writeUInt32((this->toContainerId),&writeBytes);

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
		const char* fieldName = PingTunnelResp::fieldName(Flags::ackPackageId);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->ackPackageId == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::ackPackageId);
				}
				else {
					cmdflags |= (Flags::ackPackageId);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::ackPackageId)) {
			cmdflags &= (~Flags::ackPackageId);
		}
		else {
			cmdflags |= (Flags::ackPackageId);
		}
	}

	if (cmdflags & Flags::ackPackageId) {
		//write to buffer
		r = stream.writeUInt32((this->ackPackageId),&writeBytes);

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
		const char* fieldName = PingTunnelResp::fieldName(Flags::sendTime);
		if (fieldName) {	
			uint64_t* rParam = (uint64_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::sendTime)) {
			cmdflags &= (~Flags::sendTime);
		}
		else {
			cmdflags |= (Flags::sendTime);
		}
	}

	if (cmdflags & Flags::sendTime) {
		//write to buffer
		r = stream.writeUInt64((this->sendTime),&writeBytes);

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
		const char* fieldName = PingTunnelResp::fieldName(Flags::recvData);
		if (fieldName) {	
			uint64_t* rParam = (uint64_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->recvData == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::recvData);
				}
				else {
					cmdflags |= (Flags::recvData);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::recvData)) {
			cmdflags &= (~Flags::recvData);
		}
		else {
			cmdflags |= (Flags::recvData);
		}
	}

	if (cmdflags & Flags::recvData) {
		//write to buffer
		r = stream.writeUInt64((this->recvData),&writeBytes);

		if (r != ICN_RESULT_SUCCESS) {
			return r;
		}
		totalWriteBytes += writeBytes;
		writeBytes = 0;
	}
	//-------------------------------------------------------------
	
	size_t tailPos = stream.pos();
	stream.setPos(headerPos);
	stream.writeUInt16((uint16_t)totalWriteBytes,&writeBytes);
	stream.writeUInt8(this->cmdtype,&writeBytes);
	stream.writeUInt16(cmdflags,&writeBytes);
	stream.setPos(tailPos);
	*pWriteBytes = totalWriteBytes;
	
	return ICN_RESULT_SUCCESS;
}	

		
int SnCall::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
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
		const char* fieldName = SnCall::fieldName(Flags::seq);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::seq)) {
			cmdflags &= (~Flags::seq);
		}
		else {
			cmdflags |= (Flags::seq);
		}
	}

	if (cmdflags & Flags::seq) {
		//write to buffer
		r = stream.writeUInt32((this->seq),&writeBytes);

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
		const char* fieldName = SnCall::fieldName(Flags::snPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->snPeerid.compare(rParam) ==0);
				if (isEqual) {
					cmdflags &= (~Flags::snPeerid);
				}
				else {
					cmdflags |= (Flags::snPeerid);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::snPeerid)) {
			cmdflags &= (~Flags::snPeerid);
		}
		else {
			cmdflags |= (Flags::snPeerid);
		}
	}

	if (cmdflags & Flags::snPeerid) {
		//write to buffer
		r = this->snPeerid.write(stream, &writeBytes);

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
		const char* fieldName = SnCall::fieldName(Flags::toPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->toPeerid.compare(rParam) ==0);
				if (isEqual) {
					cmdflags &= (~Flags::toPeerid);
				}
				else {
					cmdflags |= (Flags::toPeerid);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::toPeerid)) {
			cmdflags &= (~Flags::toPeerid);
		}
		else {
			cmdflags |= (Flags::toPeerid);
		}
	}

	if (cmdflags & Flags::toPeerid) {
		//write to buffer
		r = this->toPeerid.write(stream, &writeBytes);

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
		const char* fieldName = SnCall::fieldName(Flags::fromPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->fromPeerid.compare(rParam) ==0);
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
		if(this->isDefaultField(Flags::fromPeerid)) {
			cmdflags &= (~Flags::fromPeerid);
		}
		else {
			cmdflags |= (Flags::fromPeerid);
		}
	}

	if (cmdflags & Flags::fromPeerid) {
		//write to buffer
		r = this->fromPeerid.write(stream, &writeBytes);

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
		const char* fieldName = SnCall::fieldName(Flags::reverseEndpointArray);
		if (fieldName) {	
			std::vector<endpoint::Endpoint>* rParam = (std::vector<endpoint::Endpoint>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = false;
				if (isEqual) {
					cmdflags &= (~Flags::reverseEndpointArray);
				}
				else {
					cmdflags |= (Flags::reverseEndpointArray);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::reverseEndpointArray)) {
			cmdflags &= (~Flags::reverseEndpointArray);
		}
		else {
			cmdflags |= (Flags::reverseEndpointArray);
		}
	}

	if (cmdflags & Flags::reverseEndpointArray) {
		//write to buffer
		r = writeEndpointArray(stream, this->reverseEndpointArray,&writeBytes);

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
		const char* fieldName = SnCall::fieldName(Flags::peerInfo);
		if (fieldName) {	
			std::shared_ptr<peer::Info>* rParam = (std::shared_ptr<peer::Info>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->peerInfo.get() == (*rParam).get());
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
		if(this->isDefaultField(Flags::peerInfo)) {
			cmdflags &= (~Flags::peerInfo);
		}
		else {
			cmdflags |= (Flags::peerInfo);
		}
	}

	if (cmdflags & Flags::peerInfo) {
		//write to buffer
		r = peer::Info::write(stream, (this->peerInfo),&writeBytes);

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
		const char* fieldName = SnCall::fieldName(Flags::payload);
		if (fieldName) {	
			std::shared_ptr<Buffer>* rParam = (std::shared_ptr<Buffer>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = false;
				if (isEqual) {
					cmdflags &= (~Flags::payload);
				}
				else {
					cmdflags |= (Flags::payload);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::payload)) {
			cmdflags &= (~Flags::payload);
		}
		else {
			cmdflags |= (Flags::payload);
		}
	}

	if (cmdflags & Flags::payload) {
		//write to buffer
		r = stream.writeBuffer((this->payload),&writeBytes);

		if (r != ICN_RESULT_SUCCESS) {
			return r;
		}
		totalWriteBytes += writeBytes;
		writeBytes = 0;
	}
	//-------------------------------------------------------------
	
	size_t tailPos = stream.pos();
	stream.setPos(headerPos);
	stream.writeUInt16((uint16_t)totalWriteBytes,&writeBytes);
	stream.writeUInt8(this->cmdtype,&writeBytes);
	stream.writeUInt16(cmdflags,&writeBytes);
	stream.setPos(tailPos);
	*pWriteBytes = totalWriteBytes;
	
	return ICN_RESULT_SUCCESS;
}	

		
int SnCallResp::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
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
		const char* fieldName = SnCallResp::fieldName(Flags::seq);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::seq)) {
			cmdflags &= (~Flags::seq);
		}
		else {
			cmdflags |= (Flags::seq);
		}
	}

	if (cmdflags & Flags::seq) {
		//write to buffer
		r = stream.writeUInt32((this->seq),&writeBytes);

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
		const char* fieldName = SnCallResp::fieldName(Flags::snPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->snPeerid.compare(rParam) ==0);
				if (isEqual) {
					cmdflags &= (~Flags::snPeerid);
				}
				else {
					cmdflags |= (Flags::snPeerid);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::snPeerid)) {
			cmdflags &= (~Flags::snPeerid);
		}
		else {
			cmdflags |= (Flags::snPeerid);
		}
	}

	if (cmdflags & Flags::snPeerid) {
		//write to buffer
		r = this->snPeerid.write(stream, &writeBytes);

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
		const char* fieldName = SnCallResp::fieldName(Flags::result);
		if (fieldName) {	
			uint8_t* rParam = (uint8_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->result == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::result);
				}
				else {
					cmdflags |= (Flags::result);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::result)) {
			cmdflags &= (~Flags::result);
		}
		else {
			cmdflags |= (Flags::result);
		}
	}

	if (cmdflags & Flags::result) {
		//write to buffer
		r = stream.writeUInt8((this->result),&writeBytes);

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
		const char* fieldName = SnCallResp::fieldName(Flags::toPeerInfo);
		if (fieldName) {	
			std::shared_ptr<peer::Info>* rParam = (std::shared_ptr<peer::Info>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->toPeerInfo.get() == (*rParam).get());
				if (isEqual) {
					cmdflags &= (~Flags::toPeerInfo);
				}
				else {
					cmdflags |= (Flags::toPeerInfo);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::toPeerInfo)) {
			cmdflags &= (~Flags::toPeerInfo);
		}
		else {
			cmdflags |= (Flags::toPeerInfo);
		}
	}

	if (cmdflags & Flags::toPeerInfo) {
		//write to buffer
		r = peer::Info::write(stream, (this->toPeerInfo),&writeBytes);

		if (r != ICN_RESULT_SUCCESS) {
			return r;
		}
		totalWriteBytes += writeBytes;
		writeBytes = 0;
	}
	//-------------------------------------------------------------
	
	size_t tailPos = stream.pos();
	stream.setPos(headerPos);
	stream.writeUInt16((uint16_t)totalWriteBytes,&writeBytes);
	stream.writeUInt8(this->cmdtype,&writeBytes);
	stream.writeUInt16(cmdflags,&writeBytes);
	stream.setPos(tailPos);
	*pWriteBytes = totalWriteBytes;
	
	return ICN_RESULT_SUCCESS;
}	

		
int SnCalled::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
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
		const char* fieldName = SnCalled::fieldName(Flags::seq);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::seq)) {
			cmdflags &= (~Flags::seq);
		}
		else {
			cmdflags |= (Flags::seq);
		}
	}

	if (cmdflags & Flags::seq) {
		//write to buffer
		r = stream.writeUInt32((this->seq),&writeBytes);

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
		const char* fieldName = SnCalled::fieldName(Flags::snPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->snPeerid.compare(rParam) ==0);
				if (isEqual) {
					cmdflags &= (~Flags::snPeerid);
				}
				else {
					cmdflags |= (Flags::snPeerid);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::snPeerid)) {
			cmdflags &= (~Flags::snPeerid);
		}
		else {
			cmdflags |= (Flags::snPeerid);
		}
	}

	if (cmdflags & Flags::snPeerid) {
		//write to buffer
		r = this->snPeerid.write(stream, &writeBytes);

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
		const char* fieldName = SnCalled::fieldName(Flags::toPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->toPeerid.compare(rParam) ==0);
				if (isEqual) {
					cmdflags &= (~Flags::toPeerid);
				}
				else {
					cmdflags |= (Flags::toPeerid);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::toPeerid)) {
			cmdflags &= (~Flags::toPeerid);
		}
		else {
			cmdflags |= (Flags::toPeerid);
		}
	}

	if (cmdflags & Flags::toPeerid) {
		//write to buffer
		r = this->toPeerid.write(stream, &writeBytes);

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
		const char* fieldName = SnCalled::fieldName(Flags::fromPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->fromPeerid.compare(rParam) ==0);
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
		if(this->isDefaultField(Flags::fromPeerid)) {
			cmdflags &= (~Flags::fromPeerid);
		}
		else {
			cmdflags |= (Flags::fromPeerid);
		}
	}

	if (cmdflags & Flags::fromPeerid) {
		//write to buffer
		r = this->fromPeerid.write(stream, &writeBytes);

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
		const char* fieldName = SnCalled::fieldName(Flags::reverseEndpointArray);
		if (fieldName) {	
			std::vector<endpoint::Endpoint>* rParam = (std::vector<endpoint::Endpoint>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = false;
				if (isEqual) {
					cmdflags &= (~Flags::reverseEndpointArray);
				}
				else {
					cmdflags |= (Flags::reverseEndpointArray);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::reverseEndpointArray)) {
			cmdflags &= (~Flags::reverseEndpointArray);
		}
		else {
			cmdflags |= (Flags::reverseEndpointArray);
		}
	}

	if (cmdflags & Flags::reverseEndpointArray) {
		//write to buffer
		r = writeEndpointArray(stream, this->reverseEndpointArray,&writeBytes);

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
		const char* fieldName = SnCalled::fieldName(Flags::peerInfo);
		if (fieldName) {	
			std::shared_ptr<peer::Info>* rParam = (std::shared_ptr<peer::Info>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->peerInfo.get() == (*rParam).get());
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
		if(this->isDefaultField(Flags::peerInfo)) {
			cmdflags &= (~Flags::peerInfo);
		}
		else {
			cmdflags |= (Flags::peerInfo);
		}
	}

	if (cmdflags & Flags::peerInfo) {
		//write to buffer
		r = peer::Info::write(stream, (this->peerInfo),&writeBytes);

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
		const char* fieldName = SnCalled::fieldName(Flags::payload);
		if (fieldName) {	
			std::shared_ptr<Buffer>* rParam = (std::shared_ptr<Buffer>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = false;
				if (isEqual) {
					cmdflags &= (~Flags::payload);
				}
				else {
					cmdflags |= (Flags::payload);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::payload)) {
			cmdflags &= (~Flags::payload);
		}
		else {
			cmdflags |= (Flags::payload);
		}
	}

	if (cmdflags & Flags::payload) {
		//write to buffer
		r = stream.writeBuffer((this->payload),&writeBytes);

		if (r != ICN_RESULT_SUCCESS) {
			return r;
		}
		totalWriteBytes += writeBytes;
		writeBytes = 0;
	}
	//-------------------------------------------------------------
	
	size_t tailPos = stream.pos();
	stream.setPos(headerPos);
	stream.writeUInt16((uint16_t)totalWriteBytes,&writeBytes);
	stream.writeUInt8(this->cmdtype,&writeBytes);
	stream.writeUInt16(cmdflags,&writeBytes);
	stream.setPos(tailPos);
	*pWriteBytes = totalWriteBytes;
	
	return ICN_RESULT_SUCCESS;
}	

		
int SnCalledResp::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
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
		const char* fieldName = SnCalledResp::fieldName(Flags::seq);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::seq)) {
			cmdflags &= (~Flags::seq);
		}
		else {
			cmdflags |= (Flags::seq);
		}
	}

	if (cmdflags & Flags::seq) {
		//write to buffer
		r = stream.writeUInt32((this->seq),&writeBytes);

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
		const char* fieldName = SnCalledResp::fieldName(Flags::snPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->snPeerid.compare(rParam) ==0);
				if (isEqual) {
					cmdflags &= (~Flags::snPeerid);
				}
				else {
					cmdflags |= (Flags::snPeerid);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::snPeerid)) {
			cmdflags &= (~Flags::snPeerid);
		}
		else {
			cmdflags |= (Flags::snPeerid);
		}
	}

	if (cmdflags & Flags::snPeerid) {
		//write to buffer
		r = this->snPeerid.write(stream, &writeBytes);

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
		const char* fieldName = SnCalledResp::fieldName(Flags::result);
		if (fieldName) {	
			uint8_t* rParam = (uint8_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->result == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::result);
				}
				else {
					cmdflags |= (Flags::result);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::result)) {
			cmdflags &= (~Flags::result);
		}
		else {
			cmdflags |= (Flags::result);
		}
	}

	if (cmdflags & Flags::result) {
		//write to buffer
		r = stream.writeUInt8((this->result),&writeBytes);

		if (r != ICN_RESULT_SUCCESS) {
			return r;
		}
		totalWriteBytes += writeBytes;
		writeBytes = 0;
	}
	//-------------------------------------------------------------
	
	size_t tailPos = stream.pos();
	stream.setPos(headerPos);
	stream.writeUInt16((uint16_t)totalWriteBytes,&writeBytes);
	stream.writeUInt8(this->cmdtype,&writeBytes);
	stream.writeUInt16(cmdflags,&writeBytes);
	stream.setPos(tailPos);
	*pWriteBytes = totalWriteBytes;
	
	return ICN_RESULT_SUCCESS;
}	

		
int SnPing::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
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
		const char* fieldName = SnPing::fieldName(Flags::seq);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::seq)) {
			cmdflags &= (~Flags::seq);
		}
		else {
			cmdflags |= (Flags::seq);
		}
	}

	if (cmdflags & Flags::seq) {
		//write to buffer
		r = stream.writeUInt32((this->seq),&writeBytes);

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
		const char* fieldName = SnPing::fieldName(Flags::snPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->snPeerid.compare(rParam) ==0);
				if (isEqual) {
					cmdflags &= (~Flags::snPeerid);
				}
				else {
					cmdflags |= (Flags::snPeerid);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::snPeerid)) {
			cmdflags &= (~Flags::snPeerid);
		}
		else {
			cmdflags |= (Flags::snPeerid);
		}
	}

	if (cmdflags & Flags::snPeerid) {
		//write to buffer
		r = this->snPeerid.write(stream, &writeBytes);

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
		const char* fieldName = SnPing::fieldName(Flags::fromPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->fromPeerid.compare(rParam) ==0);
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
		if(this->isDefaultField(Flags::fromPeerid)) {
			cmdflags &= (~Flags::fromPeerid);
		}
		else {
			cmdflags |= (Flags::fromPeerid);
		}
	}

	if (cmdflags & Flags::fromPeerid) {
		//write to buffer
		r = this->fromPeerid.write(stream, &writeBytes);

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
		const char* fieldName = SnPing::fieldName(Flags::peerInfo);
		if (fieldName) {	
			std::shared_ptr<peer::Info>* rParam = (std::shared_ptr<peer::Info>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->peerInfo.get() == (*rParam).get());
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
		if(this->isDefaultField(Flags::peerInfo)) {
			cmdflags &= (~Flags::peerInfo);
		}
		else {
			cmdflags |= (Flags::peerInfo);
		}
	}

	if (cmdflags & Flags::peerInfo) {
		//write to buffer
		r = peer::Info::write(stream, (this->peerInfo),&writeBytes);

		if (r != ICN_RESULT_SUCCESS) {
			return r;
		}
		totalWriteBytes += writeBytes;
		writeBytes = 0;
	}
	//-------------------------------------------------------------
	
	size_t tailPos = stream.pos();
	stream.setPos(headerPos);
	stream.writeUInt16((uint16_t)totalWriteBytes,&writeBytes);
	stream.writeUInt8(this->cmdtype,&writeBytes);
	stream.writeUInt16(cmdflags,&writeBytes);
	stream.setPos(tailPos);
	*pWriteBytes = totalWriteBytes;
	
	return ICN_RESULT_SUCCESS;
}	

		
int SnPingResp::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
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
		const char* fieldName = SnPingResp::fieldName(Flags::seq);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::seq)) {
			cmdflags &= (~Flags::seq);
		}
		else {
			cmdflags |= (Flags::seq);
		}
	}

	if (cmdflags & Flags::seq) {
		//write to buffer
		r = stream.writeUInt32((this->seq),&writeBytes);

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
		const char* fieldName = SnPingResp::fieldName(Flags::snPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->snPeerid.compare(rParam) ==0);
				if (isEqual) {
					cmdflags &= (~Flags::snPeerid);
				}
				else {
					cmdflags |= (Flags::snPeerid);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::snPeerid)) {
			cmdflags &= (~Flags::snPeerid);
		}
		else {
			cmdflags |= (Flags::snPeerid);
		}
	}

	if (cmdflags & Flags::snPeerid) {
		//write to buffer
		r = this->snPeerid.write(stream, &writeBytes);

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
		const char* fieldName = SnPingResp::fieldName(Flags::result);
		if (fieldName) {	
			uint8_t* rParam = (uint8_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->result == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::result);
				}
				else {
					cmdflags |= (Flags::result);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::result)) {
			cmdflags &= (~Flags::result);
		}
		else {
			cmdflags |= (Flags::result);
		}
	}

	if (cmdflags & Flags::result) {
		//write to buffer
		r = stream.writeUInt8((this->result),&writeBytes);

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
		const char* fieldName = SnPingResp::fieldName(Flags::peerInfo);
		if (fieldName) {	
			std::shared_ptr<peer::Info>* rParam = (std::shared_ptr<peer::Info>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->peerInfo.get() == (*rParam).get());
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
		if(this->isDefaultField(Flags::peerInfo)) {
			cmdflags &= (~Flags::peerInfo);
		}
		else {
			cmdflags |= (Flags::peerInfo);
		}
	}

	if (cmdflags & Flags::peerInfo) {
		//write to buffer
		r = peer::Info::write(stream, (this->peerInfo),&writeBytes);

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
		const char* fieldName = SnPingResp::fieldName(Flags::endpointArray);
		if (fieldName) {	
			std::vector<endpoint::Endpoint>* rParam = (std::vector<endpoint::Endpoint>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = false;
				if (isEqual) {
					cmdflags &= (~Flags::endpointArray);
				}
				else {
					cmdflags |= (Flags::endpointArray);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::endpointArray)) {
			cmdflags &= (~Flags::endpointArray);
		}
		else {
			cmdflags |= (Flags::endpointArray);
		}
	}

	if (cmdflags & Flags::endpointArray) {
		//write to buffer
		r = writeEndpointArray(stream, this->endpointArray,&writeBytes);

		if (r != ICN_RESULT_SUCCESS) {
			return r;
		}
		totalWriteBytes += writeBytes;
		writeBytes = 0;
	}
	//-------------------------------------------------------------
	
	size_t tailPos = stream.pos();
	stream.setPos(headerPos);
	stream.writeUInt16((uint16_t)totalWriteBytes,&writeBytes);
	stream.writeUInt8(this->cmdtype,&writeBytes);
	stream.writeUInt16(cmdflags,&writeBytes);
	stream.setPos(tailPos);
	*pWriteBytes = totalWriteBytes;
	
	return ICN_RESULT_SUCCESS;
}	

		
int TcpSynConnection::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
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
		const char* fieldName = TcpSynConnection::fieldName(Flags::seq);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::seq)) {
			cmdflags &= (~Flags::seq);
		}
		else {
			cmdflags |= (Flags::seq);
		}
	}

	if (cmdflags & Flags::seq) {
		//write to buffer
		r = stream.writeUInt32((this->seq),&writeBytes);

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
		const char* fieldName = TcpSynConnection::fieldName(Flags::result);
		if (fieldName) {	
			uint8_t* rParam = (uint8_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->result == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::result);
				}
				else {
					cmdflags |= (Flags::result);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::result)) {
			cmdflags &= (~Flags::result);
		}
		else {
			cmdflags |= (Flags::result);
		}
	}

	if (cmdflags & Flags::result) {
		//write to buffer
		r = stream.writeUInt8((this->result),&writeBytes);

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
		const char* fieldName = TcpSynConnection::fieldName(Flags::toVPort);
		if (fieldName) {	
			uint16_t* rParam = (uint16_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->toVPort == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::toVPort);
				}
				else {
					cmdflags |= (Flags::toVPort);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::toVPort)) {
			cmdflags &= (~Flags::toVPort);
		}
		else {
			cmdflags |= (Flags::toVPort);
		}
	}

	if (cmdflags & Flags::toVPort) {
		//write to buffer
		r = stream.writeUInt16((this->toVPort),&writeBytes);

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
		const char* fieldName = TcpSynConnection::fieldName(Flags::fromSessionId);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->fromSessionId == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::fromSessionId);
				}
				else {
					cmdflags |= (Flags::fromSessionId);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::fromSessionId)) {
			cmdflags &= (~Flags::fromSessionId);
		}
		else {
			cmdflags |= (Flags::fromSessionId);
		}
	}

	if (cmdflags & Flags::fromSessionId) {
		//write to buffer
		r = stream.writeUInt32((this->fromSessionId),&writeBytes);

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
		const char* fieldName = TcpSynConnection::fieldName(Flags::fromPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->fromPeerid.compare(rParam) ==0);
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
		if(this->isDefaultField(Flags::fromPeerid)) {
			cmdflags &= (~Flags::fromPeerid);
		}
		else {
			cmdflags |= (Flags::fromPeerid);
		}
	}

	if (cmdflags & Flags::fromPeerid) {
		//write to buffer
		r = this->fromPeerid.write(stream, &writeBytes);

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
		const char* fieldName = TcpSynConnection::fieldName(Flags::toPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->toPeerid.compare(rParam) ==0);
				if (isEqual) {
					cmdflags &= (~Flags::toPeerid);
				}
				else {
					cmdflags |= (Flags::toPeerid);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::toPeerid)) {
			cmdflags &= (~Flags::toPeerid);
		}
		else {
			cmdflags |= (Flags::toPeerid);
		}
	}

	if (cmdflags & Flags::toPeerid) {
		//write to buffer
		r = this->toPeerid.write(stream, &writeBytes);

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
		const char* fieldName = TcpSynConnection::fieldName(Flags::proxyPeerid);
		if (fieldName) {	
			peer::Id* rParam = (peer::Id*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->proxyPeerid.compare(rParam) ==0);
				if (isEqual) {
					cmdflags &= (~Flags::proxyPeerid);
				}
				else {
					cmdflags |= (Flags::proxyPeerid);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::proxyPeerid)) {
			cmdflags &= (~Flags::proxyPeerid);
		}
		else {
			cmdflags |= (Flags::proxyPeerid);
		}
	}

	if (cmdflags & Flags::proxyPeerid) {
		//write to buffer
		r = this->proxyPeerid.write(stream, &writeBytes);

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
		const char* fieldName = TcpSynConnection::fieldName(Flags::peerInfo);
		if (fieldName) {	
			std::shared_ptr<peer::Info>* rParam = (std::shared_ptr<peer::Info>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->peerInfo.get() == (*rParam).get());
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
		if(this->isDefaultField(Flags::peerInfo)) {
			cmdflags &= (~Flags::peerInfo);
		}
		else {
			cmdflags |= (Flags::peerInfo);
		}
	}

	if (cmdflags & Flags::peerInfo) {
		//write to buffer
		r = peer::Info::write(stream, (this->peerInfo),&writeBytes);

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
		const char* fieldName = TcpSynConnection::fieldName(Flags::reverseEndpointArray);
		if (fieldName) {	
			std::vector<endpoint::Endpoint>* rParam = (std::vector<endpoint::Endpoint>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = false;
				if (isEqual) {
					cmdflags &= (~Flags::reverseEndpointArray);
				}
				else {
					cmdflags |= (Flags::reverseEndpointArray);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::reverseEndpointArray)) {
			cmdflags &= (~Flags::reverseEndpointArray);
		}
		else {
			cmdflags |= (Flags::reverseEndpointArray);
		}
	}

	if (cmdflags & Flags::reverseEndpointArray) {
		//write to buffer
		r = writeEndpointArray(stream, this->reverseEndpointArray,&writeBytes);

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
		const char* fieldName = TcpSynConnection::fieldName(Flags::payload);
		if (fieldName) {	
			std::shared_ptr<Buffer>* rParam = (std::shared_ptr<Buffer>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = false;
				if (isEqual) {
					cmdflags &= (~Flags::payload);
				}
				else {
					cmdflags |= (Flags::payload);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::payload)) {
			cmdflags &= (~Flags::payload);
		}
		else {
			cmdflags |= (Flags::payload);
		}
	}

	if (cmdflags & Flags::payload) {
		//write to buffer
		r = stream.writeBuffer((this->payload),&writeBytes);

		if (r != ICN_RESULT_SUCCESS) {
			return r;
		}
		totalWriteBytes += writeBytes;
		writeBytes = 0;
	}
	//-------------------------------------------------------------
	
	size_t tailPos = stream.pos();
	stream.setPos(headerPos);
	stream.writeUInt16((uint16_t)totalWriteBytes,&writeBytes);
	stream.writeUInt8(this->cmdtype,&writeBytes);
	stream.writeUInt16(cmdflags,&writeBytes);
	stream.setPos(tailPos);
	*pWriteBytes = totalWriteBytes;
	
	return ICN_RESULT_SUCCESS;
}	

		
int TcpAckConnection::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
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
		const char* fieldName = TcpAckConnection::fieldName(Flags::seq);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::seq)) {
			cmdflags &= (~Flags::seq);
		}
		else {
			cmdflags |= (Flags::seq);
		}
	}

	if (cmdflags & Flags::seq) {
		//write to buffer
		r = stream.writeUInt32((this->seq),&writeBytes);

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
		const char* fieldName = TcpAckConnection::fieldName(Flags::toSessionId);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->toSessionId == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::toSessionId);
				}
				else {
					cmdflags |= (Flags::toSessionId);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::toSessionId)) {
			cmdflags &= (~Flags::toSessionId);
		}
		else {
			cmdflags |= (Flags::toSessionId);
		}
	}

	if (cmdflags & Flags::toSessionId) {
		//write to buffer
		r = stream.writeUInt32((this->toSessionId),&writeBytes);

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
		const char* fieldName = TcpAckConnection::fieldName(Flags::result);
		if (fieldName) {	
			uint8_t* rParam = (uint8_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->result == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::result);
				}
				else {
					cmdflags |= (Flags::result);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::result)) {
			cmdflags &= (~Flags::result);
		}
		else {
			cmdflags |= (Flags::result);
		}
	}

	if (cmdflags & Flags::result) {
		//write to buffer
		r = stream.writeUInt8((this->result),&writeBytes);

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
		const char* fieldName = TcpAckConnection::fieldName(Flags::peerInfo);
		if (fieldName) {	
			std::shared_ptr<peer::Info>* rParam = (std::shared_ptr<peer::Info>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->peerInfo.get() == (*rParam).get());
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
		if(this->isDefaultField(Flags::peerInfo)) {
			cmdflags &= (~Flags::peerInfo);
		}
		else {
			cmdflags |= (Flags::peerInfo);
		}
	}

	if (cmdflags & Flags::peerInfo) {
		//write to buffer
		r = peer::Info::write(stream, (this->peerInfo),&writeBytes);

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
		const char* fieldName = TcpAckConnection::fieldName(Flags::payload);
		if (fieldName) {	
			std::shared_ptr<Buffer>* rParam = (std::shared_ptr<Buffer>*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = false;
				if (isEqual) {
					cmdflags &= (~Flags::payload);
				}
				else {
					cmdflags |= (Flags::payload);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::payload)) {
			cmdflags &= (~Flags::payload);
		}
		else {
			cmdflags |= (Flags::payload);
		}
	}

	if (cmdflags & Flags::payload) {
		//write to buffer
		r = stream.writeBuffer((this->payload),&writeBytes);

		if (r != ICN_RESULT_SUCCESS) {
			return r;
		}
		totalWriteBytes += writeBytes;
		writeBytes = 0;
	}
	//-------------------------------------------------------------
	
	size_t tailPos = stream.pos();
	stream.setPos(headerPos);
	stream.writeUInt16((uint16_t)totalWriteBytes,&writeBytes);
	stream.writeUInt8(this->cmdtype,&writeBytes);
	stream.writeUInt16(cmdflags,&writeBytes);
	stream.setPos(tailPos);
	*pWriteBytes = totalWriteBytes;
	
	return ICN_RESULT_SUCCESS;
}	

		
int TcpAckAckConnection::encode(const Package* refPackage, BufferStream& stream, size_t* pWriteBytes) const {
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
		const char* fieldName = TcpAckAckConnection::fieldName(Flags::seq);
		if (fieldName) {	
			uint32_t* rParam = (uint32_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
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
		if(this->isDefaultField(Flags::seq)) {
			cmdflags &= (~Flags::seq);
		}
		else {
			cmdflags |= (Flags::seq);
		}
	}

	if (cmdflags & Flags::seq) {
		//write to buffer
		r = stream.writeUInt32((this->seq),&writeBytes);

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
		const char* fieldName = TcpAckAckConnection::fieldName(Flags::result);
		if (fieldName) {	
			uint8_t* rParam = (uint8_t*)refPackage->fieldValue(fieldName);
			if(rParam) {
				haveRef = true;
				isEqual = (this->result == *rParam);
				if (isEqual) {
					cmdflags &= (~Flags::result);
				}
				else {
					cmdflags |= (Flags::result);
				}
			}
		}
	}
	
	if (!haveRef) {
		if(this->isDefaultField(Flags::result)) {
			cmdflags &= (~Flags::result);
		}
		else {
			cmdflags |= (Flags::result);
		}
	}

	if (cmdflags & Flags::result) {
		//write to buffer
		r = stream.writeUInt8((this->result),&writeBytes);

		if (r != ICN_RESULT_SUCCESS) {
			return r;
		}
		totalWriteBytes += writeBytes;
		writeBytes = 0;
	}
	//-------------------------------------------------------------
	
	size_t tailPos = stream.pos();
	stream.setPos(headerPos);
	stream.writeUInt16((uint16_t)totalWriteBytes,&writeBytes);
	stream.writeUInt8(this->cmdtype,&writeBytes);
	stream.writeUInt16(cmdflags,&writeBytes);
	stream.setPos(tailPos);
	*pWriteBytes = totalWriteBytes;
	
	return ICN_RESULT_SUCCESS;
}	

		

const void* ExchangeKey::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::seq()) {
		return &this->seq;
	}
	if (fieldName == FieldNames::seqAndKeySign()) {
		return &this->seqAndKeySign;
	}
	if (fieldName == FieldNames::fromPeerid()) {
		return &this->fromPeerid;
	}
	if (fieldName == FieldNames::peerInfo()) {
		return &this->peerInfo;
	}
	if (fieldName == FieldNames::sendTime()) {
		return &this->sendTime;
	}
	return NULL;
}
const void* SynTunnel::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::fromPeerid()) {
		return &this->fromPeerid;
	}
	if (fieldName == FieldNames::toPeerid()) {
		return &this->toPeerid;
	}
	if (fieldName == FieldNames::proxyPeerid()) {
		return &this->proxyPeerid;
	}
	if (fieldName == FieldNames::proxyPeerInfo()) {
		return &this->proxyPeerInfo;
	}
	if (fieldName == FieldNames::seq()) {
		return &this->seq;
	}
	if (fieldName == FieldNames::fromContainerId()) {
		return &this->fromContainerId;
	}
	if (fieldName == FieldNames::peerInfo()) {
		return &this->peerInfo;
	}
	if (fieldName == FieldNames::sendTime()) {
		return &this->sendTime;
	}
	return NULL;
}
const void* AckTunnel::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::aesKeyHash()) {
		return &this->aesKeyHash;
	}
	if (fieldName == FieldNames::seq()) {
		return &this->seq;
	}
	if (fieldName == FieldNames::fromContainerId()) {
		return &this->fromContainerId;
	}
	if (fieldName == FieldNames::toContainerId()) {
		return &this->toContainerId;
	}
	if (fieldName == FieldNames::result()) {
		return &this->result;
	}
	if (fieldName == FieldNames::peerInfo()) {
		return &this->peerInfo;
	}
	if (fieldName == FieldNames::sendTime()) {
		return &this->sendTime;
	}
	if (fieldName == FieldNames::mtu()) {
		return &this->mtu;
	}
	return NULL;
}
const void* AckAckTunnel::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::seq()) {
		return &this->seq;
	}
	if (fieldName == FieldNames::toContainerId()) {
		return &this->toContainerId;
	}
	return NULL;
}
const void* PingTunnel::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::toContainerId()) {
		return &this->toContainerId;
	}
	if (fieldName == FieldNames::packageId()) {
		return &this->packageId;
	}
	if (fieldName == FieldNames::sendTime()) {
		return &this->sendTime;
	}
	if (fieldName == FieldNames::recvData()) {
		return &this->recvData;
	}
	return NULL;
}
const void* PingTunnelResp::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::toContainerId()) {
		return &this->toContainerId;
	}
	if (fieldName == FieldNames::ackPackageId()) {
		return &this->ackPackageId;
	}
	if (fieldName == FieldNames::sendTime()) {
		return &this->sendTime;
	}
	if (fieldName == FieldNames::recvData()) {
		return &this->recvData;
	}
	return NULL;
}
const void* SnCall::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::seq()) {
		return &this->seq;
	}
	if (fieldName == FieldNames::snPeerid()) {
		return &this->snPeerid;
	}
	if (fieldName == FieldNames::toPeerid()) {
		return &this->toPeerid;
	}
	if (fieldName == FieldNames::fromPeerid()) {
		return &this->fromPeerid;
	}
	if (fieldName == FieldNames::reverseEndpointArray()) {
		return &this->reverseEndpointArray;
	}
	if (fieldName == FieldNames::peerInfo()) {
		return &this->peerInfo;
	}
	if (fieldName == FieldNames::payload()) {
		return &this->payload;
	}
	return NULL;
}
const void* SnCallResp::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::seq()) {
		return &this->seq;
	}
	if (fieldName == FieldNames::snPeerid()) {
		return &this->snPeerid;
	}
	if (fieldName == FieldNames::result()) {
		return &this->result;
	}
	if (fieldName == FieldNames::toPeerInfo()) {
		return &this->toPeerInfo;
	}
	return NULL;
}
const void* SnCalled::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::seq()) {
		return &this->seq;
	}
	if (fieldName == FieldNames::snPeerid()) {
		return &this->snPeerid;
	}
	if (fieldName == FieldNames::toPeerid()) {
		return &this->toPeerid;
	}
	if (fieldName == FieldNames::fromPeerid()) {
		return &this->fromPeerid;
	}
	if (fieldName == FieldNames::reverseEndpointArray()) {
		return &this->reverseEndpointArray;
	}
	if (fieldName == FieldNames::peerInfo()) {
		return &this->peerInfo;
	}
	if (fieldName == FieldNames::payload()) {
		return &this->payload;
	}
	return NULL;
}
const void* SnCalledResp::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::seq()) {
		return &this->seq;
	}
	if (fieldName == FieldNames::snPeerid()) {
		return &this->snPeerid;
	}
	if (fieldName == FieldNames::result()) {
		return &this->result;
	}
	return NULL;
}
const void* SnPing::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::seq()) {
		return &this->seq;
	}
	if (fieldName == FieldNames::snPeerid()) {
		return &this->snPeerid;
	}
	if (fieldName == FieldNames::fromPeerid()) {
		return &this->fromPeerid;
	}
	if (fieldName == FieldNames::peerInfo()) {
		return &this->peerInfo;
	}
	return NULL;
}
const void* SnPingResp::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::seq()) {
		return &this->seq;
	}
	if (fieldName == FieldNames::snPeerid()) {
		return &this->snPeerid;
	}
	if (fieldName == FieldNames::result()) {
		return &this->result;
	}
	if (fieldName == FieldNames::peerInfo()) {
		return &this->peerInfo;
	}
	if (fieldName == FieldNames::endpointArray()) {
		return &this->endpointArray;
	}
	return NULL;
}
const void* TcpSynConnection::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::seq()) {
		return &this->seq;
	}
	if (fieldName == FieldNames::result()) {
		return &this->result;
	}
	if (fieldName == FieldNames::toVPort()) {
		return &this->toVPort;
	}
	if (fieldName == FieldNames::fromSessionId()) {
		return &this->fromSessionId;
	}
	if (fieldName == FieldNames::fromPeerid()) {
		return &this->fromPeerid;
	}
	if (fieldName == FieldNames::toPeerid()) {
		return &this->toPeerid;
	}
	if (fieldName == FieldNames::proxyPeerid()) {
		return &this->proxyPeerid;
	}
	if (fieldName == FieldNames::peerInfo()) {
		return &this->peerInfo;
	}
	if (fieldName == FieldNames::reverseEndpointArray()) {
		return &this->reverseEndpointArray;
	}
	if (fieldName == FieldNames::payload()) {
		return &this->payload;
	}
	return NULL;
}
const void* TcpAckConnection::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::seq()) {
		return &this->seq;
	}
	if (fieldName == FieldNames::toSessionId()) {
		return &this->toSessionId;
	}
	if (fieldName == FieldNames::result()) {
		return &this->result;
	}
	if (fieldName == FieldNames::peerInfo()) {
		return &this->peerInfo;
	}
	if (fieldName == FieldNames::payload()) {
		return &this->payload;
	}
	return NULL;
}
const void* TcpAckAckConnection::fieldValue(const char* fieldName) const {
	if (fieldName == FieldNames::seq()) {
		return &this->seq;
	}
	if (fieldName == FieldNames::result()) {
		return &this->result;
	}
	return NULL;
}

bool SynTunnel::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::fromPeerid:
			return (this->fromPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::toPeerid:
			return (this->toPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::proxyPeerid:
			return (this->proxyPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::proxyPeerInfo:
			return (this->proxyPeerInfo.get() == NULL);
		case Flags::seq:
			return (this->seq == 0);
		case Flags::fromContainerId:
			return (this->fromContainerId == 0);
		case Flags::peerInfo:
			return (this->peerInfo.get() == NULL);
		case Flags::sendTime:
			return (this->sendTime == 0);
		default:
			assert(0);
			return true;
	}
}

bool AckTunnel::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::aesKeyHash:
			return (this->aesKeyHash == 0);
		case Flags::seq:
			return (this->seq == 0);
		case Flags::fromContainerId:
			return (this->fromContainerId == 0);
		case Flags::toContainerId:
			return (this->toContainerId == 0);
		case Flags::result:
			return (this->result == 0);
		case Flags::peerInfo:
			return (this->peerInfo.get() == NULL);
		case Flags::sendTime:
			return (this->sendTime == 0);
		case Flags::mtu:
			return (this->mtu == 0);
		default:
			assert(0);
			return true;
	}
}

bool AckAckTunnel::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::seq:
			return (this->seq == 0);
		case Flags::toContainerId:
			return (this->toContainerId == 0);
		default:
			assert(0);
			return true;
	}
}

bool PingTunnel::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::toContainerId:
			return (this->toContainerId == 0);
		case Flags::packageId:
			return (this->packageId == 0);
		case Flags::sendTime:
			return (this->sendTime == 0);
		case Flags::recvData:
			return (this->recvData == 0);
		default:
			assert(0);
			return true;
	}
}

bool PingTunnelResp::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::toContainerId:
			return (this->toContainerId == 0);
		case Flags::ackPackageId:
			return (this->ackPackageId == 0);
		case Flags::sendTime:
			return (this->sendTime == 0);
		case Flags::recvData:
			return (this->recvData == 0);
		default:
			assert(0);
			return true;
	}
}

bool SnCall::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::seq:
			return (this->seq == 0);
		case Flags::snPeerid:
			return (this->snPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::toPeerid:
			return (this->toPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::fromPeerid:
			return (this->fromPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::reverseEndpointArray:
			return (this->reverseEndpointArray.size() == 0);
		case Flags::peerInfo:
			return (this->peerInfo.get() == NULL);
		case Flags::payload:
			return (this->payload.get() == NULL);
		default:
			assert(0);
			return true;
	}
}

bool SnCallResp::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::seq:
			return (this->seq == 0);
		case Flags::snPeerid:
			return (this->snPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::result:
			return (this->result == 0);
		case Flags::toPeerInfo:
			return (this->toPeerInfo.get() == NULL);
		default:
			assert(0);
			return true;
	}
}

bool SnCalled::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::seq:
			return (this->seq == 0);
		case Flags::snPeerid:
			return (this->snPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::toPeerid:
			return (this->toPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::fromPeerid:
			return (this->fromPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::reverseEndpointArray:
			return (this->reverseEndpointArray.size() == 0);
		case Flags::peerInfo:
			return (this->peerInfo.get() == NULL);
		case Flags::payload:
			return (this->payload.get() == NULL);
		default:
			assert(0);
			return true;
	}
}

bool SnCalledResp::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::seq:
			return (this->seq == 0);
		case Flags::snPeerid:
			return (this->snPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::result:
			return (this->result == 0);
		default:
			assert(0);
			return true;
	}
}

bool SnPing::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::seq:
			return (this->seq == 0);
		case Flags::snPeerid:
			return (this->snPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::fromPeerid:
			return (this->fromPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::peerInfo:
			return (this->peerInfo.get() == NULL);
		default:
			assert(0);
			return true;
	}
}

bool SnPingResp::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::seq:
			return (this->seq == 0);
		case Flags::snPeerid:
			return (this->snPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::result:
			return (this->result == 0);
		case Flags::peerInfo:
			return (this->peerInfo.get() == NULL);
		case Flags::endpointArray:
			return (this->endpointArray.size() == 0);
		default:
			assert(0);
			return true;
	}
}

bool Datagram::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::seq:
			return (this->seq == 0);
		case Flags::destZone:
			return (this->destZone == 0);
		case Flags::hopLimit:
			return (this->hopLimit == 0);
		case Flags::piece:
			return (this->piece == 0);
		case Flags::ackSeq:
			return (this->ackSeq == 0);
		case Flags::sendTime:
			return (this->sendTime == 0);
		case Flags::authorPeerid:
			return (this->authorPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::authorPeerInfo:
			return (this->authorPeerInfo.get() == NULL);
		case Flags::dataSign:
			return (memcmp(this->dataSign,defaultBytes16, 16) ==0);
		case Flags::innerCmdType:
			return (this->innerCmdType == 0);
		case Flags::data:
			return (this->data.get() == NULL);
		default:
			assert(0);
			return true;
	}
}

bool TcpSynConnection::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::seq:
			return (this->seq == 0);
		case Flags::result:
			return (this->result == 0);
		case Flags::toVPort:
			return (this->toVPort == 0);
		case Flags::fromSessionId:
			return (this->fromSessionId == 0);
		case Flags::fromPeerid:
			return (this->fromPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::toPeerid:
			return (this->toPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::proxyPeerid:
			return (this->proxyPeerid.compare(&peer::Id::defaultId()) == 0);
		case Flags::peerInfo:
			return (this->peerInfo.get() == NULL);
		case Flags::reverseEndpointArray:
			return (this->reverseEndpointArray.size() == 0);
		case Flags::payload:
			return (this->payload.get() == NULL);
		default:
			assert(0);
			return true;
	}
}

bool TcpAckConnection::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::seq:
			return (this->seq == 0);
		case Flags::toSessionId:
			return (this->toSessionId == 0);
		case Flags::result:
			return (this->result == 0);
		case Flags::peerInfo:
			return (this->peerInfo.get() == NULL);
		case Flags::payload:
			return (this->payload.get() == NULL);
		default:
			assert(0);
			return true;
	}
}

bool TcpAckAckConnection::isDefaultField(uint16_t fieldFlags) const {
	switch (fieldFlags) {
		case Flags::seq:
			return (this->seq == 0);
		case Flags::result:
			return (this->result == 0);
		default:
			assert(0);
			return true;
	}
}


//THIS FUNCTION GENERATE BY packages.js	
int SynTunnel::decode(BufferStream& bufferStream, const Package* pRefPackage) {
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
		bool hasField = cmdflags & Flags::fromPeerid;
		if (hasField) {
			r = this->fromPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SynTunnel::fieldName(Flags::fromPeerid), (void*)&(this->fromPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::toPeerid;
		if (hasField) {
			r = this->toPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SynTunnel::fieldName(Flags::toPeerid), (void*)&(this->toPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::proxyPeerid;
		if (hasField) {
			r = this->proxyPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SynTunnel::fieldName(Flags::proxyPeerid), (void*)&(this->proxyPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::proxyPeerInfo;
		if (hasField) {
			r = peer::Info::read(bufferStream, &this->proxyPeerInfo);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SynTunnel::fieldName(Flags::proxyPeerInfo), (void*)&(this->proxyPeerInfo));
			}
		}
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
				r = pRefPackage->copyFieldValue(SynTunnel::fieldName(Flags::seq), (void*)&(this->seq));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::fromContainerId;
		if (hasField) {
			r = bufferStream.readUInt32(&this->fromContainerId);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SynTunnel::fieldName(Flags::fromContainerId), (void*)&(this->fromContainerId));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::peerInfo;
		if (hasField) {
			r = peer::Info::read(bufferStream, &this->peerInfo);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SynTunnel::fieldName(Flags::peerInfo), (void*)&(this->peerInfo));
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
				r = pRefPackage->copyFieldValue(SynTunnel::fieldName(Flags::sendTime), (void*)&(this->sendTime));
			}
		}
	}
	r = ICN_RESULT_SUCCESS;
END:
	return r;
}
//THIS FUNCTION GENERATE BY packages.js	
int AckTunnel::decode(BufferStream& bufferStream, const Package* pRefPackage) {
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
		bool hasField = cmdflags & Flags::aesKeyHash;
		if (hasField) {
			r = bufferStream.readUInt64(&this->aesKeyHash);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(AckTunnel::fieldName(Flags::aesKeyHash), (void*)&(this->aesKeyHash));
			}
		}
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
				r = pRefPackage->copyFieldValue(AckTunnel::fieldName(Flags::seq), (void*)&(this->seq));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::fromContainerId;
		if (hasField) {
			r = bufferStream.readUInt32(&this->fromContainerId);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(AckTunnel::fieldName(Flags::fromContainerId), (void*)&(this->fromContainerId));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::toContainerId;
		if (hasField) {
			r = bufferStream.readUInt32(&this->toContainerId);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(AckTunnel::fieldName(Flags::toContainerId), (void*)&(this->toContainerId));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::result;
		if (hasField) {
			r = bufferStream.readUInt8(&this->result);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(AckTunnel::fieldName(Flags::result), (void*)&(this->result));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::peerInfo;
		if (hasField) {
			r = peer::Info::read(bufferStream, &this->peerInfo);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(AckTunnel::fieldName(Flags::peerInfo), (void*)&(this->peerInfo));
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
				r = pRefPackage->copyFieldValue(AckTunnel::fieldName(Flags::sendTime), (void*)&(this->sendTime));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::mtu;
		if (hasField) {
			r = bufferStream.readUInt16(&this->mtu);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(AckTunnel::fieldName(Flags::mtu), (void*)&(this->mtu));
			}
		}
	}
	r = ICN_RESULT_SUCCESS;
END:
	return r;
}
//THIS FUNCTION GENERATE BY packages.js	
int AckAckTunnel::decode(BufferStream& bufferStream, const Package* pRefPackage) {
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
				r = pRefPackage->copyFieldValue(AckAckTunnel::fieldName(Flags::seq), (void*)&(this->seq));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::toContainerId;
		if (hasField) {
			r = bufferStream.readUInt32(&this->toContainerId);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(AckAckTunnel::fieldName(Flags::toContainerId), (void*)&(this->toContainerId));
			}
		}
	}
	r = ICN_RESULT_SUCCESS;
END:
	return r;
}
//THIS FUNCTION GENERATE BY packages.js	
int PingTunnel::decode(BufferStream& bufferStream, const Package* pRefPackage) {
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
		bool hasField = cmdflags & Flags::toContainerId;
		if (hasField) {
			r = bufferStream.readUInt32(&this->toContainerId);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(PingTunnel::fieldName(Flags::toContainerId), (void*)&(this->toContainerId));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::packageId;
		if (hasField) {
			r = bufferStream.readUInt32(&this->packageId);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(PingTunnel::fieldName(Flags::packageId), (void*)&(this->packageId));
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
				r = pRefPackage->copyFieldValue(PingTunnel::fieldName(Flags::sendTime), (void*)&(this->sendTime));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::recvData;
		if (hasField) {
			r = bufferStream.readUInt64(&this->recvData);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(PingTunnel::fieldName(Flags::recvData), (void*)&(this->recvData));
			}
		}
	}
	r = ICN_RESULT_SUCCESS;
END:
	return r;
}
//THIS FUNCTION GENERATE BY packages.js	
int PingTunnelResp::decode(BufferStream& bufferStream, const Package* pRefPackage) {
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
		bool hasField = cmdflags & Flags::toContainerId;
		if (hasField) {
			r = bufferStream.readUInt32(&this->toContainerId);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(PingTunnelResp::fieldName(Flags::toContainerId), (void*)&(this->toContainerId));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::ackPackageId;
		if (hasField) {
			r = bufferStream.readUInt32(&this->ackPackageId);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(PingTunnelResp::fieldName(Flags::ackPackageId), (void*)&(this->ackPackageId));
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
				r = pRefPackage->copyFieldValue(PingTunnelResp::fieldName(Flags::sendTime), (void*)&(this->sendTime));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::recvData;
		if (hasField) {
			r = bufferStream.readUInt64(&this->recvData);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(PingTunnelResp::fieldName(Flags::recvData), (void*)&(this->recvData));
			}
		}
	}
	r = ICN_RESULT_SUCCESS;
END:
	return r;
}
//THIS FUNCTION GENERATE BY packages.js	
int SnCall::decode(BufferStream& bufferStream, const Package* pRefPackage) {
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
				r = pRefPackage->copyFieldValue(SnCall::fieldName(Flags::seq), (void*)&(this->seq));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::snPeerid;
		if (hasField) {
			r = this->snPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCall::fieldName(Flags::snPeerid), (void*)&(this->snPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::toPeerid;
		if (hasField) {
			r = this->toPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCall::fieldName(Flags::toPeerid), (void*)&(this->toPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::fromPeerid;
		if (hasField) {
			r = this->fromPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCall::fieldName(Flags::fromPeerid), (void*)&(this->fromPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::reverseEndpointArray;
		if (hasField) {
			r = readEndpointArray(bufferStream, &this->reverseEndpointArray);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCall::fieldName(Flags::reverseEndpointArray), (void*)&(this->reverseEndpointArray));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::peerInfo;
		if (hasField) {
			r = peer::Info::read(bufferStream, &this->peerInfo);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCall::fieldName(Flags::peerInfo), (void*)&(this->peerInfo));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::payload;
		if (hasField) {
			r = bufferStream.readBuffer(&this->payload);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCall::fieldName(Flags::payload), (void*)&(this->payload));
			}
		}
	}
	r = ICN_RESULT_SUCCESS;
END:
	return r;
}
//THIS FUNCTION GENERATE BY packages.js	
int SnCallResp::decode(BufferStream& bufferStream, const Package* pRefPackage) {
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
				r = pRefPackage->copyFieldValue(SnCallResp::fieldName(Flags::seq), (void*)&(this->seq));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::snPeerid;
		if (hasField) {
			r = this->snPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCallResp::fieldName(Flags::snPeerid), (void*)&(this->snPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::result;
		if (hasField) {
			r = bufferStream.readUInt8(&this->result);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCallResp::fieldName(Flags::result), (void*)&(this->result));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::toPeerInfo;
		if (hasField) {
			r = peer::Info::read(bufferStream, &this->toPeerInfo);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCallResp::fieldName(Flags::toPeerInfo), (void*)&(this->toPeerInfo));
			}
		}
	}
	r = ICN_RESULT_SUCCESS;
END:
	return r;
}
//THIS FUNCTION GENERATE BY packages.js	
int SnCalled::decode(BufferStream& bufferStream, const Package* pRefPackage) {
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
				r = pRefPackage->copyFieldValue(SnCalled::fieldName(Flags::seq), (void*)&(this->seq));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::snPeerid;
		if (hasField) {
			r = this->snPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCalled::fieldName(Flags::snPeerid), (void*)&(this->snPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::toPeerid;
		if (hasField) {
			r = this->toPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCalled::fieldName(Flags::toPeerid), (void*)&(this->toPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::fromPeerid;
		if (hasField) {
			r = this->fromPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCalled::fieldName(Flags::fromPeerid), (void*)&(this->fromPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::reverseEndpointArray;
		if (hasField) {
			r = readEndpointArray(bufferStream, &this->reverseEndpointArray);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCalled::fieldName(Flags::reverseEndpointArray), (void*)&(this->reverseEndpointArray));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::peerInfo;
		if (hasField) {
			r = peer::Info::read(bufferStream, &this->peerInfo);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCalled::fieldName(Flags::peerInfo), (void*)&(this->peerInfo));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::payload;
		if (hasField) {
			r = bufferStream.readBuffer(&this->payload);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCalled::fieldName(Flags::payload), (void*)&(this->payload));
			}
		}
	}
	r = ICN_RESULT_SUCCESS;
END:
	return r;
}
//THIS FUNCTION GENERATE BY packages.js	
int SnCalledResp::decode(BufferStream& bufferStream, const Package* pRefPackage) {
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
				r = pRefPackage->copyFieldValue(SnCalledResp::fieldName(Flags::seq), (void*)&(this->seq));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::snPeerid;
		if (hasField) {
			r = this->snPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCalledResp::fieldName(Flags::snPeerid), (void*)&(this->snPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::result;
		if (hasField) {
			r = bufferStream.readUInt8(&this->result);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnCalledResp::fieldName(Flags::result), (void*)&(this->result));
			}
		}
	}
	r = ICN_RESULT_SUCCESS;
END:
	return r;
}
//THIS FUNCTION GENERATE BY packages.js	
int SnPing::decode(BufferStream& bufferStream, const Package* pRefPackage) {
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
				r = pRefPackage->copyFieldValue(SnPing::fieldName(Flags::seq), (void*)&(this->seq));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::snPeerid;
		if (hasField) {
			r = this->snPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnPing::fieldName(Flags::snPeerid), (void*)&(this->snPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::fromPeerid;
		if (hasField) {
			r = this->fromPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnPing::fieldName(Flags::fromPeerid), (void*)&(this->fromPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::peerInfo;
		if (hasField) {
			r = peer::Info::read(bufferStream, &this->peerInfo);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnPing::fieldName(Flags::peerInfo), (void*)&(this->peerInfo));
			}
		}
	}
	r = ICN_RESULT_SUCCESS;
END:
	return r;
}
//THIS FUNCTION GENERATE BY packages.js	
int SnPingResp::decode(BufferStream& bufferStream, const Package* pRefPackage) {
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
				r = pRefPackage->copyFieldValue(SnPingResp::fieldName(Flags::seq), (void*)&(this->seq));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::snPeerid;
		if (hasField) {
			r = this->snPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnPingResp::fieldName(Flags::snPeerid), (void*)&(this->snPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::result;
		if (hasField) {
			r = bufferStream.readUInt8(&this->result);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnPingResp::fieldName(Flags::result), (void*)&(this->result));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::peerInfo;
		if (hasField) {
			r = peer::Info::read(bufferStream, &this->peerInfo);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnPingResp::fieldName(Flags::peerInfo), (void*)&(this->peerInfo));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::endpointArray;
		if (hasField) {
			r = readEndpointArray(bufferStream, &this->endpointArray);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(SnPingResp::fieldName(Flags::endpointArray), (void*)&(this->endpointArray));
			}
		}
	}
	r = ICN_RESULT_SUCCESS;
END:
	return r;
}
//THIS FUNCTION GENERATE BY packages.js	
int TcpSynConnection::decode(BufferStream& bufferStream, const Package* pRefPackage) {
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
				r = pRefPackage->copyFieldValue(TcpSynConnection::fieldName(Flags::seq), (void*)&(this->seq));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::result;
		if (hasField) {
			r = bufferStream.readUInt8(&this->result);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(TcpSynConnection::fieldName(Flags::result), (void*)&(this->result));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::toVPort;
		if (hasField) {
			r = bufferStream.readUInt16(&this->toVPort);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(TcpSynConnection::fieldName(Flags::toVPort), (void*)&(this->toVPort));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::fromSessionId;
		if (hasField) {
			r = bufferStream.readUInt32(&this->fromSessionId);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(TcpSynConnection::fieldName(Flags::fromSessionId), (void*)&(this->fromSessionId));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::fromPeerid;
		if (hasField) {
			r = this->fromPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(TcpSynConnection::fieldName(Flags::fromPeerid), (void*)&(this->fromPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::toPeerid;
		if (hasField) {
			r = this->toPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(TcpSynConnection::fieldName(Flags::toPeerid), (void*)&(this->toPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::proxyPeerid;
		if (hasField) {
			r = this->proxyPeerid.read(bufferStream);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(TcpSynConnection::fieldName(Flags::proxyPeerid), (void*)&(this->proxyPeerid));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::peerInfo;
		if (hasField) {
			r = peer::Info::read(bufferStream, &this->peerInfo);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(TcpSynConnection::fieldName(Flags::peerInfo), (void*)&(this->peerInfo));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::reverseEndpointArray;
		if (hasField) {
			r = readEndpointArray(bufferStream, &this->reverseEndpointArray);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(TcpSynConnection::fieldName(Flags::reverseEndpointArray), (void*)&(this->reverseEndpointArray));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::payload;
		if (hasField) {
			r = bufferStream.readBuffer(&this->payload);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(TcpSynConnection::fieldName(Flags::payload), (void*)&(this->payload));
			}
		}
	}
	r = ICN_RESULT_SUCCESS;
END:
	return r;
}
//THIS FUNCTION GENERATE BY packages.js	
int TcpAckConnection::decode(BufferStream& bufferStream, const Package* pRefPackage) {
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
				r = pRefPackage->copyFieldValue(TcpAckConnection::fieldName(Flags::seq), (void*)&(this->seq));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::toSessionId;
		if (hasField) {
			r = bufferStream.readUInt32(&this->toSessionId);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(TcpAckConnection::fieldName(Flags::toSessionId), (void*)&(this->toSessionId));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::result;
		if (hasField) {
			r = bufferStream.readUInt8(&this->result);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(TcpAckConnection::fieldName(Flags::result), (void*)&(this->result));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::peerInfo;
		if (hasField) {
			r = peer::Info::read(bufferStream, &this->peerInfo);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(TcpAckConnection::fieldName(Flags::peerInfo), (void*)&(this->peerInfo));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::payload;
		if (hasField) {
			r = bufferStream.readBuffer(&this->payload);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(TcpAckConnection::fieldName(Flags::payload), (void*)&(this->payload));
			}
		}
	}
	r = ICN_RESULT_SUCCESS;
END:
	return r;
}
//THIS FUNCTION GENERATE BY packages.js	
int TcpAckAckConnection::decode(BufferStream& bufferStream, const Package* pRefPackage) {
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
				r = pRefPackage->copyFieldValue(TcpAckAckConnection::fieldName(Flags::seq), (void*)&(this->seq));
			}
		}
	}
	{
		bool hasField = cmdflags & Flags::result;
		if (hasField) {
			r = bufferStream.readUInt8(&this->result);
			if (r <= 0) {
				r = -1;   
				goto END;
			}
			readlen += r;
		} 
		else {
			if (pRefPackage) {
				r = pRefPackage->copyFieldValue(TcpAckAckConnection::fieldName(Flags::result), (void*)&(this->result));
			}
		}
	}
	r = ICN_RESULT_SUCCESS;
END:
	return r;
}
