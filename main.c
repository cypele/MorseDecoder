/*-------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - projekt
					Dekoder alfabetu Mors'a
					autor: Adam Cypliński
					wersja: 30.01.2023r.
----------------------------------------------------------------------------*/
					
#include "MKL05Z4.h"
#include "ADC.h"
#include "DAC.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DOT_DURATION 50
#define DASH_DURATION 150
#define INTER_WORD_GAP 300

//systick
uint32_t czas_ciemny=0;
uint32_t czas_jasny=0;				// Licznik czasu jasnego, zliczajacy sekundy od handlera
uint8_t sekunda=0;			// Licznik przerwan ( do 10)
uint8_t sekunda_OK=0;	


float adc_volt_coeff = ((float)(((float)2.91) / 4096) );			// Współczynnik korekcji wyniku, w stosunku do napięcia referencyjnego przetwornika
uint8_t wynik_ok=0;
uint16_t temp;
float wynik=0;
int i=0;
int k=0;
	
char znaki[50];
char slowo[20];



const char* morseCode[] = {
    ".-",   //A
    "-...", //B
    "-.-.", //C
    "-..",  //D
    ".",    //E
    "..-.", //F
    "--.",  //G
    "....", //H
    "..",   //I
    ".---", //J
    "-.-",  //K
    ".-..", //L
    "--",   //M
    "-.",   //N
    "---",  //O
    ".--.", //P
    "--.-", //Q
    ".-.",  //R
    "...",  //S
    "-",    //T
    "..-",  //U
    "...-", //V
    ".--",  //W
    "-..-", //X
    "-.--", //Y
    "--.."  //Z
};




void SysTick_Handler(void)	// Podprogram obslugi przerwania od SysTick'a
{ 
	sekunda+=1;	
	if(sekunda==10)
	{
		sekunda=0;
		sekunda_OK=1;		// Daj znac, ze minelo 100ms
		
	}
}



void ADC0_IRQHandler()
{	
	temp = ADC0->R[0];	// Odczyt danej i skasowanie flagi COCO

	if(!wynik_ok)					// Sprawdź, czy wynik skonsumowany przez petlę główną
	{

		wynik = temp;		// Wyślij wynik do pętli głównej
		wynik = wynik*adc_volt_coeff;
		wynik_ok=1;
	}
	
	DAC_Load_Trig(temp);	// Załadowanie nowej danej i wyzwolenie przetwornika C/A
	
}




int main (void)
{
	
	uint8_t	kal_error;

	
	char message[50];
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

	LCD1602_Init();		 // Inicjalizacja wyświetlacza LCD
	LCD1602_Backlight(TRUE);
	
	kal_error=ADC_Init();		// Inicjalizacja przetwornika A/C
	if(kal_error)
	{
		while(1);							// Klaibracja się nie powiodła
	}
	DAC_Init();		// Inicjalizacja przetwornika C/A
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(8);		// Pierwsze wyzwolenie przetwornika ADC0 w kanale 8 i odblokowanie przerwania
	
	
	SysTick_Config(SystemCoreClock/100 );
	NVIC_EnableIRQ(SysTick_IRQn);
	


	int word_size=0;
	int m;
	int n;
	int wyswietl_wiadomosc=0;
		

	LCD1602_SetCursor(1,0);
	LCD1602_Print("Dekoder Mors'a");
	LCD1602_SetCursor(1,1);
	LCD1602_Print("Adam Cyplinski");
	DELAY(2500)
	LCD1602_ClearAll();
		
	
	while(1)
	{
		if(sekunda_OK)
		{
			if(wynik_ok)
			{
			
				
			if (wynik>=0.8){
						czas_ciemny=0;
						czas_jasny+=1;
						LCD1602_SetCursor(0,1);
						sprintf(display,"%02d",czas_jasny);
						LCD1602_Print(display);
					}
			else if (wynik<0.8 && czas_jasny>0){

						czas_ciemny+=1;
						//LCD1602_SetCursor(2,1);
						//sprintf(display,"%02d",czas_ciemny);
						//LCD1602_Print(display);
						
						
						if(czas_jasny<=10 && czas_jasny>1){
							znaki[i]='.';
							i++;
						}
						else if(czas_jasny>10&&czas_jasny<20){
							znaki[i]='-';
							i++;
						}
						else if(czas_jasny>20){
							znaki[i]=' ';
							i++;						
						}

						LCD1602_SetCursor(5+i,1);
						sprintf(display,"%c",znaki[i-1]);
						LCD1602_Print(display);	
						czas_jasny=1;
					}
					
						
					if(czas_ciemny>10){
						char litera = 'A';
						for(int j=0; j<25; j++)
						{
							if(strcmp(znaki,morseCode[j])==0){
								litera=litera+j;
								slowo[k]=litera;
								k++;
								memset(znaki,0,50);
								break;
							}

							else if(strcmp(znaki," ")==0){
								memset(znaki,0,50);								
								slowo[k]=' ';
								
								for(m=word_size, n=0; m<word_size+k && n<k; m++, n++){
								message[m]=slowo[n];
								}
								word_size=word_size+k+1;
								memset(slowo,0,20);
								k=0;
								break;
							}
							else if(strcmp(znaki,".....")==0){
								wyswietl_wiadomosc=1;
								break;
							}
							else if(j==24){
								memset(znaki,0,50);
								LCD1602_SetCursor(0,1);
								LCD1602_Print(" Wrong letter!");
								DELAY(1000)
								LCD1602_ClearAll();
							}
						}
						i=0;
						czas_ciemny=0;
						czas_jasny=0;
						LCD1602_ClearAll();
						
						if(k>0){
							for(int x=0; x<k; x++){
							LCD1602_SetCursor(1+x,0);
							sprintf(display,"%c",slowo[x]);
							LCD1602_Print(display);		
							}
						}
						else{
							LCD1602_ClearAll();					
						}
							
					}
					sekunda_OK=0;
					wynik_ok=0;
					
				
			}
		}

		if(wyswietl_wiadomosc==1){
			for(int i=word_size; i<word_size+15; i++){
				message[i]=' ';
			}
			for(int a=0; a<word_size-1; a++){
				for(int b=0; b<word_size-1; b++){
						sprintf(display, "%c", message[b+a]);
						LCD1602_SetCursor(b,0);
						LCD1602_Print(display);
						}
					if(a==0)
						DELAY(1000)
					else
						DELAY(500)
					LCD1602_ClearAll();

					}
			wyswietl_wiadomosc=0;
			memset(slowo,0,20);
			memset(znaki,0,50);
			}

		}
		
		
		
		
	}

