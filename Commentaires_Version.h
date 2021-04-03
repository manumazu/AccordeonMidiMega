Version à changer dans AccordeonMidiMega.ino à chaque versement sur GitHub !

Version "Robert_20210403-1   "
Codage pour gestion capteur de force HX711, pression BM280, pression Honeywell ou pédale analogique+bouton pousse-tire avec un .h par type de capteur
Remplissage du tableau Const_Volume_Capteur à partir d'une feuille Excel
Nettoyage de noms de variables et création de noteOff
Affichage du volume/vélocité sur LCD ligne 2
Version Volume avec 1 capteur de force ou pression et calcul de vélocité, pas de changement pour les notes en cours de jeu
Option de compil pour appliquer la valeur calculée capteur / pédale au volume Midi ou à la vélocité midi
Changement deMute ampli pin A5

Version "Robert_20210212-1   "
Formatage "automatique" par Ctrl t de tous les fichiers du croquis pour faciliter la comparaison entre version
Maj affichage Tona, Program pour Midi Externe et autres info
deMute ampli après l'init sur pin 53
Actions sur Registre avec Basson par défaut (BA, BA / FL, FL), le tableau de notes mélodie est doublée (Basson, Flûte)
Les changements Pousse - tire, Tierces, Basses profondes et registre sont appliqués sur l'ensemble des notes en cours de jeu
Le changement d'instrument doit se faire sans jeu de note en cours

Version "Robert_20210124-1   "
Changement afficheur Lcd => 4 lignes 20 caractères
Implémentation multiplexeurs pour touches Basses et Accords (1 bouton en réserve mux2 addr 7)
Duplication du même codeur Instrument pour mémoriser Instrument Midi en interne et externe
Codeur Renversement accord, trois valeurs: normal, 1er renversement, 2eme renversement
Poussoir Registre Flute, Basson, Flute / Basson  sur Mux mais sans action
Midi généré simultanément en Interne ET externe
Instrument Midi interne par défaut 0 => 1ere bank Tsunami
MAj liste instruments Midi Interne (Banks Tsunami)
Pédale Vélocité provisoire devient pédale Volume mais sans action, la vélocité est fixée par paramétrage
Ajout Canal Midi différencié avec même "program" Midi
Canal 1 Mélodie
Canal 2 Basses
Canal 3 Accord

Version Mega_20201208
Provisoire Pédale Vélocité sur entrée analogique
Codeur Midi pour gérer les bank MSB et LSB, avec  Flipflop MSB / LSB par switch du codeur

Version Mega_20201205
Bouton tierces n'agit que sur les accords
Ajout Bouton Basses Profondes
Correction ajustement tona sur Basses et Accords

Version Mega_20201204
Maj Tableau touches Mélodies et Basses

Version Mega_20201128 - 01
Suppression #Define InstrumentFR TonaliteFR et OctaveFR
Suppression No_PinOctave No_PinInstrument
Ajout de basses 12 touches
Gestion des accords de basses
Bouton tierces On / Off
Gestion de l'arrêt de la tierce en cours de jeu si bouton tierce Off
Gestion du changement de note en cours de jeu avec action poussé -tiré / tiré -poussé
No_PinTierceOnOff = A12;
No_PinPousseTire = A13;   //A adapter après choix mécanisme pousse-tire et vélocité


Version Mega_20201118 - 02
Affiche version au démarrage

Version Mega_20201118 - 01

Correction Gestion Codeur Octave pour les basses
Modif MidiVelocity pour les Basses

Version Mega_20201116 - 01

AccordeonMidiMega_20201116 - 01.ino
Modification LiquidCrystal_I2C lcd(0x27, 16, 2); 16 à la place de 12
Ajout traitement TouchePanic
Boucle sur envoi NoteOff aux 128 notes Midi (0 à 127)
InitMidi

MidiVelocity remplace valeur 127 sur appel fonction noteOn. MidiVelocity est défini dans Def_AccordeonMidi.h

Modification plage Codeur Octave de 0, +1 à -2, +2
Gestion_Codeur.h
modif codeur_position_Min et codeur_position_Max
AccordeonMidiMega_20201116 - 01.ino
Ajout de + ou - x fois octave
Etat_Touche_Melodie[i][Note1_encours] += (12 * Etat_Octave[Etat_actuel]);

______________________________________________________________________________________
