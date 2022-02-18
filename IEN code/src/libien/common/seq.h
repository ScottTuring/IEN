/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once

#define SEQUENCE_TIME_BITS 20
#define SEQUENCE_INCREASE_BITS (32 - SEQUENCE_TIME_BITS)

union Sequence {
	struct {
		volatile uint32_t increase : SEQUENCE_INCREASE_BITS;
		volatile uint32_t time : SEQUENCE_TIME_BITS;
	} bits;
	volatile uint32_t u32;
	volatile int32_t i32;

	static int compare(uint32_t seq1, uint32_t seq2);
	static bool isTimeout(uint32_t seq, uint32_t timeLimitS);
} ;

class SequenceCreator {
public:
	SequenceCreator();
	~SequenceCreator();
	uint32_t value();
	uint32_t next();

private:
	Sequence lastValue;
};
