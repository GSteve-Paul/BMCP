#include <BMCP.hpp>
#include <cstdlib>
#include <cstdio>

void BMCP::BMCPSolver::Add_Item(const int item)
{
    solution_size++;
    solution[item] = 1;
    solution_weight_sum += g->weight[item];

    for (int elem_nei: g->item_neighbor[item])
    {
        solution_elements[elem_nei] += 1;
        if (solution_elements[elem_nei] == 1)
        {
            solution_profit_sum += g->profit[elem_nei];
            for (int item_nei: g->element_neighbor[elem_nei])
            {
                if (solution[item_nei]) continue;
                solution_contribution[item_nei] -= g->profit[elem_nei];
            }
        }
        else if (solution_elements[elem_nei] == 2)
        {
            for (int item_nei: g->element_neighbor[elem_nei])
            {
                if (!solution[item_nei]) continue;
                if (item_nei == item) continue;
                solution_contribution[item_nei] -= g->profit[elem_nei];
                break;
            }
        }
    }
}

void BMCP::BMCPSolver::Remove_Item(const int item)
{
    solution_size--;
    solution[item] = 0;
    solution_weight_sum -= g->weight[item];

    for (int elem_nei: g->item_neighbor[item])
    {
        solution_elements[elem_nei]--;
        if (solution_elements[elem_nei] == 0)
        {
            solution_profit_sum -= g->profit[elem_nei];
            for (int item_nei: g->element_neighbor[elem_nei])
            {
                if (item_nei == item) continue;
                solution_contribution[item_nei] += g->profit[elem_nei];
            }
        }
        else if (solution_elements[elem_nei] == 1)
        {
            for (int item_nei: g->element_neighbor[elem_nei])
            {
                if (!solution[item_nei]) continue;
                solution_contribution[item_nei] += g->profit[elem_nei];
                break;
            }
        }
    }
}

void BMCP::BMCPSolver::Add_Item_With_Conf_Change(const int item, const int iter)
{
    solution_size++;
    solution[item] = 1;
    solution_weight_sum += g->weight[item];

    origin_conf_change_in_solution[item] = conf_change_in_solution[item] = solution_contribution[item];
    conf_change_timestamp[item] = iter + rand_deviation(tabu_length1);

    for (int elem_nei: g->item_neighbor[item])
    {
        solution_elements[elem_nei] += 1;
        if (solution_elements[elem_nei] == 1)
        {
            solution_profit_sum += g->profit[elem_nei];
            for (int item_nei: g->element_neighbor[elem_nei])
            {
                if (solution[item_nei]) continue;
                solution_contribution[item_nei] -= g->profit[elem_nei];
                conf_change_out_of_solution[item_nei] -= g->profit[elem_nei];
            }
        }
        else if (solution_elements[elem_nei] == 2)
        {
            for (int item_nei: g->element_neighbor[elem_nei])
            {
                if (!solution[item_nei]) continue;
                if (item_nei == item) continue;
                solution_contribution[item_nei] -= g->profit[elem_nei];
                conf_change_in_solution[item_nei] -= g->profit[elem_nei];
                break;
            }
        }
    }
}

void BMCP::BMCPSolver::Remove_Item_With_Conf_Change(const int item, const int iter)
{
    solution_size--;
    solution[item] = 0;
    solution_weight_sum -= g->weight[item];

    origin_conf_change_out_of_solution[item] = conf_change_out_of_solution[item] = solution_contribution[item];
    conf_change_timestamp[item] = iter + rand_deviation(tabu_length1);

    for (int elem_nei: g->item_neighbor[item])
    {
        solution_elements[elem_nei]--;
        if (solution_elements[elem_nei] == 0)
        {
            solution_profit_sum -= g->profit[elem_nei];
            for (int item_nei: g->element_neighbor[elem_nei])
            {
                if (item_nei == item) continue;
                solution_contribution[item_nei] += g->profit[elem_nei];
                conf_change_out_of_solution[item_nei] -= g->profit[elem_nei];
            }
        }
        else if (solution_elements[elem_nei] == 1)
        {
            for (int item_nei: g->element_neighbor[elem_nei])
            {
                if (!solution[item_nei]) continue;
                solution_contribution[item_nei] += g->profit[elem_nei];
                conf_change_in_solution[item_nei] -= g->profit[elem_nei];
                break;
            }
        }
    }
}

