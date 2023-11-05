#pragma once

#include "process.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <set>
#include <queue>
#include <vector>

struct ChartData {
	int id;
	int time;
};

class SRTN {
public:
	std::vector<Process> processes{};
	std::vector<ChartData> gantt_chart{};
	float total_waiting_time{};
	float total_turn_around_time{};
	float average_waiting_time{};
	float average_turn_around_time{};

	void calculate();
	void resize(size_t new_size);
};