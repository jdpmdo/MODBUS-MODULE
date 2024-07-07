


#define ID1 PIN_A0  //
#define ID2 PIN_A1  //
#define ID3 PIN_A2  //
#define ID4 PIN_A3  //
#define ID5 PIN_A4  //
#define ID6 PIN_A5  //
#define ID7 PIN_E0  //
#define ID8 PIN_E1  //
#define ID9 PIN_B0  //
#define ID10 PIN_B1 //
#define ID11 PIN_B2 //
#define ID12 PIN_B3 //
#define ID13 PIN_B4 //
#define ID14 PIN_B5 //
#define ID15 PIN_B6 //
#define ID16 PIN_B7 //

/*
#define ID_I_PN1 0  // Tecla Pieza 1
#define ID_I_PN2 1  // Tecla Pieza 2
#define ID_I_PN34 2 // Tacla bano pieza principal
#define ID_I_PR 3   // Tecla Pieza Rocio
#define ID_I_PA 4   // Tecla Pieza Agu
#define ID_I_BC1 5  // Tecla bano central  1
#define ID_I_BC2 6  // Tecla bano central  2
#define ID_I_SS1 7  // Tecla sala 1
#define ID_I_SS2 8  // Tecla sala 2
#define ID_I_ES 9   // Tecla escalera
#define ID_I_PAS 10 // Tecla pasillo bano
#define ID_I_LC 11  // sensor crepuscular
#define ID_I_SA 12  // resumen sensores alarma /// prueba
#define ID_I_13 13
#define ID_I_14 14
#define ID_I_15 15
*/
//////////////////////////////////////////////////////////////////////

/// teclas ingreso

#define OD1 PIN_D0  //
#define OD2 PIN_D1  //
#define OD3 PIN_D2  //
#define OD4 PIN_D3  //
#define OD5 PIN_D4  //
#define OD6 PIN_D5  //
#define OD7 PIN_D6  //
#define OD8 PIN_D7  //
#define OD9 PIN_C0  //
#define OD10 PIN_C1 //
#define OD11 PIN_C2 //
#define OD12 PIN_C3 //
#define OD13 PIN_C4 //
#define OD14 PIN_C5 //

/*
#define O_L_PAS PIN_D0  // Luz pasillo
#define O_L_SS1 PIN_D1  // Luz sala 1
#define O_L_SS2 PIN_D2  // Luz sala 2
#define O_L_PN1 PIN_D3  // Luz Pieza principal centro
#define O_L_PN2 PIN_D4  // Luz Pieza principal placar
#define O_L_PR PIN_D5   // Luz Pieza Rocio
#define O_L_PA PIN_D6   // Luz Pieza Agu
#define O_L_BC2 PIN_D7  // Luz bano 1
#define O_L_BC1 PIN_C0  // Luz bano 2
#define O_L_BN1 PIN_C1  // Luz bano pieza principal 1
#define O_L_BN2 PIN_C2  // Luz bano pieza principal 2
#define O_L_ES PIN_C3   // Luz escalera
#define O_L_FR PIN_C4   // Luz frente casa
#define O_L_PATI PIN_C5 // Luz alero patio

#define ID_O_L_PAS 0   // Luz pasillo
#define ID_O_L_SS1 1   // Luz sala 1
#define ID_O_L_SS2 2   // Luz sala 2
#define ID_O_L_PN1 3   // Luz Pieza principal centro
#define ID_O_L_PN2 4   // Luz Pieza principal placar
#define ID_O_L_PR 5    // Luz Pieza Rocio
#define ID_O_L_PA 6    // Luz Pieza Agu
#define ID_O_L_BC2 7   // Luz bano 1
#define ID_O_L_BC1 8   // Luz bano 2
#define ID_O_L_BN1 9   // Luz bano pieza principal 1
#define ID_O_L_BN2 10  // Luz bano pieza principal 2
#define ID_O_L_ES 11   // Luz escalera
#define ID_O_L_FR 12   // Luz frente casa
#define ID_O_L_PATI 13 // Luz alero patio
*/


///////// ///////////////////////////////////////////////////////////////////////////filtro de entradas al 80%
int16 Filtro_I[_Max_I] = {};


void IO_ini()
{
   output_bit(OD1, 0);
   output_bit(OD2, 0);  //
   output_bit(OD3, 0);  //
   output_bit(OD4, 0);  //
   output_bit(OD5, 0);  //
   output_bit(OD6, 0);   //
   output_bit(OD7, 0);   //
   output_bit(OD8, 0);  //
   output_bit(OD9, 0);  //
   output_bit(OD10, 0);  //
   output_bit(OD11, 0);  //
   output_bit(OD12, 0);   //
   output_bit(OD13, 0);   //
   output_bit(OD14, 0); //
}


void IOsLectura()
{
static int8 scan = 0;

      if (input(ID1))         ++Filtro_I[0];
      if (input(ID2))         ++Filtro_I[1];
      if (input(ID3))         ++Filtro_I[2];
      if (input(ID4))         ++Filtro_I[3];
      if (input(ID5))         ++Filtro_I[4];
      if (input(ID6))         ++Filtro_I[5];
      if (input(ID7))         ++Filtro_I[6];
      if (input(ID8))         ++Filtro_I[7];
      if (input(ID9))         ++Filtro_I[8];
      if (input(ID10))        ++Filtro_I[9];
      if (input(ID11))        ++Filtro_I[10];
      if (input(ID12))        ++Filtro_I[11];
      if (input(ID13))        ++Filtro_I[12];
      if (input(ID14))        ++Filtro_I[13];
      if (input(ID15))        ++Filtro_I[14];
      if (input(ID16))        ++Filtro_I[15];

      scan++;

      if (scan > 40)
      {
         //// cantidad de lectura en alto para considerar - mayor de 80 de 10 lecturas
         int i;
         for ( i = 0; i < _Max_I; ++i)
         {
            if (Filtro_I[i] > (40 * 0.80))
               M_I[i] = 0;
            else
               M_I[i] = 1; // entrada invertida
               
            Filtro_I[i] = 0;
         }
         
         scan = 0;
      }
      
      
 ////////////////////////////////////////// memorizo estados anteriores de las DIs
         int i;
         for ( i = 0; i < _Max_I; ++i)
         M_I1[i] = M_I[i];      
      
      
      
}


void IOs_escritura()
{
 // rutina de actualizacion de salidas 
  
         output_bit(OD1, M_O[0]);
         output_bit(OD2, M_O[1]);
         output_bit(OD3, M_O[2]);
         output_bit(OD4, M_O[3]);
         output_bit(OD5, M_O[4]);
         output_bit(OD6, M_O[5]);
         output_bit(OD7, M_O[6]);
         output_bit(OD8, M_O[7]);
         output_bit(OD9, M_O[8]);
         output_bit(OD10, M_O[9]);
         output_bit(OD11, M_O[10]);
         output_bit(OD12, M_O[11]);
         output_bit(OD13, M_O[12]);
         output_bit(OD14, M_O[13]);
     
}
