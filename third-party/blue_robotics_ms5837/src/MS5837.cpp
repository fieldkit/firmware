#include "MS5837.h"
#include <Wire.h>

const uint8_t MS5837_ADDR = 0x76;
const uint8_t MS5837_RESET = 0x1E;
const uint8_t MS5837_ADC_READ = 0x00;
const uint8_t MS5837_PROM_READ = 0xA0;
const uint8_t MS5837_CONVERT_D1_8192 = 0x4A;
const uint8_t MS5837_CONVERT_D2_8192 = 0x5A;

const float MS5837::Pa = 100.0f;
const float MS5837::bar = 0.001f;
const float MS5837::mbar = 1.0f;

const uint8_t MS5837::MS5837_30BA = 0;
const uint8_t MS5837::MS5837_02BA = 1;
const uint8_t MS5837::MS5837_UNRECOGNISED = 255;

const uint8_t MS5837_02BA01 = 0x00; // Sensor version: From MS5837_02BA datasheet Version PROM Word 0
const uint8_t MS5837_02BA21 = 0x15; // Sensor version: From MS5837_02BA datasheet Version PROM Word 0
const uint8_t MS5837_30BA26 = 0x1A; // Sensor version: From MS5837_30BA datasheet Version PROM Word 0

// https://github.com/hepingood/ms5837/blob/master/src/driver_ms5837.c
// https://github.com/TEConnectivity/MS5837_Generic_C_Driver/blob/master/ms5837.c

extern "C" {
uint32_t fkb_external_printf(const char *str, ...);
void alogf(uint8_t level, const char *facility, const char *f, ...) __attribute__((format(printf, 3, 4)));
}

MS5837::MS5837() {
    fluidDensity = 1029;
}

bool MS5837::begin(TwoWire &wirePort) {
    return (init(wirePort));
}

bool MS5837::init(TwoWire &wirePort) {
    _i2cPort = &wirePort; // Grab which port the user wants us to use

    // Reset the MS5837, per datasheet
    _i2cPort->beginTransmission(MS5837_ADDR);
    _i2cPort->write(MS5837_RESET);
    _i2cPort->endTransmission();

    // Wait for reset to complete
    delay(10);

    // Read calibration values and CRC
    for (uint8_t i = 0; i < 7; i++) {
        _i2cPort->beginTransmission(MS5837_ADDR);
        _i2cPort->write(MS5837_PROM_READ + i * 2);
        _i2cPort->endTransmission();

        _i2cPort->requestFrom(MS5837_ADDR, 2);
        C[i] = (_i2cPort->read() << 8) | _i2cPort->read();
    }

    // Verify that data is correct with CRC
    uint8_t crcRead = C[0] >> 12;
    uint8_t crcCalculated = crc4(C);

    if (crcCalculated != crcRead) {
        return false; // CRC fail
    }

    uint8_t version = (C[0] >> 5) & 0x7F; // Extract the sensor version from PROM Word 0

    // Set _model according to the sensor version
    if (version == MS5837_02BA01) {
        _model = MS5837_02BA;
        alogf(3 /*INFO*/, "ms5837", "detected 02BA01");
    } else if (version == MS5837_02BA21) {
        _model = MS5837_02BA;
        alogf(3 /*INFO*/, "ms5837", "detected 02BA02");
    } else if (version == MS5837_30BA26) {
        _model = MS5837_30BA;
        alogf(3 /*INFO*/, "ms5837", "detected 30BA");
    } else {
        _model = MS5837_UNRECOGNISED;
    }
    // The sensor has passed the CRC check, so we should return true even if
    // the sensor version is unrecognised.
    // (The MS5637 has the same address as the MS5837 and will also pass the CRC check)
    // (but will hopefully be unrecognised.)
    return true;
}

void MS5837::setModel(uint8_t model) {
    _model = model;
}

uint8_t MS5837::getModel() {
    return (_model);
}

void MS5837::setFluidDensity(float density) {
    fluidDensity = density;
}

