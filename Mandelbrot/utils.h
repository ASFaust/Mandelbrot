#ifndef UTILS_H
#define UTILS_H

typedef long double float_type;
typedef long int count_type;

#define MAX_ZOOM_DOUBLE 9000000000000000

#define frand(a,b) ((float_type(rand()) / RAND_MAX) * (a - b) + b)
#define frand_sq(a,b) (frand(a,b) * frand(a,b))

#endif //UTILS_H
