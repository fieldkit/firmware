source ~/tools/bin/micro-trace-buffer.py
load
b HardFault_Handler
b hard_fault_report
b os_error
monitor reset
continue
