/**
* @mainpage ZumoBot Project
* @brief    You can make your own ZumoBot with various sensors.
* @details  <br><br>
    <p>
    <B>General</B><br>
    You will use Pololu Zumo Shields for your robot project with CY8CKIT-059(PSoC 5LP) from Cypress semiconductor.This 
    library has basic methods of various sensors and communications so that you can make what you want with them. <br> 
    <br><br>
    </p>
    
    <p>
    <B>Sensors</B><br>
    &nbsp;Included: <br>
        &nbsp;&nbsp;&nbsp;&nbsp;LSM303D: Accelerometer & Magnetometer<br>
        &nbsp;&nbsp;&nbsp;&nbsp;L3GD20H: Gyroscope<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Reflectance sensor<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Motors
    &nbsp;Wii nunchuck<br>
    &nbsp;TSOP-2236: IR Receiver<br>
    &nbsp;HC-SR04: Ultrasonic sensor<br>
    &nbsp;APDS-9301: Ambient light sensor<br>
    &nbsp;IR LED <br><br><br>
    </p>
    
    <p>
    <B>Communication</B><br>
    I2C, UART, Serial<br>
    </p>
*/

#include <project.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "LSM303D.h"
#include "IR.h"
#include "Beep.h"
#include "mqtt_sender.h"
#include <time.h>
#include <sys/time.h>
#include "serial1.h"
#include <unistd.h>
#include <stdlib.h>
#define PI 3.141592654
/**
 * @file    main.c
 * @brief   
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/

#if 0
// Hello World!
void zmain(void)
{
    printf("\nHello, World!\n");

    while(true)
    {
        vTaskDelay(100); // sleep (in an infinite loop)
    }
 }   
#endif

#if 0
// Name and age
void zmain(void)
{
    char name[32];
    int age;
    
    
    printf("\n\n");
    
    printf("Enter your name: ");
    //fflush(stdout);
    scanf("%s", name);
    printf("Enter your age: ");
    //fflush(stdout);
    scanf("%d", &age);
    
    printf("You are [%s], age = %d\n", name, age);

    while(true)
    {
        BatteryLed_Write(!SW1_Read());
        vTaskDelay(100);
    }
 }   
#endif


#if 0
//battery level//
void zmain(void)
{
    ADC_Battery_Start();        

    int16 adcresult =0;
    float volts = 0.0;

    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    //BatteryLed_Write(0); // Switch led off 
    uint8 button;
    
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed

    while(true)
    {
        //char msg[80];
        ADC_Battery_StartConvert(); // start sampling
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for ADC converted value
            adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
            // convert value to Volts
            // you need to implement the conversion
            float afterresistancevolts = adcresult *5.0 /4095;
            volts = afterresistancevolts * 3/2;
            
            if(volts < 4.0){
                
                // blink leds
                button = SW1_Read();
                while(button==1){
                    BatteryLed_Write(1);
                    printf("ON\n");
                    vTaskDelay(75);
                    BatteryLed_Write(0);
                    printf("OFF\n");
                    vTaskDelay(75);
                    button = SW1_Read();
                }
                vTaskDelay(100);
                printf("Button is being pressed\n");
                printf("%d %f\r\n",adcresult, volts);
                continue;

            }else{
                // sleep for a while and (re-read ADC)
                vTaskDelay(1000);
            }
            
            // Print both ADC results and converted value
            printf("%d %f\r\n",adcresult, volts);
        }
        //vTaskDelay(500);
    }
 }   
#endif

#if 0 

//Tick Timer Example
void zmain(void) 
{
	TickType_t Ttime = xTaskGetTickCount();
	TickType_t PreviousTtime = 0;

	while(true) 
	{
		while(SW1_Read()) vTaskDelay(1); // loop until user presses button
		Ttime = xTaskGetTickCount(); // take button press time
		/*Print out the time between button presses in seconds. int cast used to suppress warning messages*/
		printf("The amount of time between button presses is: %d.%d seconds\n", (int)(Ttime-PreviousTtime)/1000%60, (int)(Ttime-PreviousTtime)%1000);
		while(!SW1_Read())vTaskDelay(1); // loop while user is pressing the button
		PreviousTtime = Ttime; // remember previous press time
	}
	
}

#endif

#if 0
// button
void zmain(void)
{
    while(true) {
        printf("Press button within 5 seconds!\n");
	    TickType_t Ttime = xTaskGetTickCount(); // take start time
        bool timeout = false;
        while(SW1_Read() == 1) {
            if(xTaskGetTickCount() - Ttime > 5000U) { // too long time since start
                timeout = true;
                break;
            }
            vTaskDelay(10);
        }
        if(timeout) {
            printf("You didn't press the button\n");
        }
        else {
            printf("Good work\n");
            while(SW1_Read() == 0) vTaskDelay(10); // wait until button is released
        }
    }
}
#endif

#if 0
// button
void zmain(void)
{
    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed
    
    bool led = false;
    
    while(true)
    {
        // toggle led state when button is pressed
        if(SW1_Read() == 0) {
            led = !led;
            BatteryLed_Write(led);
            if(led) printf("Led is ON\n");
            else printf("Led is OFF\n");
            Beep(1000, 150);
            while(SW1_Read() == 0) vTaskDelay(10); // wait while button is being pressed
        }        
    }
 }   
#endif


#if 0
//ultrasonic sensor//
    
    //assignment 2 reversing and turing left when obstcale is less than 10com
void zmain(void)
{
    Ultra_Start();                          // Ultra Sonic Start function
    printf("\n\nBoot\n");
    while(SW1_Read() == 1) vTaskDelay(10);
    while(true) {
        int d = Ultra_GetDistance();
        // Print the detected distance (centimeters)
        printf("distance = %d\r\n", d);
        if(d < 10) {
            motor_backward(100,100);
            motor_start(0,0);
            vTaskDelay(3000);
            motor_turn(30,120,50); 
            vTaskDelay(1000);
            motor_start(0,0);   
        }
        printf("Go forward!\n");
        motor_start();
        motor_forward(100,100);
        //vTaskDelay(1000);
    }
}  
#endif

#if 0
  //reversing and making random turns between 90 to 270 degrees when the obstacle is less than 10cm