void BMCP::BMCPSolver::Solution_To_Best_Solution()
{
    in_best_solution = in_solution;
    best_solution_profit_sum = solution_profit_sum;
    best_solution_weight_sum = solution_weight_sum;
    best_solution_size = solution_size;
    for (int i = 1; i <= g->m; i++)
    {
        best_solution[i] = solution[i];
        best_solution_contribution[i] = solution_contribution[i];
    }
    for (int i = 1; i <= g->n; i++)
    {
        best_solution_elements[i] = solution_elements[i];
    }
}

void BMCP::BMCPSolver::Solution_To_Star_Solution()
{
    in_star_solution = in_solution;
    star_solution_profit_sum = solution_profit_sum;
    star_solution_weight_sum = solution_weight_sum;
    star_solution_size = solution_size;
    for (int i = 1; i <= g->m; i++)
    {
        star_solution[i] = solution[i];
        star_solution_contribution[i] = solution_contribution[i];
    }
    for (int i = 1; i <= g->n; i++)
    {
        star_solution_elements[i] = solution_elements[i];
    }
}

void BMCP::BMCPSolver::Best_Solution_To_Solution()
{
    in_solution = in_best_solution;
    solution_profit_sum = best_solution_profit_sum;
    solution_weight_sum = best_solution_weight_sum;
    solution_size = best_solution_size;
    for (int i = 1; i <= g->m; i++)
    {
        solution[i] = best_solution[i];
        solution_contribution[i] = best_solution_contribution[i];
    }
    for (int i = 1; i <= g->n; i++)
    {
        solution_elements[i] = best_solution_elements[i];
    }
}

void BMCP::BMCPSolver::Greedy_Initialization()
{
    //init in_solution
    in_solution.clear();
    while (solution_weight_sum <= g->C && solution_size < g->m)
    {
        //select the item with the highest density under the premise that total weight <= C
        int ustar = -1;
        for (int i = 1; i <= g->m; i++)
        {
            if (solution[i]) continue;
            if (g->weight[i] + solution_weight_sum > g->C) continue;
            if (solution_contribution[i] == 0) continue;
            if (ustar == -1)
            {
                ustar = i;
                continue;
            }
            if (solution_contribution[i] * g->weight[ustar] > solution_contribution[ustar] * g->weight[i])
                ustar = i;
        }
        if (ustar != -1)
        {
            Add_Item(ustar);
            //printf("add %d\n",ustar);
            in_solution.insert(ustar);
            for (int i = 0; i < in_solution.size();)
            {
                int item = in_solution[i];
                if (solution_contribution[item] == 0)
                {
                    in_solution.erase(i);
                    Remove_Item(item);
                    //printf("remove %d\n",item);
                }
                else
                    i++;
            }
        }
        else
            break;
    }
}

int BMCP::BMCPSolver::Multiple_Selections(int amount)
{
    if (random_list.size() <= amount)
        return random_list.size() - 1;
    for (int i = 0; i < amount; i++)
    {
        int random_num = rand() % (random_list.size() - i) + i;
        std::swap(random_list[random_num], random_list[i]);
    }
    return amount - 1;
}

double BMCP::BMCPSolver::Upper_Confidence_Bound(int item)
{
    return (double) r_sum[item] / std::max(1, select_times[item]);
}

double BMCP::BMCPSolver::r(int item)
{
    return (double) solution_contribution[item] /
           std::max(solution_weight_sum + g->weight[item] - g->C, 1);
}

