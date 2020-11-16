// Carte MEGA
//Pin Midi interne  On utilise le Serial1 avec la Pin TX 18
//Pin Midi externe  On utilise le Serial2 avec la Pin TX 16
const int Midi_Interne = OFF;   //Etat initial du bouton Midi Interne/Externe sur Interne au repos

// Midi Etendu
int BankMSB = 0;
int BankLSB = 0;
int Program = 0;

void Instrument(int Program) {
  if (Etat_ToucheMidiInExt[Etat_actuel] == Midi_Interne) {
    Serial1.write(0xB0);
    Serial1.write((byte)0x00);        //MSB
    Serial1.write(BankMSB);
    Serial1.write(0xBC);
    Serial1.write(0x20);       //LSB
    Serial1.write(BankLSB);    //Bank
    Serial1.write(0xC0);
    Serial1.write(Program);    //Instrument
  }
  else  {// Midi Externe
    Serial2.write(0xB0);
    Serial2.write((byte)0x00);        //MSB
    Serial2.write(BankMSB);
    Serial2.write(0xBC);
    Serial2.write(0x20);       //LSB
    Serial2.write(BankLSB);    //Bank
    Serial2.write(0xC0);
    Serial2.write(Program);    //Instrument
  }
}
void InitMidi(int cmd) {
  // Changement de banque MSB LSB générique standard
  ///*
  BankMSB = 0;
  BankLSB = 0;
  Etat_ToucheInstrument[Etat_actuel] = InstrumentDef; // Voir Def_Accordeon.h
  Etat_ToucheInstrument[Etat_avant] = InstrumentDef;

  if (Etat_ToucheMidiInExt[Etat_actuel] == Midi_Interne) {
    Serial1.begin (31250);     //pour le midi 31250
    Serial1.write(0xFF);
  }
  else {
    Serial2.begin (31250);     //pour le midi 31250
    Serial2.write(0xFF);
  }
  Instrument(InstrumentDef);
}

// pour noteOff mettre 0 dans velocity
void noteOn(int cmd, int pitch, int velocity) {
  if (Etat_ToucheMidiInExt[Etat_actuel] == Midi_Interne) {
    Serial1.write(cmd);
    Serial1.write(pitch);
    Serial1.write(velocity);
  }
  else {
    Serial2.write(cmd);
    Serial2.write(pitch);
    Serial2.write(velocity);
  }
}
