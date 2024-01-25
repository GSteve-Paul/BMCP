#ifndef BMCPSolve_H
#define BMCPSolve_H

#include <vector>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <cassert>
#include "mList.h++"

extern const int maxn;
extern const int maxm;
extern const int inf;

//limitation of the knapsack
extern int C;

//size of items
extern int m;
//weight of items
extern int weight[];

//size of elements
extern int n;
//profit of elements
extern int profit[];

//adjacent list
extern std::vector<int> items_neighbor[];
extern std::vector<int> elements_neighbor[];

//random list
extern mList random_list;

extern int solution_weight_sum;
extern int solution_profit_sum;
extern int solution[];
extern int solution_size;
extern int solution_contribution[];
extern int solution_elements[];

extern int best_solution[];
extern int best_solution_size;
extern int best_solution_weight_sum;
extern int best_solution_contribution[];
extern int best_solution_elements[];
extern int best_solution_profit_sum;

extern int star_solution_weight_sum;
extern int star_solution_profit_sum;
extern int star_solution[];
extern int star_solution_elements[];
extern int star_solution_size;
extern int star_solution_contribution[];

/*
 * Edit_Solution
 */
void Add_Vertex(int);

void Remove_Vertex(int);

void Add_Vertex_With_Conf_Change(int, int);

void Remove_Vertex_With_Conf_Change(int, int);

/*
 * Transmit
 */

void Solution_To_Best_Solution();

void Solution_To_Star_Solution();

void Best_Solution_To_Solution();

void Star_Solution_To_Solution();

/*
 * Initialization
 */

void Initialization();

/*
 * Greedy_Initialization
 */

void Greedy_Initialization();

/*
 * CC_Search
 */

extern int Imax;
extern int conf_change_out_of_solution[];
extern int origin_conf_change_out_of_solution[];
extern int conf_change_in_solution[];
extern int origin_conf_change_in_solution[];
extern int conf_change_timestamp[];
extern int timestamp_gap;

extern int arms_count;
extern double estimated_element_select_value[];
extern int element_select_times[];
extern int local_optima_count;
extern int lambda;
extern int history[];
extern int history_count;
extern double gamma_value;
extern double gamma_exp[];

double r(int);

double Upper_Confidence_Bound(int);

int Random_Select_Several_Elements_To_Add_In();

void CC_Search();

/*
 * Deep Optimization
 */

extern int block_list[];
extern int block_list_size;

int Random_Select_Several_Elements(int);

void Deep_Optimize();


/*
 * Solve
 */

extern clock_t start_time;
extern clock_t now_time;
extern int time_limit;
extern int total_iterations;

extern clock_t star_solution_time;
extern int star_solution_iterations;

void Start_Clock();

int Get_Time();

void Solve();

#endif