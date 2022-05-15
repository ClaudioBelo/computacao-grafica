#include <GL/glut.h>
#include <stdio.h>

void init(void);
void draw(void);
void bind_keyboard(unsigned char key,int x, int y);
float vectors_triangle[]={-0.5,-0.5,0.5,-0.5,0.0,0.5};
float vectors_square[]={-0.5,-0.5,0.5,-0.5,0.5,0.5,-0.5,0.5};
float colors[]={1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0};

int main(int argc, char** argv){
    glutInit(&argc, argv);
    init();
    glutDisplayFunc(draw); // Função que desenha da tela
    glutKeyboardFunc(bind_keyboard); // Função que configura teclas que serão escutadas
    glutMainLoop(); // Loop para não fechar a tela

    return 0;
}

void init(void){//Configuracao de parametros de tela
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutCreateWindow("Window");
    glClearColor(1.0,1.0,1.0,1.0);//cores de fundo da tela RGBA
}

void draw(){
    glClear(GL_COLOR_BUFFER_BIT);//limpa a configuracao de tela
    glLoadIdentity();
    
    //Primeira figura
    //Transformações geométricas
    //x y z
    glTranslatef(0.0,0.5,0.0);
    glScalef(0.5,0.5,0.0);
    glRotatef(45,0.0,0.0,1.0);

    //Triangulo
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2,GL_FLOAT,2*sizeof(float),vectors_triangle);
    glColorPointer(3,GL_FLOAT,3*sizeof(float),colors);
    glDrawArrays(GL_TRIANGLES,0,3);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    
	//Segunda figura
    //Transformações geométricas
    //x y z
    glTranslatef(0.0,0.5,0.0);
    glScalef(0.5,0.5,0.0);
    glRotatef(45,0.0,0.0,1.0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2,GL_FLOAT,2*sizeof(float),vectors_square);
    glColorPointer(3,GL_FLOAT,3*sizeof(float),colors);
    glDrawArrays(GL_QUADS,0,4);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glFlush();
}

void bind_keyboard(unsigned char key,int x, int y){
    switch (key){
        case 27: //tecla esc
            exit(0);
            break;
    }
}