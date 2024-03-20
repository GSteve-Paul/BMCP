#include <cstring>
#include <BMCP.hpp>

BMCP::Graph *g;
BMCP::BMCPSolver *solver;

unsigned int seed;
int time_limit;
int Imax1;
int Imax2;
int timestamp_gap;
double lambda;
int m, n, C;

void Set_Time_Limit(char *ti_lim)
{
    sscanf(ti_lim, "%d", &time_limit);
}

void Set_Random_Seed(char *seed_string)
{
    sscanf(seed_string, "%u", &seed);
}

void Set_Imax1(char *arg)
{
    sscanf(arg, "%d", &Imax1);
}

void Set_Imax2(char *arg)
{
    sscanf(arg, "%d", &Imax2);
}

void Set_Lambda(char *arg)
{
    sscanf(arg, "%lf", &lambda);
}

void Input_From_File1(char *file_path)
{
    FILE *file = fopen(file_path, "rb");
    if (file == nullptr)
    {
        fprintf(stderr, "fail to open this file %s\n", file_path);
        exit(1);
    }
    fscanf(file, "\n");
    fscanf(file, "m=%d n=%d knapsack size=%d\n", &m, &n, &C);
    g = new BMCP::Graph(m, n, C);
    fscanf(file, "\n");
    fscanf(file, "The weight of %*d items_next");
    for (int i = 1; i <= g->m; i++)
    {
        fscanf(file, "%d", g->weight + i);
    }
    fscanf(file, "\n");
    fscanf(file, "The profit of %*d elements_next");
    for (int i = 1; i <= g->n; i++)
    {
        fscanf(file, "%d", g->profit + i);
    }
    fscanf(file, "\n");
    fscanf(file, "Relation matix");
    int relation;
    for (int i = 1; i <= g->m; i++)
    {
        for (int j = 1; j <= g->n; j++)
        {
            fscanf(file, "%d", &relation);
            if (relation)
            {
                g->item_neighbor[i].push_back(j);
                g->element_neighbor[j].push_back(i);
            }
        }
    }
}

void Input_From_File2(char *file_path)
{
    FILE *file = fopen(file_path, "rb");
    if (file == nullptr)
    {
        fprintf(stderr, "fail to open this file %s\n", file_path);
        exit(1);
    }
    int line;
    fscanf(file, "%d %d %d %d", &m, &n, &line, &C);
    g = new BMCP::Graph(m, n, C);
    for (int i = 1; i <= line; i++)
    {
        int u, v;
        fscanf(file, "%d %d", &u, &v);
        g->item_neighbor[u].push_back(v);
        g->element_neighbor[v].push_back(u);
    }
    for (int i = 1; i <= m; i++)
    {
        fscanf(file, "%d", g->weight + i);
    }
    for (int i = 1; i <= n; i++)
    {
        fscanf(file, "%d", g->profit + i);
    }
}

int main(int argc, char *argv[])
{
    char *arg;
    char *file_path = nullptr;
    int input_type = -1;
    for (int i = 1; i < argc; i += 2)
    {
        arg = argv[i] + 2;
        if (strcmp(arg, "input_type") == 0)
            sscanf(argv[i + 1], "%d", &input_type);
    }
    if (input_type != 1 && input_type != 2)
    {
        fputs("Unknown or invalid input_type!\n", stderr);
        return -1;
    }
    for (int i = 1; i < argc; i += 2)
    {
        arg = argv[i] + 2;
        if (strcmp(arg, "data_file") == 0)
            file_path = argv[i + 1];
    }
    if (file_path == nullptr)
    {
        fputs("Unknown or invalid file_path!\n", stderr);
        return 1;
    }
    if (input_type == 1)
        Input_From_File1(file_path);
    else if (input_type == 2)
        Input_From_File2(file_path);
    for (int i = 1; i < argc; i += 2)
    {
        arg = argv[i] + 2;
        if (strcmp(arg, "seed") == 0)
        {
            Set_Random_Seed(argv[i + 1]);
        }
        else if (strcmp(arg, "time_limit") == 0) Set_Time_Limit(argv[i + 1]);
        else if (strcmp(arg, "imax1") == 0) Set_Imax1(argv[i + 1]);
        //else if (strcmp(arg, "tabu_length1") == 0) Set_Timestamp_Gap(argv[i + 1]);
        else if (strcmp(arg, "lambda") == 0) Set_Lambda(argv[i + 1]);
        else if (strcmp(arg, "imax2") == 0) Set_Imax2(argv[i + 1]);
    }
    solver = new BMCP::BMCPSolver(g, seed, time_limit, Imax1, Imax2,
                                  lambda);
    solver->Solve();

    solver->Output();

    delete solver;

    delete g;
    return 0;
}
