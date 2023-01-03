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
        dmxRx.begin();

        //Display Stuff
        tft.begin();
        tft.setRotation(3);
    }

    /// @brief Methode aktualisiert das DMXWerte Array, und ruft getParams() auf
    void HfbM::getDmxFrame()
    {
        newDmxData = false;
        int read = dmxRx.readPacket(packetBuf, startKanal, DMXCH);
        if (read == DMXCH) {
            if (memcmp(packetBuf, dmxValues, DMXCH) != 0) {
                memcpy(dmxValues, packetBuf, 3);
                newDmxData = true;
                paramsUptodate = false;
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
        paramsUptodate = true;
    }

    /// @brief Methode ruft getParams() auf um anschließend alle aktuellen Parameter auf die entsprechenden Module anzuwenden
    void HfbM::updateDevice()
    {
        if(newDmxData)getParams();
        newDmxData = false;
        digitalWrite(HVpin, heliumVentil);
        analogWrite(HFpin, heliumFluss);
        analogWrite(WHEELpin, radRPM);
        analogWrite(PWMpin, fanPWMDuty);
        analogWrite(FOGpin, nebelWert);
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
        if(0< ch >=512) this->startKanal = ch;
        else startKanal =1;
    }



    /// @brief Testfunktion für ILI9341 Display
    void HfbM::displayTest()
    {

    }
    void HfbM::dispBlack()
    {
        tft.fillScreen(ILI9341_BLACK);
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
        drawDmxIndic();
        //Tastensperre gesetzt?
        tastenSperre? drawLock() : drawUnlock();
        drawIcon(0,26,5);
        drawIcon(0,80,3);
        drawIcon(0,136,2);
        drawIcon(0,192,1);
        drawIcon(160,26,4);
        drawIcon(160,80,0);
        tft.drawRect(160,130,160,110, ILI9341_WHITE);
        tftUpdated = true;
    }

    

    /// @brief Methode füllt anhand Membervariabel, dmxAktiv, Quadrat auf Display rot oder grün
    void HfbM::drawDmxIndic()
    {
        dmxAktiv? tft.fillRect(47, 1, 11, 11, ILI9341_GREEN) : tft.fillRect(47, 1, 11, 11, ILI9341_WHITE);
        if(dmxFehler) tft.fillRect(47, 1, 11, 11, ILI9341_RED);
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
    void HfbM::switchLock()
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

    /// @brief Rekursive Funktion um alle Icons horizontal NEbeneiner zu zeichnen
    /// @param posX 
    /// @param posY 
    /// @param index 0=Bubbles, 1=Rotation, 2=Nebel, 3= Helium, 4=HeliumVentil, 5=Fan, 6= EE
    void HfbM::drawFuncIcons(int posX, int posY, int index)
    {
        static int i, x;
        tft.drawBitmap(posX+x, posY, bitmap_allArray[i], 48, 48, ILI9341_WHITE);
        i++;
        x = x + 50;
        if (i>=6) i = x = 0;
        else drawFuncIcons(posX, posY, index);
    }

    /// @brief Methode um den Bildschirm für manuellen Betrieb zu erzeugen 
    void HfbM::manualScreen()
    {
        
    }

    /// @brief Methode zeichnet Bitmap aus Array auf Bildschirm
    /// @param posX X-StartKoordinate für bitmap
    /// @param posY Y-StartKoordinate für bitmap
    /// @param index 0=Bubbles, 1=Rotation, 2=Nebel, 3= Helium, 4=HeliumVentil, 5=Fan, 6= EE
    void HfbM::drawIcon(int posX, int posY, int index)
    {
        tft.drawBitmap(posX, posY, bitmap_allArray[index], 48, 48, ILI9341_WHITE);
    }

    /// @brief Zeichnet das EE Logo an festgestzten position
    void HfbM::drawEeIcon()
    {
         tft.drawBitmap(45, 0, bitmap_allArray[6], 225, 225, ILI9341_WHITE);
    }
    
    /// @brief Funktion zeichnet vertikalen "Slider" für manuellen betrieb
    /// @param posX startposition x oben links
    /// @param posY startposition y oben links
    /// @param value Höhe = Wert der Sliders
    void HfbM::drawBarchart(int posX, int posY, int value)
    {
        static int altValue;
        value = value /2;
        tft.drawRect(posX, posY+4, 40, -135, ILI9341_WHITE);
        if(altValue>value)tft.fillRect(posX+4, posY+altValue, 32, altValue-value, ILI9341_BLACK);
        tft.fillRect(posX+4, posY, 32, -value, ILI9341_WHITE);
        altValue = value;
    }
    
    /// @brief Simuliert einen horizontalen Ladebalken, benutzt delay()!! Sollte daher nur im Setup vewendet werden
    /// @param pHeigth Höhe der Loadingbar inkl Rahmen
    /// @param pWidth Breite der Loadingbar inkl Rahmen
    /// @param posx X-Koordinaze erster Pixel
    /// @param posy Y-Koordinate erter Pixel
    /// @param color Farbe derLoadigbar
    void HfbM::drawBarLines(int pHeigth, int pWidth, int posx, int posy, int color)
    {
        tft.drawRect(posx-4,posy-4, pWidth+8,pHeigth+8,color);
        for(int j = 0; j< pWidth; j++)
        {
            for(int i = 0; i< pHeigth; i++)
            {
            tft.drawLine(posx, posy+i,posx, posy+i, color);
            }
            posx++;
        delay(500/j);
        }
    }

    /// @brief Methode für Setup, zeichnet EE Logo und Ladebalken
    void HfbM::bootScreen()
    {
    tft.fillScreen(ILI9341_BLACK);
    tft.drawBitmap(45, 0, bitmap_allArray[6], 225, 225, ILI9341_WHITE);
    drawBarLines(15, 240, 40, 220, ILI9341_WHITE);
    delay(1000);
    tft.fillScreen(ILI9341_BLACK);
    }