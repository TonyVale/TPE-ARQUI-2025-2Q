#include <tronApp.h>
#include <globalLib.h>
typedef struct {
    char player;
    int nextDirectionX;
    int nextDirectionY;
} squareType;

//PLAYER1:
int headCoordX1;
int headCoordY1;

char input1;

int dirX1;
int dirY1;


//PLAYER2:
int headCoordX2;
int headCoordY2;

char input2;
int dirX2;
int dirY2;


//GLOBAL:
squareType grid[20][28];
char gameNotOver;
char gameNotOver2;

char gameMode;

void startGame(){
    
    char charAux = 0;
    initializationScreen();

    while(charAux == 0){
        charAux = getChar();
        if (charAux == 9){
            initializationScreen();
        }
        if(charAux != 49 && charAux !=50){
            charAux = 0;
        } 
    }
    clearGrid();

    gameNotOver = 1;
    gameNotOver2 = 1;
    
    initializeTronDisplay();
    

    if (charAux == 49){ //SINGLE PLAYER MODE
        gameNotOver2 = 0;
        gameMode = 1;
        gameEngine();
    } else if (charAux == 50){ //2-PLAYER MODE
        gameMode = 2;
        gameNotOver2 = 1;
        gameEngine2P();
    }   
}

void gameEngine(){

    char inputAux;
    int seconds = seconds_elapsed(); 
    
    loadp1();

    while (gameNotOver){
        inputAux = getKeyDown();
        if(inputAux != -1){
            input1 = inputAux;
        }

        if(seconds_elapsed() != seconds){
            interpretInput1();
            seconds++;
            grid[headCoordX1][headCoordY1].nextDirectionX = 0;
            grid[headCoordX1][headCoordY1].nextDirectionY = 0;

            headCoordX1 += dirX1;
            headCoordY1 += dirY1;

            refreshScreen();
                
            checkIfOver();
            grid[headCoordX1][headCoordY1].player = 1;
        }
    }

    sound(800, 10);
    gameOverMenu();

}

void gameEngine2P(){

    char gameNotOverForAll = 1;
    char inputAux = 0;
    int seconds = seconds_elapsed(); 


    loadp1();
    loadp2();

    while (gameNotOverForAll){
        
        inputAux = getKeyDown();
        if((inputAux == 65)|(inputAux == 68)|(inputAux == 83)|(inputAux == 87)|(inputAux == 9)){
            input1 = inputAux;
        } else if ((inputAux == 73)|(inputAux == 74)|(inputAux == 75)|(inputAux == 76)){ 
            input2 = inputAux;
        }

    if(seconds_elapsed() != seconds){

            interpretInput1();
            interpretInput2();

            seconds++;

            grid[headCoordX1][headCoordY1].nextDirectionX = dirX1;
            grid[headCoordX1][headCoordY1].nextDirectionY = dirY1;

            grid[headCoordX2][headCoordY2].nextDirectionX = dirX2;
            grid[headCoordX2][headCoordY2].nextDirectionY = dirY2;

            headCoordX1 += dirX1;
            headCoordY1 += dirY1;

            headCoordX2 += dirX2;
            headCoordY2 += dirY2;

            refreshScreen();
         
            checkIfOver();
            grid[headCoordX1][headCoordY1].player = 1;
            
            checkIfOverP2();
            grid[headCoordX2][headCoordY2].player = 2; 

            if (gameNotOver == 0 || gameNotOver2 == 0){
                gameNotOverForAll = 0;
            }
        }
    }

    sound(800, 10);
    gameOverMenu();
    
}


void initializationScreen(){
    largerFontSize();
    largerFontSize();
    ClearScreen(0x003D3D3D);
    NewLine();
    printf("TRON GAME");
    NewLine();
    NewLine();
    printf("TO START SINGLE PLAYER MODE, PRESS 1");
    NewLine();
    printf("TO START 2-PLAYER MODE, PRESS 2");
    NewLine();
    printf("PLAYER 1 (BLUE) CONTROLS: A, W, S, D");
    NewLine();
    printf("PLAYER 2 (YELLOW) CONTROLS: J, I, K, L");
    NewLine();
    NewLine();
    printf("WARNING!");
    NewLine();
    printf("PRESSING TAB for viewing registers");
    NewLine();
    printf("CAUSES AN AUTOMATIC GAME OVER");
}

void clearGrid(){
    for(int j = 0; j < 20; j++){
        for(int i = 0; i < 28; i++){
            grid[i][j].player = 0;
            grid[i][j].nextDirectionX = 0;
            grid[i][j].nextDirectionY = 0;
        }
    }
}

