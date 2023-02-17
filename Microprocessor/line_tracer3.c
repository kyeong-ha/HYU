/*
 * assignment.c
 *
 *  Created on: 2022. 9. 27.
 *      Author: nykon
 */


//Line Tracer3
//#include "msp.h"
//#include "Clock.h"
//#include <stdio.h>
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
//void turn_on_led() {
//    P2->OUT |= ~0b00000001;
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
//void main(void)
//{
//    int sw1;
//
//    //initialization
//    Clock_Init48MHz();
//    led_init();
//    switch_init();
//
//    while(1) {
//        sw1 = P1->IN & 0x02;
//        if(!sw1) {
//            printf("Pressed!\n");
//            turn_on_led();
//        }
//        else
//            turn_off_led();
//        Clock_Delay1ms(100);
//    }
//
//}

