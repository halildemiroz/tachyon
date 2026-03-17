#pragma once

#include <ringBuffer.hpp>

#include <cstdint>
#include <deque>

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
	uint64_t price;
	std::deque<Order> orders;
};

struct OrderBook{
	// highest->lowest
	std::vector<PriceLevel> bids;
	// lowest->highest
	std::vector<PriceLevel> asks;
};

void addOrder(OrderBook& book, Order order, LockFreeQueue& tradeQueue);

void makeTrade(LockFreeQueue& tradeQueue, const std::vector<Order>& rawOrders);

