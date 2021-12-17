#include <proc/pic18f4520.h>
#include "config.h"
#include "ssd.h"
#include "io.h"
#include "so.h"
#include "lcd.h"
#include "keypad.h"
#include "timer.h"

 
static const char valor[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};//valores em hexa
static char display = 0;
static char v0, v1, v2;
unsigned int Teclado = 0;


void cronometro(int contador){
    float tempo;
    int cont;
    
    for (cont = contador; cont >= 0; cont--) {
            v0 = ((cont / 100) % 10);//(0.01)
            v1 = ((cont / 1000) % 10);//(0.10))
            v2 = ((cont / 10000) % 10);//(1.00))
           
            digitalWrite(DISP_1_PIN, LOW);
            digitalWrite(DISP_2_PIN, LOW);
            digitalWrite(DISP_3_PIN, LOW);
            digitalWrite(DISP_4_PIN, LOW);
            
            soWrite(0x00);

            switch (display) {
                case 0:
                    soWrite(valor[v2]);
                    digitalWrite(DISP_2_PIN, HIGH);
                    display = 1;
                    break;
                case 1:
                    soWrite(valor[v1]);
                    digitalWrite(DISP_3_PIN, HIGH);
                    display = 2;
                    break;
                case 2:
                    soWrite(valor[v0]);
                    digitalWrite(DISP_4_PIN, HIGH);
                    display = 3;
                    break;
                default:
                    display = 0;
                    break;
            }
            for (tempo = 0; tempo < 50; tempo++);
        }
}

void main(void) { 
    lcdInit();
    kpInit();
    ssdInit();
    
    unsigned char tecla = 0;
    for (;;) {
        PORTD = 0b00000010; 
        lcdCommand(0x80);
        lcdString("BANHO ECONOMICO");
        lcdCommand(0xC0);
        lcdString(" S PARA COMECAR");
        kpDebounce();
        if (kpReadKey() != tecla) {
            tecla = kpReadKey();
            switch(tecla){
                case 'S':
                    tecla = 0;
                    lcdCommand(0x80);
                    lcdString("BANHO INICIADO");
                    lcdCommand(0xC0);
                    lcdString("TEMPO RESTANTE =");
                    cronometro(6100);
                    lcdCommand(0x80);
                    lcdString("BANHO FINALIZADO");
                    lcdCommand(0xC0);
                    lcdString("AGUARDE 1 MINUTO");
                    cronometro(6100);
                    lcdCommand(0x01);
                    break;
                default:
                    break;
            }
        }
    }
}