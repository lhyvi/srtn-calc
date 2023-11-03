#include "srtn.hpp"

void SRTN::calculate() {
    // Sort the processes based on the arrival time
	gantt_chart.clear();
	for (Process& p : processes) {
		p.remaining_time = p.burst_time;
	}
	std::vector<Process> ps(processes);

    std::sort(ps.begin(), ps.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });

    std::priority_queue<Process, std::vector<Process>, std::function<bool(Process, Process)>> pq([](Process a, Process b) {
        return a.remaining_time > b.remaining_time;
    });

    int current_time = 0, next_process = 0;
    Process current_process;  // Holds the currently executing process

    while (!pq.empty() || next_process < ps.size()) {
        while (next_process < ps.size() && ps[next_process].arrival_time <= current_time) {
            pq.push(ps[next_process]);
            next_process++;
        }

        if (!pq.empty()) {
            Process next_process = pq.top();
            pq.pop();
            next_process.remaining_time--;

            // If the currently executing process changes, add the previous process and the current time to the Gantt chart
            if (current_process.id != next_process.id ) {
                gantt_chart.push_back({current_process.id, current_time});
                current_process = next_process;
            }

            if (next_process.remaining_time == 0) {
                next_process.wait_time = current_time + 1 - next_process.arrival_time - next_process.burst_time;
                next_process.end_time = current_time + 1;
                gantt_chart.push_back({next_process.id, next_process.end_time});
            } else {
                pq.push(next_process);
            }
        }

        current_time++;
    }

    average_waiting_time = 0;
    average_turnaround_time = 0;
    for (const Process& p : ps) {
        average_waiting_time += p.wait_time;
        average_turnaround_time += p.end_time - p.arrival_time;
    }

	auto cmp = [](const ChartData& a, const ChartData& b) { return a.time < b.time; };
	std::set<ChartData, decltype(cmp)> s( gantt_chart.begin(), gantt_chart.end() );
	gantt_chart.assign( s.begin(), s.end() );
    average_waiting_time /= ps.size();
    average_turnaround_time /= ps.size();
}



void SRTN::resize(int new_size) {
	if (new_size < processes.size()) {
		size_t remove_count = processes.size() - new_size;
		for(int i = 0; i < remove_count; ++i)
			processes.pop_back();
	}
	if (new_size > processes.size()) {
		size_t new_count = new_size - processes.size();
		for(int i = 0; i < new_count; ++i)
			processes.emplace_back(Process());
	}

	for (int i = 0; i < processes.size(); ++i) {
		processes[i].id = i + 1;
	}
}