void zmain(void)
{
    Ultra_Start();                          // Ultra Sonic Start function
    printf("\n\nBoot\n");
    while(SW1_Read() == 1) vTaskDelay(10);
    int seed = xTaskGetTickCount();
    srand(seed);
    while(true) {
        int d = Ultra_GetDistance();
        int r1 = rand() % 180;
        //int r2 = rand() % 180;
        
        // Print the detected distance (centimeters)
        printf("distance = %d, rand1 = %d\r\n", d, r1);
        if(d < 10) {
            motor_backward(100,5);
            motor_start(0,0);
            vTaskDelay(4000);
            motor_turn(r1,r1+90,50); 
            vTaskDelay(1000);
            motor_start(0,0);   
        }
        printf("Go forward!\n");
        motor_start();
        motor_forward(80,100);
        //vTaskDelay(1000);
    }
}
#endif

#if 0
//IR receiverm - how to wait for IR remote commands
void zmain(void)
{
    IR_Start();
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    bool led = false;
    // Toggle led when IR signal is received
    while(true)
    {
        IR_wait();  // wait for IR command
        led = !led;
        BatteryLed_Write(led);
        if(led) printf("Led is ON\n");
        else printf("Led is OFF\n");
    }    
 }   
#endif



#if 0
//IR receiver - read raw data
// RAW data is used when you know how your remote modulates data and you want to be able detect 
// which button was actually pressed. Typical remote control protocols requires a protocol specific
// state machine to decode button presses. Writing such a state machine is not trivial and requires
// that you have the specification of your remotes modulation and communication protocol    
void zmain(void)
{
    IR_Start();
    
    uint32_t IR_val; 
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    // print received IR pulses and their lengths
    while(true)
    {
        if(IR_get(&IR_val, portMAX_DELAY)) {
            int l = IR_val & IR_SIGNAL_MASK; // get pulse length
            int b = 0;
            if((IR_val & IR_SIGNAL_HIGH) != 0) b = 1; // get pulse state (0/1)
            printf("%d %d\r\n",b, l);
        }
    }    
 }   
#endif

// Week 4 assignment 1
#if 0
//reflectance
    
void zmain(void)
{
    //struct sensors_ ref;
    struct sensors_ dig;
    IR_Start();
    
    int delay=5;
    int speed=50;
    int turnFactor = 4;
    int sharpTurnFactor = 5;
    
    int maxcons = 20;
    int count = 0;
    int a = -1;
    int b = 0;
    int counter = 0;
    int inersectionCounter=0;
    
    
    motor_start();
    motor_forward(0,0);
    
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    
    // waiting for user to press the switch button and lift from the button
    while(SW1_Read() == 1) vTaskDelay(10);
    while(SW1_Read() == 0) vTaskDelay(10);
    
    // check if the vehicle is centered.
    do{
        reflectance_digital(&dig);
    } 
    while(dig.L1==0 || dig.R1==0);

    
    while(true)
    {
        /*
        // read raw sensor values
        reflectance_read(&ref);
        // print out each period of reflectance sensors
        printf("%5d %5d %5d %5d %5d %5d\r\n", ref.L3, ref.L2, ref.L1, ref.R1, ref.R2, ref.R3);       
        */

        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig); 
        //print out 0 or 1 according to results of reflectance period
        //printf("%5d %5d %5d %5d %5d %5d \r\n", dig.L3, dig.L2, dig.L1, dig.R1, dig.R2, dig.R3);        
        

        if(dig.L3==0 && dig.R3==0){
            if(count > 0){
                count -= 1;
            } else{
                a = b+1;
                counter = a;
            }
        } else if (dig.L3==1 && dig.R3==1){
            if (count <= maxcons){
                count += 1;
            }else{
                b = a+1;
                counter = b;
            }
        }
        
        

        
        if(counter==2){
            count = 0;
            a = -1;
            b = 0;
            counter = 0;
            inersectionCounter++;
            if (inersectionCounter==1||inersectionCounter==4){
                motor_forward(0,0);
                IR_wait();  // wait for IR command
            }

            
        }
        
        if(dig.L1==1 && dig.R1==1){
            motor_forward(speed,delay);
            
        } else if(dig.L2==1 && dig.L1==1){
            motor_turn(speed/turnFactor,turnFactor*speed,delay);
            
        } else if(dig.R1==1 && dig.R2==1){
            motor_turn(turnFactor*speed,speed/turnFactor,delay);
            
        } else if(dig.L3==1 && dig.L2==1){
            motor_turn(speed/sharpTurnFactor,sharpTurnFactor*speed,delay);
            
        }else if(dig.R2==1 && dig.R3==1){
            motor_turn(sharpTurnFactor*speed,speed/sharpTurnFactor,delay);
            
        }

        
        
        
        
        //vTaskDelay(10);

    }
}



#endif


// Week 4 assignment 2
#if 0
//reflectance
    
void zmain(void)
{
    //struct sensors_ ref;
    struct sensors_ dig;
    IR_Start();
    
    int delay=5;
    int speed=50;
    int turnFactor = 4;
    int sharpTurnFactor = 5;
    
    int maxcons = 20;
    int count = 0;
    int a = -1;
    int b = 0;
    int counter = 0;
    
    
    motor_start();
    motor_forward(0,0);
    
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    
    // waiting for user to press the switch button and lift from the button
    while(SW1_Read() == 1) vTaskDelay(10);
    while(SW1_Read() == 0) vTaskDelay(10);
    
    // check if the vehicle is centered.
    do{
        reflectance_digital(&dig);
    } 
    while(dig.L1==0 || dig.R1==0);

    
    while(true)
    {
        /*
        // read raw sensor values
        reflectance_read(&ref);
        // print out each period of reflectance sensors
        printf("%5d %5d %5d %5d %5d %5d\r\n", ref.L3, ref.L2, ref.L1, ref.R1, ref.R2, ref.R3);       
        */

        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig); 
        //print out 0 or 1 according to results of reflectance period
        //printf("%5d %5d %5d %5d %5d %5d \r\n", dig.L3, dig.L2, dig.L1, dig.R1, dig.R2, dig.R3);        
        

        if(dig.L3==0 && dig.R3==0){
            if(count > 0){
                count -= 1;
            } else{
                a = b+1;
                counter = a;
            }
        } else if (dig.L3==1 && dig.R3==1){
            if (count <= maxcons){
                count += 1;
            }else{
                b = a+1;
                counter = b;
            }
        }
        
        

        
        if(counter==2){
            motor_forward(0,0);
            count = 0;
            a = -1;
            b = 0;
            counter = 0;
            IR_wait();  // wait for IR command
            

        } else if(dig.L1==1 && dig.R1==1){
            motor_forward(speed,delay);
            
        } else if(dig.L2==1 && dig.L1==1){
            motor_turn(speed/turnFactor,turnFactor*speed,delay);
            
        } else if(dig.R1==1 && dig.R2==1){
            motor_turn(turnFactor*speed,speed/turnFactor,delay);
            
        } else if(dig.L3==1 && dig.L2==1){
            motor_turn(speed/sharpTurnFactor,sharpTurnFactor*speed,delay);
            
        }else if(dig.R2==1 && dig.R3==1){
            motor_turn(sharpTurnFactor*speed,speed/sharpTurnFactor,delay);
            
        }

        
        
        
        
        //vTaskDelay(10);

    }
}



