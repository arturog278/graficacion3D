#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

static float rotar=0;
GLfloat angle = 0;
GLfloat angle2 = 0;
int moving, startx, starty,caer=0;
int transx =0,transy=0;
static GLdouble tamanio = 6.0;
static GLfloat posicionLuz[6];
float c=1;
float xobj=-59,yobj=62,xpant=0,ypant=0,x2=0,y2=0,angr,paso=2.622222222;  //variables camara y objetivo
float xdona=-30,ydona=30,xfijo=30,yfijo=-30,xmov=30,ymov=30,dist=0,mov=1,down=4,t=0;             //objeto colision


char *textura[] = {
    "oooooooooooooooo",
    "o..............o",
    "o..............o",
    "o..............o",
    "o..............o",
    "o..............o",
    "o..............o",
    "o..............o",
    "o..............o",
    "o..............o",
    "o..............o",
    "o..............o",
    "o..............o",
    "o..............o",
    "o..............o",
    "oooooooooooooooo",
};

void texturaPiso(void)
{
    GLubyte imagenRGB[16][16][3];
    GLubyte *col;
    int s, t;
    // Crear RGB para textura
    col = (GLubyte*) imagenRGB;
    for (t = 0; t < 16; t++) {
        for (s = 0; s < 16; s++) {
            if (textura[t][s] == 'x') {
                col[0] = 128;
                col[1] = 128;
                col[2] = 128;
            } else if (textura[t][s] == 'o') {
                col[0] = 0;
                col[1] = 0;
                col[2] = 0;
            } else if (textura[t][s] == 'p') {
                col[0] = 0;
                col[1] = 0;
                col[2] = 255;
            }else {
                col[0] =255;
                col[1] =255;
                col[2] =255;
            }
            col += 3;
        }
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 16, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, imagenRGB);
}

void dibujaObjeto(void)
{
    glPushMatrix();
    glTranslatef(0, 5, 0);

    GLfloat mat_p_ambient[] = {0.4725,0.4245,0.8645,1};
    GLfloat mat_p_diffuse[] = {0.34615,0.5143,0.8903,1};
    GLfloat mat_pspecular[] = {1.797357,1.723991,1.708006,1};
    GLfloat mat_pshininess[] = {18.2};

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_pspecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_pshininess);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_p_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_p_diffuse);
    glShadeModel(GL_SMOOTH);
    glutSolidCube(tamanio);
    glPopMatrix();
}

