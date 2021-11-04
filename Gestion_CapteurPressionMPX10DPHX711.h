//Définition des valeurs du capteur MPX10DP avec HX711 et codage

long Etat_Capteur[2];        //Conversion valeur float du capteur en long
long Etat_ReferenceCapteur;  //valeur du capteur au setup

#include "HX711.h"

#define calibration_factor 1000.0
byte gain = 32; // 32 pour channel B, 64 ou 128 pour channel A
#define No_Pin_HX711_Data  A3
#define No_Pin_HX711_Clock  A4

HX711 capteurHX711;
bool CapteurHX711absent = false;

float lectureraw, lecture;
long lecturelong;

const int ToleranceCapteur = 10;  //tolérance du capteur, si la variation est inférieure à la tolérance, on ne prend pas en compte la mesure
const int Const_Volume_Valeur_Capteur = 0;
const int Const_Volume_Valeur_Midi = 1;
const int Const_Expression_Valeur_Midi = 2;
const int Const_Gain_Valeur_Tsunami = 3;
const int IndexTableau = 4;
const int TailleTableau = 128;
int Const_Volume_Capteur[TailleTableau][IndexTableau] = {
  {  -160  , 125 , 126 , 4 },
  { -157  , 125 , 124 , 3 },
  { -154  , 120 , 122 , 2 },
  { -151  , 120 , 120 , 1 },
  { -149  , 120 , 120 , 0 },
  { -146  , 115 , 118 , -1  },
  { -143  , 115 , 116 , -2  },
  { -140  , 115 , 114 , -3  },
  { -137  , 115 , 112 , -4  },
  { -134  , 110 , 110 , -5  },
  { -132  , 110 , 108 , -6  },
  { -129  , 110 , 108 , -7  },
  { -126  , 105 , 106 , -8  },
  { -123  , 105 , 104 , -9  },
  { -120  , 105 , 102 , -9  },
  { -117  , 100 , 100 , -10 },
  { -115  , 100 , 98  , -11 },
  { -112  , 100 , 96  , -12 },
  { -109  , 100 , 96  , -13 },
  { -106  , 95  , 94  , -14 },
  { -103  , 95  , 92  , -15 },
  { -100  , 95  , 90  , -16 },
  { -98 , 90  , 88  , -17 },
  { -95 , 90  , 86  , -18 },
  { -92 , 90  , 84  , -19 },
  { -89 , 85  , 84  , -20 },
  { -86 , 85  , 82  , -21 },
  { -83 , 85  , 80  , -22 },
  { -81 , 85  , 78  , -23 },
  { -78 , 80  , 76  , -24 },
  { -75 , 80  , 74  , -25 },
  { -72 , 80  , 72  , -26 },
  { -69 , 75  , 72  , -27 },
  { -66 , 75  , 70  , -28 },
  { -64 , 75  , 68  , -29 },
  { -61 , 70  , 66  , -30 },
  { -58 , 70  , 64  , -31 },
  { -55 , 70  , 62  , -32 },
  { -52 , 65  , 60  , -33 },
  { -49 , 65  , 60  , -34 },
  { -47 , 65  , 58  , -35 },
  { -44 , 65  , 56  , -36 },
  { -41 , 60  , 54  , -36 },
  { -38 , 60  , 52  , -37 },
  { -35 , 60  , 50  , -38 },
  { -32 , 55  , 48  , -39 },
  { -30 , 55  , 48  , -40 },
  { -27 , 55  , 46  , -41 },
  { -24 , 50  , 44  , -42 },
  { -21 , 50  , 42  , -43 },
  { -18 , 50  , 40  , -44 },
  { -15 , 50  , 38  , -45 },
  { -13 , 45  , 36  , -46 },
  { -10 , 45  , 36  , -47 },
  { -7  , 45  , 34  , -48 },
  { -4  , 40  , 32  , -49 },
  { -1  , 40  , 30  , -50 },
  { 2 , 40  , 32  , -49 },
  { 4 , 40  , 32  , -48 },
  { 7 , 45  , 34  , -48 },
  { 10  , 45  , 36  , -47 },
  { 13  , 45  , 36  , -46 },
  { 16  , 45  , 38  , -45 },
  { 19  , 50  , 40  , -45 },
  { 21  , 50  , 40  , -44 },
  { 24  , 50  , 42  , -43 },
  { 27  , 50  , 44  , -42 },
  { 30  , 55  , 44  , -42 },
  { 33  , 55  , 46  , -41 },
  { 36  , 55  , 48  , -40 },
  { 38  , 55  , 48  , -39 },
  { 41  , 60  , 50  , -39 },
  { 44  , 60  , 52  , -38 },
  { 47  , 60  , 52  , -37 },
  { 50  , 60  , 54  , -36 },
  { 53  , 65  , 56  , -36 },
  { 55  , 65  , 58  , -35 },
  { 58  , 65  , 58  , -34 },
  { 61  , 65  , 60  , -33 },
  { 64  , 70  , 62  , -33 },
  { 67  , 70  , 62  , -32 },
  { 70  , 70  , 64  , -31 },
  { 72  , 70  , 66  , -30 },
  { 75  , 70  , 66  , -29 },
  { 78  , 75  , 68  , -29 },
  { 81  , 75  , 70  , -28 },
  { 84  , 75  , 70  , -27 },
  { 87  , 75  , 72  , -26 },
  { 89  , 80  , 74  , -26 },
  { 92  , 80  , 74  , -25 },
  { 95  , 80  , 76  , -24 },
  { 98  , 80  , 78  , -23 },
  { 101 , 85  , 78  , -23 },
  { 104 , 85  , 80  , -22 },
  { 106 , 85  , 82  , -21 },
  { 109 , 85  , 82  , -20 },
  { 112 , 90  , 84  , -20 },
  { 115 , 90  , 86  , -19 },
  { 118 , 90  , 86  , -18 },
  { 121 , 90  , 88  , -17 },
  { 123 , 95  , 90  , -17 },
  { 126 , 95  , 90  , -16 },
  { 129 , 95  , 92  , -15 },
  { 132 , 95  , 94  , -14 },
  { 135 , 95  , 94  , -13 },
  { 138 , 100 , 96  , -13 },
  { 140 , 100 , 98  , -12 },
  { 143 , 100 , 98  , -11 },
  { 146 , 100 , 100 , -10 },
  { 149 , 105 , 102 , -10 },
  { 152 , 105 , 104 , -9  },
  { 155 , 105 , 104 , -8  },
  { 157 , 105 , 106 , -7  },
  { 160 , 110 , 108 , -7  },
  { 163 , 110 , 108 , -6  },
  { 166 , 110 , 110 , -5  },
  { 169 , 110 , 112 , -4  },
  { 172 , 115 , 112 , -4  },
  { 174 , 115 , 114 , -3  },
  { 177 , 115 , 116 , -2  },
  { 180 , 115 , 116 , -1  },
  { 183 , 120 , 118 , -1  },
  { 186 , 120 , 120 , 0 },
  { 189 , 120 , 120 , 1 },
  { 191 , 120 , 122 , 2 },
  { 194 , 125 , 124 , 2 },
  { 197 , 125 , 124 , 3 },
  { 200 , 125 , 126 , 4 },
};

