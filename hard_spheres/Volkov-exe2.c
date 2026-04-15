/*
* Volkov-exe2.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <tgmath.h>
#include <stdbool.h>

double get_random() {
    return rand() / (double)RAND_MAX;
}

double get_random_range(double min, double max) {
    return min + (max - min) * get_random();
}

double r_min_dist (double x1, double x0, double y1, double y0, double z1, double z0, double L) {

            double dx = fabs(x1-x0);
            double dy = fabs(y1-y0);
            double dz = fabs(z1-z0);

            if (dx > L/2) dx = L - dx;
            if (dy > L/2) dy = L - dy;
            if (dz > L/2) dz = L - dz;
            
            return sqrt(dx*dx + dy*dy + dz*dz);
        }

        


int main(void) {
    
    int N = 1000;          
    double L = 10.0;      
    double l = 0.4;
    double d = 0.5;          
    long Steps = 1000000;     
    int step = 0;
    int freq = N;
    long cnt = 0;

 
    srand(time(NULL));

         
    double coordinates [N][3];

    coordinates[0][0] = 0;
    coordinates[0][1] = 0;
    coordinates[0][2] = 0;

    

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            for (int k = 0; k < 10; k++)
            {
                coordinates[k+10*j+100*i][0] = k;
                coordinates[k+10*j+100*i][1] = j;
                coordinates[k+10*j+100*i][2] = i;
            }    
        }
    }
     
    
    printf("%d\n", N);
    printf("Начальная конфигурация:\n");
    for (int i = 0; i < N; i++) {
        printf("Частица %d: %.4f %.4f %.4f\n", i + 1, 
               coordinates[i][0], coordinates[i][1], coordinates[i][2]);
    }

    
    FILE *f = fopen("filename.xyz", "w");

    
    for (int step = 0; step < Steps; step++) {
        
        
        int n = rand() % N;
        
        
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

        bool    flag = 1;

        for (int i = 0; i < N; i++)
        {
            if (i!= n)
            {
                if(r_min_dist(coordinates[i][0], x1, coordinates[i][1], y1, coordinates[i][2], z1, L) >= d){
                /*coordinates[n][0] = x1;
                coordinates[n][1] = y1;
                coordinates[n][2] = z1;*/
                }
                else{
                    flag = 0;
                    break;
                }
            }
            
            
        }

        if (flag == 1)
        {
            coordinates[n][0] = x1;
            coordinates[n][1] = y1;
            coordinates[n][2] = z1;
            cnt++;
        }
        
        

        
        printf("\n--- Шаг № %d ---\n", step);
        printf("Выбрана частица № %d\n", n + 1);
        printf("Было: %.4f %.4f %.4f\n", x0, y0, z0);
        printf("Стало: %.4f %.4f %.4f\n", x1, y1, z1);

       if (step % freq == 0) { 
            fprintf(f, "%d\n", N);
            fprintf(f, "STEP=%d\n", step);
            for (int i = 0; i < N; i++) {
                fprintf(f, "Ar %.4f %.4f %.4f\n", 
                        coordinates[i][0], coordinates[i][1], coordinates[i][2]);
            }
        }
    }

    

    printf("\nСимуляция завершена. Счетчик принятия %f. Данные сохранены в filename.xyz\n",  (double)cnt/(double)Steps);

    fclose(f);

    return 0;
}