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
int transx =0,transy=0,vacio=0,azul=0,rojo=0;
static GLdouble tamanio = 6.0;
static GLfloat posicionLuz[6];
float c=1;
float xobj1=-59,yobj1=62,xpant1=0,ypant1=0,x2=0,y2=0,angr,paso=2.622222222;  //variables camara y objetivo
float xdona=-30,ydona=30,xfijo=30,yfijo=-30,xmov=30,ymov=30,dist=0,mov=1,down=4,t=0;//objeto colision

float xobj2=59,yobj2=-56,xpant2=0,ypant2=0;

int colored[16][16];
int gameOver = 0;
float time = 60;

int posxmat1 = 0, posymat1 = 15;
int posxmat2 = 15, posymat2 = 0;
enum {X, Y, Z, W};
enum {A, B, C, D};


static GLfloat colorLuz[] = {1, 1, 1, 1.0};

static GLfloat planoPiso[4];
static GLfloat sombraPiso[4][4];

void initMatriz(){
    for(int i=0;i<16;i++){
        for(int y=0;y<16;y++){
            colored[i][y]=0;
        }
    }
}
static GLfloat verticesPiso[4][3] = {
    { -64.0, 0.0,  64.0 },
    {  64.0, 0.0,  64.0 },
    {  64.0, 0.0, -64.0 },
    { -64.0, 0.0, -64.0 },
};


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
void shadowMatrix(float matrizSombra[4][4],
                  float ecPlano[4],
                  float posLuz[4])
{
    GLfloat dot;
    // Producto punto entre vector light position y la normal de ground plane
    dot = ecPlano[X] * posLuz[X] +
    ecPlano[Y] * posLuz[Y] +
    ecPlano[Z] * posLuz[Z] +
    ecPlano[W] * posLuz[W];
    
    matrizSombra[0][0] = dot - posLuz[X] * ecPlano[X];
    matrizSombra[1][0] = 0.f - posLuz[X] * ecPlano[Y];
    matrizSombra[2][0] = 0.f - posLuz[X] * ecPlano[Z];
    matrizSombra[3][0] = 0.f - posLuz[X] * ecPlano[W];
    
    matrizSombra[X][1] = 0.f - posLuz[Y] * ecPlano[X];
    matrizSombra[1][1] = dot - posLuz[Y] * ecPlano[Y];
    matrizSombra[2][1] = 0.f - posLuz[Y] * ecPlano[Z];
    matrizSombra[3][1] = 0.f - posLuz[Y] * ecPlano[W];
    
    matrizSombra[X][2] = 0.f - posLuz[Z] * ecPlano[X];
    matrizSombra[1][2] = 0.f - posLuz[Z] * ecPlano[Y];
    matrizSombra[2][2] = dot - posLuz[Z] * ecPlano[Z];
    matrizSombra[3][2] = 0.f - posLuz[Z] * ecPlano[W];
    
    matrizSombra[X][3] = 0.f - posLuz[W] * ecPlano[X];
    matrizSombra[1][3] = 0.f - posLuz[W] * ecPlano[Y];
    matrizSombra[2][3] = 0.f - posLuz[W] * ecPlano[Z];
    matrizSombra[3][3] = dot - posLuz[W] * ecPlano[W];
    
}
void defPlano(float plano[4], float v0[3], float v1[3], float v2[3])
{
    GLfloat vec0[3], vec1[3];
    /* Producto cruz entre 2 vectores. */
    vec0[X] = v1[X] - v0[X];
    vec0[Y] = v1[Y] - v0[Y];
    vec0[Z] = v1[Z] - v0[Z];
    
    vec1[X] = v2[X] - v0[X];
    vec1[Y] = v2[Y] - v0[Y];
    vec1[Z] = v2[Z] - v0[Z];
    
    /* Encontrar producto cruz para A, B, y C en la ec. del plano */
    plano[A] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
    plano[B] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
    plano[C] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];
    
    plano[D] = -(plano[A] * v0[X] + plano[B] * v0[Y] + plano[C] * v0[Z]);
}

