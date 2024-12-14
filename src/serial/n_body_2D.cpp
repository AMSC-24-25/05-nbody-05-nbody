#include <iostream>
#include "n_body_2D.h"


template<std::floating_point FP>
void NBody2D<FP>::setup(std::string file_name) {
    std::ifstream file(file_name); // Replace with your JSON file path

    if (!file.is_open()) {
        std::cerr << "Unable to open the file!" << std::endl;
    }

    nlohmann::json jsonData;
    file >> jsonData;
    this->delta_t = jsonData["delta_t"];

    for(auto& particleJson : jsonData["particles"]) {
        Particle<FP> particle{
            particleJson["mass"],
            {particleJson["pos"]["x"], particleJson["pos"]["y"]},
            {particleJson["vel"]["x"], particleJson["vel"]["y"]}
        };
        this->particles.push_back(particle);
    }
    this->N = this->particles.size();

    double max_time = jsonData["max_time"];
    double current_time = 0;
    while(current_time<max_time) {
        this->time.push_back(current_time);
        current_time += this->delta_t;
    }
    for (const auto& particle : this->particles) {
        std::cout << "Mass: " << particle.mass
                  << ", Position: (" << particle.pos[0] << ", " << particle.pos[1] << ")"
                  << ", Velocity: (" << particle.vel[0] << ", " << particle.vel[1] << ")"
        << std::endl;
    }
}

template<std::floating_point FP>
void NBody2D<FP>::solve() {
    for (unsigned int i = 0; i < this->time.size(); i++) {
        for (unsigned int q = 0; q < this->N; q++) {
            for (unsigned int k = q + 1; k < this->N; k++) {
                FP x_diff = this->particles[q].pos[0] - this->particles[k].pos[0];
                FP y_diff = this->particles[q].pos[1] - this->particles[k].pos[1];
                FP dist = sqrt(x_diff * x_diff + y_diff * y_diff);
                FP dist_cube = dist * dist * dist;

                FP force_qk_x = G * this->particles[q].mass * this->particles[k].mass / dist_cube * x_diff;
                FP force_qk_y = G * this->particles[q].mass * this->particles[k].mass / dist_cube * y_diff;

                this->forces_x[q] += force_qk_x;
                this->forces_x[k] -= force_qk_x;
                this->forces_y[q] += force_qk_y;
                this->forces_y[k] -= force_qk_y;
            }
        }

        for (unsigned int j = 0; j < this->N; j++) {
            this->particles[j].pos[0] += this->delta_t * this->particles[j].vel[0];
            this->particles[j].pos[1] += this->delta_t * this->particles[j].vel[1];

            this->particles[j].vel[0] += this->delta_t * this->forces_x[j] / this->particles[j].mass;
            this->particles[j].vel[1] += this->delta_t * this->forces_y[j] / this->particles[j].mass;

            // output
        }
    }
}

template<std::floating_point FP>
void NBody2D<FP>::output() {
    //TODO to Implement
}

template class NBody2D<double>;