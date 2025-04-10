# Arduino Login System

## Inspiration
We recently got an Arduino kit but had not gotten the chance to try it out yet, so we wanted to gain experience with Arduino at this hackathon.

## What it does
This project is a login system. We allow users to attempt to log in using a keypad. The passkey is 4 digits, and we store the hash of this passkey on the Arduino. The admin user can also log in to the system by scanning a card. Once the admin is logged into the system, they can also change the passkey used by the regular user. Additionally, prompts are displayed on the Arduino when various actions happen. For example, it displays prompts to enter a password when the user attempts to log in or the admin attempts to change the password. It also gives a message saying that access was granted or denied after an attempted login.

## How we built it
For the hardware side, we built the circuit using the Arduino and a breadboard to connect the Arduino, keypad, card scanner, and display together. For the software side, we wrote a program in C++ that we loaded onto the Arduino. This program is responsible for adding functionality to the Arduino parts, such as the keypad, card scanner, and display. This program also handles the logic behind the login system, such as authenticating users. It is responsible for keeping track of state, such as whether the current user logged in is a regular or admin user, or if no one is currently logged in. For validating the passkey, we made a hashing algorithm. When the admin user sets the passkey, a hash is generated for that passkey, and that hash is stored on the Arduino's persistent memory. When the user attempts to log in, they type in a 4-digit passkey. A hash is generated based on that passkey, and it is compared against the hash stored on the Arduino. If both hashes match, then the user is granted access; otherwise, the system will display a message telling them that their access was denied. The admin can log in using a key card, which stores data in the form of an array of blocks. Block 0 contains the UID for the card, and we programmed the Arduino to only accept a particular UID. If the card's UID matches the UID the Arduino expects, the user is granted access with admin privileges and will now have the ability to change the regular user's password.

## Challenges we ran into
Since we had no experience with Arduino before this, it was difficult to come up with a project idea as we were unsure of what you could do with Arduino. There was a decent amount of exploration we had to do because of this. Another challenge was that we accidentally fried the original scanner part by plugging in our Arduino to the wall. This was an issue because the wall supplied higher voltage than when we connected to our PCs. We did not put in any resistors when connecting the scanner, and because it only takes 3.3V, we believe this is the reason that it got fried.

## Accomplishments that we're proud of
We are proud of our final product, as we got a basic login system to work for both the regular and admin users. Additionally, we are proud that we gained experience with Arduino and had a lot of fun making this project.

## What we learned
We learned about many of the uses of Arduino and several interesting features we can make by combining the hardware of the Arduino circuit with a C++ program that we can run on the Arduino. We also learned more about circuits in this process, especially the value of using resistors!

## What's next for Arduino Login System
We want to add more features, such as being able to perform tasks as a user when logged in. Additionally, to make the login system more secure, we would want to allow for a stronger password and implement a better hashing algorithm to prevent brute force login attempts.
