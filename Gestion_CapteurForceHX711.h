//Définition des valeurs du capteur de force HX711 et codage

int Etat_Capteur[2];        //Conversion valeur float du capteur en int
int Etat_ReferenceCapteur;  //valeur du capteur au setup

#include "HX711.h"

#define calibration_factor -7050.0
#define No_Pin_HX711_Data  A12
#define No_Pin_HX711_Clock  A13

HX711 scale;

float lecture;

const int Const_Volume_Valeur_Capteur = 0;
const int Const_Volume_Valeur_Midi = 1;
const int TailleTableau = 256;
int Const_Volume_Capteur[TailleTableau][2] = {
  { -512  , 125 },
  { -508  , 125 },
  { -504  , 125 },
  { -500  , 125 },
  { -496  , 120 },
  { -492  , 120 },
  { -488  , 120 },
  { -484  , 120 },
  { -480  , 120 },
  { -476  , 120 },
  { -472  , 120 },
  { -468  , 115 },
  { -464  , 115 },
  { -460  , 115 },
  { -456  , 115 },
  { -452  , 115 },
  { -448  , 115 },
  { -444  , 110 },
  { -440  , 110 },
  { -436  , 110 },
  { -432  , 110 },
  { -428  , 110 },
  { -424  , 110 },
  { -420  , 110 },
  { -416  , 105 },
  { -412  , 105 },
  { -408  , 105 },
  { -404  , 105 },
  { -400  , 105 },
  { -396  , 105 },
  { -392  , 105 },
  { -388  , 100 },
  { -383  , 100 },
  { -379  , 100 },
  { -375  , 100 },
  { -371  , 100 },
  { -367  , 100 },
  { -363  , 95  },
  { -359  , 95  },
  { -355  , 95  },
  { -351  , 95  },
  { -347  , 95  },
  { -343  , 95  },
  { -339  , 95  },
  { -335  , 90  },
  { -331  , 90  },
  { -327  , 90  },
  { -323  , 90  },
  { -319  , 90  },
  { -315  , 90  },
  { -311  , 90  },
  { -307  , 85  },
  { -303  , 85  },
  { -299  , 85  },
  { -295  , 85  },
  { -291  , 85  },
  { -287  , 85  },
  { -283  , 80  },
  { -279  , 80  },
  { -275  , 80  },
  { -271  , 80  },
  { -267  , 80  },
  { -263  , 80  },
  { -259  , 80  },
  { -255  , 75  },
  { -251  , 75  },
  { -247  , 75  },
  { -243  , 75  },
  { -239  , 75  },
  { -235  , 75  },
  { -231  , 75  },
  { -227  , 70  },
  { -223  , 70  },
  { -219  , 70  },
  { -215  , 70  },
  { -211  , 70  },
  { -207  , 70  },
  { -203  , 65  },
  { -199  , 65  },
  { -195  , 65  },
  { -191  , 65  },
  { -187  , 65  },
  { -183  , 65  },
  { -179  , 65  },
  { -175  , 60  },
  { -171  , 60  },
  { -167  , 60  },
  { -163  , 60  },
  { -159  , 60  },
  { -155  , 60  },
  { -151  , 60  },
  { -147  , 55  },
  { -143  , 55  },
  { -139  , 55  },
  { -135  , 55  },
  { -131  , 55  },
  { -126  , 55  },
  { -122  , 50  },
  { -118  , 50  },
  { -114  , 50  },
  { -110  , 50  },
  { -106  , 50  },
  { -102  , 50  },
  { -98 , 50  },
  { -94 , 45  },
  { -90 , 45  },
  { -86 , 45  },
  { -82 , 45  },
  { -78 , 45  },
  { -74 , 45  },
  { -70 , 45  },
  { -66 , 40  },
  { -62 , 40  },
  { -58 , 40  },
  { -54 , 40  },
  { -50 , 40  },
  { -46 , 40  },
  { -42 , 35  },
  { -38 , 35  },
  { -34 , 35  },
  { -30 , 35  },
  { -26 , 35  },
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
  { 26  , 35  },
  { 30  , 35  },
  { 34  , 35  },
  { 38  , 35  },
  { 42  , 40  },
  { 46  , 40  },
  { 50  , 40  },
  { 54  , 40  },
  { 58  , 40  },
  { 62  , 40  },
  { 66  , 45  },
  { 70  , 45  },
  { 74  , 45  },
  { 78  , 45  },
  { 82  , 45  },
  { 86  , 45  },
  { 90  , 45  },
  { 94  , 50  },
  { 98  , 50  },
  { 102 , 50  },
  { 106 , 50  },
  { 110 , 50  },
  { 114 , 50  },
  { 118 , 50  },
  { 122 , 55  },
  { 126 , 55  },
  { 131 , 55  },
  { 135 , 55  },
  { 139 , 55  },
  { 143 , 55  },
  { 147 , 55  },
  { 151 , 60  },
  { 155 , 60  },
  { 159 , 60  },
  { 163 , 60  },
  { 167 , 60  },
  { 171 , 60  },
  { 175 , 65  },
  { 179 , 65  },
  { 183 , 65  },
  { 187 , 65  },
  { 191 , 65  },
  { 195 , 65  },
  { 199 , 65  },
  { 203 , 70  },
  { 207 , 70  },
  { 211 , 70  },
  { 215 , 70  },
  { 219 , 70  },
  { 223 , 70  },
  { 227 , 70  },
  { 231 , 75  },
  { 235 , 75  },
  { 239 , 75  },
  { 243 , 75  },
  { 247 , 75  },
  { 251 , 75  },
  { 255 , 80  },
  { 259 , 80  },
  { 263 , 80  },
  { 267 , 80  },
  { 271 , 80  },
  { 275 , 80  },
  { 279 , 80  },
  { 283 , 85  },
  { 287 , 85  },
  { 291 , 85  },
  { 295 , 85  },
  { 299 , 85  },
  { 303 , 85  },
  { 307 , 85  },
  { 311 , 90  },
  { 315 , 90  },
  { 319 , 90  },
  { 323 , 90  },
  { 327 , 90  },
  { 331 , 90  },
  { 335 , 90  },
  { 339 , 95  },
  { 343 , 95  },
  { 347 , 95  },
  { 351 , 95  },
  { 355 , 95  },
  { 359 , 95  },
  { 363 , 100 },
  { 367 , 100 },
  { 371 , 100 },
  { 375 , 100 },
  { 379 , 100 },
  { 383 , 100 },
  { 388 , 100 },
  { 392 , 105 },
  { 396 , 105 },
  { 400 , 105 },
  { 404 , 105 },
  { 408 , 105 },
  { 412 , 105 },
  { 416 , 105 },
  { 420 , 110 },
  { 424 , 110 },
  { 428 , 110 },
  { 432 , 110 },
  { 436 , 110 },
  { 440 , 110 },
  { 444 , 110 },
  { 448 , 115 },
  { 452 , 115 },
  { 456 , 115 },
  { 460 , 115 },
  { 464 , 115 },
  { 468 , 115 },
  { 472 , 120 },
  { 476 , 120 },
  { 480 , 120 },
  { 484 , 120 },
  { 488 , 120 },
  { 492 , 120 },
  { 496 , 120 },
  { 500 , 125 },
  { 504 , 125 },
  { 508 , 125 },
  { 512 , 125 },
};

