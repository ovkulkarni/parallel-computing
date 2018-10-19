#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <GL/glut.h>
#define W 800
#define H 600
int max_steps = 10;
double left = -2.5;
double right = 1.5;
double top = 1.5;
double bottom = -1.5;

double get_time()
{
	double t ;
	//
	struct timeval* ptr = (struct timeval*)malloc( sizeof(struct timeval) ) ;
	//
	gettimeofday( ptr , NULL ) ; // second argument is time zone... NULL
	//
	t = ptr->tv_sec * 1000000.0 + ptr->tv_usec ;
	//
	free( ptr ) ;
	//
	return t / 1000000.0 ;
}

typedef struct {
    double real;
    double im;
} ImNum;

__device__ ImNum next_num(ImNum z, double a, double b){
    ImNum z_i = {pow(z.real, 2) - pow(z.im, 2) + a, 2*z.real*z.im + b};
    return z_i;
}
__global__ void run_pixel(double a, double hscale, double bottom, int max_steps, float *red, float *green, float *blue){
	int y = threadIdx.x;
	ImNum curr_z = {0, 0};
	double b = bottom + (y * hscale);
	int steps = 0;
	for(int i = 0; i < max_steps; i++){
		curr_z = next_num(curr_z, a, b);
		if(hypot(curr_z.real, curr_z.im) > 2){
			break;
		}
		steps++;
	}
	if(steps != max_steps){
		red[y] = (steps*1.0/max_steps);
		green[y] = 1-(steps*1.0/max_steps);
		blue[y] = (steps*1.0/max_steps);
	}
}

void displayfunc()
{
    double hscale = fabs(top-bottom)/H;
    double wscale = fabs(right-left)/W;
    glClear(GL_COLOR_BUFFER_BIT);
    double start = get_time();
    for(int x = 0; x < W; x++){
        double a = left + (x * wscale);
        float *red, *green, *blue;
        float *red_device, *green_device, *blue_device;
        red = (float*)malloc(H*sizeof(float));
        blue = (float*)malloc(H*sizeof(float));
        green = (float*)malloc(H*sizeof(float));
        cudaMalloc((void**)&red_device, sizeof(float)*H);
        cudaMalloc((void**)&blue_device, sizeof(float)*H);
        cudaMalloc((void**)&green_device, sizeof(float)*H);
        for(int i = 0; i < H; i++){
			red[i] = 1;
			blue[i] = 1;
			green[i] = 1;
		}
		cudaMemcpy(red_device, red, sizeof(float)*H, cudaMemcpyHostToDevice);
		cudaMemcpy(blue_device, blue, sizeof(float)*H, cudaMemcpyHostToDevice);
		cudaMemcpy(green_device, green, sizeof(float)*H, cudaMemcpyHostToDevice);
		dim3 dimGrid(1), dimBlock(H);
		run_pixel<<<dimGrid,dimBlock>>>(a, hscale, bottom, max_steps, red_device, green_device, blue_device);
		cudaMemcpy(red, red_device, sizeof(float)*H, cudaMemcpyDeviceToHost);
		cudaMemcpy(blue, blue_device, sizeof(float)*H, cudaMemcpyDeviceToHost);
		cudaMemcpy(green, green_device, sizeof(float)*H, cudaMemcpyDeviceToHost);
		for(int y = 0; y < H; y++){
			glColor3f(red[y], green[y], blue[y]);
			glBegin(GL_POINTS);
			glVertex2f(x,y);
			glEnd();
		}
		
		free(red);
		free(blue);
		free(green);
		cudaFree(red_device);
		cudaFree(blue_device);
		cudaFree(green_device);
    }
    glutSwapBuffers();
    double end = get_time();
    printf("dx: %f\ndy: %f\n", fabs(right-left), fabs(top-bottom));
    printf("max steps: %d\n", max_steps);
    printf("time: %f\n", end - start);
    printf("***********************************\n");

}
void reshapefunc(int wscr,int hscr)
{
   glViewport(0,0,(GLsizei)W,(GLsizei)H);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,1.0*W,0.0,1.0*H); // always a square
   glMatrixMode(GL_MODELVIEW);
}
void mousefunc(int button,int state,int xscr,int yscr)
{
    if(button == 3 && state == GLUT_DOWN){
        max_steps *= 2;
    }
    if(button == 4 && state == GLUT_DOWN){
        max_steps /= 2;
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        double hscale = fabs(top-bottom)/H;
        double wscale = fabs(right-left)/W;
        double x_coord = left + (xscr * wscale);
        double y_coord = bottom + ((H-yscr) * hscale);
        double temp_left = x_coord - (fabs(right-left)/4);
        double temp_right = x_coord + (fabs(right-left)/4);
        left = temp_left;
        right = temp_right;
        double temp_bottom = y_coord - fabs(top-bottom)/4;
        double temp_top = y_coord + fabs(top-bottom)/4;
        bottom = temp_bottom;
        top = temp_top;
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        double hscale = fabs(top-bottom)/H;
        double wscale = fabs(right-left)/W;
        double x_coord = left + (xscr * wscale);
        double y_coord = bottom + ((H-yscr) * hscale);
        double temp_left = x_coord - (fabs(right-left));
        double temp_right = x_coord + (fabs(right-left));
        left = temp_left;
        right = temp_right;
        double temp_bottom = y_coord - fabs(top-bottom);
        double temp_top = y_coord + fabs(top-bottom);
        bottom = temp_bottom;
        top = temp_top;
    }
    glutPostRedisplay();
}
void keyfunc(unsigned char key,int xscr,int yscr)
{
    double hscale = fabs(top-bottom)/H;
    double wscale = fabs(right-left)/W;
    if(key == 'a'){
        left -= (W/10)*wscale;
        right -= (W/10)*wscale;
    }
    if(key == 'd'){
        left += (W/10)*wscale;
        right += (W/10)*wscale;
    }
    if(key == 'w'){
        bottom += (H/10)*hscale;
        top += (H/10)*hscale;
    }
    if(key == 's'){
        bottom -= (H/10)*hscale;
        top -= (H/10)*hscale;
    }
    glutPostRedisplay();
}
int main(int argc,char* argv[])
{
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(W, H);
   glutInitWindowPosition(100,50);
   glutCreateWindow("Fractals Lab");
   glClearColor(1.0,1.0,1.0,0.0);
   glShadeModel(GL_SMOOTH);
   glutDisplayFunc(displayfunc);
   glutReshapeFunc(reshapefunc);
   glutMouseFunc(mousefunc);
   glutKeyboardFunc(keyfunc);
   glutMainLoop();
   return 0;
}

