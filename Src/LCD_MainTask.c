#include "GUI.h"
#include "DIALOG.h"
#include "main_sys.h"
#include "stdio.h"

#define X_GAP 5				// odleglosc od lewej krawedzi ekranu etykiet
#define X_GAP1 130		// odleglosc od lewej krawedzi ekranu pomiarow
#define TMP_SIZE 14		// rozmiar stringu przechowujacego pomiary
#define CLRSTR(p) (snprintf((p), TMP_SIZE, ""))

static char tmp[TMP_SIZE];

void LCD_StaticText(void) {
	
	GUI_Clear();
	
	GUI_SetFont(&GUI_Font24_1);
	
	// headers
  GUI_DispStringAt("Stacja pogodowa", LCD_GetXSize()/2 - 80 , 0);
	GUI_DispStringAt("Bluetooth", LCD_GetXSize()/2 - 50 , 170);
	
	GUI_SetFont(&GUI_Font20_1);
	
	GUI_DispStringAt("Data:", X_GAP , 30);
	GUI_DispStringAt("Godzina:", X_GAP , 50);
	GUI_DispStringAt("Temperatura:", X_GAP , 70);
	GUI_DispStringAt("Cisnienie:", X_GAP , 90);
	GUI_DispStringAt("Wilgotnosc:", X_GAP , 110);
	GUI_DispStringAt("Stezenie C02:", X_GAP , 130);
	GUI_DispStringAt("Pogoda:", X_GAP , 200);
	GUI_DispStringAt("Pr. wiatru:", X_GAP , 220);
	GUI_DispStringAt("Miejscowosc:", X_GAP , 240);
}	

void LCD_MainTask(const BL_Data_TypeDef* const data) {
	

	GUI_SetFont(&GUI_Font20_1);
	
	// date
	snprintf(tmp, TMP_SIZE, "%s   ", data->remote_data.date);
	GUI_DispStringAt(tmp, X_GAP1 , 30);
	
	// time
	CLRSTR(tmp);
	snprintf(tmp, TMP_SIZE, "%s   ", data->remote_data.time);
	GUI_DispStringAt(tmp, X_GAP1 , 50);
	
	// temperature
	CLRSTR(tmp);
		snprintf(tmp, TMP_SIZE, "%u.%u C   ", data->local_data.temp_tot, data->local_data.temp_frac);
  GUI_DispStringAt(tmp, X_GAP1 , 70);
	
	// pressure
	CLRSTR(tmp);
	snprintf(tmp, TMP_SIZE, "%u HPa   ", data->local_data.press_sea);
	GUI_DispStringAt(tmp, X_GAP1 , 90);
	
	// humidity
	CLRSTR(tmp);
	snprintf(tmp, TMP_SIZE, "%u%%   ", data->local_data.humidity);
	GUI_DispStringAt(tmp, X_GAP1 , 110);
	
	// CO2
	CLRSTR(tmp);
	snprintf(tmp, TMP_SIZE, "%u%%   ", data->local_data.humidity);
	GUI_DispStringAt(tmp, X_GAP1 , 130);
}
