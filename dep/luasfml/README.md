luaSFML
=======

luaSFML is a lua binding of SFML, which let's you use SFML in your lua script. luaSFML attempts to be as compatible with SFML as possible, since lua is limited in terme of raw pointer and low level capabilities.


Naming Convention
=================

There is no documentation at the moment. So if you want to learn how to use luaSFML you will first need to learn C++ SFML version. 

To create a new instance of SFML object call the new function of the object. The class are bind to lua by concatenate sf namespace and class name. Ex:  sf::Texture is bing under the name of sfTexture.
Example to create a new sf::Texture call the new static function of sfTexture object like this :
myTexture = stTexture.new() 

to learn more about luaSFML take a look at demos. 


Binding Completude
==================

At the moment the binding is not complete. It's not a issue of feasability it's a issue of time. I've write this to write game quickly with SFML. 
But is enough advance to let you write your game. Only hight level feature are not bind like network, 3D sound spatialisation, renderTexture, shader ,vertex array and thread. 
The completude of this binding will evolve with my SFML need for my game dev. I'm sure the graphical binding will be the first to be completed.  


I've also added some object I need like tilemap. 


Building luaSFML
================

To build luaSFML you will need CMake 2.8 or higher, SFML 2.0 or higher, lua 5.1 and C++11
You may need to edit CMakeList.txt in order to specify path to SFML and lua. 

Let me know if you have encounter a bug or if you need anything. 