void texturaPiso(int num)
{
    GLubyte imagenRGB[16][16][3];
    GLubyte *col;
    int s, t;
    // Crear RGB para textura
    col = (GLubyte*) imagenRGB;
    for (t = 0; t < 16; t++) {
        for (s = 0; s < 16; s++) {
            if (textura[t][s] == 'x') {
                col[0] = 256;
                col[1] = 256;
                col[2] = 256;
            } else if (textura[t][s] == 'o') {
                col[0] = 0;
                col[1] = 0;
                col[2] = 0;
            } else if (textura[t][s] == 'p') {
                col[0] = 0;
                col[1] = 0;
                col[2] = 255;
            }else {
                if(num==1){
                    col[0] =0;
                    col[1] =0;
                    col[2] =255;
                }else
                {
                    if(num==2){
                    col[0] =255;
                    col[1] =0;
                    col[2] =0;
                }
                    else{
                        col[0] =255;
                        col[1] =255;
                        col[2] =255;
                    }
                }

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
void dibujaObjeto2(void)
{
    glPushMatrix();
    glTranslatef(0, 5, 0);
    
    GLfloat mat_p_ambient[] = {0.1745,0.01175,0.01175,1};
    GLfloat mat_p_diffuse[] = {0.61424,0.04136,0.04136,1};
    GLfloat mat_pspecular[] = {0.727811,0.626959,0.626959,1};
    GLfloat mat_pshininess[] = {18.2};
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_pspecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_pshininess);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_p_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_p_diffuse);
    glShadeModel(GL_SMOOTH);
    glutSolidCube(tamanio);
    glPopMatrix();
}

void drawFloor(void)
{
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            if(colored[i][j]==0){
                texturaPiso(0);
            }else if(colored[i][j]==1){
                texturaPiso(1);
            }else if(colored[i][j]==2){
                texturaPiso(2);
            }
            glDisable(GL_LIGHTING);
            glEnable(GL_TEXTURE_2D);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(-64+i*8, 0, -64+j*8);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(-64+(i+1)*8, 0, -64+j*8);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(-64+(i+1)*8, 0, -64+(j+1)*8);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(-64+i*8, 0, -64+(j+1)*8);
            glEnd();
            glDisable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
        }
    }
}

void checarColisiones(int avancex,int avancey){

    xpant1=xobj1; ypant1=yobj1;                             //guardo el paso anterior

    xobj1=xobj1+(avancex*paso*3);    //calculo los pasos nuevos
    yobj1=yobj1+(avancey*paso*3);
/*
    dist=sqrt((xobj-xmov)*(xobj-xmov)+(yobj-ymov)*(yobj-ymov));     //distancia con el obj movil

    if(dist<=tamanio*2 && (xmov>xobj || ymov>yobj) && down==tamanio){xmov=xobj+tamanio*2;ymov=yobj+tamanio*2;}
    if(dist<=tamanio*2 && (xmov<xobj || ymov<yobj) && down==tamanio){xmov=xobj-tamanio*2;ymov=yobj-tamanio*2;}
    if (xmov>=65 || xmov<=-65 || ymov>=65 || ymov<=-65){caer=1;} //para caer fuera del tablero
    else{caer=0;t=0;}

*/
    dist=sqrt((xobj1-xobj2)*(xobj1-xobj2)+(yobj1-yobj2)*(yobj1-yobj2));     //dist con el obj fijo
    if(dist<=tamanio){xobj1=xpant1;yobj1=ypant1;}
    else{
        posxmat1+=avancex;
        posymat1+=avancey;
        colored[posxmat1][posymat1]=1;
    }
    //deteccion col
/*

    dist=sqrt((xobj-xdona)*(xobj-xdona)+(yobj-ydona)*(yobj-ydona));     //dist con el obj a recoger
    if(dist<=tamanio){xdona=xobj;ydona=yobj;}         //deteccion col*/
}

void checarColisiones2(int avancex,int avancey){
    
    xpant2=xobj2; ypant2=yobj2;                             //guardo el paso anterior
    xobj2=xobj2+(avancex*paso*3);    //calculo los pasos nuevos
    yobj2=yobj2+(avancey*paso*3);
    
    dist=sqrt((xobj2-xobj1)*(xobj2-xobj1)+(yobj2-yobj1)*(yobj2-yobj1));     //dist con el obj fijo
    if(dist<=tamanio){xobj2=xpant2;yobj2=ypant2;}
    else{
        posxmat2+=avancex;
        posymat2+=avancey;
        colored[posxmat2][posymat2]=2;
    }

 /*
     dist=sqrt((xobj-xmov)*(xobj-xmov)+(yobj-ymov)*(yobj-ymov));     //distancia con el obj movil
     
     if(dist<=tamanio*2 && (xmov>xobj || ymov>yobj) && down==tamanio){xmov=xobj+tamanio*2;ymov=yobj+tamanio*2;}
     if(dist<=tamanio*2 && (xmov<xobj || ymov<yobj) && down==tamanio){xmov=xobj-tamanio*2;ymov=yobj-tamanio*2;}
     if (xmov>=65 || xmov<=-65 || ymov>=65 || ymov<=-65){caer=1;} //para caer fuera del tablero
     else{caer=0;t=0;}
     
     
     dist=sqrt((xobj-xfijo)*(xobj-xfijo)+(yobj-yfijo)*(yobj-yfijo));     //dist con el obj fijo
     if(dist<=tamanio*2){xobj=xpant;yobj=ypant;}         //deteccion col
     
     
     dist=sqrt((xobj-xdona)*(xobj-xdona)+(yobj-ydona)*(yobj-ydona));     //dist con el obj a recoger
     if(dist<=tamanio){xdona=xobj;ydona=yobj;}
  */
 }


