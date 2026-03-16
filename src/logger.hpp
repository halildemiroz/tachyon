#pragma once

#include <cstdint>
#include <queue>
#include <iostream>
#include <mutex>
#include <fstream>

struct TradeEvent{
	uint64_t timeStamp;
	uint64_t buyerID;
	uint64_t sellerID;
	uint64_t price;
	uint64_t quantity;
};

void Logger(std::queue<TradeEvent>& tradeQueue, std::mutex& queueMutex);

