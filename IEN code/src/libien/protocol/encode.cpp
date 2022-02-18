#include "./encode.h"

namespace ien {
	namespace protocol {
		int decodePackages(
			const uint8_t* buffer,
			size_t length,
			const Package* refPackage,
			Package** result,
			bool hasExchange) {
			int r = 0;
			int result_pos = 0;

			BufferStream bufferStream;
			bufferStream.init((uint8_t*)buffer, length);
			if (hasExchange) {
				ExchangeKey* package = new ExchangeKey();
				uint16_t totalLen = 0;
				r = bufferStream.readUInt16(&totalLen);
				if (r <= 0) {
					r = ICN_RESULT_INVALID_ENCODE;
					goto END;
				}
				r = package->decode(bufferStream, refPackage);
				if (r <= 0) {
					r = ICN_RESULT_INVALID_ENCODE;
					goto END;
				}
				result[0] = package;
				++result_pos;
			}

			r = 0;
			while (bufferStream.tail() > 3) {
				uint16_t totallen;
				uint8_t cmdtype;

				bufferStream.readUInt16(&totallen);
				bufferStream.readUInt8(&cmdtype);
				
				Package* package = Package::create(cmdtype);
				if (package == NULL) {
					r = ICN_RESULT_INVALID_ENCODE;
					goto END;
				}
				int r = package->decode(bufferStream, refPackage ? refPackage : result[0]);
				if (r != ICN_RESULT_SUCCESS) {
					delete package;
					goto END;
				}
				
				result[result_pos] = package;
				++result_pos;
			}
		END:
			return r;
		}


		int encodePackages(
			const Package** packages,
			size_t packageCount,
			const Package* refPackage,
			uint8_t* buffer,
			size_t bufferLength,
			size_t* pWriteBytes) {
			*pWriteBytes = 0;
			int r = 0;
			BufferStream stream;
			stream.init(buffer, bufferLength);
		
			size_t totalwrite = 0;

			for (size_t i = 0; i < packageCount; ++i) {
				const Package* pPackage = packages[i];
				const Package* pRefPackage = refPackage ? refPackage : (i ? packages[0] : NULL);
				size_t thisWrite = 0;
				if (pPackage->cmdtype == ExchangeKey::command && i != 0) {
					return ICN_RESULT_INVALID_PARAM;
				}

				r = pPackage->encode(pRefPackage, stream, &thisWrite);

				if (r != ICN_RESULT_SUCCESS) {
					return r;
				}
				totalwrite += thisWrite;
			}

			*pWriteBytes = totalwrite;
			return ICN_RESULT_SUCCESS;
		}
	}
}