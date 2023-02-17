/*
 * line_tracer5.c
 *
 *  Created on: 2022. 10. 11.
 *      Author: nykon
 */


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
//    int sw1;
//    int min, sec, msec = 0;
//    int count=0;
//
//    //initialization
//    Clock_Init48MHz();
//    led_init();
//    switch_init();
//    systick_init();
//
//    while(1) {
//        sw1 = P1->IN & 0x02;
//        systick_wait1ms();
//
//        if(!sw1) {
//            msec=(47999-SysTick->VAL)%1000;
//
//            min=sec/60;
//            sec=sec%60;
//
//            printf(" %d\n", SysTick->VAL);
//            printf("%d m %d s %d ms\n", min, sec, msec);
//        }
//        sec++;
//
//    }
//}


