const API_SERVER_IP = location.host;
const API_SERVER_URL = `http://${API_SERVER_IP}/api`;
const API_ENDPOINT_WHEELS = `${API_SERVER_URL}/move-wheels`;

// TODO: make a way to send claw close/open and arm up/down input in UI and implement it API-side
// init joysticks
const joystickOptions = {
    internalFillColor: '#AA0000',
    internalStrokeColor: '#330000',
    internalLineWidth: 2,
    externalStrokeColor: '#800000',
    externalLineWidth: 2,
};
const joystickRobotPosition = new JoyStick(
    'joystickRobotPosition',
    joystickOptions
);
const joystickRobotRotation = new JoyStick(
    'joystickRobotRotation',
    joystickOptions
);

const intervalId = window.setInterval(function () {
    sendData();
}, 1000); // TODO: once out of the debugging phase, change this to something much lower (faster)

// Clamp value to [-100, 100] to work around bug of joysticks sometimes going out of their intended bounds
function clamp(number) {
    let min = -100;
    let max = 100;
    return Math.max(min, Math.min(number, max));
}

function sendData() {
    let x = Number(joystickRobotPosition.GetX());
    let y = Number(joystickRobotPosition.GetY());
    let rotation_magnitude = Number(joystickRobotRotation.GetX());

    // clamp values to address bug mentioned above clamp() declaration
    x = clamp(x);
    y = clamp(y);
    rotation_magnitude = clamp(rotation_magnitude);

    // change range from [-100, 100] to [-1, 1]
    x /= 100;
    y /= 100;
    rotation_magnitude /= 100;

    let data = {
        x: x,
        y: y,
        rotation: rotation_magnitude,
    };

    console.log(data); // DEBUGGING:

    fetch(API_ENDPOINT_WHEELS, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(data),
    });
}
