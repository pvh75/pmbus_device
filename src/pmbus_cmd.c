#include "application.h"
#include "pmbus.h"

#include <string.h>

static const float kpd = 0.85f;
static const float vout = 12.0f;
static const float iout = 100.0f;
static const float pout = vout * iout;
static const float pin = pout / kpd;
static const float vin = 220.0f;
static const float iin = pin / vin;
static const float temperature_1 = 50.0f;
static const float temperature_2 = 60.0f;
static const float temperature_3 = 70.0f;
static float fan_1_speed = 10000.0f;

static pmbus_error_t pmbus_cmd_read_byte_(uint8_t dir, uint8_t* pdata, uint8_t byte) {
    if (dir == PMBUS_READ) {
        *pdata = byte;

        return PMBUS_OK;
    }

    return PMBUS_ERROR;
}

static pmbus_error_t pmbus_cmd_read_write_byte_(uint8_t dir,
                                                uint8_t* pdata,
                                                uint8_t* byte) {
    if (dir == PMBUS_READ) {
        *pdata = *byte;
    } else {
        *byte = *pdata;
    }

    return PMBUS_OK;
}

static pmbus_error_t pmbus_cmd_read_write_word_(uint8_t dir,
                                                uint16_t* pdata,
                                                uint16_t* word) {
    if (dir == PMBUS_READ) {
        *pdata = *word;
    } else {
        *word = *pdata;
    }

    return PMBUS_OK;
}

static pmbus_error_t pmbus_cmd_read_float_lin11_(uint8_t dir, uint8_t* pdata, float in) {
    if (dir == PMBUS_READ) {
        pmbus_linear11_t lin = pmbus_float_to_linear11(in);

        pdata[0] = lin.raw[0];
        pdata[1] = lin.raw[1];

        return PMBUS_OK;
    }

    return PMBUS_ERROR;
}

static pmbus_error_t pmbus_cmd_read_float_ulin16_(uint8_t dir, uint8_t* pdata, float in) {
    if (dir == PMBUS_READ) {
        pmbus_vout_mode_t voutm = {.raw = PMBUS_V_OUT_MODE};
        uint16_t ulin16 = pmbus_float_to_ulinear16(in, voutm);

        pdata[0] = ulin16;
        pdata[1] = ulin16 >> 8;

        return PMBUS_OK;
    }

    return PMBUS_ERROR;
}

static pmbus_error_t pmbus_cmd_read_write_float_lin11_(uint8_t dir,
                                                       uint8_t* pdata,
                                                       float* in) {
    pmbus_linear11_t lin = {.raw[0] = 0, .raw[1] = 0};

    if (dir == PMBUS_READ) {
        lin = pmbus_float_to_linear11(*in);

        pdata[0] = lin.raw[0];
        pdata[1] = lin.raw[1];
    } else {
        lin.raw[0] = pdata[0];
        lin.raw[1] = pdata[1];

        *in = pmbus_linear11_to_float(lin);
    }

    return PMBUS_OK;
}

static pmbus_error_t pmbus_cmd_read_str_(uint8_t dir,
                                         uint8_t* pbuff,
                                         const char* pstr,
                                         uint8_t size) {
    if (dir == PMBUS_READ) {
        pbuff[0] = size;
        strncpy((char*)&pbuff[1], pstr, SMBUS_BUFF_SIZE - 1);

        return PMBUS_OK;
    }

    return PMBUS_ERROR;
}

static pmbus_error_t pmbus_cmd_read_energy_(uint8_t dir,
                                            uint8_t* pbuff,
                                            ppmbus_energy_t penergy) {
    if (dir == PMBUS_READ) {
        pbuff[0] = 6;
        pbuff[1] = (uint8_t)((uint32_t)penergy->acc);
        pbuff[2] = (uint8_t)(((uint32_t)penergy->acc) >> 8);
        pbuff[3] = penergy->roll_over_count;
        pbuff[4] = (uint8_t)(penergy->samples_count);
        pbuff[5] = (uint8_t)(penergy->samples_count >> 8);
        pbuff[6] = (uint8_t)(penergy->samples_count >> 16);

        return PMBUS_OK;
    }

    return PMBUS_ERROR;
}

