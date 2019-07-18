/* Auto-generated config file hpl_evsys_config.h */
#ifndef HPL_EVSYS_CONFIG_H
#define HPL_EVSYS_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <e> Channel 0 settings
// <id> evsys_channel_setting_0
#ifndef CONF_EVSYS_CHANNEL_SETTINGS_0
#define CONF_EVSYS_CHANNEL_SETTINGS_0 0
#endif

// <y> Edge detection
// <i> Indicates whether channel is enabled in standby sleep mode
// <EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT_Val"> No event output when using the resynchronized or synchronous path
// <EVSYS_CHANNEL_EDGSEL_RISING_EDGE_Val"> Event is detected on the rising edge of the signal from event generator
// <EVSYS_CHANNEL_EDGSEL_FALLING_EDGE_Val"> Event is detected on the falling edge of the signal from event generator
// <EVSYS_CHANNEL_EDGSEL_BOTH_EDGES_Val"> Event is detected on the rising and falling edge of the signal from event generator
// <id> evsys_edgsel_0
#ifndef CONF_EDGSEL_0
#define CONF_EDGSEL_0 EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT_Val
#endif

// <y> Path selection
// <i> Indicates which path for the event signal is chosen
// <EVSYS_CHANNEL_PATH_SYNCHRONOUS_Val"> Synchronous path
// <EVSYS_CHANNEL_PATH_RESYNCHRONIZED_Val"> Resynchronized path
// <EVSYS_CHANNEL_PATH_ASYNCHRONOUS_Val"> Asynchronous path
// <id> evsys_path_0
#ifndef CONF_PATH_0
#define CONF_PATH_0 EVSYS_CHANNEL_PATH_SYNCHRONOUS_Val
#endif

// <o> Event generator
// <i> Determines event generator for channel
// <0x0=>No event generator
// <0x1=>RTC compare 0 or Alarm 0
// <0x2=>RTC compare 1
// <0x3=>RTC overflow
// <0x4=>RTC period 0
// <0x5=>RTC period 1
// <0x6=>RTC period 2
// <0x7=>RTC period 3
// <0x8=>RTC period 4
// <0x9=>RTC period 5
// <0xA=>RTC period 6
// <0xB=>RTC period 7
// <0xC=>EIC external interrupt 0
// <0xD=>EIC external interrupt 1
// <0xE=>EIC external interrupt 2
// <0xF=>EIC external interrupt 3
// <0x10=>EIC external interrupt 4
// <0x11=>EIC external interrupt 5
// <0x12=>EIC external interrupt 6
// <0x13=>EIC external interrupt 7
// <0x14=>DMAC channel 0
// <0x15=>DMAC channel 1
// <0x16=>DMAC channel 2
// <0x17=>DMAC channel 3
// <0x1F=>TC1 overflow
// <0x20=>TC1 match/capture 0
// <0x21=>TC1 match/capture 1
// <0x22=>TC2 overflow
// <0x23=>TC2 match/capture 0
// <0x24=>TC2 match/capture 1
// <0x25=>ADC result ready
// <0x26=>ADC window monitor
// <id> evsys_evgen_0
#ifndef CONF_EVGEN_0
#define CONF_EVGEN_0 0
#endif

// <q> Overrun channel interrupt
// <i> Indicates whether overrun channel interrupt is enabled or not
// <id> evsys_ovr_0
#ifndef CONF_OVR_0
#define CONF_OVR_0 0
#endif

// <q> Event detected interrupt
// <i> Indicates whether event detected interrupt is enabled or not
// <id> evsys_evd_0
#ifndef CONF_EVD_0
#define CONF_EVD_0 0
#endif

// </e>

// <e> Channel 1 settings
// <id> evsys_channel_setting_1
#ifndef CONF_EVSYS_CHANNEL_SETTINGS_1
#define CONF_EVSYS_CHANNEL_SETTINGS_1 0
#endif

// <y> Edge detection
// <i> Indicates whether channel is enabled in standby sleep mode
// <EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT_Val"> No event output when using the resynchronized or synchronous path
// <EVSYS_CHANNEL_EDGSEL_RISING_EDGE_Val"> Event is detected on the rising edge of the signal from event generator
// <EVSYS_CHANNEL_EDGSEL_FALLING_EDGE_Val"> Event is detected on the falling edge of the signal from event generator
// <EVSYS_CHANNEL_EDGSEL_BOTH_EDGES_Val"> Event is detected on the rising and falling edge of the signal from event generator
// <id> evsys_edgsel_1
#ifndef CONF_EDGSEL_1
#define CONF_EDGSEL_1 EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT_Val
#endif

