#include "GPIO_interface.h"
#include "STD_Types.h"
#include "BIT_Calc.h"
#include "Keypad_interface.h"
#include "util/delay.h"
#include "definition.h"
#include "LCD_interface.h"
#include"UART_interface.h"
#include "Timer1_interface.h"
#include "GIE_interface.h"
//Global Variables
u8 end_iteration2=0;
u8 start; //start of program
u8 door_recieved;
u32 opendoor_pass;
u8 menu_counter=0;
u8 firstnum_recieved;
u8 secondnum_recieved;
u8 counter=0;
u32 pass_changed;
u32 global_num2;
u8 index;
u8 recieved;
u32 num1=0 ;
u32 reconfirm_num1=0;
u8 fun1=0;
u8 opendoor_signal;
u8 closedoor_signal;
u8 no_display_whileopening=0;
u8 ISR_entry=0;
u8 opendoor_count=0; //this variable as counter for sequence of enterring a wrong pass when opening the door
u8 iteration2=0;//iteration2 of keypad typing if u enterred a wrong pass when opening the door
u32 iteration2num; //typing numner of second iteration
u32 iteration3num;
u32 eeprompass;
u8 password_changing=0;
u8 nodisplayinsecond=0;
u32 newpass;
//Functions
void Motor_response(void);
void keypad_typing1(void);
void reconfirm_keypad(void);
void LCD_timing(void);
void door_iteration2(void);
void dooriteration3(void);
void deactivation(void);
void sending_newpass(void);
void changing_password_second_time(void);
u32 UART_Receive32BitNumber(void);
void main(void) {
	UART_Config();
	TIMER1_u8SetCallBack(&LCD_timing);
	LCD_voidInit();
	KEYPAD_voidInit();
	DIO_u8SetPinDirection(DIO_u8PORTA,DIO_u8PIN4,DIO_u8PIN_OUTPUT);
	DIO_u8SetPinDirection(DIO_u8PORTA,DIO_u8PIN5,DIO_u8PIN_OUTPUT);
	DIO_u8SetPortDirection(DIO_u8PORTD,DIO_u8PORT_OUTPUT);
	DIO_u8SetPortDirection(DIO_u8PORTA,DIO_u8PORT_OUTPUT);
	GIE_voidEnable();
	while (1) {

		keypad1();

		_delay_ms(1000);
		LCD_voidClear();

		if (fun1 == 1) {
			reconfirm_keypad();

			if (num1 == reconfirm_num1) {
				UART_Send32BitNumber(num1);
				process_received_response();

				UART_Send32BitNumber(reconfirm_num1);

				process_received_response();

			} else {
				UART_Send32BitNumber(num1);
				process_received_response();

				UART_Send32BitNumber(reconfirm_num1);
				process_received_response();
			}
		}
		if((opendoor_count==1 )&& (password_changing==0)){
			DIO_u8SetPortValue(DIO_u8PORTD,DIO_u8PORT_OUTPUT);

			keypad1();
			LCD_voidClear();
			if(nodisplayinsecond==0){
				LCD_voidSetPos(0,0);
				LCD_voidSendString("iteration1 uncorrect");

				LCD_voidSetPos(1,0);
				LCD_voidSendNumber(num1); //you enter num1 that was uncorrect in first trial
				_delay_ms(5000);
				door_iteration2();
			}
		}

		if(password_changing==1){
			LCD_voidClear();
			LCD_voidSendString("second chance:");

			_delay_ms(5000);

			changing_password_second_time();
		}



	}
}