void Init_CapteurSoufflet()
{
  int i;

  capteurHX711.begin(No_Pin_HX711_Data, No_Pin_HX711_Clock, gain);
  if (capteurHX711.wait_ready_timeout(1000)) {
    capteurHX711.get_units(10);
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

  lectureraw = capteurHX711.read();      //.read() retourne un float
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
      Serial.print("Lecture Capteur MPX10DP HX711 Raw ");
      Serial.print(lectureraw);
      Serial.print("\tget_units ");
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
    Serial.print("Lecture Capteur MPX10DP HX711 ");
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
  int i, valeur;
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
  lecture = capteurHX711.get_units();      //.get_units() retourne un float
  lecturelong = lecture;  //conversion en int
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
    Serial.print("\tCh_capteur ");
    Serial.print(lecture, 2);
    Serial.print("\tAvant ");
    Serial.print(Etat_Capteur[Etat_avant]);
    Serial.print("\tActuel ");
    Serial.print(valeur);
#endif
    // Cacul d'une valeur en fonction de la référence et du capteur
    delta = Etat_Capteur[Etat_actuel] - Etat_ReferenceCapteur;
#if defined (DEBUGCAPTEUR)
    Serial.print("\tDeltaRef  ");
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
  }
#if defined (DEBUGCAPTLCD)
  lcd.setCursor(9, 3);
  if (lecture >= 0) Affiche = " ";
  else
    Affiche = "";
  Affiche += lecture;
  for (i = Affiche.length(); i < 11; i++)
    Affiche += " ";  // On remplit à blanc
  lcd.print(Affiche);
#endif
}
