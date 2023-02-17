/*
 * line_tracer7.c
 *
 *  Created on: 2022. 11. 1.
 *      Author: nykon
 */


//void main(void)
//{
//    led_init();
//    sensor_init();
//    motor_init();
//
//    int speed=50;
//    int sensor1;
//    int sensor2;
//
//    while(1) {
//        //Turn on IR LEDs
//        P5->OUT |= 0x08;
//        P9->OUT |= 0x04;
//
//        // P7 OUTPUT
//        P7->DIR = 0xFF;
//        // capacitor charge
//        P7->OUT = 0xFF;
//        // fully charge
//        Clock_Delay1us(10);
//        // P7 input
//        P7->DIR = 0x00;
//
//        // wait for a while
//        Clock_Delay1us(200);
//
//        //sensor IR 4,5th
//        sensor1=P7->IN & 0x18;
//        sensor2=P7->IN & 0x7E;
//        if(sensor1) {
//            P2->OUT |= 0x01;
//            // MOVE
//            // motor on
//            P5->OUT &= ~0x30;
//            P2->OUT |= 0xC0;
//            P3->OUT |= 0xC0;
//            Clock_Delay1ms(speed);
//        }
//
//        else if(sensor2) {
//            //don't move
//            P2->OUT &= ~0x07;
//            // motor stop
//            P2->OUT &= ~0XC0;
//        }
//
//        else {
//            P2->OUT &= ~0x07;
//            P2->OUT &= ~0XC0;
//        }
//
//        P2->OUT &= ~0XC0;
//        // turn off IR led
//        P5->OUT &= ~0x08;
//        P9->OUT &= ~0x04;
//
//        Clock_Delay1ms(10);
//    }
//}

