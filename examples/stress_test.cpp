#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

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

class FPSManager {
private:
    /// The current fps limit.
    std::chrono::nanoseconds fps_limit;
    /// The current number of frames per second.
    unsigned fps;
    /// The time at the initial frame.
    std::chrono::system_clock::time_point begin;
    /// The time at the final frame we need to reach.
    std::chrono::system_clock::time_point end;
    /// The current time in seconds.
    long current_time;
    /// The previous time in seconds.
    long prev_time;

public:
    FPSManager(unsigned max_fps = 60)
        : fps_limit(std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<double>{ 1. / max_fps })),
          fps(0),
          begin(std::chrono::system_clock::now()),
          end(std::chrono::system_clock::now() + fps_limit),
          current_time(),
          prev_time()
    {
    }

    inline void update_fps()
    {
        // Get the current time.
        current_time = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch().count();
        // Increment the number of fps.
        ++fps;
        // Check if the current time in seconds, is greater than the previous
        // time in seconds.
        if (current_time > prev_time) {
            // Reset the FPS count.
            fps = 0;
            // Update the time in seconds.
            prev_time = current_time;
        }
    }

    inline unsigned get_fps() const
    {
        return (fps * 1000) / std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - begin).count();
    }

    inline void wait()
    {
        // This part keeps the frame rate.
        std::this_thread::sleep_until(end);
        // Update the initial time.
        begin = end;
        // Update the final time in the future.
        end = begin + fps_limit;
    }
};

int main(int argc, char *argv[])
{
    FPSManager fps_manager(30);

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

    //the random device that will seed the generator
    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_real_distribution<float> distribution(-3.5f, 3.5f);
    for (int i = 0; i < 1000; ++i) {
        sim.entities.emplace_back(
            new Circle(
                sim,
                { distribution(engine), distribution(engine) },
                0.10,
                b2_dynamicBody, { 5, 5, 255, 255 }));
    }

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

        // Update the FPS.
        fps_manager.update_fps();
        // Print the FPS.
        std::stringstream ss;
        ss << "fps : " << fps_manager.get_fps();
        stringRGBA(sim.renderer, 0, 0, ss.str().c_str(), 255, 255, 255, 255);

        // Render the scene.
        sim.render();

        // Wait and cap the FPS.
        fps_manager.wait();
    }

stop_simulation:
    SDL_DestroyRenderer(sim.renderer);
    SDL_DestroyWindow(sim.window);
    SDL_Quit();
    return EXIT_SUCCESS;
}