// <y> Path selection
// <i> Indicates which path for the event signal is chosen
// <EVSYS_CHANNEL_PATH_SYNCHRONOUS_Val"> Synchronous path
// <EVSYS_CHANNEL_PATH_RESYNCHRONIZED_Val"> Resynchronized path
// <EVSYS_CHANNEL_PATH_ASYNCHRONOUS_Val"> Asynchronous path
// <id> evsys_path_1
#ifndef CONF_PATH_1
#define CONF_PATH_1 EVSYS_CHANNEL_PATH_SYNCHRONOUS_Val
#endif

// <o> Event generator
// <i> Determines event generator for channel
// <0x0=>No event generator
// <0x1=>RTC compare 0 or Alarm 0
// <0x2=>RTC compare 1
// <0x3=>RTC overflow
// <0x4=>RTC period 0
// <0x5=>RTC period 1
// <0x6=>RTC period 2
// <0x7=>RTC period 3
// <0x8=>RTC period 4
// <0x9=>RTC period 5
// <0xA=>RTC period 6
// <0xB=>RTC period 7
// <0xC=>EIC external interrupt 0
// <0xD=>EIC external interrupt 1
// <0xE=>EIC external interrupt 2
// <0xF=>EIC external interrupt 3
// <0x10=>EIC external interrupt 4
// <0x11=>EIC external interrupt 5
// <0x12=>EIC external interrupt 6
// <0x13=>EIC external interrupt 7
// <0x14=>DMAC channel 0
// <0x15=>DMAC channel 1
// <0x16=>DMAC channel 2
// <0x17=>DMAC channel 3
// <0x1F=>TC1 overflow
// <0x20=>TC1 match/capture 0
// <0x21=>TC1 match/capture 1
// <0x22=>TC2 overflow
// <0x23=>TC2 match/capture 0
// <0x24=>TC2 match/capture 1
// <0x25=>ADC result ready
// <0x26=>ADC window monitor
// <id> evsys_evgen_1
#ifndef CONF_EVGEN_1
#define CONF_EVGEN_1 0
#endif

// <q> Overrun channel interrupt
// <i> Indicates whether overrun channel interrupt is enabled or not
// <id> evsys_ovr_1
#ifndef CONF_OVR_1
#define CONF_OVR_1 0
#endif

// <q> Event detected interrupt
// <i> Indicates whether event detected interrupt is enabled or not
// <id> evsys_evd_1
#ifndef CONF_EVD_1
#define CONF_EVD_1 0
#endif

// </e>

// <e> Channel 2 settings
// <id> evsys_channel_setting_2
#ifndef CONF_EVSYS_CHANNEL_SETTINGS_2
#define CONF_EVSYS_CHANNEL_SETTINGS_2 0
#endif

// <y> Edge detection
// <i> Indicates whether channel is enabled in standby sleep mode
// <EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT_Val"> No event output when using the resynchronized or synchronous path
// <EVSYS_CHANNEL_EDGSEL_RISING_EDGE_Val"> Event is detected on the rising edge of the signal from event generator
// <EVSYS_CHANNEL_EDGSEL_FALLING_EDGE_Val"> Event is detected on the falling edge of the signal from event generator
// <EVSYS_CHANNEL_EDGSEL_BOTH_EDGES_Val"> Event is detected on the rising and falling edge of the signal from event generator
// <id> evsys_edgsel_2
#ifndef CONF_EDGSEL_2
#define CONF_EDGSEL_2 EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT_Val
#endif

// <y> Path selection
// <i> Indicates which path for the event signal is chosen
// <EVSYS_CHANNEL_PATH_SYNCHRONOUS_Val"> Synchronous path
// <EVSYS_CHANNEL_PATH_RESYNCHRONIZED_Val"> Resynchronized path
// <EVSYS_CHANNEL_PATH_ASYNCHRONOUS_Val"> Asynchronous path
// <id> evsys_path_2
#ifndef CONF_PATH_2
#define CONF_PATH_2 EVSYS_CHANNEL_PATH_SYNCHRONOUS_Val
#endif

