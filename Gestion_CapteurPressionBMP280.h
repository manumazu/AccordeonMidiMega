//Définition des valeurs du capteur de pression BMP280 et codage

long Etat_Capteur[2];        //Conversion valeur float du capteur en long
long Etat_ReferenceCapteur;  //valeur du capteur au setup

#include <Adafruit_BMP280.h>

#define ADDRI2C 0x76  //Addresse bus I2C

//Object
Adafruit_BMP280 bmp; // I2C

float lecture;

const int Const_Volume_Valeur_Capteur = 0;
const int Const_Volume_Valeur_Midi = 1;
const int TailleTableau = 256;
int Const_Volume_Capteur[TailleTableau][2] = {
  {  -500  , 125 },
  { -496  , 125 },
  { -492  , 125 },
  { -488  , 125 },
  { -484  , 120 },
  { -480  , 120 },
  { -476  , 120 },
  { -473  , 120 },
  { -469  , 120 },
  { -465  , 120 },
  { -461  , 120 },
  { -457  , 115 },
  { -453  , 115 },
  { -449  , 115 },
  { -445  , 115 },
  { -441  , 115 },
  { -437  , 115 },
  { -433  , 110 },
  { -429  , 110 },
  { -425  , 110 },
  { -422  , 110 },
  { -418  , 110 },
  { -414  , 110 },
  { -410  , 110 },
  { -406  , 105 },
  { -402  , 105 },
  { -398  , 105 },
  { -394  , 105 },
  { -390  , 105 },
  { -386  , 105 },
  { -382  , 105 },
  { -378  , 100 },
  { -375  , 100 },
  { -371  , 100 },
  { -367  , 100 },
  { -363  , 100 },
  { -359  , 100 },
  { -355  , 95  },
  { -351  , 95  },
  { -347  , 95  },
  { -343  , 95  },
  { -339  , 95  },
  { -335  , 95  },
  { -331  , 95  },
  { -327  , 90  },
  { -324  , 90  },
  { -320  , 90  },
  { -316  , 90  },
  { -312  , 90  },
  { -308  , 90  },
  { -304  , 90  },
  { -300  , 85  },
  { -296  , 85  },
  { -292  , 85  },
  { -288  , 85  },
  { -284  , 85  },
  { -280  , 85  },
  { -276  , 80  },
  { -273  , 80  },
  { -269  , 80  },
  { -265  , 80  },
  { -261  , 80  },
  { -257  , 80  },
  { -253  , 80  },
  { -249  , 75  },
  { -245  , 75  },
  { -241  , 75  },
  { -237  , 75  },
  { -233  , 75  },
  { -229  , 75  },
  { -225  , 75  },
  { -222  , 70  },
  { -218  , 70  },
  { -214  , 70  },
  { -210  , 70  },
  { -206  , 70  },
  { -202  , 70  },
  { -198  , 65  },
  { -194  , 65  },
  { -190  , 65  },
  { -186  , 65  },
  { -182  , 65  },
  { -178  , 65  },
  { -175  , 65  },
  { -171  , 60  },
  { -167  , 60  },
  { -163  , 60  },
  { -159  , 60  },
  { -155  , 60  },
  { -151  , 60  },
  { -147  , 60  },
  { -143  , 55  },
  { -139  , 55  },
  { -135  , 55  },
  { -131  , 55  },
  { -127  , 55  },
  { -124  , 55  },
  { -120  , 50  },
  { -116  , 50  },
  { -112  , 50  },
  { -108  , 50  },
  { -104  , 50  },
  { -100  , 50  },
  { -96 , 50  },
  { -92 , 45  },
  { -88 , 45  },
  { -84 , 45  },
  { -80 , 45  },
  { -76 , 45  },
  { -73 , 45  },
  { -69 , 45  },
  { -65 , 40  },
  { -61 , 40  },
  { -57 , 40  },
  { -53 , 40  },
  { -49 , 40  },
  { -45 , 40  },
  { -41 , 35  },
  { -37 , 35  },
  { -33 , 35  },
  { -29 , 35  },
  { -25 , 35  },
  { -22 , 35  },
  { -18 , 35  },
  { -14 , 30  },
  { -10 , 30  },
  { -6  , 30  },
  { -2  , 30  },
  { 2 , 30  },
  { 6 , 30  },
  { 10  , 30  },
  { 14  , 35  },
  { 18  , 35  },
  { 22  , 35  },
  { 25  , 35  },
  { 29  , 35  },
  { 33  , 35  },
  { 37  , 35  },
  { 41  , 40  },
  { 45  , 40  },
  { 49  , 40  },
  { 53  , 40  },
  { 57  , 40  },
  { 61  , 40  },
  { 65  , 45  },
  { 69  , 45  },
  { 73  , 45  },
  { 76  , 45  },
  { 80  , 45  },
  { 84  , 45  },
  { 88  , 45  },
  { 92  , 50  },
  { 96  , 50  },
  { 100 , 50  },
  { 104 , 50  },
  { 108 , 50  },
  { 112 , 50  },
  { 116 , 50  },
  { 120 , 55  },
  { 124 , 55  },
  { 127 , 55  },
  { 131 , 55  },
  { 135 , 55  },
  { 139 , 55  },
  { 143 , 55  },
  { 147 , 60  },
  { 151 , 60  },
  { 155 , 60  },
  { 159 , 60  },
  { 163 , 60  },
  { 167 , 60  },
  { 171 , 65  },
  { 175 , 65  },
  { 178 , 65  },
  { 182 , 65  },
  { 186 , 65  },
  { 190 , 65  },
  { 194 , 65  },
  { 198 , 70  },
  { 202 , 70  },
  { 206 , 70  },
  { 210 , 70  },
  { 214 , 70  },
  { 218 , 70  },
  { 222 , 70  },
  { 225 , 75  },
  { 229 , 75  },
  { 233 , 75  },
  { 237 , 75  },
  { 241 , 75  },
  { 245 , 75  },
  { 249 , 80  },
  { 253 , 80  },
  { 257 , 80  },
  { 261 , 80  },
  { 265 , 80  },
  { 269 , 80  },
  { 273 , 80  },
  { 276 , 85  },
  { 280 , 85  },
  { 284 , 85  },
  { 288 , 85  },
  { 292 , 85  },
  { 296 , 85  },
  { 300 , 85  },
  { 304 , 90  },
  { 308 , 90  },
  { 312 , 90  },
  { 316 , 90  },
  { 320 , 90  },
  { 324 , 90  },
  { 327 , 90  },
  { 331 , 95  },
  { 335 , 95  },
  { 339 , 95  },
  { 343 , 95  },
  { 347 , 95  },
  { 351 , 95  },
  { 355 , 100 },
  { 359 , 100 },
  { 363 , 100 },
  { 367 , 100 },
  { 371 , 100 },
  { 375 , 100 },
  { 378 , 100 },
  { 382 , 105 },
  { 386 , 105 },
  { 390 , 105 },
  { 394 , 105 },
  { 398 , 105 },
  { 402 , 105 },
  { 406 , 105 },
  { 410 , 110 },
  { 414 , 110 },
  { 418 , 110 },
  { 422 , 110 },
  { 425 , 110 },
  { 429 , 110 },
  { 433 , 110 },
  { 437 , 115 },
  { 441 , 115 },
  { 445 , 115 },
  { 449 , 115 },
  { 453 , 115 },
  { 457 , 115 },
  { 461 , 120 },
  { 465 , 120 },
  { 469 , 120 },
  { 473 , 120 },
  { 476 , 120 },
  { 480 , 120 },
  { 484 , 120 },
  { 488 , 125 },
  { 492 , 125 },
  { 496 , 125 },
  { 500 , 125 },
};

