#include "test_rand_stub.h"

static const int *g_seq = 0;
static int g_count = 0;
static int g_idx = 0;

static const int g_default_seq[] = {42, 10, 99, 7, 15};

void rand_set_sequence(const int *values, int count)
{
    g_seq = values;
    g_count = count;
    g_idx = 0;
}

void rand_reset(void)
{
    g_idx = 0;

    if (g_seq == 0 || g_count <= 0)
    {
        g_seq = g_default_seq;
        g_count = (int)(sizeof(g_default_seq) / sizeof(g_default_seq[0]));
    }
}

int rand_next(void)
{
    if (g_seq == 0 || g_count <= 0)
    {
        rand_reset();
    }

    int v = g_seq[g_idx % g_count];
    g_idx++;
    return v;
}