#include <isr/timer.h>
#include <isr/idt.h>
#include <isr/apic.h>
#include <terminate.h>

static void timer_isr(interrupt_frame_t *frame __attribute__ ((unused))) {
    printf("hOI!!!!!! I'm tIMER!!\n");
    wrapic(APIC_EOI_REGISTER, 0);
}

void timer_init(void) {


}
