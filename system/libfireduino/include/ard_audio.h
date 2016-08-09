 /**
 * @file ard_audio.h
 * @brief Definitions audio driver header for Fireduino
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

#ifndef _ARDUINO_ADUIO_H_
#define _ARDUINO_ADUIO_H_


#ifdef __cplusplus
extern "C" {
#endif

extern int ard_audio_init(unsigned char st);
extern int ard_audio_playfile(const wchar_t*path);
extern int ard_audio_playnetfile(const char *path);
extern int ard_audio_setVolume(unsigned char vol);
extern unsigned char ard_audio_getVolume(void);
extern int ard_audio_pause(void);
extern int ard_audio_resume(void);
extern int ard_audio_stop(void);
extern int ard_audio_getStatus(void);
extern int ard_audio_end(void);

#ifdef __cplusplus
}
#endif


#endif
