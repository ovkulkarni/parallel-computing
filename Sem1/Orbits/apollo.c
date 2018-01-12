#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define G 6.67408E-11
#define ER 6371.0E3
#define EM 5.97219E24
#define MR 1737E3
#define MM 7.34767309E22
#define MOH 384402E3
#define MV 1022
#define SD ER + (MOH / 2)
#define APV 1500
#define PI 3.14159265358979323846

int runSim(float theta, int print){
  float dt = 1.0f;
  int tot = 4 * 24 * 60 * 60;
  int pastmoon = 0;
  int returned = 0;
  float mx = ER + MOH;
  float my = 0;
  float mvy = MV;
  float mvx = 0.0f;
  float mr = hypot(mx, my);
  float ma = (G*EM)/pow(mr, 2);
  float max = -1 * ma * (mx / mr);
  float may = -1 * ma * (my / mr);
  float apx = SD * cos(theta);
  float apy = SD * sin(theta);
  float apr = hypot(apx, apy);
  float aprm = hypot(mx - apx, my - apy);
  float apvx = APV * cos(theta);
  float apvy = APV * sin(theta);
  float ape = (G*EM)/pow(apr, 2);
  float apm = (G*MM)/pow(aprm, 2);
  float apex = -1 * ape * (apx / apr);
  float apey = -1 * ape * (apy / apr);
  float apmx = -1 * apm * ((apx - mx) / aprm);
  float apmy = -1 * apm * ((apy - my) / aprm);
  float apax = apmx + apex;
  float apay = apmy + apey;
  if(print)
    printf("t\tx\ty\tmx\tmy\n");
  for(float t = dt; t < (tot*(1+print)); t += dt){
      if(print)
        printf("%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\n", t, apx, apy, mx, my);
      mr = hypot(mx, my);
      ma = (G*EM)/pow(mr, 2);
      max = -1 * ma * (mx / mr);
      may = -1 * ma * (my / mr);
      mvx +=  max*dt;
      mvy +=  may*dt;
      mx += mvx*dt;
      my += mvy*dt;
      apr = hypot(apx, apy);
      aprm = hypot(apx - mx, apy - my);
      ape = (G*EM)/pow(apr, 2);
      apm = (G*MM)/pow(aprm, 2);
      apex = -1 * ape * (apx / apr);
      apey = -1 * ape * (apy / apr);
      apmx = -1 * apm * ((apx - mx) / aprm);
      apmy = -1 * apm * ((apy - my) / aprm);
      apax = apmx + apex;
      apay = apmy + apey;
      apvx += apax*dt;
      apvy += apay*dt;
      apx += apvx*dt;
      apy += apvy*dt;
      if(!print) {
          if(aprm <= MR) return 2;
          if(apr > mr && !pastmoon) {
            if(apx/apr > mx/mr) return -1;
            pastmoon = 1;
          }
          if(apr < mr && pastmoon){
            if(apx/apr < mx/mr) return 1;
            returned = 1;
          }
        }
  }
  if(returned) return 0;
  return 3;
}

float search(float min, float max){
  if(min >= max)
    return -1;
  float mid = (min + max)/2.0f;
  int ret = runSim(mid, 0);
  fprintf(stderr, "Returned %d for %0.2f\n", ret, mid);
  if(ret == 0){
    runSim(mid, 1);
    return mid;
  }
  if(ret == -1 || ret == 2)
    return search(mid, max);
  else if(ret == 1 || ret == 3)
    return search(min, mid);
  return -1;
}
int main(int argc, char* argv[]){
    //fprintf(stderr, "Returned %d\n", runSim(0.69, 1));
    //runSim(atof(argv[1]), 1);
    float theta = search(0.0f, 0.75f);
    fprintf(stderr, "Found angle: %0.2f\n", theta);
    return 0;
}
