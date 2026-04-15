/*
* Volkov-exe3.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 
#include <tgmath.h> 
#include <stdbool.h>   

//я объявил константы
const int N = 100;
const int L = 39;
const double EPSILON = 120;
const double SIGMA = 3.4;
const double T = 94.4;
const long Steps = 10000;

//это был массив для начального цикла счёта энергии
double coordinates [N][3];

//получаю рандомные числа в диапозоне 0-1
double get_random() {
    return rand() / (double)RAND_MAX;
}

//эээээм не помню см. первый файл
double get_random_range(double min, double max) {
    return min + (max - min) * get_random();
}

//Леннард-джонс
double lj_potential(double r) {
    return 4*EPSILON*(pow(SIGMA/r, 12) - pow(SIGMA/r, 6));
}

//расстояние между двумя точками (любыми которыми я передаю в функцию) по методу ближайшнго образа
double r_min_dist (double x1, double x0, double y1, double y0, double z1, double z0) {

            double dx = fabs(x1-x0);
            double dy = fabs(y1-y0);
            double dz = fabs(z1-z0);

            if (dx > L/2) dx = L - dx;
            if (dy > L/2) dy = L - dy;
            if (dz > L/2) dz = L - dz;
            
            return sqrt(dx*dx + dy*dy + dz*dz);
        }

//double sum_energy(){


double potential_energy(int i, double x0, double y0, double z0) {
    double fullenergy = 0;
        for (int j = 0; j < N; j++)
        {
            if (i!=j)
            {
                double r = r_min_dist(coordinates[j][0], x0, coordinates[j][1], y0, coordinates[j][2], z0);
                fullenergy += lj_potential(r);
            }
            else break;
        }
       return fullenergy ;
}

//вероятность перехода))
double probability(double E1, double E2){
    if ((E1-E2) < 0)
    {
        return exp((E1-E2)/T);
    }
    else return 1;
}

int main(void){    
    double l = 0.4;
    double d = 0.5;               
    int step = 0;
    int freq = N;
    long cnt = 0;
    double energy;
    double energy1;

    srand(time(NULL));

    for (int i = 0; i < N; i++)
    {
        coordinates[i][0] = L*get_random();
        coordinates[i][1] = L*get_random();
        coordinates[i][2] = L*get_random();
    }
    

    printf("%d\n", N);
    printf("Начальная конфигурация:\n");
    for (int i = 0; i < N; i++) {
        printf("Частица %d: %.4f %.4f %.4f\n", i + 1, 
               coordinates[i][0], coordinates[i][1], coordinates[i][2]);
    }

    //double Enrg = sum_energy();

    FILE *f = fopen("lg.xyz", "w");

    for(int step = 0; step < Steps; step++) {

        int n = rand() % N;

        printf("\n Номер частицы = %d\n", n+1);

        double x0 = coordinates[n][0];
        double y0 = coordinates[n][1];
        double z0 = coordinates[n][2];


        double x1 = x0 + (get_random() - 0.5) * l;
        double y1 = y0 + (get_random() - 0.5) * l;
        double z1 = z0 + (get_random() - 0.5) * l;

        if (x1 > L) x1 -= L;
        if (x1 < 0) x1 += L;
        if (y1 > L) y1 -= L;
        if (y1 < 0) y1 += L;
        if (z1 > L) z1 -= L;
        if (z1 < 0) z1 += L;

        energy = potential_energy(n, x0,  y0,  z0);
        energy1 = potential_energy(n,  x1,  y1,  z1);

        double P = probability(energy, energy1);

        if (get_random() <= P)
        {
         coordinates[n][0] = x1;
         coordinates[n][1] = y1;
         coordinates[n][2] = z1;
        }
        else
        {
            break;
        }
        

        printf("\n--- Шаг № %d ---\n", step);
        printf("Выбрана частица № %d\n", n + 1);
        printf("Было: %.4f %.4f %.4f\n", x0, y0, z0);
        printf("Стало: %.4f %.4f %.4f\n", x1, y1, z1);

       if (step % freq == 0) { 
            fprintf(f, "%d\n", N);
            fprintf(f, "STEP=%d\n", step);
            for(int i = 0; i < N; i++) {
                fprintf(f, "Ar %.4f %.4f %.4f\n", 
                        coordinates[i][0], coordinates[i][1], coordinates[i][2]);
            }
        }
    }

    printf("\nСимуляция завершена. Счетчик принятия %f. Данные сохранены в lg.xyz\n",  (double)cnt/(double)Steps);

    fclose(f);

    return 0;
}