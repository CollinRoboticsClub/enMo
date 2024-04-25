BUILD_ARGS := --clean --verify --fqbn arduino:avr:nano:cpu=atmega328old
# BUILD_ARGS := --clean --verify --fqbn arduino:avr:nano:cpu=atmega328

# You're probably gonna wanna edit the value of "--port" based on the serial port path for your system
# UPLOAD_ARGS := --upload --port=/dev/ttyUSB0
UPLOAD_ARGS := --upload --port=COM4

build:
	arduino-cli compile ${BUILD_ARGS} .

upload:
	arduino-cli compile ${UPLOAD_ARGS} ${BUILD_ARGS} .
