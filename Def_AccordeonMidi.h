// Fichier de définition de la carte, Touche mélodies, touches et des variables de touches, etc
// Modif le 13/11/2020

// Carte MEGA
//Pin Midi interne  On utilise le Serial1 avec la Pin TX 18
//Pin Midi externe  On utilise le Serial2 avec la Pin TX 16

//Les Touche mélodies sont en INPUT_PULLUP, HIGH = Off, LOW = On
const int No_PinMidiInExt = 11;      
const int No_PinPanic = 12;
const int No_PinPousseTire = A8;   //A adapter en rotatif sur MEGA  
const int No_PinOctave = A9;      //A adapter en rotatif sur MEGA
const int No_PinInstrument = A10;  //A adapter en rotatif sur MEGA

const int No_Pin = 0;
const int NotePousse = 1;
const int NoteTire = 2;
const int Nb_ToucheMelodie = 27;

const int InstrumentDef = 21;     //Instrument par défaut Accordéon
const int MidiVelocity = 64;      //Vélocité par défaut 0-127
/*
Association des codes MIDI pour le clavier main droite (mélodie)
le premier nombre = le numéro de la Pin
le deuxième nombre = le code midi en poussé
le troisème nombre = le code midi en tiré
*/ 

const int Touche_Melodie[Nb_ToucheMelodie][3] = {

{22,26,30},    // Touche mélodie 1
{24,31,33},    // Touche mélodie 2
{26,35,36},    // Touche mélodie 3
{28,38,40},    // Touche mélodie 4
{30,43,42},    // Touche mélodie 5
{32,47,45},    // Touche mélodie 6
{34,50,48},    // Touche mélodie 7
{36,55,52},    // Touche mélodie 8
{38,59,54},    // Touche mélodie 9
{40,62,57},    // Touche mélodie 10
{42,31,35},    // Touche mélodie 11
{44,36,38},    // Touche mélodie 12
{46,40,41},    // Touche mélodie 13
{48,43,45},    // Touche mélodie 14
{50,48,47},    // Touche mélodie 15
{52,52,50},    // Touche mélodie 16
{31,55,53},    // Touche mélodie 17
{33,60,57},    // Touche mélodie 18
{35,64,59},    // Touche mélodie 19
{37,33,37},    // Touche mélodie 20
{39,39,43},    // Touche mélodie 21
{41,44,44},    // Touche mélodie 22
{43,45,46},    // Touche mélodie 23
{45,51,49},    // Touche mélodie 24
{47,56,55},    // Touche mélodie 25
{49,57,56},    // Touche mélodie 26
{51,51,58}     // Touche mélodie 27
};

/*
pour le clavier main gauche (basses) - chaque touche fait jouer 2 notes en même temps
le premier nombre = le numéro de la Pin
le deuxième nombre = le code midi en poussé pour la première note
le troisème nombre = le code midi en tiré pour la première note
le quatrième nombre = le code midi en poussé pour la deuxième note
le cinquième nombre = le code midi en tiré pour la deuxième note
Les Touche mélodies sont en INPUT, pin analog avec résistance, HIGH = Off, LOW = On*/ 

const int NoteBassePousse = 1;
const int NoteBasseTire = 2;
const int Note2BassePousse = 3;
const int Note2BasseTire = 4;
const int Nb_ToucheBasses = 8;

const int Touche_Basses[Nb_ToucheBasses][5] = {

{A0,38,33,43,38},    // Touche basses 1
{A1,31,26,43,38},    // Touche basses 2
{A2,36,36,43,43},    // Touche basses 3
{A3,24,31,36,43},    // Touche basses 4
{A4,35,33,40,47},    // Touche basses 5
{A5,28,33,40,45},    // Touche basses 6
{A6,36,36,41,41},    // Touche basses 7
{A7,29,29,41,41}};   // Touche basses 8

/* A0 = Pin 54 --- A7 = Pin 61

/*
Tous ces codes midi de base seront pris par défaut au démarrage de l'instrument.
En fonction de la position des codeurs rotatifs, deux variables seront ajoutées à ces codes.
Pour la sélection de la tonalité, on ajoutera une variable allant de -6 à + 6 demi-tons
Pour la sélection de l'octave, on ajoutera une variable de 12 demi-tons (donc 1 seule octave de décalage vers le haut)
*/
// Pin en INPUT_PULLUP
const int ON = LOW ;
const int OFF = HIGH;

// Etat actuel, Etat avant
int Etat_ToucheInstrument[2] =   {InstrumentDef, 0}; // valeur entre 0 et 127
int Etat_ToucheTona[2] =         {0, 0};
int Etat_ToucheOctave[2] =       {0, 0};
int Etat_TouchePanic[2] =        {OFF, OFF};
int Etat_ToucheMidiInExt[2] =    {OFF, OFF};
int Etat_TouchePousseTire[2] =   {OFF, OFF};

const int Etat_actuel = 0;
const int Etat_avant = 1;
const int Note_encours = 2;

int PousseTire;

// Etat actuel, Etat avant, Note en cours
int Etat_Touche_Melodie[Nb_ToucheMelodie][3] = { 
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  {OFF, OFF, 0},
                  };

const int Note2_encours = 3;

// Etat actuel, Etat avant, Note1 en cours, Note2 en cours
int Etat_Touche_Basses[Nb_ToucheBasses][4] = { 
                  {OFF, OFF, 0, 0},
                  {OFF, OFF, 0, 0},
                  {OFF, OFF, 0, 0},
                  {OFF, OFF, 0, 0},
                  {OFF, OFF, 0, 0},
                  {OFF, OFF, 0, 0},
                  {OFF, OFF, 0, 0},
                  {OFF, OFF, 0, 0},
                  };
