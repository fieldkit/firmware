/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _VARIANT_FIELDKIT_CORE_
#define _VARIANT_FIELDKIT_CORE_

// The definitions here needs a SAMD core >=1.6.10
#define ARDUINO_SAMD_VARIANT_COMPLIANCE 10610

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/** Frequency of the board main oscillator */
#define VARIANT_MAINOSC (32768ul)

/** Master clock frequency */
#define VARIANT_MCK (120000000ul)

#define VARIANT_GCLK0_FREQ (120000000UL)
#define VARIANT_GCLK1_FREQ (48000000UL)
#define VARIANT_GCLK2_FREQ (100000000UL)

/** */
#define SAMD51_EXTERNAL_CLOCK_INPUT

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "WVariant.h"

#ifdef __cplusplus
#include "SERCOM.h"
#include "Uart.h"
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

// Number of pins defined in PinDescription array
#define PINS_COUNT         (98u)
#define NUM_DIGITAL_PINS   (98u)
#define NUM_ANALOG_INPUTS  (0u)
#define NUM_ANALOG_OUTPUTS (0u)

#define analogInputToDigitalPin(p) (-1)
#define digitalPinToPort(P)        (&(PORT->Group[g_APinDescription[P].ulPort]))
#define digitalPinToBitMask(P)     (1 << g_APinDescription[P].ulPin)
#define portOutputRegister(port)   (&(port->OUT.reg))
#define portInputRegister(port)    (&(port->IN.reg))
#define portModeRegister(port)     (&(port->DIR.reg))
#define digitalPinHasPWM(P)        (g_APinDescription[P].ulPWMChannel != NOT_ON_PWM || g_APinDescription[P].ulTCChannel != NOT_ON_TIMER)

/*
 * Analog pins (We don't use any)
 */
#define PIN_A0 (uint8_t)(-1)
#define PIN_A1 (uint8_t)(-1)

static const uint8_t A0 = (uint8_t)(-1);
static const uint8_t A1 = (uint8_t)(-1);

#define ADC_RESOLUTION 12

/**
 * Serial Interfaces
 */

#define PIN_SERIAL0_RX (0ul)
#define PIN_SERIAL0_TX (1ul)
#define PAD_SERIAL0_RX (SERCOM_RX_PAD_1)
#define PAD_SERIAL0_TX (UART_TX_PAD_0)

#define PIN_SERIAL1_RX (16ul)
#define PIN_SERIAL1_TX (17ul)
#define PAD_SERIAL1_RX (SERCOM_RX_PAD_1)
#define PAD_SERIAL1_TX (UART_TX_PAD_0)

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 3

#define PIN_SPI_MISO (38u) // PB15   SERCOM4_PAD3 MISO
#define PIN_SPI_MOSI (18u) // PB12   SERCOM4_PAD0 MOSI
#define PIN_SPI_SCK  (19u) // PB13   SERCOM4_PAD1 SCK
#define PERIPH_SPI   sercom4
#define PAD_SPI_TX   SPI_PAD_0_SCK_1 // (TX) MISO = SERCOM4.0, SCK = SERCOM4.1
#define PAD_SPI_RX   SERCOM_RX_PAD_3 // (RX) MISO = SERCOM4.3

#define PIN_SPI1_MISO (80u) // PB29   SERCOM2_PAD3 MISO
#define PIN_SPI1_MOSI (82u) // PB26   SERCOM2_PAD0 MOSI
#define PIN_SPI1_SCK  (81u) // PB27   SERCOM2_PAD1 SCK
#define PERIPH_SPI1   sercom2
#define PAD_SPI1_TX   SPI_PAD_0_SCK_1 // (TX) MOSI = SERCOM2.0, SCK = SERCOM2.1
#define PAD_SPI1_RX   SERCOM_RX_PAD_3 // (RX) MISO = SERCOM2.3

#define PIN_SPI2_MISO (29u) // PB19 SERCOM5/PAD[3]
#define PIN_SPI2_MOSI (14u) // PB16 SERCOM5/PAD[0]
#define PIN_SPI2_SCK  (15u) // PB17 SERCOM5/PAD[1]
#define PERIPH_SPI2   sercom5
#define PAD_SPI2_TX   SPI_PAD_0_SCK_1 // (TX) MOSI = SERCOM5.0, SCK = SERCOM5.1
#define PAD_SPI2_RX   SERCOM_RX_PAD_3 // (RX) MISO = SERCOM5.3

#define WINC1501_SPI SPI1
#define SD_SPI       SPI2

static const uint8_t SS = 0;
static const uint8_t MOSI = 0;
static const uint8_t MISO = 0;
static const uint8_t SCK = 0;

// SPI_PAD_0_SCK_1 = 0,
// SPI_PAD_2_SCK_3,
// SPI_PAD_3_SCK_1,
// SPI_PAD_0_SCK_3

// SERCOM_RX_PAD_0 = 0,
// SERCOM_RX_PAD_1,
// SERCOM_RX_PAD_2,
// SERCOM_RX_PAD_3

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 3

#define PIN_WIRE_SDA    (20u)
#define PIN_WIRE_SCL    (21u)
#define PERIPH_WIRE     sercom3
#define WIRE_IT_HANDLER SERCOM3_Handler

#define PIN_WIRE1_SDA    (16u) // PC22  SERCOM1.0 (PAD[0] = SDA) SERCOM3.1  (PAD[1] = SCL)
#define PIN_WIRE1_SCL    (17u) // PC23  SERCOM1.1 (PAD[1] = SCL) SERCOM3.0  (PAD[0] = SDA)
#define PERIPH_WIRE1     sercom1
#define WIRE1_IT_HANDLER SERCOM1_Handler

#define PIN_WIRE2_SDA    (24u) // PC16  SERCOM6.0 (PAD[0] = SDA) SERCOM0.1  (PAD[1] = SCL)
#define PIN_WIRE2_SCL    (25u) // PC17  SERCOM6.1 (PAD[1] = SCL) SERCOM0.0  (PAD[0] = SDA)
#define PERIPH_WIRE2     sercom6
#define WIRE2_IT_HANDLER SERCOM6_Handler

/*
 * USB
 */
#define PIN_USB_HOST_ENABLE (77ul)
#define PIN_USB_DM          (78ul)
#define PIN_USB_DP          (79ul)

/*
 * I2S Interfaces
 */
#define I2S_INTERFACES_COUNT 0

/*
 * QSPI Interfaces
 */
#define PIN_QSPI_SCK (89u)
#define PIN_QSPI_CS  (90u)
#define PIN_QSPI_IO0 (91u)
#define PIN_QSPI_IO1 (92u)
#define PIN_QSPI_IO2 (93u)
#define PIN_QSPI_IO3 (94u)

// TODO: Meaningful value for this?
#define VARIANT_QSPI_BAUD_DEFAULT 5000000

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus

/*	=========================
 *	===== SERCOM DEFINITION
 *	=========================
 */
extern SERCOM sercom0;
extern SERCOM sercom1;
extern SERCOM sercom2;
extern SERCOM sercom3;
extern SERCOM sercom4;
extern SERCOM sercom5;
extern SERCOM sercom6;
extern SERCOM sercom7;

extern Uart Serial0;
extern Uart Serial1;

#endif

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
// #define SERIAL_PORT_USBVIRTUAL      Serial
// #define SERIAL_PORT_MONITOR         Serial

// #define SERIAL_PORT_HARDWARE        Serial1
// #define SERIAL_PORT_HARDWARE_OPEN   Serial1

#endif /* _VARIANT_FIELDKIT_CORE_ */
