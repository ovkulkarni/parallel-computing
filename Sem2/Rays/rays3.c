#include <stdio.h>
#include <math.h>
#define M 640
#define N 480
#define NUM_BALLS 4
#define scale 0.025
#define SHADOW 0.5
typedef struct
{
    double x ;
    double y ;
    double z ;
} triple ;
typedef struct {
    int r;
    int g;
    int b;
} color;
typedef struct {
    triple center;
    color c;
    double r;
} ball;

triple e = { 0.50 , 0.50 , -1.00 } ; // the eye
triple g = { 0.00 , 1.25 , -0.50 } ; // the light
ball a[NUM_BALLS];

double dotp( triple t , triple u )
{
    return t.x * u.x + t.y * u.y + t.z * u.z ;
}
void diff( triple* t , triple u , triple v ) // t = u - v
{
    t->x = u.x - v.x ;
    t->y = u.y - v.y ;
    t->z = u.z - v.z ;
}
double mag(triple t){
    return sqrt(pow(t.x, 2) + pow(t.y, 2) + pow(t.z, 2));
}
void init()
{
    a[0].center.x =      0.50 ;
    a[0].center.y = -20000.00 ; // the floor
    a[0].center.z =      0.50 ;
    a[0].r   =  20000.25 ;
    a[0].c.r =    205    ; // color is Peru
    a[0].c.g =    133    ;
    a[0].c.b =     63    ;
    //
    a[1].center.x =      0.50 ;
    a[1].center.y =      0.50 ;
    a[1].center.z =      0.50 ;
    a[1].r   =      0.25 ;
    a[1].c.r =      0    ; // color is Blue
    a[1].c.g =      0    ;
    a[1].c.b =    255    ;
    //
    a[2].center.x =      1.00 ;
    a[2].center.y =      0.50 ;
    a[2].center.z =      1.00 ;
    a[2].r   =      0.25 ;
    a[2].c.r =      0    ; // color is Green
    a[2].c.g =    255    ;
    a[2].c.b =      0    ;
    //
    a[3].center.x =      0.00 ;
    a[3].center.y =      0.75 ;
    a[3].center.z =      1.25 ;
    a[3].r   =      0.50 ;
    a[3].c.r =    255    ; // color is Red
    a[3].c.g =      0    ;
    a[3].c.b =      0    ;
}
double get_intersect(triple start, triple end, ball* b){
    triple ray;
    diff(&ray, start, end);
    double A = dotp(ray, ray);
    triple to_ball;
    diff(&to_ball, end, b -> center);
    double B = 2 * dotp(ray, to_ball);
    double C = dotp(to_ball, to_ball) - (b -> r*b -> r);
    double disc = pow(B, 2) - 4*A*C;
    if(disc >= 0){
        double T1 = (-1*B - sqrt(disc)) / (2*A);
        double T2 = (-1*B + sqrt(disc)) / (2*A);
        if(T1 < 0 && T2 < 0)
            return T1;
        else if(T1 > 0 && T2 < 0)
            return T1;
        else if(T1 < 0 && T2 > 0)
            return T2;
        else if(T1 > 0 && T2 > 0){
            if(T1 < T2)
                return T1;
            else
                return T2;
        }
    }
    return 0.0;
}
int main(void)
{
    init();
    int rgb[N][M][3] ; // red-green-blue for each pixel
    int y , x, i;
    color background;
    background.r = 0;
    background.g = 0;
    background.b = 0;
    double max_num = (1 << 30) * 1.0;
    FILE* fout ;
    for( y = 0 ; y < N ; y++ )
    {
        for( x = 0 ; x < M ; x++)
        {
            double min_t = max_num;
            int in_shadow = 0;
            int intersecting;
            triple pixel = {x*scale, (N-y)*scale, 0.0};
            for(i = 0; i < NUM_BALLS; i++){
                double new_t = get_intersect(pixel, e, &a[i]);
                if(new_t > 0 && (new_t < min_t || min_t == max_num)){
                    min_t = new_t;
                    intersecting = i;
                }
            }
            ball b = a[intersecting];
            triple direction;
            diff(&direction, pixel, e);
            triple from = {e.x + direction.x*min_t, e.y + direction.y*min_t, e.z + direction.z*min_t};
            double nx = (from.x - b.center.x)/b.r;
            double ny = (from.y - b.center.y)/b.r;
            double nz = (from.z - b.center.z)/b.r;
            from.x += nx*0.001;
            from.y += ny*0.001;
            from.z += nz*0.001;
            for(i = 0; i < NUM_BALLS; i++){
                double new_t = get_intersect(g, from, &a[i]);
                if(new_t > 0){
                    in_shadow = 1;
                    break;
                }
            }
            triple tocenter;
            diff(&tocenter, b.center, from);
            triple tolight;
            diff(&tolight, b.center, g);
            double lambert = dotp(tocenter, tolight)/(mag(tocenter)*mag(tolight));
            if(lambert < 0)
                lambert = 0;
            if(min_t == max_num){
                rgb[y][x][0] = background.r;
                rgb[y][x][1] = background.g;
                rgb[y][x][2] = background.b;
            }
            else{
                if(in_shadow) {
                    rgb[y][x][0] = b.c.r*SHADOW;
                    rgb[y][x][1] = b.c.g*SHADOW;
                    rgb[y][x][2] = b.c.b*SHADOW;
                }
                else{
                    rgb[y][x][0] = b.c.r*0.5 + b.c.r*0.5*lambert;
                    rgb[y][x][1] = b.c.g*0.5 + b.c.g*0.5*lambert;
                    rgb[y][x][2] = b.c.b*0.5 + b.c.b*0.5*lambert;
                }
            }
        }
    }
    fout = fopen( "rays3.ppm" , "w" ) ;
    fprintf( fout , "P3\n" ) ;
    fprintf( fout , "%d %d\n" , M , N ) ;
    fprintf( fout , "255\n" ) ;
    for( y = 0 ; y < N ; y++ )
    {
        for( x = 0 ; x < M ; x++)
        {
            fprintf( fout , "%d %d %d\n" ,
                    rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
        }
    }
    fclose( fout ) ;
    return 0 ;
}
