A DVFS Algorithm STM32F7 ARM Cortex-M7 MCU 
---
##Discliamer
This project is my Thesis for TAIST Tokyo Tech International Program of Kasetsart University, Thailand. The project is under MIT license, you are allow to use my work for any research and development purpose but please cite this repository in your work otherwise violation of license will be enforced.

##Abstract
This on-development project is anticipated to demonstrate a softwware-level energy management by implementing a Dynmaic Voltage Frequency Scaling (DVFS) Technique in order to minimize processor energy consumption. FreeRTOS, a open-source real-time operating System (RTOS), kernel is modified by inserting a optimal frequency calculation module determing from processor active power consumption so called
1. On-chip Workload
	A workload that depend on processor's operating frequency 
2. Off-chip Workload
	A workload that DOES NOT depend on processor's operating frequency
This technique employ a task-level frequency adjustment, in other word, every tasks has its own operating frequency so that energy consumption would be minimized for every execution of task instance.

##To-do List
- [x] Create Development Environment using IAR
- [x] Implement a frequency adjustment module on FreeRTOS
- [] Develop a optimal frequency calculation
- [] Evaluate algorithm performance

