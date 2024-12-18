#ifndef TEAM_05_NBODY_NBODY_ND_HPP
#define TEAM_05_NBODY_NBODY_ND_HPP

#include "abstract_n_body.hpp"
#include "integrator.hpp"
#include "../third_party/json.hpp"
#include <memory>
#include <iostream>
#include <fstream>

// Classe generica N-Body per dimensioni dinamiche
template<std::floating_point FP>
class NBodyND : public AbstractNbody<FP> {
public:
    NBodyND(std::unique_ptr<Integrator<FP>> integrator, size_t dimensions)
            : AbstractNbody<FP>(0, 0.0), integrator(std::move(integrator)), Dim(dimensions) {}

    void setup(std::string file_name) override;

    void solve() override;

protected:
    void output(size_t step) override;

private:
    std::unique_ptr<Integrator<FP>> integrator;
    size_t Dim; // Numero dinamico di dimensioni
    std::vector<std::vector<FP>> forces; // Forze dinamiche per ogni particella e dimensione

    void compute_forces();
};

// Metodo setup
// Metodo setup
template<std::floating_point FP>
void NBodyND<FP>::setup(std::string file_name) {
    std::ifstream input(file_name);
    if (!input.is_open()) throw std::runtime_error("Error: Unable to open file " + file_name);

    nlohmann::json data;
    input >> data;

    this->N = data["N"];
    this->delta_t = data["delta_t"];

    // Inizializza particelle
    this->particles.clear();
    for (auto &p: data["particles"]) {
        FP mass = p["mass"];
        std::vector<FP> position = p["position"];
        std::vector<FP> velocity = p["velocity"];
        this->particles.emplace_back(mass, position, velocity);
    }

    // Inizializza forze
    forces.resize(this->N, std::vector<FP>(Dim, 0.0));
    input.close();

    // Popola il vettore this->time con gli step temporali
    FP current_time = 0.0;
    FP max_time = data["max_time"];
    while (current_time <= max_time) { // Simula fino al tempo data["max_time"]
        this->time.push_back(current_time);
        current_time += this->delta_t;
    }
}


// Metodo compute_forces
template<std::floating_point FP>
void NBodyND<FP>::compute_forces() {
    for (auto &force: forces) {
        std::fill(force.begin(), force.end(), 0.0);
    }

    for (size_t i = 0; i < this->N; ++i) {
        for (size_t j = 0; j < this->N; ++j) {
            if (i == j) continue;

            FP dist_squared = 0.0;
            std::vector<FP> diff(Dim);

            for (size_t d = 0; d < Dim; ++d) {
                diff[d] = this->particles[j].pos[d] - this->particles[i].pos[d];
                dist_squared += diff[d] * diff[d];
            }

            FP dist = std::sqrt(dist_squared) + 1e-5;
            FP force_mag = (AbstractNbody<FP>::G * this->particles[i].mass * this->particles[j].mass) / dist_squared;

            for (size_t d = 0; d < Dim; ++d) {
                forces[i][d] += force_mag * (diff[d] / dist);
            }
        }
    }
}

// Metodo solve
template<std::floating_point FP>
void NBodyND<FP>::solve() {
    std::cout << "Starting simulation with " << this->N << " particles and delta_t = " << this->delta_t << "\n";

    for (size_t step = 0; step < this->time.size(); ++step) {
        std::cout << "Step " << step + 1 << "/" << this->time.size() << "...\n";
        compute_forces(); // Calcola le forze

        // Log delle forze calcolate
        for (size_t i = 0; i < this->N; ++i) {
            std::cout << "Particle " << i << " forces: ";
            for (size_t d = 0; d < Dim; ++d) {
                std::cout << forces[i][d] << " ";
            }
            std::cout << "\n";
        }

        integrator->integrate(this->particles, forces, this->delta_t); // Passa forze all'integratore

        // Log delle posizioni aggiornate
        for (size_t i = 0; i < this->N; ++i) {
            std::cout << "Particle " << i << " state: " << this->particles[i] << "\n";
        }

        output(step);
    }
}

// Metodo output
template<std::floating_point FP>
void NBodyND<FP>::output(size_t step) {
    std::ostringstream timestep_filename_stream;
    timestep_filename_stream << this->output_filename_prefix << std::setfill('0') << std::setw(5) << step << ".csv";
    const std::string timestep_filename = timestep_filename_stream.str();

    std::ofstream output_file(timestep_filename);
    if (!output_file.is_open()) {
        throw std::runtime_error("Error: Unable to open the output file!");
    }

    std::cout << "Writing output to '" << timestep_filename << "'...\n";

    // Scrive l'header dinamico
    output_file << "t";
    for (size_t d = 0; d < Dim; ++d) output_file << ",x" << d;
    for (size_t d = 0; d < Dim; ++d) output_file << ",v" << d;
    output_file << ",m\n";

    // Scrive i dati per ogni particella
    for (const auto &particle: this->particles) {
        output_file << this->time[step]; // Tempo
        for (size_t d = 0; d < Dim; ++d) output_file << "," << particle.pos[d];
        for (size_t d = 0; d < Dim; ++d) output_file << "," << particle.vel[d];
        output_file << "," << particle.mass << "\n";
    }

    output_file.close();
    std::cout << "Output written to '" << timestep_filename << "'\n";
}


#endif // TEAM_05_NBODY_NBODY_ND_HPP