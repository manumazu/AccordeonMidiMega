// Fichier de définition de la carte, Touche mélodies, touches et des variables de touches, etc
//

// Carte MEGA
//Pin Midi interne  On utilise le Serial1 avec la Pin TX 18
//Pin Midi externe  On utilise le Serial2 avec la Pin TX 16

//Les Touche mélodies sont en INPUT_PULLUP, HIGH = OFF, LOW = ON
const int ON = LOW ;
const int OFF = HIGH;

const int No_PinMidiInExt = 19;
const int No_PinPanic = 15;
const int No_PinMute = 53;
const int No_PinTierceOnOff = A0;
const int No_PinBassesProf = A1;

const int No_PinVolume = A14;     //Analogique A adapter après choix mécanisme pousse-tire et volume
const int No_PinPousseTire = A15;   //A adapter après choix mécanisme pousse-tire et vélocité

const int No_Pin = 0;

const int Nb_ToucheMelodie = 27;

const int InstrumentDef = 0;      //Instrument par défaut Accordéon 1ere bank tsunami
const int InstrumentExtDef = 21;  //Instrument par défaut Accordéon Midi Externe
const byte ChannelMelodie = 0;    // Channel pour les touches Mélodies
const byte ChannelBasses = 1;     // Channel pour les touches Mélodies
const byte ChannelAccord = 2;     // Channel pour les touches Mélodies
const int MidiVelocity = 64;      //Vélocité constante par défaut (1 à 127)
const int Midi_Interne = OFF;     //Etat initial du bouton Midi Interne/Externe sur Interne au repos
const int Midi_Int = 0;
const int Midi_Ext = 1;
/*
  Association des codes MIDI pour le clavier main droite (mélodie)
  le premier nombre = le numéro de la Pin
  le deuxième nombre = le code midi Basson en poussé
  le troisème nombre = le code midi Basson en tiré
  le quatrième nombre = le code midi Flûte en poussé
  le cinquième nombre = le code midi Flûte en tiré
*/

const int Touche_Melodie[Nb_ToucheMelodie][5] = {
  {22, 50, 54, 62, 66},  // Touche mélodie 1 Rangée extérieure
  {24, 55, 57, 67, 69},  // Touche mélodie 2
  {26, 59, 60, 71, 72},  // Touche mélodie 3
  {28, 62, 64, 74, 76},  // Touche mélodie 4
  {30, 67, 66, 79, 78},  // Touche mélodie 5
  {32, 71, 69, 83, 81},  // Touche mélodie 6
  {34, 74, 72, 86, 84},  // Touche mélodie 7
  {36, 79, 76, 91, 88},  // Touche mélodie 8
  {38, 83, 78, 95, 90},  // Touche mélodie 9
  {40, 86, 81, 98, 93},  // Touche mélodie 10
  {42, 55, 59, 67, 71},  // Touche mélodie 11 Rangée du milieu
  {44, 60, 62, 72, 74},  // Touche mélodie 12
  {46, 64, 65, 76, 77},  // Touche mélodie 13
  {48, 67, 69, 79, 81},  // Touche mélodie 14
  {50, 72, 71, 84, 83},  // Touche mélodie 15
  {52, 76, 74, 88, 86},  // Touche mélodie 16
  {31, 79, 77, 91, 89},  // Touche mélodie 17
  {33, 84, 81, 96, 93},  // Touche mélodie 18
  {35, 88, 83, 100, 95}, // Touche mélodie 19
  {37, 57, 61, 69, 73},  // Touche mélodie 20 Rangée intérieure
  {39, 63, 67, 75, 79},  // Touche mélodie 21
  {41, 68, 68, 80, 80},  // Touche mélodie 22
  {43, 69, 70, 81, 82},  // Touche mélodie 23
  {45, 75, 73, 87, 85},  // Touche mélodie 24
  {47, 80, 79, 92, 91},  // Touche mélodie 25
  {49, 81, 80, 93, 92},  // Touche mélodie 26
  {51, 87, 82, 99, 94}   // Touche mélodie 27
};

/*
  pour le clavier main gauche (basses et accord) - chaque touche fait jouer 3 notes en même temps,
  la tierce est supprimé suivant état du bouton TierceOnOff pour les accords
  le premier nombre = l'adresse de la touche sur les Mux
  le deuxième nombre = le code midi en poussé pour la première note
  le troisième nombre = le code midi en tiré pour la première note
  le quatrième nombre = le code midi en poussé pour la deuxième note (la tierce)
  le cinquième nombre = le code midi en tiré pour la deuxième note (la tierce)
  le sixième nombre = le code midi en poussé pour la troisième note
  le septième nombre = le code midi en tiré pour la troisième note
  Les Touches basses/accord sont géré par les multiplexeurs, HIGH = Off, LOW = On */

const int Nb_ToucheAccord = 6;

const int Touche_Accord[Nb_ToucheAccord][7] = {

  {0, 80, 83, 84, 86, 87, 90}, // Touche accord 28
  {1, 79, 74, 83, 77, 86, 81}, // Touche accord 30
  {2, 72, 79, 76, 83, 79, 86}, // Touche accord 32
  {3, 75, 82, 78, 86, 82, 89}, // Touche accord 34
  {4, 76, 76, 79, 80, 83, 83}, // Touche accord 36
  {5, 77, 77, 81, 81, 84, 84}, // Touche accord 38
};

