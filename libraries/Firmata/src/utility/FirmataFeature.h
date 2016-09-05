 /**
 * @file FirmataFeature.h
 * @brief Definitions FirmataFeature class for Fireduino
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
#ifndef FirmataFeature_h
#define FirmataFeature_h

#include <Firmata.h>

class FirmataFeature
{
  public:
    virtual void handleCapability(byte pin) = 0;
    virtual boolean handlePinMode(byte pin, int mode) = 0;
    virtual boolean handleSysex(byte command, byte argc, byte* argv) = 0;
    virtual void reset() = 0;
};

#endif
