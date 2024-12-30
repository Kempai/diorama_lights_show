# diorama_lights_show

FR version (for english see above)  : 

Ceci est un projet d'illumination d'un diorama réalisé pour un diorama de Batman du studio Sanix3D.

https://sanix3d.com/BATMAN-Diorama-STL-Files-for-3D-printing-p97678753

Il a été réalisé conjointement avec mon frère qui s'est occupé de l'impression 3D (FDM pour le décor et résine pour le modèle) et de la peinture.
Le modèle prévoit des emplacements pour leds, mais ce n'etait pas suffisant pour nous, nous avons souhaitez apporter une touche de fun dans ce projet en allant plus loin.
Peut-être ce code permettra d'aider ou de donner des idées pour un projet similaire ou détourné.

Il s'agit d'un programme pour ESP32 qui pilote 10 leds en 12V, controlés via des transistors 2N2222 : 2 rangées de LEDS, une led indépendante pour le bat signal ainsi un écran oled 128x32 affichant les portraits des grand méchant du comics. J'ai édité le modèle 3D du haut du batiment pour y intégrer l'écran oled (le fichier est joint).
Il emploie les librairies suivantes : 
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

Il consiste en une animation de démarrage affichant le nom de notre studio de création, suivit d'une séquence d'animation des leds. Puis l'ensemble des leds est ensuite pilotable via des boutons, l'écran lui fait défiler les portraits des grands méchants du comic.
Fonctions :
- Une séquence d'animation sympa lors du démarrage
- Un bouton permettant de contrôler les leds de 0%, 25, 50, 75 à 100%
- Ecran OLED : Un affichage des portraits des grands méchants en lecture en boucle, l'affichage est interrompu pour afficher les % de luminosité lorsque le bouton est pressé
- Un bouton pour allumer et éteindre le Batsignal (il clignote lorsqu'on reste appuyé sur le bouton)

Matériel requis :

- 1x ESP32 (ici un WROOM32)
- 10x LEDS 12V
- 10x transistors 2n2222
- 10x résistances 1K ohm
- 1x buckdown converter 12v>5V
- 2x boutons poussoirs
- 1x oled screen 128x32
- des câbles
- gaines thermorétractables
- fer a souder


This is a diorama illumination project created for a Batman diorama from the Sanix3D studio
UK version : 

This is a diorama illumination project created for a Batman diorama by Sanix3D studio.

https://sanix3d.com/BATMAN-Diorama-STL-Files-for-3D-printing-p97678753

It was created jointly with my brother, who took care of the 3D printing (FDM for the scenery and resin for the model) and painting.
The model includes slots for LEDs, but that wasn't enough for us, so we wanted to add a touch of fun to the project by going a step further.
Perhaps this code will help or give ideas for a similar or different project.

It's a program for ESP32 that drives 10 12V LEDs, controlled via 2N2222 transistors: 2 rows of LEDs, an independent led for the bat signal and a 128x32 oled screen displaying portraits of the comic-book villains. I've edited the 3D model of the top of the building to integrate the oled screen (the file is attached).
It uses the following libraries: 
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

It consists of a startup animation displaying the name of our design studio, followed by a sequence of LED animations. All the leds can then be controlled via buttons, while the screen scrolls through the portraits of the comic's great villains.
Features :
- A cool animation sequence on start-up
- A button to control LEDs from 0%, 25, 50, 75 to 100%.
- OLED screen: a looping display of the big villains' portraits, interrupted to show % brightness when the button is pressed.
- A button for switching the Batsignal on and off (it flashes when the button is held down)

Hardware requirements:

- 1x ESP32 (here a WROOM32)
- 10x 12V LEDS
- 10x transistors 2n2222
- 10x 1K ohm resistors
- 1x buckdown converter 12v>5V
- 2x pushbuttons
- 1x oled screen 128x32
- cables
- heat-shrink tubing
- soldering iron
