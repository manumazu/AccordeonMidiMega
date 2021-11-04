//Définition des valeurs du capteur Honeywell avec HX711 et codage

long Etat_Capteur[2];        //Conversion valeur float du capteur en long
long Etat_ReferenceCapteur;  //valeur du capteur au setup

#include "HX711.h"

#define calibration_factor 2280.0
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
  {  -300  , 125 , 126 , 4 },
  { -295  , 125 , 124 , 3 },
  { -291  , 120 , 124 , 3 },
  { -286  , 120 , 122 , 2 },
  { -281  , 120 , 120 , 2 },
  { -276  , 120 , 120 , 1 },
  { -272  , 115 , 118 , 1 },
  { -267  , 115 , 116 , 0 },
  { -262  , 115 , 116 , 0 },
  { -257  , 115 , 114 , -1  },
  { -253  , 110 , 112 , -1  },
  { -248  , 110 , 110 , -2  },
  { -243  , 110 , 110 , -2  },
  { -239  , 105 , 108 , -3  },
  { -234  , 105 , 106 , -4  },
  { -229  , 105 , 106 , -4  },
  { -224  , 105 , 104 , -5  },
  { -220  , 100 , 102 , -5  },
  { -215  , 100 , 102 , -6  },
  { -210  , 100 , 100 , -6  },
  { -206  , 100 , 98  , -7  },
  { -201  , 95  , 98  , -7  },
  { -196  , 95  , 96  , -8  },
  { -191  , 95  , 94  , -8  },
  { -187  , 95  , 94  , -9  },
  { -182  , 90  , 92  , -9  },
  { -177  , 90  , 90  , -10 },
  { -172  , 90  , 90  , -11 },
  { -168  , 85  , 88  , -11 },
  { -163  , 85  , 86  , -12 },
  { -158  , 85  , 86  , -12 },
  { -154  , 85  , 84  , -13 },
  { -149  , 80  , 82  , -13 },
  { -144  , 80  , 80  , -14 },
  { -139  , 80  , 80  , -14 },
  { -135  , 80  , 78  , -15 },
  { -130  , 75  , 76  , -15 },
  { -125  , 75  , 76  , -16 },
  { -120  , 75  , 74  , -17 },
  { -116  , 70  , 72  , -17 },
  { -111  , 70  , 72  , -18 },
  { -106  , 70  , 70  , -18 },
  { -102  , 70  , 68  , -19 },
  { -97 , 65  , 68  , -19 },
  { -92 , 65  , 66  , -20 },
  { -87 , 65  , 64  , -20 },
  { -83 , 65  , 64  , -21 },
  { -78 , 60  , 62  , -21 },
  { -73 , 60  , 60  , -22 },
  { -69 , 60  , 60  , -22 },
  { -64 , 60  , 58  , -23 },
  { -59 , 55  , 56  , -24 },
  { -54 , 55  , 56  , -24 },
  { -50 , 55  , 54  , -25 },
  { -45 , 50  , 52  , -25 },
  { -40 , 50  , 50  , -26 },
  { -35 , 50  , 50  , -26 },
  { -31 , 50  , 48  , -27 },
  { -26 , 45  , 46  , -27 },
  { -21 , 45  , 46  , -28 },
  { -17 , 45  , 44  , -28 },
  { -12 , 45  , 42  , -29 },
  { -7  , 40  , 42  , -29 },
  { -2  , 40  , 40  , -30 },
  { 2 , 40  , 42  , -29 },
  { 7 , 45  , 42  , -29 },
  { 12  , 45  , 44  , -28 },
  { 17  , 45  , 46  , -28 },
  { 21  , 45  , 46  , -27 },
  { 26  , 50  , 48  , -27 },
  { 31  , 50  , 50  , -26 },
  { 35  , 50  , 50  , -26 },
  { 40  , 50  , 52  , -25 },
  { 45  , 55  , 54  , -25 },
  { 50  , 55  , 54  , -24 },
  { 54  , 55  , 56  , -24 },
  { 59  , 55  , 58  , -23 },
  { 64  , 60  , 58  , -23 },
  { 69  , 60  , 60  , -22 },
  { 73  , 60  , 62  , -22 },
  { 78  , 65  , 62  , -21 },
  { 83  , 65  , 64  , -20 },
  { 87  , 65  , 66  , -20 },
  { 92  , 65  , 66  , -19 },
  { 97  , 70  , 68  , -19 },
  { 102 , 70  , 70  , -18 },
  { 106 , 70  , 70  , -18 },
  { 111 , 70  , 72  , -17 },
  { 116 , 75  , 74  , -17 },
  { 120 , 75  , 74  , -16 },
  { 125 , 75  , 76  , -16 },
  { 130 , 75  , 78  , -15 },
  { 135 , 80  , 78  , -15 },
  { 139 , 80  , 80  , -14 },
  { 144 , 80  , 82  , -14 },
  { 149 , 85  , 84  , -13 },
  { 154 , 85  , 84  , -12 },
  { 158 , 85  , 86  , -12 },
  { 163 , 85  , 88  , -11 },
  { 168 , 90  , 88  , -11 },
  { 172 , 90  , 90  , -10 },
  { 177 , 90  , 92  , -10 },
  { 182 , 90  , 92  , -9  },
  { 187 , 95  , 94  , -9  },
  { 191 , 95  , 96  , -8  },
  { 196 , 95  , 96  , -8  },
  { 201 , 95  , 98  , -7  },
  { 206 , 100 , 100 , -7  },
  { 210 , 100 , 100 , -6  },
  { 215 , 100 , 102 , -6  },
  { 220 , 100 , 104 , -5  },
  { 224 , 105 , 104 , -5  },
  { 229 , 105 , 106 , -4  },
  { 234 , 105 , 108 , -3  },
  { 239 , 110 , 108 , -3  },
  { 243 , 110 , 110 , -2  },
  { 248 , 110 , 112 , -2  },
  { 253 , 110 , 112 , -1  },
  { 257 , 115 , 114 , -1  },
  { 262 , 115 , 116 , 0 },
  { 267 , 115 , 116 , 0 },
  { 272 , 115 , 118 , 1 },
  { 276 , 120 , 120 , 1 },
  { 281 , 120 , 120 , 2 },
  { 286 , 120 , 122 , 2 },
  { 291 , 120 , 124 , 3 },
  { 295 , 125 , 124 , 3 },
  { 300 , 125 , 126 , 4 },
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
      Serial.print("Lecture Capteur Honey HX711 Raw ");
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
    Serial.print("Lecture Capteur Honey HX711 ");
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
