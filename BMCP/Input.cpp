#include <BMCP.hpp>

BMCP::BMCPSolver::BMCPSolver(Graph *g, unsigned int seed, int time_limit, int Imax1,
                             int Imax2, double lambda) :
        g(g), seed(seed), time_limit(time_limit), Imax1(Imax1), Imax2(Imax2), lambda(lambda)
{
    //rand engine
    //linear_rand = new std::minstd_rand(seed);
    srand(this->seed);

    //distribute memory
    solution = new int[g->m + 1];
    solution_contribution = new int[g->m + 1];
    solution_elements = new int[g->n + 1];

    best_solution = new int[g->m + 1];
    best_solution_contribution = new int[g->m + 1];
    best_solution_elements = new int[g->n + 1];

    star_solution = new int[g->m + 1];
    star_solution_contribution = new int[g->m + 1];
    star_solution_elements = new int[g->n + 1];

    conf_change_out_of_solution = new int[g->m + 1];
    origin_conf_change_out_of_solution = new int[g->m + 1];
    conf_change_in_solution = new int[g->m + 1];
    origin_conf_change_in_solution = new int[g->m + 1];
    conf_change_timestamp = new int[g->m + 1];

    block_list = new int[g->m + 1];
    r_sum = new double[g->m + 1];
    select_times = new int[g->m + 1];


    //init star_solution
    total_iterations = 0;
    star_solution_iterations = 0;
    star_solution_profit_sum = 0;
    star_solution_weight_sum = 0;
    star_solution_size = 0;
    for (int i = 1; i <= g->m; i++)
    {
        star_solution_contribution[i] = 0;
        for (int nei: g->item_neighbor[i])
        {
            star_solution_contribution[i] += g->profit[nei];
        }
        star_solution[i] = 0;
    }
    for (int i = 1; i <= g->n; i++)
    {
        star_solution_elements[i] = 0;
    }

    //init solution
    solution_weight_sum = 0;
    solution_profit_sum = 0;
    solution_size = 0;
    for (int i = 1; i <= g->m; i++)
    {
        solution_contribution[i] = 0;
        for (int nei: g->item_neighbor[i])
        {
            solution_contribution[i] += g->profit[nei];
        }
        solution[i] = 0;
    }
    for (int i = 1; i <= g->n; i++)
    {
        solution_elements[i] = 0;
    }

    //init deep-opt
    ll avg_weight = 0;
    block_list = new int[g->m + 1];
    for (int i = 1; i <= g->m; i++)
    {
        block_list[i] = 0;
        avg_weight += g->weight[i];
    }
    block_list_size = std::max(1, (int) ceil(g->C * g->m / avg_weight * 0.1));
    tabu_list = new int[g->m + 1];
}