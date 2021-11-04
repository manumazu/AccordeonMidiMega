//Définition des valeurs du capteur pédale analogique et codage
//Le pousse-tire est effectué par un bouton

const int No_PinVolume = A7;     //Analogique pour volume
const int No_PinPousseTire = A8; //Analogique pour pousse-tire

int Etat_Capteur[2];  //Pas de valeur initiale, la pédale va de 0 à n, 0 étant volume 0

int lecture;
int lectureint;

const int ToleranceCapteur = 5;  //tolérance du capteur, si la variation est inférieure à la tolérance, on ne prend pas en compte la mesure
const int Const_Volume_Valeur_Capteur = 0;
const int Const_Volume_Valeur_Midi = 1;
const int Const_Expression_Valeur_Midi = 2;
const int Const_Gain_Valeur_Tsunami = 3;
const int IndexTableau = 4;
const int TailleTableau = 128;
int Const_Volume_Capteur[TailleTableau][IndexTableau] = {
  {  0 , 0 , 0 , -30 },
  { 6 , 1 , 1 , -30 },
  { 11  , 2 , 2 , -29 },
  { 17  , 3 , 2 , -29 },
  { 22  , 4 , 3 , -29 },
  { 28  , 5 , 4 , -29 },
  { 33  , 6 , 5 , -28 },
  { 39  , 7 , 6 , -28 },
  { 44  , 8 , 6 , -28 },
  { 50  , 9 , 7 , -28 },
  { 55  , 10  , 8 , -27 },
  { 61  , 11  , 9 , -27 },
  { 66  , 12  , 9 , -27 },
  { 72  , 13  , 10  , -27 },
  { 77  , 14  , 11  , -26 },
  { 83  , 15  , 12  , -26 },
  { 88  , 16  , 13  , -26 },
  { 94  , 17  , 13  , -25 },
  { 99  , 18  , 14  , -25 },
  { 105 , 19  , 15  , -25 },
  { 110 , 20  , 16  , -25 },
  { 116 , 21  , 17  , -24 },
  { 121 , 22  , 17  , -24 },
  { 127 , 23  , 18  , -24 },
  { 132 , 24  , 19  , -24 },
  { 138 , 25  , 20  , -23 },
  { 143 , 26  , 20  , -23 },
  { 149 , 27  , 21  , -23 },
  { 154 , 28  , 22  , -23 },
  { 160 , 29  , 23  , -22 },
  { 165 , 30  , 24  , -22 },
  { 171 , 31  , 24  , -22 },
  { 176 , 32  , 25  , -21 },
  { 182 , 33  , 26  , -21 },
  { 187 , 34  , 27  , -21 },
  { 193 , 35  , 28  , -21 },
  { 198 , 36  , 28  , -20 },
  { 204 , 37  , 29  , -20 },
  { 209 , 38  , 30  , -20 },
  { 215 , 39  , 31  , -20 },
  { 220 , 40  , 31  , -19 },
  { 226 , 41  , 32  , -19 },
  { 231 , 42  , 33  , -19 },
  { 237 , 43  , 34  , -18 },
  { 243 , 44  , 35  , -18 },
  { 248 , 45  , 35  , -18 },
  { 254 , 46  , 36  , -18 },
  { 259 , 47  , 37  , -17 },
  { 265 , 48  , 38  , -17 },
  { 270 , 49  , 39  , -17 },
  { 276 , 50  , 39  , -17 },
  { 281 , 51  , 40  , -16 },
  { 287 , 52  , 41  , -16 },
  { 292 , 53  , 42  , -16 },
  { 298 , 54  , 43  , -16 },
  { 303 , 55  , 43  , -15 },
  { 309 , 56  , 44  , -15 },
  { 314 , 57  , 45  , -15 },
  { 320 , 58  , 46  , -14 },
  { 325 , 59  , 46  , -14 },
  { 331 , 60  , 47  , -14 },
  { 336 , 61  , 48  , -14 },
  { 342 , 62  , 49  , -13 },
  { 347 , 63  , 50  , -13 },
  { 353 , 64  , 50  , -13 },
  { 358 , 65  , 51  , -13 },
  { 364 , 66  , 52  , -12 },
  { 369 , 67  , 53  , -12 },
  { 375 , 68  , 54  , -12 },
  { 380 , 69  , 54  , -12 },
  { 386 , 70  , 55  , -11 },
  { 391 , 71  , 56  , -11 },
  { 397 , 72  , 57  , -11 },
  { 402 , 73  , 57  , -10 },
  { 408 , 74  , 58  , -10 },
  { 413 , 75  , 59  , -10 },
  { 419 , 76  , 60  , -10 },
  { 424 , 77  , 61  , -9  },
  { 430 , 78  , 61  , -9  },
  { 435 , 79  , 62  , -9  },
  { 441 , 80  , 63  , -9  },
  { 446 , 81  , 64  , -8  },
  { 452 , 82  , 65  , -8  },
  { 457 , 83  , 65  , -8  },
  { 463 , 84  , 66  , -8  },
  { 469 , 85  , 67  , -7  },
  { 474 , 86  , 68  , -7  },
  { 480 , 87  , 69  , -7  },
  { 485 , 88  , 69  , -6  },
  { 491 , 89  , 70  , -6  },
  { 496 , 90  , 71  , -6  },
  { 502 , 91  , 72  , -6  },
  { 507 , 92  , 72  , -5  },
  { 513 , 93  , 73  , -5  },
  { 518 , 94  , 74  , -5  },
  { 524 , 95  , 75  , -5  },
  { 529 , 96  , 76  , -4  },
  { 535 , 97  , 76  , -4  },
  { 540 , 98  , 77  , -4  },
  { 546 , 99  , 78  , -3  },
  { 551 , 100 , 79  , -3  },
  { 557 , 101 , 80  , -3  },
  { 562 , 102 , 80  , -3  },
  { 568 , 103 , 81  , -2  },
  { 573 , 104 , 82  , -2  },
  { 579 , 105 , 83  , -2  },
  { 584 , 106 , 83  , -2  },
  { 590 , 107 , 84  , -1  },
  { 595 , 108 , 85  , -1  },
  { 601 , 109 , 86  , -1  },
  { 606 , 110 , 87  , -1  },
  { 612 , 111 , 87  , 0 },
  { 617 , 112 , 88  , 0 },
  { 623 , 113 , 89  , 0 },
  { 628 , 114 , 90  , 1 },
  { 634 , 115 , 91  , 1 },
  { 639 , 116 , 91  , 1 },
  { 645 , 117 , 92  , 1 },
  { 650 , 118 , 93  , 2 },
  { 656 , 119 , 94  , 2 },
  { 661 , 120 , 94  , 2 },
  { 667 , 121 , 95  , 2 },
  { 672 , 122 , 96  , 3 },
  { 678 , 123 , 97  , 3 },
  { 683 , 124 , 98  , 3 },
  { 689 , 125 , 98  , 3 },
  { 694 , 126 , 99  , 4 },
  { 700 , 127 , 100 , 4 },
};

