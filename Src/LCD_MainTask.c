#include "GUI.h"
#include "DIALOG.h"
#include "main_sys.h"
#include "stdio.h"

#define X_GAP 5
#define CLRSTR(p) (snprintf((p), 20, ""))
#define TMP_SIZE 22

static char tmp[TMP_SIZE];

void LCD_MainTask(const BL_Data_TypeDef* const data) {
	
//	GUI_Clear();
  GUI_SetFont(&GUI_Font24_1);
	
	// headers
  GUI_DispStringAt("Stacja pogodowa", LCD_GetXSize()/2 - 80 , 0);
	GUI_DispStringAt("Bluetooth", LCD_GetXSize()/2 - 50 , 170);
	GUI_SetFont(&GUI_Font20_1);
	
	// date
	snprintf(tmp, TMP_SIZE, "Data: %s    ", data->remote_data.date);
	GUI_DispStringAt(tmp, X_GAP , 30);
	
	// time
	CLRSTR(tmp);
	snprintf(tmp, TMP_SIZE, "Godzina: %s      ", data->remote_data.time);
	GUI_DispStringAt(tmp, X_GAP , 50);
	
	// temperature
	CLRSTR(tmp);
	snprintf(tmp, TMP_SIZE, "Temperatura: %2u.%1u C  ", data->local_data.temp_tot, data->local_data.temp_frac);
  GUI_DispStringAt(tmp, X_GAP , 70);
	
	// pressure
	CLRSTR(tmp);
	snprintf(tmp, TMP_SIZE, "Cisnienie: %4u HPa", data->local_data.press_sea);
	GUI_DispStringAt(tmp, X_GAP , 90);
	
	// humidity
	CLRSTR(tmp);
	snprintf(tmp, TMP_SIZE, "Wilgotnosc: %2u%%      ", data->local_data.humidity);
	GUI_DispStringAt(tmp, X_GAP , 110);
	
	// CO2
	CLRSTR(tmp);
	snprintf(tmp, TMP_SIZE, "Stezenie C0  : %2u%%   ", data->local_data.humidity);
	GUI_DispStringAt(tmp, X_GAP , 130);
	GUI_DispStringAt("2", 102 , 135);
	
	
	GUI_DispStringAt("Prognoza:", X_GAP , 200);
	GUI_DispStringAt("Stan Baterii:", X_GAP , 240);
	GUI_DispStringAt("Zasieg:", X_GAP , 260);
}
