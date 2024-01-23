#include "../include/BMCPSolve.h++"

using ll = long long;

const int maxm = 1e6 + 5;
const int maxn = 1e6 + 5;
const int inf = INT32_MAX;
int C;
int m;
int weight[maxm];
int n;
int profit[maxn];
std::vector<int> items_neighbor[maxm];
std::vector<int> elements_neighbor[maxn];
mList random_list;
int solution_weight_sum;
int solution_profit_sum;
int solution[maxm];
int solution_size;
int solution_contribution[maxm];
int solution_elements[maxn];
int best_solution[maxm];
int best_solution_size;
int best_solution_weight_sum;
int best_solution_contribution[maxm];
int best_solution_elements[maxn];
int best_solution_profit_sum = -1;
int star_solution_weight_sum;
int star_solution_profit_sum = -1;
int star_solution[maxm];
int star_solution_elements[maxn];
int star_solution_size;
int star_solution_contribution[maxm];


//O(|Vi| * |Ve|)
void Add_Vertex(int itemIdx)
{
    //assert(solution[itemIdx] == 0);
    solution_size++;
    solution[itemIdx] = 1;
    solution_weight_sum += weight[itemIdx];
    //attention! solution_contribution[itemIdx] will not change!
    for (int elementIdx: items_neighbor[itemIdx])
    {
        solution_elements[elementIdx] += 1;
        if (solution_elements[elementIdx] == 1)
        {
            //add the profit sum
            solution_profit_sum += profit[elementIdx];
            //decrease the contribution (for those vertices out of solution and cover this element)
            //maybe there are multiple vertices
            for (int itemNei: elements_neighbor[elementIdx])
            {
                if (solution[itemNei]) continue;
                if (itemNei == itemIdx) continue;
                solution_contribution[itemNei] -= profit[elementIdx];
            }
        }
        if (solution_elements[elementIdx] == 2)
        {
            //decrease the contribution (for those vertices in solution and cover this element
            //in fact, there must be only exactly one vertex
            for (int itemNei: elements_neighbor[elementIdx])
            {
                if (!solution[itemNei]) continue;
                if (itemNei == itemIdx) continue;
                solution_contribution[itemNei] -= profit[elementIdx];
            }
        }
    }
}

void Remove_Vertex(int itemIdx)
{
    //assert(solution[itemIdx] == 1);
    solution_size--;
    solution[itemIdx] = 0;
    solution_weight_sum -= weight[itemIdx];
    //attention! solution_contribution[itemIdx] will not change!
    for (int elementIdx: items_neighbor[itemIdx])
    {
        solution_elements[elementIdx]--;
        if (solution_elements[elementIdx] == 0)
        {
            solution_profit_sum -= profit[elementIdx];
            //increase the contribution (for those vertices out of solution and cover this element)
            for (int itemNei: elements_neighbor[elementIdx])
            {
                if (solution[itemNei]) continue;
                if (itemNei == itemIdx) continue;
                solution_contribution[itemNei] += profit[elementIdx];
            }
        }
        if (solution_elements[elementIdx] == 1)
        {
            //increase the contribution (for those vertices in solution and cover this element)
            for (int itemNei: elements_neighbor[elementIdx])
            {
                if (!solution[itemNei]) continue;
                if (itemNei == itemIdx) continue;
                solution_contribution[itemNei] += profit[elementIdx];
            }
        }
    }
}