void Init_CapteurSoufflet()
{

  scale.begin(No_Pin_HX711_Data, No_Pin_HX711_Clock);
  scale.set_scale(calibration_factor);
  scale.tare();  //On laisse Robert tranquille à l'allumage !

  lecture = scale.get_units();      //scale.get_units() returns a float
  Etat_ReferenceCapteur = Etat_Capteur[Etat_actuel] = lecture;  //conversion en int

#if defined (DEBUGCAPTEUR)
  Serial.print("Lecture Capteur Force HX711 ");
  Serial.print(lecture);
  Serial.print("\tEtat_Capteur ");
  Serial.println(Etat_Capteur[Etat_actuel]);
#endif
}

void Lecture_CapteurSoufflet()
{
  int i, j, valeur, valeurcalc, delta_milli;
  int delta;
  unsigned long millisec;

  lecture = scale.get_units();      //scale.get_units() returns a float
  Etat_Capteur[Etat_actuel] = lecture;  //conversion en int
  if (Etat_Capteur[Etat_avant] != Etat_Capteur[Etat_actuel]) {
    millisec = millis();
    Etat_Volume_Millis[Etat_actuel] = millisec;
    valeur = Etat_Capteur[Etat_actuel];
#if defined (DEBUGCAPTEUR)
    Serial.print(millisec);
    Serial.print("\tCh_capteur ");
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
  Affiche += " lbs";
  for (i = Affiche.length(); i < 11; i++)
    Affiche += " ";  // On remplit à blanc
  lcd.print(Affiche);
#endif
}
