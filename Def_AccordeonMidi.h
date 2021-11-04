// Fichier de définition de la carte, Touche mélodies, touches et des variables de touches, etc
//

// Carte MEGA
//Pin Midi interne  On utilise le Serial1 avec la Pin TX 18
//Pin Midi externe  On utilise le Serial2 avec la Pin TX 16

//Les Touche mélodies sont en INPUT_PULLUP, HIGH = OFF, LOW = ON
const int ON = LOW ;
const int OFF = HIGH;
const int No_PinMuxTierceOnOff = 8;
const int No_PinMuxBassesProf = 9;
const int No_PinMuxMidiInExt = 10;
const int No_PinMuxPanic = 11;
const int No_PinMute = A14;

const int No_Pin = 0;

const int Nb_ToucheMelodie = 27;

const int InstrumentDef = 0;      //Instrument par défaut Accordéon 1ere bank tsunami
const int InstrumentExtDef = 21;  //Instrument par défaut Accordéon Midi Externe
const byte ChannelMelodie = 0;    // Channel pour les touches Mélodies
const byte ChannelBasses = 1;     // Channel pour les touches Mélodies
const byte ChannelAccord = 2;     // Channel pour les touches Mélodies
const int Midi_Interne = OFF;     //Etat initial du bouton Midi Interne/Externe sur Interne au repos
const int Midi_Int = 0;
const int Midi_Ext = 1;
const int Defaut_Midi_Volume = 120;
const int Defaut_Midi_Expression = 80;
const int Defaut_Midi_Velocity = 64;
int MidiVelocity = Defaut_Midi_Velocity;        //Vélocité constante par défaut (1 à 127)
// Si MIDIEXT_EXPRESSION
int MidiExtVelocity = Defaut_Midi_Expression;   //Vélocité Ext constante par défaut (1 à 127)
int MidiExtVolume = Defaut_Midi_Volume;         //Volume Ext constante par défaut (1 à 127)
// Si TSUNAMI_SERIALCONTROL
const int Defaut_Tsu_Gain = 0;                  //Gain Tsunami par défaut (-70 à +4)

int Volume_EnCours_Midi;
int Expression_EnCours_Midi;
int Gain_EnCours_Tsunami;
const int Const_Volume_VarMax_Midi = 10;
const int Const_Expression_VarMax_Midi = 5;
const int Const_Gain_VarMax_Tsunami = 2;

/*
  Association des codes MIDI pour le clavier main droite (mélodie)
  le premier nombre = le numéro de la Pin
  le deuxième nombre = le code midi Basson en poussé
  le troisème nombre = le code midi Basson en tiré
  le quatrième nombre = le code midi Flûte en poussé
  le cinquième nombre = le code midi Flûte en tiré
*/

const int Touche_Melodie[Nb_ToucheMelodie][5] = {
  {25, 50, 54, 62, 66},  // Touche mélodie m1  Rangée extérieure en haut
  {26, 55, 57, 67, 69},  // Touche mélodie m2
  {30, 59, 60, 71, 72},  // Touche mélodie m3
  {32, 62, 64, 74, 76},  // Touche mélodie m4
  {35, 67, 66, 79, 78},  // Touche mélodie m5
  {37, 71, 69, 83, 81},  // Touche mélodie m6
  {38, 74, 72, 86, 84},  // Touche mélodie m7
  {43, 79, 76, 91, 88},  // Touche mélodie m8
  {44, 83, 78, 95, 90},  // Touche mélodie m9
  {49, 86, 81, 98, 93},  // Touche mélodie m10 Rangée extérieure en bas
  {23, 55, 59, 67, 71},  // Touche mélodie m11 Rangée du milieu en haut
  {24, 60, 62, 72, 74},  // Touche mélodie m12
  {29, 64, 65, 76, 77},  // Touche mélodie m13
  {33, 67, 69, 79, 81},  // Touche mélodie m14
  {34, 72, 71, 84, 83},  // Touche mélodie m15
  {39, 76, 74, 88, 86},  // Touche mélodie m16
  {41, 79, 77, 91, 89},  // Touche mélodie m17
  {45, 84, 81, 96, 93},  // Touche mélodie m18
  {46, 88, 83, 100, 95}, // Touche mélodie m19 Rangée du milieu en bas
  {22, 57, 61, 69, 73},  // Touche mélodie m20 Rangée intérieure en haut
  {27, 63, 67, 75, 79},  // Touche mélodie m21
  {28, 68, 68, 80, 80},  // Touche mélodie m22
  {31, 69, 70, 81, 82},  // Touche mélodie m23
  {36, 75, 73, 87, 85},  // Touche mélodie m24
  {40, 80, 79, 92, 91},  // Touche mélodie m25
  {42, 81, 80, 93, 92},  // Touche mélodie m26
  {47, 87, 82, 99, 94}   // Touche mélodie m27 Rangée intérieure en bas
};

