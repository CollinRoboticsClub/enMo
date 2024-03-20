// This class is for L298N and similar motor drivers.
// However, it lowers the PWM frequency to have better torque at low speeds.

// Note that setting frequency registers is microcontroller-specific.
// This code assumes an ATmega8, 168, or 328P -- used on the Arduino Uno and Nano.

// [Brake-mode functionality is removed in favor of defaulting to Brake (no coast). Still, check link.
// It just means in1 and in2 both go HIGH on brake instead of conventional LOW]

// References:
// https://www.dprg.org/l298n-motor-driver-board-drive-modes/ for brake and coast mode
// https://learn.adafruit.com/improve-brushed-dc-motor-performance/choosing-decay-mode-and-pwm-frequency for PWM frequency and how it affects motors
// https://domoticx.com/arduino-pwm-frequency-and-timers/ for setting PWM frequency on an Arduino Uno (nano is identical)

use arduino_hal::{
    pac::{TC0, TC1, TC2},
    port::{
        mode::{Output, PwmOutput},
        Pin,
    },
    simple_pwm::{Prescaler, Timer0Pwm, Timer1Pwm, Timer2Pwm},
    Peripherals,
};

// See, with https://rahix.github.io/avr-hal/avr_hal_generic/port/struct.Pin.html#downgrading,
// Pins can carry their "internal registers" (per se) with their type, but stay dynamic?
// well, THE TIMER COUNTER REGISTERS DON'T HAVE THAT
// ...so here's a wrapper to avoid generics hell! (which would really only be a syntax problem, see link; but still)
// enum TCType {
//     #[deprecated(
//         note = "timer 0 breaks millis() and delay(), not sure it affects rust avr though lol"
//     )]
//     T0(Timer0Pwm),
//     T1(Timer1Pwm),
//     T2(Timer2Pwm),
// }

pub fn setup_pwm_timers(dp: Peripherals) {
    // let timer0 = Timer0Pwm::new(dp.TC0, Prescaler::Prescale64);
    let timer1 = Timer1Pwm::new(dp.TC1, Prescaler::Prescale1024);
    let timer2 = Timer2Pwm::new(dp.TC2, Prescaler::Prescale1024);
}

pub struct Motor {
    // https://rahix.github.io/avr-hal/avr_hal_generic/port/struct.Pin.html#downgrading
    // en_pin: Pin<PwmOutput<Timer0Pwm>>,
    // en_pin: Pin<PwmOutput<Timer0Pwm>, arduino_hal::hal::port::PD5>,  //  works
    en_pin: Pin<PwmOutput<Timer0Pwm>, arduino_hal::hal::port::PD1>,
    in1_pin: Pin<Output>,
    in2_pin: Pin<Output>,
    reversed: bool,
    deadzone: u8,
}

impl Motor {
    pub fn new(
        // en_pin: Pin<PwmOutput<TC0>>,
        en_pin: Pin<PwmOutput<Timer0Pwm>, arduino_hal::hal::port::PD1>,
        in1_pin: Pin<Output>,
        in2_pin: Pin<Output>,
        reversed: bool,
        deadzone: u8,
    ) -> Self {
        let mut m = Motor {
            en_pin,
            in1_pin,
            in2_pin,
            reversed,
            deadzone,
        };

        m.set_speed(0);

        m
    }

    /// Accepts -255 to 255.
    pub fn set_speed(&mut self, mut speed: i16) {
        // Restrict speed to -255 to 255 in the event it came out of bounds
        speed = speed.clamp(-255, 255);

        // Apply deadzone
        if speed.abs() < self.deadzone.into() {
            speed = 0;
        }

        // Apply direction reversal
        if self.reversed {
            speed = -speed;
        }

        // Set direction
        match speed.cmp(&0) {
            core::cmp::Ordering::Greater => {
                self.in1_pin.set_high();
                self.in2_pin.set_low();
            }
            core::cmp::Ordering::Less => {
                self.in1_pin.set_low();
                self.in2_pin.set_high();
            }
            core::cmp::Ordering::Equal => {
                // HIGH, HIGH allows for the motor to stop quicker, consuming more current
                // aka "brake mode"
                self.in1_pin.set_high();
                self.in2_pin.set_high();
            }
        }

        // Write speed
        self.en_pin.set_duty(speed.abs().try_into().unwrap());
    }
}