//O(|Vi| * |Ve|)
void Add_Vertex_With_Conf_Change(int itemIdx, int iter)
{
    //assert(solution[itemIdx] == 0);
    solution_size++;
    solution[itemIdx] = 1;
    solution_weight_sum += weight[itemIdx];
    //attention! solution_contribution[itemIdx] will not change!
    conf_change_in_solution[itemIdx] = 0;
    for (int elementIdx: items_neighbor[itemIdx])
    {
        solution_elements[elementIdx] += 1;
        if (solution_elements[elementIdx] == 1)
        {
            //add the profit sum
            solution_profit_sum += profit[elementIdx];
            //decrease the contribution (for those vertices out of solution and cover this element)
            //maybe there are multiple vertices

            //decrease the conf_change_out_of_solution (for vertices which are out of solution and cover this element)
            for (int itemNei: elements_neighbor[elementIdx])
            {
                if (solution[itemNei]) continue;
                if (itemNei == itemIdx) continue;
                solution_contribution[itemNei] -= profit[elementIdx];
                conf_change_out_of_solution[itemNei]--;
            }

            //increase the conf_change_out_of_solution of itemIdx
            conf_change_in_solution[itemIdx]++;
        }
        if (solution_elements[elementIdx] == 2)
        {
            //decrease the contribution (for those vertices in solution and cover this element
            //in fact, there must be only exactly one vertex
            for (int itemNei: elements_neighbor[elementIdx])
            {
                if (!solution[itemNei]) continue;
                if (itemNei == itemIdx) continue;
                solution_contribution[itemNei] -= profit[elementIdx];
                break;
            }
        }
    }
    origin_conf_change_in_solution[itemIdx] = conf_change_in_solution[itemIdx];
    conf_change_timestamp[itemIdx] = iter;
}

void Remove_Vertex_With_Conf_Change(int itemIdx, int iter)
{
    //assert(solution[itemIdx] == 1);
    solution_size--;
    solution[itemIdx] = 0;
    solution_weight_sum -= weight[itemIdx];
    //attention! solution_contribution[itemIdx] will not be updated!
    //update contribution, profit sum, conf_change_out_of_solution value and so on.
    conf_change_out_of_solution[itemIdx] = 0;
    for (int elementIdx: items_neighbor[itemIdx])
    {
        solution_elements[elementIdx]--;
        if (solution_elements[elementIdx] == 0)
        {
            solution_profit_sum -= profit[elementIdx];
            //increase the contribution (for those vertices out of solution and cover this element)
            for (int itemNei: elements_neighbor[elementIdx])
            {
                if (solution[itemNei]) continue;
                if (itemNei == itemIdx) continue;
                solution_contribution[itemNei] += profit[elementIdx];
            }
            //increase the conf_change_out_of_solution value for itemIdx
            conf_change_out_of_solution[itemIdx]++;
        }
        if (solution_elements[elementIdx] == 1)
        {
            //increase the conf_change_in_solution (for those vertices in solution and cover this element)
            //increase the contribution (for those vertices in solution and cover this element)
            for (int itemNei: elements_neighbor[elementIdx])
            {
                if (!solution[itemNei]) continue;
                if (itemNei == itemIdx) continue;
                solution_contribution[itemNei] += profit[elementIdx];
                conf_change_in_solution[itemNei]++;
                break;
            }
        }
    }
    origin_conf_change_out_of_solution[itemIdx] = conf_change_out_of_solution[itemIdx];
    conf_change_timestamp[itemIdx] = iter;
}


//O(m + n)
void Solution_To_Best_Solution()
{
    best_solution_profit_sum = solution_profit_sum;
    best_solution_weight_sum = solution_weight_sum;
    best_solution_size = solution_size;
    for (int i = 1; i <= m; i++)
    {
        best_solution[i] = solution[i];
        best_solution_contribution[i] = solution_contribution[i];
    }
    for (int i = 1; i <= n; i++)
    {
        best_solution_elements[i] = solution_elements[i];
    }
}

void Solution_To_Star_Solution()
{
    star_solution_profit_sum = solution_profit_sum;
    star_solution_weight_sum = solution_weight_sum;
    star_solution_size = solution_size;
    for (int i = 1; i <= m; i++)
    {
        star_solution[i] = solution[i];
        star_solution_contribution[i] = solution_contribution[i];
    }
    for (int i = 1; i <= n; i++)
    {
        star_solution_elements[i] = solution_elements[i];
    }
}

