# Trajectory Tracking Feature

## Overview
The trajectory tracking feature allows you to draw a custom waveform on a canvas, and the PID controller will attempt to follow it in real-time.

## How to Use

### 1. Build and Run the GUI
```bash
cd gui
qmake PWMController.pro
make
./PWMController
```

### 2. Draw Your Trajectory
1. Click the **"Draw Trajectory"** button in the main window
2. A new window will open with a drawing canvas
3. Click and drag your mouse to draw the desired waveform
   - Top of canvas = 3.3V (ADC 1023)
   - Bottom of canvas = 0V (ADC 0)
   - Middle line = 1.65V (ADC 512)

### 3. Send to Device
1. Click **"Send to Device"** button
   - This samples your drawing into ~100 points
   - Sends each point to the microcontroller via serial
   - Device stores the trajectory in memory

### 4. Start Tracking
1. Click **"Start Tracking"** button
   - Enables PID control
   - Activates trajectory tracking mode
   - PID will cycle through your drawn points every ~210ms per point

### 5. Stop Tracking
1. Click **"Stop Tracking"** button
   - Disables trajectory mode
   - Stops PID control

### 6. Clear and Redraw
1. Click **"Clear"** button to erase and start over

## Serial Commands (Manual Control)

If you want to control via serial terminal:

```
C 0       - Clear trajectory
A 512     - Add trajectory point (value 0-1023)
A 600     - Add another point
A 400     - Add another point
W 0       - Start trajectory tracking
P 0       - Enable PID
E 0       - End trajectory tracking
Q 0       - Disable PID
```

## Example Use Cases

1. **Sine Wave**: Draw a smooth sine wave to test PID response
2. **Square Wave**: Draw sharp transitions to test settling time
3. **Ramp**: Draw a linear ramp to test tracking accuracy
4. **Custom Pattern**: Draw any pattern you want the system to follow

## Technical Details

- Maximum trajectory points: 200
- Typical sampling: 100 points
- Update rate: ~210ms per point (configurable in firmware)
- ADC range: 0-1023 (0-3.3V)
- PID parameters: Kp=0.05, Ki=0.01, Kd=0.0

## Tips

- Draw slowly for smoother trajectories
- Keep drawings within the canvas bounds
- More complex patterns may be harder for PID to track
- Adjust PID gains in firmware for better tracking performance
