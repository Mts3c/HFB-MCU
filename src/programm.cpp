/*

C++ Header Datei einer Klasse für eine DMX gesteuerte Heiulm Nebel Seifenblasenmaschine
Author: Mathis Rathjen 
geschrieben am: 23.12.22
Zuletzt geaändert am:

From Microscope over Macrosscope to Globalscope

Funktionsbaum:


This Code ist mainly controlled by TaskManagerIO, wich is used to schedule the main Eventes on this Device.
Lamba Expressions are used for inline checking, in taskmanger schedule Call, which work should be done
*/

#include "HfbM.h"
#include <TaskManagerIO.h>
//#include <math.h>

//TaskManager tm;
HfbM maschine;

int fps = 0;
bool fpsCountActive=true;

static inline void countFps(const int);

void setup() 
{
    //Setup funktionen
    maschine.hwSetup();
    maschine.dispBlack();
    Serial.begin(115200);

    //Taskmanger 
    //Event um aktuellen DMX Frame zu empfangen
    taskManager.scheduleFixedRate(1, [] 
    {
        if(maschine.getDmxAktiv()) maschine.getDmxFrame();
    });

    //Event um aktuellen Bildscgirm Bild zu zeichnen
    taskManager.scheduleFixedRate(10, [] 
    {                       
        if (!maschine.tftUpdated) maschine.mainScreen(); // Hier noch "oder" mit in if bed. mit variabel falls anzuzeigende werte geändert wurden 
    });

    //Event um die Geräte parameter zu setzten
    taskManager.scheduleFixedRate(5, []
    {
        if(maschine.getNewDmxData() || !maschine.getParamsUptodate()) maschine.updateDevice(); // In GUI Implementierung muss paramsUptodate auf false gesetzt werden sobald benutzer manuelll betreibt
    });

    //Event um DMX indikator auf Display anzupasen
    taskManager.scheduleFixedRate(800, [] 
    {
        if(maschine.getDmxAktiv())  maschine.drawDmxIndic();
    });

    maschine.bootScreen();
}

void loop() 
{
    taskManager.runLoop();
    countFps(5);
}

static inline void countFps(const int seconds){
  // Create static variables so that the code and variables can
  // all be declared inside a function
  static unsigned long lastMillis;
  static unsigned long frameCount;
  static unsigned int framesPerSecond;
  
  // It is best if we declare millis() only once
  unsigned long now = millis();
  frameCount ++;
  if (now - lastMillis >= seconds * 1000) {
    framesPerSecond = frameCount / seconds;
    Serial.println(framesPerSecond);
    frameCount = 0;
    lastMillis = now;
  }
}
