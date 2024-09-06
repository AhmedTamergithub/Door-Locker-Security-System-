#include "external_eeprom.h"
#include "GPIO_interface.h"
#include "util/delay.h"
#include "TWI_interface.h"
#include "BIT_Calc.h"
#include "STD_Types.h"
#include "UART_interface.h"
#include "DCMOTOR_interface.h"
#include "Timer0_interface.h"
#include "Timer1_interface.h"
#include "GIE_interface.h"
u8 wait=0;
u8 motor_trigger=0;
u8 data_read;
// Global Variables
u32 uartnum1_recieved;
u32 uartnum2_recieved;
u32 eeprom_return;
u32 saved_password;
u32 menu_password_rcv;
u8 stop_it;
u8  menu_function_ended=0;
u8 open_timer1=0;
u8 end=0;
u32 newpass;
// Function Declaration
u32 UART_Receive32BitNumber(void);
u32 EEPROM_read32Byte(u16 startAddress);
void door_password_func(void);
void timer1_isr(void);
//void open_motor(void);
void motor_isr(void);
void goto_timer1(void);
void Changing_pass(void);
int main(void) {

    DCMOTOR_Init();
    UART_Config();
    I2C_config();
    TIMER0_u8SetCallBack(&motor_isr);
    TIMER1_u8SetCallBack(&timer1_isr);
    // Set RX as input and TX as output for UART
    DIO_u8SetPinDirection(DIO_u8PORTD, DIO_u8PIN0, DIO_u8PIN_INPUT);
    DIO_u8SetPinDirection(DIO_u8PORTD, DIO_u8PIN1, DIO_u8PIN_OUTPUT);

    // Set PORTA and PORTC as output (PORT A FOR LEDS)&&(PORT C FOR I2C AND LED OF TRIGGERING DOOR PASSWORD NOT SAME AS SAVED PASSWORD IN EEPROM
    DIO_u8SetPortDirection(DIO_u8PORTA, DIO_u8PORT_OUTPUT);
    DIO_u8SetPortDirection(DIO_u8PORTC, DIO_u8PORT_OUTPUT);
    DIO_u8SetPortDirection(DIO_u8PORTB, DIO_u8PORT_OUTPUT);

  //  DIO_u8SetPortValue(DIO_u8PORTA, DIO_u8PORT_LOW); // Turn OFF all LEDs initially

    // Led for triggering sending door password and matching saved password in second ECU
    DIO_u8SetPinDirection(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN_OUTPUT);

    GIE_voidEnable();


    while (1) {
        uartnum1_recieved = UART_Receive32BitNumber();
        UART_voidsend(1); // Send confirmation signal 1
        _delay_ms(1000);  // Wait for 1 second

        // Receive the second 32-bit number
        uartnum2_recieved = UART_Receive32BitNumber();

        // Compare the two numbers and send appropriate response
        if (uartnum1_recieved == uartnum2_recieved) {
            UART_voidsend(2); // Send response 2 if numbers are the same
            _delay_ms(1000);  // Wait for 1 second

            EEPROM_Write32BitNumber(0x0311, uartnum1_recieved);
            _delay_ms(100);
            eeprom_return = EEPROM_read32Byte(0x0311);

            if (eeprom_return == uartnum1_recieved) {
                //DIO_u8SetPortValue(DIO_u8PORTA, 0b10101010); // Example action for matching numbers
                saved_password = eeprom_return;
                menu_password_func();
            }
        } else {
            UART_voidsend(3); // Send response 3 if numbers are different
            _delay_ms(1000);  // Wait for 1 second
           // DIO_u8SetPortValue(DIO_u8PORTA, 0b00000001); // Example action for differing numbers
        }


        if(menu_function_ended==1){
        	PWM_TIMER0_Start(100);
        	TIMER1_voidInit();


        }
    //    if(menu_password_rcv!=eeprom_return){
//Changing_pass();
      //  }

    }
}

void I2C_config(void) {
    TWI_ConfigType TWI_Configuration = {0b00000010, 2};
    I2C_voidInit(&TWI_Configuration);
}

void UART_Config(void) {
    UART_ConfigType UART_configuration2 = {8, 0, 1, 9600};
    UART_init(&UART_configuration2);
}

// Function to receive a 32-bit number via UART
u32 UART_Receive32BitNumber(void) {
    u32 number = 0;
    u8 byte;

    // Receive the 32-bit number as 4 bytes
    byte = UART_voidreceive();  // Receive the most significant byte
    number |= ((u32)byte << 24);

    byte = UART_voidreceive();  // Receive the second byte
    number |= ((u32)byte << 16);

    byte = UART_voidreceive();  // Receive the third byte
    number |= ((u32)byte << 8);

    byte = UART_voidreceive();  // Receive the least significant byte
    number |= ((u32)byte);

    return number;
}