#endif


// Week 4 assignment 3
#if 0
//reflectance
    void tank_left(uint8 l_speed, uint8 r_speed, uint32 delay);
    
    void tank_right(uint8 l_speed, uint8 r_speed, uint32 delay);
    
void zmain(void)
{
    //struct sensors_ ref;
    struct sensors_ dig;
    IR_Start();
    

    
    int delay=5;
    int speed=50;
    int turnFactor = 2;
    int sharpTurnFactor = 3;
    
    int maxcons = 20;
    int count = 0;
    int a = -1;
    int b = 0;
    int counter = 0;
    int inersectionCounter=0;
    
    
    motor_start();
    motor_forward(0,0);
    
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    
    // waiting for user to press the switch button and lift from the button
    while(SW1_Read() == 1) vTaskDelay(10);
    while(SW1_Read() == 0) vTaskDelay(10);
    
    // check if the vehicle is centered.
    do{
        reflectance_digital(&dig);
    } 
    while(dig.L1==0 || dig.R1==0);

    
    while(true)
    {
        /*
        // read raw sensor values
        reflectance_read(&ref);
        // print out each period of reflectance sensors
        printf("%5d %5d %5d %5d %5d %5d\r\n", ref.L3, ref.L2, ref.L1, ref.R1, ref.R2, ref.R3);       
        */

        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig); 
        //print out 0 or 1 according to results of reflectance period
        //printf("%5d %5d %5d %5d %5d %5d \r\n", dig.L3, dig.L2, dig.L1, dig.R1, dig.R2, dig.R3);        
        

        if(dig.L3==0 && dig.R3==0){
            if(count > 0){
                count -= 1;
            } else{
                a = b+1;
                counter = a;
            }
        } else if (dig.L3==1 && dig.R3==1){
            if (count <= maxcons){
                count += 1;
            }else{
                b = a+1;
                counter = b;
            }
        }
        
        

        
        if(counter==2){
            count = 0;
            a = -1;
            b = 0;
            counter = 0;
            inersectionCounter++;
            if (inersectionCounter==1){
                motor_forward(0,0);
                IR_wait();  // wait for IR command
            
            }else if (inersectionCounter==2){
                // Turn Left
                // make a function to turn sharp left
                motor_forward(0,0);
                //Tank turn
                
                //SetMotors(1,0, speed, speed, delay*100);
                tank_left(100, 100, 400);
                
                //vTaskDelay(5000);
                
                
            }else if (inersectionCounter==3||inersectionCounter==4){
                // Turn Right
                // make a function to turn sharp right
                motor_forward(0,0);
                //Tank turn
                
                tank_right(100, 100, 400);
                
            } else{
                motor_forward(0,0);                
                motor_stop();
            }

        }
        
        if(dig.L1==1 && dig.R1==1){
            motor_forward(speed,delay);
            
        } else if(dig.L2==1 && dig.L1==1){
            motor_turn(speed/turnFactor,turnFactor*speed,delay);
            
        } else if(dig.R1==1 && dig.R2==1){
            motor_turn(turnFactor*speed,speed/turnFactor,delay);
            
        } else if(dig.L3==1 && dig.L2==1){
            motor_turn(speed/sharpTurnFactor,sharpTurnFactor*speed,delay);
            
        }else if(dig.R2==1 && dig.R3==1){
            motor_turn(sharpTurnFactor*speed,speed/sharpTurnFactor,delay);
            
        }

        
        
        
        
        //vTaskDelay(10);

    }
}

void tank_left(uint8 l_speed, uint8 r_speed, uint32 delay)
{
    SetMotors(1,0, l_speed, r_speed, delay);
}

void tank_right(uint8 l_speed, uint8 r_speed, uint32 delay)
{
    SetMotors(0,1, l_speed, r_speed, delay);
}


#endif

#if 0
//Week 5 Assignment 1
// MQTT test
void zmain(void)
{
    printf("\nBoot\n");
    send_mqtt("Zumo06/debug", "Boot");

    BatteryLed_Write(1); // Switch led on 
    while(SW1_Read()==1);
    BatteryLed_Write(0); // Switch led off 
    vTaskDelay(1000);
    int t1 = xTaskGetTickCount();
    int t0 = 0;
    int interval;

    while(true)
    {
        while(SW1_Read() == 1);
        vTaskDelay(100);
        t1 = xTaskGetTickCount();
        interval = t1-t0;
        print_mqtt("Zumo06\t", "Time since last button push : %d milliseconds", interval);
        while(SW1_Read() == 0);
        vTaskDelay(100);
        t0 = t1;        
    }

 }   
#endif

#if 0
    // Week 5 Assignment 2
    void zmain(void)
    {
    Ultra_Start();                          // Ultra Sonic Start function
    send_mqtt("Zumo039/debug", "Boot");
    
    printf("\n\nBoot\n");
    
    while(SW1_Read() == 1) vTaskDelay(10);
    int seed = xTaskGetTickCount();
    srand(seed);
    int ctr = 0;
    
    while(true) {
        int d = Ultra_GetDistance();
        // Print the detected distance (centimeters)
        //printf("distance = %d, rand1 = %d\r\n", d, r1);
        if(d < 10) {
            motor_backward(100,5);
            vTaskDelay(1000);
            motor_forward(0,0);
            
            int r = rand() % 2;
            
            
            if(r == 0){
                motor_turn(10,250,300);
                motor_forward(0,0);
                motor_forward(80,100);
                printf("Ctr: %d, Button: %d\n", ctr, SW1_Read());
                print_mqtt("Zumo039/debug", "Ctr: %d, Button: %d, TURN = LEFT", ctr, SW1_Read());
   
            }
            else
            {
                motor_turn(250,10,300);
                motor_forward(0,0);
                motor_forward(80,100);
                printf("Ctr: %d, Button: %d\n", ctr, SW1_Read());
                print_mqtt("Zumo039/debug", "Ctr: %d, Button: %d, TURN = RIGHT", ctr, SW1_Read());
            }
           
            
            vTaskDelay(1000);
            ctr++;
            vTaskDelay(1000);
            motor_forward(0,0);
            
            
        }
        printf("Go forward!\n");
        motor_start();
        motor_forward(80,100);
        //vTaskDelay(1000);
    }
}   
#endif