void puntaje(void){
    vacio=0;
    rojo=0;
    azul=0;
    for(int i=0;i<16;i++){
        for(int y=0;y<16;y++){
            if(colored[i][y]==0)vacio++;
            if(colored[i][y]==1)azul++;
            if(colored[i][y]==2)rojo++;
        }
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    

    
    shadowMatrix(sombraPiso, planoPiso, posicionLuz);


  
    glPushMatrix();
    glRotatef(0/4, 1, 0, 0);  //rotar arriba/abajo con mouse

    angr=0*3.141592/180;        //convertir a rad rotacion en y

    x2 =  cos(angr) + 80*sin(angr); //rotar la camara
    y2 = -sin(angr) + 80*cos(angr); //con matriz de rotacion

    gluLookAt(x2,150,y2+100,       // posicion de camara
              0,0,0,              // Hacia donde ve (objetivo)
              0,1,0);               // Eje de rotacion



    
    glLightfv(GL_LIGHT0, GL_POSITION, posicionLuz);//Reacomodar la luz
    
    //*************************************************************
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 1.0, 1.0, 0.6);
    drawFloor();
    
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    glColor4f(0.0, 0.0, 0.0, 0.2);
    glMultMatrixf((GLfloat *) sombraPiso);
    //Sombra
    glTranslatef(xobj1, 0, yobj1);    //trasladar objeto
    glRotatef(angle, 0, 1, 0);  //rotar objeto sobre y
    dibujaObjeto();
    glPopMatrix();
    
    glPushMatrix();
    glColor4f(0.0, 0.0, 0.0, 0.2);
    glMultMatrixf((GLfloat *) sombraPiso);
    //Sombra
    glTranslatef(xobj2, 0, yobj2);    //trasladar objeto
    glRotatef(angle, 0, 1, 0);  //rotar objeto sobre y
    dibujaObjeto2();
    glPopMatrix();
    
    
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    //dibujo objeto solito
    glPushMatrix();
    glTranslatef(xobj1, 0, yobj1);    //trasladar objeto
    glRotatef(angle, 0, 1, 0);  //rotar objeto sobre y
    dibujaObjeto();             //cubo
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(xobj2, 0, yobj2);    //trasladar objeto
    glRotatef(angle, 0, 1, 0);  //rotar objeto sobre y
    dibujaObjeto2();             //cubo
    glPopMatrix();
    
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 0.7);
    glTranslatef(posicionLuz[0], posicionLuz[1], posicionLuz[2]);
    glEnable(GL_LIGHTING);
    
    glPopMatrix();
    

    glPopMatrix();


    glutSwapBuffers();
    puntaje();
    printf("Posicion x=%d\n",posxmat1);
    printf("Posicion y=%d\n",posymat1);
    printf("Puntaje azul=%d\n",azul);
    printf("Puntaje rojo=%d\n",rojo);
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

void temp(int value){
    switch (value) {
        case 1:
            if(time == 0){
                gameOver = 1;
            }else{
                time --;
                glutTimerFunc(1000, temp, 1);
            }
            break;
        case 2:
            
        default:
            break;
    }
}

static void
key(unsigned char c, int x, int y)
{
    if (c == 27) {
        exit(0);
    }
    if(gameOver!=1){
        if (c == 'd' && xobj1<=52.0) {
            checarColisiones(1,0);
            // angle=angle-5;
        }
        if (c == 'a' && xobj1>=-52.0) {
            // angle=angle+5;
            checarColisiones(-1,0);
        }
        if (c == 's' && yobj1<=61.0) {
            checarColisiones(0,1);                   //revisar colision conra movil

        }
        if (c == 'w' && yobj1>=-54.0) {
            checarColisiones(0,-1);                   //revisar colision conra movil

        }
        if (c == 'r') {
            xmov=0;ymov=0;down=tamanio,caer=0;
        }
        glutPostRedisplay();
    }
}
void spkey(int key,int x, int y){
    if(gameOver!=1){
        switch(key){
            case GLUT_KEY_UP:
                if (yobj2>=-54.0)checarColisiones2(0,-1);
                break;
            case GLUT_KEY_DOWN:
                if (yobj2<=61.0)checarColisiones2(0,1);
                break;
            case GLUT_KEY_LEFT:
                if (xobj2>=-52.0)checarColisiones2(-1,0);            break;
            case GLUT_KEY_RIGHT:
                 if (xobj2<=52.0)checarColisiones2(1,0);
                break;
        }
        glutPostRedisplay();
    }
}

void init(void){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(40.0,1.0,20.0,500.0);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    posicionLuz[0] = 0;
    posicionLuz[1] = 70;
    posicionLuz[2] = 0;
    posicionLuz[3] = 0.8;//luz posicional o direccional
    glLightfv(GL_LIGHT0, GL_POSITION, posicionLuz); /*light position. */
    texturaPiso(0);
    glClearColor(1, 1, 1, 1);
    glLineWidth(2);
    defPlano(planoPiso, verticesPiso[1], verticesPiso[2], verticesPiso[3]);//Plano para sombra}
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
    glutTimerFunc(1000, temp, 1);
    glutKeyboardFunc(key);
    glutSpecialFunc(spkey);
    init();
    initMatriz();

    glutMainLoop();
    return 0;
}
