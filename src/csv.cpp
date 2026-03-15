#include <csv.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>

std::vector<Order> readCSV(const std::string csvPath){
	std::ifstream file(csvPath);
	std::string line;
	std::vector<Order> data;

	if(!file.is_open()){
		std::cerr << "Could not open CSV file: " << csvPath << "\n";
		return data;
	}
	
	std::getline(file, line);

	while(std::getline(file, line)){
		if(line.empty()) { continue; }

		std::istringstream iss(line);
		std::string word;
		Order order;

		// read timetamp
		std::getline(iss, word, ',');
		order.timeStamp = std::stoull(word);

		// read id
		std::getline(iss, word, ',');
		order.id = std::stoull(word);

		// read side
		std::getline(iss, word, ',');
		if(word == "BUY"){order.side = OrderType::BIDS;}
		else {order.side = OrderType::ASKS;}

		// read price
		std::getline(iss, word, ',');
		order.price = std::stoull(word);

		// read quantity
		std::getline(iss, word, ',');
		order.quantity = std::stoull(word);

		data.push_back(order);
	}
	return data;
}

void renderCSV(const std::vector<Order>& orders){
	std::cout << std::left
		<< std::setw(15) << "Timestamp"
		<< std::setw(10) << "OrderID"
		<< std::setw(10) << "Side"
		<< std::setw(10) << "Price"
		<< std::setw(10) << "Quantity" << "\n";
	
	std::cout << std::string(55, '-') << "\n";

	for(const auto& order : orders){
		std::cout << std::left
			<< std::setw(15) << order.timeStamp
			<< std::setw(10) << order.id
			<< std::setw(10) << (order.side == OrderType::BIDS ? "BUY" : "SELL")
			<< std::setw(10) << order.price
			<< std::setw(10) << order.quantity << "\n";
	}
}