void Best_Solution_To_Solution()
{
    solution_profit_sum = best_solution_profit_sum;
    solution_weight_sum = best_solution_weight_sum;
    solution_size = best_solution_size;
    for (int i = 1; i <= m; i++)
    {
        solution[i] = best_solution[i];
        solution_contribution[i] = best_solution_contribution[i];
    }
    for (int i = 1; i <= n; i++)
    {
        solution_elements[i] = best_solution_elements[i];
    }
}

void Star_Solution_To_Solution()
{
    solution_profit_sum = star_solution_profit_sum;
    solution_weight_sum = star_solution_weight_sum;
    solution_size = star_solution_size;
    for (int i = 1; i <= m; i++)
    {
        solution[i] = star_solution[i];
        solution_contribution[i] = star_solution_contribution[i];
    }
    for (int i = 1; i <= n; i++)
    {
        solution_elements[i] = star_solution_elements[i];
    }
}

void Initialization()
{
    //initialize solution
    ll avg_weight = 0;
    solution_weight_sum = 0;
    solution_profit_sum = 0;
    solution_size = 0;
    for (int i = 1; i <= m; i++)
    {
        solution_contribution[i] = 0;
        for (int nei: items_neighbor[i])
        {
            solution_contribution[i] += profit[nei];
        }
        solution[i] = 0;
        block_list[i] = 0;
        avg_weight += weight[i];
    }
    avg_weight /= m;
    block_list_size = std::max(1, int(C / avg_weight * 0.1));
    for (int i = 1; i <= n; i++)
    {
        solution_elements[i] = 0;
    }
    //initialize gamma_value
    gamma_exp[0] = gamma_value;
    for (int i = 1; i <= history_count; i++)
    {
        gamma_exp[i] = gamma_exp[i - 1] * gamma_value;
    }
}

void Greedy_Initialization()
{
    while (solution_weight_sum <= C && solution_size < m)
    {
        //greedy select the item with the highest dynamic_density
        //contribution[i]/weight[i] >= contribution[ustar]/weight[ustar]
        int ustar = -1;
        for (int i = 1; i <= m; i++)
        {
            if (solution[i]) continue;
            if (ustar == -1)
            {
                ustar = i;
                continue;
            }
            if (solution_contribution[i] * weight[ustar] > solution_contribution[ustar] * weight[i])
            {
                ustar = i;
            }
        }
        if (solution_weight_sum + weight[ustar] <= C)
        {
            Add_Vertex(ustar);
        }
        else
        {
            break;
        }
    }
}

//select items from unselected item set.
int Random_Select_Several_Elements_To_Add_In()
{
    random_list.clear();
    for (int i = 1; i <= n; i++)
    {
        if (solution_elements[i]) continue;
        random_list.push(i);
    }
    if (random_list.size <= arms_count) return random_list.size - 1;
    for (int i = 0; i < arms_count; i++)
    {
        int random_num = rand() % (random_list.size - i) + i;
        //swap i and random_num
        std::swap(random_list[random_num], random_list[i]);
    }
    //from 0 to max_num - 1
    return arms_count - 1;
}

int Imax = 100000;
int conf_change_out_of_solution[maxm];
int origin_conf_change_out_of_solution[maxm];
int conf_change_in_solution[maxm];
int origin_conf_change_in_solution[maxm];
int conf_change_timestamp[maxm];
int timestamp_gap = 3;

int arms_count = 20;
double estimated_element_select_value[maxn];
int element_select_times[maxn];
int local_optima_count = 1;
int lambda = 1;
int history[maxn];
int history_count = 20;

double gamma_value = 0.9;
double gamma_exp[1001];

double Upper_Confidence_Bound(int elementIdx)
{
    return estimated_element_select_value[elementIdx] +
           lambda * sqrt(log(local_optima_count) / (element_select_times[elementIdx] + 1.0));
}

double r(int itemIdx)
{
    int cost_before = solution_profit_sum;
    int cost_new = solution_profit_sum + solution_contribution[itemIdx];
    int cost_before_best = best_solution_profit_sum;
    return (cost_before - cost_new) / (cost_before - cost_before_best + 1.0);
}

