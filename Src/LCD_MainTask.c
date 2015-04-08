#include "GUI.h"
#include "DIALOG.h"

void LCD_MainTask(void) {
 // GUI_Clear();
  GUI_SetFont(&GUI_Font24_1);
  GUI_DispStringAt("Stacja pogodowa", LCD_GetXSize()/2 - 80 , 0);
	GUI_SetFont(&GUI_Font20_1);
  GUI_DispStringAt("Temperatura:", 0 , 30);
	GUI_DispStringAt("Cisnienie:", 0 , 50);
	GUI_DispStringAt("Wilgotnosc:", 0 , 70);
	GUI_DispStringAt("Stezenie C02:", 0 , 90);
	GUI_DispStringAt("Temperatura:", 0 , 110);
}

void LCD_PhoneTask(void) {
	GUI_SetFont(&GUI_Font24_1);
	GUI_DispStringAt("Bluetooth", LCD_GetXSize()/2 - 80 , 140);
	GUI_SetFont(&GUI_Font20_1);
	 GUI_DispStringAt("Prognoza:", 0 , 170);
	
}