#include <stdio.h>
#include <math.h>

#define G 6.67408E-11
#define R 6371.0E3
#define M 5.97219E24
#define OH 384400E3
#define MOS 1022
int main(int argc, char* argv[]){
    float dt = 0.25f;
    int time = 90*60;
    float x[(int)(time/dt)];
    float y[(int)(time/dt)];
    float vx[(int)(time/dt)];
    float vy[(int)(time/dt)];
    x[0] = 0.0f;
    y[0] = R + OH;
    vx[0] = MOS;
    vy[0] = 0.0f;
    for(float t = dt; t < time; t += dt){
        int curr = (int)(t/dt);
        float r = hypot(x[curr-1], y[curr-1]);
        float a = (G*M)/pow(r, 2);
        float ax = -1 * a * (x[curr-1] / r);
        float ay = -1 * a * (y[curr-1] / r);
        vx[curr] = vx[curr-1] + ax*dt;
        vy[curr] = vy[curr-1] + ay*dt;
        x[curr] = x[curr-1] + vx[curr]*dt;
        y[curr] = y[curr-1] + vy[curr]*dt;
    }
    printf("t\tx\ty\tr\tvx\tvy\tv\n");
    for(int i = 0; i < (int)(time/dt); i++){
        printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\n", i * dt, x[i], y[i], hypot(x[i], y[i]), vx[i], vy[i], hypot(vx[i], vy[i]));
    }
    return 0;
}