void CC_Search()
{
    //Sb <- S
    Solution_To_Best_Solution();
    //initialize conf_change value
    for (int i = 1; i <= m; i++)
    {
        conf_change_out_of_solution[i] = 0;
        origin_conf_change_out_of_solution[i] = 1;
        conf_change_in_solution[i] = 1;
        origin_conf_change_in_solution[i] = 0;
        conf_change_timestamp[i] = -1;
    }
    //initialize estimated_element_select_value and element_select_times
    for (int i = 1; i <= n; i++)
    {
        estimated_element_select_value[i] = 1;
        element_select_times[i] = 0;
    }
    //initialize history
    for (int i = 1; i <= history_count; i++)
    {
        history[i] = -1;
    }
    int iter = 0;
    while (iter < Imax)
    {
        if (solution_weight_sum > C)
        {
            //select an item in solution with the lowest dd, and then remove it
            //contribution[i]/weight[i] < contribution[ustar]/weight[ustar]
            int ustar = -1;
            for (int i = 1; i <= m; i++)
            {
                if (!solution[i]) continue;
                if (block_list[i]) continue;
                if (conf_change_in_solution[i] == origin_conf_change_in_solution[i] &&
                    iter - conf_change_timestamp[i] <= timestamp_gap)
                    continue;
                if (ustar == -1)
                {
                    ustar = i;
                    continue;
                }
                if (solution_contribution[i] * weight[ustar] < solution_contribution[ustar] * weight[i])
                {
                    ustar = i;
                }
            }
            if (ustar != -1)
            {
                Remove_Vertex_With_Conf_Change(ustar, iter);
                //printf("remove vertex %d\n", ustar);
            }

        }
        if (solution_weight_sum <= C && solution_profit_sum > best_solution_profit_sum)
        {
            //printf("profit %d\n", best_solution_profit_sum);
            if(solution_profit_sum > star_solution_profit_sum) star_solution_time = Get_Time();
            Solution_To_Best_Solution();
        }
        //randomly select up to arms_count elements and then find out the best element named ustar by Upper_Confidence_Bound
        if (solution_weight_sum <= C)
        {
            int idx = Random_Select_Several_Elements_To_Add_In();
            int ustar = -1;
            double ustar_upper_confidence_bound;
            double temp_upper_confidence_bound;
            for (int i = 0; i <= idx; i++)
            {
                if (ustar == -1)
                {
                    ustar = random_list[i];
                    ustar_upper_confidence_bound = Upper_Confidence_Bound(ustar);
                    continue;
                }
                if ((temp_upper_confidence_bound = Upper_Confidence_Bound(random_list[i])) >
                    ustar_upper_confidence_bound)
                {
                    ustar = random_list[i];
                    ustar_upper_confidence_bound = temp_upper_confidence_bound;
                }
            }
            if (ustar == -1) continue;
            //find the best item which can cover the element and then select the best one named vstar by r value
            int vstar = -1;
            double vstar_r;
            double temp_r;
            for (int item_nei: elements_neighbor[ustar])
            {
                if (solution[item_nei]) continue;
                if (conf_change_out_of_solution[item_nei] == origin_conf_change_out_of_solution[item_nei] &&
                    iter - conf_change_timestamp[item_nei] <= timestamp_gap)
                {
                    continue;
                }
                if (vstar == -1)
                {
                    vstar = item_nei;
                    vstar_r = r(vstar);
                    continue;
                }
                if ((temp_r = r(item_nei)) > vstar_r)
                {
                    vstar = item_nei;
                    vstar_r = temp_r;
                }
            }
            if (vstar == -1) continue;
            //add vertex
            //printf("add item %d, element %d\n",vstar,ustar);
            element_select_times[ustar]++;
            Add_Vertex_With_Conf_Change(vstar, iter);
            //update the estimated value of the elements in history
            for (int i = 1; i <= history_count; i++)
            {
                if (history[i] == -1) continue;
                estimated_element_select_value[history[i]] += gamma_exp[history_count - i] * vstar_r;
            }
            //add ustar into the history
            for (int i = history_count; i >= 1; i--)
            {
                history[i + 1] = history[i];
            }
            history[1] = ustar;
        }
        if (solution_weight_sum <= C && solution_profit_sum > best_solution_profit_sum)
        {
            //printf("profit %d\n", best_solution_profit_sum);
            if(solution_profit_sum > star_solution_profit_sum) star_solution_time = Get_Time();
            Solution_To_Best_Solution();
        }
        bool get_into_local_optima = true;
        for (int i = 1; i <= m; i++)
        {
            if (solution[i]) continue;
            if (solution_weight_sum + weight[i] <= C && solution_contribution[i] > 0)
            {
                get_into_local_optima = false;
                break;
            }
        }
        if (get_into_local_optima) local_optima_count++;
        iter++;
        //printf("%d\n", solution_profit_sum);
    }
}

