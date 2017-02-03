#include <isr/timer.h>
#include <isr/idt.h>
#include <isr/apic.h>
#include <terminate.h>

static void timer_isr(interrupt_frame_t *frame __attribute__ ((unused))) {
    printf("hOI!!!!!! I'm tIMER!!\n");
    wrapic(APIC_EOI_REGISTER, 0);
}

void timer_init(void) {
    install_isr(timer_isr, TMR_VECTOR);

    wrapic(APIC_TMR_INITCNT_REGISTER, 10000000);
    wrapic(APIC_TMR_LVT_REGISTER, TMR_VECTOR | APIC_TMR_LVT_PERIODIC);
    wrapic(APIC_TMR_DIV_REGISTER, APIC_TMR_DIV_DIV16);
}