void BMCP::BMCPSolver::CC_Search()
{
    Solution_To_Best_Solution();

    for (int i = 1; i <= g->m; i++)
    {
        r_sum[i] = r(i);
        select_times[i] = solution[i];
        if (solution[i])
        {
            origin_conf_change_in_solution[i] = solution_contribution[i];
            conf_change_in_solution[i] = 0;
            conf_change_timestamp[i] = 0;
        }
        else
        {
            origin_conf_change_out_of_solution[i] = solution_contribution[i];
            conf_change_out_of_solution[i] = 0;
            conf_change_timestamp[i] = 0;
        }
    }

    int iter = 0;
    while (iter < Imax1)
    {
        if (solution_weight_sum > g->C)
        {
            //select an item in solution with the lowest density
            int ustar = -1;
            int ustar_idx = -1;
            for (int i = 0; i < in_solution.size(); i++)
            {
                int item = in_solution[i];
                if (conf_change_in_solution[item] > origin_conf_change_in_solution[item] * lambda &&
                    iter < conf_change_timestamp[item])
                    continue;
                if (ustar == -1 || solution_contribution[item] * g->weight[ustar] <
                                   solution_contribution[ustar] * g->weight[item])
                {
                    ustar = item;
                    ustar_idx = i;
                }
            }
            if (ustar != -1)
            {
                in_solution.erase(ustar_idx);
                Remove_Item_With_Conf_Change(ustar, iter);
            }
        }
        //printf("%d %d %d\n", iter, solution_weight_sum, solution_profit_sum);
        //fflush(stdout);
        if (solution_weight_sum <= g->C && solution_profit_sum > best_solution_profit_sum)
        {
            Solution_To_Best_Solution();
        }

        if (solution_weight_sum <= g->C)
        {
            int randnum = rand() % 100;
            if (randnum < 50) // direct select
            {
                random_list.clear();
                for (int i = 1; i <= g->m; i++)
                {
                    if (solution[i]) continue;
                    if (solution_contribution[i] == 0) continue;
                    random_list.push_back(i);
                }
                int idx = Multiple_Selections(15);
                int ustar = -1;
                for (int i = 0; i <= idx; i++)
                {
                    int item = random_list[i];
                    if (ustar == -1 || solution_contribution[item] * g->weight[ustar] >
                                       solution_contribution[ustar] * g->weight[item])
                        ustar = item;
                }
                if (ustar != -1)
                {
                    in_solution.insert(ustar);
                    Add_Item_With_Conf_Change(ustar, iter);
                }
            }
            else
            {
                int ustar = -1;
                double ustar_ucb;
                double tmp_ucb;
                for (int i = 1; i <= g->m; i++)
                {
                    if (solution[i]) continue;
                    if (solution_contribution[i] == 0) continue;
                    if (ustar == -1)
                    {
                        ustar = i;
                        ustar_ucb = Upper_Confidence_Bound(ustar);
                        continue;
                    }
                    if ((tmp_ucb = Upper_Confidence_Bound(i)) > ustar_ucb)
                    {
                        ustar = i;
                        ustar_ucb = tmp_ucb;
                    }
                }
                if (ustar != -1)
                {
                    in_solution.insert(ustar);
                    r_sum[ustar] += r(ustar);
                    select_times[ustar]++;
                    Add_Item_With_Conf_Change(ustar, iter);
                }
            }
        }
        //printf("%d %d %d\n", iter, solution_weight_sum, solution_profit_sum);
        //fflush(stdout);
        if (solution_weight_sum <= g->C && solution_profit_sum > best_solution_profit_sum)
        {
            Solution_To_Best_Solution();
        }
        iter++;
    }
}

