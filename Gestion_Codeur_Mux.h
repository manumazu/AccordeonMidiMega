// CODE POUR ACCORDEON DIATONIQUE MIDI

// Raccordement des codeurs rotatifs

const int Nb_Codeurs = 7;

const int inst_codeur_clock = 2;    // Codeur inst - Clock sur pin 2
const int inst_codeur_data = 3;     // Codeur inst - Data sur pin 3
const int inst_codeur_switch = 4;   // Codeur inst - Switch sur pin 4

const int tona_codeur_clock = 5;    // Codeur tonalité - Clock sur pin 5
const int tona_codeur_data = 6;     // Codeur tonalité - Data sur pin 6
const int tona_codeur_switch = 7;   // Codeur tonalité - Switch sur pin 7

const int octa_codeur_clock = 8;    // Codeur octave - Clock sur pin 8
const int octa_codeur_data = 9;     // Codeur octave - Data sur pin 9
const int octa_codeur_switch = 10;  // Codeur octave - Switch sur pin 10

const int renv_codeur_clock = 11;   // Codeur renversement - Clock sur pin 11
const int renv_codeur_data = 12;    // Codeur renversement - Data sur pin 12
const int renv_codeur_switch = 13;  // Codeur renversement - Switch sur pin 13

const int MMSB_codeur_clock = A2;   // Codeur MidiExt MSB - Clock sur pin A2
const int MMSB_codeur_data = A3;    // Codeur MidiExt MSB - Data sur pin A3
const int MMSB_codeur_switch = A4;  // Codeur MidiExt MSB - Switch sur pin A4

const int MLSB_codeur_clock = A2;   // Codeur MidiExt LSB - Clock sur pin A2 On utilise le meme codeur pour MSB LSB
const int MLSB_codeur_data = A3;    // Codeur MidiExt LSB - Data sur pin A3
const int MLSB_codeur_switch = A4;  // Codeur MidiExt LSB - Switch sur pin A4

const int insE_codeur_clock = 2;    // Codeur inst Midi Externe - Clock sur pin 2 On utilise le meme codeur Inst
const int insE_codeur_data = 3;     // Codeur inst Midi Externe - Data sur pin 3
const int insE_codeur_switch = 4;   // Codeur inst Midi Externe - Switch sur pin 4

const int codeur_position_Min[Nb_Codeurs] = {0, -6, -2, 0, 0, 0, 0};
const int codeur_position_Max[Nb_Codeurs] = {Nb_Libelle_Midi_Interne - 1, 6, 2, 2, 127, 127, 127};


//Stocke les pins
int codeur_clock[Nb_Codeurs] = {inst_codeur_clock, tona_codeur_clock, octa_codeur_clock, renv_codeur_clock, MMSB_codeur_clock, MLSB_codeur_clock, insE_codeur_clock};
int codeur_data[Nb_Codeurs] = {inst_codeur_data, tona_codeur_data, octa_codeur_data, renv_codeur_data, MMSB_codeur_data, MLSB_codeur_data, insE_codeur_data};

//Stock les états des clocks et des valeurs des compteurs
int codeur_position[Nb_Codeurs] = {0, 0, 0, 0, 0, 0, 0};
int codeur_previous_CLK[Nb_Codeurs] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW};
int codeur_current_CLK[Nb_Codeurs] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW};


// Déclaration des variables

// Switch des codeurs en INPUT_PULLUP
int inst_codeur = 0;
int tona_codeur = 1;
int octa_codeur = 2;
int renv_codeur = 3;
int MMSB_codeur = 4;
int MLSB_codeur = 5;  //switch dupliqué
int insE_codeur = 6;  //switch dupliqué

int Etat_SwitchInst[2] =       {OFF, OFF};
int Etat_SwitchTona[2] =       {OFF, OFF};
int Etat_SwitchOcta[2] =       {OFF, OFF};
int Etat_SwitchRenv[2] =       {OFF, OFF};
int Etat_SwitchMMSB[2] =       {OFF, OFF};
int Etat_SwitchMLSB[2] =       {ON, ON};    //Par défaut codeur LSB sélectionné
int Etat_SwitchInsE[2] =       {OFF, OFF};