void MS5837::read() {
    // Check that _i2cPort is not NULL (i.e. has the user forgoten to call .init or .begin?)
    if (_i2cPort == NULL) {
        return;
    }

    // Request D1 conversion
    _i2cPort->beginTransmission(MS5837_ADDR);
    _i2cPort->write(MS5837_CONVERT_D1_8192);
    _i2cPort->endTransmission();

    delay(20); // Max conversion time per datasheet

    _i2cPort->beginTransmission(MS5837_ADDR);
    _i2cPort->write(MS5837_ADC_READ);
    _i2cPort->endTransmission();

    _i2cPort->requestFrom(MS5837_ADDR, 3);
    D1_pres = 0;
    D1_pres = _i2cPort->read();
    D1_pres = (D1_pres << 8) | _i2cPort->read();
    D1_pres = (D1_pres << 8) | _i2cPort->read();

    // Request D2 conversion
    _i2cPort->beginTransmission(MS5837_ADDR);
    _i2cPort->write(MS5837_CONVERT_D2_8192);
    _i2cPort->endTransmission();

    delay(20); // Max conversion time per datasheet

    _i2cPort->beginTransmission(MS5837_ADDR);
    _i2cPort->write(MS5837_ADC_READ);
    _i2cPort->endTransmission();

    _i2cPort->requestFrom(MS5837_ADDR, 3);
    D2_temp = 0;
    D2_temp = _i2cPort->read();
    D2_temp = (D2_temp << 8) | _i2cPort->read();
    D2_temp = (D2_temp << 8) | _i2cPort->read();

    calculate();
}

void MS5837::calculate() {
    // Given C1-C6 and D1, D2, calculated TEMP and P
    // Do conversion first and then second order temp compensation

    int32_t dT = 0;
    int64_t SENS = 0;
    int64_t OFF = 0;
    int32_t SENSi = 0;
    int32_t OFFi = 0;
    int32_t Ti = 0;
    int64_t OFF2 = 0;
    int64_t SENS2 = 0;

    // Terms called
    dT = D2_temp - uint32_t(C[5]) * 256l;

    alogf(2 /*DEBUG*/, "ms5837", "C[1] = %" PRId16 "", C[1]);
    alogf(2 /*DEBUG*/, "ms5837", "C[2] = %" PRId16 "", C[2]);
    alogf(2 /*DEBUG*/, "ms5837", "C[3] = %" PRId16 "", C[3]);
    alogf(2 /*DEBUG*/, "ms5837", "C[4] = %" PRId16 "", C[4]);
    alogf(2 /*DEBUG*/, "ms5837", "C[5] = %" PRId16 "", C[5]);
    alogf(2 /*DEBUG*/, "ms5837", "C[6] = %" PRId16 "", C[6]);

    alogf(2 /*DEBUG*/, "ms5837", "dT = %" PRIu32 " - %" PRIu32 " * 256 = %" PRId32 "", D2_temp, uint32_t(C[5]), dT);

    if (_model == MS5837_02BA) {
        SENS = (int64_t(C[1]) * int64_t(65536l)) + ((int64_t(C[3]) * dT) / 128l);
        OFF = (int64_t(C[2]) * int64_t(131072l)) + ((int64_t(C[4]) * dT) / 64l);
        P = int64_t(D1_pres * (SENS / int64_t(2097152l)) - OFF) / (32768l);
    } else {
        SENS = (int64_t(C[1]) * 32768l) + ((int64_t(C[3]) * dT) / 256l);
        OFF = (int64_t(C[2]) * 65536l) + ((int64_t(C[4]) * dT) / 128l);
        P = (D1_pres * SENS / (2097152l) - OFF) / (8192l);
    }

    alogf(2 /*DEBUG*/, "ms5837", "SENS = %" PRId64 "", SENS);
    alogf(2 /*DEBUG*/, "ms5837", "OFF = %" PRId64 "", OFF);
    alogf(2 /*DEBUG*/, "ms5837", "P = %" PRId32 "", P);

    // Temp conversion
    TEMP = 2000l + int64_t(dT) * C[6] / 8388608LL;

    alogf(2 /*DEBUG*/, "ms5837", "TEMP = 2000 + %" PRId64 " * %" PRIu32 " / 8388608 = %" PRId32 "", int64_t(dT), uint32_t(C[6]), TEMP);

    // Second order compensation
    if (_model == MS5837_02BA) {
        if ((TEMP / 100) < 20) { // Low temp
            Ti = (11 * int64_t(dT) * int64_t(dT)) / (34359738368LL);
            OFFi = (31 * (TEMP - 2000) * (TEMP - 2000)) / 8;
            SENSi = (63 * (TEMP - 2000) * (TEMP - 2000)) / 32;

            alogf(2 /*DEBUG*/, "ms5837", "1.Ti = %" PRId32 "", Ti);
            alogf(2 /*DEBUG*/, "ms5837", "1.OFFi = %" PRId32 "", OFFi);
            alogf(2 /*DEBUG*/, "ms5837", "1.SENSi = %" PRId32 "", SENSi);
        }
    } else {
        if ((TEMP / 100) < 20) { // Low temp
            Ti = (3 * int64_t(dT) * int64_t(dT)) / (8589934592LL);
            OFFi = (3 * (TEMP - 2000) * (TEMP - 2000)) / 2;
            SENSi = (5 * (TEMP - 2000) * (TEMP - 2000)) / 8;

            alogf(2 /*DEBUG*/, "ms5837", "2.Ti = %" PRId32 "", Ti);

            if ((TEMP / 100) < -15) { // Very low temp
                OFFi = OFFi + 7 * (TEMP + 1500l) * (TEMP + 1500l);
                SENSi = SENSi + 4 * (TEMP + 1500l) * (TEMP + 1500l);

                alogf(2 /*DEBUG*/, "ms5837", "3.OFFi = %" PRId32 "", OFFi);
                alogf(2 /*DEBUG*/, "ms5837", "3.SENSi = %" PRId32 "", SENSi);
            } else {
                alogf(2 /*DEBUG*/, "ms5837", "2.OFFi = %" PRId32 "", OFFi);
                alogf(2 /*DEBUG*/, "ms5837", "2.SENSi = %" PRId32 "", SENSi);
            }
        } else if ((TEMP / 100) >= 20) { // High temp
            Ti = 2 * (dT * dT) / (137438953472LL);
            OFFi = (1 * (TEMP - 2000) * (TEMP - 2000)) / 16;
            SENSi = 0;

            alogf(2 /*DEBUG*/, "ms5837", "4.Ti = %" PRId32 "", Ti);
            alogf(2 /*DEBUG*/, "ms5837", "4.OFFi = %" PRId32 "", OFFi);
            alogf(2 /*DEBUG*/, "ms5837", "4.SENSi = %" PRId32 "", SENSi);
        }
    }

    OFF2 = OFF - OFFi; // Calculate pressure and temp second order
    SENS2 = SENS - SENSi;

    TEMP = (TEMP - Ti);

    alogf(2 /*DEBUG*/, "ms5837", "D1_pres = %" PRIu32 "", D1_pres);

    if (_model == MS5837_02BA) {
        P = (((int64_t(D1_pres) * int64_t(SENS2) / 2097152l) - OFF2) / 32768l);
    } else {
        P = (((int64_t(D1_pres) * int64_t(SENS2) / 2097152l) - OFF2) / 8192l);
    }

    alogf(2 /*DEBUG*/, "ms5837", "TEMP = %" PRId32 "", TEMP);
    alogf(2 /*DEBUG*/, "ms5837", "P = %" PRId32 "", P);
}

