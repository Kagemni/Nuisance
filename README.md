## Nuisance

When prompted with the concept of **Useless Inventions**, and the slight delay from procrastinating the brainstorming process of our idea, we suddenly felt very motivated to make a little friend to help us. Introducing **Nuisance**. A (not so friendly) Bot that will sense when you have given him your phone. Promptly running away, and screaming if you get too close. An interesting take on the game of manhunt.

Check out the project on Devpost! [HERE](https://devpost.com/software/nuisance)

# Overview

Nuisance is a fun hack where the presence of a phone, or object, is detected using a force sensor. Once detected, our lovable friend embarks on an adventure in a game of keepaway. However, there's a catch. 
This isn't your normal game of keepaway. If daring enough to approach, Nuisance will scream, elliciting sounds of horror


# How we built it
- Arduino Due
- 2 wheels
- Caster Wheel
- H-bridge/ Motor Driver
- Motors
- 2 UltraSonic Sensors
- Noise Sound Audio Sensor
- PIR Motion sensor
- 1 Grove Buzzer v1.2
- large breadboard
- 2 small breadboard
- OCD display 
- Force sensor
- 9V battery
- 3*1.5 = 4.5 V battery
- A bit of wires
and a **lot** of cardboard

*and some software*


# How to integrate
N/a?

## Challenges
We had an issue during the debugging phase of our code regarding the Ultrasonic Sensors. No matter what was done, they just seemed to constantly be timing out. After looking extensively into the issue, we figured out that the issue was neither hardware nor software related. The breadboard had sporadic faulty pins that we had to be considerate of. Thus causing us to test the rest of the breadboard for integrity.

We also had issues with the collision detection algorithm at first. However, with a lil tenacity, and the power of friendship, you too can solve this problem. We originally had the wrong values being processed, causing out algorithm to disregard the numbers we required to gauge distance accurately.

Accomplishments that we're proud of
## Accomplishments that we're proud of
High fives to the team 🙌, as we ...

## What we learned
- wee woo wee woo

## Collaborators
This project was made possible by the collaboration of [EemanAleem](https://github.com/EemanAleem), [aseelelwasila](https://github.com/aseelelwasila), and [missingscythe](https://github.com/missingscythe).


## Installation
Required installations: Arduino IDE, or VSCode extension capable of running code on Arduino's.


## Licenses
No licenses
