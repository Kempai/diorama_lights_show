#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "images.h" // Inclut les bitmaps prédéfinis

// Déclarations pour l'écran OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C // Adresse de l'écran OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Déclarations des broches
#define BUTTON_PIN 5  // Broche pour le bouton qui contrôle les LEDs
#define BRIGHTNESS_BUTTON_PIN 6  // Broche pour le bouton qui contrôle la luminosité
#define LED_FRONT_PINS {13, 12, 32, 27, 26, 25}  // Pins pour les LEDs frontales
#define LED_BACK_PINS {33, 14, 23}  // Pins pour les LEDs arrière
#define BATSIGNAL_PIN 19  // Pin pour le BatSignal
#define BAT_SIGNAL_BUTTON_PIN 18  // Pin pour le bouton du BatSignal

// Variables pour la gestion des LEDs et du BatSignal
bool ledsState = false;  // État des LEDs (allumées ou éteintes)
bool batSignalState = false;  // État du BatSignal (allumé ou éteint)

const int niveauxPWM[] = {0, 51, 102, 153, 204}; // Niveaux de luminosité (0 à 255)
int currentBrightnessIndex = 0;                    // Index actuel de la luminosité

unsigned long dernierTempsBouton = 0;       // Dernière fois que le bouton a été pressé
const unsigned long debounceDelay = 300;     // Anti-rebond (300ms)
unsigned long dernierTempsLuminosité = 0;    // Dernier temps où le pourcentage de luminosité a été affiché
const unsigned long luminositéDuration = 2000; // Temps d'affichage de la luminosité (2 secondes)

bool startupCompleted = false;  // Flag pour signaler la fin de la séquence de démarrage
bool showBrightness = false;   // Flag pour indiquer si on affiche la luminosité ou pas

unsigned long dernierTempsImage = 0;  // Dernier temps de changement d'image
const unsigned long imageInterval = 3000;  // Intervalle pour changer d'image (3s)
int currentImageIndex = 0;  // Index de l'image à afficher

// Déclaration globale de ledBack[]
int ledBack[] = LED_BACK_PINS;

// Fonction pour appliquer la luminosité sur les LEDs
void setLedsBrightness(int brightness) {
  int ledFront[] = LED_FRONT_PINS;
  int ledBack[] = LED_BACK_PINS;

  // Appliquer la luminosité sur les LEDs frontales
  for (int i = 0; i < 6; i++) {
    analogWrite(ledFront[i], brightness);  // Utilisation de PWM pour ajuster la luminosité
  }

  // Appliquer la luminosité sur les LEDs arrière
  for (int i = 0; i < 3; i++) {
    analogWrite(ledBack[i], brightness);  // Utilisation de PWM pour ajuster la luminosité
  }
}

// Fonction pour contrôler le BatSignal
void controlBatSignal(bool state) {
  digitalWrite(BATSIGNAL_PIN, state ? HIGH : LOW);  // Allume ou éteint le BatSignal
}

// Fonction pour allumer ou éteindre les LEDs frontales et arrière
void controlLeds(bool state) {
  int ledFront[] = LED_FRONT_PINS;
  int ledBack[] = LED_BACK_PINS;

  // Allume/éteint les LEDs frontales
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledFront[i], state ? HIGH : LOW);
  }

  // Allume/éteint les LEDs arrière
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledBack[i], state ? HIGH : LOW);
  }
}

// Fonction pour afficher une image sur l'écran OLED
void displayImage(const uint8_t *bitmap) {
  display.clearDisplay();                       // Effacer l'écran avant d'afficher une nouvelle image
  display.drawBitmap(0, 0, bitmap, 128, 32, 1); // Afficher l'image
  display.display();                            // Mettre à jour l'écran
}

