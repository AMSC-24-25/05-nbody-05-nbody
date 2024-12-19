#ifndef TEAM_05_NBODY_NBODY_2D_HPP
#define TEAM_05_NBODY_NBODY_2D_HPP

#include "abstract_n_body.hpp"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <memory>

using json = nlohmann::json;

template<std::floating_point FP>
class NBody2D : public AbstractNbody<FP> {
public:
    NBody2D()
            : AbstractNbody<FP>(0, 0.0) {}

    void setup(std::string file_name) override;

    void solve() override;

protected:
    void output(size_t step) override;

private:
    std::vector<std::vector<FP>> forces; // Forze dinamiche per ogni particella (x, y)

    void compute_forces();

    void euler_explicit();

    FP calculate_total_energy();
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
    this->t_max = data["max_time"];

    // Inizializza particelle
    this->particles.clear();
    for (auto &p: data["particles"]) {
        FP mass = p["mass"];
        std::vector<FP> position = p["position"];
        std::vector<FP> velocity = p["velocity"];
        this->particles.emplace_back(mass, position, velocity);
    }

    // Inizializza forze
    forces.resize(this->N, std::vector<FP>(2, 0.0)); // Forze x e y

    // Inizializza tempo
    this->time.push_back(0.0);
    for (FP t = this->delta_t; t <= this->t_max; t += this->delta_t) {
        this->time.push_back(t);
    }

    input.close();
}

// Metodo solve: Usa l'integratore
template<std::floating_point FP>
void NBody2D<FP>::solve() {

    FP initial_energy = calculate_total_energy();

    for (size_t step = 0; step < this->time.size(); ++step) {
        compute_forces();
        euler_explicit();

        FP current_energy = calculate_total_energy();
        FP relative_change = std::abs((current_energy - initial_energy) / initial_energy);

        std::cout << "Step " << step << ": Energy = " << current_energy
                  << ", Relative change = " << relative_change << std::endl;

        output(step);
    }
}

// Metodo per calcolare le forze
template<std::floating_point FP>
void NBody2D<FP>::compute_forces() {
    for (auto &force: forces) {
        std::fill(force.begin(), force.end(), 0.0);
    }

    for (size_t q = 0; q < this->N; ++q) {
        for (size_t k = q + 1; k < this->N; ++k) {

            FP dx = this->particles[q].pos[0] - this->particles[k].pos[0];
            FP dy = this->particles[q].pos[1] - this->particles[k].pos[1];
            FP dist = std::sqrt(dx * dx + dy * dy);
            if (dist == 0)
                continue;
            FP dist_cubed = dist * dist * dist;

            FP force_x = (AbstractNbody<FP>::G * this->particles[q].mass * this->particles[k].mass) / dist_cubed * dx;
            FP force_y = (AbstractNbody<FP>::G * this->particles[q].mass * this->particles[k].mass) / dist_cubed * dy;

            forces[q][0] -= force_x;
            forces[q][1] -= force_y;
            forces[k][0] += force_x;
            forces[k][1] += force_y;
        }
    }
}

// Metodo eulero esplicito
template<std::floating_point FP>
void NBody2D<FP>::euler_explicit() {
    for (unsigned int j = 0; j < this->N; ++j) {
        this->particles[j].pos[0] += this->delta_t * this->particles[j].vel[0];
        this->particles[j].pos[1] += this->delta_t * this->particles[j].vel[1];

        this->particles[j].vel[0] += this->delta_t * this->forces[j][0] / this->particles[j].mass;
        this->particles[j].vel[1] += this->delta_t * this->forces[j][1] / this->particles[j].mass;
    }
}

template<std::floating_point FP>
FP NBody2D<FP>::calculate_total_energy() {
    FP kinetic_energy = 0;
    FP potential_energy = 0;

    // Energia cinetica
    for (auto &particle: this->particles) {
        FP speed_squared = particle.vel[0] * particle.vel[0] + particle.vel[1] * particle.vel[1];

        kinetic_energy += 0.5 * particle.mass * speed_squared;
    }

    // Energia potenziale
    for (size_t i = 0; i < this->N; ++i) {
        for (size_t j = i + 1; j < this->N; ++j) {
            FP dx = this->particles[j].pos[0] - this->particles[i].pos[0];
            FP dy = this->particles[j].pos[1] - this->particles[i].pos[1];
            FP distance = std::sqrt(dx * dx + dy * dy);

            potential_energy -= (this->G * this->particles[i].mass * this->particles[j].mass) / distance;
        }
    }

    // Energia totale
    return kinetic_energy + potential_energy;
}

// Metodo output: Stampa i risultati
template<std::floating_point FP>
void NBody2D<FP>::output(size_t step) {
    std::ostringstream timestep_filename_stream;
    timestep_filename_stream << this->output_filename_prefix << std::setfill('0') << std::setw(5) << step << ".csv";
    const std::string timestep_filename = timestep_filename_stream.str();

    std::ofstream output_file(timestep_filename);
    if (!output_file.is_open()) {
        throw std::runtime_error("Error: Unable to open the output file!");
    }

    std::cout << "Writing output to '" << timestep_filename << "'...\n";

    // Scrive l'header dinamico
    output_file << "t,x0,x1,v0,v1,m\n";

    // Scrive i dati per ogni particella
    for (const auto &particle: this->particles) {
        output_file << this->time[step]; // Tempo
        output_file << "," << particle.pos[0] << "," << particle.pos[1];
        output_file << "," << particle.vel[0] << "," << particle.vel[1];
        output_file << "," << particle.mass << "\n";
    }

    output_file.close();
    std::cout << "Output written to '" << timestep_filename << "'\n";
}

#endif // TEAM_05_NBODY_NBODY_2D_HPP