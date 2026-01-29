#ifndef TEST_RAND_STUB_H
#define TEST_RAND_STUB_H


void rand_reset(void);
void rand_set_sequence(const int *values, int count);
int  rand_next(void);


#endif