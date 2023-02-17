/*
 * line_tracer6.c
 *
 *  Created on: 2022. 10. 25.
 *      Author: nykon
 */
//
//#include "msp.h"
//#include "Clock.h"
//#include <stdio.h>
//
//#define LED_RED 1
//#define LED_GREEN (LED_RED << 1)
//#define LED_BLUE  (LED_RED << 2)
//
///**
// * main.c
// */
//
//void led_init() {
//    P2->SEL0 &= ~0x07;
//    P2->SEL1 &= ~0X07;
//    P2->DIR |= 0X07;
//    P2->OUT &= ~0X07;
//}
//void turn_on_led(int color) {
//    P2->OUT |= ~0b00000001;
//    P2->OUT |= color;
//}
//
//void turn_off_led() {
//    P2->OUT &= ~0b11111110;
//}
//
//void switch_init() {
//    P1->SEL0 &= ~0x12;
//    P1->SEL1 &= ~0x12;
//
//    P1->DIR &= ~0x12;
//
//    P1->REN |= 0x12;
//
//    P1->OUT |= 0x12;
//}
//
//void systick_init(void) {
//    SysTick->LOAD = 0x00FFFFFF;
//    SysTick->CTRL = 0x00000005;
//}
//
//void systick_wait1ms() {
//    SysTick->LOAD = 47999;
//    SysTick->VAL = 0;
//    while((SysTick->CTRL & 0x00010000)==0) {};
//}
//
//void systick_wait1s() {
//    int i;
//    int count = 1000;
//
//    for(i=0; i<count; i++) {
//        systick_wait1ms();
//    }
//}
//
//
//
//void main(void)
//{
//    int sensor;
//    led_init();
//
//    //0, 2, 4, 6 IR Emitter
//    P5->SEL0 &= ~0x08;
//    P5->SEL1 &= ~0x08;
//    P5->DIR |= 0x08;
//    P5->OUT &= ~0x08;
//
//    //1, 3, 5, 7 IR Emitter
//    P9->SEL0 &= ~0x04;
//    P9->SEL1 &= ~0x04;
//    P9->DIR |= 0x04;
//    P9->OUT &= ~0x04;
//
//    //0~7 IR Sensor
//    P7->SEL0 &= ~0xFF;
//    P7->SEL1 &= ~0xFF;
//    P7->DIR &= ~0xFF;
//
//    while(1) {
//        //Turn on IR LEDs
//        P5->OUT |= 0x08;
//        P9->OUT |= 0x04;
//
//        P7->DIR = 0xFF;
//        P7->OUT = 0xFF;
//        Clock_Delay1us(10);
//
//        P7->DIR = 0x00;
//
//        Clock_Delay1us(80);
//
//        //4, 5th sensor
////        int i;
////        for(i=0;i<10000;i++) {
////            sensor=P7->IN & 0x18;
////            if(!sensor) {
////                //P2->OUT |= 0x01;
////                printf("timing constantL %d\n", i);
////                break;
////            }
////            Clock_Delay1us(1);
////        }
////
//        sensor=P7->IN & 0x18;
//        if(sensor) {
//            P2->OUT |= 0x01;
//        }
//        else {
//            P2->OUT &= ~0x07;
//        }
//
//
//        P5->OUT &= ~0x08;
//        P9->OUT &= ~0x04;
//
//        Clock_Delay1ms(10);
//    }
//}


