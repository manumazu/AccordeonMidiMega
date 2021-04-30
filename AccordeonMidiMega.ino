#define DEBUG          // si Debug sur Moniteur série standard
#define DEBUGCAPTEUR   // si Debug capteur sur Moniteur série standard et écran LCD
// Choix du capteur : un seul define parmi les x suivants
//#define ANALOGPEDALE  // si pas de capteur de force, remplacé par pédale volume analogique et bouton pousse-tire
//#define CAPTEURFORCEHX711     // Capteur de force HX711
#define CAPTEURPRESSIONBMP280  // Capteur de pression BMP280
//#define CAPTEURPRESSIONHONEY  // Capteur de pression Honeywell en analogique
//Fin des define choix du capteur

#define MIDIVELOCITY     // Application du calcul après lecture capteur/pédale sur la vélocity, sinon Midi CC # 7 sur les trois canaux

const String Version = "Robert_20210430-1   ";

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
#if defined (ANALOGPEDALE)
#include "Gestion_CapteurAnalogPedale.h"
#endif
#if defined (CAPTEURFORCEHX711)
#include "Gestion_CapteurForceHX711.h"
#endif
#if defined (CAPTEURPRESSIONBMP280)
#include "Gestion_CapteurPressionBMP280.h"
#endif
#if defined (CAPTEURPRESSIONHONEY)
#include "Gestion_CapteurPressionHoney.h"
#endif

