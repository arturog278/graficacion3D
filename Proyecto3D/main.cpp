#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include "/Users/arturo/Downloads/glm.h"
#include "/Users/arturo/Downloads/glm.c"
#else
#include <windows.h>
#include <GL/glut.h>
#endif

GLMmodel* modelInicia;
GLMmodel* model1;
GLMmodel* model2;
GLMmodel* model3;
GLMmodel* model4;
GLMmodel* model5;
GLMmodel* modelFin;
int modelID = 0;
int timerInicial = 1;

int transx =0,transy=0,vacio=0,azul=0,rojo=0;
int xsorm,ysorm,xsorp,ysorp,magnitud,sorUp=0;
static GLdouble tamanio = 6.0;
static GLfloat posicionLuz[6];
float xobj1=-51.26666,yobj1=69.73333,xpant1=0,ypant1=0,paso=2.622222222*3;  //variables camara y objetivo
float dist=0;//objeto colision

float xobj2=66.73333,yobj2=-48.2666,xpant2=0,ypant2=0;

int colored[18][18];
int gameOver = 1;
int time = 40;
int timeInit = 5;
float izquierda=-50.0,derecha=60.0,arriba=-48.0,abajo=65.0;
int posxmat1 = 1, posymat1 = 16;
int posxmat2 = 16, posymat2 = 1;
enum {X, Y, Z, W};
enum {A, B, C, D};
float scaleModel=0;

static GLfloat planoPiso[4];
static GLfloat sombraPiso[4][4];


void dibujaModelo(int modelID){
    glPushMatrix();
    glRotatef(-90, 1.0, 0.0, 0.0);
    switch (modelID) {
        case 1:
            glmUnitize(model1);
            glmScale(model1, scaleModel);
            glmDraw(model1,GLM_MATERIAL);
            break;
        case 2:
            glmUnitize(model2);
            glmScale(model2, scaleModel);
            glmDraw(model2,GLM_MATERIAL);
            break;
        case 3:
            glmUnitize(model3);
            glmScale(model3, scaleModel);
            glmDraw(model3,GLM_MATERIAL);
            break;
        case 4:
            glmUnitize(model4);
            glmScale(model4, scaleModel);
            glmDraw(model4,GLM_MATERIAL);
            break;
        case 5:
            glmUnitize(model5);
            glmScale(model5, scaleModel);
            glmDraw(model5,GLM_MATERIAL);
            break;
        case 6:
            glmUnitize(modelInicia);
            glmScale(modelInicia, scaleModel);
            glmDraw(modelInicia,GLM_MATERIAL);
            break;
        case 7:
            glmUnitize(modelFin);
            glmScale(modelFin, scaleModel);
            glmDraw(modelFin,GLM_MATERIAL);
            break;
        default:
            break;
    }

}

void generaSorpresa(){
    xsorm = rand() % 16 + 1;
    ysorm = rand() % 16 + 1;
    xsorp = -51.26666+(xsorm-1)*paso;
    ysorp = -48.26666+(ysorm-1)*paso;
    magnitud = rand() % 4 + 2;
    sorUp=1;
}

