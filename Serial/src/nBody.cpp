#include "nBody.hpp"

class Nbody : public AbstractNbody<Particle> {
public:
    void setup (unsigned int N) override {
        // da dove prendiamo i dati delle particelle (masse, posizioni iniziali, velocità iniziali)?
    }

    void solve () override {
        const double G = -6.673e-11;
        for (unsigned int i = 0; i < time.size(); i++) {
            for (unsigned int q = 0; q < N; q++)
            {
                for (unsigned int k = q + 1; k < N; k++)
                {
                    double x_diff = particles[q].pos[0] - particles[k].pos[0];
                    double y_diff = particles[q].pos[1] - particles[k].pos[1];
                    double dist = sqrt(x_diff * x_diff + y_diff * y_diff);
                    double dist_cube = dist * dist * dist;

                    double force_qk_x = G * particles[q].mass * particles[k].mass / dist_cube * x_diff;
                    double force_qk_y = G * particles[q].mass * particles[k].mass / dist_cube * y_diff;

                    forces_x[q] += force_qk_x;
                    forces_x[k] -= force_qk_x;
                    forces_y[q] += force_qk_y;
                    forces_y[k] -= force_qk_y;
                }
            }

            for (unsigned int j = 0; j < N; j++) {
                particles[j].pos[0] = particles[j].pos[0] + delta_t * particles[j].vel[0];
                particles[j].pos[1] = particles[j].pos[1] + delta_t * particles[j].vel[1];

                particles[j].vel[0] = particles[j].vel[0] + delta_t * forces_x[j] / particles[j].mass;
                particles[j].vel[1] = particles[j].vel[1] + delta_t * forces_y[j] / particles[j].mass;

                // output
            }

        }
        
        
    }

    void output () override {

    }
};