static GLfloat verticesPiso[4][3] = {
    { -64.0, 0.0,  64.0 },
    {  64.0, 0.0,  64.0 },
    {  64.0, 0.0, -64.0 },
    { -64.0, 0.0, -64.0 },
};
void drawFloor(void)
{
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(verticesPiso[0]);
    glTexCoord2f(0.0, 16.0);
    glVertex3fv(verticesPiso[1]);
    glTexCoord2f(16.0, 16.0);
    glVertex3fv(verticesPiso[2]);
    glTexCoord2f(16.0, 0.0);
    glVertex3fv(verticesPiso[3]);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

void checarColisiones(int avancex,int avancey){

    xpant=xobj; ypant=yobj;                             //guardo el paso anterior

    xobj=xobj+(avancex*paso*3);    //calculo los pasos nuevos
    yobj=yobj+(avancey*paso*3);

    dist=sqrt((xobj-xmov)*(xobj-xmov)+(yobj-ymov)*(yobj-ymov));     //distancia con el obj movil

    if(dist<=tamanio*2 && (xmov>xobj || ymov>yobj) && down==tamanio){xmov=xobj+tamanio*2;ymov=yobj+tamanio*2;}
    if(dist<=tamanio*2 && (xmov<xobj || ymov<yobj) && down==tamanio){xmov=xobj-tamanio*2;ymov=yobj-tamanio*2;}
    if (xmov>=65 || xmov<=-65 || ymov>=65 || ymov<=-65){caer=1;} //para caer fuera del tablero
    else{caer=0;t=0;}


    dist=sqrt((xobj-xfijo)*(xobj-xfijo)+(yobj-yfijo)*(yobj-yfijo));     //dist con el obj fijo
    if(dist<=tamanio*2){xobj=xpant;yobj=ypant;}         //deteccion col


    dist=sqrt((xobj-xdona)*(xobj-xdona)+(yobj-ydona)*(yobj-ydona));     //dist con el obj a recoger
    if(dist<=tamanio){xdona=xobj;ydona=yobj;}         //deteccion col

}

void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glPushMatrix();
    glRotatef(0/4, 1, 0, 0);  //rotar arriba/abajo con mouse

    angr=0*3.141592/180;        //convertir a rad rotacion en y

    x2 =  cos(angr) + 80*sin(angr); //rotar la camara
    y2 = -sin(angr) + 80*cos(angr); //con matriz de rotacion

    gluLookAt(x2,150,y2+100,       // posicion de camara
              0,0,0,              // Hacia donde ve (objetivo)
              0,1,0);               // Eje de rotacion

    glPushMatrix();
    glTranslatef(xobj, 0, yobj);    //trasladar objeto
    glRotatef(angle, 0, 1, 0);  //rotar objeto sobre y
    dibujaObjeto();             //tetera
    glPopMatrix();

    glPushMatrix();
    glTranslatef(xmov,down,ymov);    //trasladar esfera movil
    glutSolidSphere(tamanio,64,64);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-30,4,-32);    //trasladar esfera sin colisiones
    glutSolidSphere(tamanio,64,64);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(xfijo,tamanio,yfijo);    //trasladar icosaedro fijo
    glScalef(tamanio, tamanio, tamanio);
    glutSolidIcosahedron();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(xdona,tamanio,ydona);    // dona
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.5, tamanio, 32, 32);
    glPopMatrix();

    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    glVertex3f(xobj,tamanio,yobj);
    glVertex3f(xmov,down,ymov);
    glEnd();
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(xobj,tamanio,yobj);
    glVertex3f(xfijo,tamanio,yfijo);
    glEnd();
    glColor3f(1, 1, 0);
    glBegin(GL_LINES);
    glVertex3f(xobj,tamanio,yobj);
    glVertex3f(xdona,tamanio,ydona);
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1, 1, 1,0.5);
    drawFloor();
    glDisable(GL_BLEND);

    glPopMatrix();


    glutSwapBuffers();
    printf("Posicion x=%f\n",xobj);
    printf("Posicion y=%f\n",yobj);
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            moving = 1;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP) {
            moving = 0;
        }
    }
}

void mover(int x, int y)
{
    if (moving) {
        angle = angle + (x - startx);
        // angle2 = angle2 + (y - starty);
        startx = x;
        // starty = y;
        glutPostRedisplay();
    }
}
void idle(void)
{
    if (caer==1){t=t+0.05; down=-9.81*t*t; glutPostRedisplay();}
    // printf("%f\r",fall);
}

static void
key(unsigned char c, int x, int y)
{
    if (c == 27) {
        exit(0);
    }
    if (c == 'd' && xobj<=52.0) {
        checarColisiones(1,0);
        // angle=angle-5;
    }
    if (c == 'a' && xobj>=-52.0) {
        // angle=angle+5;
        checarColisiones(-1,0);
    }
    if (c == 's' && yobj<=61.0) {
        checarColisiones(0,1);                   //revisar colision conra movil

    }
    if (c == 'w' && yobj>=-54.0) {
        checarColisiones(0,-1);                   //revisar colision conra movil

    }
    if (c == 'r') {
        xmov=0;ymov=0;down=tamanio,caer=0;
    }
    glutPostRedisplay();
}

void init(void){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(40.0,1.0,20.0,500.0);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    posicionLuz[0] = 12;
    posicionLuz[1] = 15;
    posicionLuz[2] = 0;
    posicionLuz[3] = 0.8;//luz posicional o direccional
    glLightfv(GL_LIGHT0, GL_POSITION, posicionLuz); /*light position. */
    texturaPiso();
    glClearColor(1, 1, 1, 1);
    glLineWidth(2);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);

    glutInitWindowSize(1024, 800);

    glutCreateWindow("CAMARA Y COLISION");

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(mover);
    glutIdleFunc(idle);
    glutKeyboardFunc(key);

    init();
    glutMainLoop();
    return 0;
}
