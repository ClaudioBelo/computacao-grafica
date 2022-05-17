// Trabalho 3 de computacao grafica
// Claudio Belo Rodrigues Junior RA 2483
// Gustavo Furlan RA 2576

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Parametros de configuracao
int blockHeight = 20; // Altura dos blocos 
int blockWidth = 50; // Largura dos blocos
int goalBlockHeight = 40; // Altura da chegada
int goalBlockWidth = 60; // Largura dos blocos de chegada
int windowHeight = 600; // Altura da tela
int windowWidth = 300; // Largura da tela

// Parametros do jogo
int fails = 0;
int maxScore = 0;
int score = 0;
int step = 2;
int timerGen = 4000;
int timerStep = 1000/30;
bool paused = false;
bool pausedGen = false;
bool pausedStep = false;
bool pressedBlocks[5] = {false, false, false, false, false};
enum Keys{
    letterA = 97, // A
    letterS = 115, // S
    letterJ = 106, // J
    letterK = 107, // K
    letterL = 108, // L
    keyPlusDifficult = 61, // +
    keyMinusDifficult = 45,
    keySpace = 32  // -
};
float blockColors[5][3] = {
    {0.231f, 0.925f, 0.133f}, // Verde
    {0.925f, 0.133f, 0.231f}, // Vermelho
    {0.925f, 0.862f, 0.133f}, // Amarelo
    {0.133f, 0.231f, 0.925f}, // Azul
    {0.925f, 0.550f, 0.0f} // Laranja
};
typedef struct {
    int index;
    int height;
} block;
block blocks[100] = {};
int blockPointer = 0;

// Assinatira das Funcoes
void bindKeyboard(unsigned char key,int x, int y);
void bindUpKeyboard(unsigned char key,int x, int y);
void blockPressed(int index, bool isDown);
void configGlut();
void configKeyboard();
void clearDisplay();
void drawBlocks();
void drawDisplayBase();
void drawInfo();
void drawRectangle(int xcenter, int ycenter, int width, int height);
void drawStaticLines();
void funcReshape();
void genBlocks();
int genRandomInt(int min, int max);
void removeBlock(int positionBlock);
void resetGame();
void showInfo();
void stepBlocks();

int main(int argc, char** argv){
    glutInit(&argc, argv);
    configGlut();
    glutTimerFunc(timerGen, genBlocks, 1);
    glutTimerFunc(timerStep, stepBlocks, 1);
    glutMainLoop();
}

void bindKeyboard(unsigned char key,int x, int y){
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case letterA:
            blockPressed(0, true);
            break;
        case letterS:
            blockPressed(1, true);
            break;
        case letterJ:
            blockPressed(2, true);
            break;
        case letterK:
            blockPressed(3, true);
            break;
        case letterL:
            blockPressed(4, true);
            break;
        case keyPlusDifficult:
            step += 1;
            break;
        case keyMinusDifficult:
            step -= 1;
            break;
        case keySpace:
            paused = !paused;
            showInfo();
            if (!paused && pausedGen){
                pausedGen = false;
                glutTimerFunc(genRandomInt(200, 2000), genBlocks, 1);
            }
            if (!paused && pausedStep){
                pausedStep = false;
                glutTimerFunc(10, stepBlocks, 1);
            }
            break;
        // default:
        //     printf("%d\n", key);
        //     break;
    }
}

void bindUpKeyboard(unsigned char key,int x, int y){
    switch (key)
    {
        case letterA:
            blockPressed(0, false);
            break;
        case letterS:
            blockPressed(1, false);
            break;
        case letterJ:
            blockPressed(2, false);
            break;
        case letterK:
            blockPressed(3, false);
            break;
        case letterL:
            blockPressed(4, false);
            break;
    }
}

void blockPressed(int index, bool isDown){
    if (pressedBlocks[index] == isDown){
        return;
    }

    pressedBlocks[index] = isDown;
    if (isDown){
        bool hit = false;
        for (int i = 0; i < blockPointer;i++){
            if (blocks[i].height > goalBlockHeight){
                break;
            } else if (blocks[i].index == index){
                hit = true;
                removeBlock(i);
                break;
            }
        }
        if (hit){
            score += step;
        } else {
            score -= step*2;
            if (score < 0 )
                score = 0;
        }
    }
    drawDisplayBase();
    if (!paused)
        showInfo();
}

void configGlut(){
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Trabalho 3 - Time Block.");
    glutDisplayFunc(drawDisplayBase);
    glutReshapeFunc(funcReshape);
    configKeyboard();
}

void configKeyboard(){
    glutKeyboardFunc(bindKeyboard);
    glutKeyboardUpFunc(bindUpKeyboard);
}

