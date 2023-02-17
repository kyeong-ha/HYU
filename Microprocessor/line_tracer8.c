/*
 * line_tracer8.c
 *
 *  Created on: 2022. 11. 15.
 *      Author: nykon
 */


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
//void main(void)
//{
//    Clock_Init48MHz();
//    TimerA2_Init(&task, 50000);
//
//    while(1) {};
//}
