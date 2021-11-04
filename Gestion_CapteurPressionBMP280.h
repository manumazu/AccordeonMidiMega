//Définition des valeurs du capteur de pression BMP280 et codage

long Etat_Capteur[2];        //Conversion valeur float du capteur en long
long Etat_ReferenceCapteur;  //valeur du capteur au setup

#include <Adafruit_BMP280.h>

#define ADDRI2C 0x76  //Addresse bus I2C

//Object
Adafruit_BMP280 bmp; // I2C

bool CapteurBMP280absent = false;

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
  {  -672  , 125 , 100 , 4 },
  { -660  , 125 , 98  , 3 },
  { -647  , 120 , 96  , 3 },
  { -635  , 120 , 94  , 2 },
  { -623  , 120 , 92  , 1 },
  { -611  , 115 , 90  , 1 },
  { -598  , 115 , 88  , 0 },
  { -586  , 115 , 88  , 0 },
  { -574  , 110 , 86  , -1  },
  { -561  , 110 , 84  , -2  },
  { -549  , 105 , 82  , -2  },
  { -537  , 105 , 80  , -3  },
  { -525  , 105 , 78  , -4  },
  { -512  , 100 , 76  , -4  },
  { -500  , 100 , 74  , -5  },
  { -488  , 100 , 72  , -5  },
  { -475  , 95  , 70  , -6  },
  { -463  , 95  , 68  , -7  },
  { -451  , 95  , 66  , -7  },
  { -439  , 90  , 64  , -8  },
  { -426  , 90  , 62  , -9  },
  { -414  , 90  , 62  , -9  },
  { -402  , 85  , 60  , -10 },
  { -389  , 85  , 58  , -10 },
  { -377  , 85  , 56  , -11 },
  { -365  , 80  , 54  , -12 },
  { -353  , 80  , 52  , -12 },
  { -340  , 75  , 50  , -13 },
  { -328  , 75  , 48  , -14 },
  { -316  , 75  , 46  , -14 },
  { -303  , 70  , 44  , -15 },
  { -291  , 70  , 42  , -16 },
  { -279  , 70  , 40  , -16 },
  { -267  , 65  , 38  , -17 },
  { -254  , 65  , 38  , -17 },
  { -242  , 65  , 36  , -18 },
  { -230  , 60  , 34  , -19 },
  { -218  , 60  , 32  , -19 },
  { -205  , 60  , 30  , -20 },
  { -193  , 55  , 28  , -21 },
  { -181  , 55  , 26  , -21 },
  { -168  , 55  , 24  , -22 },
  { -156  , 50  , 22  , -22 },
  { -144  , 50  , 20  , -23 },
  { -132  , 50  , 18  , -24 },
  { -119  , 45  , 16  , -24 },
  { -107  , 45  , 14  , -25 },
  { -95 , 40  , 12  , -26 },
  { -82 , 40  , 12  , -26 },
  { -70 , 40  , 10  , -27 },
  { -58 , 35  , 8 , -27 },
  { -46 , 35  , 6 , -28 },
  { -33 , 35  , 4 , -29 },
  { -21 , 30  , 2 , -29 },
  { -9  , 30  , 0 , -30 },
  { 4 , 30  , 2 , -30 },
  { 16  , 35  , 2 , -29 },
  { 28  , 35  , 4 , -29 },
  { 40  , 35  , 6 , -28 },
  { 53  , 35  , 6 , -28 },
  { 65  , 40  , 8 , -27 },
  { 77  , 40  , 10  , -27 },
  { 90  , 40  , 10  , -26 },
  { 102 , 40  , 12  , -26 },
  { 114 , 45  , 14  , -25 },
  { 126 , 45  , 16  , -25 },
  { 139 , 45  , 16  , -24 },
  { 151 , 45  , 18  , -24 },
  { 163 , 50  , 20  , -23 },
  { 176 , 50  , 20  , -23 },
  { 188 , 50  , 22  , -23 },
  { 200 , 50  , 24  , -22 },
  { 212 , 55  , 24  , -22 },
  { 225 , 55  , 26  , -21 },
  { 237 , 55  , 28  , -21 },
  { 249 , 55  , 28  , -20 },
  { 262 , 60  , 30  , -20 },
  { 274 , 60  , 32  , -19 },
  { 286 , 60  , 32  , -19 },
  { 298 , 65  , 34  , -18 },
  { 311 , 65  , 36  , -18 },
  { 323 , 65  , 36  , -17 },
  { 335 , 65  , 38  , -17 },
  { 348 , 70  , 40  , -16 },
  { 360 , 70  , 42  , -16 },
  { 372 , 70  , 42  , -16 },
  { 384 , 70  , 44  , -15 },
  { 397 , 75  , 46  , -15 },
  { 409 , 75  , 46  , -14 },
  { 421 , 75  , 48  , -14 },
  { 434 , 75  , 50  , -13 },
  { 446 , 80  , 50  , -13 },
  { 458 , 80  , 52  , -12 },
  { 470 , 80  , 54  , -12 },
  { 483 , 80  , 54  , -11 },
  { 495 , 85  , 56  , -11 },
  { 507 , 85  , 58  , -10 },
  { 519 , 85  , 58  , -10 },
  { 532 , 85  , 60  , -10 },
  { 544 , 90  , 62  , -9  },
  { 556 , 90  , 64  , -9  },
  { 569 , 90  , 64  , -8  },
  { 581 , 90  , 66  , -8  },
  { 593 , 95  , 68  , -7  },
  { 605 , 95  , 68  , -7  },
  { 618 , 95  , 70  , -6  },
  { 630 , 100 , 72  , -6  },
  { 642 , 100 , 72  , -5  },
  { 655 , 100 , 74  , -5  },
  { 667 , 100 , 76  , -4  },
  { 679 , 105 , 76  , -4  },
  { 691 , 105 , 78  , -3  },
  { 704 , 105 , 80  , -3  },
  { 716 , 105 , 80  , -3  },
  { 728 , 110 , 82  , -2  },
  { 741 , 110 , 84  , -2  },
  { 753 , 110 , 84  , -1  },
  { 765 , 110 , 86  , -1  },
  { 777 , 115 , 88  , 0 },
  { 790 , 115 , 90  , 0 },
  { 802 , 115 , 90  , 1 },
  { 814 , 115 , 92  , 1 },
  { 827 , 120 , 94  , 2 },
  { 839 , 120 , 94  , 2 },
  { 851 , 120 , 96  , 3 },
  { 863 , 120 , 98  , 3 },
  { 876 , 125 , 98  , 4 },
  { 888 , 125 , 100 , 4 },
};

