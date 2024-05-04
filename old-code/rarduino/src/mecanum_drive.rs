use crate::motor::Motor;

pub struct MecanumDrive {
    front_left: Motor,
    front_right: Motor,
    back_left: Motor,
    back_right: Motor,
}

impl MecanumDrive {
    pub fn new(front_left: Motor, front_right: Motor, back_left: Motor, back_right: Motor) -> Self {
        Self {
            front_left,
            front_right,
            back_left,
            back_right,
        }
    }

    // Source: Method 1 in https://www.youtube.com/watch?v=gnSW2QpkGXQ
    pub fn set_speed(&mut self, x: i16, y: i16, rotation: i16) {
        self.front_left.set_speed(y + x + rotation);
        self.front_right.set_speed(y - x - rotation);
        self.back_left.set_speed(y - x + rotation);
        self.back_right.set_speed(y + x - rotation);
    }
}