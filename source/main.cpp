#include <iostream>
#include <ctime>
#include <vector>

typedef enum{
	BID=0, ASK
} OrderType;

typedef struct{
	OrderType ot;
	float price;
	int quantity;
	int id;
	time_t timestamp;
} Order;

class OrderBook{
private:
	std::vector<Order>Bids;
	std::vector<Order>Asks;
public:
	void addOrder();
};

int main(){

	std::cout << "HEllo World!" << std::endl;

	return 0;
}
