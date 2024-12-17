#ifndef TEAM_05_NBODY_NBODY_2D_HPP
#define TEAM_05_NBODY_NBODY_2D_HPP

#include "abstract_n_body.hpp"
#include "integrator.hpp"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <memory>

using json = nlohmann::json;

template<std::floating_point FP>
class NBody2D : public AbstractNbody<FP> {
public:
    explicit NBody2D(std::unique_ptr<Integrator<FP>> integrator)
        : AbstractNbody<FP>(0, 0.0), integrator(std::move(integrator)) {}

    void setup(std::string file_name) override;
    void solve() override;
    void output() override;

private:
    std::unique_ptr<Integrator<FP>> integrator; // Integratore scelto
    std::vector<std::vector<FP>> forces;        // Forze dinamiche per ogni particella (x, y)

    void compute_forces();
};

// Metodo setup: Lettura dei dati da file JSON
template<std::floating_point FP>
void NBody2D<FP>::setup(std::string file_name) {
    std::ifstream input(file_name);
    if (!input.is_open()) {
        throw std::runtime_error("Error: Unable to open file " + file_name);
    }

    // Legge il file JSON
    json data;
    input >> data;

    this->N = data["N"];
    this->delta_t = data["delta_t"];

    // Inizializza particelle
    this->particles.clear();
    for (auto& p : data["particles"]) {
        FP mass = p["mass"];
        std::vector<FP> position = p["position"];
        std::vector<FP> velocity = p["velocity"];
        this->particles.emplace_back(mass, position, velocity);
    }

    // Inizializza forze
    forces.resize(this->N, std::vector<FP>(2, 0.0)); // Forze x e y

    input.close();
}

// Metodo solve: Usa l'integratore
template<std::floating_point FP>
void NBody2D<FP>::solve() {
    for (size_t step = 0; step < this->time.size(); ++step) {
        compute_forces();
        integrator->integrate(this->particles, forces, this->delta_t); // Passa le forze
    }
}

// Metodo per calcolare le forze
template<std::floating_point FP>
void NBody2D<FP>::compute_forces() {
    for (auto& force : forces) {
        std::fill(force.begin(), force.end(), 0.0);
    }

    for (size_t i = 0; i < this->N; ++i) {
        for (size_t j = 0; j < this->N; ++j) {
            if (i == j) continue;

            FP dx = this->particles[j].pos[0] - this->particles[i].pos[0];
            FP dy = this->particles[j].pos[1] - this->particles[i].pos[1];
            FP dist_squared = dx * dx + dy * dy + 1e-5;
            FP dist = std::sqrt(dist_squared);
            FP force_mag = (AbstractNbody<FP>::G * this->particles[i].mass * this->particles[j].mass) / dist_squared;

            forces[i][0] += force_mag * (dx / dist); // Forza x
            forces[i][1] += force_mag * (dy / dist); // Forza y
        }
    }
}

// Metodo output: Stampa i risultati
template<std::floating_point FP>
void NBody2D<FP>::output() {
    std::cout << "Final positions of particles:\n";
    for (const auto& particle : this->particles) {
        std::cout << "Mass: " << particle.mass
                  << ", Position: (" << particle.pos[0] << ", " << particle.pos[1] << ")\n";
    }
}

#endif // TEAM_05_NBODY_NBODY_2D_HPP
