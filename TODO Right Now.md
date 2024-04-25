TODO Right Now - 4/25/24 overnight - josh 😢😢😢😢😢😭😭

- Arm code
  - PCA9685 to Pi - using python, perhaps... might be quick

- Drive code
  - SquareDrive class
    - uses existing Motor class

- Make a TESTING ROUTINES .MD ! ! !
  - Drive
    - Instructions for us to ensure pins are set correctly
    - Instructions for directional testing of each motor once pin mapping is good
  - Arm
    - Same old pinout check (way easier though)
    - Instructions to test arm directions and switch them incrementally



# What if we make it run some autonomous demos for Friday...?
Just for HARDWARE proof that it can run off that???
This might mean NOT using the WebUI


---
In theory, nice to have:
  - AIMING, which needs:
    - webcam access, ideally streaming the opencv output to the webserver
      - consider... CameraServer??? https://docs.wpilib.org/en/stable/docs/software/vision-processing/wpilibpi/using-cameraserver.html
    - a PID controller or other motion controller to point the drivetrain at that spot
      - not impossible... but takes *LOTS of time!*