void initializeTronDisplay(){
    ClearScreen(0x003D3D3D);
    int initialY = 44;
    for(int j = 1; j <= 20; j++){
        int initialX = 36;
        for(int i = 1; i <= 28; i++){
            if((i + j) % 2 == 0){
                drawRectangle(initialX, initialY, 34, 34, 0x00FFFFFF);
            } else {
                drawRectangle(initialX, initialY, 34, 34, 0x00CCCCCC);
            }
            initialX += 34;
        }
        initialY += 34;
    }
}


void loadp1(){

    headCoordX1 = 5;
    headCoordY1 = 5;
    dirX1 = 1;
    dirY1 = 0;

    grid[5][5].player = 1;

    drawRectangle(36 + (headCoordX1) * 34 , 44 + headCoordY1 * 34, 34, 34, 0x000000FF);

}

void loadp2(){

    headCoordX2 = 22;
    headCoordY2 = 14;
    dirX2 = -1;
    dirY2 = 0;

    grid[22][14].player = 2;

    drawRectangle(36 + (headCoordX2) * 34 , 44 + headCoordY2 * 34, 34, 34, 0x00FFFF00);
    
}

void refreshScreen(){

    drawRectangle(36 + headCoordX1 * 34 , 44 + headCoordY1 * 34, 34, 34, 0x000000FF);
    //PrintScore1: 
    drawRectangle(36 + 3 * 34 + 10 * 16, 14, 8 * 16 ,16,0x003D3D3D);
    char scoreBuffer[8];

    //PrintScore2 If necesary
    if(gameNotOver2 == 1){
        drawRectangle(36 + headCoordX2 * 34 , 44 + headCoordY2 * 34, 34, 34, 0x00FFFF00);
        drawRectangle(36 + (3 + 14) * 34 + 10 * 16, 14, 8 * 16 ,16,0x003D3D3D);
    }
}

void checkIfOver(){
    if(headCoordX1 >= 28 || headCoordX1 < 0 || headCoordY1 < 0 || headCoordY1 >= 20 || grid[headCoordX1][headCoordY1].player != 0){  
        gameNotOver = 0;
    }
}

void checkIfOverP2(){
    if(headCoordX2 >= 28 || headCoordX2 < 0 || headCoordY2 < 0 || headCoordY2 >= 20 || grid[headCoordX2][headCoordY2].player != 0){
        gameNotOver2 = 0;
    }
}

void initializeGameOverScreen(){
    ClearScreen(0x003D3D3D);
    printfPos("GAME OVER!", 300, 300, 3);
    printfPos("To start Again, press spacebar", 300, 320, 3);
    printfPos("To quit, press Q", 300, 340, 3);
    if (gameMode == 2){
        if(headCoordX1 == headCoordX2 && headCoordY1 == headCoordY2){
            printfPos("TIE!", 300, 440, 3);
        }
        else if (gameNotOver2 == 0){
            printfPos("PLAYER ONE WINS!", 300, 440, 3);
        } else if (gameNotOver == 0){
            printfPos("PLAYER TWO WINS!", 300, 440, 3);
        }
    }
}

void interpretInput1(){ 
    if (input1 == 9){//TAB
        gameNotOver = 0;
    }
    //Los inputs son en ascii
    if(dirX1 == 0){ //Si estaba yendo verticalmente
        if(input1 == 65){ //A
            dirX1= -1;
            dirY1= 0;
        } else if (input1 == 68){ //D
            dirX1= 1;
            dirY1= 0;
        }
    } else if (dirY1 == 0) { //Si estaba yendo horizontalmente
        if(input1 == 87){ //W
            dirX1= 0;
            dirY1= -1;
        } else if (input1 == 83){ //S
            dirX1= 0;
            dirY1= 1;
        }
    }
}

void interpretInput2(){ 
    if (input2 == 9){//TAB
        gameNotOver2 = 0;
    }
    //Los inputs son en ascii
    if(dirX2 == 0){ //Si estaba yendo verticalmente
        if(input2 == 74){ //J
            dirX2= -1;
            dirY2= 0;
        } else if (input2 == 76){ //L
            dirX2= 1;
            dirY2= 0;
        }
    } else if (dirY2 == 0) { //Si estaba yendo horizontalmente
        if(input2 == 73){ //I
            dirX2= 0;
            dirY2= -1;
        } else if (input2 == 75){ //K
            dirX2= 0;
            dirY2= 1;
        }
    }
}

void resetInputs(){
    input1 = 0;
    input2 = 0;
}

void gameOverMenu(){
    resetInputs();
    initializeGameOverScreen();
    
    char inputAux = 0;
    while(inputAux == 0){
        inputAux = getChar();
        if (inputAux == 9){
            initializeGameOverScreen();
        }
        if(inputAux != 32 && inputAux != 81){
            inputAux = 0;
        } 
    }

    clearGrid();

    if (inputAux == 32){
        startGame();
    }else{
        smallerFontSize();
        ClearScreen(0x000000FF);
    }
}