/*
Modulo modbus rtu 16 entradas 14 salidas
chip base 16f877a
JDP 
Funciona con registros modbus RTU y puede ejecutar una logica de usuario personalizada independiente en la rutina EjecutaLogica()
Timers definidas a tiempo base de contador 1 segundo
Compila en CCS PCW 4.114
/////////////////////////////////////////////Datos placa D16O14 planta alta - 16 ingresos - 14 salidas - 1 canal modbus 485
///////////////listado de ingresos digitales cableados


I1 
I2 
I3 
I4 
I5 
I6 
I7 
I8 
I9 
I10 
I11 
I12 
I13 
I14 
I15 
I16 


/////////////////////////////////////////////////////Listado de salidas

O1 
O2 
O3 
O4 
O5 
O6 
O7 
O8 
O9 
O10 
O11 
O12 
O13 
O14 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Listado Modbus /////////////////esclavo 20 --- Baud rate 19200 - Word length 8 - parity EVENT - Stop bit 1

holding register numero 1  -> ingresos digitales del 1 al 16 (igual que holding register numero 1 )
holding register numero 2  -> salidas digitales del 1 al 14 (15 y 16 sin efecto)
holding register numero 2 -->salidas digitales O17 a O32 -> Comandos auto reseteables


Coils del 1 al 14 -> latch salidas digitales O1 a O14 (15 y 16 sin efecto)

Coils del 17 al 32 -> latch salidas digitales O17 a O32 -> Comandos auto reseteables


         O17 comando encendido crepuscular
         O18 comando apagado crepuscular
         O19 comando apagado total
         O20 comando luz temporal de alero patio  120 s
         O21 Luz temporal escalera



Inputs del 1 al 16 -> entradas digitales I1 a I16

*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include <18F4550.h>
#include <16F877A.h>
// #include <18F452.h>

#FUSES NOWDT // Watch Dog Timer//NOWDT
#FUSES NOPUT // No Power Up Timer
#FUSES XT    // Crystal osc <= 4mhz for PCM/PCH , 3mhz to 10 mhz for PCD
// #FUSES NOMCLR                     //Master Clear pin enabled
#FUSES NOBROWNOUT // No brownout reset
#FUSES NOLVP      // No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOCPD      // No EE protection
#FUSES NOPROTECT

#USE FAST_IO(A)
#USE FAST_IO(B)
#USE FAST_IO(C)
#USE FAST_IO(D)

// Code not protected from reading
#use delay(clock = 4000000)

#define RS485_ID_MODULO 123
#define RS485_RX_BUFFER_SIZE 10

#define MODBUS_TYPE MODBUS_TYPE_SLAVE
#define MODBUS_SERIAL_TYPE MODBUS_RTU // use MODBUS_ASCII for ASCII mode
#define MODBUS_SERIAL_RX_BUFFER_SIZE 20
#define MODBUS_SERIAL_BAUD 19200
#define MODBUS_ADDRESS 20
#ifndef USE_WITH_PC
#define MODBUS_SERIAL_INT_SOURCE MODBUS_INT_RDA // MODBUS_INT_EXT

#define MODBUS_SERIAL_TX_PIN PIN_C6 // Data transmit pin
#define MODBUS_SERIAL_RX_PIN PIN_C7 // Data receive pin
// The following should be defined for RS485 communication
#define MODBUS_SERIAL_ENABLE_PIN PIN_E2 // Controls DE pin for RS485  -> pin de control RX/TX
#define MODBUS_SERIAL_RX_ENABLE 0       // Controls RE pin for RS485
#endif

#define _Max_O 16                   /// cantidad de matriz de salidas digitales Coils
#define _Max_OC 16                  /// cantidad de  partir de la posicion _Max_O son salidas de comandos
#define _Max_O_T (_Max_O + _Max_OC) /// cantidad de total de coils
#define _Max_I 16                   /// cantidad de matriz de ingresos digitales Coils
#define _Max_HR 3                   /// cantidad de holding registers
#define _Max_IR 2                   /// cantidad de inputs registers
#define _Max_T 16                   /// cantidad de Timers
////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int1 M_O[_Max_O + _Max_OC] = {}; /// salidas digitales en memoria
int1 M_I[_Max_I] = {};/// Ingresos digitales en memoria
int1 M_I1[_Max_I] = {};/// Ingresos digitales en memoria en scan-1 - se momoriza el estado anterior para aplicar posible logica de cambio de ingreso



////////////////////////////////////////////////////////
#include <modbus.c> //-> (C) Copyright 1996, 2010 Custom Computer Services 
#include <timers.c>
#include <IOs.c>
#include <logica.c>
#include <modbus_functions.c>


void main(void)
{
   //setup_wdt(WDT_1152MS);      //1152mSec nominal

   
   SET_TRIS_A(0b00111111);
   SET_TRIS_E(0b00000011);
   SET_TRIS_B(0b11111111);

   SET_TRIS_D(0b00000000); // 0 salida
   SET_TRIS_C(0b10000000);

   // Establecer el preescaler del Timer1 a 1:8 (para lograr una precisiï¿½n de 1 ï¿½s)

   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);

   clear_interrupt(INT_TIMER1); // Clear Timer2 interrupt flag bit
   set_timer1(59286);           ///(65535-100ms/(1/(4m/8/4))  -> cada 50ms
   enable_interrupts(INT_TIMER1); // Enable Timer2 interrupt
   enable_interrupts(GLOBAL);

   TimersIni();
   IO_ini();

/////////////// juego de led para señal de inicio
   int i = 0;
   for (i = 0; i <= 25; ++i)
   {
      output_toggle(MODBUS_SERIAL_TX_PIN);
      delay_ms(100);
   }
   
   logicaIni();
   modbus_init();
   
   ///////////////////////////bucle principal
   while (true)
   {
      //restart_wdt();
      LeerMSGsModbus();
      ////////////////actualizo salidas si piso el reg 1
      CargarDOsDesdeModbus();
      //////////////////////////////////////
      IOsLectura();
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////
      EjecutaLogica();
      //////////////////////////////////////////////////
      IOs_escritura();
   }
}

#INT_TIMER1  // interrupcion cada 50ms
void timer1_isr(void)
{
   static int8 SegDiv20=0;
   
   clear_interrupt(INT_TIMER1);
   set_timer1(59286); /// 50 ms
   
   SegDiv20++;
   if (segDiv20 >= 20) // 20*50ms -> 1 segundo
   {
      UpdateTimers(); // actualizo todos los timers
      SegDiv20 = 0;
   }
     
}
