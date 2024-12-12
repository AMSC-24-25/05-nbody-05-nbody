#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

struct Particle {
    const double mass;
    std::vector<double> pos;
    std::vector<double> vel;
};

template <typename T>
class AbstractNbody {
public:
    virtual ~AbstractNbody() {}

    virtual void setup (unsigned int N) = 0;

    virtual void solve () = 0;

    virtual void output () = 0;

protected:
    unsigned int const N;
    unsigned double const delta_t;
    std::vector<T> particles;
    std::vector<double> forces_x;
    std::vector<double> forces_y;
    std::vector<double> time;
};