static void pmbus_add_energy_sample_(ppmbus_energy_t penergy, float power) {
    static const float max_acc_val = 0x7FFF;
    static const uint32_t max_samples_count = 0xFFFFFF;

    penergy->acc += power;
    if (penergy->acc > max_acc_val) {
        penergy->acc -= max_acc_val;
        ++penergy->roll_over_count;
    }

    ++penergy->samples_count;
    if (penergy->samples_count > max_samples_count) {
        penergy->samples_count = 0;
    }
}

pmbus_error_t pmbus_cmd_page(uint8_t dir, uint8_t* pdata) {
    static uint8_t page = 0xFF;

    return pmbus_cmd_read_write_byte_(dir, pdata, &page);
}

pmbus_error_t pmbus_cmd_operation(uint8_t dir, uint8_t* pdata) {
    static uint8_t oper = PMBUS_CMD_OPERATION;

    return pmbus_cmd_read_write_byte_(dir, pdata, &oper);
}

pmbus_error_t pmbus_cmd_on_off_config(uint8_t dir, uint8_t* pdata) {
    static uint8_t conf = PMBUS_CMD_ON_OFF_CONFIG;

    return pmbus_cmd_read_write_byte_(dir, pdata, &conf);
}

pmbus_error_t pmbus_cmd_clear_faults(uint8_t dir, uint8_t* pdata) {
    (void)pdata;

    if (dir == PMBUS_READ) {
        return PMBUS_ERROR;
    }

    return PMBUS_OK;
}

pmbus_error_t pmbus_cmd_capability(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_byte_(dir, pdata, 0x90);
}

pmbus_error_t pmbus_cmd_vout_mode(uint8_t dir, uint8_t* pdata) {
    static uint8_t mode = PMBUS_V_OUT_MODE;

    return pmbus_cmd_read_write_byte_(dir, pdata, &mode);
}

pmbus_error_t pmbus_cmd_coefficients(uint8_t dir, uint8_t* pdata) {
    (void)dir;

    static uint8_t ml, mh, bl, bh, r;

    if (pdata[2] == PMBUS_COEFFICIENTS_READ || pdata[2] == PMBUS_COEFFICIENTS_WRITE) {
        pdata[0] = 5;
        pdata[1] = ml;
        pdata[2] = mh;
        pdata[3] = bl;
        pdata[4] = bh;
        pdata[5] = r;
    } else {
        return PMBUS_ERROR;
    }

    return PMBUS_OK;
}

pmbus_error_t pmbus_cmd_fan_config_1_2(uint8_t dir, uint8_t* pdata) {
    static uint8_t conf = 0xC0;

    return pmbus_cmd_read_write_byte_(dir, pdata, &conf);
}

pmbus_error_t pmbus_cmd_fan_command_1(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_write_float_lin11_(dir, pdata, &fan_1_speed);
}

pmbus_error_t pmbus_cmd_mfr_id(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_str_(dir, pdata, PMBUS_MFR_ID, sizeof(PMBUS_MFR_ID));
}

pmbus_error_t pmbus_cmd_mfr_model(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_str_(dir, pdata, PMBUS_MFR_MODEL, sizeof(PMBUS_MFR_MODEL));
}

pmbus_error_t pmbus_cmd_mfr_revision(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_str_(
            dir, pdata, PMBUS_MFR_REVISION, sizeof(PMBUS_MFR_REVISION));
}

pmbus_error_t pmbus_cmd_mfr_location(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_str_(
            dir, pdata, PMBUS_MFR_LOCATION, sizeof(PMBUS_MFR_LOCATION));
}

pmbus_error_t pmbus_cmd_mfr_date(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_str_(dir, pdata, PMBUS_MFR_DATE, sizeof(PMBUS_MFR_DATE));
}

pmbus_error_t pmbus_cmd_mfr_serial(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_str_(dir, pdata, PMBUS_MFR_SERIAL, sizeof(PMBUS_MFR_SERIAL));
}

pmbus_error_t pmbus_cmd_app_profile_support(uint8_t dir, uint8_t* pdata) {
    if (dir == PMBUS_READ) {
        pdata[0] = 2;
        pdata[1] = PMBUS_APP_PROFILE;
        pdata[2] = PMBUS_APP_PROFILE_REVISION;

        return PMBUS_OK;
    }

    return PMBUS_ERROR;
}

