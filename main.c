#include <stdint.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pins.h"
#include "pwm.h"

int main(void)
{
    //wdt_enable(WDTO_1S);    // enable 1s watchdog timer
    uint8_t last_value = 0;

	// Set output lines
	SET_BIT(DDR_LED, BIT_LED);
	SET_BIT(DDR_DEBUG, BIT_DEBUG);

    // Set switch as input
    CLEAR_BIT(DDR_SWITCH_1, BIT_SWITCH_1);

    pwm_init();
    sei();

    while(1) {
        wdt_reset();

        if(GET_BIT(PIN_SWITCH_1, BIT_SWITCH_1)) {
            SET_BIT(PORT_LED, BIT_LED);
            // Check for transition
            if(last_value == 0) {
                TCNT1 = 0;
                last_value = 1;
                set_pwm_interrupt(1);
            }
        } else {
            CLEAR_BIT(PORT_LED, BIT_LED);
            // Check for transition
            if(last_value == 1) {
                TCNT1 = 0;
                last_value = 0;
                set_pwm_interrupt(1);
            }
        }

        _delay_ms(1);
    }
}
