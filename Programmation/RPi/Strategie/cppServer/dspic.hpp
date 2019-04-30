#ifndef DSPIC_H
#define DSPIC_H

#define BAUDRATE	500000

#define RX_CODE_START 1
#define RX_CODE_STOP 2
#define RX_CODE_SET 3
#define RX_CODE_GET 4
#define RX_CODE_SERVO 5
#define RX_CODE_MOTOR 6
#define RX_CODE_AX12 7
#define RX_CODE_GO 8
#define RX_CODE_TURN 9

#define RX_CODE_MOTOR_VOLTAGE   10

#define RX_SIZE_START 2
#define RX_SIZE_STOP 2
#define RX_SIZE_SET       // var,type,value
#define RX_SIZE_SET_8b 	5
#define RX_SIZE_SET_32b 8
#define RX_SIZE_SET_64b 12
#define RX_SIZE_GET     3 // var
#define RX_SIZE_SERVO   5 // id,value_H,value_L
#define RX_SIZE_MOTOR   4 // id,value
#define RX_SIZE_MOTOR_VOLTAGE   7 // id,val_double[4]
#define RX_SIZE_AX12    5 // id,value_H,value_L
#define RX_SIZE_GO      7 // option,x_H,x_L,y_H,y_L
#define RX_SIZE_TURN    5 // option,t_H,t_L

#define MASK_OPTION_RELATIVE    0x2
#define MASK_OPTION_REVERSE     0x1

#define TX_CODE_
#define TX_CODE_
#define TX_CODE_
#define TX_CODE_
#define TX_CODE_

#define TX_SIZE_

#define TX_CODE_VAR     1
#define TX_CODE_LOG     2
#define TX_CODE_PLOT    3

#define CODE_VAR_X      1
#define CODE_VAR_Y      2
#define CODE_VAR_T      3

#define CODE_VAR_RUPT   4

#define CODE_VAR_VERBOSE    5

#define CODE_VAR_X_LD       6
#define CODE_VAR_Y_LD       7
#define CODE_VAR_T_LD       8

#define CODE_VAR_ALLPID		9

#define CODE_VAR_STATE       10
#define CODE_VAR_BAT         11

#define CODE_VAR_P_SPEED_L   12
#define CODE_VAR_I_SPEED_L   13
#define CODE_VAR_D_SPEED_L   14
#define CODE_VAR_P_SPEED_R   15
#define CODE_VAR_I_SPEED_R   16
#define CODE_VAR_D_SPEED_R   17
#define CODE_VAR_P_DISTANCE  18
#define CODE_VAR_I_DISTANCE  19
#define CODE_VAR_D_DISTANCE  20
#define CODE_VAR_P_ANGLE     21
#define CODE_VAR_I_ANGLE     22
#define CODE_VAR_D_ANGLE     23

#define CODE_VAR_ODO         29

#define CODE_VAR_COEF_DISSYMETRY                    30
#define CODE_VAR_MM_PER_TICKS                       31
#define CODE_VAR_DISTANCE_BETWEEN_ENCODER_WHEELS    32

#define CODE_VAR_COEF_DISSYMETRY_LD                 33
#define CODE_VAR_MM_PER_TICKS_LD                    34
#define CODE_VAR_RAD_PER_TICKS_LD                   35

#define VAR_8b      0
#define VAR_16b     1
#define VAR_32b     2
#define VAR_64b     3

#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <queue>
#include <unistd.h>

struct microswitch{
  unsigned int ass0 : 1;
  unsigned int ass1 : 1;
  unsigned int ass2 : 1;
  unsigned int ass3 : 1;
  unsigned int act0 : 1;
  unsigned int act1 : 1;
  unsigned int act2 : 1;
  unsigned int act3 : 1;
  unsigned int act4 : 1;
  unsigned int act5 : 1;
};
struct point{
  uint8_t id;
  uint32_t x;
  int32_t y;
};
struct pid{
  uint32_t Kp;
  uint32_t Ki;
  uint32_t Kd;
};

class DsPIC
{
    public:
        DsPIC();
        virtual ~DsPIC();

		void servo(uint8_t id, uint16_t value);
		void AX12(uint8_t id, uint16_t value);
		void motor(uint8_t id, int8_t value);
		void motorVoltage(uint8_t id, float value);
		void start();
		void stop();
		void go(int16_t x, int16_t y,unsigned char rev, unsigned char relative);
		void turn(int16_t t,unsigned char rev, unsigned char relative);
		void setVar8(uint8_t varCode, uint8_t var);
		void setVar32(uint8_t varCode, uint32_t var);
		void initPos(double x, double y, double t);
		void setVarDouble64b(uint8_t varCode, double Var);
		void loadPID();
		void getVar(uint8_t varCode);
		std::string async_read();
        std::vector<uint8_t> readMsg();
		int16_t x = 1500,y = 1000, t = 45;
		int16_t US[6];
		microswitch rupt;
		std::queue<std::string> logs;
		std::queue<point> plots;
		pid pidSpeedLeft = {0,0,0};
		pid pidSpeedRight = {0,0,0};
		pid pidDistance = {0,0,0};
		pid pidAngle = {0,0,0};
		//uint16_t nbUpdatePID = 0;
		bool isPIDUpdated = false;
		float bat = 0;
		double x_ld = 0;
		double y_ld = 0;
		double t_ld = 0;
		double coef_dissymetry = 0;
		double mm_per_tick = 0;
		double rad_per_tick = 0;
    protected:
		int fd;
    private:
};

#endif // DSPIC_H
