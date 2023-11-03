#include "process.hpp"


Process::Process(int id, int arrival_time, int burst_time): 
	id(id),
	arrival_time(arrival_time),
	burst_time(burst_time),
	remaining_time(remaining_time)
{};

bool operator<(const Process& a, const Process& b) {
return a.remaining_time > b.remaining_time; 
}