float MS5837::pressure(float conversion) {
    if (_model == MS5837_02BA) {
        return (float)P * conversion / 100.0f;
    } else {
        return (float)P * conversion / 10.0f;
    }
}

float MS5837::temperature() {
    return TEMP / 100.0f;
}

// The pressure sensor measures absolute pressure, so it will measure the atmospheric pressure + water pressure
// We subtract the atmospheric pressure to calculate the depth with only the water pressure
// The average atmospheric pressure of 101300 pascal is used for the calcuation, but atmospheric pressure varies
// If the atmospheric pressure is not 101300 at the time of reading, the depth reported will be offset
// In order to calculate the correct depth, the actual atmospheric pressure should be measured once in air, and
// that value should subtracted for subsequent depth calculations.
float MS5837::depth() {
    return (pressure(MS5837::Pa) - 101300) / (fluidDensity * 9.80665);
}

float MS5837::altitude() {
    return (1 - pow((pressure() / 1013.25), .190284)) * 145366.45 * .3048;
}

uint8_t MS5837::crc4(uint16_t n_prom[]) {
    uint16_t n_rem = 0;

    n_prom[0] = ((n_prom[0]) & 0x0FFF);
    n_prom[7] = 0;

    for (uint8_t i = 0; i < 16; i++) {
        if (i % 2 == 1) {
            n_rem ^= (uint16_t)((n_prom[i >> 1]) & 0x00FF);
        } else {
            n_rem ^= (uint16_t)(n_prom[i >> 1] >> 8);
        }
        for (uint8_t n_bit = 8; n_bit > 0; n_bit--) {
            if (n_rem & 0x8000) {
                n_rem = (n_rem << 1) ^ 0x3000;
            } else {
                n_rem = (n_rem << 1);
            }
        }
    }

    n_rem = ((n_rem >> 12) & 0x000F);

    return n_rem ^ 0x00;
}
