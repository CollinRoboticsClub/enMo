const API_SERVER_IP = location.origin;
const API_SERVER_URL = `${API_SERVER_IP}/api`;
const API_ENDPOINT_WHEELS = `${API_SERVER_URL}/move-wheels`;
const API_ENDPOINT_ARM = `${API_SERVER_URL}/move-arm`;

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
const joystickArmBaseShoulder = new JoyStick(
    'joystickArmBaseShoulder',
    joystickOptions
);
const joystickArmElbowWrist = new JoyStick(
    'joystickArmElbowWrist',
    joystickOptions
);
const joystickArmGripper = new JoyStick(
    'joystickArmGripper',
    joystickOptions
);


const sendDataIntervalMS = 200;
window.setInterval(function () {
    sendWheelData();
    sendArmData();
}, sendDataIntervalMS);

// Clamp value to [-100, 100] to work around bug of joysticks sometimes going out of their intended bounds
function clamp(number) {
    let min = -100;
    let max = 100;
    return Math.max(min, Math.min(number, max));
}

function sendData(jsonData, endpoint) {
    console.log(jsonData); // DEBUGGING:

    fetch(endpoint, {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify(jsonData),
    });
}

function sendWheelData() {
    // wheels
    let wheelX = Number(joystickRobotPosition.GetX());
    let wheelY = Number(joystickRobotPosition.GetY());
    let wheelRotationMagnitude = Number(joystickRobotRotation.GetX());

    let wheelData = {
        x: wheelX,
        y: wheelY,
        rotation: wheelRotationMagnitude,
    };

    for (const value in wheelData) {
        // clamp values to address bug mentioned above clamp() declaration
        wheelData[value] = clamp(wheelData[value])

        // change wheel range from [-100, 100] to [-1, 1]
        wheelData[value] /= 100
    }

    sendData(wheelData, API_ENDPOINT_WHEELS);
}

function sendArmData() {
    // arm
    let armBaseRotation = Number(joystickArmBaseShoulder.GetX());
    let armShoulderRotation = Number(joystickArmBaseShoulder.GetY());
    let armElbowRotation = Number(joystickArmElbowWrist.GetX());
    let armWristRotation = Number(joystickArmElbowWrist.GetY());
    let armGripperRotation = Number(joystickArmGripper.GetX());
    let armUnusedRotation = Number(joystickArmGripper.GetY());

    let armData = {
        base_angle: armBaseRotation,
        shoulder_angle: armShoulderRotation,
        elbow_angle: armElbowRotation,
        wrist_angle: armWristRotation,
        gripper_angle: armGripperRotation
    };

    for (const servo in armData) {
        // clamp values to address bug mentioned above clamp() declaration
        armData[servo] = clamp(armData[servo]);

        // TODO: check if this speed is fine, if not, adjust it and delete this TODO
        // Since these are *relative* movements (change in degrees) that we're sending, and we send them pretty
        // quickly, we wouldn't want to move the servos *too* quickly, so:
        // change arm range from [-100, 100] to [-10, 10]
        armData[servo] /= 10;

        // turn into int
        armData[servo] = Math.trunc(armData[servo])
    }

    sendData(armData, API_ENDPOINT_ARM);
}