//Déclaration pour les multiplexeurs
const int No_PinEtatMux1 = A7;    //Lecture Etat Mux 1
const int No_PinEtatMux2 = A8;    //Lecture Etat Mux 2
const int No_PinMuxAddr1 = A9;
const int No_PinMuxAddr2 = A10;
const int No_PinMuxAddr3 = A11;

// A0 = Pin 54 --- A11 = Pin 65

// tableau Etat Entrées MUX
int Etat_Mux1[8] = {
  OFF,
  OFF,
  OFF,
  OFF,
  OFF,
  OFF,
  OFF,
  OFF
};

int Etat_Mux2[8] = {
  OFF,
  OFF,
  OFF,
  OFF,
  OFF,
  OFF,
  OFF,
  OFF
};

//************************************************************
int Lecture_Codeur(int num_codeur, bool boucle) {

  codeur_current_CLK[num_codeur] = digitalRead(codeur_clock[num_codeur]);
  if ((codeur_previous_CLK[num_codeur] == LOW) && (codeur_current_CLK[num_codeur] == HIGH))
  {

    if (digitalRead(codeur_data[num_codeur]) == LOW) {
      if (codeur_position[num_codeur] != codeur_position_Min[num_codeur])
        codeur_position[num_codeur]--;
      else if (boucle) codeur_position[num_codeur] = codeur_position_Max[num_codeur];
    }
    else {
      if (codeur_position[num_codeur] != codeur_position_Max[num_codeur])
        codeur_position[num_codeur]++;
      else if (boucle) codeur_position[num_codeur] = codeur_position_Min[num_codeur];
    }

#if defined (DEBUG)
    AfficheDebug = "Lecture_Codeur " + String(num_codeur) + ": ";
    AfficheDebug +=  codeur_position[num_codeur];
    Serial.println (AfficheDebug);
#endif
  }
  codeur_previous_CLK[num_codeur] = codeur_current_CLK[num_codeur];

  return codeur_position[num_codeur];
}