// <o> Event generator
// <i> Determines event generator for channel
// <0x0=>No event generator
// <0x1=>RTC compare 0 or Alarm 0
// <0x2=>RTC compare 1
// <0x3=>RTC overflow
// <0x4=>RTC period 0
// <0x5=>RTC period 1
// <0x6=>RTC period 2
// <0x7=>RTC period 3
// <0x8=>RTC period 4
// <0x9=>RTC period 5
// <0xA=>RTC period 6
// <0xB=>RTC period 7
// <0xC=>EIC external interrupt 0
// <0xD=>EIC external interrupt 1
// <0xE=>EIC external interrupt 2
// <0xF=>EIC external interrupt 3
// <0x10=>EIC external interrupt 4
// <0x11=>EIC external interrupt 5
// <0x12=>EIC external interrupt 6
// <0x13=>EIC external interrupt 7
// <0x14=>DMAC channel 0
// <0x15=>DMAC channel 1
// <0x16=>DMAC channel 2
// <0x17=>DMAC channel 3
// <0x1F=>TC1 overflow
// <0x20=>TC1 match/capture 0
// <0x21=>TC1 match/capture 1
// <0x22=>TC2 overflow
// <0x23=>TC2 match/capture 0
// <0x24=>TC2 match/capture 1
// <0x25=>ADC result ready
// <0x26=>ADC window monitor
// <id> evsys_evgen_2
#ifndef CONF_EVGEN_2
#define CONF_EVGEN_2 0
#endif

// <q> Overrun channel interrupt
// <i> Indicates whether overrun channel interrupt is enabled or not
// <id> evsys_ovr_2
#ifndef CONF_OVR_2
#define CONF_OVR_2 0
#endif

// <q> Event detected interrupt
// <i> Indicates whether event detected interrupt is enabled or not
// <id> evsys_evd_2
#ifndef CONF_EVD_2
#define CONF_EVD_2 0
#endif

// </e>

// <e> Channel 3 settings
// <id> evsys_channel_setting_3
#ifndef CONF_EVSYS_CHANNEL_SETTINGS_3
#define CONF_EVSYS_CHANNEL_SETTINGS_3 0
#endif

// <y> Edge detection
// <i> Indicates whether channel is enabled in standby sleep mode
// <EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT_Val"> No event output when using the resynchronized or synchronous path
// <EVSYS_CHANNEL_EDGSEL_RISING_EDGE_Val"> Event is detected on the rising edge of the signal from event generator
// <EVSYS_CHANNEL_EDGSEL_FALLING_EDGE_Val"> Event is detected on the falling edge of the signal from event generator
// <EVSYS_CHANNEL_EDGSEL_BOTH_EDGES_Val"> Event is detected on the rising and falling edge of the signal from event generator
// <id> evsys_edgsel_3
#ifndef CONF_EDGSEL_3
#define CONF_EDGSEL_3 EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT_Val
#endif

// <y> Path selection
// <i> Indicates which path for the event signal is chosen
// <EVSYS_CHANNEL_PATH_SYNCHRONOUS_Val"> Synchronous path
// <EVSYS_CHANNEL_PATH_RESYNCHRONIZED_Val"> Resynchronized path
// <EVSYS_CHANNEL_PATH_ASYNCHRONOUS_Val"> Asynchronous path
// <id> evsys_path_3
#ifndef CONF_PATH_3
#define CONF_PATH_3 EVSYS_CHANNEL_PATH_SYNCHRONOUS_Val
#endif

