//Définition des valeurs du capteur de force HX711 et codage

long Etat_Capteur[2];        //Conversion valeur float du capteur en long
long Etat_ReferenceCapteur;  //valeur du capteur au setup

#include "HX711.h"

#define calibration_factor -7050.0
#define No_Pin_HX711_Data  A3
#define No_Pin_HX711_Clock  A4

HX711 capteurHX711;
bool CapteurHX711absent = false;

float lecture;
long lecturelong;

const int ToleranceCapteur = 5;  //tolérance du capteur, si la variation est inférieure à la tolérance, on ne prend pas en compte la mesure
const int Const_Volume_Valeur_Capteur = 0;
const int Const_Volume_Valeur_Midi = 1;
const int Const_Expression_Valeur_Midi = 2;
const int Const_Gain_Valeur_Tsunami = 3;
const int IndexTableau = 4;
const int TailleTableau = 128;
int Const_Volume_Capteur[TailleTableau][IndexTableau] = {
  {  -166  , 125 , 100 , 4 },
  { -164  , 125 , 98  , 4 },
  { -162  , 120 , 98  , 3 },
  { -159  , 120 , 96  , 3 },
  { -157  , 120 , 94  , 2 },
  { -155  , 120 , 94  , 2 },
  { -153  , 115 , 92  , 1 },
  { -150  , 115 , 90  , 1 },
  { -148  , 115 , 90  , 0 },
  { -146  , 115 , 88  , 0 },
  { -144  , 110 , 86  , -1  },
  { -141  , 110 , 86  , -1  },
  { -139  , 110 , 84  , -2  },
  { -137  , 110 , 82  , -2  },
  { -135  , 105 , 82  , -2  },
  { -132  , 105 , 80  , -3  },
  { -130  , 105 , 78  , -3  },
  { -128  , 105 , 78  , -4  },
  { -126  , 100 , 76  , -4  },
  { -124  , 100 , 74  , -5  },
  { -121  , 100 , 72  , -5  },
  { -119  , 100 , 72  , -6  },
  { -117  , 95  , 70  , -6  },
  { -115  , 95  , 68  , -7  },
  { -112  , 95  , 68  , -7  },
  { -110  , 95  , 66  , -7  },
  { -108  , 90  , 64  , -8  },
  { -106  , 90  , 64  , -8  },
  { -103  , 90  , 62  , -9  },
  { -101  , 90  , 60  , -9  },
  { -99 , 85  , 60  , -10 },
  { -97 , 85  , 58  , -10 },
  { -94 , 85  , 56  , -11 },
  { -92 , 85  , 56  , -11 },
  { -90 , 80  , 54  , -12 },
  { -88 , 80  , 52  , -12 },
  { -85 , 80  , 52  , -13 },
  { -83 , 80  , 50  , -13 },
  { -81 , 75  , 48  , -13 },
  { -79 , 75  , 48  , -14 },
  { -77 , 75  , 46  , -14 },
  { -74 , 70  , 44  , -15 },
  { -72 , 70  , 44  , -15 },
  { -70 , 70  , 42  , -16 },
  { -68 , 70  , 40  , -16 },
  { -65 , 65  , 40  , -17 },
  { -63 , 65  , 38  , -17 },
  { -61 , 65  , 36  , -18 },
  { -59 , 65  , 36  , -18 },
  { -56 , 60  , 34  , -19 },
  { -54 , 60  , 32  , -19 },
  { -52 , 60  , 32  , -19 },
  { -50 , 60  , 30  , -20 },
  { -47 , 55  , 28  , -20 },
  { -45 , 55  , 28  , -21 },
  { -43 , 55  , 26  , -21 },
  { -41 , 55  , 24  , -22 },
  { -39 , 50  , 22  , -22 },
  { -36 , 50  , 22  , -23 },
  { -34 , 50  , 20  , -23 },
  { -32 , 50  , 18  , -24 },
  { -30 , 45  , 18  , -24 },
  { -27 , 45  , 16  , -24 },
  { -25 , 45  , 14  , -25 },
  { -23 , 45  , 14  , -25 },
  { -21 , 40  , 12  , -26 },
  { -18 , 40  , 10  , -26 },
  { -16 , 40  , 10  , -27 },
  { -14 , 40  , 8 , -27 },
  { -12 , 35  , 6 , -28 },
  { -9  , 35  , 6 , -28 },
  { -7  , 35  , 4 , -29 },
  { -5  , 35  , 2 , -29 },
  { -3  , 30  , 2 , -30 },
  { -1  , 30  , 0 , -30 },
  { 2 , 30  , 2 , -29 },
  { 4 , 35  , 4 , -29 },
  { 6 , 35  , 6 , -28 },
  { 8 , 35  , 8 , -27 },
  { 11  , 40  , 10  , -27 },
  { 13  , 40  , 12  , -26 },
  { 15  , 45  , 14  , -26 },
  { 17  , 45  , 16  , -25 },
  { 20  , 45  , 16  , -24 },
  { 22  , 50  , 18  , -24 },
  { 24  , 50  , 20  , -23 },
  { 26  , 50  , 22  , -22 },
  { 29  , 55  , 24  , -22 },
  { 31  , 55  , 26  , -21 },
  { 33  , 55  , 28  , -20 },
  { 35  , 60  , 30  , -20 },
  { 37  , 60  , 32  , -19 },
  { 40  , 60  , 34  , -18 },
  { 42  , 65  , 36  , -18 },
  { 44  , 65  , 38  , -17 },
  { 46  , 70  , 40  , -17 },
  { 49  , 70  , 42  , -16 },
  { 51  , 70  , 44  , -15 },
  { 53  , 75  , 46  , -15 },
  { 55  , 75  , 48  , -14 },
  { 58  , 75  , 50  , -13 },
  { 60  , 80  , 50  , -13 },
  { 62  , 80  , 52  , -12 },
  { 64  , 80  , 54  , -11 },
  { 67  , 85  , 56  , -11 },
  { 69  , 85  , 58  , -10 },
  { 71  , 85  , 60  , -9  },
  { 73  , 90  , 62  , -9  },
  { 76  , 90  , 64  , -8  },
  { 78  , 95  , 66  , -8  },
  { 80  , 95  , 68  , -7  },
  { 82  , 95  , 70  , -6  },
  { 84  , 100 , 72  , -6  },
  { 87  , 100 , 74  , -5  },
  { 89  , 100 , 76  , -4  },
  { 91  , 105 , 78  , -4  },
  { 93  , 105 , 80  , -3  },
  { 96  , 105 , 82  , -2  },
  { 98  , 110 , 84  , -2  },
  { 100 , 110 , 84  , -1  },
  { 102 , 110 , 86  , 0 },
  { 105 , 115 , 88  , 0 },
  { 107 , 115 , 90  , 1 },
  { 109 , 120 , 92  , 1 },
  { 111 , 120 , 94  , 2 },
  { 114 , 120 , 96  , 3 },
  { 116 , 125 , 98  , 3 },
  { 118 , 125 , 100 , 4 },
};

