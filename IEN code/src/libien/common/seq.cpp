#include "./common.h"

#define TIME_NOW (uint32_t)((IcnTimeGetNow(false) >> 20) & ((1 << SEQUENCE_TIME_BITS) - 1))

SequenceCreator::SequenceCreator() {
	this->lastValue.bits.time = TIME_NOW;
	this->lastValue.bits.increase = (uint32_t)(random64() & ((1 << SEQUENCE_INCREASE_BITS) - 1));
}

uint32_t SequenceCreator::value() {
	return creator->lastValue.u32;
}

SequenceCreator::~SequenceCreator() {

}

uint32_t SequenceCreator::next() {
	Sequence lastValue = this->lastValue;
	Sequence newValue;
	newValue.bits.time = TIME_NOW;
	newValue.bits.increase = lastValue.bits.increase + 1;

	if (lastValue.i32 != IcnAtomicCompareAndSwap32(&creator->lastValue.i32, lastValue.i32, newValue.i32)) {
		return this->next();
	}
	return newValue.u32;
}

// 假定同时不存在两个相差过大的活跃sequence；
// 相差过大多半是因为时间段刚好溢出导致sequence绝对值变小
int Sequence::compare(uint32_t seq1, uint32_t seq2)
{
	if (seq1 == seq2) {
		return 0;
	}

	if (seq1 > seq2) {
		if (seq1 - seq2 < 0x80000000) {
			return 1;
		}
		else {
			return -1;
		}
	}
	else {
		if (seq2 - seq1 < 0x80000000) {
			return -1;
		}
		else {
			return 1;
		}
	}
}

bool Sequence::isTimeout(uint32_t seq, uint32_t timeLimitS) {
	Sequence seqObj = {
		.u32 = seq
	};

	uint32_t now = TIME_NOW;
	if (now < seqObj.bits.time) {
		now += (1 << SEQUENCE_TIME_BITS);
	}
	return now - seqObj.bits.time > timeLimitS;
}

