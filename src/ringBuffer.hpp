#pragma once

#include <atomic>
#include <cstddef>
#include <logger.hpp>

constexpr std::size_t BUFFER_SIZE = 1048576;
constexpr std::size_t BUFFER_MASK = BUFFER_SIZE - 1;

class LockFreeQueue{
	private:
		TradeEvent buffer[BUFFER_SIZE];

		alignas(64) std::atomic<size_t> writeIndex{0};
		alignas(64) std::atomic<size_t> readIndex{0};

	public:
		bool push(const TradeEvent& event){
			size_t currentWrite = writeIndex.load(std::memory_order_relaxed);
			size_t nextWrite = (currentWrite + 1) & BUFFER_MASK;
			
			if(nextWrite == readIndex.load(std::memory_order_acquire))
				return false;

			buffer[currentWrite] = event;
			writeIndex.store(nextWrite, std::memory_order_release);
			return true;
		}

		bool pop(TradeEvent& event){
			size_t currentRead = readIndex.load(std::memory_order_relaxed);

			if(currentRead == writeIndex.load(std::memory_order_acquire))
				return false;

			event = buffer[currentRead];
			readIndex.store((currentRead + 1) & BUFFER_MASK, std::memory_order_release);
			return true;
		}

};