//************************************************************
int Lecture_SwitchCodeur() {
  int retour = OFF;               // Si retour ON, faire un rafraichissement LCD

  if (Etat_MidiInExt[Etat_actuel] == Midi_Interne) {
    Etat_SwitchInst[Etat_actuel] = digitalRead(inst_codeur_switch);     // Détection du switch codeur inst
    if (Etat_SwitchInst[Etat_actuel] != Etat_SwitchInst[Etat_avant]) {
      Etat_SwitchInst[Etat_avant] = Etat_SwitchInst[Etat_actuel];
      Etat_Instrument[Etat_actuel] = InstrumentDef;               // Réinit du compteur inst
      codeur_position[inst_codeur] = InstrumentDef;                     // Réinit du codeur inst
      retour = ON;
#if defined (DEBUG)
      AfficheDebug = "Reset Inst SW ";
      AfficheDebug += Etat_SwitchInst[Etat_actuel];
      Serial.println(AfficheDebug);
#endif
    }
  }
  else {
    Etat_SwitchInsE[Etat_actuel] = digitalRead(insE_codeur_switch);     // Détection du switch codeur insE
    if (Etat_SwitchInsE[Etat_actuel] != Etat_SwitchInsE[Etat_avant]) {
      Etat_SwitchInsE[Etat_avant] = Etat_SwitchInsE[Etat_actuel];
      Etat_InstrumentE[Etat_actuel] = InstrumentExtDef;              // Réinit du compteur insE
      codeur_position[insE_codeur] = InstrumentExtDef;                     // Réinit du codeur insE
      retour = ON;
#if defined (DEBUG)
      AfficheDebug = "Reset InsE SW ";
      AfficheDebug += Etat_SwitchInsE[Etat_actuel];
      Serial.println(AfficheDebug);
#endif
    }
  }

  Etat_SwitchTona[Etat_actuel] = digitalRead(tona_codeur_switch);       // Détection du switch codeur tona
  if (Etat_SwitchTona[Etat_actuel] != Etat_SwitchTona[Etat_avant]) {
    Etat_SwitchTona[Etat_avant] = Etat_SwitchTona[Etat_actuel];
    Etat_Tona[Etat_actuel] = 0;                                   // Réinit du compteur tona
    Etat_Tona[Etat_avant] = 0;
    codeur_position[tona_codeur] = 0;                                   // Réinit du codeur tona
    retour = ON;
#if defined (DEBUG)
    AfficheDebug = "Reset Tona SW ";
    AfficheDebug += Etat_SwitchTona[Etat_actuel];
    Serial.println(AfficheDebug);
#endif
  }

  Etat_SwitchOcta[Etat_actuel] = digitalRead(octa_codeur_switch);       // Détection du switch codeur octave
  if (Etat_SwitchOcta[Etat_actuel] != Etat_SwitchOcta[Etat_avant]) {
    Etat_SwitchOcta[Etat_avant] = Etat_SwitchOcta[Etat_actuel];
    Etat_Octave[Etat_actuel] = 0;                                 // Réinit du compteur octave
    Etat_Octave[Etat_avant] = 0;                                  // Réinit du compteur octave
    codeur_position[octa_codeur] = 0;                                   // Réinit du codeur octave
    retour = ON;
#if defined (DEBUG)
    AfficheDebug = "Reset Octave SW ";
    AfficheDebug += Etat_SwitchOcta[Etat_actuel];
    Serial.println(AfficheDebug);
#endif
  }

  Etat_SwitchRenv[Etat_actuel] = digitalRead(renv_codeur_switch);       // Détection du switch codeur renversement
  if (Etat_SwitchRenv[Etat_actuel] != Etat_SwitchRenv[Etat_avant]) {
    Etat_SwitchRenv[Etat_avant] = Etat_SwitchRenv[Etat_actuel];
    Etat_RenvAccord[Etat_actuel] = 0;                             // Réinit du compteur renversement
    Etat_RenvAccord[Etat_avant] = 0;                              // Réinit du compteur renversement
    codeur_position[renv_codeur] = 0;                                   // Réinit du codeur renversement
    retour = ON;
#if defined (DEBUG)
    AfficheDebug = "Reset Renversement SW ";
    AfficheDebug += Etat_SwitchRenv[Etat_actuel];
    Serial.println(AfficheDebug);
#endif
  }

  if (Etat_MidiInExt[Etat_actuel] != Midi_Interne) {
    Etat_SwitchMMSB[Etat_actuel] = digitalRead(MMSB_codeur_switch);     // Détection du switch codeur MSB/LSB
    if (Etat_SwitchMMSB[Etat_actuel] != Etat_SwitchMMSB[Etat_avant]) {
      Etat_SwitchMMSB[Etat_avant] = Etat_SwitchMMSB[Etat_actuel];
      if (Etat_SwitchMMSB[Etat_actuel] == OFF) {                        //Test état Switch
        Etat_SwitchMLSB[Etat_actuel] = !Etat_SwitchMLSB[Etat_avant];    //Flipflop Etat MSB LSB stocké sur MLSB
        Etat_SwitchMLSB[Etat_avant] = Etat_SwitchMLSB[Etat_actuel];
#if defined (DEBUG)
        AfficheDebug = "Flipflop MSB/LSB SW ";
        AfficheDebug += Etat_SwitchMLSB[Etat_actuel];
        Serial.println(AfficheDebug);
#endif
      }
      retour = ON;
    }
  }
  return retour;
}

