/*
* Volkov-exe4.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>


const int    N       = 1000;
const double L       = 39.0;   
const double EPSILON = 120.0;
const double SIGMA   = 3.4;
const double T       = 94.4;
const long   STEPS   = 300000;

// Глобальный массив 
double coordinates[1000][3];

// Случайное число 
double get_random() {
    return rand() / (double)RAND_MAX;
}

// Потенциал Леннард-Джонса
double lj_potential(double r) {
    double sr6  = pow(SIGMA / r, 6);
    double sr12 = sr6 * sr6;
    return 4.0 * EPSILON * (sr12 - sr6);
}

double r_min_dist(double x0, double y0, double z0,  double x1, double y1, double z1) {
    double dx = fabs(x1 - x0);
    double dy = fabs(y1 - y0);
    double dz = fabs(z1 - z0);

    if (dx > L / 2.0) dx = L - dx;
    if (dy > L / 2.0) dy = L - dy;
    if (dz > L / 2.0) dz = L - dz;

    return sqrt(dx*dx + dy*dy + dz*dz);
}

double potential_energy(int i, double x0, double y0, double z0) {
    double energy = 0.0;
    for (int j = 0; j < N; j++) {
        if (i == j) continue;   

        double r = r_min_dist(x0, y0, z0, coordinates[j][0], coordinates[j][1], coordinates[j][2]);
        
        if (r < 0.1) continue;

        energy += lj_potential(r);
    }
    return energy;
}

double probability(double E_old, double E_new) {
    double dE = E_new - E_old;
    if (dE < 0.0) return 1.0;         
    return exp(-dE / T);               
}

// Периодические граничные условия
double apply_pbc(double x) {
    if (x >  L) x -= L;
    if (x <  0) x += L;
    return x;
}

int main(void) {
    double l   = 0.9;   
    int    freq      = 10;     // частота записи в файл
    long   accepted  = 0;     // счётчик принятых шагов 

    srand((unsigned int)time(NULL));

    
    for (int i = 0; i < N; i++) {
        coordinates[i][0] = L * get_random();
        coordinates[i][1] = L * get_random();
        coordinates[i][2] = L * get_random();
    }

    
    printf("%d\n", N);
    printf("Начальная конфигурация:\n");
    for (int i = 0; i < N; i++) {
        printf("Частица %3d: %8.4f %8.4f %8.4f\n",
               i + 1,
               coordinates[i][0],
               coordinates[i][1],
               coordinates[i][2]);
    }

    FILE *f = fopen("lg.xyz", "w");
    if (!f) {                          
        fprintf(stderr, "Ошибка: не удалось открыть lg.xyz\n");
        return 1;
    }

    
    for (long step = 0; step < STEPS; step++) {

    
        int n = rand() % N;

        double x0 = coordinates[n][0];
        double y0 = coordinates[n][1];
        double z0 = coordinates[n][2];

    
        double x1 = apply_pbc(x0 + (get_random() - 0.5) * l);
        double y1 = apply_pbc(y0 + (get_random() - 0.5) * l);
        double z1 = apply_pbc(z0 + (get_random() - 0.5) * l);

        
        double E_old = potential_energy(n, x0, y0, z0);
        double E_new = potential_energy(n, x1, y1, z1);

        double P = probability(E_old, E_new);

        
        if (get_random() <= P) {
            coordinates[n][0] = x1;
            coordinates[n][1] = y1;
            coordinates[n][2] = z1;
            accepted++;                
        }
        

        
        printf("\n--- Шаг № %d ---\n", (int)step);
        printf("Выбрана частица № %d\n", n + 1);
        printf("Было: %.4f %.4f %.4f\n", x0, y0, z0);
        printf("Стало: %.4f %.4f %.4f\n", x1, y1, z1);

        if (step % freq == 0) {
            fprintf(f, "%d\n", N);
            fprintf(f, "STEP=%d\n", (int)step);
            for (int i = 0; i < N; i++) {
                fprintf(f, "Ar %.4f %.4f %.4f\n",
                        coordinates[i][0],
                        coordinates[i][1],
                        coordinates[i][2]);
            }
        }
    }
    

    printf("\n=== Симуляция завершена ===\n");
    printf("Всего шагов:    %ld\n", STEPS);
    printf("Принято шагов:  %ld\n", accepted);
    printf("Доля принятия:  %.2f%%\n", 100.0 * accepted / STEPS);
    printf("Данные сохранены в lg.xyz\n");

    fclose(f);
    return 0;
}
