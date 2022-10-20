#pragma once

#include "common.h"

namespace fk {

// TODO Still working on moving these, right now they're referenced in static
// constructors, which makes moving them behind get_pins() harder.
constexpr uint8_t SPI_FLASH_CS_BANK_1 = 44u; // PC11
constexpr uint8_t SPI_FLASH_CS_BANK_2 = 41u; // PC12
constexpr uint8_t SPI_FLASH_CS_BANK_3 = 40u; // PC13
constexpr uint8_t SPI_FLASH_CS_BANK_4 = 43u; // PC14

constexpr uint8_t PIN_SD_CS = 8u; // PB18

constexpr uint8_t QSPI_FLASH_CS = 90u; // PB11 (PIN_QSPI_CS)

#if defined(FK_UNDERWATER)
constexpr uint8_t GPS_POWER = 55u;
#else
constexpr uint8_t WINC1500_CS = 95u;    // PC25
constexpr uint8_t WINC1500_POWER = 56u; // PB07
constexpr uint8_t WINC1500_IRQ = 97u;   // PC27
constexpr uint8_t WINC1500_RESET = 96u; // PC26

constexpr uint8_t GPS_POWER = 55u;

constexpr uint8_t LORA_POWER = 76u; // PC30

constexpr uint8_t MODULE_EEPROM_LOCK = 13u; // PB01
constexpr uint8_t MODULE_SWAP = 69u;        // PB03
constexpr uint8_t MODULE_SOLO_ENABLE = 75u; // PB03
#endif

}