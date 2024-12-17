#ifndef EULER_IMPLICIT_INTEGRATOR_HPP
#define EULER_IMPLICIT_INTEGRATOR_HPP

#include "integrator.hpp"
#include <cmath>
#include <vector>

// Metodo di Eulero implicito
template<std::floating_point FP>
class EulerImplicitIntegrator : public Integrator<FP> {
public:
    // Implementazione del metodo di integrazione
    void integrate(std::vector<Particle<FP>>& particles,
                   const std::vector<std::vector<FP>>& forces,
                   FP delta_t) override {
        for (size_t i = 0; i < particles.size(); ++i) { // Itera su ogni particella
            for (size_t dim = 0; dim < particles[i].pos.size(); ++dim) { // Itera su ogni dimensione
                // Stima iniziale per posizione e velocità
                FP new_pos = particles[i].pos[dim];
                FP new_vel = particles[i].vel[dim];

                // Iterazioni di Newton-Raphson per correzione implicita
                for (size_t iter = 0; iter < max_iterations; ++iter) {
                    // Accelerazione calcolata dalle forze pre-computate
                    FP acceleration = forces[i][dim] / particles[i].mass;

                    // Aggiorna velocità e posizione
                    new_vel = particles[i].vel[dim] + delta_t * acceleration;
                    new_pos = particles[i].pos[dim] + delta_t * new_vel;

                    // Controllo di convergenza
                    if (std::abs(new_pos - particles[i].pos[dim]) < tolerance &&
                        std::abs(new_vel - particles[i].vel[dim]) < tolerance) {
                        break; // Se la convergenza è soddisfatta, esci dall'iterazione
                    }
                }

                // Aggiorna i valori finali di posizione e velocità
                particles[i].pos[dim] = new_pos;
                particles[i].vel[dim] = new_vel;
            }
        }
    }

private:
    static constexpr size_t max_iterations = 10; // Numero massimo di iterazioni Newton-Raphson
    static constexpr FP tolerance = 1e-5;        // Tolleranza per la convergenza
};

#endif // EULER_IMPLICIT_INTEGRATOR_HPP
