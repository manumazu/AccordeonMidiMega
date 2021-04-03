//Définition des valeurs du capteur pédale analogique et codage
//Le pousse-tire est effectué par un bouton

const int No_PinVolume = A14;     //Analogique pour volume
const int No_PinPousseTire = A15; //Analogique pour pousse-tire

int Etat_Capteur[2];  //Pas de valeur initiale, la pédale va de 0 à n, 0 étant volume 0

int lecture;

const int Const_Volume_Valeur_Capteur = 0;
const int Const_Volume_Valeur_Midi = 1;
const int TailleTableau = 128;
int Const_Volume_Capteur[TailleTableau][2] = {
  { 0 , 0 },
  { 6 , 1 },
  { 11  , 2 },
  { 17  , 3 },
  { 22  , 4 },
  { 28  , 5 },
  { 33  , 6 },
  { 39  , 7 },
  { 44  , 8 },
  { 50  , 9 },
  { 55  , 10  },
  { 61  , 11  },
  { 66  , 12  },
  { 72  , 13  },
  { 77  , 14  },
  { 83  , 15  },
  { 88  , 16  },
  { 94  , 17  },
  { 99  , 18  },
  { 105 , 19  },
  { 110 , 20  },
  { 116 , 21  },
  { 121 , 22  },
  { 127 , 23  },
  { 132 , 24  },
  { 138 , 25  },
  { 143 , 26  },
  { 149 , 27  },
  { 154 , 28  },
  { 160 , 29  },
  { 165 , 30  },
  { 171 , 31  },
  { 176 , 32  },
  { 182 , 33  },
  { 187 , 34  },
  { 193 , 35  },
  { 198 , 36  },
  { 204 , 37  },
  { 209 , 38  },
  { 215 , 39  },
  { 220 , 40  },
  { 226 , 41  },
  { 231 , 42  },
  { 237 , 43  },
  { 243 , 44  },
  { 248 , 45  },
  { 254 , 46  },
  { 259 , 47  },
  { 265 , 48  },
  { 270 , 49  },
  { 276 , 50  },
  { 281 , 51  },
  { 287 , 52  },
  { 292 , 53  },
  { 298 , 54  },
  { 303 , 55  },
  { 309 , 56  },
  { 314 , 57  },
  { 320 , 58  },
  { 325 , 59  },
  { 331 , 60  },
  { 336 , 61  },
  { 342 , 62  },
  { 347 , 63  },
  { 353 , 64  },
  { 358 , 65  },
  { 364 , 66  },
  { 369 , 67  },
  { 375 , 68  },
  { 380 , 69  },
  { 386 , 70  },
  { 391 , 71  },
  { 397 , 72  },
  { 402 , 73  },
  { 408 , 74  },
  { 413 , 75  },
  { 419 , 76  },
  { 424 , 77  },
  { 430 , 78  },
  { 435 , 79  },
  { 441 , 80  },
  { 446 , 81  },
  { 452 , 82  },
  { 457 , 83  },
  { 463 , 84  },
  { 469 , 85  },
  { 474 , 86  },
  { 480 , 87  },
  { 485 , 88  },
  { 491 , 89  },
  { 496 , 90  },
  { 502 , 91  },
  { 507 , 92  },
  { 513 , 93  },
  { 518 , 94  },
  { 524 , 95  },
  { 529 , 96  },
  { 535 , 97  },
  { 540 , 98  },
  { 546 , 99  },
  { 551 , 100 },
  { 557 , 101 },
  { 562 , 102 },
  { 568 , 103 },
  { 573 , 104 },
  { 579 , 105 },
  { 584 , 106 },
  { 590 , 107 },
  { 595 , 108 },
  { 601 , 109 },
  { 606 , 110 },
  { 612 , 111 },
  { 617 , 112 },
  { 623 , 113 },
  { 628 , 114 },
  { 634 , 115 },
  { 639 , 116 },
  { 645 , 117 },
  { 650 , 118 },
  { 656 , 119 },
  { 661 , 120 },
  { 667 , 121 },
  { 672 , 122 },
  { 678 , 123 },
  { 683 , 124 },
  { 689 , 125 },
  { 694 , 126 },
  { 700 , 127 },
};

void Init_CapteurSoufflet()
{
  pinMode(No_PinVolume, INPUT);            //Analogique pédale
  pinMode(No_PinPousseTire, INPUT_PULLUP); //Digital bouton
  lecture = analogRead(No_PinVolume);
  lecture = lecture - lecture % 2; // on arrondi au module 2 pour limiter les variations

#if defined (DEBUGCAPTEUR)
  Serial.print("Lecture arrondi Pédale analogique ");
  Serial.println(lecture);
#endif
}
void Lecture_CapteurSoufflet()
{
  int i, j, valeur, valeurcalc, delta_milli;
  int delta;
  unsigned long millisec;

  lecture = analogRead(No_PinVolume);
  lecture = lecture - lecture % 2; // on arrondi au module 2 pour limiter les variations

  Etat_Capteur[Etat_actuel] = lecture;
  if (Etat_Capteur[Etat_avant] != Etat_Capteur[Etat_actuel]) {
    millisec = millis();
    Etat_Volume_Millis[Etat_actuel] = millisec;
    valeur = Etat_Capteur[Etat_actuel];
#if defined (DEBUGCAPTEUR)
    Serial.print(millisec);
    Serial.print("\tCh_capteur_Avant ");
    Serial.print(Etat_Capteur[Etat_avant]);
    Serial.print("\tActuel ");
    Serial.print(valeur);
#endif
    // Pas de delta sur pédale analogique, on prend la valeur de la pédale, on garde le delta pour lee debug
    valeur = Etat_Capteur[Etat_actuel];
    delta = Etat_Capteur[Etat_actuel] - Etat_Capteur[Etat_avant];
#if defined (DEBUGCAPTEUR)
    Serial.print("\tdelta  ");
    Serial.print(delta);
#endif
    i = 0;
    while ( i < TailleTableau) {
      if (Const_Volume_Capteur[i][Const_Volume_Valeur_Capteur] >= valeur) {
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

#if defined (DEBUGCAPTEUR)
  lcd.setCursor(9, 3);
  Affiche = " ";
  Affiche += lecture;
  for (i = Affiche.length(); i < 11; i++)
    Affiche += " ";  // On remplit à blanc
  lcd.print(Affiche);
#endif
}