//Affichage de la ligne 1 ou 2 sur l'afficheur LCD
void Affiche_LCD(int ligne) {
  int i;

  switch (ligne) {
    case 1 :  //Instrument
      if (Etat_MidiInExt[Etat_actuel] == Midi_Interne) {
        Affiche = Libelle_Instr_Midi1[Etat_Instrument[Etat_actuel]];
      }
      else  {//Midi externe on affiche le numéro de Bank et Program Midi
        Affiche = "M ";
        Affiche += Etat_MidiMSB[Etat_actuel];
        Affiche += " L ";
        Affiche += Etat_MidiLSB[Etat_actuel];
        Affiche += " Program ";
        Affiche += Etat_InstrumentE[Etat_actuel];
        for (i = Affiche.length(); i < 20; i++)
          Affiche += " ";  // On remplit à blanc
      }
      lcd.setCursor(0, 0);
      lcd.print(Affiche);
      break;
    case 2: // Tonalité
      Affiche = Libelle_Tona[6 + Etat_Tona[Etat_actuel]];
      lcd.setCursor(0, 1);
      lcd.print(Affiche);
      Affiche = "V ";
      Affiche += Etat_Volume_Midi[Etat_actuel];
      for (i = Affiche.length(); i < 5; i++)
        Affiche += " ";  // On remplit à blanc
      lcd.setCursor(14, 1);
      lcd.print(Affiche);
      break;
    case 3 :  // Octave - Renversement - Registre
      Affiche = "Oct";
      Affiche += Etat_Octave[Etat_actuel]  < 0 ? "" : "+";
      Affiche += Etat_Octave[Etat_actuel];
      lcd.setCursor(0, 2);
      lcd.print(Affiche);
      Affiche = "Ren";
      Affiche += Etat_RenvAccord[Etat_actuel]  < 0 ? "" : "+";
      Affiche += Etat_RenvAccord[Etat_actuel];
      lcd.setCursor(7, 2);
      lcd.print(Affiche);
      Affiche = Libelle_Registre[Etat_Registre[Etat_actuel]];
      lcd.setCursor(14, 2);
      lcd.print(Affiche);
      break;
    case 4: // Basses Profondes - Tierces
      Affiche = "Bas.";
      Affiche += Etat_BassesProf[Etat_actuel] ? "Norm " : "Prof ";
      lcd.setCursor(0, 3);
      lcd.print(Affiche);
      Affiche = Etat_TierceOnOff[Etat_actuel] ?  "Sans" : "Avec";
      Affiche += " tierce";
      lcd.setCursor(9, 3);
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

#if defined (DEBUG)
  Serial.begin (9600);    //pour le debug écran PC 9600
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Debug");
#endif

  // initialisation pin digital du tableau de Touches Mélodie en INPUT_PULLUP
  for (int i = 0; i < Nb_ToucheMelodie; i++) {
    pinMode(Touche_Melodie[i][No_Pin], INPUT_PULLUP);
  }
  // Initialisation touche Regsitre
  pinMode(No_PinBasson, INPUT_PULLUP);
  pinMode(No_PinFluteBasson, INPUT_PULLUP);
  pinMode(No_PinFlute, INPUT_PULLUP);

  // deMute Ampli
  pinMode(No_PinMute, OUTPUT);
  digitalWrite(No_PinMute, HIGH);

  // Initialisation des codeurs instrument, tonalité, octave et Midi MSB LSB
  Init_Codeur();

  // Initialisation des multiplexeurs Basses,Accord et spare
  Init_Mux();

  // Initialisation du capteur de soufflet
  Etat_Volume_Millis[Etat_actuel] = Etat_Volume_Millis[Etat_avant] = millis();
  Init_CapteurSoufflet();

  //Init fonctions Midi
  InitMidi();
#if defined (MIDIVELOCITY)
  MidiVelocity = 0;
#else
  Volume(ChannelMelodie, Etat_Volume_Midi[Etat_actuel]);
  Volume(ChannelBasses, Etat_Volume_Midi[Etat_actuel]);
  Volume(ChannelAccord, Etat_Volume_Midi[Etat_actuel]);
#endif
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
  if (Etat_MidiInExt[Etat_actuel] == Midi_Interne) {
    if (Etat_Instrument[Etat_actuel] != Etat_Instrument[Etat_avant]) {
      Instrument(ChannelMelodie, Etat_Instrument[Etat_actuel], Midi_Int);
      Instrument(ChannelBasses, Etat_Instrument[Etat_actuel], Midi_Int);
      Instrument(ChannelAccord, Etat_Instrument[Etat_actuel], Midi_Int);
      Etat_Instrument[Etat_avant] = Etat_Instrument[Etat_actuel];
#if defined (DEBUG)
      AfficheDebug = "Reset Instrument ";
      AfficheDebug += Etat_Instrument[Etat_actuel];
      Serial.println(AfficheDebug);
#endif
      Affiche_LCD(1);
    }
  }
  else   {
    if (Etat_InstrumentE[Etat_actuel] != Etat_InstrumentE[Etat_avant]) {
      Instrument(ChannelMelodie, Etat_InstrumentE[Etat_actuel], Midi_Ext);
      Instrument(ChannelBasses, Etat_InstrumentE[Etat_actuel], Midi_Ext);
      Instrument(ChannelAccord, Etat_InstrumentE[Etat_actuel], Midi_Ext);
      Etat_InstrumentE[Etat_avant] = Etat_InstrumentE[Etat_actuel];
#if defined (DEBUG)
      AfficheDebug = "Reset InstrumentExt ";
      AfficheDebug += Etat_InstrumentE[Etat_actuel];
      Serial.println(AfficheDebug);
#endif
      Affiche_LCD(1);
    }
  }

  if (retour == ON ) {
    Affiche_LCD(2);       // On rafraichit l'affichage Tona
    Affiche_LCD(3);       // On rafraichit l'affichage Octa
  }
  //Lecture des codeurs instrument, tonalité, octave et Midi MSB LSB

  //Codeur Midi LSB et MSB
  if (Etat_MidiInExt[Etat_actuel] == ON) {
    if (Etat_SwitchMLSB[Etat_actuel] == ON) {
      Etat_MidiLSB[Etat_actuel] = Lecture_Codeur(MLSB_codeur, true);
      if (Etat_MidiLSB[Etat_actuel] != Etat_MidiLSB[Etat_avant]) {
#if defined (DEBUG)
        AfficheDebug = "Change LSB ";
        AfficheDebug += Etat_MidiLSB[Etat_actuel];
        Serial.println(AfficheDebug);
#endif
        Etat_MidiLSB[Etat_avant] = Etat_MidiLSB[Etat_actuel];
        BankLSB = Etat_MidiLSB[Etat_avant];
        Instrument(ChannelMelodie, Etat_InstrumentE[Etat_actuel], Midi_Ext);
        Instrument(ChannelBasses, Etat_InstrumentE[Etat_actuel], Midi_Ext);
        Instrument(ChannelAccord, Etat_InstrumentE[Etat_actuel], Midi_Ext);
        Affiche_LCD(1);
      }
    }
    else
    {
      Etat_MidiMSB[Etat_actuel] = Lecture_Codeur(MMSB_codeur, true);
      if (Etat_MidiMSB[Etat_actuel] != Etat_MidiMSB[Etat_avant]) {
#if defined (DEBUG)
        AfficheDebug = "Change MSB ";
        AfficheDebug += Etat_MidiMSB[Etat_actuel];
        Serial.println(AfficheDebug);
#endif
        Etat_MidiMSB[Etat_avant] = Etat_MidiMSB[Etat_actuel];
        BankMSB = Etat_MidiMSB[Etat_avant];
        Instrument(ChannelMelodie, Etat_InstrumentE[Etat_actuel], Midi_Ext);
        Instrument(ChannelBasses, Etat_InstrumentE[Etat_actuel], Midi_Ext);
        Instrument(ChannelAccord, Etat_InstrumentE[Etat_actuel], Midi_Ext);
        Affiche_LCD(1);
      }
    }
  }

  //Codeur instrument
  if (Etat_MidiInExt[Etat_actuel] == OFF) {
    Etat_Instrument[Etat_actuel] = Lecture_Codeur(inst_codeur, false);
    if (Etat_Instrument[Etat_actuel] != Etat_Instrument[Etat_avant]) {
#if defined (DEBUG)
      AfficheDebug = "ChangeCodeur Instrument ";
      AfficheDebug += Etat_Instrument[Etat_actuel];
      Serial.println(AfficheDebug);
#endif
      Etat_Instrument[Etat_avant] = Etat_Instrument[Etat_actuel];
      Instrument(ChannelMelodie, Etat_Instrument[Etat_actuel], Midi_Int);
      Instrument(ChannelBasses, Etat_Instrument[Etat_actuel], Midi_Int);
      Instrument(ChannelAccord, Etat_Instrument[Etat_actuel], Midi_Int);
      Affiche_LCD(1);
    }
  }
  else {
    Etat_InstrumentE[Etat_actuel] = Lecture_Codeur(insE_codeur, true);
    if (Etat_InstrumentE[Etat_actuel] != Etat_InstrumentE[Etat_avant]) {
#if defined (DEBUG)
      AfficheDebug = "ChangeCodeur InstrumentExt ";
      AfficheDebug += Etat_InstrumentE[Etat_actuel];
      Serial.println(AfficheDebug);
#endif
      Etat_InstrumentE[Etat_avant] = Etat_InstrumentE[Etat_actuel];
      Instrument(ChannelMelodie, Etat_InstrumentE[Etat_actuel], Midi_Ext);
      Instrument(ChannelBasses, Etat_InstrumentE[Etat_actuel], Midi_Ext);
      Instrument(ChannelAccord, Etat_InstrumentE[Etat_actuel], Midi_Ext);
      Affiche_LCD(1);
    }
  }
  //Codeur Tona
  Etat_Tona[Etat_actuel] = Lecture_Codeur(tona_codeur, false);
  if (Etat_Tona[Etat_actuel] != Etat_Tona[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change Tona ";
    AfficheDebug += Etat_Tona[Etat_actuel];
    Serial.println(AfficheDebug);
#endif
    Etat_Tona[Etat_avant] = Etat_Tona[Etat_actuel];
    Affiche_LCD(2);
  }

  //Codeur Octave
  Etat_Octave[Etat_actuel] = Lecture_Codeur(octa_codeur, false);
  if (Etat_Octave[Etat_actuel] != Etat_Octave[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change Octave ";
    AfficheDebug += Etat_Octave[Etat_actuel];
    Serial.println(AfficheDebug);
#endif
    Etat_Octave[Etat_avant] = Etat_Octave[Etat_actuel];
    Affiche_LCD(3);
  }

  //Codeur Renversement
  Etat_RenvAccord[Etat_actuel] = Lecture_Codeur(renv_codeur, false);
  if (Etat_RenvAccord[Etat_actuel] != Etat_RenvAccord[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change Renversement ";
    AfficheDebug += Etat_RenvAccord[Etat_actuel];
    Serial.println(AfficheDebug);
#endif
    Etat_RenvAccord[Etat_avant] = Etat_RenvAccord[Etat_actuel];
    Affiche_LCD(3);
  }

  //Touche Panic
  Etat_Panic[Etat_actuel] = Etat_Mux[No_PinMuxPanic];
  if (Etat_Panic[Etat_actuel] != Etat_Panic[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change Panic ";
    AfficheDebug += Etat_Panic[Etat_actuel] ? "OFF" : "ON";
    Serial.println(AfficheDebug);
#endif
    Etat_Panic[Etat_avant] = Etat_Panic[Etat_actuel];
    if (Etat_Panic[Etat_avant] == ON) {
      for (int i = 0; i < 128; i++)  {
        noteOff(ChannelMelodie, i);   //On arrête toutes les touches channel 1
        noteOff(ChannelBasses, i);    //On arrête toutes les touches channel 2
        noteOff(ChannelAccord, i);    //On arrête toutes les touches channel 3
      }
      delay(100);
      InitMidi();                //On réinitialise le Midi
    }
  }
  //Touche MidiInExt
  Etat_MidiInExt[Etat_actuel] = Etat_Mux[No_PinMuxMidiInExt];
  if (Etat_MidiInExt[Etat_actuel] != Etat_MidiInExt[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change MidiInExt ";
    AfficheDebug += Etat_MidiInExt[Etat_actuel] ? "OFF" : "ON";
    Serial.println(AfficheDebug);
#endif
    Etat_MidiInExt[Etat_avant] = Etat_MidiInExt[Etat_actuel];
    Affiche_LCD(1);
  }
  Lecture_CapteurSoufflet();

#if defined (ANALOGPEDALE)
  //Touche PousseTire
  Etat_PousseTire[Etat_actuel] = digitalRead(No_PinPousseTire);
#else
  if (Etat_Capteur[Etat_actuel] != Etat_Capteur[Etat_avant]) {  // Test valeur capteur pour le pousse tire
    if (Etat_Capteur[Etat_actuel] >= Etat_Capteur[Etat_avant]) Etat_PousseTire[Etat_actuel] = ON;
    else Etat_PousseTire[Etat_actuel] = OFF;
  }
#endif
  if (Etat_Volume_Midi[Etat_actuel] != Etat_Volume_Midi[Etat_avant]) {
#if defined (DEBUGtoto)
    AfficheDebug = "Change Valeur capteur ";
    AfficheDebug += Etat_Capteur[Etat_actuel];
    AfficheDebug += " Valeur calculée ";
    AfficheDebug += Etat_Volume_Midi[Etat_actuel];
    Serial.println(AfficheDebug);
#endif
#if defined (MIDIVELOCITY)
    MidiVelocity = Etat_Volume_Midi[Etat_actuel];
#else
    Volume(ChannelMelodie, Etat_Volume_Midi[Etat_actuel]);
    Volume(ChannelBasses, Etat_Volume_Midi[Etat_actuel]);
    Volume(ChannelAccord, Etat_Volume_Midi[Etat_actuel]);
#endif
  }
  Etat_Capteur[Etat_avant] = Etat_Capteur[Etat_actuel];
  Etat_Volume_Midi[Etat_avant] = Etat_Volume_Midi[Etat_actuel];
  Etat_Volume_Millis[Etat_avant] = Etat_Volume_Millis[Etat_actuel];
  Affiche_LCD(2);

  if (Etat_PousseTire[Etat_actuel] != Etat_PousseTire[Etat_avant]) {
    Etat_PousseTire[Etat_avant] = Etat_PousseTire[Etat_actuel];
#if defined (DEBUG)
    AfficheDebug = "Change PousseTire ";
    AfficheDebug += Etat_PousseTire[Etat_actuel] ? "OFF" : "ON";
    Serial.println(AfficheDebug);
#endif

    //inversion des notes Mélodie poussé-tiré en cours
    for (int i = 0; i < Nb_ToucheMelodie; i++) {
      if (Etat_Touche_Melodie[i][Etat_actuel] == ON) {
        if (Etat_Registre[Etat_actuel] < RegistreFlute) {
          noteOff(ChannelMelodie, Etat_Touche_Melodie[i][Note1_encours]);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Melodie[i][No_Pin];
          AfficheDebug += " Note ";
          AfficheDebug += Etat_Touche_Melodie[i][Note1_encours];
          AfficheDebug +=  " OFF ";
          Serial.println(AfficheDebug);
#endif
          PousseTire = Etat_PousseTire[Etat_actuel] ? 1 : 2;
          Etat_Touche_Melodie[i][Note1_encours] = Touche_Melodie[i][PousseTire];
          Etat_Touche_Melodie[i][Note1_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          Etat_Touche_Melodie[i][Note1_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
          noteOn(ChannelMelodie, Etat_Touche_Melodie[i][Note1_encours], MidiVelocity);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Melodie[i][No_Pin];
          AfficheDebug += " Note2 ";
          AfficheDebug += Etat_Touche_Melodie[i][Note1_encours];
          AfficheDebug +=  " ON Velo ";
          AfficheDebug += MidiVelocity;
          Serial.println(AfficheDebug);
#endif
        }
        if (Etat_Registre[Etat_actuel] > RegistreBasson) {
          noteOff(ChannelMelodie, Etat_Touche_Melodie[i][Note2_encours]);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Melodie[i][No_Pin];
          AfficheDebug += " Note ";
          AfficheDebug += Etat_Touche_Melodie[i][Note2_encours];
          AfficheDebug +=  " OFF ";
          Serial.println(AfficheDebug);
#endif
          PousseTire = Etat_PousseTire[Etat_actuel] ? 3 : 4;
          Etat_Touche_Melodie[i][Note2_encours] = Touche_Melodie[i][PousseTire];
          Etat_Touche_Melodie[i][Note2_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          Etat_Touche_Melodie[i][Note2_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
          noteOn(ChannelMelodie, Etat_Touche_Melodie[i][Note2_encours], MidiVelocity);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Melodie[i][No_Pin];
          AfficheDebug += " Note2 ";
          AfficheDebug += Etat_Touche_Melodie[i][Note2_encours];
          AfficheDebug +=  " ON Velo ";
          AfficheDebug += MidiVelocity;
          Serial.println(AfficheDebug);
#endif
        }
      }
    }
    //inversion des notes Basses poussé-tiré en cours
    for (int i = 0; i < Nb_ToucheBasses; i++) {
      if (Etat_Touche_Basses[i][Etat_actuel] == ON) {
        if (Etat_BassesProf[Etat_actuel] == ON) {
          noteOff(ChannelBasses, Etat_Touche_Basses[i][Note1_encours]);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Basses[i][No_Pin];
          AfficheDebug += " Note1 ";
          AfficheDebug += Etat_Touche_Basses[i][Note1_encours];
          AfficheDebug +=  " OFF ";
          Serial.println(AfficheDebug);
#endif
          PousseTire = Etat_PousseTire[Etat_actuel] ? 1 : 2;
          Etat_Touche_Basses[i][Note1_encours] = Touche_Basses[i][PousseTire];
          Etat_Touche_Basses[i][Note1_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

          Etat_Touche_Basses[i][Note1_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
          noteOn(ChannelBasses, Etat_Touche_Basses[i][Note1_encours], MidiVelocity);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Basses[i][No_Pin];
          AfficheDebug += " Note1 ";
          AfficheDebug += Etat_Touche_Basses[i][Note1_encours];
          AfficheDebug +=  " ON Velo ";
          AfficheDebug += MidiVelocity;
          Serial.println(AfficheDebug);
#endif
        }
        noteOff(ChannelBasses, Etat_Touche_Basses[i][Note2_encours]);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Basses[i][No_Pin];
        AfficheDebug += " Note2 ";
        AfficheDebug += Etat_Touche_Basses[i][Note2_encours];
        AfficheDebug +=  " OFF ";
        Serial.println(AfficheDebug);
#endif
        PousseTire = Etat_PousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Basses[i][Note2_encours] = Touche_Basses[i][PousseTire + 2];
        Etat_Touche_Basses[i][Note2_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

        Etat_Touche_Basses[i][Note2_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelBasses, Etat_Touche_Basses[i][Note2_encours], MidiVelocity);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Basses[i][No_Pin];
        AfficheDebug += " Note2 ";
        AfficheDebug += Etat_Touche_Basses[i][Note2_encours];
        AfficheDebug +=  " ON Velo ";
        AfficheDebug += MidiVelocity;
        Serial.println(AfficheDebug);
#endif
        noteOff(ChannelBasses, Etat_Touche_Basses[i][Note3_encours]);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Basses[i][No_Pin];
        AfficheDebug += " Note3 ";
        AfficheDebug += Etat_Touche_Basses[i][Note3_encours];
        AfficheDebug +=  " OFF ";
        Serial.println(AfficheDebug);
#endif
        PousseTire = Etat_PousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Basses[i][Note3_encours] = Touche_Basses[i][PousseTire + 4];
        Etat_Touche_Basses[i][Note3_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

        Etat_Touche_Basses[i][Note3_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelBasses, Etat_Touche_Basses[i][Note3_encours], MidiVelocity);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Basses[i][No_Pin];
        AfficheDebug += " Note3 ";
        AfficheDebug += Etat_Touche_Basses[i][Note3_encours];
        AfficheDebug +=  " ON Velo ";
        AfficheDebug += MidiVelocity;
        Serial.println(AfficheDebug);
#endif
      }
    }
    //inversion des notes Accord poussé-tiré en cours
    for (int i = 0; i < Nb_ToucheAccord; i++) {
      if (Etat_Touche_Accord[i][Etat_actuel] == ON) {
        noteOff(ChannelAccord, Etat_Touche_Accord[i][Note1_encours]);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Accord[i][No_Pin];
        AfficheDebug += " Note1 ";
        AfficheDebug += Etat_Touche_Accord[i][Note1_encours];
        AfficheDebug +=  " OFF ";
        Serial.println(AfficheDebug);
#endif
        PousseTire = Etat_PousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Accord[i][Note1_encours] = Touche_Accord[i][PousseTire];
        Etat_Touche_Accord[i][Note1_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

        Etat_Touche_Accord[i][Note1_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelAccord, Etat_Touche_Accord[i][Note1_encours], MidiVelocity);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Accord[i][No_Pin];
        AfficheDebug += " Note1 ";
        AfficheDebug += Etat_Touche_Accord[i][Note1_encours];
        AfficheDebug +=  " ON Velo ";
        AfficheDebug += MidiVelocity;
        Serial.println(AfficheDebug);
#endif
        if (Etat_TierceOnOff[Etat_actuel] == ON) {
          noteOff(ChannelAccord, Etat_Touche_Accord[i][Note2_encours]);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Accord[i][No_Pin];
          AfficheDebug += " Note2 ";
          AfficheDebug += Etat_Touche_Accord[i][Note2_encours];
          AfficheDebug +=  " OFF ";
          Serial.println(AfficheDebug);
#endif
          PousseTire = Etat_PousseTire[Etat_actuel] ? 1 : 2;
          Etat_Touche_Accord[i][Note2_encours] = Touche_Accord[i][PousseTire + 2];
          Etat_Touche_Accord[i][Note2_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          Etat_Touche_Accord[i][Note2_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
          noteOn(ChannelAccord, Etat_Touche_Accord[i][Note2_encours], MidiVelocity);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Accord[i][No_Pin];
          AfficheDebug += " Note2 ";
          AfficheDebug += Etat_Touche_Accord[i][Note2_encours];
          AfficheDebug +=  " ON Velo ";
          AfficheDebug += MidiVelocity;
          Serial.println(AfficheDebug);
#endif
        }
        noteOff(ChannelAccord, Etat_Touche_Accord[i][Note3_encours]);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Accord[i][No_Pin];
        AfficheDebug += " Note3 ";
        AfficheDebug += Etat_Touche_Accord[i][Note3_encours];
        AfficheDebug +=  " OFF ";
        Serial.println(AfficheDebug);
#endif
        PousseTire = Etat_PousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Accord[i][Note3_encours] = Touche_Accord[i][PousseTire + 4];
        Etat_Touche_Accord[i][Note3_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons

        Etat_Touche_Accord[i][Note3_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelAccord, Etat_Touche_Accord[i][Note3_encours], MidiVelocity);
#if defined (DEBUG)
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Accord[i][No_Pin];
        AfficheDebug += " Note3 ";
        AfficheDebug += Etat_Touche_Accord[i][Note3_encours];
        AfficheDebug +=  " ON Velo ";
        AfficheDebug += MidiVelocity;
        Serial.println(AfficheDebug);
#endif
      }
    }
  }
  //Touche TierceOnOff
  Etat_TierceOnOff[Etat_actuel] = Etat_Mux[No_PinMuxTierceOnOff];
  if (Etat_TierceOnOff[Etat_actuel] != Etat_TierceOnOff[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change TierceOnOff ";
    AfficheDebug += Etat_TierceOnOff[Etat_actuel] ? "OFF" : "ON";
    Serial.println(AfficheDebug);
#endif
    Etat_TierceOnOff[Etat_avant] = Etat_TierceOnOff[Etat_actuel];
    Affiche_LCD(4);
    //Arrêt des tierces en cours de jeu sur les seules touches accords
    if (Etat_TierceOnOff[Etat_actuel] == OFF)
      for (int i = 0; i < Nb_ToucheAccord; i ++) {
        if (Etat_Touche_Accord[i][Etat_actuel] == ON) {
          noteOff(ChannelBasses, Etat_Touche_Accord[i][Note2_encours]);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Accord[i][No_Pin];
          AfficheDebug += " Note2 ";
          AfficheDebug += Etat_Touche_Accord[i][Note2_encours];
          AfficheDebug +=  " OFF ";
          Serial.println(AfficheDebug);
#endif
        }
      }
    else
      for (int i = 0; i < Nb_ToucheAccord; i ++)
        if (Etat_Touche_Accord[i][Etat_actuel] == ON) {
          Etat_Touche_Accord[i][Note2_encours] = Touche_Accord[i][PousseTire + 2]; //Deuxième note à jouer
          Etat_Touche_Accord[i][Note2_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          Etat_Touche_Accord[i][Note2_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
          if (Etat_RenvAccord[Etat_actuel] == 2)
            Etat_Touche_Accord[i][Note2_encours] += 12; // 2eme renversement On augmente la tierce d'une octave
          noteOn(ChannelAccord, Etat_Touche_Accord[i][Note2_encours], MidiVelocity);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Accord[i][No_Pin];
          AfficheDebug += " Note2 ";
          AfficheDebug += Etat_Touche_Accord[i][Note2_encours];
          AfficheDebug +=  " ON Velo ";
          AfficheDebug += MidiVelocity;
          Serial.println(AfficheDebug);
#endif
        }

  }
  //Touche BassesProfondes
  Etat_BassesProf[Etat_actuel] = Etat_Mux[No_PinMuxBassesProf];
  if (Etat_BassesProf[Etat_actuel] != Etat_BassesProf[Etat_avant]) {
#if defined (DEBUG)
    AfficheDebug = "Change BassesProfondes ";
    AfficheDebug += Etat_BassesProf[Etat_actuel] ? "OFF" : "ON";
    Serial.println(AfficheDebug);
#endif
    Etat_BassesProf[Etat_avant] = Etat_BassesProf[Etat_actuel];
    Affiche_LCD(4);
    //Arrêt des basses profondes en cours de jeu sur les seules touches basses
    if (Etat_BassesProf[Etat_actuel] == OFF)
      for (int i = 0; i < Nb_ToucheBasses; i ++) {
        if (Etat_Touche_Basses[i][Etat_actuel] == ON) {
          noteOff(ChannelBasses, Etat_Touche_Basses[i][Note1_encours]);
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
    else
      for (int i = 0; i < Nb_ToucheBasses; i ++) {
        if (Etat_Touche_Basses[i][Etat_actuel] == ON) {
          Etat_Touche_Basses[i][Note1_encours] = Touche_Basses[i][PousseTire]; //Première note à jouer
          Etat_Touche_Basses[i][Note1_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          Etat_Touche_Basses[i][Note1_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
          noteOn(ChannelBasses, Etat_Touche_Basses[i][Note1_encours], MidiVelocity);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Basses[i][No_Pin];
          AfficheDebug += " Note1 ";
          AfficheDebug += Etat_Touche_Basses[i][Note1_encours];
          AfficheDebug +=  " ON Velo ";
          AfficheDebug += MidiVelocity;
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
        PousseTire = Etat_PousseTire[Etat_actuel] ? 1 : 2;
        if (Etat_Registre[Etat_actuel] < RegistreFlute) //On traitre les notes Basson
        {
          Etat_Touche_Melodie[i][Note1_encours] = Touche_Melodie[i][PousseTire];
          Etat_Touche_Melodie[i][Note1_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          Etat_Touche_Melodie[i][Note1_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
          noteOn(ChannelMelodie, Etat_Touche_Melodie[i][Note1_encours], MidiVelocity);
        }
        PousseTire = Etat_PousseTire[Etat_actuel] ? 3 : 4;
        if (Etat_Registre[Etat_actuel] > RegistreBasson) //On traitre les notes Flûte
        {
          Etat_Touche_Melodie[i][Note2_encours] = Touche_Melodie[i][PousseTire];
          Etat_Touche_Melodie[i][Note2_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          Etat_Touche_Melodie[i][Note2_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
          noteOn(ChannelMelodie, Etat_Touche_Melodie[i][Note2_encours], MidiVelocity);
        }
      }
      else {
        if (Etat_Registre[Etat_actuel] < RegistreFlute)
          noteOff(ChannelMelodie, Etat_Touche_Melodie[i][Note1_encours]);
        if (Etat_Registre[Etat_actuel] > RegistreBasson)
          noteOff(ChannelMelodie, Etat_Touche_Melodie[i][Note2_encours]);
      }
      Etat_Touche_Melodie[i][Etat_avant] = Etat_Touche_Melodie[i][Etat_actuel];
#if defined (DEBUG)
      if (Etat_Registre[Etat_actuel] < RegistreFlute) {
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Melodie[i][No_Pin];
        AfficheDebug += " Note ";
        AfficheDebug += Etat_Touche_Melodie[i][Note1_encours];
        if (Etat_Touche_Melodie[i][Etat_actuel] == ON) {
          AfficheDebug += " ON Velo ";
          AfficheDebug += MidiVelocity;
        }
        Serial.println(AfficheDebug);
      }
      if (Etat_Registre[Etat_actuel] > RegistreBasson) {
        AfficheDebug = "Pin ";
        AfficheDebug += Touche_Melodie[i][No_Pin];
        AfficheDebug += " Note ";
        AfficheDebug += Etat_Touche_Melodie[i][Note2_encours];
        if (Etat_Touche_Melodie[i][Etat_actuel] == ON) {
          AfficheDebug += " ON Velo ";
          AfficheDebug += MidiVelocity;
        }
        else AfficheDebug += " OFF  ";
        Serial.println(AfficheDebug);
      }
#endif
    }
  }
  //Lecture Mux
  Lecture_Mux();

  //Lecture des poussoirs Registre, le dernier gagne !
  if (digitalRead(No_PinBasson) == ON) Etat_Registre[Etat_actuel] = RegistreBasson;           // Lecture poussoir Basson
  if (digitalRead(No_PinFluteBasson) == ON) Etat_Registre[Etat_actuel] = RegistreFluteBasson; // Lecture poussoir FluteBasson
  if (digitalRead(No_PinFlute) == ON) Etat_Registre[Etat_actuel] = RegistreFlute;             // Lecture poussoir Flute
  if (Etat_Registre[Etat_actuel] != Etat_Registre[Etat_avant]) {
    //changement/ajout registre des notes Mélodie en cours
    for (int i = 0; i < Nb_ToucheMelodie; i++)
      if (Etat_Touche_Melodie[i][Etat_actuel] == ON) {
        if ((Etat_Registre[Etat_actuel] == RegistreFlute) && (Etat_Registre[Etat_avant] < RegistreFlute) ) {
          noteOff(ChannelMelodie, Etat_Touche_Melodie[i][Note1_encours]);                    //On stoppe les notes Basson
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Melodie[i][No_Pin];
          AfficheDebug += " Note ";
          AfficheDebug += Etat_Touche_Melodie[i][Note1_encours];
          AfficheDebug +=  " OFF ";
          Serial.println(AfficheDebug);
#endif
        }
        if ((Etat_Registre[Etat_actuel] == RegistreBasson) && (Etat_Registre[Etat_avant] > RegistreBasson) ) {
          noteOff(ChannelMelodie, Etat_Touche_Melodie[i][Note2_encours]);                   //On stoppe les notes Flûte
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Melodie[i][No_Pin];
          AfficheDebug += " Note ";
          AfficheDebug += Etat_Touche_Melodie[i][Note2_encours];
          AfficheDebug +=  " OFF ";
          Serial.println(AfficheDebug);
#endif
        }
        if ((Etat_Registre[Etat_actuel] < RegistreFlute) && (Etat_Registre[Etat_avant] == RegistreFlute)) {  //On joue le Basson
          PousseTire = Etat_PousseTire[Etat_actuel] ? 1 : 2;
          Etat_Touche_Melodie[i][Note1_encours] = Touche_Melodie[i][PousseTire];
          Etat_Touche_Melodie[i][Note1_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          Etat_Touche_Melodie[i][Note1_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
          noteOn(ChannelMelodie, Etat_Touche_Melodie[i][Note1_encours], MidiVelocity);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Melodie[i][No_Pin];
          AfficheDebug += " Note ";
          AfficheDebug += Etat_Touche_Melodie[i][Note1_encours];
          AfficheDebug +=  " ON Velo ";
          AfficheDebug += MidiVelocity;
          Serial.println(AfficheDebug);
#endif
        }
        if ((Etat_Registre[Etat_actuel] > RegistreBasson) && (Etat_Registre[Etat_avant] == RegistreBasson)) { //On joue la Flûte
          PousseTire = Etat_PousseTire[Etat_actuel] ? 3 : 4;
          Etat_Touche_Melodie[i][Note2_encours] = Touche_Melodie[i][PousseTire];
          Etat_Touche_Melodie[i][Note2_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          Etat_Touche_Melodie[i][Note2_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
          noteOn(ChannelMelodie, Etat_Touche_Melodie[i][Note2_encours], MidiVelocity);
#if defined (DEBUG)
          AfficheDebug = "Pin ";
          AfficheDebug += Touche_Melodie[i][No_Pin];
          AfficheDebug += " Note ";
          AfficheDebug += Etat_Touche_Melodie[i][Note2_encours];
          AfficheDebug +=  " ON Velo ";
          AfficheDebug += MidiVelocity;
          Serial.println(AfficheDebug);
#endif
        }
      }
    Etat_Registre[Etat_avant] = Etat_Registre[Etat_actuel];
    Affiche_LCD(3);
#if defined (DEBUG)
    AfficheDebug = "Registre ";
    AfficheDebug += Libelle_Registre[Etat_Registre[Etat_actuel]];
    Serial.println(AfficheDebug);
#endif
  }

  //Lecture Etat touche Accord
  for (int i = 0; i < Nb_ToucheAccord; i++) {
    Etat_Touche_Accord [i][Etat_actuel] = Etat_Mux[Touche_Accord[i][No_Pin]]; // Lecture Touche)
    if (Etat_Touche_Accord[i][Etat_actuel] != Etat_Touche_Accord[i][Etat_avant]) {
      if (Etat_Touche_Accord[i][Etat_actuel] == ON) {
        PousseTire = Etat_PousseTire[Etat_actuel] ? 1 : 2;
        Etat_Touche_Accord[i][Note1_encours] = Touche_Accord[i][PousseTire]; //Première note à jouer
        Etat_Touche_Accord[i][Note1_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
        Etat_Touche_Accord[i][Note1_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
        if (Etat_RenvAccord[Etat_actuel] > 0)
          Etat_Touche_Accord[i][Note1_encours] += 12; // 1er ou 2eme renversement On augmente la basse d'une octave
        noteOn(ChannelAccord, Etat_Touche_Accord[i][Note1_encours], MidiVelocity);

        //Jeu note 2 des Accords si Tierce
        if (Etat_TierceOnOff[Etat_actuel] == ON)
        {
          Etat_Touche_Accord[i][Note2_encours] = Touche_Accord[i][PousseTire + 2]; //Deuxième note à jouer
          Etat_Touche_Accord[i][Note2_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          Etat_Touche_Accord[i][Note2_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
          if (Etat_RenvAccord[Etat_actuel] == 2)
            Etat_Touche_Accord[i][Note2_encours] += 12; // 2eme renversement On augmente la tierce d'une octave
          noteOn(ChannelAccord, Etat_Touche_Accord[i][Note2_encours], MidiVelocity);
        }
        Etat_Touche_Accord[i][Note3_encours] = Touche_Accord[i][PousseTire + 4]; //Troisième note à jouer
        Etat_Touche_Accord[i][Note3_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
        Etat_Touche_Accord[i][Note3_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelAccord, Etat_Touche_Accord[i][Note3_encours], MidiVelocity);
      }
      else
      {
        noteOff(ChannelAccord, Etat_Touche_Accord[i][Note1_encours]);
        if (Etat_TierceOnOff[Etat_actuel] == ON)
          noteOff(ChannelAccord, Etat_Touche_Accord[i][Note2_encours]);
        noteOff(ChannelAccord, Etat_Touche_Accord[i][Note3_encours]);
      }
      Etat_Touche_Accord[i][Etat_avant] = Etat_Touche_Accord[i][Etat_actuel];
#if defined (DEBUG)
      AfficheDebug = "Acc ";
      AfficheDebug += Touche_Accord[i][No_Pin] + 1;
      AfficheDebug += " Note1 ";
      AfficheDebug += Etat_Touche_Accord[i][Note1_encours];
      AfficheDebug += Etat_Touche_Accord[i][Etat_actuel] ? " OFF  " : " ON ";
      Serial.println(AfficheDebug);
      if (Etat_TierceOnOff[Etat_actuel] == ON) {
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
    Etat_Touche_Basses [i][Etat_actuel] = Etat_Mux[Touche_Basses[i][No_Pin]]; // Lecture Touche)
    if (Etat_Touche_Basses[i][Etat_actuel] != Etat_Touche_Basses[i][Etat_avant]) {
      if (Etat_Touche_Basses[i][Etat_actuel] == ON) {
        PousseTire = Etat_PousseTire[Etat_actuel] ? 1 : 2;
        if (Etat_BassesProf[Etat_actuel] == ON)
        {
          Etat_Touche_Basses[i][Note1_encours] = Touche_Basses[i][PousseTire]; //Première note à jouer
          Etat_Touche_Basses[i][Note1_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
          Etat_Touche_Basses[i][Note1_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
          noteOn(ChannelBasses, Etat_Touche_Basses[i][Note1_encours], MidiVelocity);
        }
        //Jeu note 2 des Basses
        Etat_Touche_Basses[i][Note2_encours] = Touche_Basses[i][PousseTire + 2]; //Deuxième note à jouer
        Etat_Touche_Basses[i][Note2_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
        Etat_Touche_Basses[i][Note2_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelBasses, Etat_Touche_Basses[i][Note2_encours], MidiVelocity);
        //Jeu note 3 des Basses
        Etat_Touche_Basses[i][Note3_encours] = Touche_Basses[i][PousseTire + 4]; //Troisième note à jouer
        Etat_Touche_Basses[i][Note3_encours] += (12 * Etat_Octave[Etat_actuel]); // On augmente la note de + ou - n fois 12 tons
        Etat_Touche_Basses[i][Note3_encours] += Etat_Tona[Etat_actuel];   // On ajuste avec la tona
        noteOn(ChannelBasses, Etat_Touche_Basses[i][Note3_encours], MidiVelocity);
      }
      else
      {
        if (Etat_BassesProf[Etat_actuel] == ON)
          noteOff(ChannelBasses, Etat_Touche_Basses[i][Note1_encours]);
        noteOff(ChannelBasses, Etat_Touche_Basses[i][Note2_encours]);
        noteOff(ChannelBasses, Etat_Touche_Basses[i][Note3_encours]);
      }
      Etat_Touche_Basses[i][Etat_avant] = Etat_Touche_Basses[i][Etat_actuel];
#if defined (DEBUG)
      if (Etat_BassesProf[Etat_actuel] == ON) {
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