#if 0
    
// Week 5 Assignment 3
void zmain(void)
{
    int count = 0;
    struct sensors_ dig;
    //int inital = 0;
    TickType_t start = 0, end = 0;
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000);
    
    BatteryLed_Write(1); // Switch led on 
    while(SW1_Read()==1);
    BatteryLed_Write(0); // Switch led off 
    vTaskDelay(1000);
   
    IR_Start();
    IR_flush();
    motor_start();
    motor_forward(0,0);
     
    while(true)
    {  
    reflectance_digital(&dig);
        if ((dig.R1 && dig.L1))  // if zumo is in the center 
        {
            motor_forward(50, 10);
        }
        if (dig.R3 && dig.R2 && dig.R1 && dig.L1 && dig.L2 && dig.L3) //when robot reacher the line
        { 
            count++;
            if (count == 1)
            {  
                motor_forward(0,0);
                IR_wait(); 
                start = xTaskGetTickCount();
                //print_mqtt("Zumo06/start", "%d ms", start);
                //inital = start;         
            }   
            if(count == 2) 
            { 
                motor_forward(0,0);  
                end = xTaskGetTickCount();
                //print_mqtt("Zumo06/end", "%d ms", end);
                int interval = end - start;
                print_mqtt("Zumo06/lap", "The time interval is %d ms", interval); 
                break;      
            }

            while(dig.R3 && dig.L3) { 
                reflectance_digital(&dig);
                motor_forward(50, 10);
            }
             
        }          
    }
    while(true) 
    { 
        vTaskDelay(100);
    }

    
}
    
    
#endif

#if 0
    
    //LINE FOLLOWER Bibek's master code
    
void zmain(void){
    
    
    send_mqtt("Zumo06/debug", "Boot");
    
    struct sensors_ dig;
    //struct sensors_ ref;
    reflectance_set_threshold(15000, 14000, 13500, 13500, 14000, 15000);
    
    reflectance_start();
    IR_Start();
    motor_start();
    motor_forward(0,0);
    
    BatteryLed_Write(1);
    while (SW1_Read()==1);
    BatteryLed_Write(0);
    //vTaskDelay(1000);
    
    int intersection = 0;
    int count = 0;
    
    int start = 0, end = 0;
    
    
    while(true){
    reflectance_digital(&dig);
    
    // to do once first intersection
    if (dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && intersection == 0) {
    intersection = 1;
    count++;
    print_mqtt("Zumo06/","ready line");
    motor_forward(0,0);
    IR_wait();
    start = xTaskGetTickCount();
    motor_forward(130,100);
    }
    
    // to do after second intersection
    else if (dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && intersection == 1){
        intersection = 2;
        count ++;
        motor_forward(130,0);
        //print_mqtt("Zumo06/","1st line");
        //vTaskDelay(2000);
    }
    
    // 
    else if(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && intersection == 2){
        intersection = 3;
        count++;
        motor_forward(180,0);
        //print_mqtt("Zumo06/","2rd line stop and count is %d and intersection no is %d", count, intersection);
        //vTaskDelay(500);
    }
    
    // 
    else if(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && intersection == 3){
        motor_forward(0,0);
        end = xTaskGetTickCount();
        //print_mqtt("Zumo06/","3rd line stop and count is %d and intersection no is %d", count, intersection);
        //vTaskDelay(1000);
        break;
    }
    
    
    
    // if centered go straight
    if (dig.L3 == 0 && dig.L2 == 0 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 0 && dig.R3 == 0) {
            motor_forward(130,0);    
    }
    
    // Turn right if L1 and L2 sensor are 0
    else if ( dig.L1 == 0 && dig.L2 == 0 ) {
        motor_turn(200, 0, 0);
    }
    
    // Turn left if R1 and R2 sensor are 0
    else if (dig.R1 == 0 && dig.R2 == 0 ){
        motor_turn(0, 200, 0);
    }
    
    // Turn hard Right if R2 and/or R3 sensor is 1
    else if ((dig.R2 == 1 && dig.R3 == 1)||(dig.R3 == 1)) {
        motor_turn(255,0,0);
    }
    
    // Turn hard left if L2 and/or R3 sensor is 1
    else if ((dig.L2 == 1 && dig.L3 == 1)||(dig.L3 == 1)) {
        motor_turn(0,255,0);
    }
    
    
    

    }
    // end of while loop
    
    motor_stop();
    int timeTaken = end - start;
    print_mqtt("Zumo06/", "The time taken to finish the track was %d ms", timeTaken);

}
    
   
    
#endif





#if 0
//motor
void zmain(void)
{
    motor_start();              // enable motor controller
    motor_forward(0,0);         // set speed to zero to stop motors

    vTaskDelay(3000);
    
    motor_forward(100,3150);     // moving forward
    motor_turn(200,50,575);     // turn
    //motor_turn(255,0,250);
    motor_forward(100,2375);
    motor_turn(200,50,560);     // turn
    //motor_turn(255,0,250);
    motor_forward(100,2500);
    motor_turn(200,0,400);
    motor_turn(150,100,1400);     // turn
    motor_turn(200,100,500);     // turn
    motor_forward(100,1100);
    
    //motor_backward(100,2000);    // moving backward
     
    motor_forward(0,0);         // stop motors

    motor_stop();               // disable motor controller
    
    while(true)
    {
        vTaskDelay(100);
    }
}
#endif

#if 0
/* Example of how to use te Accelerometer!!!*/
void zmain(void)
{
    struct accData_ data;
    
    printf("Accelerometer test...\n");

    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Device Ok...\n");
    }
    
    while(true)
    {
        LSM303D_Read_Acc(&data);
        printf("%8d %8d %8d\n",data.accX, data.accY, data.accZ);
        vTaskDelay(50);
    }
 }   
#endif    

#if 0
// MQTT test
void zmain(void)
{
    int ctr = 0;

    printf("\nBoot\n");
    send_mqtt("Zumo01/debug", "Boot");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 

    while(true)
    {
        printf("Ctr: %d, Button: %d\n", ctr, SW1_Read());
        print_mqtt("Zumo01/debug", "Ctr: %d, Button: %d", ctr, SW1_Read());

        vTaskDelay(1000);
        ctr++;
    }
 }   
#endif


