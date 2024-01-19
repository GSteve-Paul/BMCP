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
int best_solution_profit_sum;
int star_solution_weight_sum;
int star_solution_profit_sum;
int star_solution[maxm];
int star_solution_elements[maxn];
int star_solution_size;
int star_solution_contribution[maxm];


//O(|Vi| * |Ve|)
void Add_Vertex(int itemIdx)
{
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
int Random_Select_Several_Items(int max_num, int iter)
{
    random_list.clear();
    for (int i = 1; i <= m; i++)
    {
        if (solution[i]) continue;
        //conf_change_out_of_solution[i]/origin_conf_change_out_of_solution[i] < random / 100 (random = 1 ~ 100)
        if (conf_change_out_of_solution[i] * 10 < origin_conf_change_out_of_solution[i] * 9 ||
            iter - conf_change_timestamp[i] >= timestamp_gap)
            random_list.push(i);
    }
    if (random_list.size <= max_num) return random_list.size - 1;
    for (int i = 0; i < max_num; i++)
    {
        int random_num = rand() % (random_list.size - i) + i;
        //swap i and random_num
        std::swap(random_list[random_num], random_list[i]);
    }
    //from 0 to max_num - 1
    return max_num - 1;
}

int Imax = 100000;
int conf_change_out_of_solution[maxm];
int origin_conf_change_out_of_solution[maxm];
int conf_change_in_solution[maxm];
int origin_conf_change_in_solution[maxm];
int conf_change_timestamp[maxm];
int timestamp_gap = 5;

void CC_Search()
{
    //Sb <- S
    Solution_To_Best_Solution();
    //initialize conf_change_out_of_solution
    for (int i = 1; i <= m; i++)
    {
        conf_change_out_of_solution[i] = 0;
        origin_conf_change_out_of_solution[i] = 1;
        conf_change_in_solution[i] = 1;
        origin_conf_change_in_solution[i] = 0;
        conf_change_timestamp[i] = -1;
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
            if (solution_profit_sum > star_solution_profit_sum) star_solution_time = Get_Time();
            Solution_To_Best_Solution();
        }
        //randomly select up to 50 items and then select the best one with the highest dd
        if (solution_weight_sum <= C)
        {
            int idx = Random_Select_Several_Items(50, iter);
            int ustar = -1;
            for (int i = 0; i <= idx; i++)
            {
                if (ustar == -1)
                {
                    ustar = random_list[i];
                    continue;
                }
                if (solution_contribution[random_list[i]] * weight[ustar] >
                    solution_contribution[ustar] * weight[random_list[i]])
                {
                    ustar = random_list[i];
                }
            }
            if (ustar != -1)
            {
                Add_Vertex_With_Conf_Change(ustar, iter);
                //printf("add vertex %d\n", ustar);
            }
        }
        if (solution_weight_sum <= C && solution_profit_sum > best_solution_profit_sum)
        {
            if (solution_profit_sum > star_solution_profit_sum) star_solution_time = Get_Time();
            Solution_To_Best_Solution();
        }
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
clock_t star_solution_time;
int time_limit = 600;//second

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
    Solution_To_Star_Solution();
    while (Get_Time() < time_limit * CLOCKS_PER_SEC)
    {
        CC_Search();
        Best_Solution_To_Solution();
        if (solution_profit_sum > star_solution_profit_sum)
        {
            Solution_To_Star_Solution();
        }
        //printf("cc %d\n",solution_profit_sum);
        Deep_Optimize();
        //printf("deepopt %d\n",solution_profit_sum);
    }
}