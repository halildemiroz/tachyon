#pragma once

#include <string>
#include <sys/types.h>
#include <vector>
#include <order.hpp>


std::vector<Order> readCSV(const std::string csvPath);
void renderCSV(const std::vector<Order>& order);
