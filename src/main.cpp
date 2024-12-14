#include <n_body_2D.h>

int main(int argc, char *argv[]) {
    NBody2D<double> nbody;

    nbody.setup("../data/input.json");

    return 0;
}