#if 0
void zmain(void)
{    
    struct accData_ data;
    struct sensors_ ref;
    struct sensors_ dig;
    
    printf("MQTT and sensor test...\n");

    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Accelerometer Ok...\n");
    }
    
    int ctr = 0;
    reflectance_start();
    while(true)
    {
        LSM303D_Read_Acc(&data);
        // send data when we detect a hit and at 10 second intervals
        if(data.accX > 1500 || ++ctr > 1000) {
            printf("Acc: %8d %8d %8d\n",data.accX, data.accY, data.accZ);
            print_mqtt("Zumo01/acc", "%d,%d,%d", data.accX, data.accY, data.accZ);
            reflectance_read(&ref);
            printf("Ref: %8d %8d %8d %8d %8d %8d\n", ref.L3, ref.L2, ref.L1, ref.R1, ref.R2, ref.R3);       
            print_mqtt("Zumo01/ref", "%d,%d,%d,%d,%d,%d", ref.L3, ref.L2, ref.L1, ref.R1, ref.R2, ref.R3);
            reflectance_digital(&dig);
            printf("Dig: %8d %8d %8d %8d %8d %8d\n", dig.L3, dig.L2, dig.L1, dig.R1, dig.R2, dig.R3);
            print_mqtt("Zumo01/dig", "%d,%d,%d,%d,%d,%d", dig.L3, dig.L2, dig.L1, dig.R1, dig.R2, dig.R3);
            ctr = 0;
        }
        vTaskDelay(10);
    }
 }

#endif

#if 0
void zmain(void)
{    
    RTC_Start(); // start real time clock
    
    RTC_TIME_DATE now;

    // set current time
    now.Hour = 12;
    now.Min = 34;
    now.Sec = 56;
    now.DayOfMonth = 25;
    now.Month = 9;
    now.Year = 2018;
    RTC_WriteTime(&now); // write the time to real time clock

    while(true)
    {
        if(SW1_Read() == 0) {
            // read the current time
            RTC_DisableInt(); /* Disable Interrupt of RTC Component */
            now = *RTC_ReadTime(); /* copy the current time to a local variable */
            RTC_EnableInt(); /* Enable Interrupt of RTC Component */

            // print the current time
            printf("%2d:%02d.%02d\n", now.Hour, now.Min, now.Sec);
            
            // wait until button is released
            while(SW1_Read() == 0) vTaskDelay(50);
        }
        vTaskDelay(50);
    }
 }   
#endif









// Sumo wrestling Group 6 Code Rough

#if 0
    
void check_obstacle(struct sensors_ *);
void wait_button(void);
void tank_turn(float);
void wait_end_line(struct sensors_);
void wait_until_line(struct sensors_);
void move_next_line(uint8, struct sensors_);



/**
* @brief    Clockwise tank turn
* @details  sets motors to opposite speeds to perform rank turn
* @param    float turns : multiple of 90 degrees to turn
*/
void tank_turn(float turns)
{
    // Hardcoded values here are calibrated from manual testing
    uint8 speed = 200;
    // Delay unit is the time needed to turn 90 degrees * the absolute value of turns
    uint32 delay_unit = round(131 * fabsf(turns));
    if(turns > 0) {
        SetMotors(0, 1, speed, speed, delay_unit);
    } else if(turns < 0) {
        SetMotors(1, 0, speed, speed, delay_unit);
    }
    SetMotors(0, 0, 0, 0, 0);
}

/*
*@brief    waits for button press event
*/
void wait_button(void){
    BatteryLed_Write(1);
    while(SW1_Read());
    BatteryLed_Write(0);
}

/**
* @brief    Blocks until the robot is over the line
* @details  Loops over all the sensors until any one of them is no longer black
* @param    struct _sensors dig : struct with digital sensor data
*/
void wait_end_line(struct sensors_ dig)
{
    do {
        reflectance_digital(&dig);
    } while (dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1 &&
        dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1);
}

/**
* @brief    Blocks until the robot is over a line
* @details  Loops over all the sensors until any one of them is no longer black
* @param    struct _sensors dig : struct with digital sensor data
*/
void wait_until_line(struct sensors_ dig)
{
    while(true)
    {
        reflectance_digital(&dig);
        if(dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1 &&
            dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1)
        {
            break;
        }
    }
}

/**
* @brief    Blocks until end of next line
* @details  Uses the two wait_*_line functions to move the robot to the end of the next line
* @param    uint8 speed : the movement speed of the robot
* @param    struct _sensors dig : struct with digital sensor data
*/
void move_next_line(uint8 speed, struct sensors_ dig) {
    motor_forward(speed, 0);
    wait_until_line(dig);
    wait_end_line(dig);
    motor_forward(0, 0);
}


void zmain(void){
    // Robot angle on the track 135
    TickType_t start_time = 0;
    TickType_t stop_time = 0;
    struct sensors_ dig;
    
    // Initialisation
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000);
    Ultra_Start();
    IR_Start();
    motor_start();
    motor_forward(0,0);
    
    // Robot drives until it reaches the edge of the ring
    move_next_line(100, dig);
    print_mqtt("/ready", "zumo");
    // print_mqtt(TOPIC,SUBTOPIC_ready" zumo");
    
    // Robot waits for IR remote signal
    IR_wait();
    start_time = xTaskGetTickCount(); // Stores starting time of robot
    print_mqtt("/start", "%i", time); // Prints starting time after getting IR remote signal
    
    // Robot enters the ring and drives until user button is pressed
    // This first one needs a delay to get the reflectance sensors over the line
    motor_forward(100,100);
    do {
        reflectance_digital(&dig);
        check_obstacle(&dig);
    } while(SW1_Read()); //when user button is pressed loop ends and robot stops
    
    stop_time = xTaskGetTickCount(); // Stores stopping time after user button is pressed
    print_mqtt("/stop", "%i", stop_time); // Prints robot stopping time
    print_mqtt("/time", "%i", (stop_time - start_time)); // Prints difference of starting time and stopping time
    motor_forward(0,0);
    motor_stop();
}

// Function checks for obstacles
void check_obstacle(struct sensors_ *dig){
    int distance = Ultra_GetDistance();
    // Condition to check obstacle or edge
    if(distance < 10 || (dig->L3 == 1 || dig->R3 == 1))
    {
        print_mqtt("/obstacle", "%i", xTaskGetTickCount());
        // Turns in random direction when obstacle is found
        // float random_turn = (float)rand()/(float)(RAND_MAX/2) + 1;
        //tank_turn(random_turn);
        // Reset forward motion after turn
        //motor_forward(50 ,0);
        motor_backward(150,100);
        motor_turn(250,0,200);
        motor_forward(100,100);
    }
}
/* [] END OF FILE */
#endif

