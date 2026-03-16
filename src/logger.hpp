#pragma once

#include <cstdint>

struct TradeEvent{
	uint64_t timeStamp;
	uint64_t buyerID;
	uint64_t sellerID;
	uint64_t price;
	uint64_t quantity;
};

void aofLogger();