void temp(int value){
    switch (value) {
        case 1:
            //Timer de juego
            printf("Entrando a temp 1\n");
            printf("Time = %d\n",time);
            if(time != 0){
                if(time==5){
                    timeInit = 5;
                    timerInicial = 0;
                    glutTimerFunc(0, temp, 3);
                }
                time --;
                glutTimerFunc(1000, temp, 1);
            }
            break;
        case 2:
            //Timer de animacion de modelos para numeros
            printf("Entrando a temp 2\n");
            printf("ScaleModel = %f\n",scaleModel);
            if(scaleModel!=5){
                scaleModel+=0.5;
                glutTimerFunc(100, temp, 2);
                glutPostRedisplay();
            }else{
                glutTimerFunc(0, temp, 3);
            }
            break;
        case 3:
            //Timer de 5 segundos
            printf("Entrando a temp 3\n");
            printf("timeInit = %d\n",timeInit);
            if (timeInit!=0) {
                modelID = timeInit;
                scaleModel = 0;
                glutTimerFunc(0, temp, 2);
                timeInit--;
            }else{
                if (timerInicial==1) {
                    modelID = 6;
                    gameOver = 0;
                }else{
                    modelID = 7;
                    gameOver = 1;
                }
                scaleModel = 0;
                glutTimerFunc(0, temp, 4);
            }
            break;
        case 4:
            //Timer de animacion para texto
            if (scaleModel!=20) {
                scaleModel+=0.5;
                glutTimerFunc(20, temp, 4);
                glutPostRedisplay();
            }else{
                //scaleModel = 0;
                modelID = 0;
                if(timerInicial==1){
                    glutTimerFunc(0, temp, 1);
                    glutTimerFunc(5000, temp, 5);
                }
                glutPostRedisplay();
            }
            break;
        case 5:
        {
            generaSorpresa();
            glutPostRedisplay();
            break;
        }
        default:
            break;
    }
}

void initMatriz(){
    for(int i=0;i<18;i++){
        for(int y=0;y<18;y++){
            colored[i][y]=0;
        }
    }
    colored[1][16]=1;
    colored[16][1]=2;
}


void pintaSorpresa(int player){
    for(int i = -magnitud ; i<=magnitud ;  i++){
        for(int j = -magnitud; j<=magnitud;j++){
            int xpint = xsorm+i;
            int ypint = ysorm+j;
            if(xpint>0 && xpint<17 && ypint>0 && ypint<17){
                colored[xpint][ypint] = player;
            }
        }
    }
    if(time>=4){
        glutTimerFunc(3000, temp, 5);
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
                }else if(num==2){
                    col[0] =255;
                    col[1] =0;
                    col[2] =0;
                }else if(num==3){
                    col[0] =128;
                    col[1] =128;
                    col[2] =128;
                }else{
                    col[0] =255;
                    col[1] =255;
                    col[2] =255;
                }

            }
            col += 3;
        }
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 16, 0,
                GL_RGB, GL_UNSIGNED_BYTE, imagenRGB);
}

void dibujaObjeto(int mat)
{
    glPushMatrix();
    glTranslatef(0, 5, 0);

    switch (mat) {
        case 1: //Jugador Azul
        {
             GLfloat mat_p_ambient[] = {0.4725,0.4245,0.8645,1};
             GLfloat mat_p_diffuse[] = {0.34615,0.5143,0.8903,1};
             GLfloat mat_pspecular[] = {1.797357,1.723991,1.708006,1};
             GLfloat mat_pshininess[] = {18.2};
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_pspecular);
            glMaterialfv(GL_FRONT, GL_SHININESS, mat_pshininess);
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_p_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_p_diffuse);
            break;
        }
        case 2: //Jugador Red
        {
            GLfloat mat_p_ambient[] = {0.1745,0.01175,0.01175,1};
            GLfloat mat_p_diffuse[] = {0.61424,0.04136,0.04136,1};
            GLfloat mat_pspecular[] = {0.727811,0.626959,0.626959,1};
            GLfloat mat_pshininess[] = {18.2};
            
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_pspecular);
            glMaterialfv(GL_FRONT, GL_SHININESS, mat_pshininess);
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_p_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_p_diffuse);
            break;
        }
        case 3: //Sorpresa
        {
            GLfloat mat_p_ambient[] = {0.1745,0.01175,0.01175,1};
            GLfloat mat_p_diffuse[] = {0.61424,0.04136,0.04136,1};
            GLfloat mat_pspecular[] = {0.727811,0.626959,0.626959,1};
            GLfloat mat_pshininess[] = {18.2};
            
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_pspecular);
            glMaterialfv(GL_FRONT, GL_SHININESS, mat_pshininess);
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_p_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_p_diffuse);
            break;
        }
            
        default:
            break;
    }
    

    glShadeModel(GL_SMOOTH);
    glutSolidCube(tamanio);
    glPopMatrix();
}