void startupSequence() {
  // Afficher les images séquentiellement
  displayImage(epd_bitmap_kagepai);
  delay(2000);
  displayImage(epd_bitmap_Hoka_feat_Kempai);
  delay(2000);
  displayImage(epd_bitmap_present);
  delay(2000);

  // Allumer progressivement les LEDs ensemble
  for (int i = 0; i <= 255; i++) {  // 255 est la luminosité maximale
    for (int j = 0; j < 3; j++) {
      analogWrite(ledBack[j], i);  // Augmenter progressivement la luminosité de chaque LED
    }
    delay(10);  // Ajustez la vitesse de l'effet (plus petit = plus rapide)
  }

  // Allumer les LEDs avant une par une
  int ledFront[] = LED_FRONT_PINS;
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledFront[i], HIGH);
    delay(300);
  }

  // Faire clignoter le BatSignal 4 fois
  for (int i = 0; i < 3; i++) {
    digitalWrite(BATSIGNAL_PIN, HIGH);
    delay(500);
    digitalWrite(BATSIGNAL_PIN, LOW);
    delay(500);
  }

  startupCompleted = true;  // Marquer la fin de la séquence de démarrage
}

void setup() {
  // Initialisation des pins
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Bouton pour contrôler les LEDs
  pinMode(BAT_SIGNAL_BUTTON_PIN, INPUT_PULLUP);  // Bouton pour le BatSignal

  int ledFront[] = LED_FRONT_PINS;
  int ledBack[] = LED_BACK_PINS;

  // Initialisation de l'écran OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Échec de l'initialisation de l'écran OLED !");
    while (true);
  }
  display.clearDisplay();
  display.display();

  // Initialisation des broches des LEDs
  for (int i = 0; i < 6; i++) {
    pinMode(ledFront[i], OUTPUT);
  }
  for (int i = 0; i < 3; i++) {
    pinMode(ledBack[i], OUTPUT);
  }

  // Initialisation du BatSignal (LED)
  pinMode(BATSIGNAL_PIN, OUTPUT);
  digitalWrite(BATSIGNAL_PIN, LOW);  // Éteindre le BatSignal au début

  // Exécuter la séquence de démarrage
  startupSequence();
}

void loop() {
  unsigned long maintenant = millis();

  // Lecture de l'état du bouton pour les LEDs
  if (digitalRead(BUTTON_PIN) == LOW) {  // Le bouton est pressé
    if (millis() - dernierTempsBouton > debounceDelay) {  // Anti-rebond
      dernierTempsBouton = millis();
      // Augmenter la luminosité en cycle, après 100% on revient à 0%
      currentBrightnessIndex = (currentBrightnessIndex + 1) % 5;  // Limité à 5 pour 100%

      // Appliquer la nouvelle luminosité à toutes les LEDs
      setLedsBrightness(niveauxPWM[currentBrightnessIndex]);

      // Afficher la luminosité pendant 2 secondes
      showBrightness = true;
      dernierTempsLuminosité = millis(); // Réinitialiser le temps d'affichage de la luminosité
    }
    delay(200);  // Anti-rebond
  }

  // Lecture de l'état du bouton pour le BatSignal
  if (digitalRead(BAT_SIGNAL_BUTTON_PIN) == LOW) {  // Le bouton BatSignal est pressé
    batSignalState = !batSignalState;  // Inverser l'état du BatSignal
    controlBatSignal(batSignalState);  // Appliquer l'état du BatSignal
    delay(200);  // Anti-rebond
  }

  // Si on doit afficher la luminosité, on l'affiche et on attend 2 secondes
  if (showBrightness) {
    display.clearDisplay();  // Effacer l'écran
    display.setTextSize(2);   // Texte plus grand
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(40, 10);  // Centrer le texte
    display.print(currentBrightnessIndex * 25);  // Affiche le % de luminosité
    display.print("%");
    display.display();

    if (millis() - dernierTempsLuminosité >= luminositéDuration) {
      // Reprendre l'affichage des images après 2 secondes
      showBrightness = false;
    }
  } else {
    // **Affichage des images** toutes les 3 secondes
    if (millis() - dernierTempsImage > imageInterval) {
      dernierTempsImage = millis();  // Réinitialiser le temps
      displayImage(epd_bitmap_allArray[currentImageIndex]); // Affiche l'image actuelle
      currentImageIndex = (currentImageIndex + 1) % 16;      // Passe à l'image suivante
      if (currentImageIndex < 3) currentImageIndex = 3;     // Boucle sur les images 
      }
    }
  }
  
  