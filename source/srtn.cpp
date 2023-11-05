#include "srtn.hpp"

void SRTN::calculate() {
    // Reset gantt chart
	gantt_chart.clear();

    // Make a copy of processes array
	std::vector<std::reference_wrapper<Process>> ps {};
    // Reset process variables
	for (Process& p : processes) {
		p.remaining_time = p.burst_time;
        p.wait_time = 0;
        p.end_time = 0;
        ps.emplace_back(p);
    }
	
    // Sort array by arrival time
    std::sort(ps.begin(), ps.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });

    // Make a priority queue that sorts by remaining time
    std::priority_queue<std::reference_wrapper<Process>, std::vector<std::reference_wrapper<Process>>, std::function<bool(Process&, Process&)>> pq([](Process &a, Process &b) {
        return a.remaining_time > b.remaining_time;
    });

    // Setup calculation variables
    int current_time = 0;
    size_t next_idx = 0; // Used to keep track of the next process to calculate
    Process zero_process = Process(); // Process with id 0 is used when nothing is running
    Process *current_process = &zero_process;  // Holds the currently executing process

    // Keep calculating while queue isn't empty or there are processes that havent arrived yet
    while (!pq.empty() || next_idx < ps.size()) {
        // Update the queue if there is are new processes that arrived
        while (next_idx < ps.size() && ps[next_idx].get().arrival_time <= current_time) {
            pq.push(ps[next_idx]);
            next_idx++;
        }

        // If the queue isn't empty, calculate for top of queue
        if (!pq.empty()) {
            // Set top process as next and decrement time
            Process &next_process = pq.top();
            next_process.remaining_time--;
            pq.pop();

            // If the currently executing process changes, add the previous process and the current time to the Gantt chart
            if (current_process->id != next_process.id ) {
                gantt_chart.push_back({current_process->id, current_time});
                current_process = &next_process;
                next_process.start_time = current_time;
            }

            if (next_process.remaining_time == 0) {
				// If the process has ended, set time and add to gantt chart
                next_process.end_time = current_time + 1;
                gantt_chart.push_back({next_process.id, next_process.end_time});
            } else {
                // If the process hasn't ended yet, add back to the queue
                pq.push(next_process);
            }
        } else { // If the queue is empty that means there are no processes needed to be run
            current_process = &zero_process;
        }

        // Increment time
        current_time++;
    }

    // Calculate turn-around time and waiting time

    total_turn_around_time = 0;
    total_waiting_time = 0;
    // Calculate each processes TaT and WT
    for (Process& p : ps) {
        p.turn_around_time = p.end_time - p.arrival_time;
        p.wait_time = p.turn_around_time - p.burst_time;

        // Add result to total
        total_waiting_time += p.wait_time;
        total_turn_around_time += p.turn_around_time;
    }

    // Remove duplicates from gantt chart

    // Make a new vector to hold the filtered gantt chart
	std::vector<ChartData> filtered_gantt_chart{};

    for (ChartData data : gantt_chart) {
        if (data.id == 0) // Don't include zero_process
            continue;
        
        // If the data is a duplicate, don't include it
        if (!filtered_gantt_chart.empty()
            && filtered_gantt_chart.back().id == data.id
            && filtered_gantt_chart.back().time == data.time
            )
            continue;

        // Push data onto the filtered gantt chart
        filtered_gantt_chart.push_back(data);
    }
	gantt_chart = filtered_gantt_chart;

    // Calculate average
    average_waiting_time = total_waiting_time / ps.size();
    avarege_turn_around_time = total_turn_around_time / ps.size();
}



void SRTN::resize(size_t new_size) {
    // If the new_size is less than current_size
    // Pop processes until it matches to new_size
	if (new_size < processes.size()) {
		size_t remove_count = processes.size() - new_size;
		for(size_t i = 0; i < remove_count; ++i)
			processes.pop_back();
	}

    // If the new_size is greater than current_size
    // Add processes until it matches to new_size
	if (new_size > processes.size()) {
		size_t new_count = new_size - processes.size();
		for(size_t i = 0; i < new_count; ++i)
			processes.emplace_back(Process());
	}

    // Assign the id of each process
	for (size_t i = 0; i < processes.size(); ++i) {
		processes[i].id = i + 1;
	}
}