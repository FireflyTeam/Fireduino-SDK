 /**
 * @file Servo.h
 * @brief Definitions Servo class for Fireduino
 * @author jiang<jdz@t-chip.com.cn> 
 * @version V1.0
 * @date 2016.02
 * 
 * @par Copyright:
 * Copyright (c) 2016 T-CHIP INTELLIGENCE TECHNOLOGY CO.,LTD. \n\n
 *
 * For more information, please visit website <http://www.t-firefly.com/>, \n\n
 * or email to <service@t-firefly.com>.
 */ 
#ifndef Servo_h
#define Servo_h


#define Servo_VERSION           2     // software version of this library

#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached
#define REFRESH_INTERVAL    20000     // minumim time to refresh servos in microseconds 




typedef enum { _timer1, _timer2, _timer3, _timer4, _Nbr_16timers } timer16_Sequence_t ;
#define SERVOS_PER_TIMER       8     // the maximum number of servos controlled by one timer
#define MAX_SERVOS   (_Nbr_16timers  * SERVOS_PER_TIMER)

#define INVALID_SERVO         255     // flag indicating an invalid servo index

typedef struct  {
  unsigned char nbr        :6 ;             // a pin number from 0 to 63
  unsigned char isActive   :1 ;             // true if this channel is enabled, pin not pulsed if false
} ServoPin_t   ;  

typedef struct {
  ServoPin_t Pin;
  volatile unsigned int ticks;
} servo_t;

class Servo
{
public:
  Servo();
  unsigned char attach(int pin);           // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
  unsigned char attach(int pin, int min, int max); // as above but also sets min and max values for writes.
  void detach();
  void write(int value);             // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds 
  void writeMicroseconds(int value); // Write pulse width in microseconds 
  int read();                        // returns current pulse width as an angle between 0 and 180 degrees
  int readMicroseconds();            // returns current pulse width in microseconds for this servo (was read_us() in first release)
  bool attached();                   // return true if this servo is attached, otherwise false 
private:
  unsigned char servoIndex;               // index into the channel data for this servo
  signed char  min;                       // minimum is this value times 4 added to MIN_PULSE_WIDTH
  signed char  max;                       // maximum is this value times 4 added to MAX_PULSE_WIDTH
};

#endif
