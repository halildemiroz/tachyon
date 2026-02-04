#pragma once

#include <cstdint>
#include <vector>

enum class Side{
	Bid=0, Ask
};

enum class OrderType{
	Limit=0, Market
};

struct Order{
	uint64_t orderID;
	double price;
	uint32_t quantity;
	uint64_t timestamp;
	Side side;
	OrderType ot;
};

struct Trade{
	uint64_t bidOrderId;
    uint64_t askOrderId;
    double price;
    uint32_t quantity;
    uint64_t timestamp;
};

struct Level{
	double price;
	uint32_t totalQuantity;
	std::vector<Order>OrderList;
};

inline uint64_t getCurrentTimestamp() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}
