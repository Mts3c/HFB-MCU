/*
C++ Klasse für eine DMX gesteuerte Heiulm Nebel Seifenblasenmaschine
Author: Mathis Rathjen 
geschrieben am: 23.12.22
Zuletzt geändert am:
*/
#include "HfbM.h"
#include "myIcons.h"

    

    /// @brief Hardware Setup Methode für Settup Schleife
    void HfbM::hwSetup()
    {
        //DMX
        this->dmxRx.begin();

        //Display Stuff
        this->tft.begin();
        this->tft.setRotation(3);
    }

    /// @brief Methode aktualisiert das DMXWerte Array
    void HfbM::getDmxFrame()
    {
        newDmxData = false;
        int read = dmxRx.readPacket(packetBuf, startKanal, DMXCH);
        if (read == DMXCH) {
            if (memcmp(packetBuf, dmxValues, DMXCH) != 0) {
                memcpy(dmxValues, packetBuf, 3);
                newDmxData = true;
            }
        }
    }

    /// @brief Methode um Parameter der komponenten aus DMXV Array zu bestimmen
    void HfbM::getParams()
    {
        if(dmxValues[chHV-1] >= 225) heliumVentil = true;
        else if(dmxValues[chHV-1] < 225) heliumVentil = false;
        heliumFluss = dmxValues[chHF-1];
        radRPM = dmxValues[chWHEEL-1];
        fanPWMDuty = dmxValues[chFAN-1];
        nebelWert = dmxValues[chFOG-1];
    }

    /// @brief Methode um alle aktuellen Parameter auf die entsprechenden Module anzuwenden
    void HfbM::updateDevice()
    {
        digitalWrite(HVpin, heliumVentil);
        analogWrite(HFpin, heliumFluss);
        analogWrite(WHEELpin, radRPM);
        analogWrite(PWMpin, fanPWMDuty);
        analogWrite(FOGpin, nebelWert);
        paramsUptodate = true;
    }

    /// @brief Methode um DMXKanäle des Geräts, inklusive des Startkanals zuzuweisen
    /// @param chHv Dmx Kanal des Heliumventils
    /// @param chHf Dmx Kanal des Heliumdurchflussventils
    /// @param chFog Dmx Kanal für Nebelmaschine
    /// @param chFan  Dmx Kanal für PWM Lüfter
    /// @param chWheel Dmx Kanal für Bubblewheel
    /// @param ch Dmx StartKanal dür fortlaufendes Kanalmapping
    void HfbM::dmxSetup(int chHv, int chHf, int chFog, int chFan, int chWheel, int ch)
    {
        this->chHV = chHv;
        this->chHF = chHf;
        this->chFOG = chFog;
        this->chFAN = chFan;
        this->chWHEEL = chWheel;
        this->startKanal = ch;
    }

    /// @brief Methode um DMXKanäle des Geräts, ohne Startkanal, einzustellen
    /// @param chHv Dmx Kanal des Heliumventils
    /// @param chHf Dmx Kanal des Heliumdurchflussventils
    /// @param chFog Dmx Kanal für Nebelmaschine
    /// @param chFan  Dmx Kanal für PWM Lüfter
    /// @param chWheel Dmx Kanal für Bubblewheel
    void HfbM::dmxSetup(int chHv, int chHf, int chFog, int chFan, int chWheel)
    {
        this->chHV = chHv;
        this->chHF = chHf;
        this->chFOG = chFog;
        this->chFAN = chFan;
        this->chWHEEL = chWheel;
    }

    /// @brief Methode um Startkanal einzustellen
    /// @param ch Erster DMX Kanal der eingestellt wird.
    void HfbM::setFirstDmxCh(int ch)
    {
        this->startKanal = ch;
    }



    /// @brief Testfunktion für ILI9341 Display
    void HfbM::displayTest()
    {

    }

    /// @brief Methode um den Hauptbildschirm des GUIs zu erzeugen
    void HfbM::mainScreen()
    {
        //Linie oben
        tft.drawLine(0, 18, 320, 18, ILI9341_ORANGE);
        tft.drawLine(0, 17, 320, 17, ILI9341_ORANGE);

        //Bar oben
        tft.setFont(&FreeSansBold9pt7b);
        tft.setCursor(1, 12);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(0);
        tft.println("DMX       Add:             Batt:");

        tft.setCursor(120, 12);
        tft.println(startKanal);                         
        tft.drawRect(46, 0, 13, 13, ILI9341_WHITE);

        //DMX indikator
        dispDmxSq();

        //Tastensperre gesetzt?
        tastenSperre? drawLock() : drawUnlock();
    }

    

    /// @brief Methode füllt anhand Membervariabel, dmxAktiv, Quadrat auf Display rot oder grün
    void HfbM::dispDmxSq()
    {
        dmxAktiv? tft.fillRect(47, 1, 11, 11, ILI9341_GREEN) : tft.fillRect(47, 1, 11, 11, ILI9341_RED);
    }

    /// @brief Methode zeichnet Tastensperre Icon als GESPERRT
    void HfbM::drawLock()
    {
        //Position von Lock
        uint16_t x = 312, y = 6;
        //delete unlock
        tft.drawRect(x, y, 7, 6, ILI9341_BLACK);
        tft.drawPixel(x+1, y-1, ILI9341_BLACK);
        tft.drawPixel(x+1, y-2, ILI9341_BLACK);
        tft.drawPixel(x+1, y-3, ILI9341_BLACK);
        tft.drawPixel(x, y-4, ILI9341_BLACK);
        tft.drawPixel(x-1, y-4, ILI9341_BLACK);
        tft.drawPixel(x-2, y-4, ILI9341_BLACK);
        tft.drawPixel(x-3, y-3, ILI9341_BLACK);
        tft.drawPixel(x-3, y-2, ILI9341_BLACK);
        tft.drawPixel(x-3, y-1, ILI9341_BLACK);
        //draw lock
        tft.drawRect(x, y, 7, 6, ILI9341_RED);
        tft.drawPixel(x+1, y-1, ILI9341_RED);
        tft.drawPixel(x+1, y-2, ILI9341_RED);
        tft.drawPixel(x+1, y-3, ILI9341_RED);
        tft.drawPixel(x+2, y-4, ILI9341_RED);
        tft.drawPixel(x+3, y-4, ILI9341_RED);
        tft.drawPixel(x+4, y-4, ILI9341_RED);
        tft.drawPixel(x+5, y-3, ILI9341_RED);
        tft.drawPixel(x+5, y-2, ILI9341_RED);
        tft.drawPixel(x+5, y-1, ILI9341_RED);
    }

    /// @brief Methode zeichnet Tastensperre Icon als ENTSPERRT
    void HfbM::drawUnlock()
    {   
        //Position von Lock
        uint16_t x = 312, y = 6;
        //delete lock
        tft.drawRect(x, y, 7, 6, ILI9341_BLACK);
        tft.drawPixel(x+1, y-1, ILI9341_BLACK);
        tft.drawPixel(x+1, y-2, ILI9341_BLACK);
        tft.drawPixel(x+1, y-3, ILI9341_BLACK);
        tft.drawPixel(x+2, y-4, ILI9341_BLACK);
        tft.drawPixel(x+3, y-4, ILI9341_BLACK);
        tft.drawPixel(x+4, y-4, ILI9341_BLACK);
        tft.drawPixel(x+5, y-3, ILI9341_BLACK);
        tft.drawPixel(x+5, y-2, ILI9341_BLACK);
        tft.drawPixel(x+5, y-1, ILI9341_BLACK);
        //draw unlock
        tft.drawRect(x, y, 7, 6, ILI9341_WHITE);
        tft.drawPixel(x+1, y-1, ILI9341_WHITE);
        tft.drawPixel(x+1, y-2, ILI9341_WHITE);
        tft.drawPixel(x+1, y-3, ILI9341_WHITE);
        tft.drawPixel(x, y-4, ILI9341_WHITE);
        tft.drawPixel(x-1, y-4, ILI9341_WHITE);
        tft.drawPixel(x-2, y-4, ILI9341_WHITE);
        tft.drawPixel(x-3, y-3, ILI9341_WHITE);
        tft.drawPixel(x-3, y-2, ILI9341_WHITE);
        tft.drawPixel(x-3, y-1, ILI9341_WHITE);
    }

    /// @brief Methode setzt Tastensperre Icon entsprechend Membervariable "tastenSperre"
    void HfbM:: switchLock()
    {
        if(tastenSperre)
        {
            //Position von Lock
            uint16_t x = 312, y = 6;
            //delete unlock
            tft.drawRect(x, y, 7, 6, ILI9341_BLACK);
            tft.drawPixel(x+1, y-1, ILI9341_BLACK);
            tft.drawPixel(x+1, y-2, ILI9341_BLACK);
            tft.drawPixel(x+1, y-3, ILI9341_BLACK);
            tft.drawPixel(x, y-4, ILI9341_BLACK);
            tft.drawPixel(x-1, y-4, ILI9341_BLACK);
            tft.drawPixel(x-2, y-4, ILI9341_BLACK);
            tft.drawPixel(x-3, y-3, ILI9341_BLACK);
            tft.drawPixel(x-3, y-2, ILI9341_BLACK);
            tft.drawPixel(x-3, y-1, ILI9341_BLACK);
            //draw lock
            tft.drawRect(x, y, 7, 6, ILI9341_RED);
            tft.drawPixel(x+1, y-1, ILI9341_RED);
            tft.drawPixel(x+1, y-2, ILI9341_RED);
            tft.drawPixel(x+1, y-3, ILI9341_RED);
            tft.drawPixel(x+2, y-4, ILI9341_RED);
            tft.drawPixel(x+3, y-4, ILI9341_RED);
            tft.drawPixel(x+4, y-4, ILI9341_RED);
            tft.drawPixel(x+5, y-3, ILI9341_RED);
            tft.drawPixel(x+5, y-2, ILI9341_RED);
            tft.drawPixel(x+5, y-1, ILI9341_RED);
        }
        else if(!tastenSperre)
        {
            //Position von Lock
            uint16_t x = 312, y = 6;
            //delete lock
            tft.drawRect(x, y, 7, 6, ILI9341_BLACK);
            tft.drawPixel(x+1, y-1, ILI9341_BLACK);
            tft.drawPixel(x+1, y-2, ILI9341_BLACK);
            tft.drawPixel(x+1, y-3, ILI9341_BLACK);
            tft.drawPixel(x+2, y-4, ILI9341_BLACK);
            tft.drawPixel(x+3, y-4, ILI9341_BLACK);
            tft.drawPixel(x+4, y-4, ILI9341_BLACK);
            tft.drawPixel(x+5, y-3, ILI9341_BLACK);
            tft.drawPixel(x+5, y-2, ILI9341_BLACK);
            tft.drawPixel(x+5, y-1, ILI9341_BLACK);
            //draw unlock
            tft.drawRect(x, y, 7, 6, ILI9341_WHITE);
            tft.drawPixel(x+1, y-1, ILI9341_WHITE);
            tft.drawPixel(x+1, y-2, ILI9341_WHITE);
            tft.drawPixel(x+1, y-3, ILI9341_WHITE);
            tft.drawPixel(x, y-4, ILI9341_WHITE);
            tft.drawPixel(x-1, y-4, ILI9341_WHITE);
            tft.drawPixel(x-2, y-4, ILI9341_WHITE);
            tft.drawPixel(x-3, y-3, ILI9341_WHITE);
            tft.drawPixel(x-3, y-2, ILI9341_WHITE);
            tft.drawPixel(x-3, y-1, ILI9341_WHITE);
        }
    }

    void HfbM:: drawActScreen(){
        //2DO!!
    }

    /// @brief Methode um den Bildschirm für manuellen Betrieb zu erzeugen 
    void HfbM:: manualScreen()
    {
        
    }
    /// @brief überprüft ob es eine touchscreen eingabe gab, falls ja werden die koordinaten in p geschrieben
    void HfbM:: updateTsData()
    {
        if(ts.tirqTouched()){
            p = ts.getPoint();
            newTouched = true;
        }
        else newTouched = false;
    }
    