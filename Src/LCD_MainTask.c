#include "GUI.h"
#include "DIALOG.h"

#define X_GAP 5

void LCD_MainTask(void) {
	GUI_Clear();
  GUI_SetFont(&GUI_Font24_1);
  GUI_DispStringAt("Stacja pogodowa", LCD_GetXSize()/2 - 80 , 0);
	GUI_DispStringAt("Bluetooth", LCD_GetXSize()/2 - 50 , 170);
	GUI_SetFont(&GUI_Font20_1);
	GUI_DispStringAt("Data:", X_GAP , 30);
	GUI_DispStringAt("Godzina:", X_GAP , 50);
  GUI_DispStringAt("Temperatura:", X_GAP , 70);
	GUI_DispStringAt("Cisnienie:", X_GAP , 90);
	GUI_DispStringAt("Wilgotnosc:", X_GAP , 110);
	GUI_DispStringAt("Stezenie C0  :", X_GAP , 130);
	GUI_DispStringAt("2", 102 , 135);
	GUI_DispStringAt("Prognoza:", X_GAP , 200);
	GUI_DispStringAt("Stan Baterii:", X_GAP , 240);
	GUI_DispStringAt("Zasieg:", X_GAP , 260);
}
