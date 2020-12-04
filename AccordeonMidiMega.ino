#define DEBUG;          // si Debug sur Moniteur série standard

const String Version = "Mega_20201204-01";

#include "Def_AccordeonMidi.h"    //Définition externe des touches, fichier à placer au même endroit que le fichier code ino
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);; //Afficheur 2 lignes 16 caractères adresse I2C 0x27

String Affiche;

#include "Gestion_Codeur.h"
#include "Gestion_Midi.h"
#include "Libelle_Midi.h"

//Affichage de la ligne 1 ou 2 sur l'afficheur LCD
void Affiche_LCD(int ligne) {
  if (ligne == 1) {
    if (Etat_ToucheMidiInExt[Etat_actuel] == Midi_Interne) {
      Affiche = Libelle_Instr_Midi1[Etat_ToucheInstrument[Etat_actuel]];
    }
    else  {//Midi externe on affiche le numéro de Program Midi
      Affiche = "Instrument Ext ";
      Affiche += Etat_ToucheInstrument[Etat_actuel];
      Affiche += "         ";  // On remplit à blanc
    }
    lcd.setCursor(0, 0);
    lcd.print(Affiche);
  } else
  { Affiche = "Tona=";
    Affiche += Etat_ToucheTona[Etat_actuel]  < 0 ? "" : "+";
    Affiche += Etat_ToucheTona[Etat_actuel];
    lcd.setCursor(0, 1);
    lcd.print(Affiche);
    Affiche = "Octa=";
    Affiche += Etat_ToucheOctave[Etat_actuel]  < 0 ? "" : "+";
    Affiche += Etat_ToucheOctave[Etat_actuel];
    lcd.setCursor(8, 1);
    lcd.print(Affiche);
  }
}
// the setup function runs once when you press reset or power the board
void setup() {

  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.println(Version); //Affiche la version
  delay(2000);

  // initialisation pin digital du tableau de Touches Mélodie en INPUT_PULLUP
  for (int i = 0; i < Nb_ToucheMelodie; i++) {
    pinMode(Touche_Melodie[i][No_Pin], INPUT_PULLUP);
  }
  // initialisation pin analog en digital du tableau de Touches Basses en INPUT_PULLUP
  for (int i = 0; i < Nb_ToucheBasses; i++) {
    pinMode(Touche_Basses[i][No_Pin], INPUT_PULLUP);
  }

  // Initialisation touche Midic Panic
  pinMode(No_PinPanic, INPUT_PULLUP);
  pinMode(No_PinMidiInExt, INPUT_PULLUP);
  pinMode(No_PinTierceOnOff, INPUT_PULLUP);
  pinMode(No_PinPousseTire, INPUT_PULLUP);

  // Initialisation des codeurs instrument, tonalité et octave
  Init_Codeur();

#if defined (DEBUG)
  Serial.begin (9600);    //pour le debug écran PC 9600
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Debug");
#endif
  //Init fonctions Midi
  InitMidi(0);
  Affiche_LCD(1);
  Affiche_LCD(2);
}

