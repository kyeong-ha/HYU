/*
 * basic_track.c
 *
 *  Created on: 2022. 11. 29.
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
//void (*TimerA2Task)(void);
//
//void TimerA2_Init(void(*task)(void), uint16_t period) {
//    TimerA2Task = task;
//    TIMER_A2->CTL = 0x0280;
//    TIMER_A2->CCTL[0] = 0x0010;
//    TIMER_A2->CCR[0] = (period - 1);
//    TIMER_A2->EX0 = 0X0005;
//    NVIC->IP[3] = (NVIC->IP[3]&0xFFFFFF00)|0x00000040;
//    NVIC->ISER[0] = 0x00001000;
//    TIMER_A2->CTL |= 0x0014;
//}
//
//void TA2_0_IRQHandler (void) {
//    TIMER_A2->CCTL[0] &= ~0x0001;
//    (*TimerA2Task)();
//}
//
//void task() {
//    printf("interrupt occurs!\n");
//}
//
//void timer_A3_capture_init() {
//    P10->SEL0 |= 0x30;
//    P10->SEL1 &= ~0x30;
//    P10->DIR &= ~0x30;
//
//    TIMER_A3->CTL &= ~0x0030;
//    TIMER_A3->CTL = 0x0200;
//
//    TIMER_A3->CCTL[0] = 0x4910;
//    TIMER_A3->CCTL[1] = 0x4910;
//    TIMER_A3->EX0 &= ~0x0007;
//
//    NVIC->IP[3] = (NVIC->IP[3]&0x0000FFFF) | 0x40400000;
//    NVIC->ISER[0] = 0x0000C000;
//    TIMER_A3->CTL |= 0x0024;
//}
//
//uint16_t first_left;
//uint16_t first_right;
//uint16_t period_left;
//uint16_t period_right;
//
//void TA3_0_IRQHandler(void) {
//    TIMER_A3->CCTL[0] &= ~0x0001;
//    period_right = TIMER_A3->CCR[0] - first_right;
//    first_right = TIMER_A3->CCR[0];
//}
//
////void TA3_N_IRQHandler(void) {
////    TIMER_A3->CCTL[1] &= ~0x0001;
////    period_left = TIMER_A3->CCR[1] - first_left;
////    first_left = TIMER_A3->CCR[1];
////}
//
//uint32_t get_left_rpm() {
//    return 2000000 / period_left;
//}
//
//uint32_t left_count;
//void TA3_N_IRQHandler(void) {
//    TIMER_A3->CCTL[1] &= ~0x0001;
//    left_count++;
//}
//
//void main(void)
//{
//    Clock_Init48MHz();
//    systick_init();
//    led_init();
//    switch_init();
//    sensor_init();
//    motor_init();
//    timer_A3_capture_init();
//
//    int sensor2;
//    int sensor3;
//    int sensor4;
//    int sensor5;
//    int sensor6;
//    int sensor7;
//    int sensor_start;
//    int count=0;
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
//        //sensor IR 2th
//        sensor2=P7->IN & 0x02;
//        //sensor IR 3th
//        sensor3=P7->IN & 0x04;
//        // sensor IR 4th
//        sensor4=P7->IN & 0x08;
//        //sensor IR 5th
//        sensor5=P7->IN & 0x10;
//        //sensor IR 6th
//        sensor6=P7->IN & 0x20;
//        //sensor IR 7th
//        sensor7=P7->IN & 0x40;
//        //sensor IR 2 to 7th
//        sensor_start = P7->IN & 0x7E;
//
////sensor2 && sensor3 && sensor4 && sensor5 && sensor6 && sensor7
//        if(sensor2 && sensor3 && sensor4 && sensor5 && sensor6 && sensor7) {
//            left_forward();
//            right_forward();
//            move(1000, 1000);
//
//            if(count > 2) {
//                move(0,0);
//                return;
//            }
//            else{
//                while(1) {
////                    left_forward();
////                    right_forward();
//                    if(left_count > 20) {
//                        count += 1;
//                        break;
//                    }
//                }
//            }
//            printf("%d\n", count);
//        }
//
//        else if (sensor4 && sensor5) {
//                P2->OUT |= 0x01;
//                // go straight
////            printf("go straight\n");
//                left_forward();
//                right_forward();
//                move(1000, 1000);
////            systick_wait1s();
//
//        }
//
//        else if(sensor4) {
//            if(sensor3){
//                while(1) {
//                    sensor3=P7->IN & 0x04;
//
////                    printf("sensor 3 turn!\n");
//                    left_forward();
//                    right_backward();
//                    move(1000, 1000);
//                    if(!sensor3){
////                        printf("sensor 3 not turn!\n");
//                        break;
//                    }
//                }
//            }
//
//            else{
//                P2->OUT |= 0x01;
//                //turn right
//    //            printf("turn right\n");
//                right_forward();
//                move(0, 800);
//    //            systick_wait1s();
//            }
//        }
//
//        else if(sensor5) {
//            if(sensor6){
//                while(1) {
//                    sensor6=P7->IN & 0x04;
//
////                    printf("sensor 3 turn!\n");
//                    right_forward();
//                    left_backward();
//                    move(1000, 1000);
//                    if(!sensor6){
////                        printf("sensor 3 not turn!\n");
//                        break;
//                    }
//                }
//            }
//
//            else {
//                P2->OUT |= 0x01;
//                //turn left
//    //            printf("turn left\n");
//                left_forward();
//                move(800, 0);
//    //            systick_wait1s();
//            }
//        }
//
//
//
//        else {
////            printf("no signal\n");
//            //LED turn off
//            P2->OUT &= ~0x07;
//            // motor stop
//            move(0,0);
//        }
//
//    }
//}
