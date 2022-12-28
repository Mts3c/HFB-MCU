/*
C++ Header Datei einer Klasse für eine DMX gesteuerte Heiulm Nebel Seifenblasenmaschine
Author: Mathis Rathjen 
geschrieben am: 23.12.22
Zuletzt geaändert am:
*/

#pragma once
#include "TeensyDMX.h"
#include <Arduino.h>
#include <cstring>
#include <HardwareSerial.h> 
#include "ILI9341_t3n.h"

//Namespace define
namespace teensydmx = ::qindesign::teensydmx;

//Hardwarepins
#define HVpin 7     //Helium Ventil
#define HFpin 5     //Heliumdurchflussregler
#define FOGpin 14   //PowerTiny
#define PWMpin 6    //Pwm Controlled Fan
#define WHEELpin 15 //Motor BubbleWheel

//Displaypins
#define TFT_CS   10  // Chip-Select-Pin des Displays
#define TFT_DC   9   // Data/Command-Pin des Displays
#define TFT_RST  8   // Reset-Pin des Displays
#define TFT_MOSI 11  // MOSI-Pin des Displays (für den Datentransfer von der MCU zum Display)
#define TFT_CLK  13  // CLK-Pin des Displays (für den Datentransfer von der MCU zum Display)
#define TFT_MISO 12  // MISO-Pin des Displays (für den Datentransfer von der MCU zum Display)

//Serielle Schnitstelle
#define serialPort Serial1

//Indexvariabel für DMX Buffer Array, muss den abzuhörenden DMX Kanälen entsprechen 
//2Do: ggfs dyn Array
const int DMXCH = 512;

class HfbM
{
 private:
    //Memberobjekt von Klasse teensydmx
     teensydmx::Receiver dmxRx;
    //Memberobjekt von Klasse ILI9341_t3n
     ILI9341_t3n tft;

    //DMX-Kanalvariabeln
    int chHV;
    int chHF;
    int chFOG;
    int chFAN;
    int chWHEEL;
   
    //Generelle Variabeln
    std::string firmwareNum;
    std::string typ;
    std::string name;
    std::string dmxProfil;
    unsigned int kanalAnz;
    unsigned int startKanal;
    bool istAktiv;
    bool dmxAktiv;

    //Variabeln/ Parameter der einzel Module
    bool heliumVentil;
    double heliumFluss;
    int radRPM;
    int fanPWMDuty;
    int nebelWert;

    //2DO: Gggfs dynanmisches Array
    //8-Bit integer Array, enthälte aktuelle DMXWerte
    u_int8_t dmxValues[DMXCH]{0};
    // Buffer in which to store packet data (Example 2)
    u_int8_t packetBuf[DMXCH]{0};

    //FehlerFlags
    bool noFault = true;
    bool noDmxFault = true;

 public:
 //Defaultkonstruktor
    HfbM() : dmxRx(serialPort),  tft(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_CLK, TFT_MISO)
    {
        //Serial debug init
        firmwareNum = "V0.1 PTyp1";
        name = "HFB-Maschine";
        typ = "Prototyp 1";
        dmxProfil ="6CH manual";
        kanalAnz = 6;
        startKanal = 1;
        istAktiv = true;
        dmxAktiv = true;

        chHV = 1;
        chHF = 2;
        chFOG = 3;
        chFAN = 4;
        chWHEEL = 5;

        if(dmxValues[chHV-1] >= 225) heliumVentil = true;
        else if(dmxValues[chHV-1] < 225) heliumVentil = false;
        heliumFluss = dmxValues[chHF-1];
        radRPM = dmxValues[chWHEEL-1];
        fanPWMDuty = dmxValues[chFAN-1];
        nebelWert = dmxValues[chFOG-1];
    };
 
 //Destruktor
    ~HfbM() = default;



 //Methoden
    //Methode für SetupFunktion.
    void hwSetup();
    //Methode holt aktuellen DMX Frame, falls dieser vom vorherigen abweicht wird der Buffer in dmxValues kopiert
    void getDmxFrame();
    //Methode um Parameter der komponenten aus DMXV Array zu bestimmen
    void getParams();
    //Methode um alle aktuellen Parameter auf die entsprechenden Module anzuwenden
    void updateDevice();
    //Methode um DMXKanäle des Geräts, inkluse des Startkanals zuzuweisen
    void dmxSetup(int chHv, int chHf, int chFog, int chFan, int chWheel, int ch);
    //Methode um DMXKanäle des Geräts, ohne Startkanal, einzustellen
    void dmxSetup(int chHv, int chHf, int chFog, int chFan, int chWheel);
    //Methode um Startkanal einzustellen
    void setFirstDmxCh(int ch);
    //Schreibt Hello World auf Display
    void displayTest();
};