const int NoteBassePousse = 1;
const int NoteBasseTire = 2;
const int Note2BassePousse = 3;
const int Note2BasseTire = 4;
const int Note3BassePousse = 5;
const int Note3BasseTire = 6;
const int Nb_ToucheBasses = 6;

const int Touche_Basses[Nb_ToucheBasses][7] = {

  {0, 56, 59, 68, 71, 80, 83}, // Touche basses 29
  {1, 55, 50, 67, 62, 79, 74}, // Touche basses 31
  {2, 48, 55, 60, 67, 72, 79}, // Touche basses 33
  {3, 51, 58, 63, 70, 75, 82}, // Touche basses 35
  {4, 52, 57, 64, 69, 76, 81}, // Touche basses 37
  {5, 53, 53, 65, 65, 77, 77} // Touche basses 39
};

// A0 = Pin 54 --- A11 = Pin 65

/*
  Tous ces codes midi de base seront pris par défaut au démarrage de l'instrument.
  En fonction de la position des codeurs rotatifs, deux variables seront ajoutées à ces codes.
  Pour la sélection de la tonalité, on ajoutera une variable allant de -6 à + 6 demi-tons
  Pour la sélection de l'octave, on ajoutera une variable de 12 demi-tons (donc 1 seule octave de décalage vers le haut)
*/

// Etat actuel, Etat avant
int Etat_ToucheInstrument[2] =   {InstrumentDef, 0}; // valeur entre 0 et 127
int Etat_ToucheTona[2] =         {0, 0};
int Etat_ToucheOctave[2] =       {0, 0};
int Etat_ToucheRenvAccord[2] =   {0, 0};
int Etat_ToucheTierceOnOff[2] =  {OFF, OFF};
int Etat_TouchePanic[2] =        {OFF, OFF};
int Etat_ToucheMidiInExt[2] =    {OFF, OFF};
int Etat_TouchePousseTire[2] =   {OFF, OFF};
int Etat_ToucheVolume[2] =     {0, 0};
int Etat_ToucheBassesProf[2] =   {OFF, OFF};
int Etat_ToucheMidiMSB[2] =      {0, 0};         // Midi Bank MSB valeur entre 0 et 127
int Etat_ToucheMidiLSB[2] =      {0, 0};         // Midi Bank LSBvaleur entre 0 et 127
int Etat_ToucheInstrumentE[2] =  {InstrumentExtDef, 0}; // valeur entre 0 et 127
int Etat_ToucheRegistre[2] =  {0, 0};            // valeur 0 Basson, 1 Flüte+Basson, 2 Flûte
int PinMux1Basson = 6;
int PinMux1FluteBasson = 7;
int PinMux2Flute = 6;
int RegistreBasson = 0;
int RegistreFluteBasson = 1;
int RegistreFlute = 2;

const String Libelle_Registre[3] = {             // Ecran LCD
  {"BA   "},
  {"BA/FL"},
  {"FL   "}
};

const String Libelle_Tona[13] = {                 // Libellé Tona de -6 à +6
  {"DO#/FA#  (-6)"},
  {"RE/SOL   (-5)"},
  {"RE#/SOL# (-4)"},
  {"MI/LA    (-3)"},
  {"FA/LA#   (-2)"},
  {"FA#/SI   (-1)"},
  {"SOL/DO    (0)"},
  {"SOL#/DO# (+1)"},
  {"LA/RE    (+2)"},
  {"LA#/RE#  (+3)"},
  {"SI/MI    (+4)"},
  {"DO/FA    (+5)"},
  {"DO#/FA#  (+6)"}
};

const int Etat_actuel = 0;
const int Etat_avant = 1;
const int Note1_encours = 2;
const int Note2_encours = 3;
const int Note3_encours = 4;


int PousseTire;

// Etat actuel, Etat avant, Note1 en cours Basson, Note2 en cours Flûte
int Etat_Touche_Melodie[Nb_ToucheMelodie][4] = {
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
  {OFF, OFF, 0, 0},
};

// Etat actuel, Etat avant, Note1 en cours, Note2 en cours, Note3 en cours
int Etat_Touche_Basses[Nb_ToucheBasses][5] = {
  {OFF, OFF, 0, 0, 0},
  {OFF, OFF, 0, 0, 0},
  {OFF, OFF, 0, 0, 0},
  {OFF, OFF, 0, 0, 0},
  {OFF, OFF, 0, 0, 0},
  {OFF, OFF, 0, 0, 0},
};

int Etat_Touche_Accord[Nb_ToucheAccord][5] = {
  {OFF, OFF, 0, 0, 0},
  {OFF, OFF, 0, 0, 0},
  {OFF, OFF, 0, 0, 0},
  {OFF, OFF, 0, 0, 0},
  {OFF, OFF, 0, 0, 0},
  {OFF, OFF, 0, 0, 0},
};