void Init_CapteurSoufflet()
{
  int i;

  pinMode(No_PinVolume, INPUT);            //Analogique pédale
  pinMode(No_PinPousseTire, INPUT_PULLUP); //Digital bouton
  lecture = analogRead(No_PinVolume);
  lectureint = lecture;  //conversion en int
  Etat_Capteur[Etat_avant] = Etat_Capteur[Etat_actuel] = lectureint;

  i = 0;
  while ( i < TailleTableau) {
    if (Const_Volume_Capteur[i][Const_Volume_Valeur_Capteur] >= lectureint) {
      Etat_Volume_Midi[Etat_actuel] = Const_Volume_Capteur[i][Const_Volume_Valeur_Midi];
      Etat_Expression_Midi[Etat_actuel] = Const_Volume_Capteur[i][Const_Expression_Valeur_Midi];
      Etat_Gain_Tsunami[Etat_actuel] = Const_Volume_Capteur[i][Const_Gain_Valeur_Tsunami];
#if defined (DEBUGCAPTEUR)
      Serial.print("Lecture Pédale analogique ");
      Serial.print(lecture);
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
  if (i == TailleTableau) {    //si valeur hors tableau, on prend la dernière valeur du tableau
    Etat_Volume_Midi[Etat_actuel] = Const_Volume_Capteur[TailleTableau - 1][Const_Volume_Valeur_Midi];
    Etat_Expression_Midi[Etat_actuel] = Const_Volume_Capteur[TailleTableau - 1][Const_Expression_Valeur_Midi];
    Etat_Gain_Tsunami[Etat_actuel] = Const_Volume_Capteur[TailleTableau - 1][Const_Gain_Valeur_Tsunami];
#if defined (DEBUGCAPTEUR)
    Serial.print("Lecture Pédale analogique ");
    Serial.print(lecture);
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
  int delta;
  unsigned long millisec;

  lecture = analogRead(No_PinVolume);
  lectureint = lecture;  //conversion en int
  delta = abs(Etat_Capteur[Etat_avant] - lectureint);
  if (delta > ToleranceCapteur) {
    Etat_Capteur[Etat_actuel] = lectureint;
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
    Serial.print(lecture);
    Serial.print("\tAvant ");
    Serial.print(Etat_Capteur[Etat_avant]);
    Serial.print("\tActuel ");
    Serial.print(valeur);
#endif
    // Pas de delta sur pédale analogique, on prend la valeur de la pédale, on garde le delta pour le debug
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
    if (i == TailleTableau) {    //si valeur hors tableau, on prend la dernière valeur du tableau
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
#if defined (DEBUGCAPTLCD)
  lcd.setCursor(9, 3);
  Affiche = " ";
  Affiche += lecture;
  for (i = Affiche.length(); i < 11; i++)
    Affiche += " ";  // On remplit à blanc
  lcd.print(Affiche);
#endif
}
