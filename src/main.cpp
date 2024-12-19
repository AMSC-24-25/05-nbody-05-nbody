#include "n_body_2D.hpp"
#include "../third_party/json.hpp"
#include <fstream>

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <dimensions>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string input_file = argv[1];

    // Crea il sistema NBody2D
    NBody2D<double> nbody;

    // Setup, solve e output
    nbody.setup(input_file);
    nbody.solve();

    return EXIT_SUCCESS;
}
