#ifndef BANDBMCP_BMCP_HPP
#define BANDBMCP_BMCP_HPP

#include <IntList.hpp>
#include <Graph.hpp>
#include <SetList.hpp>
#include <QueueList.hpp>
#include <ctime>
#include <random>

namespace BMCP
{
    using ll = long long;

    class BMCPSolver
    {
    private:
        Graph *g;

        //random list
        IntList random_list;

        //solution
        int solution_weight_sum;
        int solution_profit_sum;
        int *solution;
        int solution_size;
        int *solution_contribution;
        int *solution_elements;
        SetList in_solution;

        //best_solution
        int best_solution_weight_sum;
        int best_solution_profit_sum;
        int *best_solution;
        int best_solution_size;
        int *best_solution_contribution;
        int *best_solution_elements;
        SetList in_best_solution;

        //Set Time
        clock_t start_time;
        clock_t now_time;
        int time_limit;

        void Start_Clock();

        int Get_Time();

        //update solution
        void Add_Item(int);

        void Remove_Item(int);

        void Add_Item_With_Conf_Change(int, int);

        void Remove_Item_With_Conf_Change(int, int);

        static int rand_deviation(int);

        //transmit
        void Solution_To_Best_Solution();

        void Solution_To_Star_Solution();

        void Best_Solution_To_Solution();

        //initialize greedily
        void Greedy_Initialization();

        //local search (cc)
        int Imax1;
        int *conf_change_out_of_solution;
        int *origin_conf_change_out_of_solution;
        int *conf_change_in_solution;
        int *origin_conf_change_in_solution;
        int *conf_change_timestamp;
        int tabu_length1 = 5;
        double lambda;

        //multi-armed bandit
        double *r_sum;
        int *select_times;

        double r(int);

        double Upper_Confidence_Bound(int);

        int Multiple_Selections(int);

        void CC_Search();

        //deep-optimization
        int Imax2;
        int *block_list;
        int block_list_size;
        int *tabu_list;
        int tabu_length2 = 5;
        double gamma = 1;
        int bandit_count;

        void Deep_Optimize();

        //restart
        int remove_size = 5;

        void Restart();
    public:
        //star_solution
        int star_solution_weight_sum;
        int star_solution_profit_sum;
        int *star_solution;
        int star_solution_size;
        int *star_solution_contribution;
        int *star_solution_elements;
        SetList in_star_solution;
        clock_t star_solution_time;
        int star_solution_iterations;
        int total_iterations;

        //test info
        unsigned int seed;

        BMCPSolver(Graph *g, unsigned int seed, int time_limit,
                   int Imax1, int Imax2, double lambda);

        void Solve();

        void Output() const;

        ~BMCPSolver();
    };
}

#endif //BANDBMCP_BMCP_HPP