// ************************************************************






























// **************************** PROJECT 1 *********************
// Sumo wrestling group 6:
// ***********************************************************

#if 0

    // Initializing tank turn
    void tank_left(uint8 l_speed, uint32 delay);
    void tank_right(uint8 r_speed, uint32 delay);
    
void zmain(void)
{

    // Code starting time stamp
    int codeStart=xTaskGetTickCount();
    int start=0, end=0, hit=0;
    
    Ultra_Start();   
    struct sensors_ dig;
    IR_Start();
    struct accData_ data;

    // Internal variables controlling robot spped and time
    int slowSpeed = 100;
    int delay=0;
    int speed=230;
    int backTime = 300;
    int inersectionCounter=0;
    int c=0;
    double angle=0;
    char robotName[10]="Zumo06/";
    
    // caliberated varibles of acceleration and time window to to filter noise
    int maxXimpact=16000;
    int maxYimpact=16000;
    int bufferNumber = 100;
    
    // Starting Motor
    motor_start();
    motor_forward(0,0);
    
    // Starting reflection sensor
    reflectance_start();
    reflectance_set_threshold(15000, 14000, 14000, 14000, 14000, 15000); 
 
    // Starting Acceleration sensor
    LSM303D_Start();
    
    
    // waiting for user to press the switch button and lift from the button
    while(SW1_Read() == 1) vTaskDelay(10);
    while(SW1_Read() == 0) vTaskDelay(1000);
    

    
    // Initializing seed for random algorithm 
    int seed = xTaskGetTickCount();
    srand(seed);
    
    
    /*
    // calibration of acceleration code starts here
    int forward=0;
    int backward=0;
    int dummy=0;
    
    while(true){
        LSM303D_Read_Acc(&data);
        
        if(c<100){
        c++;
        }
        
        if((abs(data.accX) > maxXimpact||abs(data.accY) > maxYimpact)&&c==100){
            print_mqtt(robotName, "X: %d, Y: %d",data.accX, data.accY);
            c=0;
        }
        
        if(forward<300 && dummy==0){
        motor_forward(255,delay);
        forward++;
        dummy=0;
        
        if(forward==300){
            dummy=1;
            backward=0;
        }
        }
        else if(backward<300 && dummy==1){
        motor_backward(255,delay);
        backward++;
        dummy=1;
        if(backward==300){
            dummy=0;
            forward=0;
        }
        }
    }
    // calibration code ends here
    */

    
    // check if the vehicle is centered.
    do{
        reflectance_digital(&dig);
    } 
    while(dig.L1==0 || dig.R1==0);
    
    // find first intersection
    while(inersectionCounter==0){
        reflectance_digital(&dig); 
        motor_forward(slowSpeed,delay);
        
        if(dig.L3==1 && dig.R3==1 ){
            motor_forward(0,0);
            print_mqtt(robotName, "ready zumo");
            IR_wait();  // wait for IR from user
            start=xTaskGetTickCount()-codeStart;
            print_mqtt(robotName, "start: %d",start);
            inersectionCounter++;
            motor_forward(speed,500);
        }
    }
    
    // Main Loop
    while(SW1_Read() == 1)
    {
        LSM303D_Read_Acc(&data);
        int d = Ultra_GetDistance();
        int r = rand() % 6;
        
        // moving motor forward normally
        motor_forward(speed,delay);
        reflectance_digital(&dig); 
        
        // Speeding up when robot sees some obstacles nearby
        if(d < 10) {
            motor_forward(255,delay);
        }

        
        // Reverse and tank turn right with random angle
        if(dig.L3==1 || dig.L2==1 || dig.L1==1){
            motor_backward(speed,backTime);
            tank_right(speed,50000/speed + r*50000/speed/10);
        }
        
        // Reverse and tank turn left with random angle
        else if(dig.R3==1 || dig.R2==1 || dig.R1==1){
            motor_backward(speed,backTime);
            tank_left(speed,50000/speed + r*50000/speed/10);
        } 
        
        
        // time for ignore acceleration spikes once impact acceleration is detected
        if (c<bufferNumber){
            c++;
        }
        

        // 0 to 90 degree or 0 to 270
        if (data.accX < -maxXimpact && c==bufferNumber){
            if (data.accY>0){
                // angle is 0 to 90 degree
                angle = atan(1.0*abs(data.accY)/abs(data.accX))*180.0/PI;
                hit=xTaskGetTickCount();
                print_mqtt(robotName, "hit: %d, Angle: %.0f",hit-codeStart, angle);
                c=0;
            }
            
            else{
                // angle is 0 to 270 degree
                angle = 270.0 + atan(1.0*abs(data.accX)/abs(data.accY))*180.0/PI;
                hit=xTaskGetTickCount();
                print_mqtt(robotName, "hit: %d, Angle: %.0f",hit-codeStart, angle);
                c=0;
            }
        }
        
        // 0 to 90 and 90 to 180
        else if(data.accY > maxYimpact && c==bufferNumber){
            if (data.accX<0){
                // angle is 0 to 90 degree
                angle = atan(1.0*abs(data.accY)/abs(data.accX))*180.0/PI;
                hit=xTaskGetTickCount();
                print_mqtt(robotName, "hit: %d, Angle: %.0f",hit-codeStart, angle);
                c=0;
            } else{
                // angle is 90 to 180
            angle = 90.0 + atan(1.0*abs(data.accX)/abs(data.accY))*180.0/PI;
                hit=xTaskGetTickCount();
                print_mqtt(robotName, "hit: %d, Angle: %.0f",hit-codeStart, angle);
            c=0;
            }
        }
        
        // 90 to 180 and 180 to 270
        else if(data.accX > maxXimpact && c==bufferNumber){
            if(data.accY>0){
                // angle is 90 to 180
                angle = 90.0 + atan(1.0*abs(data.accX)/abs(data.accY))*180.0/PI;
                hit=xTaskGetTickCount();
                print_mqtt(robotName, "hit: %d, Angle: %.0f",hit-codeStart, angle);
                c=0;
            }
            else{
                // angle is 180 to 270
                angle = 180.0 + atan(1.0*abs(data.accY)/abs(data.accX))*180.0/PI;
                hit=xTaskGetTickCount();
                print_mqtt(robotName, "hit: %d, Angle: %.0f",hit-codeStart, angle);
                c=0;
            }
        }
        
        // 180 to 270 and 270 to 360
        else if(data.accY < -maxYimpact && c==bufferNumber){
            if(data.accX>0){
                // angle is 180 to 270
                angle = 180.0 + atan(1.0*abs(data.accY)/abs(data.accX))*180.0/PI;
                hit=xTaskGetTickCount();
                print_mqtt(robotName, "hit: %d, Angle: %.0f",hit-codeStart, angle);
                c=0;
            }else{
                // angle is 270 to 360
                angle = 270.0 + atan(1.0*abs(data.accX)/abs(data.accY))*180.0/PI;
                hit=xTaskGetTickCount();
                print_mqtt(robotName, "hit: %d, Angle: %.0f",hit-codeStart, angle);
                c=0;
            }
        }
        
    }
    motor_forward(0,0);
    motor_stop();
    end = xTaskGetTickCount();
    print_mqtt(robotName, "stop: %d",end-codeStart);
    print_mqtt(robotName, "time: %d",end-start);
}

