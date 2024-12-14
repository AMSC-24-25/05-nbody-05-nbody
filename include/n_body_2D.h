#include "abstract_n_body.hpp"
#include "json.hpp"


template<std::floating_point FP>
class NBody2D : public AbstractNbody<FP> {
public:
    void setup(std::string file_name) override;

    void solve() override;

    void output() override;

private:
    using AbstractNbody<FP>::G;
    std::vector<FP> forces_x;
    std::vector<FP> forces_y;
};