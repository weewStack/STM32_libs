#include "adc_drive.h"
#include "msg_drive.h"

/*
PA0 -> ADC12_IN0
PA1 -> ADC12_IN1
PA2 -> ADC12_IN2
PA3 -> ADC12_IN3
PA4 -> ADC12_IN4
PA5 -> ADC12_IN5
PA6 -> ADC12_IN6
PA7 -> ADC12_IN7
PB0 -> ADC12_IN8
PB1 -> ADC12_IN9

PC0 -> ADC12_IN10
PC1 -> ADC12_IN11
PC2 -> ADC12_IN12
PC3 -> ADC12_IN13
PC4 -> ADC12_IN14
PC5 -> ADC12_IN15

ADC12_IN16 input channel which is used to convert the sensor output voltage into a digital value.


*/


int analog_rx = 0;
int main(void)
{

	systick_init(); // Initialize the delay function
	UART_init(2,115200); // Initialize the Uart communication : 
	adc_init(adc1, PA, 0);// Initialize the ADC: adc_init(ADC1 or ADC2, Port, Pin)

while(1)
{
	if(adc_check(adc1, PA, 0)) // Checking if the ADC value is ready
	{
		analog_rx = adc_rx(adc1, PA, 0);// Getting the ADC Value
		basic_msg(2, analog_rx); // Transforming the ADC Value and send it through UART
	}
}
}