void clearDisplay() {
    glClearColor(0.9f,0.9f,0.9f,0.9f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void drawBlocks(){
    for (int index = 0;index < sizeof(pressedBlocks); index++){
        if (!pressedBlocks[index]) {
            continue;
        }

        int column = (index+1);
        int xcenter = 0 + goalBlockWidth*column - goalBlockWidth/2;
        int ycenter = goalBlockHeight/2;

        glColor3f(
            blockColors[index][0],
            blockColors[index][1],
            blockColors[index][2]
        );
        drawRectangle(xcenter, ycenter, goalBlockWidth, goalBlockHeight);
    }

    for (int index = 0; index < blockPointer;index++){

        int column = (blocks[index].index+1);
        int xcenter = 0 + goalBlockWidth*column - goalBlockWidth/2;
        int ycenter = blocks[index].height + blockHeight/2;

        glColor3f(
            blockColors[blocks[index].index][0],
            blockColors[blocks[index].index][1],
            blockColors[blocks[index].index][2]
        );
        drawRectangle(xcenter, ycenter, blockWidth, blockHeight);        
    }
}

void drawDisplayBase(){
    clearDisplay();

    // Escreve os desenhos estaticos da tela
    drawStaticLines();

    // Desenha os blocos em tela
    drawBlocks();

    glutSwapBuffers();    
}

void drawRectangle(int xcenter, int ycenter, int width, int height){
    int x = xcenter - width / 2;
    int y = ycenter - height / 2;
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+width,y);
    glVertex2f(x+width,y+height);
    glVertex2f(x,y+height);
    glEnd();
}

void drawStaticLines(){
    glColor3f(0.1f,0.1f,0.1f);
    glBegin(GL_LINES);
    glVertex2f(0,goalBlockHeight);
    glVertex2f(windowWidth,goalBlockHeight);
    glEnd();

    for (int column = 1;column < sizeof(pressedBlocks); column++){
        glBegin(GL_LINES);
        glVertex2f(goalBlockWidth * column,0);
        glVertex2f(goalBlockWidth * column,goalBlockHeight);
        glEnd();
    }

    glColor3f(0.8f,0.8f,0.8f);
    for (int column = 1;column <= sizeof(pressedBlocks); column++){
        int halfGoalBlockSize = goalBlockWidth / 2;
        glBegin(GL_LINES);
        glVertex2f(goalBlockWidth * column - halfGoalBlockSize,windowHeight - 20);
        glVertex2f(goalBlockWidth * column - halfGoalBlockSize,goalBlockHeight + 20);
        glEnd();
    }
}

void funcReshape(){
    glViewport(0,0,windowWidth,windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0f,windowWidth,0.0f,windowHeight);
    glMatrixMode(GL_MODELVIEW);
}

void genBlocks(){
    if (paused){
        pausedGen = true;
        return;
    } 

    block newBlock;
    newBlock.index = genRandomInt(0, (int)sizeof(pressedBlocks));
    newBlock.height = windowHeight;
    blocks[blockPointer] = newBlock;
    blockPointer++;
    
    glutTimerFunc(genRandomInt(1200/step, 6000/step), genBlocks, 1);
}

int genRandomInt(int min, int max) {
    time_t t;
    srand((unsigned)time(&t));
    int number = rand() % max;
    if (number < min){
        number = min;
    }
    return number;
}

void removeBlock(int positionBlock){
    for (int i = positionBlock; i < blockPointer;i++){
        blocks[i] = blocks[i+1];
    }
    blockPointer--;
}

void resetGame(){
    paused = true;
    printf("---------------------------\n");
    printf("\n\nFIM DE JOGO...\n");
    printf("---------------------------\n");
    showInfo();
    printf("Dificuldade: %d\n", step);
    printf("Precione espaco para continuar jogando.\n");
    printf("---------------------------\n\n");
    fails = 0;
    if (score > maxScore) {
        maxScore = score;
    }
    score = 0;
    for (int i = 0; i < blockPointer;i++){
        blocks[i] = blocks[i-1];
    }
    blockPointer = 0;
}

void showInfo(){
    printf("Recorde: %d\n", maxScore);
    printf("Pontuacao: %d\n", score);
    printf("Falhas: %d\n", fails);
}

void stepBlocks(){
    if (paused){
        pausedStep = true;
        return;
    } 

    for (int index = 0; index < blockPointer;index++){
        blocks[index].height -= (step + (int)(score / 400));
        if (blocks[index].height < 0) {
            removeBlock(index);
            fails++;
            if (fails >= 10){
                resetGame();
                break;
            }
        }
    }
    drawDisplayBase();
    glutTimerFunc(10, stepBlocks, 1);
}
