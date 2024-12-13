#include "nBody.hpp"
#include <concepts>

template<std::floating_point FP>
class Nbody : public AbstractNbody<FP> {
public:
    void setup(unsigned int N) override {
        // da dove prendiamo i dati delle particelle (masse, posizioni iniziali, velocità iniziali)?
    }

    void solve() override {
        using AbstractNbody<FP>::G;

        for (unsigned int i = 0; i < this->time.size(); i++) {
            for (unsigned int q = 0; q < this->N; q++) {
                for (unsigned int k = q + 1; k < this->N; k++) {
                    FP x_diff = this->particles[q].pos[0] - this->particles[k].pos[0];
                    FP y_diff = this->particles[q].pos[1] - this->particles[k].pos[1];
                    FP dist = sqrt(x_diff * x_diff + y_diff * y_diff);
                    FP dist_cube = dist * dist * dist;

                    FP force_qk_x = G * this->particles[q].mass * this->particles[k].mass / dist_cube * x_diff;
                    FP force_qk_y = G * this->particles[q].mass * this->particles[k].mass / dist_cube * y_diff;

                    this->forces_x[q] += force_qk_x;
                    this->forces_x[k] -= force_qk_x;
                    this->forces_y[q] += force_qk_y;
                    this->forces_y[k] -= force_qk_y;
                }
            }

            for (unsigned int j = 0; j < this->N; j++) {
                this->particles[j].pos[0] += this->delta_t * this->particles[j].vel[0];
                this->particles[j].pos[1] += this->delta_t * this->particles[j].vel[1];

                this->particles[j].vel[0] += this->delta_t * this->forces_x[j] / this->particles[j].mass;
                this->particles[j].vel[1] += this->delta_t * this->forces_y[j] / this->particles[j].mass;

                // output
            }

        }


    }

    void output() override {

    }
};