// <o> Event generator
// <i> Determines event generator for channel
// <0x0=>No event generator
// <0x1=>RTC compare 0 or Alarm 0
// <0x2=>RTC compare 1
// <0x3=>RTC overflow
// <0x4=>RTC period 0
// <0x5=>RTC period 1
// <0x6=>RTC period 2
// <0x7=>RTC period 3
// <0x8=>RTC period 4
// <0x9=>RTC period 5
// <0xA=>RTC period 6
// <0xB=>RTC period 7
// <0xC=>EIC external interrupt 0
// <0xD=>EIC external interrupt 1
// <0xE=>EIC external interrupt 2
// <0xF=>EIC external interrupt 3
// <0x10=>EIC external interrupt 4
// <0x11=>EIC external interrupt 5
// <0x12=>EIC external interrupt 6
// <0x13=>EIC external interrupt 7
// <0x14=>DMAC channel 0
// <0x15=>DMAC channel 1
// <0x16=>DMAC channel 2
// <0x17=>DMAC channel 3
// <0x1F=>TC1 overflow
// <0x20=>TC1 match/capture 0
// <0x21=>TC1 match/capture 1
// <0x22=>TC2 overflow
// <0x23=>TC2 match/capture 0
// <0x24=>TC2 match/capture 1
// <0x25=>ADC result ready
// <0x26=>ADC window monitor
// <id> evsys_evgen_3
#ifndef CONF_EVGEN_3
#define CONF_EVGEN_3 0
#endif

// <q> Overrun channel interrupt
// <i> Indicates whether overrun channel interrupt is enabled or not
// <id> evsys_ovr_3
#ifndef CONF_OVR_3
#define CONF_OVR_3 0
#endif

// <q> Event detected interrupt
// <i> Indicates whether event detected interrupt is enabled or not
// <id> evsys_evd_3
#ifndef CONF_EVD_3
#define CONF_EVD_3 0
#endif

// </e>

// <e> Channel 4 settings
// <id> evsys_channel_setting_4
#ifndef CONF_EVSYS_CHANNEL_SETTINGS_4
#define CONF_EVSYS_CHANNEL_SETTINGS_4 0
#endif

// <y> Edge detection
// <i> Indicates whether channel is enabled in standby sleep mode
// <EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT_Val"> No event output when using the resynchronized or synchronous path
// <EVSYS_CHANNEL_EDGSEL_RISING_EDGE_Val"> Event is detected on the rising edge of the signal from event generator
// <EVSYS_CHANNEL_EDGSEL_FALLING_EDGE_Val"> Event is detected on the falling edge of the signal from event generator
// <EVSYS_CHANNEL_EDGSEL_BOTH_EDGES_Val"> Event is detected on the rising and falling edge of the signal from event generator
// <id> evsys_edgsel_4
#ifndef CONF_EDGSEL_4
#define CONF_EDGSEL_4 EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT_Val
#endif

// <y> Path selection
// <i> Indicates which path for the event signal is chosen
// <EVSYS_CHANNEL_PATH_SYNCHRONOUS_Val"> Synchronous path
// <EVSYS_CHANNEL_PATH_RESYNCHRONIZED_Val"> Resynchronized path
// <EVSYS_CHANNEL_PATH_ASYNCHRONOUS_Val"> Asynchronous path
// <id> evsys_path_4
#ifndef CONF_PATH_4
#define CONF_PATH_4 EVSYS_CHANNEL_PATH_SYNCHRONOUS_Val
#endif

// <o> Event generator
// <i> Determines event generator for channel
// <0x0=>No event generator
// <0x1=>RTC compare 0 or Alarm 0
// <0x2=>RTC compare 1
// <0x3=>RTC overflow
// <0x4=>RTC period 0
// <0x5=>RTC period 1
// <0x6=>RTC period 2
// <0x7=>RTC period 3
// <0x8=>RTC period 4
// <0x9=>RTC period 5
// <0xA=>RTC period 6
// <0xB=>RTC period 7
// <0xC=>EIC external interrupt 0
// <0xD=>EIC external interrupt 1
// <0xE=>EIC external interrupt 2
// <0xF=>EIC external interrupt 3
// <0x10=>EIC external interrupt 4
// <0x11=>EIC external interrupt 5
// <0x12=>EIC external interrupt 6
// <0x13=>EIC external interrupt 7
// <0x14=>DMAC channel 0
// <0x15=>DMAC channel 1
// <0x16=>DMAC channel 2
// <0x17=>DMAC channel 3
// <0x1F=>TC1 overflow
// <0x20=>TC1 match/capture 0
// <0x21=>TC1 match/capture 1
// <0x22=>TC2 overflow
// <0x23=>TC2 match/capture 0
// <0x24=>TC2 match/capture 1
// <0x25=>ADC result ready
// <0x26=>ADC window monitor
// <id> evsys_evgen_4
#ifndef CONF_EVGEN_4
#define CONF_EVGEN_4 0
#endif

