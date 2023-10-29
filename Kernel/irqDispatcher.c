#include <time.h>
#include <stdint.h>
#include <keyboard_driver.h>
#include <sys_calls.h>

static void int_20();
static void int_21(uint64_t scancode);

void irqDispatcher(uint64_t irq, uint64_t scancode) {
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21(scancode);
			break;
	}
	return;
}

void int_20() {
	timer_handler();
}

void int_21(uint64_t scancode) {
	keyHandler(scancode);
}