// the loop function runs over and over again forever
void loop() {

  int retour;

  //Lecture des switchs des codeurs instrument, tonalité et octave
  retour = Lecture_SwitchCodeur();
  //Vérifier si reset Instrument pour reinit Midi
  if (Etat_ToucheInstrument[Etat_actuel] != Etat_ToucheInstrument[Etat_avant]) {
    Instrument(Etat_ToucheInstrument[Etat_actuel]);
    Etat_ToucheInstrument[Etat_avant] = Etat_ToucheInstrument[Etat_actuel];
#if defined (DEBUG)
    Affiche = "Change Instrument ";
    Affiche += Etat_ToucheInstrument[Etat_actuel];
    Serial.println(Affiche);
#endif
    Affiche_LCD(1);
  }
  if (retour == ON )
    Affiche_LCD(2);       // On rafraichit l'affichage Tona Octa

  //Lecture des codeurs instrument, tonalité et octave

  //Codeur instrument
  Etat_ToucheInstrument[Etat_actuel] = Lecture_Codeur(inst_codeur);
  if (Etat_ToucheInstrument[Etat_actuel] != Etat_ToucheInstrument[Etat_avant]) {
#if defined (DEBUG)
    Affiche = "Change Instrument ";
    Affiche += Etat_ToucheInstrument[Etat_actuel];
    Serial.println(Affiche);
#endif
    Etat_ToucheInstrument[Etat_avant] = Etat_ToucheInstrument[Etat_actuel];
    Instrument (Etat_ToucheInstrument[Etat_actuel]);
    Affiche_LCD(1);
  }

  //Codeur Tona
  Etat_ToucheTona[Etat_actuel] = Lecture_Codeur(tona_codeur);
  if (Etat_ToucheTona[Etat_actuel] != Etat_ToucheTona[Etat_avant]) {
#if defined (DEBUG)
    Affiche = "Change Tona ";
    Affiche += Etat_ToucheTona[Etat_actuel];
    Serial.println(Affiche);
#endif
    Etat_ToucheTona[Etat_avant] = Etat_ToucheTona[Etat_actuel];
    Affiche_LCD(2);
  }

  //Codeur Octave
  Etat_ToucheOctave[Etat_actuel] = Lecture_Codeur(octa_codeur);
  if (Etat_ToucheOctave[Etat_actuel] != Etat_ToucheOctave[Etat_avant]) {
#if defined (DEBUG)
    Affiche = "Change Octave ";
    Affiche += Etat_ToucheOctave[Etat_actuel];
    Serial.println(Affiche);
#endif
    Etat_ToucheOctave[Etat_avant] = Etat_ToucheOctave[Etat_actuel];
    Affiche_LCD(2);
  }

  //Touche Panic
  Etat_TouchePanic[Etat_actuel] = digitalRead(No_PinPanic);
  if (Etat_TouchePanic[Etat_actuel] != Etat_TouchePanic[Etat_avant]) {
#if defined (DEBUG)
    Affiche = "Change Panic ";
    Affiche += Etat_TouchePanic[Etat_actuel] ? "OFF" : "ON";
    Serial.println(Affiche);
#endif
    Etat_TouchePanic[Etat_avant] = Etat_TouchePanic[Etat_actuel];
    if (Etat_TouchePanic[Etat_avant] == ON) {
      for (int i = 0; i < 128; i++)  {
        noteOn(0x90, i, 0x00);    //On arrête toutes les touches
      }
      delay(100);
      InitMidi(0);                //On réinitialise le Midi
    }
  }
  //Touche MidiInExt
  Etat_ToucheMidiInExt[Etat_actuel] = digitalRead(No_PinMidiInExt);
  if (Etat_ToucheMidiInExt[Etat_actuel] != Etat_ToucheMidiInExt[Etat_avant]) {
#if defined (DEBUG)
    Affiche = "Change MidiInExt ";
    Affiche += Etat_ToucheMidiInExt[Etat_actuel] ? "OFF" : "ON";
    Serial.println(Affiche);
#endif
    Etat_ToucheMidiInExt[Etat_avant] = Etat_ToucheMidiInExt[Etat_actuel];
    //ReInit fonctions Midi
    InitMidi(0);
  }
  //Touche PousseTire
  Etat_TouchePousseTire[Etat_actuel] = digitalRead(No_PinPousseTire);
  if (Etat_TouchePousseTire[Etat_actuel] != Etat_TouchePousseTire[Etat_avant]) {
#if defined (DEBUG)
    Affiche = "Change PousseTire ";
    Affiche += Etat_TouchePousseTire[Etat_actuel] ? "OFF" : "ON";
    Serial.println(Affiche);
#endif
    Etat_TouchePousseTire[Etat_avant] = Etat_TouchePousseTire[Etat_actuel];

    //inversion des notes poussé-tiré en cours
    for (int i = 0; i < Nb_ToucheMelodie; i++) {
      if (Etat_Touche_Melodie[i][Etat_actuel] == ON) {
        noteOn(0x90, Etat_Touche_Melodie[i][Note_encours], 0x00);
#if defined (DEBUG)
        Affiche = "Pin ";
        Affiche += Touche_Melodie[i][No_Pin];
        Affiche += " Note2 ";
        Affiche += Etat_Touche_Melodie[i][Note_encours];
        Affiche +=  " OFF ";
        Serial.println(Affiche);
#endif
        PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Melodie[i][Note_encours] = Touche_Melodie[i][PousseTire];
        Etat_Touche_Melodie[i][Note_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

        Etat_Touche_Melodie[i][Note_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
        noteOn(0x90, Etat_Touche_Melodie[i][Note_encours], MidiVelocity);
#if defined (DEBUG)
        Affiche = "Pin ";
        Affiche += Touche_Melodie[i][No_Pin];
        Affiche += " Note2 ";
        Affiche += Etat_Touche_Melodie[i][Note_encours];
        Affiche +=  " ON ";
        Serial.println(Affiche);
#endif
      }
    }
    for (int i = 0; i < Nb_ToucheBasses; i++) {
      if (Etat_Touche_Basses[i][Etat_actuel] == ON) {
        noteOn(0x90, Etat_Touche_Basses[i][Note1_encours], 0x00);
#if defined (DEBUG)
        Affiche = "Pin ";
        Affiche += Touche_Basses[i][No_Pin];
        Affiche += " Note1 ";
        Affiche += Etat_Touche_Basses[i][Note1_encours];
        Affiche +=  " OFF ";
        Serial.println(Affiche);
#endif
        PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Basses[i][Note1_encours] = Touche_Basses[i][PousseTire];
        Etat_Touche_Basses[i][Note1_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

        Etat_Touche_Basses[i][Note1_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
        noteOn(0x90, Etat_Touche_Basses[i][Note1_encours], MidiVelocity);
#if defined (DEBUG)
        Affiche = "Pin ";
        Affiche += Touche_Basses[i][No_Pin];
        Affiche += " Note1 ";
        Affiche += Etat_Touche_Basses[i][Note1_encours];
        Affiche +=  " ON ";
        Serial.println(Affiche);
#endif
        if (Etat_ToucheTierceOnOff[Etat_actuel] == ON) {
          noteOn(0x90, Etat_Touche_Basses[i][Note2_encours], 0x00);
#if defined (DEBUG)
          Affiche = "Pin ";
          Affiche += Touche_Basses[i][No_Pin];
          Affiche += " Note2 ";
          Affiche += Etat_Touche_Basses[i][Note2_encours];
          Affiche +=  " OFF ";
          Serial.println(Affiche);
#endif
          PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
          Etat_Touche_Basses[i][Note2_encours] = Touche_Basses[i][PousseTire + 2];
          Etat_Touche_Basses[i][Note2_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

          Etat_Touche_Basses[i][Note2_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
          noteOn(0x90, Etat_Touche_Basses[i][Note2_encours], MidiVelocity);
#if defined (DEBUG)
          Affiche = "Pin ";
          Affiche += Touche_Basses[i][No_Pin];
          Affiche += " Note2 ";
          Affiche += Etat_Touche_Basses[i][Note2_encours];
          Affiche +=  " ON ";
          Serial.println(Affiche);
#endif
        }
        noteOn(0x90, Etat_Touche_Basses[i][Note3_encours], 0x00);
#if defined (DEBUG)
        Affiche = "Pin ";
        Affiche += Touche_Basses[i][No_Pin];
        Affiche += " Note3 ";
        Affiche += Etat_Touche_Basses[i][Note3_encours];
        Affiche +=  " OFF ";
        Serial.println(Affiche);
#endif
        PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Basses[i][Note3_encours] = Touche_Basses[i][PousseTire + 4];
        Etat_Touche_Basses[i][Note3_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

        Etat_Touche_Basses[i][Note3_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
        noteOn(0x90, Etat_Touche_Basses[i][Note3_encours], MidiVelocity);
#if defined (DEBUG)
        Affiche = "Pin ";
        Affiche += Touche_Basses[i][No_Pin];
        Affiche += " Note3 ";
        Affiche += Etat_Touche_Basses[i][Note3_encours];
        Affiche +=  " ON ";
        Serial.println(Affiche);
#endif
      }
    }
  }
  //Touche TierceOnOff
  Etat_ToucheTierceOnOff[Etat_actuel] = digitalRead(No_PinTierceOnOff);
  if (Etat_ToucheTierceOnOff[Etat_actuel] != Etat_ToucheTierceOnOff[Etat_avant]) {
#if defined (DEBUG)
    Affiche = "Change TierceOnOff ";
    Affiche += Etat_ToucheTierceOnOff[Etat_actuel] ? "OFF" : "ON";
    Serial.println(Affiche);
#endif
    Etat_ToucheTierceOnOff[Etat_avant] = Etat_ToucheTierceOnOff[Etat_actuel];
    //Arrêt des tierces en cours de jeu
    if (Etat_ToucheTierceOnOff[Etat_actuel] == OFF)
      for (int i = 0; i < Nb_ToucheBasses; i++) {
        if (Etat_Touche_Basses[i][Etat_actuel] == ON) {
          noteOn(0x90, Etat_Touche_Basses[i][Note2_encours], 0x00);
#if defined (DEBUG)
          Affiche = "Pin ";
          Affiche += Touche_Basses[i][No_Pin];
          Affiche += " Note2 ";
          Affiche += Etat_Touche_Basses[i][Note2_encours];
          Affiche +=  " OFF ";
          Serial.println(Affiche);
#endif
        }
      }
  }
  //Lecture Etat touche Mélodie
  for (int i = 0; i < Nb_ToucheMelodie; i++) {
    Etat_Touche_Melodie [i][Etat_actuel] = digitalRead(Touche_Melodie[i][No_Pin]); // Lecture Touche)
    if (Etat_Touche_Melodie[i][Etat_actuel] != Etat_Touche_Melodie[i][Etat_avant]) {
      if (Etat_Touche_Melodie[i][Etat_actuel] == ON) {
        PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Melodie[i][Note_encours] = Touche_Melodie[i][PousseTire];
        Etat_Touche_Melodie[i][Note_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

        Etat_Touche_Melodie[i][Note_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona

        //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
        noteOn(0x90, Etat_Touche_Melodie[i][Note_encours], MidiVelocity);
      }
      else
        //Note on channel 1 (0x90), some note value (note), silent velocity (0x00):
        noteOn(0x90, Etat_Touche_Melodie[i][Note_encours], 0x00);

      Etat_Touche_Melodie[i][Etat_avant] = Etat_Touche_Melodie[i][Etat_actuel];
#if defined (DEBUG)
      Affiche = "Pin ";
      Affiche += Touche_Melodie[i][No_Pin];
      Affiche += " Note ";
      Affiche += Etat_Touche_Melodie[i][Note_encours];
      Affiche += Etat_Touche_Melodie[i][Etat_actuel] ? " OFF  " : " ON ";
      Serial.println(Affiche);
#endif
    }
  }
  //Lecture Etat touche Basse
  for (int i = 0; i < Nb_ToucheBasses; i++) {
    Etat_Touche_Basses [i][Etat_actuel] = digitalRead(Touche_Basses[i][No_Pin]); // Lecture Touche)
    if (Etat_Touche_Basses[i][Etat_actuel] != Etat_Touche_Basses[i][Etat_avant]) {
      if (Etat_Touche_Basses[i][Etat_actuel] == ON) {
        PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Basses[i][Note1_encours] = Touche_Basses[i][PousseTire];
        Etat_Touche_Basses[i][Note1_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
        noteOn(0x90, Etat_Touche_Basses[i][Note1_encours], MidiVelocity);
        if (Etat_ToucheTierceOnOff[Etat_actuel] == ON) {
          Etat_Touche_Basses[i][Note2_encours] = Touche_Basses[i][PousseTire + 2]; //Deuxième note à jouer
          Etat_Touche_Basses[i][Note2_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          noteOn(0x90, Etat_Touche_Basses[i][Note2_encours], MidiVelocity);
        }
        Etat_Touche_Basses[i][Note3_encours] = Touche_Basses[i][PousseTire + 4]; //Troisième note à jouer
        Etat_Touche_Basses[i][Note3_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
        noteOn(0x90, Etat_Touche_Basses[i][Note3_encours], MidiVelocity);
      }
      else
      {
        //Note on channel 1 (0x90), some note value (note), silent velocity (0x00):
        noteOn(0x90, Etat_Touche_Basses[i][Note1_encours], 0x00);
        if (Etat_ToucheTierceOnOff[Etat_actuel] == ON)
          noteOn(0x90, Etat_Touche_Basses[i][Note2_encours], 0x00);
        noteOn(0x90, Etat_Touche_Basses[i][Note3_encours], 0x00);
      }
      Etat_Touche_Basses[i][Etat_avant] = Etat_Touche_Basses[i][Etat_actuel];
#if defined (DEBUG)
      Affiche = "Pin ";
      Affiche += Touche_Basses[i][No_Pin];
      Affiche += " Note1 ";
      Affiche += Etat_Touche_Basses[i][Note1_encours];
      Affiche += Etat_Touche_Basses[i][Etat_actuel] ? " OFF  " : " ON ";
      Serial.println(Affiche);
      if (Etat_ToucheTierceOnOff[Etat_actuel] == ON) {
        Affiche = "Pin ";
        Affiche += Touche_Basses[i][No_Pin];
        Affiche += " Note2 ";
        Affiche += Etat_Touche_Basses[i][Note2_encours];
        Affiche += Etat_Touche_Basses[i][Etat_actuel] ? " OFF  " : " ON ";
        Serial.println(Affiche);
      }
      Affiche = "Pin ";
      Affiche += Touche_Basses[i][No_Pin];
      Affiche += " Note3 ";
      Affiche += Etat_Touche_Basses[i][Note3_encours];
      Affiche += Etat_Touche_Basses[i][Etat_actuel] ? " OFF  " : " ON ";
      Serial.println(Affiche);
#endif
    }
  }
}