// <q> Overrun channel interrupt
// <i> Indicates whether overrun channel interrupt is enabled or not
// <id> evsys_ovr_4
#ifndef CONF_OVR_4
#define CONF_OVR_4 0
#endif

// <q> Event detected interrupt
// <i> Indicates whether event detected interrupt is enabled or not
// <id> evsys_evd_4
#ifndef CONF_EVD_4
#define CONF_EVD_4 0
#endif

// </e>

// <e> Channel 5 settings
// <id> evsys_channel_setting_5
#ifndef CONF_EVSYS_CHANNEL_SETTINGS_5
#define CONF_EVSYS_CHANNEL_SETTINGS_5 0
#endif

// <y> Edge detection
// <i> Indicates whether channel is enabled in standby sleep mode
// <EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT_Val"> No event output when using the resynchronized or synchronous path
// <EVSYS_CHANNEL_EDGSEL_RISING_EDGE_Val"> Event is detected on the rising edge of the signal from event generator
// <EVSYS_CHANNEL_EDGSEL_FALLING_EDGE_Val"> Event is detected on the falling edge of the signal from event generator
// <EVSYS_CHANNEL_EDGSEL_BOTH_EDGES_Val"> Event is detected on the rising and falling edge of the signal from event generator
// <id> evsys_edgsel_5
#ifndef CONF_EDGSEL_5
#define CONF_EDGSEL_5 EVSYS_CHANNEL_EDGSEL_NO_EVT_OUTPUT_Val
#endif

// <y> Path selection
// <i> Indicates which path for the event signal is chosen
// <EVSYS_CHANNEL_PATH_SYNCHRONOUS_Val"> Synchronous path
// <EVSYS_CHANNEL_PATH_RESYNCHRONIZED_Val"> Resynchronized path
// <EVSYS_CHANNEL_PATH_ASYNCHRONOUS_Val"> Asynchronous path
// <id> evsys_path_5
#ifndef CONF_PATH_5
#define CONF_PATH_5 EVSYS_CHANNEL_PATH_SYNCHRONOUS_Val
#endif

// <o> Event generator
// <i> Determines event generator for channel
// <0x0=>No event generator
// <0x1=>RTC compare 0 or Alarm 0
// <0x2=>RTC compare 1
// <0x3=>RTC overflow
// <0x4=>RTC period 0
// <0x5=>RTC period 1
// <0x6=>RTC period 2
// <0x7=>RTC period 3
// <0x8=>RTC period 4
// <0x9=>RTC period 5
// <0xA=>RTC period 6
// <0xB=>RTC period 7
// <0xC=>EIC external interrupt 0
// <0xD=>EIC external interrupt 1
// <0xE=>EIC external interrupt 2
// <0xF=>EIC external interrupt 3
// <0x10=>EIC external interrupt 4
// <0x11=>EIC external interrupt 5
// <0x12=>EIC external interrupt 6
// <0x13=>EIC external interrupt 7
// <0x14=>DMAC channel 0
// <0x15=>DMAC channel 1
// <0x16=>DMAC channel 2
// <0x17=>DMAC channel 3
// <0x1F=>TC1 overflow
// <0x20=>TC1 match/capture 0
// <0x21=>TC1 match/capture 1
// <0x22=>TC2 overflow
// <0x23=>TC2 match/capture 0
// <0x24=>TC2 match/capture 1
// <0x25=>ADC result ready
// <0x26=>ADC window monitor
// <id> evsys_evgen_5
#ifndef CONF_EVGEN_5
#define CONF_EVGEN_5 0
#endif

// <q> Overrun channel interrupt
// <i> Indicates whether overrun channel interrupt is enabled or not
// <id> evsys_ovr_5
#ifndef CONF_OVR_5
#define CONF_OVR_5 0
#endif

// <q> Event detected interrupt
// <i> Indicates whether event detected interrupt is enabled or not
// <id> evsys_evd_5
#ifndef CONF_EVD_5
#define CONF_EVD_5 0
#endif

// </e>

// <h>  Event User Security Attribution Settings

//</h>