void UART_Config(void){
	UART_ConfigType UART_configuration={8,0,1,9600};

	UART_init(&UART_configuration);

}
void keypad1(void) {
	if(menu_counter==1 || menu_counter==2){
		LCD_voidClear();
		_delay_ms(2000);
	}
	static char keypad_char1, keypad_char2, keypad_char3, keypad_char4, keypad_char5;
	static char enter_keypad;
	//no_display_whileopening =1;

	while(no_display_whileopening==1);

	LCD_voidSetPos(0, 0); // Set cursor to the first line
	LCD_voidSendString("Enter the pass:");
	LCD_voidSetPos(1, 0); // Set cursor to the second line

	// Capture and display '*' for each character
	keypad_char1 = KEYPAD_charGetValue2();
	if (keypad_char1 != Unpressed) {
		LCD_voidSendString("*");
		_delay_ms(1000);
	}

	keypad_char2 = KEYPAD_charGetValue2();
	if (keypad_char2 != Unpressed) {
		LCD_voidSendString("*");
		_delay_ms(1000);
	}

	keypad_char3 = KEYPAD_charGetValue2();
	if (keypad_char3 != Unpressed) {
		LCD_voidSendString("*");
		_delay_ms(1000);
	}

	keypad_char4 = KEYPAD_charGetValue2();
	if (keypad_char4 != Unpressed) {
		LCD_voidSendString("*");
		_delay_ms(1000);
	}

	keypad_char5 = KEYPAD_charGetValue2();
	if (keypad_char5 != Unpressed) {
		LCD_voidSendString("*");
		_delay_ms(1000);
	}
	enter_keypad=KEYPAD_charGetValue2();
	if(enter_keypad!=Unpressed ){
		LCD_voidSendString(" ");
		_delay_ms(1000);
	}



	if((iteration2==0) && (password_changing==0)){
		// Construct the 5-digit number
		num1 = (keypad_char1 - '0') * 10000 +
				(keypad_char2 - '0') * 1000 +
				(keypad_char3 - '0') * 100 +
				(keypad_char4 - '0') * 10 +
				(keypad_char5 - '0');
	}
	if(iteration2==1){
		iteration2num = (keypad_char1 - '0') * 10000 +
				(keypad_char2 - '0') * 1000 +
				(keypad_char3 - '0') * 100 +
				(keypad_char4 - '0') * 10 +
				(keypad_char5 - '0');
	}
	if(iteration2==2){
		iteration3num = (keypad_char1 - '0') * 10000 +
				(keypad_char2 - '0') * 1000 +
				(keypad_char3 - '0') * 100 +
				(keypad_char4 - '0') * 10 +
				(keypad_char5 - '0');
	}
	if(password_changing==1){
		pass_changed = (keypad_char1 - '0') * 10000 +
				(keypad_char2 - '0') * 1000 +
				(keypad_char3 - '0') * 100 +
				(keypad_char4 - '0') * 10 +
				(keypad_char5 - '0');
	}

	if(opendoor_count==0){
		if((menu_counter==0) && (password_changing==0)){

			if((keypad_char1 != Unpressed)&&(keypad_char2 != Unpressed)&&(keypad_char3 != Unpressed)&&(keypad_char4 != Unpressed)&&(keypad_char5 != Unpressed))
			{ // Clear the display and show the number
				//	enter_keypad== KEYPAD_charGetValue2();
				//if((enter_keypad == '=') && (enter_keypad !=Unpressed)){
				LCD_voidClear();

				LCD_voidSetPos(0, 0);
				LCD_voidSendString("Number:");
				LCD_voidSetPos(1, 0);
				LCD_voidSendNumber(num1);

				_delay_ms(2000); // Adjust delay as needed
				fun1=1;
			}


		}

		else if(menu_counter==1){
			LCD_voidClear();
			LCD_voidSetPos(0, 0);
			LCD_voidSendString("DOOR PASS:");
			LCD_voidSetPos(1, 0);
			LCD_voidSendNumber(num1);
			opendoor_pass=num1;
			_delay_ms(2000); // Adjust delay as needed
			menu_counter=0;
		}
		else if(menu_counter==2){
			LCD_voidClear();
			LCD_voidSetPos(0, 0);
			LCD_voidSendString("Change Pass:");
			LCD_voidSetPos(1, 0);
			LCD_voidSendNumber(pass_changed);

			_delay_ms(2000); // Adjust delay as needed
			menu_counter=0;

		}
	}

}

