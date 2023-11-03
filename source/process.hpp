#pragma once

#include <string>
#include <queue>
#include <algorithm>
#include <vector>

class Process {
public:
	int id{};
	int arrival_time{};
	int burst_time{1};
	int remaining_time{};

	int wait_time{};
	int end_time{};


	Process(int id=0, int arrival_time=0, int burst_time=1);

	friend bool operator<(const Process& a, const Process& b);
};