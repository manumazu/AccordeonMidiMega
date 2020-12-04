// Fichier de définition de la carte, Touche mélodies, touches et des variables de touches, etc
//

// Carte MEGA
//Pin Midi interne  On utilise le Serial1 avec la Pin TX 18
//Pin Midi externe  On utilise le Serial2 avec la Pin TX 16

//Les Touche mélodies sont en INPUT_PULLUP, HIGH = Off, LOW = On
const int No_PinMidiInExt = 11;      
const int No_PinPanic = 12;
const int No_PinTierceOnOff = A12;   
const int No_PinPousseTire = A13;   //A adapter après choix mécanisme pousse-tire et vélocité  

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

{22,50,54},    // Touche mélodie 1 Rangée extérieure
{24,55,57},    // Touche mélodie 2
{26,59,60},    // Touche mélodie 3
{28,62,64},    // Touche mélodie 4
{30,67,66},    // Touche mélodie 5
{32,71,69},    // Touche mélodie 6
{34,74,72},    // Touche mélodie 7
{36,79,76},    // Touche mélodie 8
{38,83,78},    // Touche mélodie 9
{40,86,81},    // Touche mélodie 10
{42,55,59},    // Touche mélodie 11 Rangée du milieu
{44,60,62},    // Touche mélodie 12
{46,64,65},    // Touche mélodie 13
{48,67,69},    // Touche mélodie 14
{50,72,71},    // Touche mélodie 15
{52,76,74},    // Touche mélodie 16
{31,79,77},    // Touche mélodie 17
{33,84,81},    // Touche mélodie 18
{35,88,83},    // Touche mélodie 19
{37,57,61},    // Touche mélodie 20 Rangée intérieure
{39,63,67},    // Touche mélodie 21
{41,68,68},    // Touche mélodie 22
{43,69,70},    // Touche mélodie 23
{45,75,73},    // Touche mélodie 24
{47,80,79},    // Touche mélodie 25
{49,81,80},    // Touche mélodie 26
{51,87,82}     // Touche mélodie 27
};

/*
pour le clavier main gauche (basses) - chaque touche fait jouer 3 notes en même temps, 
la tierce est supprimé suivant état du bouton TierceOnOff
le premier nombre = le numéro de la Pin
le deuxième nombre = le code midi en poussé pour la première note
le troisième nombre = le code midi en tiré pour la première note
le quatrième nombre = le code midi en poussé pour la deuxième note (la tierce)
le cinquième nombre = le code midi en tiré pour la deuxième note (la tierce)
le sixième nombre = le code midi en poussé pour la troisième note
le septième nombre = le code midi en tiré pour la troisième note
Les Touches basses sont en INPUT_PULLUP, pin analog utilisée en digital, HIGH = Off, LOW = On*/ 

const int NoteBassePousse = 1;
const int NoteBasseTire = 2;
const int Note2BassePousse = 3;
const int Note2BasseTire = 4;
const int Note3BassePousse = 5;
const int Note3BasseTire = 6;
const int Nb_ToucheBasses = 12;

const int Touche_Basses[Nb_ToucheBasses][7] = {

{A0,80,83,84,86,87,90},    // Touche accord 28
{A1,56,59,68,71,80,83},    // Touche basses 29  
{A2,79,74,83,77,86,81},    // Touche accord 30
{A3,55,50,67,62,79,74},    // Touche basses 31
{A4,72,79,76,83,79,86},    // Touche accord 32
{A5,48,55,60,67,72,79},    // Touche basses 33
{A6,75,82,78,86,82,89},    // Touche accord 34
{A7,51,58,63,70,75,82},    // Touche basses 35
{A8,76,76,79,80,83,83},    // Touche accord 36
{A9,52,57,64,69,76,81},    // Touche basses 37
{A10,77,77,81,81,84,84},   // Touche accord 38
{A11,53,53,65,65,77,77}    // Touche basses 39
};  

/* A0 = Pin 54 --- A11 = Pin 65

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
int Etat_ToucheTierceOnOff[2] =  {OFF, OFF};
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

const int Note1_encours = 2;
const int Note2_encours = 3;
const int Note3_encours = 4;

// Etat actuel, Etat avant, Note1 en cours, Note2 en cours, Note3 en cours
int Etat_Touche_Basses[Nb_ToucheBasses][5] = { 
                  {OFF, OFF, 0, 0, 0},
                  {OFF, OFF, 0, 0, 0},
                  {OFF, OFF, 0, 0, 0},
                  {OFF, OFF, 0, 0, 0},
                  {OFF, OFF, 0, 0, 0},
                  {OFF, OFF, 0, 0, 0},
                  {OFF, OFF, 0, 0, 0},
                  {OFF, OFF, 0, 0, 0},
                  {OFF, OFF, 0, 0, 0},
                  {OFF, OFF, 0, 0, 0},
                  {OFF, OFF, 0, 0, 0},
                  {OFF, OFF, 0, 0, 0},
                  };
