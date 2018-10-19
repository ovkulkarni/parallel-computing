#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#define W 800
#define H 600
int max_steps = 10;
double left = -2.5;
double right = 1.5;
double top = 1.5;
double bottom = -1.5;
double constant_r = 0;
double constant_i = 1;

typedef struct {
    double real;
    double im;
} ImNum;

ImNum next_num(ImNum z, double a, double b){
    ImNum z_i = {pow(z.real, 2) - pow(z.im, 2) + a, 2*z.real*z.im + b};
    return z_i;
}

void displayfunc()
{
    double hscale = fabs(top-bottom)/H;
    double wscale = fabs(right-left)/W;
    glClear(GL_COLOR_BUFFER_BIT);
    for(int x = 0; x < W; x++){
        double a = left + (x * wscale);
        for(int y = 0; y < H; y++){
            double b = bottom + (y * hscale);
            ImNum curr_z = {a, b};
            int steps = 0;
            for(int i = 0; i < max_steps; i++){
                curr_z = next_num(curr_z, constant_r, constant_i);
                if(hypot(curr_z.real, curr_z.im) > 2){
                    break;
                }
                steps++;
            }
            if(steps == max_steps){
                glColor3f(1, 1, 1);
            }
            else
                glColor3f((steps*1.0/max_steps), 1-(steps*1.0/max_steps), (steps*1.0/max_steps));
            glBegin(GL_POINTS);
            glVertex2f(x,y);
            glEnd();
        }
    }
    printf("dx: %f\ndy: %f\n", fabs(right-left), fabs(top-bottom));
    printf("max steps: %d\n", max_steps);
    printf("Constant: %f + %fi\n", constant_r, constant_i);
    printf("***********************************\n");
    glutSwapBuffers();

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
    if(key == '8'){
        constant_r += rand()/1000000000.0;
    }
    if(key == '2'){
        constant_r -= rand()/1000000000.0;
    }
    if(key == '6'){
        constant_i += rand()/1000000000.0;
    }
    if(key == '4'){
        constant_i -= rand()/1000000000.0;
    }
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
    srand(time(NULL));
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

