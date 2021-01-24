#define DEBUG          // si Debug sur Moniteur série standard

const String Version = "Robert_20210124-1   ";

#include "Def_AccordeonMidi.h"    //Définition externe des touches
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);; //Afficheur 4 lignes 20 caractères adresse I2C 0x27

String Affiche;
#if defined (DEBUG)
String AfficheDebug;
#endif

#include "Libelle_Midi.h"
#include "Gestion_Codeur_Mux.h"
#include "Gestion_Midi.h"


//Affichage de la ligne 1 ou 2 sur l'afficheur LCD
void Affiche_LCD(int ligne) {
  int i;

  switch (ligne) {
    case 1 :  //Instrument
      if (Etat_ToucheMidiInExt[Etat_actuel] == Midi_Interne) {
        Affiche = Libelle_Instr_Midi1[Etat_ToucheInstrument[Etat_actuel]];
      }
      else  {//Midi externe on affiche le numéro de Bank et Program Midi
        Affiche = "M ";
        Affiche += Etat_ToucheMidiMSB[Etat_actuel];
        Affiche += " L ";
        Affiche += Etat_ToucheMidiLSB[Etat_actuel];
        Affiche += " Inst ";
        Affiche += Etat_ToucheInstrumentE[Etat_actuel];
        for (i = Affiche.length(); i < 20; i++)
          Affiche += " ";  // On remplit à blanc
      }
      lcd.setCursor(0, 0);
      lcd.print(Affiche);
      break;
    case 2 :  //tonalité - Octave - Renversement
      Affiche = "Ton";
      Affiche += Etat_ToucheTona[Etat_actuel]  < 0 ? "" : "+";
      Affiche += Etat_ToucheTona[Etat_actuel];
      lcd.setCursor(0, 1);
      lcd.print(Affiche);
      Affiche = "Oct";
      Affiche += Etat_ToucheOctave[Etat_actuel]  < 0 ? "" : "+";
      Affiche += Etat_ToucheOctave[Etat_actuel];
      lcd.setCursor(7, 1);
      lcd.print(Affiche);
      Affiche = "Ren";
      Affiche += Etat_ToucheRenvAccord[Etat_actuel]  < 0 ? "" : "+";
      Affiche += Etat_ToucheRenvAccord[Etat_actuel];
      lcd.setCursor(14, 1);
      lcd.print(Affiche);
      break;
    case 3: // Basses Profondes - Tierces
      Affiche = "Bas.";
      Affiche += Etat_ToucheBassesProf[Etat_actuel] ? "Sans" : "Prof ";
      lcd.setCursor(0, 2);
      lcd.print(Affiche);
      Affiche = Etat_ToucheTierceOnOff[Etat_actuel] ?  "Sans" : "Avec";
      Affiche += " tierce";
      lcd.setCursor(9, 2);
      lcd.print(Affiche);
      break;
    case 4: // Registre
      Affiche = Libelle_Registre[Etat_ToucheRegistre[Etat_actuel]];
      for (i = Affiche.length(); i < 20; i++)
        Affiche += " ";  // On remplit à blanc
      lcd.setCursor(0, 3);
      lcd.print(Affiche);
      break;
    default:
      break;
  }
}
// the setup function runs once when you press reset or power the board
void setup() {

  lcd.init();                      // initialize the lcd
  delay(100);
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.clear();
  lcd.print(Version); //Affiche la version
  delay(4000);

  // initialisation pin digital du tableau de Touches Mélodie en INPUT_PULLUP
  for (int i = 0; i < Nb_ToucheMelodie; i++) {
    pinMode(Touche_Melodie[i][No_Pin], INPUT_PULLUP);
  }

  // Initialisation touche Midic Panic
  pinMode(No_PinPanic, INPUT_PULLUP);
  pinMode(No_PinMidiInExt, INPUT_PULLUP);
  pinMode(No_PinTierceOnOff, INPUT_PULLUP);
  pinMode(No_PinBassesProf, INPUT_PULLUP);
  pinMode(No_PinVolume, INPUT);          //Analogique provisoire
  pinMode(No_PinPousseTire, INPUT_PULLUP); //Digital provisoire

  // Initialisation des codeurs instrument, tonalité, octave et Midi MSB LSB
  Init_Codeur();

  // Initialisation des multiplexeurs Basses,Accord et spare
  Init_Mux();

#if defined (DEBUG)
  Serial.begin (9600);    //pour le debug écran PC 9600
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Debug");
#endif
  //Init fonctions Midi
  InitMidi();
  Affiche_LCD(1);
  Affiche_LCD(2);
  Affiche_LCD(3);
  Affiche_LCD(4);
}

