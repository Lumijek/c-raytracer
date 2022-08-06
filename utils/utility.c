#include "utility.h"


double random_double()
{
    return (double)rand() / (double)RAND_MAX ;
}

double random_double_mm(double min, double max) {
    return min + (max-min)*random_double();
}