void tank_left(uint8 l_speed, uint32 delay)
{
    SetMotors(1,0, l_speed, l_speed, delay);
}

void tank_right(uint8 r_speed, uint32 delay)
{
    SetMotors(0,1, r_speed, r_speed, delay);
}

# endif
// ****************************************************




















// ******************** PROJECT 2 ***********************
// Line follower Group 6:
// ******************************************************

#if 0

    // Initializing tank turn
    void tank_left(uint8 l_speed, uint32 delay);
    void tank_right(uint8 r_speed, uint32 delay);
    

void zmain(void)
{
    // time marker for code start
    int codeStart;
    codeStart= xTaskGetTickCount();
   
    int start=0, stop=0;
    
    // Mqtt code
    int line=0, miss=0;
    // Mqtt

    struct sensors_ dig;
    IR_Start();
    
    // Internal Variables to control speed and turning speed
    int delay=0;
    int slowSpeed = 100;
    int speed=235;
    int turnSpeed = 235;
    int tankTurnSpeed = 180;
    int turnMinSpeed = 90;

    // internal variables for intersection counter and filter any noise if happen so based on maxcons number.
    int maxcons = 2;
    int count = 0;
    int a = -1;
    int b = 0;
    int counter = 0;
    int inersectionCounter=0;
    
    // starting the motor
    motor_start();
    motor_forward(0,0);
    
    // starting the reflection sensor and assigning caliberated threshold number.
    reflectance_start();
    reflectance_set_threshold(15000, 14000, 13500, 13500, 14000, 15000);
    
    // waiting for user to press the switch button and lift from the button
    while(SW1_Read() == 1) vTaskDelay(10);
    while(SW1_Read() == 0) vTaskDelay(1000);
    
    
    // check if the vehicle is centered.
    do{
        reflectance_digital(&dig);
    } 
    while(dig.L1==0 || dig.R1==0);
    
    
    // Main loop
    while(true)
    {
        
        reflectance_digital(&dig); 
        
        // Logic finding the intersection with filtering some noise based on maxcons number
        if(dig.L3==0 && dig.R3==0){
            if(count > 0){
                count -= 1;
            } else{
                a = b+1;
                counter = a;
            }
        } else if (dig.L3==1 && dig.R3==1){
            if (count <= maxcons){
                count += 1;
            }else{
                b = a+1;
                counter = b;
            }
        }

        // Detection of intersection.
        if(counter==2){
            count = 0;
            a = -1;
            b = 0;
            counter = 0;
            inersectionCounter++;
            
            // First intersection
            if (inersectionCounter==1){
                motor_forward(0,0);
                print_mqtt("Zumo06/", "ready line");
                BatteryLed_Write(1);
                IR_wait();  // wait for IR command
                BatteryLed_Write(0);
                start = xTaskGetTickCount();
                print_mqtt("Zumo06/", "start: %d",start-codeStart);
                line=1;             //Mqtt code
                motor_forward(160,200);
            }
            
            // Second intersection
            else if (inersectionCounter==2){
                motor_forward(speed,delay);
                
            } 
            
            // Any intersection after second intersection(third intersection to stop the robot)
            else{
                motor_forward(0,0);                
                motor_stop();
                stop = xTaskGetTickCount();
                print_mqtt("Zumo06/", "stop: %d",stop);
                print_mqtt("Zumo06/", "time: %d",stop-start);
                break;
            }
        }
        
        
        // go straight before first intersection line with slow speed
        if(dig.L1==1 && dig.R1==1 && inersectionCounter<1){
            motor_forward(slowSpeed,delay);
        } 
        
        
        // go straight after first intersection line
        else if(dig.L1==1 && dig.R1==1 && inersectionCounter>=1){
            motor_forward(speed,delay);
            
            // print line once it turns back from miss
            if (miss==1){
                print_mqtt("Zumo06/", "line");
                miss=0;
                line=1;}
        } 
        
        // slow turn left
        else if(dig.L2==1 && dig.L1==1){
            motor_turn(turnMinSpeed,turnSpeed,delay);          
        } 
        
        // slow turn right
        else if(dig.R1==1 && dig.R2==1){
            motor_turn(turnSpeed,turnMinSpeed,delay);
        } 
        
        // sharp turn left
        else if(dig.L3==1 && dig.L2==1){
            tank_left(tankTurnSpeed,delay);
            
            // print miss once it deviate from line
            if  (line==1){
            print_mqtt("Zumo06/", "miss");
            miss=1;
            line=0;}
        }
        
        // sharp turn right
        else if(dig.R2==1 && dig.R3==1){
            tank_right(tankTurnSpeed,delay);
            
            // print miss once it deviate from line
            if  (line==1){
            print_mqtt("Zumo06/", "miss");
            miss=1;
            line=0;}
        }
    }
}

// Function for left tank turn
void tank_left(uint8 l_speed, uint32 delay)
{
    SetMotors(1,0, l_speed, l_speed, delay);
}

// Functon for right tank turn
void tank_right(uint8 r_speed, uint32 delay)
{
    SetMotors(0,1, r_speed, r_speed, delay);
}

#endif
// *************************************************************































// ********************* PROJECT 3 ******************************************
// Maze runner project group 6 Code with start time end time and coordinate complete
// **************************************************************************
#if 1

    // Initializing tank turn
    void tank_left(uint8 l_speed, uint32 distDelay, uint32 angleDelay);
    void tank_right(uint8 r_speed, uint32 distDelay, uint32 angleDelay);
    