void reconfirm_keypad(void){

	static char keypad_char1, keypad_char2, keypad_char3, keypad_char4, keypad_char5;

	//u32 num = 0; // Ensure proper initialization

	LCD_voidSetPos(0, 0); // Set cursor to the first line
	LCD_voidSendString("Re-Enter the pass:");
	LCD_voidSetPos(1, 0); // Set cursor to the second line

	// Capture and display '*' for each character
	keypad_char1 = KEYPAD_charGetValue2();
	if (keypad_char1 != Unpressed) {
		LCD_voidSendString("*");
		_delay_ms(1000);
	}

	keypad_char2 = KEYPAD_charGetValue2();
	if (keypad_char2 != Unpressed) {
		LCD_voidSendString("*");
		_delay_ms(1000);
	}

	keypad_char3 = KEYPAD_charGetValue2();
	if (keypad_char3 != Unpressed) {
		LCD_voidSendString("*");
		_delay_ms(1000);
	}

	keypad_char4 = KEYPAD_charGetValue2();
	if (keypad_char4 != Unpressed) {
		LCD_voidSendString("*");
		_delay_ms(1000);
	}

	keypad_char5 = KEYPAD_charGetValue2();
	if (keypad_char5 != Unpressed) {
		LCD_voidSendString("*");
		_delay_ms(1000);
	}

	// Construct the 5-digit number
	reconfirm_num1 = (keypad_char1 - '0') * 10000 +
			(keypad_char2 - '0') * 1000 +
			(keypad_char3 - '0') * 100 +
			(keypad_char4 - '0') * 10 +
			(keypad_char5 - '0');

	_delay_ms(1000); // Adjust delay as needed
	if((keypad_char1 != Unpressed)&&(keypad_char2 != Unpressed)&&(keypad_char3 != Unpressed)&&(keypad_char4 != Unpressed)&&(keypad_char5 != Unpressed))
	{ // Clear the display and show the number
		LCD_voidClear();
		LCD_voidSetPos(0, 0);
		LCD_voidSendString("Number reconfirm:");
		LCD_voidSetPos(1, 0);
		LCD_voidSendNumber(reconfirm_num1);
		_delay_ms(2000); // Adjust delay as needed

		fun1=0;

	}


}
void UART_Send32BitNumber(u32 number) {
	// Send the 32-bit number as 4 bytes
	UART_voidsend((number >> 24) & 0xFF); // Send the most significant byte
	UART_voidsend((number >> 16) & 0xFF); // Send the second byte
	UART_voidsend((number >> 8) & 0xFF);  // Send the third byte
	UART_voidsend(number & 0xFF);         // Send the least significant byte
}
void process_received_response(void) {
	u8 response = UART_voidreceive(); // Receive the response from ecu2

	if (response == 1) {
		// Handle response 1
		LCD_voidClear();
		LCD_voidSetPos(0, 0);
		LCD_voidSendString("Number1 sent");
		_delay_ms(2000);
	} else if (response == 2) {
		// Handle response 2
		LCD_voidClear();
		LCD_voidSetPos(1, 0);
		LCD_voidSendString("numbers same");
		_delay_ms(2000);
		eeprompass=reconfirm_num1;
		LCD_voidClear();
		LCD_voidSetPos(0, 0);
		LCD_voidSendString("SAVED PASS");
		LCD_voidSetPos(1, 0);

		LCD_voidSendNumber(eeprompass);
		_delay_ms(7000);
		menu();



	} else if (response == 3) {
		// Handle response 3
		LCD_voidClear();
		LCD_voidSetPos(1, 0);
		LCD_voidSendString("Numbers are different");
		_delay_ms(2000);
	}
	else if(response==4){
		LCD_voidClear();
		LCD_voidSetPos(1, 0);
		LCD_voidSendString("4 sent");
		//_delay_ms(2000);
		opendoor_signal=UART_voidreceive();
		if(opendoor_signal==6){
			no_display_whileopening=1;

			TIMER1_voidInit();

		}



	}
	else if(response==5){ //you enterred a wrong password when opening the door
		//this func deal only when enterring a wrong pass when choosing menu option:open door
		LCD_voidClear();
		LCD_voidSetPos(0, 0);
		LCD_voidSendString("5 sent");
		LCD_voidSetPos(1, 0);
		LCD_voidSendString("repeat");
		_delay_ms(1000);
		if(password_changing==1){
			//UART_voidsend(22);
			DIO_u8SetPinValue(DIO_u8PORTA,DIO_u8PIN7,1);
			//_delay_ms(2000);
			newpass=UART_Receive32BitNumber();
			LCD_voidClear();
			LCD_voidSetPos(0,0);
			LCD_voidSendString("EEPROM");
			LCD_voidSetPos(1,0);
			LCD_voidSendNumber(newpass);
			_delay_ms(4000);
			//password_changing=1;
		}
		opendoor_count=1;
		iteration2=1;

	}


}
void menu (void){
	static char menu_keypad;
	LCD_voidClear();
	_delay_ms(1000);
	LCD_voidSetPos(0, 0);
	LCD_voidSendString("Welcome to menu");
	LCD_voidClear();
	_delay_ms(3000);
	LCD_voidSetPos(0, 0);
	LCD_voidSendString("+ : Open Door");
	LCD_voidSetPos(1, 0);
	LCD_voidSendString("- : Change Pass");
	_delay_ms(2000);
	menu_keypad=KEYPAD_charGetValue2();

	if((menu_keypad=='+')&&(menu_keypad!=Unpressed)){
		//A led to indicate to enter pass to open the door
		//DIO_u8SetPinValue(DIO_u8PORTA,DIO_u8PIN4,DIO_u8PIN_HIGH);
		menu_counter=1;
		LCD_voidClear();
		_delay_ms(2000);

		keypad1();
		LCD_voidClear();
		_delay_ms(1000);
		LCD_voidSendNumber(num1);
		UART_Send32BitNumber(num1);
		_delay_ms(1000);

		process_received_response();
	}
	else if((menu_keypad=='-')&& (menu_keypad!=Unpressed)){
		menu_counter=2;
		LCD_voidClear();
		_delay_ms(2000);
		password_changing=1;
		keypad1();

		LCD_voidClear();
		_delay_ms(1000);
		LCD_voidSetPos(0,0);
		LCD_voidSendString("OUTSIDE FUNC");
		LCD_voidSetPos(1,0);
		LCD_voidSendNumber(pass_changed);
		_delay_ms(2000);

		UART_Send32BitNumber(pass_changed);

		if(eeprompass==pass_changed){    //FIRST TIME
			LCD_voidClear();
			_delay_ms(1000);
			LCD_voidSetPos(0,0);
			LCD_voidSendString("SAME PASS"); //you must change it retuen to while(1) in main

			password_changing=0;
		}
		else { // iteration1
			DIO_u8SetPinValue(DIO_u8PORTA,DIO_u8PIN5,1);
			password_changing=1;
			process_received_response();

		}
	}
}