void Init_Codeur() {
  // Configure une résistance PULLUP sur l'entrée du bouton poussoir codeur
  pinMode(inst_codeur_switch, INPUT_PULLUP);       // Un seul codeur pour inst et insE
  pinMode(tona_codeur_switch, INPUT_PULLUP);       // Configure une résistance PULLUP sur l'entrée du bouton poussoir codeur tonalité
  pinMode(octa_codeur_switch, INPUT_PULLUP);       // Configure une résistance PULLUP sur l'entrée du bouton poussoir codeur octave
  pinMode(renv_codeur_switch, INPUT_PULLUP);       // Configure une résistance PULLUP sur l'entrée du bouton poussoir codeur octave
  pinMode(MMSB_codeur_switch, INPUT_PULLUP);       // Un seul codeur pour MSB et LSB
  pinMode(inst_codeur_clock, INPUT);               // Un seul codeur pour inst et insE
  pinMode(tona_codeur_clock, INPUT);
  pinMode(octa_codeur_clock, INPUT);
  pinMode(renv_codeur_clock, INPUT);
  pinMode(MMSB_codeur_clock, INPUT);               // Un seul codeur pour MSB et LSB
  pinMode(inst_codeur_data, INPUT);                // Un seul codeur pour inst et insE
  pinMode(tona_codeur_data, INPUT);
  pinMode(octa_codeur_data, INPUT);
  pinMode(renv_codeur_data, INPUT);
  pinMode(MMSB_codeur_data, INPUT);                // Un seul codeur pour MSB et LSB
  codeur_position[inst_codeur] = InstrumentDef;    // Init du codeur inst
  codeur_position[tona_codeur] = 0;                // Init du codeur tona
  codeur_position[octa_codeur] = 0;                // Init du codeur octa
  codeur_position[renv_codeur] = 0;                // Init du codeur renv
  codeur_position[MMSB_codeur] = 0;                // Init du codeur MSB
  codeur_position[MLSB_codeur] = 0;                // Init du codeur LSB
  codeur_position[insE_codeur] = InstrumentExtDef; // Init du codeur insE
  // On fait une lecture pour initialiser les variables.
  Lecture_Codeur(inst_codeur, false);
  Lecture_Codeur(tona_codeur, false);
  Lecture_Codeur(octa_codeur, false);
  Lecture_Codeur(renv_codeur, false);
  Lecture_Codeur(MMSB_codeur, false);
  Lecture_Codeur(MLSB_codeur, false);
  Lecture_Codeur(insE_codeur, false);
  // On init les codeurs car ils sont parfoit à 1 après première lecture
  codeur_position[inst_codeur] = InstrumentDef;    // Réinit du codeur inst
  codeur_position[tona_codeur] = 0;                // Réinit du codeur tona
  codeur_position[octa_codeur] = 0;                // Réinit du codeur octa
  codeur_position[renv_codeur] = 0;                // Réinit du codeur renv
  codeur_position[MMSB_codeur] = 0;                // Réinit du codeur MSB
  codeur_position[MLSB_codeur] = 0;                // Réinit du codeur LSB
  codeur_position[insE_codeur] = InstrumentExtDef; // Réinit du codeur insE
}

void Lecture_Mux() {
  for (int Addr = 0; Addr < 8; Addr++)  {
    int A1 = Addr % 2;
    int A2 = Addr / 2 % 2;
    int A3 = Addr / 4 % 2;
    digitalWrite(No_PinMuxAddr1, A1);
    digitalWrite(No_PinMuxAddr2, A2);
    digitalWrite(No_PinMuxAddr3, A3);
    Etat_Mux1[Addr] = digitalRead(No_PinEtatMux1);
    Etat_Mux2[Addr] = digitalRead(No_PinEtatMux2);
  }
}

void Init_Mux() {

  pinMode(No_PinMuxAddr1, OUTPUT);
  pinMode(No_PinMuxAddr2, OUTPUT);
  pinMode(No_PinMuxAddr3, OUTPUT);
  pinMode(No_PinEtatMux1, INPUT_PULLUP);
  pinMode(No_PinEtatMux2, INPUT_PULLUP);
  // On fait une lecture pour initialiser les variables.
  Lecture_Mux();
}
