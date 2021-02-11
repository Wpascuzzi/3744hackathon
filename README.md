# 3744hackathon

# 3744hackathon Project

This project is a simple game designed for the ATxMega128 microcontroller.

The idea behind the game is two microcontrollers connect to each other utilizing their USART peripherals. Each person plays with one microcontroller. A player is either an attacker or defender. Each player sets up their "field", which is a set of leds they can choose to turn individually on or off. 

Once the players are finished setting up their field, they press a button to lock in their decision. The microcontrollers will send this data to each other, and score based on the data received. Attacackers gain points for having led values not match the values of the defender (their units successfully navigated through enemy defenses, or they caused defenders to waste resources). Defenders gain points for having their led values match the attacker's values (their defensive structures were placed appropriately).