//YOU WILL ENTER THIS FUNCTION IF U OPEN DOOR AND IN RIGHT PATH
void LCD_timing(void) {
	static LCD_opencounter = 0;
	LCD_opencounter++;

	if (ISR_entry==0) {  // Only update if not entering a passcode
		LCD_voidClear();
		LCD_voidSetPos(0, 0);
		LCD_voidSendString("DOOR OPENING");
	}

	if(ISR_entry==1){
		LCD_voidClear();
		//LCD_voidSetPos(0, 0);
		LCD_voidSendString("");

	}


	if ((LCD_opencounter == 35)&&(ISR_entry==0)) {
		DIO_u8SetPinValue(DIO_u8PORTA, DIO_u8PIN3, 1);
		//LCD_opencounter = 0;
		ISR_entry=1;

	}
	if((LCD_opencounter==60) && (ISR_entry==1)){
		LCD_voidClear();
		LCD_voidSetPos(0, 0);
		LCD_voidSendString("DOOR CLOSING");
		ISR_entry=2;
	}

	if(ISR_entry==2){
		LCD_voidSetPos(0, 0);
		LCD_voidSendString("DOOR CLOSING");
		if(LCD_opencounter==100){
			LCD_voidClear();
			LCD_voidSetPos(0, 0);
			LCD_voidSendString("THANK YOU");
			while(1);
			//Timer1_disable();
			//no_display_whileopening=0;

			//menu();
		}
	}
}
void  door_iteration2(void){
	u8 x=2;
	u8 same=0;
	u8 diff=0;
	//here trial2
	_delay_ms(1000);
	LCD_voidClear();
	_delay_ms(1000);
	keypad1();
	_delay_ms(1000);
	LCD_voidClear();
	LCD_voidSetPos(0,0);
	LCD_voidSendString("iteration2 uncorrect");
	//UART_voidsend(1);
	LCD_voidSetPos(1,0);
	LCD_voidSendNumber(iteration2num);



	UART_Send32BitNumber(iteration2num);
	if(eeprompass==iteration2num){
		LCD_voidClear();
		nodisplayinsecond=1;
		TIMER1_voidInit();
		no_display_whileopening=1;
	}

	else if(eeprompass!=iteration2num){
		LCD_voidClear();
		LCD_voidSetPos(0,0);
		LCD_voidSendString("Item1 Wrong");
		LCD_voidSetPos(1,0);
		LCD_voidSendString("Item2 Wrong");
		_delay_ms(3000);
		dooriteration3();
	}

}
void dooriteration3(void){


	iteration2=2;
	LCD_voidClear();
	LCD_voidSetPos(0,0);
	LCD_voidSendString("last chance:");
	_delay_ms(3000);
	keypad1();
	UART_Send32BitNumber(iteration3num);
	if(eeprompass==iteration3num){
		LCD_voidClear();
		nodisplayinsecond=1;
		TIMER1_voidInit();
		no_display_whileopening=1;
	}

	else if(eeprompass!=iteration3num){
		deactivation();

	}

}
void deactivation(void){
	DIO_u8SetPortValue(DIO_u8PORTC,0);
	LCD_voidClear();
	LCD_voidSetPos(0,0);
	LCD_voidSendString("ERROR");
	_delay_ms(5000);
	_delay_ms(5000);
	_delay_ms(5000);
	_delay_ms(5000);
	//Unlock keypad
	menu();
}

