# Self-sustainable-alert-device
The device is conceptualised and built from scratch including program on which it process the information and sends signal to various actuators. The PCB design is also built from scratch and the device is entirely ready to be sent for production, along with the various safety considerations and enhancements.
All the failure conditions of the device is also determined after full testing of the device.

The device is made to interface with a capacitive load and sense various parameters like power supply to the device and if its low, switching it to a battery backup on-board, measure the battery voltage along with regulating and monitoring the battery health and state, etc., along with logging and sending the parameters to the cloud through 2G-3G band using a GSM/GPRS system.

**The device was built using the following approach:**

1.5V 2A SMPS takes 220V AC as input and gives 5V as output so as to provide main power to the device.

2.Here we interface an LED to act as status LED for mains Power.

3.TP4056 Li-ion Single Cell Battery Charger Module is interfaced here. The TP4056 act as Charge controller and 8205a and DW01 IC for protection from undercharge and overcharge. This system can easily be designed on the main circuit but this module cost approximately the same so I used it. Here IN+ and IN- takes input from the output of SMPS to charge battery during mains power ON state. Battery is connected to BAT+ and BAT- while output is taken from OUT+ and OUT-.

4.A 5V relay is used now to switch the device power from mains to battery reserve. The coil is connected to the 5V and GND output of the SMPS. Here a IN4007 diode can be used to act as flyback diode. The 5V output of SMPS is connected to NO and OUT+ of TP4056 module is connected to NC. Now the main VCC of the device is connected to the COM. IN-, OUT- and GND of SMPS are connected together to give device a common ground GND for the device. I didn't used MOSFET although it's fast and can switch faster to battery power but it's difficult to switch a MOSFET at 5V.

5.Neoway M590e is the cheapest GSM module available in the market. The link provided directs to a kit that will make it compatible to the 5V supply. If we use the IC directly in the circuit, it will become even cheap. The TX of GSM is connected to RX of MCU and RX of GSM is connected to TX of MCU.

6.For sensing status of the load, an optocoupler PC817 is interfaced to the screw terminal from the load through capacitor (AC) like Mica and resistor. A diode is used in parallel to ensure rectification. When the AC signal is sensed from terminal LED sends a light signal to base of transistor and complete the circuit. Hence a signal is sent through a line given to PD5 of the MCU to map the analog value here to a digital 0 or 1 to enable the MCU to read status of load.

7.Earlier I used an isolated non-contact voltage sensor circuit that worked on principle of mutual inductance of inductor kept in close proximity of screw terminal from load but its difficult to shield it from induction from other currents in circuit or define its range. Although being isolated was it's plus point but the idea was dropped.

8.Using AtMega 328p microcontroller here gives manifold advantages here like the flash memory, the speed, GPIOs along with adaptability to use arduino boot-loader to use user friendly libraries. An LED is interfaced to PB1 pin of the MCU which is a PWM pin. We can use AT command AT+CSQ to get the RSSI of registered network of GSM and map it to produce PWM signal to turn off LED for 0 signal, turn dim for half signal strength and glow brightly for full signal. We take a input from the opto-coupler terminal of AC sense to PD2 which is external interrupt pin INT0. Whenever the mains supply is off, it causes a change in the status, commencing an interrupt and ISR is executed to send the status of Load and mains along with timestamp retrieved from GPS module using AT command and put the device to sleep mode, toggling a flag variable. There are 6 sleep modes available for the same in AtMega328 giving it flexibility to choose what to operate during sleep. This is one of the reason to choose this microcontroller. Now, when the next change will occur on INT0 (that is mains is ON), the flag variable will be checked in the ISR causing to wake up the device and change state to active. The LED interfaced to PB2 will glow according to the code, once the mains supply is detected off. This indicates that the device is working on battery now. An LED is also interfaced to PB3 that will glow according to the code when the Load is sensed at PD5.

**HIGH LEVEL DIAGRAM FOR THE DEVICE**
![Image text](https://github.com/KNITPhoenix/Self-sustainable-alert-device/blob/main/high_level.PNG)

**MODULE LEVEL DIAGRAM**
![Image text](https://github.com/KNITPhoenix/Self-sustainable-alert-device/blob/main/Module%20level%20diagram.jpg)

**one of the attractive features of the device is tamper proofing: A simple circuit that provides tamper proofing for the device such that if someone tries to steal or detach the device from the attached load, it will send an alert message, mentioning that the device has been tampered.**

![Image text](https://github.com/KNITPhoenix/Self-sustainable-alert-device/blob/main/tamper_proof.PNG)

1.A button is attached to a digital pin which is initially logic 0. We also initialize a variable k=0.

2.Now, whenever we install the device , we power it with 220V and press pinhole reset button attached to logic given above. It toggles the status of digital pin and cause k=1.

3.Now, in order to ethically remove the device from the starter, we must press the pinhole reset button again before disconnecting the power, which makes k=0.

4.Else, if someone tries to remove the device forcefully from the device, he will be completely unaware of the reset button since its pinhole type, which cause the microcontroller to detect that AC mains is set to 0 through optocoupler circuit, while k is still equal to 1. This cause an ISR to initiate and thus sending an alert. To initiate ISR in this case we should attach the button to one of PC pins so as to get pin-change interrupt.
