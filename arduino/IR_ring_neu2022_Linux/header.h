RunningMedian<int,5> ballmedian;        // rechts ist plus, links ist minus
RunningMedian<int,5> balldistmedian;    // Distanz                                    nachher für den Master nur Median
RunningMedian<int,5> ballanglemedian;         // Winkel

int tssp_pin[] = {26, 25, 33, 32, 35, 34, 39, 36, 23, 22,  2, 15, 14, 27};      //14 Stück siehe unten
int wert[] =    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};      //14 Stück siehe unten

int wertmax;
int richtungmax;

int ballrichtung;
int ballmax;
byte zone;
byte can_richtung;

elapsedMillis zeit;                               // kann mit zeit = 0 gesetzt werden und läuft dann rund 8 Minuten

int durchgaenge;
int zaehler=0;
