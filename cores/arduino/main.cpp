 /**
 * @file main.cpp
 * @brief Definitions main class for Fireduino
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
extern void setup(void);
extern void loop(void);

extern "C" void arduino_main(void);
void arduino_main(void)
{

    setup();

    while (1) {
        loop();
    }

}
