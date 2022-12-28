/*
C++ Klasse für eine DMX gesteuerte Heiulm Nebel Seifenblasenmaschine
Author: Mathis Rathjen 
geschrieben am: 23.12.22
Zuletzt geaändert am:
*/
#include "HfbM.h"

    
    void HfbM::hwSetup()
    {
     this->dmxRx.begin();
     this->tft.begin();
    }

    //Methode aktualisiert das DMXWerte Array
    void HfbM::getDmxFrame()
    {
    int read = dmxRx.readPacket(packetBuf, startKanal, DMXCH);
     if (read == DMXCH) {
        if (memcmp(packetBuf, dmxValues, DMXCH) != 0) {
        memcpy(dmxValues, packetBuf, 3);
        }
    }
    }

    //Methode um Parameter der komponenten aus DMXV Array zu bestimmen
    void HfbM::getParams()
    {
        if(dmxValues[chHV-1] >= 225) heliumVentil = true;
        else if(dmxValues[chHV-1] < 225) heliumVentil = false;
        heliumFluss = dmxValues[chHF-1];
        radRPM = dmxValues[chWHEEL-1];
        fanPWMDuty = dmxValues[chFAN-1];
        nebelWert = dmxValues[chFOG-1];
    }

    //Methode um alle aktuellen Parameter auf die entsprechenden Module anzuwenden
    void HfbM::updateDevice()
    {
        digitalWrite(HVpin, heliumVentil);
        analogWrite(HFpin, heliumFluss);
        analogWrite(WHEELpin, radRPM);
        analogWrite(PWMpin, fanPWMDuty);
        analogWrite(FOGpin, nebelWert);
    }

    //Methode um DMXKanäle des Geräts, inkluse des Startkanals zuzuweisen
    void HfbM::dmxSetup(int chHv, int chHf, int chFog, int chFan, int chWheel, int ch)
    {
        this->chHV = chHv;
        this->chHF = chHf;
        this->chFOG = chFog;
        this->chFAN = chFan;
        this->chWHEEL = chWheel;
        this->startKanal = ch;
    }

    //Methode um DMXKanäle des Geräts, ohne Startkanal, einzustellen
    void HfbM::dmxSetup(int chHv, int chHf, int chFog, int chFan, int chWheel)
    {
        this->chHV = chHv;
        this->chHF = chHf;
        this->chFOG = chFog;
        this->chFAN = chFan;
        this->chWHEEL = chWheel;
    }

    //Methode um Startkanal einzustellen
    void HfbM::setFirstDmxCh(int ch)
    {
        this->startKanal = ch;
    }

    void HfbM::displayTest()
    {
        // Setze Hintergrundfarbe auf Schwarz
        tft.fillScreen(ILI9341_BLACK);
        // Setze Schriftfarbe auf Weiß
        tft.setTextColor(ILI9341_WHITE);
        // Setze Schriftgröße auf 36
        tft.setTextSize(3);
        // Zeichne "Hello World" auf dem Display
        tft.drawString("Hello World", 20, 20);
        tft.updateScreen();
    }

    