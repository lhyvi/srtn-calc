// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

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

    AppContext app = AppContext();
    auto io = app.getIO();
    auto table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;

    static bool window_open = true;
    static SRTN srtn_state {};
    srtn_state.resize(1);

    while (!app.done) {
        if (!window_open) {
            app.done = true;
            continue;
        }
        app.poll_events();
        app.start_frame();


        ImGui::Begin("SRTN Calculator", &window_open);                          // Create a window called "Hello, world!" and append into it.

        static int table_cols = 1;
        if(ImGui::InputInt("Table Cols", &table_cols)) {
            if (table_cols < 1)
                table_cols = 1;
            srtn_state.resize(table_cols);
            srtn_state.calculate();
        }

        bool changed_srtn = false;

        ImGui::Text("Processes");
        ImGui::BeginTable("Process Table", 3, table_flags);
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Process ID");
        ImGui::TableNextColumn();
        ImGui::Text("Arrival Time");
        ImGui::TableNextColumn();
        ImGui::Text("Burst Time");
        for (int row = 0; row < srtn_state.processes.size(); ++row) {
            Process *process = &(srtn_state.processes[row]);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("P%d", process->id);
            ImGui::TableNextColumn();
            if (ImGui::InputInt(std::format("{}at", row + 1).c_str(), &process->arrival_time))
                changed_srtn = true;
			if (process->arrival_time < 0)
				process->arrival_time = 0;
            ImGui::TableNextColumn();
            if (ImGui::InputInt(std::format("{}bt", row + 1).c_str(), &process->burst_time))
                changed_srtn = true;
			if (process->burst_time < 1)
				process->burst_time = 1;
        }

        ImGui::EndTable();

        ImGui::Separator();

        ImGui::Text("Gantt Chart");
        const auto& gantt_chart = srtn_state.gantt_chart;
        ImGui::BeginTable("Gantt", std::max<size_t>(gantt_chart.size(), 1), table_flags);

        for (int row = 0; row < 2; row++) {
            ImGui::TableNextRow();
            for (int col = 0; col < gantt_chart.size(); ++col) {
                ImGui::TableSetColumnIndex(col);
                if (row == 0) {
                    ImGui::Text("P%d", gantt_chart[col].id);
                } else {
                    ImGui::Text("%d", gantt_chart[col].time);
                }
            }
        }

        ImGui::EndTable();

        ImGui::Separator();

        ImGui::BeginTable("WaT", 2, table_flags);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Process ID");
        ImGui::TableNextColumn();
        ImGui::Text("Waiting Time");
        for (int row = 0; row < srtn_state.processes.size(); ++row) {
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

        ImGui::BeginTable("WaT", 2, table_flags);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Process ID");
        ImGui::TableNextColumn();
        ImGui::Text("Turn-Around Time");
        for (int row = 0; row < srtn_state.processes.size(); ++row) {
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

        if (changed_srtn)
            srtn_state.calculate();

        app.end_frame();
    }

    return 0;
}
