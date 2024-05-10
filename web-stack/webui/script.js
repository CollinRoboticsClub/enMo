const API_SERVER_WS_URL = `ws://${location.host}/api`;
const API_ENDPOINT_WHEELS = `${API_SERVER_WS_URL}/wheels/move`;
const API_ENDPOINT_ARM = `${API_SERVER_WS_URL}/arm/move`;

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
const joystickArmGripper = new JoyStick('joystickArmGripper', joystickOptions);

var ws_arm = new WebSocket(API_ENDPOINT_ARM);
var ws_wheels = new WebSocket(API_ENDPOINT_WHEELS);

const sendWheelDataIntervalMS = 100;
const sendArmDataIntervalMS = 25;

ws_wheels.onopen = (event) => {
    window.setInterval(function () {
        sendWheelData();
    }, sendWheelDataIntervalMS);
};
ws_arm.onopen = (event) => {
    window.setInterval(function () {
        sendArmData();
    }, sendArmDataIntervalMS);
};

// Clamp value to [-100, 100] to work around bug of joysticks sometimes going out of their intended bounds
function clamp(number) {
    let min = -100;
    let max = 100;
    return Math.max(min, Math.min(number, max));
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
        wheelData[value] = clamp(wheelData[value]);

        // change wheel range from [-100, 100] to [-1, 1]
        wheelData[value] /= 100;
    }

    ws_wheels.send(JSON.stringify(wheelData));
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
        gripper_angle: armGripperRotation,
    };

    let allZeroes = true;
    for (const servo in armData) {
        if (armData[servo] == 0) {
            continue;
        } else {
            allZeroes = false;
        }

        // clamp values to address bug mentioned above clamp() declaration
        armData[servo] = clamp(armData[servo]);

        // TODO: check if this speed is fine, if not, adjust it and delete this TODO
        // Since these are *relative* movements (change in degrees) that we're sending, and we send them pretty
        // quickly, we wouldn't want to move the servos *too* quickly, so:
        // change arm range from [-100, 100] to [-10, 10]
        armData[servo] /= 10;

        // turn into int
        armData[servo] = Math.trunc(armData[servo]);
    }

    if (!allZeroes) {
        ws_arm.send(JSON.stringify(armData));
    }
}