// the loop function runs over and over again forever
void loop() {

  int retour;

  //Lecture des switchs des codeurs instrument, tonalité, octave et Midi MSB LSB
  retour = Lecture_SwitchCodeur();
  //Vérifier si reset Instrument pour reinit Midi
  if (Etat_ToucheMidiInExt[Etat_actuel] == Midi_Interne) {
    if (Etat_ToucheInstrument[Etat_actuel] != Etat_ToucheInstrument[Etat_avant]) {
      Instrument(ChannelMelodie, Etat_ToucheInstrument[Etat_actuel], Midi_Interne);
      Instrument(ChannelBasses, Etat_ToucheInstrument[Etat_actuel], Midi_Interne);
      Instrument(ChannelAccord, Etat_ToucheInstrument[Etat_actuel], Midi_Interne);
      Etat_ToucheInstrument[Etat_avant] = Etat_ToucheInstrument[Etat_actuel];
#if defined (DEBUG)
      AfficheDebug = "Reset Instrument ";
      AfficheDebug += Etat_ToucheInstrument[Etat_actuel];
      Serial.println(AfficheDebug);
#endif
      Affiche_LCD(1);
    }
  }
  else   {
    if (Etat_ToucheInstrumentE[Etat_actuel] != Etat_ToucheInstrumentE[Etat_avant]) {
      Instrument(ChannelMelodie, Etat_ToucheInstrument[Etat_actuel], Midi_Interne);
      Instrument(ChannelBasses, Etat_ToucheInstrument[Etat_actuel], Midi_Interne);
      Instrument(ChannelAccord, Etat_ToucheInstrument[Etat_actuel], Midi_Interne);
      Etat_ToucheInstrumentE[Etat_avant] = Etat_ToucheInstrumentE[Etat_actuel];
#if defined (DEBUG)
      AfficheDebug = "Reset InstrumentExt ";
      AfficheDebug += Etat_ToucheInstrumentE[Etat_actuel];
      Serial.println(AfficheDebug);
#endif
      Affiche_LCD(1);
    }
  }

  if (retour == ON )
    Affiche_LCD(2);       // On rafraichit l'affichage Tona Octa

  //Lecture des codeurs instrument, tonalité, octave et Midi MSB LSB

  //Codeur Midi LSB et MSB
  if (Etat_ToucheMidiInExt[Etat_actuel] == ON) {
    if (Etat_SwitchMLSB[Etat_actuel] == ON) {
      Etat_ToucheMidiLSB[Etat_actuel] = Lecture_Codeur(MLSB_codeur, true);
      if (Etat_ToucheMidiLSB[Etat_actuel] != Etat_ToucheMidiLSB[Etat_avant]) {
#if defined (DEBUG)
        AfficheDebug = "Change LSB ";
        AfficheDebug += Etat_ToucheMidiLSB[Etat_actuel];
        Serial.println(AfficheDebug);
#endif
        Etat_ToucheMidiLSB[Etat_avant] = Etat_ToucheMidiLSB[Etat_actuel];
        BankLSB = Etat_ToucheMidiLSB[Etat_avant];
        Instrument(ChannelMelodie, Etat_ToucheInstrumentE[Etat_actuel], Midi_Ext);
        Instrument(ChannelBasses, Etat_ToucheInstrumentE[Etat_actuel], Midi_Ext);
        Instrument(ChannelAccord, Etat_ToucheInstrumentE[Etat_actuel], Midi_Ext);
        Affiche_LCD(1);
      }
    }
    else
    {
      Etat_ToucheMidiMSB[Etat_actuel] = Lecture_Codeur(MMSB_codeur, true);
      if (Etat_ToucheMidiMSB[Etat_actuel] != Etat_ToucheMidiMSB[Etat_avant]) {
#if defined (DEBUG)
        AfficheDebug = "Change MSB ";
        AfficheDebug += Etat_ToucheMidiMSB[Etat_actuel];
        Serial.println(AfficheDebug);
#endif
        Etat_ToucheMidiMSB[Etat_avant] = Etat_ToucheMidiMSB[Etat_actuel];
        BankMSB = Etat_ToucheMidiMSB[Etat_avant];
        Instrument(ChannelMelodie, Etat_ToucheInstrumentE[Etat_actuel], Midi_Ext);
        Instrument(ChannelBasses, Etat_ToucheInstrumentE[Etat_actuel], Midi_Ext);
        Instrument(ChannelAccord, Etat_ToucheInstrumentE[Etat_actuel], Midi_Ext);
        Affiche_LCD(1);
      }
    }
  }

  //Codeur instrument
  if (Etat_ToucheMidiInExt[Etat_actuel] == OFF) {
    Etat_ToucheInstrument[Etat_actuel] = Lecture_Codeur(inst_codeur, false);
    if (Etat_ToucheInstrument[Etat_actuel] != Etat_ToucheInstrument[Etat_avant]) {
#if defined (DEBUG)
      AfficheDebug = "ChangeCodeur Instrument ";
      AfficheDebug += Etat_ToucheInstrument[Etat_actuel];
      Serial.println(AfficheDebug);
#endif
      Etat_ToucheInstrument[Etat_avant] = Etat_ToucheInstrument[Etat_actuel];
      Instrument(ChannelMelodie, Etat_ToucheInstrument[Etat_actuel], Midi_Int);
      Instrument(ChannelBasses, Etat_ToucheInstrument[Etat_actuel], Midi_Int);
      Instrument(ChannelAccord, Etat_ToucheInstrument[Etat_actuel], Midi_Int);
      Affiche_LCD(1);
    }
  }
  else {
    Etat_ToucheInstrumentE[Etat_actuel] = Lecture_Codeur(insE_codeur, true);
    if (Etat_ToucheInstrumentE[Etat_actuel] != Etat_ToucheInstrumentE[Etat_avant]) {
#if defined (DEBUG)
      AfficheDebug = "ChangeCodeur InstrumentExt ";
      AfficheDebug += Etat_ToucheInstrumentE[Etat_actuel];
      Serial.println(AfficheDebug);
#endif
      Etat_ToucheInstrumentE[Etat_avant] = Etat_ToucheInstrumentE[Etat_actuel];
      Instrument(ChannelMelodie, Etat_ToucheInstrumentE[Etat_actuel], Midi_Ext);
      Instrument(ChannelBasses, Etat_ToucheInstrumentE[Etat_actuel], Midi_Ext);
      Instrument(ChannelAccord, Etat_ToucheInstrumentE[Etat_actuel], Midi_Ext);
      Affiche_LCD(1);
    }
  }
  //Codeur Tona
  Etat_ToucheTona[Etat_actuel] = Lecture_Codeur(tona_codeur, false);
  if (Etat_ToucheTona[Etat_actuel] != Etat_ToucheTona[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change Tona ";
    AfficheDebug += Etat_ToucheTona[Etat_actuel];
    Serial.println(AfficheDebug);
#endif
    Etat_ToucheTona[Etat_avant] = Etat_ToucheTona[Etat_actuel];
    Affiche_LCD(2);
  }

  //Codeur Octave
  Etat_ToucheOctave[Etat_actuel] = Lecture_Codeur(octa_codeur, false);
  if (Etat_ToucheOctave[Etat_actuel] != Etat_ToucheOctave[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change Octave ";
    AfficheDebug += Etat_ToucheOctave[Etat_actuel];
    Serial.println(AfficheDebug);
#endif
    Etat_ToucheOctave[Etat_avant] = Etat_ToucheOctave[Etat_actuel];
    Affiche_LCD(2);
  }

  //Codeur Renversement
  Etat_ToucheRenvAccord[Etat_actuel] = Lecture_Codeur(renv_codeur, false);
  if (Etat_ToucheRenvAccord[Etat_actuel] != Etat_ToucheRenvAccord[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change Renversement ";
    AfficheDebug += Etat_ToucheRenvAccord[Etat_actuel];
    Serial.println(AfficheDebug);
#endif
    Etat_ToucheRenvAccord[Etat_avant] = Etat_ToucheRenvAccord[Etat_actuel];
    Affiche_LCD(2);
  }

  //Touche Panic
  Etat_TouchePanic[Etat_actuel] = digitalRead(No_PinPanic);
  if (Etat_TouchePanic[Etat_actuel] != Etat_TouchePanic[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change Panic ";
    AfficheDebug += Etat_TouchePanic[Etat_actuel] ? "OFF" : "ON";
    Serial.println(AfficheDebug);
#endif
    Etat_TouchePanic[Etat_avant] = Etat_TouchePanic[Etat_actuel];
    if (Etat_TouchePanic[Etat_avant] == ON) {
      for (int i = 0; i < 128; i++)  {
        noteOn(ChannelMelodie, i, 0x00);   //On arrête toutes les touches channel 1
        noteOn(ChannelBasses, i, 0x00);    //On arrête toutes les touches channel 2
        noteOn(ChannelAccord, i, 0x00);    //On arrête toutes les touches channel 3
      }
      delay(100);
      InitMidi();                //On réinitialise le Midi
    }
  }
  //Touche MidiInExt
  Etat_ToucheMidiInExt[Etat_actuel] = digitalRead(No_PinMidiInExt);
  if (Etat_ToucheMidiInExt[Etat_actuel] != Etat_ToucheMidiInExt[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change MidiInExt ";
    AfficheDebug += Etat_ToucheMidiInExt[Etat_actuel] ? "OFF" : "ON";
    Serial.println(AfficheDebug);
#endif
    Etat_ToucheMidiInExt[Etat_avant] = Etat_ToucheMidiInExt[Etat_actuel];
    Affiche_LCD(1);
  }
  //Entrée Analogique Vélocité
  Etat_ToucheVolume[Etat_actuel] = analogRead(No_PinVolume);
  if (Etat_ToucheVolume[Etat_actuel] != Etat_ToucheVolume[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change Volume ";
    AfficheDebug += Etat_ToucheVolume[Etat_actuel];
    Serial.println(AfficheDebug);
#endif
    Etat_ToucheVolume[Etat_avant] = Etat_ToucheVolume[Etat_actuel];
  }
  //Touche PousseTire
  Etat_TouchePousseTire[Etat_actuel] = digitalRead(No_PinPousseTire);
  if (Etat_TouchePousseTire[Etat_actuel] != Etat_TouchePousseTire[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change PousseTire ";
    AfficheDebug += Etat_TouchePousseTire[Etat_actuel] ? "OFF" : "ON";
    Serial.println(AfficheDebug);
#endif
    Etat_TouchePousseTire[Etat_avant] = Etat_TouchePousseTire[Etat_actuel];

    //inversion des notes Mélodie poussé-tiré en cours
    for (int i = 0; i < Nb_ToucheMelodie; i++) {
      if (Etat_Touche_Melodie[i][Etat_actuel] == ON) {
        noteOn(ChannelMelodie, Etat_Touche_Melodie[i][Note_encours], 0x00);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Melodie[i][No_Pin];
        AfficheDebug += " Note ";
        AfficheDebug += Etat_Touche_Melodie[i][Note_encours];
        AfficheDebug +=  " OFF ";
        Serial.println(AfficheDebug);
#endif
        PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Melodie[i][Note_encours] = Touche_Melodie[i][PousseTire];
        Etat_Touche_Melodie[i][Note_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

        Etat_Touche_Melodie[i][Note_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelMelodie, Etat_Touche_Melodie[i][Note_encours], MidiVelocity);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Melodie[i][No_Pin];
        AfficheDebug += " Note2 ";
        AfficheDebug += Etat_Touche_Melodie[i][Note_encours];
        AfficheDebug +=  " ON ";
        Serial.println(AfficheDebug);
#endif
      }
    }
    //inversion des notes Basses poussé-tiré en cours
    for (int i = 0; i < Nb_ToucheBasses; i++) {
      if (Etat_Touche_Basses[i][Etat_actuel] == ON) {
        if (Etat_ToucheBassesProf[Etat_actuel] == ON) {
          noteOn(ChannelBasses, Etat_Touche_Basses[i][Note1_encours], 0x00);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Basses[i][No_Pin];
          AfficheDebug += " Note1 ";
          AfficheDebug += Etat_Touche_Basses[i][Note1_encours];
          AfficheDebug +=  " OFF ";
          Serial.println(AfficheDebug);
#endif
          PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
          Etat_Touche_Basses[i][Note1_encours] = Touche_Basses[i][PousseTire];
          Etat_Touche_Basses[i][Note1_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

          Etat_Touche_Basses[i][Note1_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
          noteOn(ChannelBasses, Etat_Touche_Basses[i][Note1_encours], MidiVelocity);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Basses[i][No_Pin];
          AfficheDebug += " Note1 ";
          AfficheDebug += Etat_Touche_Basses[i][Note1_encours];
          AfficheDebug +=  " ON ";
          Serial.println(AfficheDebug);
#endif
        }
        noteOn(ChannelBasses, Etat_Touche_Basses[i][Note2_encours], 0x00);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Basses[i][No_Pin];
        AfficheDebug += " Note2 ";
        AfficheDebug += Etat_Touche_Basses[i][Note2_encours];
        AfficheDebug +=  " OFF ";
        Serial.println(AfficheDebug);
#endif
        PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Basses[i][Note2_encours] = Touche_Basses[i][PousseTire + 2];
        Etat_Touche_Basses[i][Note2_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

        Etat_Touche_Basses[i][Note2_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelBasses, Etat_Touche_Basses[i][Note2_encours], MidiVelocity);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Basses[i][No_Pin];
        AfficheDebug += " Note2 ";
        AfficheDebug += Etat_Touche_Basses[i][Note2_encours];
        AfficheDebug +=  " ON ";
        Serial.println(AfficheDebug);
#endif
        noteOn(ChannelBasses, Etat_Touche_Basses[i][Note3_encours], 0x00);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Basses[i][No_Pin];
        AfficheDebug += " Note3 ";
        AfficheDebug += Etat_Touche_Basses[i][Note3_encours];
        AfficheDebug +=  " OFF ";
        Serial.println(AfficheDebug);
#endif
        PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Basses[i][Note3_encours] = Touche_Basses[i][PousseTire + 4];
        Etat_Touche_Basses[i][Note3_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

        Etat_Touche_Basses[i][Note3_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelBasses, Etat_Touche_Basses[i][Note3_encours], MidiVelocity);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Basses[i][No_Pin];
        AfficheDebug += " Note3 ";
        AfficheDebug += Etat_Touche_Basses[i][Note3_encours];
        AfficheDebug +=  " ON ";
        Serial.println(AfficheDebug);
#endif
      }
    }
    //inversion des notes Accord poussé-tiré en cours
    for (int i = 0; i < Nb_ToucheAccord; i++) {
      if (Etat_Touche_Accord[i][Etat_actuel] == ON) {
        noteOn(ChannelAccord, Etat_Touche_Accord[i][Note1_encours], 0x00);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Accord[i][No_Pin];
        AfficheDebug += " Note1 ";
        AfficheDebug += Etat_Touche_Accord[i][Note1_encours];
        AfficheDebug +=  " OFF ";
        Serial.println(AfficheDebug);
#endif
        PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Accord[i][Note1_encours] = Touche_Accord[i][PousseTire];
        Etat_Touche_Accord[i][Note1_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

        Etat_Touche_Accord[i][Note1_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelAccord, Etat_Touche_Accord[i][Note1_encours], MidiVelocity);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Accord[i][No_Pin];
        AfficheDebug += " Note1 ";
        AfficheDebug += Etat_Touche_Accord[i][Note1_encours];
        AfficheDebug +=  " ON ";
        Serial.println(AfficheDebug);
#endif
        if (Etat_ToucheTierceOnOff[Etat_actuel] == ON) {
          noteOn(ChannelAccord, Etat_Touche_Accord[i][Note2_encours], 0x00);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Accord[i][No_Pin];
          AfficheDebug += " Note2 ";
          AfficheDebug += Etat_Touche_Accord[i][Note2_encours];
          AfficheDebug +=  " OFF ";
          Serial.println(AfficheDebug);
#endif
          PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
          Etat_Touche_Accord[i][Note2_encours] = Touche_Accord[i][PousseTire + 2];
          Etat_Touche_Accord[i][Note2_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          Etat_Touche_Accord[i][Note2_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
          noteOn(ChannelAccord, Etat_Touche_Accord[i][Note2_encours], MidiVelocity);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Accord[i][No_Pin];
          AfficheDebug += " Note2 ";
          AfficheDebug += Etat_Touche_Accord[i][Note2_encours];
          AfficheDebug +=  " ON ";
          Serial.println(AfficheDebug);
#endif
        }
        noteOn(ChannelAccord, Etat_Touche_Accord[i][Note3_encours], 0x00);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Accord[i][No_Pin];
        AfficheDebug += " Note3 ";
        AfficheDebug += Etat_Touche_Accord[i][Note3_encours];
        AfficheDebug +=  " OFF ";
        Serial.println(AfficheDebug);
#endif
        PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Accord[i][Note3_encours] = Touche_Accord[i][PousseTire + 4];
        Etat_Touche_Accord[i][Note3_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

        Etat_Touche_Accord[i][Note3_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelAccord, Etat_Touche_Accord[i][Note3_encours], MidiVelocity);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Accord[i][No_Pin];
        AfficheDebug += " Note3 ";
        AfficheDebug += Etat_Touche_Accord[i][Note3_encours];
        AfficheDebug +=  " ON ";
        Serial.println(AfficheDebug);
#endif
      }
    }
  }
  //Touche TierceOnOff
  Etat_ToucheTierceOnOff[Etat_actuel] = digitalRead(No_PinTierceOnOff);
  if (Etat_ToucheTierceOnOff[Etat_actuel] != Etat_ToucheTierceOnOff[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change TierceOnOff ";
    AfficheDebug += Etat_ToucheTierceOnOff[Etat_actuel] ? "OFF" : "ON";
    Serial.println(AfficheDebug);
#endif
    Etat_ToucheTierceOnOff[Etat_avant] = Etat_ToucheTierceOnOff[Etat_actuel];
    Affiche_LCD(3);
    //Arrêt des tierces en cours de jeu sur les seules touches accords
    if (Etat_ToucheTierceOnOff[Etat_actuel] == OFF)
      for (int i = 0; i < Nb_ToucheBasses; i += 2) {
        if (Etat_Touche_Basses[i][Etat_actuel] == ON) {
          noteOn(ChannelBasses, Etat_Touche_Basses[i][Note2_encours], 0x00);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Basses[i][No_Pin];
          AfficheDebug += " Note2 ";
          AfficheDebug += Etat_Touche_Basses[i][Note2_encours];
          AfficheDebug +=  " OFF ";
          Serial.println(AfficheDebug);
#endif
        }
      }
  }
  //Touche BassesProfondes
  Etat_ToucheBassesProf[Etat_actuel] = digitalRead(No_PinBassesProf);
  if (Etat_ToucheBassesProf[Etat_actuel] != Etat_ToucheBassesProf[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change BassesProfondes ";
    AfficheDebug += Etat_ToucheBassesProf[Etat_actuel] ? "OFF" : "ON";
    Serial.println(AfficheDebug);
#endif
    Etat_ToucheBassesProf[Etat_avant] = Etat_ToucheBassesProf[Etat_actuel];
    Affiche_LCD(3);
    //Arrêt des basses profondes en cours de jeu sur les seules touches basses
    if (Etat_ToucheBassesProf[Etat_actuel] == OFF)
      for (int i = 1; i < Nb_ToucheBasses; i += 2) {
        if (Etat_Touche_Basses[i][Etat_actuel] == ON) {
          noteOn(ChannelBasses, Etat_Touche_Basses[i][Note1_encours], 0x00);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Basses[i][No_Pin];
          AfficheDebug += " Note1 ";
          AfficheDebug += Etat_Touche_Basses[i][Note1_encours];
          AfficheDebug +=  " OFF ";
          Serial.println(AfficheDebug);
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
        noteOn(ChannelMelodie, Etat_Touche_Melodie[i][Note_encours], MidiVelocity);
      }
      else
        //Note on channel 1 (0x90), some note value (note), silent velocity (0x00):
        noteOn(ChannelMelodie, Etat_Touche_Melodie[i][Note_encours], 0x00);

      Etat_Touche_Melodie[i][Etat_avant] = Etat_Touche_Melodie[i][Etat_actuel];
#if defined (DEBUG)
      AfficheDebug = "Pin ";
      AfficheDebug += Touche_Melodie[i][No_Pin];
      AfficheDebug += " Note ";
      AfficheDebug += Etat_Touche_Melodie[i][Note_encours];
      AfficheDebug += Etat_Touche_Melodie[i][Etat_actuel] ? " OFF  " : " ON ";
      Serial.println(AfficheDebug);
#endif
    }
  }
  //Lecture Mux
  Lecture_Mux();

  //Lecture des poussoirs Registre, le dernier gagne !
  if (Etat_Mux1[PinMux1Flute] == ON) Etat_ToucheRegistre[Etat_actuel] = 0;       // Lecture poussoir Flute
  if (Etat_Mux1[PinMux1Basson] == ON) Etat_ToucheRegistre[Etat_actuel] = 1;      // Lecture poussoir Basson
  if (Etat_Mux2[PinMux2FluteBasson] == ON) Etat_ToucheRegistre[Etat_actuel] = 2; // Lecture poussoir FluteBasson
  if (Etat_ToucheRegistre[Etat_actuel] != Etat_ToucheRegistre[Etat_avant]) {
    (Etat_ToucheRegistre[Etat_avant] = Etat_ToucheRegistre[Etat_actuel]);
    Affiche_LCD(4);
#if defined (DEBUG)
    AfficheDebug = "Registre ";
    AfficheDebug += Libelle_Registre[Etat_ToucheRegistre[Etat_actuel]];
    Serial.println(AfficheDebug);
#endif
  }

  //Lecture Etat touche Accord
  for (int i = 0; i < Nb_ToucheAccord; i++) {
    Etat_Touche_Accord [i][Etat_actuel] = Etat_Mux1[Touche_Accord[i][No_Pin]]; // Lecture Touche)
    if (Etat_Touche_Accord[i][Etat_actuel] != Etat_Touche_Accord[i][Etat_avant]) {
      if (Etat_Touche_Accord[i][Etat_actuel] == ON) {
        PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Accord[i][Note1_encours] = Touche_Accord[i][PousseTire]; //Première note à jouer
        Etat_Touche_Accord[i][Note1_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
        Etat_Touche_Accord[i][Note1_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
        if (Etat_ToucheRenvAccord[Etat_actuel] > 0)
          Etat_Touche_Accord[i][Note1_encours] += 12; // 1er ou 2eme renversement On augmente la basse d'une octave
        noteOn(ChannelAccord, Etat_Touche_Accord[i][Note1_encours], MidiVelocity);

        //Jeu note 2 des Accord ou note 2 des Accords si Tierce
        if (Etat_ToucheTierceOnOff[Etat_actuel] == ON)
        {
          Etat_Touche_Accord[i][Note2_encours] = Touche_Accord[i][PousseTire + 2]; //Deuxième note à jouer
          Etat_Touche_Accord[i][Note2_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          Etat_Touche_Accord[i][Note2_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
          if (Etat_ToucheRenvAccord[Etat_actuel] == 2)
            Etat_Touche_Accord[i][Note2_encours] += 12; // 2eme renversement On augmente la tierce d'une octave
          noteOn(ChannelAccord, Etat_Touche_Accord[i][Note2_encours], MidiVelocity);
        }
        Etat_Touche_Accord[i][Note3_encours] = Touche_Accord[i][PousseTire + 4]; //Troisième note à jouer
        Etat_Touche_Accord[i][Note3_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
        Etat_Touche_Accord[i][Note3_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelAccord, Etat_Touche_Accord[i][Note3_encours], MidiVelocity);
      }
      else
      {
        //Note on channel 1 (0x90), some note value (note), silent velocity (0x00):
        noteOn(ChannelAccord, Etat_Touche_Accord[i][Note1_encours], 0x00);
        if (Etat_ToucheTierceOnOff[Etat_actuel] == ON)
          noteOn(ChannelAccord, Etat_Touche_Accord[i][Note2_encours], 0x00);
        noteOn(ChannelAccord, Etat_Touche_Accord[i][Note3_encours], 0x00);
      }
      Etat_Touche_Accord[i][Etat_avant] = Etat_Touche_Accord[i][Etat_actuel];
#if defined (DEBUG)
      AfficheDebug = "Acc ";
      AfficheDebug += Touche_Accord[i][No_Pin] + 1;
      AfficheDebug += " Note1 ";
      AfficheDebug += Etat_Touche_Accord[i][Note1_encours];
      AfficheDebug += Etat_Touche_Accord[i][Etat_actuel] ? " OFF  " : " ON ";
      Serial.println(AfficheDebug);
      if (Etat_ToucheTierceOnOff[Etat_actuel] == ON) {
        AfficheDebug = "Acc ";
        AfficheDebug += Touche_Accord[i][No_Pin] + 1;
        AfficheDebug += " Note2 ";
        AfficheDebug += Etat_Touche_Accord[i][Note2_encours];
        AfficheDebug += Etat_Touche_Accord[i][Etat_actuel] ? " OFF  " : " ON ";
        Serial.println(AfficheDebug);
      }
      AfficheDebug = "Acc ";
      AfficheDebug += Touche_Accord[i][No_Pin] + 1;
      AfficheDebug += " Note3 ";
      AfficheDebug += Etat_Touche_Accord[i][Note3_encours];
      AfficheDebug += Etat_Touche_Accord[i][Etat_actuel] ? " OFF  " : " ON ";
      Serial.println(AfficheDebug);
#endif
    }
  }

  //Lecture Etat touche Basse
  for (int i = 0; i < Nb_ToucheBasses; i++) {
    Etat_Touche_Basses [i][Etat_actuel] = Etat_Mux2[Touche_Basses[i][No_Pin]]; // Lecture Touche)
    if (Etat_Touche_Basses[i][Etat_actuel] != Etat_Touche_Basses[i][Etat_avant]) {
      if (Etat_Touche_Basses[i][Etat_actuel] == ON) {
        PousseTire = Etat_TouchePousseTire[Etat_actuel] ? 1 : 2;
        if (Etat_ToucheBassesProf[Etat_actuel] == ON)
        {
          Etat_Touche_Basses[i][Note1_encours] = Touche_Basses[i][PousseTire]; //Première note à jouer
          Etat_Touche_Basses[i][Note1_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          Etat_Touche_Basses[i][Note1_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
          noteOn(ChannelBasses, Etat_Touche_Basses[i][Note1_encours], MidiVelocity);
        }
        //Jeu note 2 des Basses
        Etat_Touche_Basses[i][Note2_encours] = Touche_Basses[i][PousseTire + 2]; //Deuxième note à jouer
        Etat_Touche_Basses[i][Note2_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
        Etat_Touche_Basses[i][Note2_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelBasses, Etat_Touche_Basses[i][Note2_encours], MidiVelocity);
        //Jeu note 3 des Basses
        Etat_Touche_Basses[i][Note3_encours] = Touche_Basses[i][PousseTire + 4]; //Troisième note à jouer
        Etat_Touche_Basses[i][Note3_encours] += (12 * Etat_ToucheOctave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
        Etat_Touche_Basses[i][Note3_encours] += Etat_ToucheTona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelBasses, Etat_Touche_Basses[i][Note3_encours], MidiVelocity);
      }
      else
      {
        //Note on channel 1 (0x90), some note value (note), silent velocity (0x00):
        if (Etat_ToucheBassesProf[Etat_actuel] == ON)
          noteOn(ChannelBasses, Etat_Touche_Basses[i][Note1_encours], 0x00);
        noteOn(ChannelBasses, Etat_Touche_Basses[i][Note2_encours], 0x00);
        noteOn(ChannelBasses, Etat_Touche_Basses[i][Note3_encours], 0x00);
      }
      Etat_Touche_Basses[i][Etat_avant] = Etat_Touche_Basses[i][Etat_actuel];
#if defined (DEBUG)
      if (Etat_ToucheBassesProf[Etat_actuel] == ON) {
        AfficheDebug = "Bas ";
        AfficheDebug += Touche_Basses[i][No_Pin] + 1;
        AfficheDebug += " Note1 ";
        AfficheDebug += Etat_Touche_Basses[i][Note1_encours];
        AfficheDebug += Etat_Touche_Basses[i][Etat_actuel] ? " OFF  " : " ON ";
        Serial.println(AfficheDebug);
      }
      AfficheDebug = "Bas ";
      AfficheDebug += Touche_Basses[i][No_Pin] + 1;
      AfficheDebug += " Note2 ";
      AfficheDebug += Etat_Touche_Basses[i][Note2_encours];
      AfficheDebug += Etat_Touche_Basses[i][Etat_actuel] ? " OFF  " : " ON ";
      Serial.println(AfficheDebug);
      AfficheDebug = "Bas ";
      AfficheDebug += Touche_Basses[i][No_Pin] + 1;
      AfficheDebug += " Note3 ";
      AfficheDebug += Etat_Touche_Basses[i][Note3_encours];
      AfficheDebug += Etat_Touche_Basses[i][Etat_actuel] ? " OFF  " : " ON ";
      Serial.println(AfficheDebug);
#endif
    }
  }
}
