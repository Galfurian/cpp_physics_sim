
#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>
#include <cmath>
#include <thread>

#include "entity/control.hpp"
#include "entity/circle.hpp"
#include "entity/edge.hpp"
#include "entity/box.hpp"
#include "simulator.hpp"

void DrawDottedLine(SDL_Renderer *renderer, int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err   = dx + dy, e2;
    int count = 0;
    while (1) {
        if (count < 10) {
            SDL_RenderDrawPoint(renderer, x0, y0);
        }
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 > dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
        count = (count + 1) % 20;
    }
}

int main(int argc, char *argv[])
{
    auto fps_limit = std::chrono::duration_cast<std::chrono::system_clock::duration>(
        std::chrono::duration<double>{ 1. / 60 });
    auto begin_frame                = std::chrono::system_clock::now();
    auto end_frame                  = begin_frame + fps_limit;
    auto prev_time_in_seconds       = std::chrono::time_point_cast<std::chrono::seconds>(begin_frame);
    auto time_in_seconds            = prev_time_in_seconds;
    unsigned frame_count_per_second = 0;

    // Initialize the simulator.
    Simulator sim(80, 1024, 512, { 0.f, 9.81f });
    // Initialize SDL.
    SDL_Init(SDL_INIT_EVERYTHING);
    // Initialize the window.
    sim.window = SDL_CreateWindow(
        "Physics Simulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        sim.width,
        sim.height,
        SDL_WINDOW_SHOWN);
    // Initialize the renderer.
    sim.renderer = SDL_CreateRenderer(
        sim.window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    float min_x = -(sim.width / 2.0f) / sim.met2pix + 0.15;
    float max_x = +(sim.width / 2.0f) / sim.met2pix - 0.15;
    float min_y = -(sim.height / 2.0f) / sim.met2pix + 0.15;
    float max_y = +(sim.height / 2.0f) / sim.met2pix - 0.15;
    sim.entities.emplace_back(
        new Edge(sim, { min_x, max_y }, { min_x, min_y },
                 b2_staticBody, { 0, 0, 0, 255 }));
    sim.entities.emplace_back(
        new Edge(sim, { min_x, min_y }, { max_x, min_y },
                 b2_staticBody, { 0, 0, 0, 255 }));
    sim.entities.emplace_back(
        new Edge(sim, { max_x, min_y }, { max_x, max_y },
                 b2_staticBody, { 0, 0, 0, 255 }));

    sim.entities.emplace_back(
        new Control(
            new Circle(
                sim,
                { -4 + 0.15, 2 },
                0.15,
                b2_dynamicBody, { 5, 5, 255, 255 }),
            new Edge(
                sim,
                { -5, 2 },
                { -2, 0 },
                b2_staticBody, { 255, 100, 50, 255 })));

    std::stringstream ss;
    ss << "fps : " << frame_count_per_second;

    // The game Loop.
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                goto stop_simulation;
            } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                goto stop_simulation;
            }
        }
        // Draw the scene.
        sim.draw();
        // Update the simulation.
        sim.update();
        // Wait.
        //SDL_Delay(1000 / 60);

        time_in_seconds = std::chrono::time_point_cast<std::chrono::seconds>(
            std::chrono::system_clock::now());
        ++frame_count_per_second;
        if (time_in_seconds > prev_time_in_seconds) {
            ss.str("");
            ss << "fps : " << frame_count_per_second;
            frame_count_per_second = 0;
            prev_time_in_seconds   = time_in_seconds;
        }
        stringRGBA(sim.renderer, 0, 0, ss.str().c_str(), 255, 255, 255, 255);

        SDL_SetRenderDrawColor(sim.renderer, 255, 255, 255, 0);
        DrawDottedLine(
            sim.renderer,
            0 + 15, sim.height / 2.f,
            sim.width - 15, sim.height / 2.f);

        sim.render();

        // This part keeps the frame rate.
        std::this_thread::sleep_until(end_frame);
        begin_frame = end_frame;
        end_frame   = begin_frame + fps_limit;
    }

stop_simulation:
    SDL_DestroyRenderer(sim.renderer);
    SDL_DestroyWindow(sim.window);
    SDL_Quit();
    return EXIT_SUCCESS;
}