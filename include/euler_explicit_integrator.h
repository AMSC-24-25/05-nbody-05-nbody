#ifndef EULER_EXPLICIT_INTEGRATOR_HPP
#define EULER_EXPLICIT_INTEGRATOR_HPP

#include "integrator.hpp"

// Implementazione del metodo di Eulero esplicito
template<std::floating_point FP>
class EulerExplicitIntegrator : public Integrator<FP> {
    void integrate(std::vector<Particle<FP>>& particles, const std::vector<std::vector<FP>>& forces, FP delta_t) override {
        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t d = 0; d < particles[i].pos.size(); ++d) {
                // Aggiorna velocità usando le forze
                particles[i].vel[d] += delta_t * forces[i][d] / particles[i].mass;
                // Aggiorna posizione usando la velocità aggiornata
                particles[i].pos[d] += delta_t * particles[i].vel[d];
            }
        }
    }

private:
    // Forze applicate a ogni particella (ipotizziamo che siano calcolate prima)
    std::vector<FP> forces;
};

#endif // EULER_EXPLICIT_INTEGRATOR_HPP