void drawFloor(void)
{
    for(int i=0;i<18;i++){
        for(int j=0;j<18;j++){
            if(i==0||j==0||i==17||j==17){
                texturaPiso(3);
            }else{
                if(colored[i][j]==0){
                    texturaPiso(0);
                }else if(colored[i][j]==1){
                    texturaPiso(1);
                }else if(colored[i][j]==2){
                    texturaPiso(2);
                }
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

    xobj1=xobj1+(avancex*paso);    //calculo los pasos nuevos
    yobj1=yobj1+(avancey*paso);
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
    
    dist=sqrt((xobj1-xsorp)*(xobj1-xsorp)+(yobj1-ysorp)*(yobj1-ysorp));     //dist con el obj a recoger
    if(dist<=tamanio && sorUp){
        sorUp = 0;
        pintaSorpresa(1);
    }
    //deteccion col
/*

    dist=sqrt((xobj-xdona)*(xobj-xdona)+(yobj-ydona)*(yobj-ydona));     //dist con el obj a recoger
    if(dist<=tamanio){xdona=xobj;ydona=yobj;}         //deteccion col*/
}

void checarColisiones2(int avancex,int avancey){
    
    xpant2=xobj2; ypant2=yobj2;                             //guardo el paso anterior
    xobj2=xobj2+(avancex*paso);    //calculo los pasos nuevos
    yobj2=yobj2+(avancey*paso);
    
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
     
     */
     dist=sqrt((xobj2-xsorp)*(xobj2-xsorp)+(yobj2-ysorp)*(yobj2-ysorp));     //dist con el obj a recoger
     if(dist<=tamanio && sorUp){
         sorUp = 0;
         pintaSorpresa(2);
     }

 }


void puntaje(void){
    vacio=0;
    rojo=0;
    azul=0;
    for(int i=0;i<18;i++){
        for(int y=0;y<18;y++){
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
    
    gluLookAt(8,150,220,       // posicion de camara
              8,0,0,              // Hacia donde ve (objetivo)
              0,1,0);               // Eje de rotacion


    
    glLightfv(GL_LIGHT0, GL_POSITION, posicionLuz);//Reacomodar la luz
    
    //*************************************************************
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 1.0, 1.0, 0.6);
    drawFloor();

    /*glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 1.0, 1.0, 0.6);
    drawFloor2();
    */
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    glColor4f(0.0, 0.0, 0.0, 0.2);
    glMultMatrixf((GLfloat *) sombraPiso);
    //Sombra
    glTranslatef(xobj1, 0, yobj1);    //trasladar objeto
    dibujaObjeto(1);
    glPopMatrix();
    
    glPushMatrix();
    glColor4f(0.0, 0.0, 0.0, 0.2);
    glMultMatrixf((GLfloat *) sombraPiso);
    //Sombra
    glTranslatef(xobj2, 0, yobj2);    //trasladar objeto
    dibujaObjeto(2);
    glPopMatrix();
    
    if(sorUp){
        glPushMatrix();
        glColor4f(0.0, 0.0, 0.0, 0.2);
        glMultMatrixf((GLfloat *) sombraPiso);
        //Sombra
        glTranslatef(xsorp, 0, ysorp);    //trasladar objeto
        dibujaObjeto(3);
        glPopMatrix();
    }
    
    
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    //dibujo objeto solito
    glPushMatrix();
    glTranslatef(xobj1, 0, yobj1);    //trasladar objeto
    dibujaObjeto(1);//cub
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(xobj2, 0, yobj2);    //trasladar objeto
    dibujaObjeto(2);             //cubo
    glPopMatrix();
    
    if(sorUp){
        glPushMatrix();
        glTranslatef(xsorp, 0, ysorp);    //trasladar objeto
        dibujaObjeto(3);             //cubo
        glPopMatrix();
    }
    
    glPushMatrix();
    glTranslatef(10, 80, 0);
    dibujaModelo(modelID);
    glPopMatrix();
    
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 0.7);
    glTranslatef(posicionLuz[0], posicionLuz[1], posicionLuz[2]);
    glEnable(GL_LIGHTING);
    
    glPopMatrix();
    

    glPopMatrix();


    glutSwapBuffers();
    puntaje();
//    printf("Posicion x=%f\n",xobj1);
//    printf("Posicion y=%f\n",yobj1);
//    printf("Puntaje azul=%d\n",azul);
//    printf("Puntaje rojo=%d\n",rojo);
}

static void
key(unsigned char c, int x, int y)
{
    if (c == 27) {
        exit(0);
    }
    if(gameOver==0){
        if (c == 'd' && xobj1<=derecha) {
            checarColisiones(1,0);
            // angle=angle-5;
        }
        if (c == 'a' && xobj1>=izquierda) {
            // angle=angle+5;
            checarColisiones(-1,0);
        }
        if (c == 's' && yobj1<=abajo) {
            checarColisiones(0,1);                   //revisar colision conra movil

        }
        if (c == 'w' && yobj1>=arriba) {
            checarColisiones(0,-1);                   //revisar colision conra movil

        }
        glutPostRedisplay();
    }else if(gameOver==1){
        if (c == 'r') {
            xobj1=-51.26666,yobj1=69.73333,xpant1=0,ypant1=0;
            xobj2=66.73333,yobj2=-48.2666,xpant2=0,ypant2=0;
            sorUp = 0;
           // gameOver = 0;
            time = 40;
            posxmat1 = 1, posymat1 = 16;
            posxmat2 = 16, posymat2 = 1;
            initMatriz();
           // glutTimerFunc(1000, temp, 1);
            timeInit=5;
            timerInicial = 1;
        }else if (c == 'p' && time!=0){
            gameOver = 2;
            glutTimerFunc(0, temp, 3);
            //glutTimerFunc(100, temp, 2);
        }
        glutPostRedisplay();
    }
}
void spkey(int key,int x, int y){
    if(gameOver==0){
        switch(key){
            case GLUT_KEY_UP:
                if (yobj2>=arriba)checarColisiones2(0,-1);
                break;
            case GLUT_KEY_DOWN:
                if (yobj2<=abajo)checarColisiones2(0,1);
                break;
            case GLUT_KEY_LEFT:
                if (xobj2>=izquierda)checarColisiones2(-1,0);
                break;
            case GLUT_KEY_RIGHT:
                 if (xobj2<=derecha)checarColisiones2(1,0);
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

void initModels(){
    model1 = glmReadOBJ("/Users/arturo/Downloads/modelos/n1.obj");
    glmReadMTL(model1,"n1.mtl");

    model2 = glmReadOBJ("/Users/arturo/Downloads/modelos/n2.obj");
    glmReadMTL(model2,"n2.mtl");
    
    model3 = glmReadOBJ("/Users/arturo/Downloads/modelos/n3.obj");
    glmReadMTL(model3,"n3.mtl");
    
    model4 = glmReadOBJ("/Users/arturo/Downloads/modelos/n4.obj");
    glmReadMTL(model4,"n4.mtl");
    
    model5 = glmReadOBJ("/Users/arturo/Downloads/modelos/n5.obj");
    glmReadMTL(model5,"n5.mtl");
    
    modelInicia = glmReadOBJ("/Users/arturo/Downloads/modelos/Empieza.obj");
    glmReadMTL(modelInicia,"Empieza.mtl");
    
    modelFin = glmReadOBJ("/Users/arturo/Downloads/modelos/fin.obj");
    glmReadMTL(modelFin,"fin.mtl");
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);

    glutInitWindowSize(1024, 800);

    glutCreateWindow("CAMARA Y COLISION");

    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(spkey);
    init();
    initMatriz();
    initModels();

    glutMainLoop();
    return 0;
}