// <h> DMAC events
// <o> Channel selection for DMAC channel 0
// <0x0=>No channel output selected
// <0x1=>Channel 0
// <0x2=>Channel 1
// <0x3=>Channel 2
// <0x4=>Channel 3
// <0x5=>Channel 4
// <0x6=>Channel 5
// <id> evsys_channel_0
// <i> Indicates which channel is chosen for user
#ifndef CONF_CHANNEL_0
#define CONF_CHANNEL_0 0
#endif

// <o> Channel selection for DMAC channel 1
// <0x0=>No channel output selected
// <0x1=>Channel 0
// <0x2=>Channel 1
// <0x3=>Channel 2
// <0x4=>Channel 3
// <0x5=>Channel 4
// <0x6=>Channel 5
// <id> evsys_channel_1
// <i> Indicates which channel is chosen for user
#ifndef CONF_CHANNEL_1
#define CONF_CHANNEL_1 0
#endif

// <o> Channel selection for DMAC channel 2
// <0x0=>No channel output selected
// <0x1=>Channel 0
// <0x2=>Channel 1
// <0x3=>Channel 2
// <0x4=>Channel 3
// <0x5=>Channel 4
// <0x6=>Channel 5
// <id> evsys_channel_2
// <i> Indicates which channel is chosen for user
#ifndef CONF_CHANNEL_2
#define CONF_CHANNEL_2 0
#endif

// <o> Channel selection for DMAC channel 3
// <0x0=>No channel output selected
// <0x1=>Channel 0
// <0x2=>Channel 1
// <0x3=>Channel 2
// <0x4=>Channel 3
// <0x5=>Channel 4
// <0x6=>Channel 5
// <id> evsys_channel_3
// <i> Indicates which channel is chosen for user
#ifndef CONF_CHANNEL_3
#define CONF_CHANNEL_3 0
#endif
//</h>

// <h> Reserved
#ifndef CONF_CHANNEL_4
#define CONF_CHANNEL_4 0
#endif

#ifndef CONF_CHANNEL_5
#define CONF_CHANNEL_5 0
#endif

#ifndef CONF_CHANNEL_6
#define CONF_CHANNEL_6 0
#endif

#ifndef CONF_CHANNEL_7
#define CONF_CHANNEL_7 0
#endif

#ifndef CONF_CHANNEL_8
#define CONF_CHANNEL_8 0
#endif

#ifndef CONF_CHANNEL_9
#define CONF_CHANNEL_9 0
#endif
//</h>

// <h> TC event
// <o> Channel selection for TC1 event
// <0x0=>No channel output selected
// <0x1=>Channel 0
// <0x2=>Channel 1
// <0x3=>Channel 2
// <0x4=>Channel 3
// <0x5=>Channel 4
// <0x6=>Channel 5
// <id> evsys_channel_10
// <i> Indicates which channel is chosen for user
#ifndef CONF_CHANNEL_10
#define CONF_CHANNEL_10 0
#endif

// <o> Channel selection for TC2 event
// <0x0=>No channel output selected
// <0x1=>Channel 0
// <0x2=>Channel 1
// <0x3=>Channel 2
// <0x4=>Channel 3
// <0x5=>Channel 4
// <0x6=>Channel 5
// <id> evsys_channel_11
// <i> Indicates which channel is chosen for user
#ifndef CONF_CHANNEL_11
#define CONF_CHANNEL_11 0
#endif
//</h>

// <h> ADC events
// <o> Channel selection for ADC start conversion event
// <0x0=>No channel output selected
// <0x1=>Channel 0
// <0x2=>Channel 1
// <0x3=>Channel 2
// <0x4=>Channel 3
// <0x5=>Channel 4
// <0x6=>Channel 5
// <id> evsys_channel_12
// <i> Indicates which channel is chosen for user
#ifndef CONF_CHANNEL_12
#define CONF_CHANNEL_12 0
#endif

// <o> Channel selection for ADC flush event
// <0x0=>No channel output selected
// <0x1=>Channel 0
// <0x2=>Channel 1
// <0x3=>Channel 2
// <0x4=>Channel 3
// <0x5=>Channel 4
// <0x6=>Channel 5
// <id> evsys_channel_13
// <i> Indicates which channel is chosen for user
#ifndef CONF_CHANNEL_13
#define CONF_CHANNEL_13 0
#endif
//</h>

// <<< end of configuration section >>>

#endif // HPL_EVSYS_CONFIG_H