void Init_CapteurSoufflet()
{
  int i;

  capteurHX711.begin(No_Pin_HX711_Data, No_Pin_HX711_Clock);
  if (capteurHX711.wait_ready_timeout(1000)) {
    long reading = capteurHX711.get_units(10);
  }
  else
  {
#if defined (DEBUG)
    Serial.println(F("Ne trouve pas de capteur HX711 valide, vérifier le cablage!"));
#endif
    CapteurHX711absent = true;
    lcd.setCursor(0, 0);
    Affiche = "HX711 absent";
    for (int i = Affiche.length(); i < 20; i++)
      Affiche += " ";  // On remplit à blanc
    lcd.print(Affiche);
    Etat_Volume_Midi[Etat_avant] = Etat_Volume_Midi[Etat_actuel] = Defaut_Midi_Volume;
    Etat_Expression_Midi[Etat_avant] = Etat_Expression_Midi[Etat_actuel] = Defaut_Midi_Expression;
    Etat_Gain_Tsunami[Etat_avant] = Etat_Gain_Tsunami[Etat_actuel] = Defaut_Tsu_Gain;
    return;
  }
  capteurHX711.set_scale(calibration_factor);
  capteurHX711.tare();  //On laisse Robert tranquille à l'allumage !

  lecture = capteurHX711.get_units();      //.get_units() retourne un float
  lecturelong = lecture;  //conversion en long
  Etat_ReferenceCapteur = Etat_Capteur[Etat_avant] = Etat_Capteur[Etat_actuel] = lecturelong;

  i = 0;
  while ( i < TailleTableau) {
    if (Const_Volume_Capteur[i][Const_Volume_Valeur_Capteur] >= lecturelong) {
      Etat_Volume_Midi[Etat_actuel] = Const_Volume_Capteur[i][Const_Volume_Valeur_Midi];
      Etat_Expression_Midi[Etat_actuel] = Const_Volume_Capteur[i][Const_Expression_Valeur_Midi];
      Etat_Gain_Tsunami[Etat_actuel] = Const_Volume_Capteur[i][Const_Gain_Valeur_Tsunami];
#if defined (DEBUGCAPTEUR)
      Serial.print("Lecture Capteur Force HX711 ");
      Serial.print(lecture);
      Serial.print("\tEtat_Capteur ");
      Serial.print(Etat_Capteur[Etat_actuel]);
      Serial.print("\tVolume ");
      Serial.print(Etat_Volume_Midi[Etat_actuel]);
      Serial.print("\tExp ");
      Serial.print(Etat_Expression_Midi[Etat_actuel]);
      Serial.print("\tGain ");
      Serial.print(Etat_Gain_Tsunami[Etat_actuel]);
      Serial.print("\t fin ");
      Serial.print(millis());
      Serial.println();
#endif
      break;
    }
    else {
      i++;
    }
  }
  if (i == TailleTableau) {    //si valeur hors tableau
    Etat_Volume_Midi[Etat_actuel] = Const_Volume_Capteur[TailleTableau - 1][Const_Volume_Valeur_Midi];
    Etat_Expression_Midi[Etat_actuel] = Const_Volume_Capteur[TailleTableau - 1][Const_Expression_Valeur_Midi];
    Etat_Gain_Tsunami[Etat_actuel] = Const_Volume_Capteur[TailleTableau - 1][Const_Gain_Valeur_Tsunami];
#if defined (DEBUGCAPTEUR)
    Serial.print("Lecture Capteur Force HX711 ");
    Serial.print(lecture);
    Serial.print("\tEtat_Capteur ");
    Serial.print(Etat_Capteur[Etat_actuel]);
    Serial.print("\tVolHors ");
    Serial.print(Etat_Volume_Midi[Etat_actuel]);
    Serial.print("\tExpHors ");
    Serial.print(Etat_Expression_Midi[Etat_actuel]);
    Serial.print("\tGainHors ");
    Serial.print(Etat_Gain_Tsunami[Etat_actuel]);
    Serial.print("\t fin ");
    Serial.print(millis());
    Serial.println();
#endif
  }
  Etat_Volume_Midi[Etat_avant] = Etat_Volume_Midi[Etat_actuel];
  Etat_Expression_Midi[Etat_avant] = Etat_Expression_Midi[Etat_actuel];
  Etat_Gain_Tsunami[Etat_avant] = Etat_Gain_Tsunami[Etat_actuel];
}

