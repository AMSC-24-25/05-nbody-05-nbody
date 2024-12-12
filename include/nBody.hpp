#ifndef TEAM_05_NBODY_NBODY_H
#define TEAM_05_NBODY_NBODY_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <concepts>

template<std::floating_point FP>
struct Particle {
    const FP mass;
    std::vector<FP> pos;
    std::vector<FP> vel;
};

template<std::floating_point FP>
class AbstractNbody {
public:
    virtual ~AbstractNbody() = default;

    virtual void setup(unsigned int N) = 0;

    virtual void solve() = 0;

    virtual void output() = 0;

protected:
    static constexpr FP G = 6.673e-11;

    const unsigned int N;
    const FP delta_t;
    std::vector<Particle<FP>> particles;
    std::vector<FP> forces_x;
    std::vector<FP> forces_y;
    std::vector<FP> time;
};

#endif //TEAM_05_NBODY_NBODY_H