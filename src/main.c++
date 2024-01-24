#include "../include/BMCPSolve.h++"

void Set_Random_Seed(char *seed_string)
{
    unsigned int seed;
    sscanf(seed_string, "%u", &seed);
    srand(seed);
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
    printf("%s %s %d %d\n", file_path, seed_string, star_solution_profit_sum, (int) star_solution_time);
}

int main(int argc, char *argv[])
{
    Input_From_File2(argv[1]);
    Set_Random_Seed(argv[2]);
    Solve();
    Output_Info(argv[1], argv[2]);
    //system("pause");
}
//
