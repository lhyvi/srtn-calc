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
	float average_waiting_time{};
	float average_turnaround_time{};

	void calculate();
	void resize(int new_size);
};