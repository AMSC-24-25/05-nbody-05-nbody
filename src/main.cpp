#include "../include/n_body_nd.h"
#include "../include/euler_explicit_integrator.h"
#include "../include/euler_implicit_integrator.h"
#include "../third_party/json.hpp"
#include <fstream>

double read_delta_t(const std::string &input_file) {
    std::ifstream input(input_file);
    if (!input.is_open()) {
        throw std::runtime_error("Error: Unable to open file " + input_file);
    }

    nlohmann::json data;
    input >> data;
    input.close();

    return data["delta_t"];
}

int main(int argc, char *argv[]) {
    try {
        if (argc < 3) {
            std::cerr << "Usage: " << argv[0] << " <input_file> <dimensions>" << std::endl;
            return EXIT_FAILURE;
        }

        std::string input_file = argv[1];
        size_t dimensions = std::stoul(argv[2]);

        // Leggi delta_t dal file JSON
        double delta_t = read_delta_t(input_file);

        // Seleziona automaticamente l'integratore
        std::unique_ptr<Integrator<double>> integrator;
        if (delta_t <= 0.01) { // Soglia arbitraria per stabilità
            std::cout << "Using explicit Euler integrator (delta_t = " << delta_t << ")\n";
            integrator = std::make_unique<EulerExplicitIntegrator<double>>();
        } else {
            std::cout << "Using implicit Euler integrator (delta_t = " << delta_t << ")\n";
            integrator = std::make_unique<EulerImplicitIntegrator<double>>();
        }

        // Crea il sistema NBodyND
        NBodyND<double> nbody(std::move(integrator), dimensions);

        // Setup, solve e output
        nbody.setup(input_file);
        nbody.solve();

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