int block_list[maxm];
//  C/avg_cost * 0.1
int block_list_size;

int Random_Select_Several_Elements(int max_num)
{
    random_list.clear();
    for (int i = 1; i <= n; i++)
    {
        if (solution_elements[i]) continue;
        random_list.push(i);
    }
    if (random_list.size <= max_num) return random_list.size - 1;
    for (int i = 0; i < max_num; i++)
    {
        int random_num = rand() % (random_list.size - i) + i;
        std::swap(random_list[random_num], random_list[i]);
    }
    return max_num - 1;
}

void Deep_Optimize()
{
    int block_weight_sum = 0;
    for (int i = 1; i <= m; i++)
        block_list[i] = 0;
    int num = Random_Select_Several_Elements(block_list_size);
    for (int i = 0; i <= num; i++)
    {
        int elementIdx = random_list[i];
        if (solution_elements[elementIdx]) continue;
        //find ustar
        //contribution[item_nei]/weight[item_nei] > contribution[ustar]/weight[ustar]
        int ustar = -1;
        for (int item_nei: elements_neighbor[elementIdx])
        {
            if (ustar == -1)
            {
                ustar = item_nei;
                continue;
            }
            if (solution_contribution[item_nei] * weight[ustar] > solution_contribution[ustar] * weight[item_nei])
                ustar = item_nei;
        }
        if (ustar != -1)
        {
            if (block_weight_sum + weight[ustar] > C) continue;
            //printf("%d ",ustar);
            Add_Vertex(ustar);
            block_list[ustar] = 1;
            block_weight_sum += weight[ustar];
        }
    }
    while (solution_weight_sum > C)
    {
        int ustar = -1;
        for (int i = 1; i <= m; i++)
        {
            if (!solution[i]) continue;
            if (block_list[i]) continue;
            if (ustar == -1)
            {
                ustar = i;
                continue;
            }
            if (solution_contribution[i] * weight[ustar] < solution_contribution[ustar] * weight[i])
            {
                ustar = i;
            }
        }
        if (ustar != -1)
            Remove_Vertex(ustar);
    }
}

clock_t start_time;
clock_t now_time;
clock_t star_solution_time = -1;
int time_limit = 30;//second

void Start_Clock()
{
    start_time = clock();
}

int Get_Time()
{
    now_time = clock();
    return now_time - start_time;
}

void Solve()
{
    Start_Clock();
    Initialization();
    Greedy_Initialization();
//    star_solution_time = Get_Time();
//    Solution_To_Star_Solution();
    while (Get_Time() < time_limit * CLOCKS_PER_SEC)
    {
        if (solution_profit_sum > star_solution_profit_sum)
        {
            Solution_To_Star_Solution();
            star_solution_time = Get_Time();
        }
        //printf("profit %d\n", solution_profit_sum);
        CC_Search();
        Best_Solution_To_Solution();
        if (solution_profit_sum > star_solution_profit_sum)
        {
            Solution_To_Star_Solution();
        }
        //printf("cc %d\n",solution_profit_sum);
        Deep_Optimize();
        //printf("deepopt %d\n",solution_profit_sum);
        //printf("time %d\n", Get_Time());
    }
}