#ifndef TEAM_05_NBODY_NBODY_H
#define TEAM_05_NBODY_NBODY_H

#include <fstream>
#include <vector>
#include <cmath>
#include <stdexcept>

template<std::floating_point FP>
struct Particle {
    FP mass; // Rimuovi 'const' per permettere il costruttore predefinito
    std::vector<FP> pos;
    std::vector<FP> vel;

    // Costruttore predefinito
    Particle() : mass(0.0), pos(), vel() {}

    // Costruttore parametrizzato
    Particle(FP m, const std::vector<FP>& p, const std::vector<FP>& v)
        : mass(m), pos(p), vel(v) {
        if (p.size() != v.size()) {
            throw std::invalid_argument("Position and velocity vectors must have the same size");
        }
    }
};

template<std::floating_point FP>
class AbstractNbody {
public:
    virtual ~AbstractNbody() = default;

    virtual void setup(std::string file_name) = 0;

    virtual void solve() = 0;

    virtual void output() = 0;

protected:
    AbstractNbody(unsigned int num_particles, FP dt)
        : N(num_particles), delta_t(dt), particles(num_particles) {}

    static constexpr FP G = 6.673e-11;

    unsigned int N = 0;
    FP delta_t = 0.0;
    std::vector<Particle<FP>> particles;
    std::vector<FP> time;

    // Utils
    //Calcola la distanza tra particelle
    FP distance(const Particle<FP>& a, const Particle<FP>& b) const {
        FP sum = 0.0;
        for (size_t i = 0; i < a.pos.size(); ++i) {
            FP diff = a.pos[i] - b.pos[i];
            sum += diff * diff;
        }
        return std::sqrt(sum);
    }

    //Resetta le forze a 0
    void reset_forces(std::vector<FP>& forces) {
        std::fill(forces.begin(), forces.end(), 0.0);
    }
};

#endif //TEAM_05_NBODY_NBODY_H