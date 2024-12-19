#ifndef TEAM_05_NBODY_NBODY_H
#define TEAM_05_NBODY_NBODY_H

#include <fstream>
#include <vector>
#include <cmath>
#include <stdexcept>

#define DEF_OUTPUT_FILENAME_PREFIX "./output/nbody-"

template<std::floating_point FP>
struct Particle {
    FP mass; // Rimuovi 'const' per permettere il costruttore predefinito
    std::vector<FP> pos;
    std::vector<FP> vel;

    // Costruttore predefinito
    Particle() : mass(0.0), pos(), vel() {}

    // Costruttore parametrizzato
    Particle(FP m, const std::vector<FP> &p, const std::vector<FP> &v)
            : mass(m), pos(p), vel(v) {
        if (p.size() != v.size()) {
            throw std::invalid_argument("Position and velocity vectors must have the same size");
        }
    }

    friend std::ostream &operator<<(std::ostream &stream, const Particle &particle) {
        stream << "m=" << particle.mass << " ";
        for (size_t d = 0; d < particle.pos.size(); ++d) stream << "x" << d << "=" << particle.pos[d] << " ";
        for (size_t d = 0; d < particle.vel.size(); ++d) stream << "v" << d << "=" << particle.vel[d] << " ";
        return stream;
    }
};

template<std::floating_point FP>
class AbstractNbody {
public:
    virtual ~AbstractNbody() = default;

    virtual void setup(std::string file_name) = 0;

    virtual void solve() = 0;

protected:
    AbstractNbody(unsigned int num_particles, FP dt)
            : N(num_particles), delta_t(dt), particles(num_particles) {}

    virtual void output(size_t step) = 0;

    static constexpr FP G = 6.673e-11;

    unsigned int N = 0;
    FP delta_t = 0.0;
    FP t_max = 0.0;
    std::vector<Particle<FP>> particles;
    std::vector<FP> time;

    std::string output_filename_prefix = DEF_OUTPUT_FILENAME_PREFIX;

};

#endif //TEAM_05_NBODY_NBODY_H