#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* GLOBAL VARIABLES */
int c_size, s_size, faults, fault_flag, alg_option;
int *cache, *stream;
const char *algorithms[] =
    {
        "OPTIMAL",
        "FIFO",
        "LRU",
        "CLOCK"};

void init()
{
    c_size = 0;
    s_size = 0;
    faults = 0;
    fault_flag = 0;
    alg_option = -1;
    stream = (int *)malloc(25 * sizeof(int));
}

void init_cache()
{
    cache = (int *)malloc(c_size * sizeof(int));
    for (int i = 0; i < c_size; i++)
    {
        cache[i] = -1;
    }
}

/* UTILITY FUNCTIONS */

int in_cache(int x)
{
    for (int i = 0; i < c_size; i++)
    {
        if (cache[i] == x)
        {
            return 1;
        }
    }
    return 0;
}

int empty_index()
{
    for (int i = 0; i < c_size; i++)
    {
        if (cache[i] == -1)
            return i;
    }
    return -1;
}

void print_state(int input)
{
    if (fault_flag)
    {
        if (input / 10 > 0)
            printf("%d F   ", input);
        else
            printf("0%d F   ", input);
    }
    else
    {
        if (input / 10 > 0)
            printf("%d     ", input);
        else
            printf("0%d     ", input);
    }
    for (int i = 0; i < c_size; i++)
    {
        if (cache[i] != -1)
        {
            if (input / 10 > 0)
                printf("%d ", cache[i]);
            else
                printf("0%d ", cache[i]);
        }
    }
    printf("\n");
}

/* OPTIMAL ALGORITHM */

int ejection(int index)
{
    int farthest = index, ejected = -1;

    for (int i = 0; i < c_size; i++)
    {
        int j;
        for (j = index; j < s_size; j++)
        {
            if (stream[j] == cache[i])
            {
                if (j > farthest)
                {
                    farthest = j;
                    ejected = i;
                }
                break;
            }
        }

        if (j == s_size)
        {
            return i;
        }
    }

    return (ejected == -1) ? 0 : ejected;
}

void optimal_replacement()
{
    int input;

    for (int i = 0; i < s_size; i++)
    {
        input = empty_index();
        fault_flag = 0;
        // if input == -1, no empty spaces in cache
        if (input == -1)
        {
            if (!in_cache(stream[i]))
            {
                faults++;
                fault_flag = 1;
                int replacement_index = ejection(i);
                cache[replacement_index] = stream[i];
            }
        }
        else
        {
            if (!in_cache(stream[i]))
            {
                cache[input] = stream[i];
            }
        }
        print_state(stream[i]);
    }
}

/* FIFO ALGORITHM */

void fifo_replacement()
{
    int input;
    int pointer = 0;

    for (int i = 0; i < s_size; i++)
    {
        input = empty_index();
        fault_flag = 0;
        // if input == -1, no empty spaces in cache
        if (input == -1)
        {
            if (!in_cache(stream[i]))
            {
                faults++;
                fault_flag = 1;
                cache[pointer] = stream[i];
                pointer = (pointer + 1) % c_size;
            }
        }
        else
        {
            if (!in_cache(stream[i]))
            {
                cache[input] = stream[i];
            }
        }
        print_state(stream[i]);
    }
}

/* LRU ALGORITHM */

int LRU(int *used)
{
    int min = used[0];
    int min_index = 0;
    for (int i = 0; i < c_size; i++)
    {
        if (used[i] < min)
        {
            min = used[i];
            min_index = i;
        }
    }
    return min_index;
}

void lru_replacement()
{
    int input;
    int *used = (int *)malloc(c_size * sizeof(int));

    for (int i = 0; i < s_size; i++)
    {
        input = empty_index();
        fault_flag = 0;
        // if input == -1, no empty spaces in cache
        if (input == -1)
        {
            if (!in_cache(stream[i]))
            {
                faults++;
                fault_flag = 1;
                int replacement_index = LRU(used);
                cache[replacement_index] = stream[i];
                used[replacement_index] = INT_MAX;
            }
            else
            {
                for (int j = 0; j < c_size; j++)
                {
                    if (cache[j] == stream[i])
                    {
                        used[j] = INT_MAX;
                        break;
                    }
                }
            }
        }
        else
        {
            if (!in_cache(stream[i]))
            {
                cache[input] = stream[i];
                used[input] = INT_MAX;
            }
            else
            {
                for (int j = 0; j < c_size; j++)
                {
                    if (cache[j] == stream[i])
                    {
                        used[j] = INT_MAX;
                        break;
                    }
                }
            }
        }
        for (int u = 0; u < c_size; u++)
        {
            used[u]--;
        }
        print_state(stream[i]);
    }
}

/* CLOCK ALGORITHM */

int use_check(int *use, int i)
{
    while (1)
    {
        if (use[i] == 0)
            return i;
        else
            use[i]--;
        i = (i + 1) % c_size;
    }
    return -1;
}

void clock_replacement()
{
    int input, pointer;
    int *use = (int *)malloc(c_size * sizeof(int));
    pointer = 0;

    for (int i = 0; i < s_size; i++)
    {
        input = empty_index();
        fault_flag = 0;
        // if input == -1, no empty spaces in cache
        if (input == -1)
        {
            if (!in_cache(stream[i]))
            {
                faults++;
                fault_flag = 1;
                int replacement_index = use_check(use, pointer);
                cache[replacement_index] = stream[i];
                use[replacement_index] = 1;
                pointer = (pointer + 1) % c_size;
            }
            else
            {
                for (int j = 0; j < c_size; j++)
                {
                    if (cache[j] == stream[i])
                    {
                        use[j] = 1;
                        break;
                    }
                }
            }
        }
        else
        {
            if (!in_cache(stream[i]))
            {
                cache[pointer] = stream[i];
                use[pointer] = 1;
                pointer = (pointer + 1) % c_size;
            }
            else
            {
                for (int j = 0; j < c_size; j++)
                {
                    if (cache[j] == stream[i])
                    {
                        use[j] = 1;
                        break;
                    }
                }
            }
        }
        print_state(stream[i]);
    }
}

/* FILE IO */
void read_input()
{
    scanf("%d", &c_size);
    init_cache();

    char *alg;
    alg = (char *)malloc(10 * sizeof(char));
    scanf("%s", alg);

    int i = 0;
    while (stream[i - 1] != -1)
    {
        scanf("%d", &stream[i++]);
    }
    s_size = i - 1;
    for (int j = 0; j < 4; j++)
    {
        if (strcmp(algorithms[j], alg) == 0)
        {
            alg_option = j;
        }
    }
    if (alg_option == -1)
    {
        puts("Please enter a valid algorithm!");
        exit(1);
    }
}

int main()
{
    init();
    read_input();
    printf("Replacement Policy = %s\n-------------------------------------\nPage   Content of Frames\n----   -----------------\n", algorithms[alg_option]);

    switch (alg_option)
    {
    case 0:
        optimal_replacement();
        break;
    case 1:
        fifo_replacement();
        break;
    case 2:
        lru_replacement();
        break;
    case 3:
        clock_replacement();
        break;
    }
    printf("-------------------------------------\nNumber of page faults = %d", faults);
    return 0;
}