void Lecture_CapteurSoufflet()
{
  int i, valeur, valeurcalc, delta_milli;
  long delta;
  unsigned long millisec;

  if (CapteurHX711absent == true)
  {
    lcd.setCursor(0, 0);
    Affiche = "HX711 absent";
    for (int i = Affiche.length(); i < 20; i++)
      Affiche += " ";  // On remplit à blanc
    lcd.print(Affiche);
    return;
  }
  lecture = capteurHX711.get_units();      //capteurHX711.get_units() retourn un float
  lecturelong = lecture;  //conversion en long
  delta = abs(Etat_Capteur[Etat_avant] - lecturelong);
  if (delta > ToleranceCapteur) {
    Etat_Capteur[Etat_actuel] = lecturelong;
    millisec = millis();
    Etat_Volume_Millis[Etat_actuel] = millisec;
    valeur = Etat_Capteur[Etat_actuel];
#if defined (DEBUGCAPTEUR)
    Serial.print(millisec);
    Serial.print("\tDelta");
    Serial.print(ToleranceCapteur);
    Serial.print(" ");
    Serial.print(delta);
    Serial.print("\tLecture ");
    Serial.print(lecture, 2);
    Serial.print("\tAvant ");
    Serial.print(Etat_Capteur[Etat_avant]);
    Serial.print("\tActuel ");
    Serial.print(valeur);
#endif
    // Cacul d'une valeur en fonction du capteur et du temps écoulé depuis le dernier changement
    delta_milli = Etat_Volume_Millis[Etat_actuel] - Etat_Volume_Millis[Etat_avant];
    valeur = Etat_Capteur[Etat_actuel] - Etat_Capteur[Etat_avant];
    valeurcalc = valeur * 300;
    delta = valeurcalc / delta_milli;
#if defined (DEBUGCAPTEUR)
    Serial.print("\tdifmil ");
    Serial.print(delta_milli);
    Serial.print("\tval ");
    Serial.print(valeur);
    Serial.print("\tvalcalc  ");
    Serial.print(valeurcalc);
    Serial.print("\tdelta  ");
    Serial.print(delta);
#endif
    i = 0;
    while ( i < TailleTableau) {
      if (Const_Volume_Capteur[i][Const_Volume_Valeur_Capteur] >= delta) {
        Etat_Volume_Midi[Etat_actuel] = Const_Volume_Capteur[i][Const_Volume_Valeur_Midi];
        Etat_Expression_Midi[Etat_actuel] = Const_Volume_Capteur[i][Const_Expression_Valeur_Midi];
        Etat_Gain_Tsunami[Etat_actuel] = Const_Volume_Capteur[i][Const_Gain_Valeur_Tsunami];
#if defined (DEBUGCAPTEUR)
        Serial.print("\tVolume ");
        Serial.print(Etat_Volume_Midi[Etat_actuel]);
        Serial.print("\tExp ");
        Serial.print(Etat_Expression_Midi[Etat_actuel]);
        Serial.print("\tGain ");
        Serial.print(Etat_Gain_Tsunami[Etat_actuel]);
        Serial.print("\t fin ");
        Serial.print(millis());
        Serial.println();
#endif
        break;
      }
      else {
        i++;
      }
    }
    if (i == TailleTableau) {    //si valeur hors tableau
      Etat_Volume_Midi[Etat_actuel] = Const_Volume_Capteur[TailleTableau - 1][Const_Volume_Valeur_Midi];
      Etat_Expression_Midi[Etat_actuel] = Const_Volume_Capteur[TailleTableau - 1][Const_Expression_Valeur_Midi];
      Etat_Gain_Tsunami[Etat_actuel] = Const_Volume_Capteur[TailleTableau - 1][Const_Gain_Valeur_Tsunami];
#if defined (DEBUGCAPTEUR)
      Serial.print("\tVolHors ");
      Serial.print(Etat_Volume_Midi[Etat_actuel]);
      Serial.print("\tExpHors ");
      Serial.print(Etat_Expression_Midi[Etat_actuel]);
      Serial.print("\tGain ");
      Serial.print(Etat_Gain_Tsunami[Etat_actuel]);
      Serial.print("\t fin ");
      Serial.print(millis());
      Serial.println();
#endif
    }
  }
  else
  {
    Etat_Capteur[Etat_actuel] = Etat_Capteur[Etat_avant];  //pas de changement si la variation est dans la tolérance
    Etat_Volume_Midi[Etat_actuel] = 0;  //Si pas de changement, plus de déplacement A adapter !!!
  }
#if defined (DEBUGCAPTLCD)
  lcd.setCursor(9, 3);
  if (lecture >= 0) Affiche = " ";
  else
    Affiche = "";
  Affiche += lecture;
  Affiche += " lbs";
  for (i = Affiche.length(); i < 11; i++)
    Affiche += " ";  // On remplit à blanc
  lcd.print(Affiche);
#endif
}