/*
  pour le clavier main gauche (basses et accord) - chaque touche fait jouer 3 notes en même temps,
  la tierce est supprimé suivant état du bouton TierceOnOff pour les accords
  le premier nombre = l'adresse de la touche sur les Mux
  le deuxième nombre = le code midi en poussé pour la première note (la basse profonde pour les basses)
  le troisième nombre = le code midi en tiré pour la première note (la basse profonde pour les basses)
  le quatrième nombre = le code midi en poussé pour la deuxième note (la tierce pour les accords)
  le cinquième nombre = le code midi en tiré pour la deuxième note (la tierce pour les accords)
  le sixième nombre = le code midi en poussé pour la troisième note
  le septième nombre = le code midi en tiré pour la troisième note
  Les Touches basses/accord sont géré par les multiplexeurs, HIGH = Off, LOW = On */

const int Nb_ToucheAccord = 6;

const int Touche_Accord[Nb_ToucheAccord][7] = {

  {0, 80, 83, 84, 86, 87, 90},  // Touche accord a10 Rangée extérieure en haut
  {13, 79, 74, 83, 77, 86, 81}, // Touche accord a2
  {14, 72, 79, 76, 83, 79, 86}, // Touche accord a4  Rangée extérieure en bas
  {3, 75, 82, 78, 86, 82, 89},  // Touche accord a12 Rangée intérieure en haut
  {5, 76, 76, 79, 80, 83, 83},  // Touche accord a6
  {7, 77, 77, 81, 81, 84, 84},  // Touche accord a8  Rangée intérieure en bas
};

const int NoteBassePousse = 1;
const int NoteBasseTire = 2;
const int Note2BassePousse = 3;
const int Note2BasseTire = 4;
const int Note3BassePousse = 5;
const int Note3BasseTire = 6;
const int Nb_ToucheBasses = 6;

const int Touche_Basses[Nb_ToucheBasses][7] = {

  {1, 56, 59, 68, 71, 80, 83},  // Touche basses b9 Rangée extérieure en haut
  {12, 55, 50, 67, 62, 79, 74}, // Touche basses b1
  {15, 48, 55, 60, 67, 72, 79}, // Touche basses b3 Rangée extérieure en bas
  {2, 51, 58, 63, 70, 75, 82},  // Touche basses b11 Rangée intérieure en haut
  {4, 52, 57, 64, 69, 76, 81},  // Touche basses b5
  {6, 53, 53, 65, 65, 77, 77}   // Touche basses b7 Rangée intérieure en bas
};

// A0 = Pin 54 --- A11 = Pin 65

/*
  Tous ces codes midi de base seront pris par défaut au démarrage de l'instrument.
  En fonction de la position des codeurs rotatifs, deux variables seront ajoutées à ces codes.
  Pour la sélection de la tonalité, on ajoutera une variable allant de -6 à + 6 demi-tons
  Pour la sélection de l'octave, on ajoutera une variable de 12 demi-tons (donc 1 seule octave de décalage vers le haut)
*/

// Etat actuel, Etat avant
int Etat_Instrument[2] =   {InstrumentDef, 0}; // valeur entre 0 et 127
int Etat_Tona[2] =         {0, 0};
int Etat_Octave[2] =       {0, 0};
int Etat_RenvAccord[2] =   {0, 0};
int Etat_TierceOnOff[2] =  {OFF, OFF};
int Etat_Panic[2] =        {OFF, OFF};
int Etat_MidiInExt[2] =    {OFF, OFF};
int Etat_PousseTire[2] =   {OFF, OFF};
int Etat_BassesProf[2] =   {OFF, OFF};
int Etat_MidiMSB[2] =      {0, 0};         // Midi Bank MSB valeur entre 0 et 127
int Etat_MidiLSB[2] =      {0, 0};         // Midi Bank LSBvaleur entre 0 et 127
int Etat_InstrumentE[2] =  {InstrumentExtDef, 0}; // valeur entre 0 et 127
int Etat_Registre[2] =  {0, 0};            // valeur 0 Basson, 1 Flüte+Basson, 2 Flûte
int Etat_Volume_Midi[2] = {0, 0};          // Valeur Midi calculé du Volume
int Etat_Expression_Midi[2] = {0, 0};      // Valeur Midi calculé de l'Expression pour Midi Ext
int Etat_Gain_Tsunami[2] = {0, 0};         // Valeur Tsunami calculé du Gain
unsigned long Etat_Volume_Millis[2] =     {0, 0};// Valeur Milliseconde au moment de la lecture
int No_PinBasson = 48;
int No_PinFluteBasson = 52;
int No_PinFlute = 50;
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
