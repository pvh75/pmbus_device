#include "pmbus.h"

#include <gtest/gtest.h>
#include <cmath>

enum { PMBUS_V_OUT_MODE = 0x17 };

TEST(pmbus, float_to_linear11) {
    const float flt{-0.5f};
    const pmbus_linear11_t lin = pmbus_float_to_linear11(flt);

    EXPECT_EQ(0x00, lin.raw[0]);
    EXPECT_EQ(0xB6, lin.raw[1]);
}

TEST(pmbus, lineral11_to_float) {
    const pmbus_linear11_t lin{.raw{0xAB, 0xCD}};
    const float flt = pmbus_linear11_to_float(lin);

    EXPECT_NEAR(flt, -4.66406f, 1e-5f);
}

TEST(pmbus, float_to_ulineral16) {
    const pmbus_vout_mode_t v_out_mode{.raw = PMBUS_V_OUT_MODE};
    float v_out_voltage{11.9f};
    uint16_t test_val = pmbus_float_to_ulinear16(v_out_voltage, v_out_mode);

    EXPECT_NEAR(v_out_voltage, test_val * pow(2, v_out_mode.data.exp), 1e-2f);

    v_out_voltage = 12.0f;
    test_val = pmbus_float_to_ulinear16(v_out_voltage, v_out_mode);

    EXPECT_NEAR(v_out_voltage, test_val * pow(2, v_out_mode.data.exp), 1e-2f);

    v_out_voltage = 12.1f;
    test_val = pmbus_float_to_ulinear16(v_out_voltage, v_out_mode);

    EXPECT_NEAR(v_out_voltage, test_val * pow(2, v_out_mode.data.exp), 1e-2f);
}

TEST(pmbus, encode_decode_linear11) {
    pmbus_linear11_t lin{};
    float flt{};

    flt = 1.52587891e-5f;
    lin = pmbus_float_to_linear11(flt);
    EXPECT_NEAR(flt, pmbus_linear11_to_float(lin), 1e-5f);

    flt = -1.52587891e-5f;
    lin = pmbus_float_to_linear11(flt);
    EXPECT_NEAR(flt, pmbus_linear11_to_float(lin), 1e-5f);

    flt = 100500.0f;
    lin = pmbus_float_to_linear11(flt);
    EXPECT_NEAR(flt, pmbus_linear11_to_float(lin), 1e+2f);

    flt = -100500.0f;
    lin = pmbus_float_to_linear11(flt);
    EXPECT_NEAR(flt, pmbus_linear11_to_float(lin), 1e+2f);

    flt = 0.0f;
    lin = pmbus_float_to_linear11(flt);
    EXPECT_NEAR(flt, pmbus_linear11_to_float(lin), 1e-5f);

    flt = 0.00123f;
    lin = pmbus_float_to_linear11(flt);
    EXPECT_NEAR(flt, pmbus_linear11_to_float(lin), 1e-5f);

    flt = -0.00123f;
    lin = pmbus_float_to_linear11(flt);
    EXPECT_NEAR(flt, pmbus_linear11_to_float(lin), 1e-5f);

    flt = 33.521664e6;
    lin = pmbus_float_to_linear11(flt);
    EXPECT_NEAR(flt, pmbus_linear11_to_float(lin), 1e+5f);

    flt = -33.521664e6;
    lin = pmbus_float_to_linear11(flt);
    EXPECT_NEAR(flt, pmbus_linear11_to_float(lin), 1e+5f);
}
