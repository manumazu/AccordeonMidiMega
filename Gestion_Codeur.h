// CODE POUR ACCORDEON DIATONIQUE MIDI

// Raccordement des codeurs rotatifs

const int Nb_Codeurs = 3;

const int inst_codeur_clock = 2;  // Codeur inst - Clock sur pin 2
const int inst_codeur_data = 3;   // Codeur inst - Data sur pin 3
const int inst_codeur_switch = 4; // Codeur inst - Switch sur pin 4

const int tona_codeur_clock = 5;  // Codeur tonalité - Clock sur pin 5
const int tona_codeur_data = 6;   // Codeur tonalité - Data sur pin 6
const int tona_codeur_switch = 7; // Codeur tonalité - Switch sur pin 7

const int octa_codeur_clock = 8;   // Codeur octave - Clock sur pin 8
const int octa_codeur_data = 9;    // Codeur octave - Data sur pin 9
const int octa_codeur_switch = 10; // Codeur octave - Switch sur pin 10

const int codeur_position_Min[Nb_Codeurs] = {0, -6, -2};
const int codeur_position_Max[Nb_Codeurs] = {127, 6, 2};


//Stocke les pins
int codeur_clock[Nb_Codeurs] = {inst_codeur_clock, tona_codeur_clock,octa_codeur_clock};
int codeur_data[Nb_Codeurs] = {inst_codeur_data, tona_codeur_data,octa_codeur_data};

//Stock les états des clocks et des valeurs des compteurs
int codeur_position[Nb_Codeurs] = {0, 0, 0};
int codeur_previous_CLK[Nb_Codeurs] = {LOW, LOW, LOW};
int codeur_current_CLK[Nb_Codeurs] = {LOW, LOW, LOW};


// Déclaration des variables

// Switch des codeurs en INPUT_PULLUP
int inst_codeur = 0;
int tona_codeur = 1;
int octa_codeur = 2;
int Etat_SwitchInst[2] =       {OFF, OFF};
int Etat_SwitchTona[2] =       {OFF, OFF};
int Etat_SwitchOcta[2] =       {OFF, OFF};


//************************************************************
int Lecture_Codeur(int num_codeur) {

  codeur_current_CLK[num_codeur] = digitalRead(codeur_clock[num_codeur]);
  if ((codeur_previous_CLK[num_codeur] == LOW) && (codeur_current_CLK[num_codeur] == HIGH))
  {

    if (digitalRead(codeur_data[num_codeur]) == LOW) {
      if (codeur_position[num_codeur] != codeur_position_Min[num_codeur])
        codeur_position[num_codeur]--;
    } else {
      if (codeur_position[num_codeur] != codeur_position_Max[num_codeur])
        codeur_position[num_codeur]++;
    }

#if defined (DEBUG)
    Affiche = "Lecture_Codeur " + String(num_codeur) + ": ";
    Affiche +=  codeur_position[num_codeur];
    Serial.println (Affiche);
#endif
  }
  codeur_previous_CLK[num_codeur] = codeur_current_CLK[num_codeur];

  return codeur_position[num_codeur];
}

//************************************************************
int Lecture_SwitchCodeur() {
  int retour = OFF;               // Si retour ON, faire un rafraichissement LCD

  Etat_SwitchInst[Etat_actuel] = digitalRead(inst_codeur_switch);       // Détection du switch codeur inst
  if (Etat_SwitchInst[Etat_actuel] != Etat_SwitchInst[Etat_avant]) {
    Etat_SwitchInst[Etat_avant] = Etat_SwitchInst[Etat_actuel];
    Etat_ToucheInstrument[Etat_actuel] = InstrumentDef;                 // Réinit du compteur inst
    codeur_position[inst_codeur] = InstrumentDef;                       // Réinit du codeur inst
    retour = ON;
#if defined (DEBUG)
    Affiche = "Reset Inst SW ";
    Affiche += Etat_SwitchInst[Etat_actuel];
    Serial.println(Affiche);
#endif
  }

  Etat_SwitchTona[Etat_actuel] = digitalRead(tona_codeur_switch);       // Détection du switch codeur tona
  if (Etat_SwitchTona[Etat_actuel] != Etat_SwitchTona[Etat_avant]) {
    Etat_SwitchTona[Etat_avant] = Etat_SwitchTona[Etat_actuel];
    Etat_ToucheTona[Etat_actuel] = 0;                                   // Réinit du compteur tona
    Etat_ToucheTona[Etat_avant] = 0;
    codeur_position[tona_codeur] = 0;                                   // Réinit du codeur inst
    retour = ON;
#if defined (DEBUG)
    Affiche = "Reset Tona SW ";
    Affiche += Etat_SwitchTona[Etat_actuel];
    Serial.println(Affiche);
#endif
  }

  Etat_SwitchOcta[Etat_actuel] = digitalRead(octa_codeur_switch);        // Détection du switch codeur octave
  if (Etat_SwitchOcta[Etat_actuel] != Etat_SwitchOcta[Etat_avant]) {
    Etat_SwitchOcta[Etat_avant] = Etat_SwitchOcta[Etat_actuel];
    Etat_ToucheOctave[Etat_actuel] = 0;                                // Réinit du compteur octave
    Etat_ToucheOctave[Etat_avant] = 0;                                 // Réinit du compteur octave
    codeur_position[octa_codeur] = 0;                                    // Réinit du codeur inst
    retour = ON;
#if defined (DEBUG)
    Affiche = "Reset Octave SW ";
    Affiche += Etat_SwitchOcta[Etat_actuel];
    Serial.println(Affiche);
#endif
  }
  return retour;
}

void Init_Codeur() {

  pinMode(inst_codeur_switch, INPUT_PULLUP);   // Configure une résistance PULLUP sur l'entrée du bouton poussoir codeur instrument
  pinMode(tona_codeur_switch, INPUT_PULLUP);   // Configure une résistance PULLUP sur l'entrée du bouton poussoir codeur tonalité
  pinMode(octa_codeur_switch, INPUT_PULLUP);   // Configure une résistance PULLUP sur l'entrée du bouton poussoir codeur octave
  pinMode(inst_codeur_clock, INPUT);
  pinMode(tona_codeur_clock, INPUT);
  pinMode(octa_codeur_clock, INPUT);
  pinMode(inst_codeur_data, INPUT);
  pinMode(tona_codeur_data, INPUT);
  pinMode(octa_codeur_data, INPUT);
  codeur_position[inst_codeur] = InstrumentDef;                       // Init du codeur inst
  codeur_position[tona_codeur] = 0;                                   // Réinit du codeur inst
  codeur_position[octa_codeur] = 0;                                    // Réinit du codeur inst
  // On fait une lecture pour initialiser les variables. 
  Lecture_Codeur(inst_codeur);
  Lecture_Codeur(tona_codeur);
  Lecture_Codeur(octa_codeur);
  // On init les codeurs car ils sont parfoit à 1 après première lecture
  codeur_position[inst_codeur] = InstrumentDef;                       // Init du codeur inst
  codeur_position[tona_codeur] = 0;                                   // Réinit du codeur inst
  codeur_position[octa_codeur] = 0;                                   // Réinit du codeur inst
}
