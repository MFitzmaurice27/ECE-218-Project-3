# Engine and Windshield Wiper Control System
James Dwyer and Michael Fitzmaurice

## System Behavior
This is a car system which has the windshield wiper functionality included.  

When the driver and passenger are seated, and they have their seatbelts fastened, the engine is able to be started with the ignition button. If any of these conditions are not met when the ignition button is pressed, the alarm will go off, but the user can still fulfil all of the conditions and start the engine after the alarm goes off.  

As long as the engine is started, the windshield wiper system can be used. There are four modes that the user can select with the use of a dial: HIGH, LOW, OFF, and INTERMITTENT. THe HIGH mode has the wiper continuously wiping at a rotational speed of 40 rpm. The LOW mode has the wiper wiping with a slight delay between wipes at a rotational speed of 30 rpm. The OFF mode has the wiper completely off, that is, it never wipes. The INTERMITTENT mode has the wiper wipe at a rotational speed of 30 rpm, with a variable delay dependant on a second dial, with the possible delays 3 seconds, 6 seconds, or 8 seconds.

## Design Alternatives
One choice we had to make in the construction of this project was the use of a positional servo motor over a continuous servo motor. We chose to use the positional servo motor because of its ability to rotate to a precise 67° angle as specified in the project requirements. One downside to using the positional servo compared to the continuous servo is that it is not easy to change the speed of the rotation of the motor. We decided that the ability to achieve a precise angle outweighs the ability to easily change the speed of rotation.

## Summary of Testing Results

## Modular Diagram of the Code
![Blank diagram (1)](https://github.com/user-attachments/assets/69c243e4-1b90-4d63-b989-ea2e3d67886c)
