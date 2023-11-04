#include "srtn.hpp"

void SRTN::calculate() {
    // Sort the processes based on the arrival time
	gantt_chart.clear();

	std::vector<std::reference_wrapper<Process>> ps {};
	for (Process& p : processes) {
		p.remaining_time = p.burst_time;
        p.wait_time = 0;
        p.end_time = 0;
        ps.emplace_back(p);
    }
	

    std::sort(ps.begin(), ps.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });

    std::priority_queue<std::reference_wrapper<Process>, std::vector<std::reference_wrapper<Process>>, std::function<bool(Process&, Process&)>> pq([](Process &a, Process &b) {
        return a.remaining_time > b.remaining_time;
    });

    int current_time = 0, next_process = 0;
    Process zero_process = Process();
    Process *current_process = &zero_process;  // Holds the currently executing process

    while (!pq.empty() || next_process < ps.size()) {
        while (next_process < ps.size() && ps[next_process].get().arrival_time <= current_time) {
            pq.push(ps[next_process]);
            next_process++;
        }

        if (!pq.empty()) {
            Process &next_process = pq.top();
            next_process.remaining_time--;

            // If the currently executing process changes, add the previous process and the current time to the Gantt chart
            if (current_process->id != next_process.id ) {
                gantt_chart.push_back({current_process->id, current_time});
                current_process = &next_process;
                next_process.start_time = current_time;
            }
            pq.pop();

            if (next_process.remaining_time == 0) {
                next_process.end_time = current_time + 1;

                gantt_chart.push_back({next_process.id, next_process.end_time});
            } else {
                pq.push(next_process);
            }
        }

        current_time++;
    }

    total_turn_around_time = 0;
    total_waiting_time = 0;
    for (Process& p : ps) {
        p.turn_around_time = p.end_time - p.arrival_time;
        p.wait_time = p.turn_around_time - p.burst_time;

        total_waiting_time += p.wait_time;
        total_turn_around_time += p.turn_around_time;
    }

	auto cmp = [](const ChartData& a, const ChartData& b) { return a.time < b.time; };
	std::set<ChartData, decltype(cmp)> s( gantt_chart.begin(), gantt_chart.end() );
	gantt_chart.assign( s.begin(), s.end() );

    average_waiting_time = total_waiting_time / ps.size();
    avarege_turn_around_time = total_turn_around_time / ps.size();
}



void SRTN::resize(size_t new_size) {
	if (new_size < processes.size()) {
		size_t remove_count = processes.size() - new_size;
		for(size_t i = 0; i < remove_count; ++i)
			processes.pop_back();
	}
	if (new_size > processes.size()) {
		size_t new_count = new_size - processes.size();
		for(size_t i = 0; i < new_count; ++i)
			processes.emplace_back(Process());
	}

	for (size_t i = 0; i < processes.size(); ++i) {
		processes[i].id = i + 1;
	}
}