#include <BMCP.hpp>
#include <cstdio>

void BMCP::BMCPSolver::Output() const
{
    //printf("%d %d %lf %d %d", seed, star_solution_profit_sum, 1.0 * star_solution_time / CLOCKS_PER_SEC,
    //       star_solution_iterations, total_iterations);
}

BMCP::BMCPSolver::~BMCPSolver()
{
    delete[] solution;
    delete[] solution_contribution;
    delete[] solution_elements;

    delete[] best_solution;
    delete[] best_solution_contribution;
    delete[] best_solution_elements;

    delete[] star_solution;
    delete[] star_solution_contribution;
    delete[] star_solution_elements;

    delete[] conf_change_out_of_solution;
    delete[] origin_conf_change_out_of_solution;
    delete[] conf_change_in_solution;
    delete[] origin_conf_change_in_solution;
    delete[] conf_change_timestamp;

    delete[] r_sum;
    delete[] select_times;

    delete[] block_list;
    delete[] tabu_list;
}