void Init_CapteurSoufflet()
{
  bool retour;
  int i;

  retour = bmp.begin(ADDRI2C);
  if (!retour) {
#if defined (DEBUG)
    Serial.println(F("Ne trouve pas de capteur BMP280 valide, vérifier le cablage!"));
#endif
    CapteurBMP280absent = true;
    lcd.setCursor(0, 0);
    Affiche = "BMP280 absent";
    for (int i = Affiche.length(); i < 20; i++)
      Affiche += " ";  // On remplit à blanc
    lcd.print(Affiche);
    Etat_Volume_Midi[Etat_avant] = Etat_Volume_Midi[Etat_actuel] = Defaut_Midi_Volume;
    Etat_Expression_Midi[Etat_avant] = Etat_Expression_Midi[Etat_actuel] = Defaut_Midi_Expression;
    Etat_Gain_Tsunami[Etat_avant] = Etat_Gain_Tsunami[Etat_actuel] = Defaut_Tsu_Gain;
    return;
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_1);   /* Standby time. 1 ms */

  lecture = bmp.readPressure();
  lecturelong = lecture;  //conversion en long
  Etat_ReferenceCapteur = Etat_Capteur[Etat_avant] = Etat_Capteur[Etat_actuel] = lecturelong;  //conversion en long

  i = 0;
  while ( i < TailleTableau) {
    if (Const_Volume_Capteur[i][Const_Volume_Valeur_Capteur] >= lecturelong) {
      Etat_Volume_Midi[Etat_actuel] = Const_Volume_Capteur[i][Const_Volume_Valeur_Midi];
      Etat_Expression_Midi[Etat_actuel] = Const_Volume_Capteur[i][Const_Expression_Valeur_Midi];
      Etat_Gain_Tsunami[Etat_actuel] = Const_Volume_Capteur[i][Const_Gain_Valeur_Tsunami];
#if defined (DEBUGCAPTEUR)
      Serial.print("Lecture Capteur Pression BMP280 ");
      Serial.print(lecture);
      Serial.print("\tReferenceCapteur ");
      Serial.print(Etat_ReferenceCapteur);
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
    Serial.print("Lecture Capteur Pression BMP280 ");
    Serial.print(lecture);
    Serial.print("\tReferenceCapteur ");
    Serial.print(Etat_ReferenceCapteur);
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
  int i;
  long delta;
  long valeur;
  unsigned long millisec;

  if (CapteurBMP280absent == true)
  {
    lcd.setCursor(0, 0);
    Affiche = "BMP280 absent";
    for (int i = Affiche.length(); i < 20; i++)
      Affiche += " ";  // On remplit à blanc
    lcd.print(Affiche);
    return;
  }
  lecture = bmp.readPressure();      //bmp.readPressure() returns a float
  lecturelong = lecture;             //conversion en long
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
    Serial.print("\tLecture_Pa ");
    Serial.print(lecture, 2);
    Serial.print("\tAvant ");
    Serial.print(Etat_Capteur[Etat_avant]);
    Serial.print("\tActuel ");
    Serial.print(valeur);
#endif
    // Cacul d'une valeur en fonction de la référence et du capteur
    delta = Etat_Capteur[Etat_actuel] - Etat_ReferenceCapteur;
#if defined (DEBUGCAPTEUR)
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
