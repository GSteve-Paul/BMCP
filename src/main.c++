#include "../include/BMCPSolve.h++"

void Set_Time_Limit(char *ti_lim)
{
    sscanf(ti_lim, "%d", &time_limit);
}

void Set_Random_Seed(char *seed_string)
{
    unsigned int seed;
    sscanf(seed_string, "%u", &seed);
    srand(seed);
}

void Set_Imax(char *arg)
{
    sscanf(arg, "%d", &Imax);
}

void Set_TimeStamp_Gap(char *arg)
{
    sscanf(arg, "%d", &timestamp_gap);
}

void Set_Arms_Count(char *arg)
{
    sscanf(arg, "%d", &arms_count);
}

void Set_Lambda(char *arg)
{
    sscanf(arg, "%d", &lambda);
}

void Set_History_Count(char *arg)
{
    sscanf(arg, "%d", &history_count);
}

void Set_Gamma(char *arg)
{
    sscanf(arg, "%lf", &gamma_value);
}

void Input_From_File(char *file_path)
{
    FILE *file = fopen(file_path, "rb");
    if (file == nullptr)
    {
        printf("fail to open this file %s.\n", file_path);
        exit(1);
    }
    fscanf(file, "\n");
    fscanf(file, "m=%d n=%d knapsack size=%d\n", &m, &n, &C);
    fscanf(file, "\n");
    fscanf(file, "The weight of %*d items_next");
    for (int i = 1; i <= m; i++)
    {
        fscanf(file, "%d", weight + i);
    }
    fscanf(file, "\n");
    fscanf(file, "The profit of %*d elements_next");
    for (int i = 1; i <= n; i++)
    {
        fscanf(file, "%d", profit + i);
    }
    fscanf(file, "\n");
    fscanf(file, "Relation matix");
    int relation;
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            fscanf(file, "%d", &relation);
            if (relation)
            {
                items_neighbor[i].push_back(j);
                elements_neighbor[j].push_back(i);
            }
        }
    }
}

void Input_From_File2(char *file_path)
{
    FILE *file = fopen(file_path, "rb");
    if (file == nullptr)
    {
        printf("fail to open this file %s.\n", file_path);
        exit(1);
    }
    int line;
    fscanf(file, "%d %d %d %d", &m, &n, &line, &C);
    for (int i = 1; i <= line; i++)
    {
        int u, v;
        fscanf(file, "%d %d", &u, &v);
        items_neighbor[u].push_back(v);
        elements_neighbor[v].push_back(u);
    }
    for (int i = 1; i <= m; i++)
    {
        fscanf(file, "%d", weight + i);
    }
    for (int i = 1; i <= n; i++)
    {
        fscanf(file, "%d", profit + i);
    }
}

void Output_Info(char *file_path, char *seed_string)
{
    printf("%s %s %d %d %d %d\n", file_path, seed_string, star_solution_profit_sum, (int) star_solution_time,
           star_solution_iterations, total_iterations);
}

int main(int argc, char *argv[])
{
    /*
     * arg:
     * input_type 2
     * data_file /home/....
     * random_seed 0
     * time_limit 600
     * Imax 100000
     * timestamp_gap 3
     * arms_count 20
     * lambda 1000
     * history_count 20
     * gamma 0.9
     *
     */
    int input_type;
    for(int i=1;i<argc;i+=2)
    {
        char* arg = argv[i] + 2;
        if(strcmp(arg,"input_type")==0) sscanf(argv[i+1], "%d", &input_type);
        else if(strcmp(arg,"data_file")==0)
        {
            if (input_type == 1)
                Input_From_File(argv[i+1]);
            else
                Input_From_File2(argv[i+1]);
        }
        else if(strcmp(arg,"seed")==0) Set_Random_Seed(argv[i+1]);
        else if(strcmp(arg,"time_limit")==0) Set_Time_Limit(argv[i+1]);
        else if(strcmp(arg,"imax")==0) Set_Imax(argv[i+1]);
        else if(strcmp(arg,"timestamp_gap")==0) Set_TimeStamp_Gap(argv[i+1]);
        else if(strcmp(arg,"arms_count")==0) Set_Arms_Count(argv[i+1]);
        else if(strcmp(arg,"lambda")==0) Set_Lambda(argv[i+1]);
        else if(strcmp(arg,"history_count")==0) Set_History_Count(argv[i+1]);
        else if(strcmp(arg,"gamma")==0) Set_Gamma(argv[i+1]);
    }
    Solve();
    Output_Info(argv[2], argv[3]);
    //system("pause");
}
