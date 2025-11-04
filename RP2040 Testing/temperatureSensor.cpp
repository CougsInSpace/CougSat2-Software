#include <stdio.h>
#include "pico/stdlib.h"
#include <math.h>

int main()
{
    stdio_init_all();

    while (true) {
        double a = (2.108508173*pow(10,-3));
        double b= (0.7979204727*pow(10,-4));
        double c = (6.535076315 * pow(10,-7));
        double r = 5312.0;//resistence always add .0
        double tempK = 1/(a+(b*log(r)+(c*pow(log(r),3))));
        double tempC = tempK-273.15;
        printf("%f\n",tempC);
        sleep_ms(1000);
    }
}
