// Carte MEGA
//Pin Midi interne  On utilise le Serial1 avec la Pin TX 18
//Pin Midi externe  On utilise le Serial2 avec la Pin TX 16

// Midi Etendu
int BankMSB = 0;
int BankLSB = 0;
int Program = 0;

void Instrument(byte channel, int Program, int IntExt) {
  if (IntExt == Midi_Int) {
    Serial1.write(0xC0 | channel);
    Serial1.write(Program);    //Instrument
  }
  else  {// Midi Externe
    Serial2.write(0xB0 | channel);
    Serial2.write((byte)0x00);        //MSB
    Serial2.write(BankMSB);
    Serial2.write(0xB0 | channel);
    Serial2.write(0x20);       //LSB
    Serial2.write(BankLSB);    //Bank
    Serial2.write(0xC0 | channel);
    Serial2.write(Program);    //Program
  }
#if defined (DEBUG)
  AfficheDebug = "Appel Instrument Channel ";
  AfficheDebug += channel;
  AfficheDebug += " Program ";
  AfficheDebug += Program;
  AfficheDebug += " Midi ";
  AfficheDebug += IntExt ? "Externe" : "Interne";
  Serial.println(AfficheDebug);
#endif
}
void InitMidi() {
  // Changement de banque MSB LSB générique standard
  ///*
  BankMSB = 0;
  BankLSB = 0;
  Etat_ToucheInstrument[Etat_actuel] = InstrumentDef; // Voir Def_Accordeon.h
  Etat_ToucheInstrument[Etat_avant] = InstrumentDef;
  Etat_ToucheInstrumentE[Etat_actuel] = InstrumentExtDef; // Voir Def_Accordeon.h
  Etat_ToucheInstrumentE[Etat_avant] = InstrumentExtDef;

  Serial1.begin (31250);     //pour le midi Interne Tsunami 31250
  Serial1.write(0xFF);
  Serial2.begin (31250);     //pour le midi Externe 31250
  Serial2.write(0xFF);

  Instrument(ChannelMelodie, InstrumentDef, Midi_Int);
  Instrument(ChannelBasses, InstrumentDef, Midi_Int);
  Instrument(ChannelAccord, InstrumentDef, Midi_Int);
  Instrument(ChannelMelodie, InstrumentExtDef, Midi_Ext);
  Instrument(ChannelBasses, InstrumentExtDef, Midi_Ext);
  Instrument(ChannelAccord, InstrumentExtDef, Midi_Ext);
}

// pour noteOff mettre 0 dans velocity
void noteOn(byte channel, int pitch, int velocity) {
  // Midi Interne vers Tsunami
  Serial1.write(0X90 | channel);
  Serial1.write(pitch);
  Serial1.write(velocity);
  // Midi Externe
  Serial2.write(0X90 | channel);
  Serial2.write(pitch);
  Serial2.write(velocity);
}