void zmain(void)
{
    struct sensors_ dig;
    IR_Start();
    Ultra_Start(); 

    int start=0, end=0;
    
    // variables controlling speed and course adjustment parameters
    int delay=0;
    int speed=100;
    int turnSpeed = 200;
    int turnMinSpeed = 0;
    int turnSharpSpeed = 250;
    int turnMinSharpSpeed = 0;
    
    // caliberated values for tank turn.
    //int halfDist=22000;
    //int halfAngle=26250;
    int halfDist=20000;
    int backHalfDist=17000;
    int halfAngle=25000;

    // internal variables taking care of intersection counters
    int maxcons = 20;
    int count = 0;
    int a = -1;
    int b = 0;
    int counter = 0;
    int inersectionCounter=0;
    

    motor_start();
    motor_forward(0,0);
    
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); 

    // wait for user button to pressed and released, code will start running after 1 sec.
    while(SW1_Read() == 1) vTaskDelay(10);
    while(SW1_Read() == 0) vTaskDelay(1000);

    // code start timestamps and use this as a seed for random algorithm
    int codeStart = xTaskGetTickCount();
    srand(codeStart);
    
    // Variables controlling coordinates and movement tracking
    int X=0;
    int Y=0;
    int back=0, side=0, right = rand()%2, margin=0;
    
    
    // check if the vehicle is centered.
    do{
        reflectance_digital(&dig);
    } 
    while(dig.L1==0 || dig.R1==0);

    // main loop of the programme
    while(true)
    {
        reflectance_digital(&dig); 
        int d = Ultra_GetDistance();
        
        
        // it filters noise in 00 and 11 in center sensors and count when center flips from 0 to 1 based on maxcons number
        if(dig.L3==0 && dig.R3==0){
            if(count > 0){
                count -= 1;
            } else{
                a = b+1;
                counter = a;
            }
        } else if ((dig.L3==1 && dig.L2==1&& dig.L1==1 )||(dig.R3==1 && dig.R2==1&& dig.R1==1 )){
            if (count <= maxcons){
                count += 1;
            }else{
                b = a+1;
                counter = b;
            }
        }
        
        // Detection of intersection
        if(counter==2){
            count = 0;
            a = -1;
            b = 0;
            counter = 0;
            inersectionCounter++;
            
            // first intersection wait for IR command
            if (inersectionCounter==1){
                motor_forward(0,0);
                printf("ready maze\n");
                IR_wait();  // wait for IR command
                start = xTaskGetTickCount();
                printf("start: %d\n",start-codeStart);
            }
            
            // second intersection where X = 0 and Y=0
            else if(inersectionCounter==2){
                printf("Coordinate X = %d, Y = %d\n",X, Y);
            }
            
            // all intersection after second intersection
            else if (inersectionCounter>2){
                
                // updating Y coordinate based on Back and side value
                if (back==0 && side==0){
                    Y=Y+1;
                }
                
                // updating X coordinate based on side value
                X += side;
                
                // creating margin which taked the boundary of maze in account
                if (Y<=10){
                    margin=3;
                }else if(Y==11){
                    margin=2;
                } else if(Y==12){
                    margin=1;
                }else if (Y==13){
                    margin=0;
                }
                
                // Deciding where to turn based on margin value and X coordinate
                if (X <= -margin){
                    right =1;
                }else if (X >= margin){
                    right = 0;
                }
                
                // Function turning left and then right after backward movement or hitting right margin/boundary
                if ((back==1 && X>-margin && right==0)||(back==0 && X>margin)){
                    if (back==1){
                        tank_left(speed, backHalfDist/speed, halfAngle/speed);
                    }else if (back ==0){
                        tank_left(speed, halfDist/speed, halfAngle/speed);
                    }
                    side=-1;
                    back=0;
                } else if (side==-1){
                    tank_right(speed, halfDist/speed, halfAngle/speed);
                    side=0;
                    back=0;
                }
                
                // Function turning right and then left after backward movement or hitting left margin/boundary
                if ((back==1 && X<margin && right==1)||(back==0 && X<-margin)){
                    if (back==1){
                        tank_right(speed, backHalfDist/speed, halfAngle/speed);
                    }else{
                        tank_right(speed, halfDist/speed, halfAngle/speed);
                    }
                    side=1;
                    back=0;
                } else if (side==1){
                    tank_left(speed, halfDist/speed, halfAngle/speed);
                    side=0;
                    back=0;
                }
                
                // Printing coordinate of each intersection
                printf("Coordinate X = %d, Y = %d\n",X, Y);
            }
        }
        
        // detect obstruction and ask to back
        if (d<4){
            back=1;
        }
        
        // stop the robot at the end and terminate from the loop
        if (Y==13 && dig.L3==0 && dig.L2==0 && dig.L1==0 && dig.R1==0&& dig.R2==0&& dig.R3==0){
            motor_forward(0,0);
            motor_stop();
            end = xTaskGetTickCount();
            printf("stop: %d\n",end-codeStart);
            printf("time: %d\n",end-start);
            break;
        }

        
        // Moving motor forward
        if(dig.L1==1 && dig.R1==1 && back==0){
            motor_forward(speed,delay);
            } 
        // Moving motor backward
        else if (dig.L1==1 && dig.R1==1 && back==1){
            motor_backward(speed,delay);
            }
        
        // Turn left for course adjustment
        else if(dig.L2==1 && dig.L1==1 && back==0){
            motor_turn(turnMinSpeed,turnSpeed,delay);
        }
        
        // Turn right for course adjustment
        else if(dig.R1==1 && dig.R2==1 && back==0){
            motor_turn(turnSpeed,turnMinSpeed, delay);
        } 
        
        // Turn sharp left for course adjustment
        else if(dig.L3==1 && dig.L2==1 && back==0){
            motor_turn(turnMinSharpSpeed,turnSharpSpeed,delay);
        }
        
        // Turn sharp right for course adjustment
        else if(dig.R2==1 && dig.R3==1 && back==0){
            motor_turn(turnSharpSpeed, turnMinSharpSpeed, delay);
        }
    }
}

// Function for Tank left turn in the intersection
void tank_left(uint8 l_speed, uint32 distDelay, uint32 angleDelay)
{
    SetMotors(0,0, l_speed, l_speed, distDelay);
    SetMotors(1,0, l_speed, l_speed, angleDelay);
}

// Function for Tank right turn in the intersection
void tank_right(uint8 r_speed, uint32 distDelay, uint32 angleDelay)
{
    SetMotors(0,0, r_speed, r_speed, distDelay);
    SetMotors(0,1, r_speed, r_speed, angleDelay);
}

# endif

// ********************************************************************




/* [] END OF FILE */
