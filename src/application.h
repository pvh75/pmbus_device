#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#ifdef __cplusplus
extern "C" {
#endif

void application_init(void);
void application(void);

void start_info(void);
void run_time_stats_info(void);

#define SMBUS_BUFF_SIZE 128

#define PMBUS_APP_PROFILE 0x01
#define PMBUS_APP_PROFILE_REVISION 0x12

#define PMBUS_V_OUT_MODE 0x17

#define PMBUS_MFR_ID "PVK"
#define PMBUS_MFR_MODEL "PMBUS-DEVICE"
#define PMBUS_MFR_REVISION "1.0"
#define PMBUS_MFR_LOCATION "RUSSIA"
#define PMBUS_MFR_DATE __DATE__
#define PMBUS_MFR_SERIAL "00000001"

#define PMBUS_IC_DEVICE_ID "STM32G474RE"
#define PMBUS_IC_DEVICE_REV "Revision X"

#ifdef __cplusplus
}
#endif

#endif