void changing_password_second_time(void){ //Here you have second chance to change pass

	keypad1();

	if(nodisplayinsecond==0){
		LCD_voidClear();
		LCD_voidSetPos(1,0);
		LCD_voidSendNumber(pass_changed); //you enter num1 that was uncorrect in first trial
		_delay_ms(5000);
		if(eeprompass==pass_changed){
			DIO_u8SetPinValue(DIO_u8PORTA,DIO_u8PIN4,1);
			LCD_voidClear();
				LCD_voidSetPos(0,0);
			LCD_voidSendString("changed at ");
			LCD_voidSetPos(1,0);
						LCD_voidSendString("chance2");
			while(1);
		}
		else if(eeprompass!=pass_changed){
			DIO_u8SetPinValue(DIO_u8PORTA,DIO_u8PIN7,1);
			LCD_voidClear();
							LCD_voidSetPos(0,0);
						LCD_voidSendString("you've chance3");
						_delay_ms(3000);
			changing_password_third_time();
		}

	}

}
void changing_password_third_time(void){

	keypad1();

	if(nodisplayinsecond==0){
		LCD_voidClear();
		LCD_voidSetPos(1,0);
		LCD_voidSendNumber(pass_changed); //you enter num1 that was uncorrect in first trial
		_delay_ms(5000);
		if(eeprompass==pass_changed){
			DIO_u8SetPinValue(DIO_u8PORTA,DIO_u8PIN3,1);
			LCD_voidClear();
			LCD_voidSetPos(0,0);

			LCD_voidSendString("changed at ");
						LCD_voidSetPos(1,0);
									LCD_voidSendString("chance3");
			while(1);
		}
		else if(eeprompass!=pass_changed){
			//LCD_voidClear();
				//						LCD_voidSetPos(0,0);
					//	LCD_voidSendString("changed at chance3");
			DIO_u8SetPinValue(DIO_u8PORTA,DIO_u8PIN7,0);
		deactivation();
		}

	}
}

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
