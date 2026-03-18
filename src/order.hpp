#pragma once

#include <ringBuffer.hpp>
#include <cstdint>
#include <deque>
#include <array>
#include <vector>

// dummy prices
constexpr uint64_t MIN_PRICE = 5000000;
constexpr uint64_t MAX_PRICE = 8000000;
constexpr uint64_t TICK_SIZE = 1000;
constexpr std::size_t NUM_PRICE_LEVELS = (MAX_PRICE - MIN_PRICE) / TICK_SIZE + 1;

struct TradeEvent{
	uint64_t timeStamp;
	uint64_t buyerID;
	uint64_t sellerID;
	uint64_t price;
	uint64_t quantity;
};

inline int priceToIndex(uint64_t price){
	return (price - MIN_PRICE) / TICK_SIZE;
}

enum class OrderSide {
	BIDS=0, ASKS
};

struct Order{
	uint64_t id;
	uint64_t price;
	uint64_t quantity;
	uint64_t timeStamp;
	OrderSide side;
};

struct PriceLevel{
	std::deque<Order> orders;
};

struct OrderBook{
	// highest->lowest
	std::array<PriceLevel, NUM_PRICE_LEVELS> bids;
	// lowest->highest
	std::array<PriceLevel, NUM_PRICE_LEVELS> asks;

	int bestBidIndex = -1;
	int bestAskIndex = NUM_PRICE_LEVELS;
};

void addOrder(OrderBook& book, Order order, LockFreeQueue<TradeEvent>& tradeQueue, bool isRecovery = false);
void makeTrade(LockFreeQueue<TradeEvent>& tradeQueue, LockFreeQueue<Order>& orderQueue, const std::vector<Order>& rawOrders);
OrderBook recoverOrderBook();

