# 1 "timer.c"
# 1 "/Users/cj/Documents/cs107e-home/cs107e.github.io/lectures/Linking/code/blink//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "timer.c"
# 1 "timer.h" 1
# 18 "timer.h"
void timer_init(void);
# 27 "timer.h"
unsigned int timer_get_ticks(void);






void timer_delay_us(unsigned int usec);






void timer_delay_ms(unsigned int msec);






void timer_delay(unsigned int sec);
# 2 "timer.c" 2

void timer_init(void) {
}

unsigned int timer_get_ticks(void) {
    return 0;
}

void timer_delay_us(unsigned int usecs) {
    unsigned int start = timer_get_ticks();
    while (timer_get_ticks() - start < usecs) { }
}

void timer_delay_ms(unsigned int msecs) {
    timer_delay_us(1000*msecs);
}

void timer_delay(unsigned int secs) {
    timer_delay_us(1000000*secs);
}
