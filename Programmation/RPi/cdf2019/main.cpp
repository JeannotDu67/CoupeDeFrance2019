#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <queue>
#include "web.hpp"
#include "dspic.hpp"
#include "actuators.hpp"
#include "SPI.hpp"
#include "lidar.hpp"
#include "actuators.hpp"
#include "hmi.hpp"

#include <wiringPi.h>
#include <wiringPiSPI.h>

void *print(void *ptr);
void debugAct();
void debugTestAllDelay();
void debugTestAllInstant();
void debugBN();
void debugGoldenium();
int main()
{
    //debugAct();
    //exit(0);
    wiringPiSetup();
	
    SPI spi(SPI_CHANNEL,SPI_SPEED); //initialise SPI
	
    Actuators actFront(&spi,SPI_ID_ACT_FRONT), actBack(&spi,SPI_ID_ACT_BACK);
	
    

	/*HMI hmi(&spi,SPI_ID_HMI);
    hmi.SetPos(0,400);
    hmi.SetPos(1,300);
    hmi.SetPrgm(1,"hello");
    hmi.SetPrgm(2,"world");
    hmi.SetPrgm(3,"=) !");*/
    //hmi.SetTxtFull("Hello CRIS ! =D ");

    /*actFront.MoveServo(0,SERVO_VALUE_HIGH);
    actFront.MoveServo(1,SERVO_VALUE_HIGH);
    actFront.MoveServo(2,SERVO_VALUE_HIGH);
    actBack.MoveServo(0,SERVO_VALUE_HIGH);
    actBack.MoveServo(1,SERVO_VALUE_HIGH);
    actBack.MoveServo(2,SERVO_VALUE_HIGH);*/

	DsPIC dspic;
    dspic.async_read(); //flush rx buffer

	Web web(&dspic);
    web.startThread();

    Lidar lidar(&spi, SPI_ID_LIDAR, &web);
    //lidar.flush(255);
    lidar.setSpeed(150);
    lidar.start();
    //lidar.stop();
    lidar.startThreadDetection();

    puts("Hello human ! I, your fervent robot, am initialised. Press <ENTER> to continue.");

    /*
	getchar();
	//lidar.start();
    //getchar();
	lidar.startThreadDetection();
    getchar();
	actFront.MoveServo(0,valueL);
    actFront.MoveServo(1,valueL);
    actFront.MoveServo(2,valueL);
    actBack.MoveServo(0,valueL);
    actBack.MoveServo(1,valueL);
    actBack.MoveServo(2,valueL);
	delay(500);
	actFront.MoveServo(0,valueH);
    actFront.MoveServo(1,valueH);
    actFront.MoveServo(2,valueH);
    actBack.MoveServo(0,valueH);
    actBack.MoveServo(1,valueH);
    actBack.MoveServo(2,valueH);
	getchar();
	lidar.stopThreadDetection();
	//lidar.stop();
	*/
    getchar();
	dspic.setVar8(CODE_VAR_VERBOSE,1);
	puts("verbose set to 1");
	dspic.getVar(CODE_VAR_BAT);
    dspic.loadVarDspicFromFile("config.txt");
    //dspic.initPos(1000,1500,3.14159);
    //dspic.initPos(1200,2550,-3.14159/2);
    dspic.initPos(1200-155,2550+124,-3.14159/2);
    //dspic.initPos(1000,1500,-3.14159/2);
    //dspic.initPos(0,0,0);
    //dspic.initPos(1000,3000,-3.14159/2);
    /*getchar();
    dspic.start();
    getchar();
	dspic.go(1200,450,0,0);
    getchar();
	dspic.go(1200,2550,1,0);*/
    /*f° turn buguée (thetac dans le dspic)
    int nTurn = 1;
    dspic.turn(nTurn*360,1);
    getchar();
    dspic.turn(-nTurn*360,1);*/

    /*trigo
    for(int i = 0; i < 1; i++){
        dspic.go(500,0,0,0);
        getchar();
        dspic.go(500,500,0,0);
        getchar();
        dspic.go(0,500,0,0);
        getchar();
        dspic.go(0,0,0,0);
        getchar();
    }*/

    //horaire
    /*for(int i = 0; i < 10; i++){
        dspic.go(0,500,0,0);
        getchar();
        dspic.go(500,500,0,0);
        getchar();
        dspic.go(500,0,0,0);
        getchar();
        dspic.go(0,0,0,0);
        getchar();
    }*/

    /*getchar();
    dspic.go(709,1303,0,0);
    getchar();
    dspic.go(703,1123,0,0);
    actFront.SetPump(0,1);
    //actFront.SetPump(1,1);
    //actFront.SetPump(2,1);
    getchar();
    dspic.go(709,1303,1,0);
    getchar();
    actFront.MoveServo(0,valueL);
    //actFront.MoveServo(1,valueL);
    //actFront.MoveServo(2,valueL);
    getchar();
    actFront.SetPump(0,0);
    actFront.SetPump(1,0);
    actFront.SetPump(2,0);
    getchar();
    actFront.MoveServo(0,valueH);
    actFront.MoveServo(1,valueH);
    actFront.MoveServo(2,valueH);*/

    /*getchar();
    lidar.flush();*/
    getchar();
    dspic.stop();
	dspic.setVar8(CODE_VAR_VERBOSE,0);
	puts("verbose set to 0");
    puts("exiting ...");
	lidar.stop();
    return 0;
}
void debugAct(){

    wiringPiSetup();
    SPI spi(SPI_CHANNEL,SPI_SPEED); //initialise SPI
    /*A AJOUTER : FLUSH tous les slaves*/
    Actuators actFront(&spi,SPI_ID_ACT_FRONT), actBack(&spi,SPI_ID_ACT_BACK);

    DsPIC dspic;

    dspic.async_read(); //flush rx buffer

    dspic.setVar8(CODE_VAR_VERBOSE,1);
    puts("verbose set to 1");
    dspic.getVar(CODE_VAR_BAT);
    dspic.loadVarDspicFromFile("config.txt");
    dspic.initPos(1000,1500,3.14159);

    int choice = 0;
    while(choice != -1){
        std::cout << "Press -1 to exit" << std::endl;
        std::cout << "Select arm (0-5)" << std::endl;
        std::cout << "Press 10 to set goldenium arm" << std::endl;
        std::cout << "Press 11 to reset goldenium arm" << std::endl;
        std::cin >> choice;
        if(choice >= 0 && choice <= 5){
            int id = choice;
            std::cout << "Press 1 to start pump" << std::endl;
            std::cout << "Press 2 to stop pump" << std::endl;
            std::cout << "Press 3 to measure current" << std::endl;
            std::cout << "Press 4 to set servo" << std::endl;
            std::cout << "Press 5 to testMeasure" << std::endl;
            std::cin >> choice;
            switch(choice){
                case 1:
                    if(id < 3){
                        actFront.SetPump(id,1);
                    }
                    else{
                        actBack.SetPump(id-3,1);
                    }
                    break;
                case 2:
                    if(id < 3){
                        actFront.SetPump(id,0);
                    }
                    else{
                        actBack.SetPump(id-3,0);
                    }
                    break;
                case 3:
                    if(id < 3){
                        std::cout << "current : " << actFront.debugGetCurrent(id) << std::endl;
                    }
                    else{
                        std::cout << "current : " << actBack.debugGetCurrent(id-3) << std::endl;
                    }
                    break;
                case 4:
                    std::cout << "enter duty cycle" << std::endl;
                    int dutyCycle;
                    std::cin >> dutyCycle;
                    if(id < 3){
                        actFront.MoveServo(id,dutyCycle);
                    }
                    else{
                        actBack.MoveServo(id-3,dutyCycle);
                    }
                    break;
                case 5:
                    if(id < 3){
                        actFront.SetPump(id,1);
                        delay(2000);
                        std::cout << "current : " << actFront.debugGetCurrent(id) << std::endl;
                        std::cout << "currentFull : " << actFront.debugGetCurrentFull(id) << std::endl;
                        actFront.SetPump(id,0);
                    }
                    else{
                        actBack.SetPump(id-3,1);
                        delay(500);
                        std::cout << "current : " << actBack.debugGetCurrent(id-3) << std::endl;
                        std::cout << "currentFull : " << actBack.debugGetCurrentFull(id-3) << std::endl;
                        actBack.SetPump(id-3,0);
                    }
                    break;
                case 6:
                    if(id < 3){
                        std::cout << "color : " << actFront.debugGetColor(id) << std::endl;
                    }
                    else{
                        std::cout << "color : " << actBack.debugGetColor(id-3) << std::endl;
                    }
                    break;
            }

        }
        else if(choice == 10){
            dspic.setMotLin(1);
        }
        else if(choice == 11){
            dspic.setMotLin(0);
        }
        else if(choice == 20){
            dspic.motorVoltage(0,8);
        }
        else if(choice == 21){
            dspic.motorVoltage(0,0);
        }

    }
    dspic.setVar8(CODE_VAR_VERBOSE,0);
    puts("verbose set to 0");
}
void debugTestAllDelay(){
    SPI spi(SPI_CHANNEL,SPI_SPEED); //initialise SPI
    /*A AJOUTER : FLUSH tous les slaves*/
    Actuators actFront(&spi,SPI_ID_ACT_FRONT), actBack(&spi,SPI_ID_ACT_BACK);

    int valueH = 800;
    int valueL = 1600;
    int valueDrop = 1500;
    int valueMiddle = 1000;

    actFront.SetPump(0,1);
    actFront.MoveServo(0,valueL);
    delay(250);
    actFront.MoveServo(0,valueH);

    actFront.SetPump(1,1);
    actFront.MoveServo(1,valueL);
    delay(250);
    actFront.MoveServo(1,valueH);

    actFront.SetPump(2,1);
    actFront.MoveServo(2,valueL);
    delay(250);
    actFront.MoveServo(2,valueH);

    actBack.SetPump(0,1);
    actBack.MoveServo(0,valueL);
    delay(250);
    actBack.MoveServo(0,valueH);

    actBack.SetPump(1,1);
    actBack.MoveServo(1,valueL);
    delay(250);
    actBack.MoveServo(1,valueH);

    actBack.SetPump(2,1);
    actBack.MoveServo(2,valueL);
    delay(250);
    actBack.MoveServo(2,valueH);


    getchar();

    actFront.MoveServo(0,valueL);
    delay(250);
    actFront.SetPump(0,0);

    actFront.MoveServo(1,valueL);
    delay(250);
    actFront.SetPump(1,0);

    actFront.MoveServo(2,valueL);
    delay(250);
    actFront.SetPump(2,0);

    actBack.MoveServo(0,valueL);
    delay(250);
    actBack.SetPump(0,0);

    actBack.MoveServo(1,valueL);
    delay(250);
    actBack.SetPump(1,0);

    actBack.MoveServo(2,valueL);
    delay(250);
    actBack.SetPump(2,0);

    getchar();

    actFront.MoveServo(0,valueDrop);
    actFront.MoveServo(1,valueDrop);
    actFront.MoveServo(2,valueDrop);

    actBack.MoveServo(0,valueDrop);
    actBack.MoveServo(1,valueDrop);
    actBack.MoveServo(2,valueDrop);

    getchar();

    actFront.MoveServo(0,valueMiddle);
    actFront.MoveServo(1,valueMiddle);
    actFront.MoveServo(2,valueMiddle);

    actBack.MoveServo(0,valueMiddle);
    actBack.MoveServo(1,valueMiddle);
    actBack.MoveServo(2,valueMiddle);
}
void debugTestAllInstant(){
    SPI spi(SPI_CHANNEL,SPI_SPEED); //initialise SPI
    /*A AJOUTER : FLUSH tous les slaves*/
    Actuators actFront(&spi,SPI_ID_ACT_FRONT), actBack(&spi,SPI_ID_ACT_BACK);

    int valueH = 800;
    int valueL = 1600;
    int valueDrop = 1500;
    int valueMiddle = 1000;

    actFront.SetPump(0,1);
    actFront.SetPump(1,1);
    actFront.SetPump(2,1);

    actBack.SetPump(0,1);
    actBack.SetPump(1,1);
    actBack.SetPump(2,1);

    actFront.MoveServo(0,valueL);
    actFront.MoveServo(1,valueL);
    actFront.MoveServo(2,valueL);

    actBack.MoveServo(0,valueL);
    actBack.MoveServo(1,valueL);
    actBack.MoveServo(2,valueL);

    delay(250);


    actFront.MoveServo(0,valueH);
    actFront.MoveServo(1,valueH);
    actFront.MoveServo(2,valueH);

    actBack.MoveServo(0,valueH);
    actBack.MoveServo(1,valueH);
    actBack.MoveServo(2,valueH);

    getchar();

    actFront.MoveServo(0,valueL);
    actFront.MoveServo(1,valueL);
    actFront.MoveServo(2,valueL);

    actBack.MoveServo(0,valueL);
    actBack.MoveServo(1,valueL);
    actBack.MoveServo(2,valueL);


    delay(250);

    actFront.SetPump(0,0);
    actFront.SetPump(1,0);
    actFront.SetPump(2,0);

    actBack.SetPump(0,0);
    actBack.SetPump(1,0);
    actBack.SetPump(2,0);

    getchar();

    actFront.MoveServo(0,valueDrop);
    actFront.MoveServo(1,valueDrop);
    actFront.MoveServo(2,valueDrop);

    actBack.MoveServo(0,valueDrop);
    actBack.MoveServo(1,valueDrop);
    actBack.MoveServo(2,valueDrop);

    getchar();

    actFront.MoveServo(0,valueMiddle);
    actFront.MoveServo(1,valueMiddle);
    actFront.MoveServo(2,valueMiddle);

    actBack.MoveServo(0,valueMiddle);
    actBack.MoveServo(1,valueMiddle);
    actBack.MoveServo(2,valueMiddle);
}
void debugBN(){
    SPI spi(SPI_CHANNEL,SPI_SPEED); //initialise SPI
    /*A AJOUTER : FLUSH tous les slaves*/
    Actuators actFront(&spi,SPI_ID_ACT_FRONT), actBack(&spi,SPI_ID_ACT_BACK);

    //int valueH = 800;
    int valueL = 1600;
    int valueDrop = 1500;
    int valueMiddle = 1000;

    actBack.SetPump(2,1);
    delay(10);
    actBack.MoveServo(2,valueL);
    getchar();
    actBack.MoveServo(2,1000);

    getchar();
    actBack.MoveServo(2,valueL);
    delay(250);
    actBack.SetPump(2,0);
    delay(250);
    actBack.MoveServo(2,valueDrop);
    delay(250);
    actBack.MoveServo(2,valueMiddle);
}
void debugGoldenium(){
    DsPIC dspic;

    dspic.async_read(); //flush rx buffer

    std::cout << "Press enter to continue"  << std::endl;
    getchar();
    dspic.setVar8(CODE_VAR_VERBOSE,1);
    dspic.start();
    dspic.getVar(CODE_VAR_BAT);
    getchar();
    dspic.setMotLin(1);
    //dspic.motorVoltage(1,8);
    //dspic.getVar(CODE_VAR_RUPT);
    getchar();
    dspic.motorVoltage(0,10);
    getchar();
    dspic.setMotLin(0);
    getchar();
    dspic.setMotLin(1);
    getchar();
    dspic.motorVoltage(0,0);
    //dspic.motorVoltage(1,-8);
    getchar();
    dspic.motorVoltage(1,0);
    getchar();
    dspic.setMotLin(0);
    dspic.stop();
    dspic.setVar8(CODE_VAR_VERBOSE,0);
}
void reglageOdometrie(){

}
