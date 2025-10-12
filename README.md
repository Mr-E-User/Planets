# A rough multi-planetary simulator
_Note: running this script requires SFML 2.6.1 to be linked_

## Overview
Click on the screen to place a planet with the current configurations and watch the free-bodies interact. The code certainly isn't perfect, but it was a means for me to start learning C++ in high school.

## List of Planet's Controllable Settings:
- Stationary/Moveable: toggled on/off by pressing `t`
  * When objects are initialized as stationary objects, they will not be influenced by the gravity of other bodies, and they will have no initial velocity
  * If the stationary feature is toggled off, planets will have the prescribed initial velocity and will be affected by the gravity of other bodies
- Initial Velocity: click and drag in the direction you wish the planet to travel
  * The white line generated is representative of the velocity the planet will be initialized with
- Mass: hold `Up`/`Right` to increase the mass and hold `Down`/`Left` to decrease the mass
  * `Up`/`Down` will slowly change the mass
  * `Right`/`Left` will change the mass four times faster than `Up`/`Down`
- Radius: hold `Up`/`Right` to increase the radius and hold `Down`/`Left` to decrease the radius
  * `Up`/`Down` will slowly change the radius
  * `Right`/`Lef`t will change the radius four times faster than `Up`/`Down`

## Other Key-Binds
- Pressing `r` will remove all planets
- Pressing `space` will toggle on/off the visualization of the acceleration/velocity vectors of each planet

## Video Example
![screen_recording_planets]("Screen Recording/screen_recording_planets.gif")
