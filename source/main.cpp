#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include "lib.hpp"
#include "process.hpp"
#include "srtn.hpp"
#include <iostream>
#include <format>
#include <string>

// Main code
int main(int, char**)
{
    // Initialize app instance
    AppContext app = AppContext();
    auto table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;

    // Program state variables
    static bool window_open = true;
    static SRTN srtn_state {};
    srtn_state.resize(1);

    // Main app loop
    while (!app.done) {
        if (!window_open) { // window_open turns false when the x button on the window is pressed
            app.done = true;
            continue;
        }
        // Start the current frame
        app.poll_events();
        app.start_frame();


        // Begin appending to window
        ImGui::Begin("SRTN Calculator", &window_open);

        // Keep track of the amount of process the user wants
        static int process_count = 1;
        if( ImGui::InputInt("Process Count", &process_count) ) { // Returns true if user has input
            if (process_count < 1) // Don't allow process_count that is less than one
                process_count = 1;

            // Resize SRTN state and recalculate
            srtn_state.resize(static_cast<size_t>(process_count));
            srtn_state.calculate();
        }

        // Keep track of if user changes info about processes
        bool changed_srtn = false;

        // Display Processes and the input for their info
        ImGui::Text("Processes");
        ImGui::BeginTable("Process Table", 3, table_flags);
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Process ID");
        ImGui::TableNextColumn();
        ImGui::Text("Arrival Time");
        ImGui::TableNextColumn();
        ImGui::Text("Burst Time");

        // Loop through each process and show and get input for their info
        for (unsigned int row = 0; row < srtn_state.processes.size(); ++row) {
            Process *process = &(srtn_state.processes[row]);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("P%d", process->id);
            ImGui::TableNextColumn();

            // Get input for process arrival_time and burst_time
            if (ImGui::InputInt(std::format("{}at", row + 1).c_str(), &process->arrival_time))
                changed_srtn = true;
			if (process->arrival_time < 0) // arrival_time cant be less than 0
				process->arrival_time = 0;

            ImGui::TableNextColumn();

            if (ImGui::InputInt(std::format("{}bt", row + 1).c_str(), &process->burst_time))
                changed_srtn = true;
			if (process->burst_time < 1) // burst_time cant be less than 1
				process->burst_time = 1;
        }

        ImGui::EndTable();

        ImGui::Separator();

        // Display Gantt Chart
        ImGui::Text("Gantt Chart");

        const auto& gantt_chart = srtn_state.gantt_chart; // Make a reference variable for conciseness
        ImGui::BeginTable("Gantt", std::max<int>(static_cast<int>(gantt_chart.size()), 1), table_flags);

        for (int row = 0; row < 2; row++) {
            ImGui::TableNextRow();
            for (unsigned int col = 0; col < gantt_chart.size(); ++col) {
                ImGui::TableSetColumnIndex(static_cast<int>(col));
                if (row == 0) {
                    ImGui::Text("P%d", gantt_chart[col].id);
                } else {
                    ImGui::Text("%d", gantt_chart[col].time);
                }
            }
        }

        ImGui::EndTable();

        ImGui::Separator();

        // Display Waiting Time of each Process

        ImGui::Text("Waiting Time = Turn-Around Time - Burst Time");
        ImGui::BeginTable("WaT", 2, table_flags);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Process ID");
        ImGui::TableNextColumn();
        ImGui::Text("Waiting Time");
        for (unsigned int row = 0; row < srtn_state.processes.size(); ++row) {
            Process &process = srtn_state.processes[row];
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("P%d", process.id);
            ImGui::TableNextColumn();
            ImGui::Text("%d", process.wait_time);
        }
        ImGui::EndTable();
        ImGui::Text("Total Waiting Time: %.0f", srtn_state.total_waiting_time);
        ImGui::Text("Average Waiting Time: %.02f", srtn_state.average_waiting_time);

        ImGui::Separator();

        // Display Turn-Around Time of each Process

        ImGui::Text("Turn-Around Time = End Time - Arrival Time");
        ImGui::BeginTable("TaT", 2, table_flags);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Process ID");
        ImGui::TableNextColumn();
        ImGui::Text("Turn-Around Time");
        for (unsigned int row = 0; row < srtn_state.processes.size(); ++row) {
            Process &process = srtn_state.processes[row];
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("P%d", process.id);
            ImGui::TableNextColumn();
            ImGui::Text("%d", process.turn_around_time);
        }
        ImGui::EndTable();

        ImGui::Text("Total Turn-Around Time: %.0f", srtn_state.total_turn_around_time);
        ImGui::Text("Average Turn-Around Time: %.02f", srtn_state.avarege_turn_around_time);

        ImGui::End();

        if (changed_srtn) // If user changed info about processes, recalculate
            srtn_state.calculate();

        // End the current frame
        app.end_frame();
    }

    return 0;
}
