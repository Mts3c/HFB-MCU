/*

C++ Header Datei einer Klasse für eine DMX gesteuerte Heiulm Nebel Seifenblasenmaschine
Author: Mathis Rathjen 
geschrieben am: 23.12.22
Zuletzt geaändert am:

From Microscope over Macrosscope to Globalscope

Funktionsbaum:


This Code ist mainly controlled by TaskManagerIO, wich is used to schedule the main Eventes on this Devices.
Lamba Expressions are used for inline checking, in taskmanger schedule Call, which work should be done
*/

#include "HfbM.h"
#include <TaskManagerIO.h>
#include <math.h>

//TaskManager tm;
HfbM maschine;

int fps = 0;
bool fpsCountActive=true;

static inline void countFps(const int);

void setup() 
{
    
    
    //Setup funktionen
    maschine.hwSetup();
    maschine.displayTest();
    maschine.drawEeIcon();
    delay(2000);
    maschine.dispBlack();
    maschine.mainScreen();
    //maschine.drawIcon(140, 90, 0);
    Serial.begin(115200);
    //2DO: Taskmanger 
    //Kann das funktionieren?
    //Event um aktuellen DMX Frame zu empfangen
    taskManager.scheduleFixedRate(1, [] {
        if(maschine.getDmxAktiv()) maschine.getDmxFrame();
    });
    //Event um aktuellen Bildscgirm Bild zu zeichnen
    taskManager.scheduleFixedRate(10, [] {
        maschine.updateTsData();                                // Muss aufgerufen werden um interrupt variable zu prüfen
        if (maschine.getNewTouched()) maschine.drawActScreen(); // Hier noch "oder" mit in if bed. mit variabel falls anzuzeigende werte geändert wurden 
    });
    //Event um die Geräte parameter zu setzten
    taskManager.scheduleFixedRate(5, []{
        if(maschine.getNewDmxData() || !maschine.getParamsUptodate()) maschine.updateDevice(); // In GUI Implementierung muss paramsUptodate auf false gesetzt werden sobald benutzer manuelll betreibt
    });
    //Event um DMX indikator auf Display anzupasen
    taskManager.scheduleFixedRate(800, [] {
        if(maschine.getDmxAktiv())  maschine.drawDmxIndic();
        
});
    


    //Signals & Slots 

}

void loop() 
{

    //2DO: Taskmanger 
    taskManager.runLoop();
    countFps(10);
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
    framesPerSecond = (int)frameCount / (int)seconds;
    Serial.println(framesPerSecond);
    frameCount = 0;
    lastMillis = now;
  }
    //fps = framesPerSecond;
    //Serial.println(fps);
}