pmbus_error_t pmbus_cmd_status_byte(uint8_t dir, uint8_t* pdata) {
    static uint8_t stb = 0x01;

    return pmbus_cmd_read_write_byte_(dir, pdata, &stb);
}

pmbus_error_t pmbus_cmd_status_word(uint8_t dir, uint8_t* pdata) {
    static uint16_t stw = 0x0201;

    return pmbus_cmd_read_write_word_(dir, (uint16_t*)pdata, &stw);
}

pmbus_error_t pmbus_cmd_status_vout(uint8_t dir, uint8_t* pdata) {
    static uint8_t stvout = 0x01;

    return pmbus_cmd_read_write_byte_(dir, pdata, &stvout);
}

pmbus_error_t pmbus_cmd_status_iout(uint8_t dir, uint8_t* pdata) {
    static uint8_t stiout = 0x01;

    return pmbus_cmd_read_write_byte_(dir, pdata, &stiout);
}

pmbus_error_t pmbus_cmd_status_input(uint8_t dir, uint8_t* pdata) {
    static uint8_t stin = 0x01;

    return pmbus_cmd_read_write_byte_(dir, pdata, &stin);
}

pmbus_error_t pmbus_cmd_status_temperature(uint8_t dir, uint8_t* pdata) {
    static uint8_t sttemp = 0x01;

    return pmbus_cmd_read_write_byte_(dir, pdata, &sttemp);
}

pmbus_error_t pmbus_cmd_status_cml(uint8_t dir, uint8_t* pdata) {
    static uint8_t stcml = 0x01;

    return pmbus_cmd_read_write_byte_(dir, pdata, &stcml);
}

pmbus_error_t pmbus_cmd_status_other(uint8_t dir, uint8_t* pdata) {
    static uint8_t sto = 0x01;

    return pmbus_cmd_read_write_byte_(dir, pdata, &sto);
}

pmbus_error_t pmbus_cmd_status_fans_1_2(uint8_t dir, uint8_t* pdata) {
    static uint8_t stf12 = 0x01;

    return pmbus_cmd_read_write_byte_(dir, pdata, &stf12);
}

pmbus_error_t pmbus_cmd_read_ein(uint8_t dir, uint8_t* pdata) {
    static pmbus_energy_t ine = {0.0f, 0, 0};

    pmbus_add_energy_sample_(&ine, pin);
    return pmbus_cmd_read_energy_(dir, pdata, &ine);
}

pmbus_error_t pmbus_cmd_read_eout(uint8_t dir, uint8_t* pdata) {
    static pmbus_energy_t oute = {0.0f, 0, 0};

    pmbus_add_energy_sample_(&oute, pout);
    return pmbus_cmd_read_energy_(dir, pdata, &oute);
}

pmbus_error_t pmbus_cmd_read_vin(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_float_lin11_(dir, pdata, vin);
}

pmbus_error_t pmbus_cmd_read_iin(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_float_lin11_(dir, pdata, iin);
}

pmbus_error_t pmbus_cmd_read_vout(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_float_ulin16_(dir, pdata, vout);
}

pmbus_error_t pmbus_cmd_read_iout(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_float_lin11_(dir, pdata, iout);
}

pmbus_error_t pmbus_cmd_read_temperature_1(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_float_lin11_(dir, pdata, temperature_1);
}

pmbus_error_t pmbus_cmd_read_temperature_2(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_float_lin11_(dir, pdata, temperature_2);
}

pmbus_error_t pmbus_cmd_read_temperature_3(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_float_lin11_(dir, pdata, temperature_3);
}

pmbus_error_t pmbus_cmd_read_fan_speed_1(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_float_lin11_(dir, pdata, fan_1_speed);
}

pmbus_error_t pmbus_cmd_read_pout(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_float_lin11_(dir, pdata, pout);
}

pmbus_error_t pmbus_cmd_read_pin(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_float_lin11_(dir, pdata, pin);
}

pmbus_error_t pmbus_cmd_ic_device_id(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_str_(
            dir, pdata, PMBUS_IC_DEVICE_ID, sizeof(PMBUS_IC_DEVICE_ID));
}

pmbus_error_t pmbus_cmd_ic_device_rev(uint8_t dir, uint8_t* pdata) {
    return pmbus_cmd_read_str_(
            dir, pdata, PMBUS_IC_DEVICE_REV, sizeof(PMBUS_IC_DEVICE_REV));
}
