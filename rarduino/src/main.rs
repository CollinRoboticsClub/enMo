#![no_std]
#![no_main]

/*
* For examples (and inspiration), head to
*
*     https://github.com/Rahix/avr-hal/tree/main/examples
*
* NOTE: Not all examples were ported to all boards!  There is a good chance though, that code
* for a different board can be adapted for yours.  The Arduino Uno currently has the most
* examples available.
*/
mod mecanum_drive;
mod motor;

use arduino_hal::simple_pwm::{IntoPwmPin, Prescaler, Timer0Pwm};
use panic_halt as _;

use motor::Motor;

#[arduino_hal::entry]
fn main() -> ! {
    let dp = arduino_hal::Peripherals::take().unwrap();
    let pins = arduino_hal::pins!(dp);

    let timer0 = Timer0Pwm::new(dp.TC0, Prescaler::Prescale64);

    let mut led = pins.d13.into_output();

    // Digital pin 5 is connected to a LED and a resistor in series
    let mut pwm_led = pins.d5.into_output().into_pwm(&timer0);
    pwm_led.enable();

    loop {
        // led.toggle();
        // arduino_hal::delay_ms(1000);

        for x in (0..=255).chain((0..=254).rev()) {
            pwm_led.set_duty(x);
            arduino_hal::delay_ms(10);
        }
    }
}