void Init_CapteurSoufflet()
{
  bool retour;

  retour = bmp.begin(ADDRI2C);
#if defined (DEBUG)
  Serial.println(F("Test BMP280"));
  if (!retour) {
    Serial.println(F("Ne trouve pas de capteur BMP280 valide, vérifier le cablage!"));
    while (1);
  }
#endif

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_1);   /* Standby time. 1 ms */

  lecture = bmp.readPressure();    //conversion float en int
  Etat_ReferenceCapteur = lecture;

#if defined (DEBUGCAPTEUR)
  Serial.print("Lecture Capteur Pression BMP280 ");
  Serial.print(lecture);
  Serial.print("\tReferenceCapteur ");
  Serial.println(Etat_ReferenceCapteur);
#endif
}
void Lecture_CapteurSoufflet()
{
  int i, j, delta_milli;
  int delta;
  long valeur;
  unsigned long millisec;

  lecture = bmp.readPressure();      //bmp.readPressure() returns a float

  Etat_Capteur[Etat_actuel] = lecture;  //conversion en int
  if (Etat_Capteur[Etat_avant] != Etat_Capteur[Etat_actuel]) {
    millisec = millis();
    Etat_Volume_Millis[Etat_actuel] = millisec;
    valeur = Etat_Capteur[Etat_actuel];
#if defined (DEBUGCAPTEUR)
    Serial.print(millisec);
    Serial.print("\tlecture_Pa ");
    Serial.print(lecture, 2);
    Serial.print("\tCh_capteur_Avant ");
    Serial.print(Etat_Capteur[Etat_avant]);
    Serial.print("\tActuel ");
    Serial.print(valeur);
#endif
    // Cacul d'une valeur en fonction de la référence et du capteur
    delta = Etat_ReferenceCapteur - Etat_Capteur[Etat_actuel];
#if defined (DEBUGCAPTEUR)
    Serial.print("\tdelta  ");
    Serial.print(delta);
#endif
    i = 0;
    while ( i < TailleTableau) {
      if (Const_Volume_Capteur[i][Const_Volume_Valeur_Capteur] >= delta) {
        Etat_Volume_Midi[Etat_actuel] = Const_Volume_Capteur[i][Const_Volume_Valeur_Midi];
#if defined (DEBUGCAPTEUR)
        Serial.print("\tVolume ");
        Serial.print(Etat_Volume_Midi[Etat_actuel]);
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
#if defined (DEBUGCAPTEUR)
    if (i == TailleTableau) {    //si valeur hors tableau
      Etat_Volume_Midi[Etat_actuel] = 127;
#if defined (DEBUGCAPTEUR)
      Serial.print("\tVolume ");
      Serial.print(Etat_Volume_Midi[Etat_actuel]);
      Serial.print("\t fin ");
      Serial.print(millis());
      Serial.println();
#endif
    }

#endif
  }
  else
    Etat_Volume_Midi[Etat_actuel] = 0;

#if defined (DEBUGCAPTEUR)
  lcd.setCursor(9, 3);
  if (lecture >= 0) Affiche = " ";
  else
    Affiche = "";
  Affiche += lecture;
  Affiche += " Pa";
  for (i = Affiche.length(); i < 11; i++)
    Affiche += " ";  // On remplit à blanc
  lcd.print(Affiche);
#endif
}
