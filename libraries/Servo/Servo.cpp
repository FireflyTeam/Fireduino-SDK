 /**
 * @file Servo.c
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
#include <Arduino.h>
#include <Servo.h>
#include "ard_servo.h"


#define usToTicks(_us)    (ard_servo_clock_us() *_us)     // converts microseconds to tick
#define ticksToUs(_ticks) (_ticks/ard_servo_clock_us())	  // converts from ticks back to microseconds


static servo_t servos[MAX_SERVOS];                          // static array of servo structures

uint8_t ServoCount = 0;                                     // the total number of attached servos

static volatile int8_t Channel[_Nbr_16timers ];             // counter for the servo being pulsed for each timer (or -1 if refresh interval)

// convenience macros
#define SERVO_INDEX_TO_TIMER(_servo_nbr) ((timer16_Sequence_t)(_servo_nbr / SERVOS_PER_TIMER)) // returns the timer controlling this servo
#define SERVO_INDEX_TO_CHANNEL(_servo_nbr) (_servo_nbr % SERVOS_PER_TIMER)       // returns the index of the servo on this timer
#define SERVO_INDEX(_timer,_channel)  ((_timer*SERVOS_PER_TIMER) + _channel)     // macro to access servo index by timer and channel
#define SERVO(_timer,_channel)  (servos[SERVO_INDEX(_timer,_channel)])            // macro to access servo class by timer and channel

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)  // maximum value in uS for this servo

/************ static functions common to all instances ***********************/

void Servo_Handler(timer16_Sequence_t timer);
static void Servo_Hanler_ISR_1(void)
{
	Servo_Handler(_timer1);
}

static void Servo_Hanler_ISR_2(void)
{
	Servo_Handler(_timer2);
}

static void Servo_Hanler_ISR_3(void)
{
	Servo_Handler(_timer3);
}

static void Servo_Hanler_ISR_4(void)
{
	Servo_Handler(_timer4);
}

static const pFun_isr Servo_Hanler_ISR[_Nbr_16timers] = {
		Servo_Hanler_ISR_1,
		Servo_Hanler_ISR_2,
		Servo_Hanler_ISR_3,
		Servo_Hanler_ISR_4
};

static unsigned int channel_total_count[_Nbr_16timers];
void Servo_Handler(timer16_Sequence_t timer)
{
#if 1
    // clear interrupt
    if (Channel[timer] < 0) {
        // channel set to -1 indicated that refresh interval completed so reset the timer
    	channel_total_count[timer] = 0;
    } else {
        if (SERVO_INDEX(timer,Channel[timer]) < ServoCount && SERVO(timer,Channel[timer]).Pin.isActive == true && SERVO(timer,Channel[timer]).ticks != 0) {
            digitalWrite(SERVO(timer,Channel[timer]).Pin.nbr, LOW); // pulse this channel low if activated
        }
    }

    Channel[timer]++;    // increment to the next channel
    if( SERVO_INDEX(timer,Channel[timer]) < ServoCount && Channel[timer] < SERVOS_PER_TIMER) {
    	channel_total_count[timer] += SERVO(timer,Channel[timer]).ticks;
    	ard_servo_timer_set(timer,SERVO(timer,Channel[timer]).ticks);
        if(SERVO(timer,Channel[timer]).Pin.isActive == true && SERVO(timer,Channel[timer]).ticks != 0) {    // check if activated
            digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,HIGH); // its an active channel so pulse it high
        }
    }
    else {
        Channel[timer] = -1; // this will get incremented at the end of the refresh period to start again at the first channel
        if(channel_total_count[timer] < usToTicks(REFRESH_INTERVAL))
        {
        	ard_servo_timer_set(timer,usToTicks(REFRESH_INTERVAL) - channel_total_count[timer]);
        }
        else
        {
        	Servo_Handler(timer);
        }
    }
#endif
}


static void finISR(timer16_Sequence_t timer)
{
	ard_servo_timer_disable(timer);
}


static boolean isTimerActive(timer16_Sequence_t timer)
{
  // returns true if any servo is active on this timer
  for(uint8_t channel=0; channel < SERVOS_PER_TIMER; channel++) {
    if(SERVO(timer,channel).Pin.isActive == true)
      return true;
  }
  return false;
}

/****************** end of static functions ******************************/

Servo::Servo()
{
  if (ServoCount < MAX_SERVOS) {
    this->servoIndex = ServoCount++;                    // assign a servo index to this instance
    servos[this->servoIndex].ticks = usToTicks(DEFAULT_PULSE_WIDTH);   // store default values
  } else {
    this->servoIndex = INVALID_SERVO;  // too many servos
  }
}

uint8_t Servo::attach(int pin)
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
  timer16_Sequence_t timer;

  if (this->servoIndex < MAX_SERVOS) {
    pinMode(pin, OUTPUT);                                   // set servo pin to output
    servos[this->servoIndex].Pin.nbr = pin;
    // todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128
    this->min  = (MIN_PULSE_WIDTH - min)/4; //resolution of min/max is 4 uS
    this->max  = (MAX_PULSE_WIDTH - max)/4;
    // initialize the timer if it has not already been initialized
    timer = SERVO_INDEX_TO_TIMER(servoIndex);
    if (isTimerActive(timer) == false) {
    	Channel[timer] = -1;
    	ard_servo_timer_isr(timer,Servo_Hanler_ISR[timer]);
    	ard_servo_timer_set(timer,servos[this->servoIndex].ticks);
    }
    servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
  }
//  printf("this->servoIndex:%d pin:%d\r\n",this->servoIndex,servos[this->servoIndex].Pin.nbr);
  return this->servoIndex;
}

void Servo::detach()
{
  timer16_Sequence_t timer;

  servos[this->servoIndex].Pin.isActive = false;
  timer = SERVO_INDEX_TO_TIMER(servoIndex);
  if(isTimerActive(timer) == false) {
    finISR(timer);
  }
}

void Servo::write(int value)
{
  // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
  if (value < MIN_PULSE_WIDTH)
  {
    if (value < 0)
      value = 0;
    else if (value > 180)
      value = 180;

    value = map(value, 0, 180, SERVO_MIN(), SERVO_MAX());
  }
//  printf("write:%d \r\n",value);
  writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
  // calculate and store the values for the given channel
  byte channel = this->servoIndex;
  if( (channel < MAX_SERVOS) )   // ensure channel is valid
  {
    if (value < SERVO_MIN())          // ensure pulse width is valid
      value = SERVO_MIN();
    else if (value > SERVO_MAX())
      value = SERVO_MAX();

    value = usToTicks(value);  // convert to ticks after compensating for interrupt overhead
    servos[channel].ticks = value;
//    printf("writeMicroseconds:%d \r\n",value);
  }
}

int Servo::read() // return the value as degrees
{
  return map(readMicroseconds()+1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::readMicroseconds()
{
  unsigned int pulsewidth;
  if (this->servoIndex != INVALID_SERVO)
    pulsewidth = ticksToUs(servos[this->servoIndex].ticks);
  else
    pulsewidth  = 0;

  return pulsewidth;
}

bool Servo::attached()
{
  return servos[this->servoIndex].Pin.isActive;
}


