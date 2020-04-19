# 1 "cstart.c"
# 1 "/Users/cj/Documents/cs107e-home/cs107e.github.io/lectures/Linking/code/blink//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "cstart.c"

extern int __bss_start__, __bss_end__;

extern void main(void);





void _cstart(void)
{
    int *bss = &__bss_start__;
    int *bss_end = &__bss_end__;

    while (bss < bss_end) {
        *bss++ = 0;
    }

    main();


    volatile unsigned int *GPIO_FSEL4 = (unsigned int *)0x20200010;
    volatile unsigned int *GPIO_SET1 = (unsigned int *)0x20200020;
    *GPIO_FSEL4 = (*GPIO_FSEL4 & ~(7 << 21)) | (1 << 21);
    *GPIO_SET1 = 1 << 15;
}
