/*

C++ Header Datei einer Klasse für eine DMX gesteuerte Heiulm Nebel Seifenblasenmaschine
Author: Mathis Rathjen 
geschrieben am: 23.12.22
Zuletzt geaändert am:

From Microscope over Macrosscope to Globalscope
*/

#include "HfbM.h"
#include <TaskManagerIO.h>

TaskManager tm;
HfbM maschine;


void setup() 
{
maschine.hwSetup();
maschine.displayTest();

}

void loop() 
{



}
