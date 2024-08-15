- Understanding the Car's Electrical System:


# Boraginaceae

Eletronic interface with the car


## Car Interface Documentation



## Understanding the Car's Electrical System:
### Hazard lights
    Hazard Lights: These are controlled through a relay or the car’s Body Control Module (BCM). The lights themselves operate on a 12V system.
### Door locks
    Door Locks: These can be actuated by a motor or solenoid, typically controlled by  the car's central locking system.

## Safety Considerations:
### Isolation:
    Use optocouplers or relays to isolate the Arduino from the car’s high-voltage electrical system.
### Power Supply
    Ensure the microcontroler board is powered safely, typically using a DC-DC converter to step down from 12V to 5V or 3.3v.
#### Fusing
    Add appropriate fuses to protect your circuit.

## Components Required
    Microcontroler board
    Relays of MOSFETS
    Optocouplers
    Diodes (for flyback protection)
    Voltage regulator or DC-DC converter
    Software tools like Platformio

## Interface Board with Car Systems

### Step 1 Powering
    Voltage Regulator: Use a DC-DC buck converter to step down the car's 12V supply to 5V or 3.3v as it suits better

### Connection
    Connect the 12V from the car battery to the input of the DC-DC converter and the 5V output to the boards 5V or 3.3v and GND pins.

## Step 2: Controlling the Hazard Lights

### Locate Hazard Light Circuit:

    Identify the wires or relay connected to the hazard lights. You might need to consult your car’s wiring diagram.

### Design the Circuit
    Relay Control: Use a relay to interface with the hazard lights. The Arduino will control the relay, which in turn switches the 12V power to the hazard lights.

    Optocoupler Isolation: For better safety, use an optocoupler to isolate the Arduino from the car’s high voltage.   
    Circuit

### Circuit:
```
Car 12V ------ Relay (NO) ------ Hazard Lights
                |
               Optocoupler
                |
            Board Pin
```
### code sample:

```
int hazardPin = 8; // Pin connected to the relay

void setup() {
    pinMode(hazardPin, OUTPUT);
}

void loop() {
    digitalWrite(hazardPin, HIGH); // Turn on hazard lights
    delay(1000); // 1 second delay
    digitalWrite(hazardPin, LOW); // Turn off hazard lights
    delay(1000); // 1 second delay
}
```

## Step 3: Interfacing with Door Locks

### Locate Door Lock Actuator
    Find the wires leading to the door lock motor or solenoid. Again, a wiring diagram may be necessary.

### Circuit Design:
    Relay/MOSFET Control: Use a relay or MOSFET to control the high current required by the door lock motor.

    Flyback Diode: Protect the MOSFET or relay from voltage spikes using a flyback diode across the door lock motor.


### Circuit:
```
Car 12V ---- Relay (NO) ---- Door Lock Motor
              |
             MOSFET
              |
          Arduino Pin

```
### code sample:

```
int doorLockPin = 9; // Pin connected to the relay or MOSFET

void setup() {
    pinMode(doorLockPin, OUTPUT);
}

void loop() {
    digitalWrite(doorLockPin, HIGH); // Lock the door
    delay(1000); // 1 second delay
    digitalWrite(doorLockPin, LOW); // Unlock the door
    delay(1000); // 1 second delay
}

```

#### This is only for basic integration with any car model and maker. Canbus, OBDII and others can be implemented, but for now the simply the best! 