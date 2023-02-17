/*
 * line_tracer8.c
 *
 *  Created on: 2022. 11. 8.
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
//void sensor_init() {
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
//}
//
//void pwm_init34(uint16_t period, uint16_t duty3, uint16_t duty4) {
//    TIMER_A0->CCR[0] = period;
//
//    TIMER_A0->EX0 = 0x0000;
//
//    TIMER_A0->CCTL[3] = 0x0040;
//    TIMER_A0->CCR[3] = duty3;
//    TIMER_A0->CCTL[4] = 0x0040;
//    TIMER_A0->CCR[4] = duty4;
//
//    TIMER_A0->CTL = 0x02F0;
//
//    P2->DIR |= 0xC0;
//    P2->SEL0 |= 0xC0;
//    P2->SEL1 &= ~0xC0;
//}
//
//void motor_init(void) {
//    P3->SEL0 &= ~0xC0;
//    P3->SEL1 &= ~0xC0;
//    P3->DIR |= 0xC0;
//    P3->OUT &= ~0xC0;
//
//    P5->SEL0 &=~0x30;
//    P5->SEL1 &= ~0X30;
//    P5->DIR |= 0x30;
//    P5->OUT &= ~0x30;
//
//    P2->SEL0 &= ~0xC0;
//    P2->SEL1 &= ~0xC0;
//    P2->DIR |= 0xC0;
//    P2->OUT &= ~0xC0;
//
//    pwm_init34(7500, 0, 0);
//}
//
//void move(uint16_t leftDuty, uint16_t rightDuty) {
//    P3->OUT |= 0xC0;
//    TIMER_A0->CCR[3] = leftDuty;
//    TIMER_A0->CCR[4] = rightDuty;
//}
//
//void left_forward() {
//    P5->OUT &= ~0x10;
//}
//
//void left_backward() {
//    P5->OUT |= 0x10;
//}
//
//void right_forward() {
//    P5->OUT &= ~0x20;
//}
//
//void right_backward() {
//    P5->OUT |= 0x20;
//}
//
//void main(void)
//{
//    Clock_Init48MHz();
//    systick_init();
//    led_init();
//    sensor_init();
//    motor_init();
//
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
//        Clock_Delay1us(1000);
//
//        //sensor IR 4th
//        sensor1=P7->IN & 0x08;
//        //sensor IR 5th
//        sensor2=P7->IN & 0x10;
//
//        if(sensor1&sensor2) {
//            P2->OUT |= 0x01;
//            // MOVE
//            left_forward();
//            right_forward();
//            move(400, 400);
//            systick_wait1s();
//        }
//
//        else if(sensor1) {
//            P2->OUT |= 0x01;
//            // turn left
//            right_forward();
//            move(0, 400);
//            systick_wait1s();
//        }
//
//        else if(sensor2) {
//            P2->OUT |= 0x01;
//            //turn right
//            left_forward();
//            move(400, 0);
//            systick_wait1s();
//        }
//
//        else {
//            //LED turn off
//            P2->OUT &= ~0x07;
//            // motor stop
//            move(0,0);
//        }
//
//    }
//
//}