void EEPROM_Write32BitNumber(u16 startAddress, u32 number) {
    EEPROM_writeByte(startAddress, (number >> 24) & 0xFF); // Write MSB
    _delay_ms(10);
    EEPROM_writeByte(startAddress + 1, (number >> 16) & 0xFF); // Write second byte
    _delay_ms(10);
    EEPROM_writeByte(startAddress + 2, (number >> 8) & 0xFF); // Write third byte
    _delay_ms(10);
    EEPROM_writeByte(startAddress + 3, number & 0xFF); // Write LSB
    _delay_ms(10);
}

u32 EEPROM_read32Byte(u16 startAddress) {
    u8 byte1;
    u8 byte2;
    u8 byte3;
    u8 byte4;
    u32 returned_byte = 0;

    EEPROM_readByte(startAddress, &byte1);
    EEPROM_readByte(startAddress + 1, &byte2);
    EEPROM_readByte(startAddress + 2, &byte3);
    EEPROM_readByte(startAddress + 3, &byte4);

    returned_byte |= ((u32)byte4);
    returned_byte |= ((u32)byte3 << 8);
    returned_byte |= ((u32)byte2 << 16);
    returned_byte |= ((u32)byte1 << 24);

    return returned_byte;
}

void menu_password_func(void) {
	//u8 wait=0;
    menu_password_rcv = UART_Receive32BitNumber(); // Receive menu password
    _delay_ms(1000);

    if (menu_password_rcv == saved_password) {
        DIO_u8SetPinValue(DIO_u8PORTD, DIO_u8PIN2, 1); // Indicate correct password
        UART_voidsend(4);

        // Rotate the motor using PWM0
       // PWM_TIMER0_Start(100); // Start PWM with 100% duty cycle
        menu_function_ended=1;

        // Start Timer1 to stop the motor after 5 seconds
       // TIMER1_voidInit(); // Initialize Timer1 for 1-second intervals
    }
    else if(menu_password_rcv!=saved_password) {
        DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN2, 1); // Indicate incorrect password when opening the door
        UART_voidsend(5);
        _delay_ms(2000);

UART_Send32BitNumber(menu_password_rcv);
multiple_calling();
        	}



    }





// ISR for Timer1, to stop the motor after 5 seconds
void timer1_isr(void) {
    static u16 motor_counter = 0;
    motor_counter++;

    if (motor_counter == 40) {
        PWM_TIMER0_Start(0);
        motor_trigger=1;// Stop the PWM (effectively stopping the motor)


    }

    if (motor_counter == 60) { // After another 5 seconds



        PWM_TIMER0_Start(100); // Restart the PWM with 100% duty cycle

     motor_trigger=2;


    }
    if(motor_counter==100){
    	PWM_TIMER0_Start(0);
    	motor_trigger=1; //stop motor
    	end=1; //end of algorithm and motor ends its operation
    	motor_counter=0;
    }

}

void motor_isr (void){
	if(motor_trigger==0){
		UART_voidsend(6);
	 DCMOTOR_Rotate(CW);

	 DIO_u8SetPinValue(DIO_u8PORTC,DIO_u8PIN3,1);

	}
	else if(motor_trigger==1){

		 DCMOTOR_Rotate(Stop);
		 DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN4, 1); // Turn off the stop LED
		 if(end==1){
			 DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN4, 0);
			 DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN5, 0);
			 DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN3, 0);
			 Timer1_disable();
		 }
	}
	else if(motor_trigger==2){
         UART_voidsend(7);
		 DCMOTOR_Rotate(A_CW);
		 DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN5, 1); // Turn off the stop LED
		// motor_trigger=0;

	}
}
void multiple_calling(void){
	u32 opendoor_iter2=0;
	u8 sent=0;
	while(sent!=1){

opendoor_iter2=UART_Receive32BitNumber();


if(opendoor_iter2==saved_password){
	//UART_voidsend(1);
	DIO_u8SetPinValue(DIO_u8PORTA,DIO_u8PIN0,0);
	DIO_u8SetPinValue(DIO_u8PORTA,DIO_u8PIN1,1);
	sent=1;
	//DIO_u8SetPinValue(DIO_u8PORTA,DIO_u8PIN2,0);
	PWM_TIMER0_Start(100);
	        	TIMER1_voidInit();

}
if(opendoor_iter2!=saved_password) {
	//UART_voidsend(0);
	DIO_u8SetPinValue(DIO_u8PORTA,DIO_u8PIN0,1);
sent=0;
}

}
}
void UART_Send32BitNumber(u32 number) {
	// Send the 32-bit number as 4 bytes
	UART_voidsend((number >> 24) & 0xFF); // Send the most significant byte
	UART_voidsend((number >> 16) & 0xFF); // Send the second byte
	UART_voidsend((number >> 8) & 0xFF);  // Send the third byte
	UART_voidsend(number & 0xFF);         // Send the least significant byte
}

/*
void Changing_pass(void){
	u8 wait=0;
	wait=UART_voidreceive();
	_delay_ms(3000);
	if(wait==22){
		DIO_u8SetPinValue(DIO_u8PORTB,DIO_u8PIN7,1);
	}
if(eeprom_return==10000){

}

}
*/
