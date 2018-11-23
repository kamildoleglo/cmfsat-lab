#include <stdio.h>
#include <gsl/gsl_ieee_utils.h>

int main() {
    //gsl_ieee_printf_double
    double a = 1.1;
    while (a > 0){
        gsl_ieee_printf_double(&a);
        printf("\r\n");
        a /= 2.0;
    }

    float b = 1.1;
    while (b > 0){
        gsl_ieee_printf_float(&b);
        printf("\r\n");
        b /= 2.0;
    }

    return 0;
}