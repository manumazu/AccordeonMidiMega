Version Mega_20201128-01
Supression #Define InstrumentFR TonaliteFR et OctaveFR
Supression No_PinOctave No_PinInstrument
Ajout de basses 12 touches
Gestion des accords de basses
Bouton tierces On/Off
Gestion de l'arrêt de la tierce en cours de jeu si bouton tierce Off
Gestion du changement de note en cours de jeu avec action poussé-tiré/tiré-poussé
No_PinTierceOnOff = A12;   
No_PinPousseTire = A13;   //A adapter après choix mécanisme pousse-tire et vélocité  


Version Mega_20201118-02
Affiche version au démarrage

Version Mega_20201118-01

Correction Gestion Codeur Octave pour les basses
Modif MidiVelocity pour les Basses

Version Mega_20201116-01

AccordeonMidiMega_20201116-01.ino
Modification LiquidCrystal_I2C lcd(0x27, 16, 2); 16 à la place de 12
Ajout traitement TouchePanic
  Boucle sur envoi NoteOff aux 128 notes Midi (0 à 127)
  InitMidi
  
MidiVelocity remplace valeur 127 sur appel fonction noteOn. MidiVelocity est défini dans Def_AccordeonMidi.h

Modification plage Codeur Octave de 0,+1 à -2,+2
  Gestion_Codeur.h
    modif codeur_position_Min et codeur_position_Max
  AccordeonMidiMega_20201116-01.ino
    Ajout de + ou - x fois octave
    Etat_Touche_Melodie[i][Note_encours] += (12 * Etat_ToucheOctave[Etat_actuel]);

______________________________________________________________________________________
