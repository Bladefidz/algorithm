void print2DMatrix(int *matrix, unsigned int n)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < count; ++j)
        {
            printf("%d\n", *((matrix + i * n) + j));
        }
        printf("\n");
    }
}

int addbyColRow(int *matrix, unsigned int n, int pair[])
{
    unsigned int f = 0;
    for (int i = 0; i < n; ++i)
    {
        if (*(matrix + i * n) == p[0]) {
            for (int j = 1; j < n; ++j)
            {
                if (*((matrix + i * n) + j) == 0) {
                    *((matrix + i * n) + j) = p[1];
                    f = 1;
                    break;
                } else {
                    if (*((matrix + i * n) + j) == p[1]) {
                        f = 1;
                        break;
                    }
                }
            }
            break;
        }
    }
    return f;
}

int deletebyColRow(int *matrix,  unsigned int n, int pair[])
{
    unsigned int f = 0;
    int tmp;
    for (int i = 0; i < n; ++i)
    {
        if (*(matrix + i * n) == p[0]) {
            for (int j = 1; j < n; ++j)
            {
                if (*((matrix + i * n) + j) == p[1]) {
                    *((matrix + i * n) + j) == 0;
                    f = 1;
                    break;
                }
            }
            break;
        }
    }
    return f;
}