void BMCP::BMCPSolver::Deep_Optimize()
{
    Solution_To_Best_Solution();

    //init block_list
    int block_weight_sum = 0;
    for (int i = 1; i <= g->m; i++)
        block_list[i] = 0;

    random_list.clear();
    for (int i = 1; i <= g->n; i++)
    {
        if (solution_elements[i]) continue;
        if (g->element_neighbor[i].empty()) continue;
        random_list.push_back(i);
    }
    int idx = Multiple_Selections(block_list_size);
    for (int i = 0; i <= idx; i++)
    {
        int elem = random_list[i];
        if (solution_elements[elem]) continue;

        int ustar = -1;
        for (int item_nei: g->element_neighbor[elem])
        {
            if (ustar == -1)
            {
                ustar = item_nei;
                continue;
            }
            if (solution_contribution[item_nei] * g->weight[ustar] >
                solution_contribution[ustar] * g->weight[item_nei])
                ustar = item_nei;
        }
        if (ustar != -1)
        {
            if (block_weight_sum + g->weight[ustar] > g->C) continue;
            Add_Item(ustar);
            in_solution.insert(ustar);
            block_list[ustar] = 1;
            block_weight_sum += g->weight[ustar];
        }
    }
    //init tabu
    for (int i = 1; i <= g->m; i++)
    {
        tabu_list[i] = 0;
    }
    int iter = 0;
    while (iter < Imax2)
    {
        if (solution_weight_sum < g->C)
        {
            int ustar = -1;
            for (int i = 1; i <= g->m; i++)
            {
                if (solution[i]) continue;
                if (solution_profit_sum + solution_contribution[i] > best_solution_profit_sum &&
                    solution_weight_sum + g->weight[i] <= g->C)
                {
                    if (ustar == -1 ||
                        solution_contribution[i] * g->weight[ustar] >
                        solution_contribution[ustar] * g->weight[i])
                        ustar = i;
                    continue;
                }
                if (iter < tabu_list[i]) continue;
                if (ustar == -1 ||
                    solution_contribution[i] * g->weight[ustar] >
                    solution_contribution[ustar] * g->weight[i])
                    ustar = i;
            }
            if (ustar != -1)
            {
                Add_Item(ustar);
                in_solution.insert(ustar);
                tabu_list[ustar] = iter + rand_deviation(tabu_length2);
            }
        }
        if (solution_weight_sum <= g->C && solution_profit_sum > best_solution_profit_sum)
        {
            Solution_To_Best_Solution();
        }
        if (solution_weight_sum >= g->C)
        {
            int ustar = -1;
            int ustar_idx;
            for (int i = 0; i < in_solution.size(); i++)
            {
                int item = in_solution[i];
                if (block_list[item]) continue;
                if (iter < tabu_list[item]) continue;
                if (ustar == -1 ||
                    solution_contribution[item] * g->weight[ustar] <
                    solution_contribution[ustar] * g->weight[item])
                {
                    ustar = item;
                    ustar_idx = i;
                }
            }
            if (ustar != -1)
            {
                Remove_Item(ustar);
                in_solution.erase(ustar_idx);
                tabu_list[ustar] = iter + rand_deviation(tabu_length2);
            }
        }
        if (solution_weight_sum <= g->C && solution_profit_sum > best_solution_profit_sum)
        {
            Solution_To_Best_Solution();
        }
        iter++;
    }
}

void BMCP::BMCPSolver::Restart()
{
    int erase_cnt = 0;
    while(erase_cnt < remove_size && solution_weight_sum > 0)
    {
        int ustar = -1;
        int ustar_idx;
        for (int i = 0; i != in_solution.size(); i++)
        {
            int item = in_solution[i];
            if(ustar == -1 || select_times[item] < select_times[ustar])
            {
                ustar = item;
                ustar_idx = i;
            }
        }
        if(ustar != -1)
        {
            in_solution.erase(ustar_idx);
            Remove_Item(ustar);
            erase_cnt++;
        }
    }

}

void BMCP::BMCPSolver::Start_Clock()
{
    start_time = clock();
}

int BMCP::BMCPSolver::Get_Time()
{
    now_time = clock();
    return now_time - start_time;
}

void BMCP::BMCPSolver::Solve()
{
    Start_Clock();
    Greedy_Initialization();
    if (solution_profit_sum > star_solution_profit_sum)
    {
        Solution_To_Star_Solution();
        printf("%lf %d %d\n", 1.0 * Get_Time() / CLOCKS_PER_SEC, total_iterations, star_solution_profit_sum);
    }
    while (Get_Time() < time_limit * CLOCKS_PER_SEC)
    {

        total_iterations++;
        /* ConfChange Search */
        CC_Search();
        Best_Solution_To_Solution();
        if (solution_profit_sum > star_solution_profit_sum)
        {
            Solution_To_Star_Solution();
            printf("%lf %d %d\n", 1.0 * Get_Time() / CLOCKS_PER_SEC, total_iterations, star_solution_profit_sum);
        }
        /* DeepOpt */
        Deep_Optimize();
        Best_Solution_To_Solution();
        if (solution_profit_sum > star_solution_profit_sum)
        {
            Solution_To_Star_Solution();
            printf("%lf %d %d\n", 1.0 * Get_Time() / CLOCKS_PER_SEC, total_iterations, star_solution_profit_sum);
        }
        Restart();
        //printf("%lf %d %d\n", 1.0 * Get_Time() / CLOCKS_PER_SEC, total_iterations, star_solution_profit_sum);
        fflush(stdout);
    }
}

int BMCP::BMCPSolver::rand_deviation(int num)
{
    int tmp1 = std::max(1, (int) (0.5 * num));
    return num + (rand() % tmp1 - tmp1 / 2);
}
