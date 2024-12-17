// Interfaccia astratta per tutti gli integratori numerici (es. Eulero, Newton).
// Ogni integratore implementa un metodo per aggiornare le particelle.

#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "abstract_n_body.hpp"

template<std::floating_point FP>
class Integrator {
public:
    virtual ~Integrator() = default;
    virtual void integrate(std::vector<Particle<FP>>& particles,
                           const std::vector<std::vector<FP>>& forces,
                           FP delta_t) = 0;
};

#endif //INTEGRATOR_H

