

////////////////////////////////////////////////////////////////////////////////////////////////////

#define _SegTecla 4 // 4 seg
int8 C_I[_Max_I] = {};

// funcion para deteccion de flanco
int1 Teclas_pulsada(int8 id)
{
   return (M_I[id] && !M_I1[id]);
}

void logicaIni() //////////////////////////////////////////////////////////////////////////////////

{
int i;

for ( i = 0; i <= _Max_I; ++i)C_I[i] = 0;
for ( i = 0; i < _Max_O; ++i )M_O[0] =0; 
for ( i = 0; i <= _Max_I; ++i)Filtro_I[i]=0;
      
SetTimer(0, 1);

}

void EjecutaLogica() //////////////////////////////////////////////////////////////////////////////////

{
   ///     funcion de logica de luces
   // listado de comandos
   /// M_O[17] comando encendido crepuscular
   // M_O[18] comando apagado crepuscular
   // M_O[19] comando apagado total
   // M_O[20] comando luz temporal de alero patio
   // M_O[21] Luz temporal escalera

     if (TimerOn(0) )
     {
          M_O[0] = !M_O[0];
          SetTimer(0, 3); // 3 seg// cada medio segundo
     }
/*

   int i;
   ///////////////////////////////////////////////////////proceso control de luces sala 
   if (Teclas_pulsada(ID_I_SS1)) /// /////////////////sala 
   {
      int8 estado = C_I[ID_I_SS1];

      if (timer_on(ID_I_SS1) && estado > 0 || estado == 2)
         estado = 0;
      else
         estado++;
      set_timer(ID_I_SS1, _SegTecla); // 10 seg// cada medio segundo
      switch (estado)
      {
      case 0:
         M_O[ID_O_L_SS1] = 0;
         M_O[ID_O_L_SS2] = 0;
         break;
      case 1:
         M_O[ID_O_L_SS1] = 1;
         M_O[ID_O_L_SS2] = 0;
         break;
      case 2:
         M_O[ID_O_L_SS1] = 1;
         M_O[ID_O_L_SS2] = 1;
         break;
      default:
         estado = 0;
         break;
      }

      C_I[ID_I_SS1] = estado;
   }

   /////////////////////////////proceso pieza principal

   if (Teclas_pulsada(ID_I_PN1))
   {
      int8 estado = C_I[ID_I_PN1];
      if (timer_on(ID_I_PN1) && estado > 0 || estado == 2)
         estado = 0;
      else
         estado++;
      set_timer(ID_I_PN1, _SegTecla); // 10 seg// cada medio segundo

      switch (estado)
      {
      case 0:
         M_O[ID_O_L_PN1] = 0;
         M_O[ID_O_L_PN2] = 0;
         break;
      case 1:
         M_O[ID_O_L_PN1] = 1;
         M_O[ID_O_L_PN2] = 0;
         break;
      case 2:
         M_O[ID_O_L_PN1] = 1;
         M_O[ID_O_L_PN2] = 1;
         break;
      default:
         estado = 0;
         break;
      }
      C_I[ID_I_PN1] = estado;
   }

   //////////////////////////////////////proceso apagado de luz pasillo placar

   if (Teclas_pulsada(ID_I_PN2))
      M_O[ID_O_L_PN2] = !M_O[ID_O_L_PN2];

   ////////////////////////////////////////proceso pieza agu

   if (Teclas_pulsada(ID_I_PA))
      M_O[ID_O_L_PA] = !M_O[ID_O_L_PA];

   ////////////////////////////////////////proceso luz pasillo

   if (Teclas_pulsada(ID_I_PAS))
      M_O[ID_O_L_PAS] = !M_O[ID_O_L_PAS];

   ////////////////////////////////////////////////////pieza RO

   if (Teclas_pulsada(ID_I_PR))
      M_O[ID_O_L_PR] = !M_O[ID_O_L_PR];

   /////////////////////////lus escalera  I_ES
   // M_O[20]=1;

   if (Teclas_pulsada(ID_I_ES))
      M_O[ID_O_L_ES] = !M_O[ID_O_L_ES];

   if (M_O[ID_O_L_ES] && !timer_on(ID_I_ES)) //|| M_O[20] == 1)
   {
      M_O[20] = 0;
      set_timer(ID_I_ES, 60);
   } // 150 seg// cada medio segundo
   if (timer_on(ID_I_ES))
      M_O[ID_O_L_ES] = 0;

   ////////////////////////////////////////luz frente
   /////////////////////////////proceso bano pieza principal

   if (Teclas_pulsada(ID_I_PN34)) /// /////////////////
   {
      int8 estado = C_I[ID_I_PN34];

      if (timer_on(ID_I_PN34) && estado > 0 || estado == 2)
         estado = 0;
      else
         estado++;
      set_timer(ID_I_PN34, _SegTecla); // 10 seg// cada medio segundo

      switch (estado)
      {
      case 0:
         M_O[ID_O_L_BN1] = 0;
         M_O[ID_O_L_BN2] = 0;

         break;
      case 1:
         M_O[ID_O_L_BN1] = 1;
         M_O[ID_O_L_BN2] = 0;

         break;
      case 2:
         M_O[ID_O_L_BN1] = 1;
         M_O[ID_O_L_BN2] = 1;

         break;
      default:
         estado = 0;
         break;
      }
      C_I[ID_I_PN34] = estado;
   }

   ///////////////////////////////////bano chicos////////////

   if (Teclas_pulsada(ID_I_BC1)) /// /////////////////

   {
      int8 estado = C_I[ID_I_BC1];
      if (timer_on(ID_I_BC1) && estado > 0 || estado == 2)
         estado = 0;
      else
         estado++;

      set_timer(ID_I_BC1, _SegTecla); // 10 seg// cada medio segundo

      switch (estado)
      {
      case 0:
         M_O[ID_O_L_BC1] = 0;
         M_O[ID_O_L_BC2] = 0;

         break;
      case 1:
         M_O[ID_O_L_BC1] = 1;
         M_O[ID_O_L_BC2] = 0;

         break;
      case 2:
         M_O[ID_O_L_BC1] = 1;
         M_O[ID_O_L_BC2] = 1;

         break;
      default:
         estado = 0;
         break;
      }
      C_I[ID_I_BC1] = estado;
   }

   ///////////////////////////////////////////////luz lava

   if (Teclas_pulsada(ID_I_BC2))
      M_O[ID_O_L_BC2] = !M_O[ID_O_L_BC2];

   ///////////////////////////////////////////////

   if (M_I[ID_I_LC] == 1 && M_I1[ID_I_LC] == 0 || M_O[16] == 1) /// se activa luz crepuscular
   {

      M_O[16] = 0;
      M_O[ID_O_L_BC1] = 1;
      M_O[ID_O_L_BC2] = 0;
   }

   if ((M_I[ID_I_LC] == 0 && M_I1[ID_I_LC] == 1) || M_O[17] == 1) /// se apaga luz crepuscular
   {
      M_O[17] = 0;
      for (i = 0; i < 16; i++)
         M_O[i] = 0;
   }

   /////////////////////////////////////////////////////////comando alero

   if (Teclas_pulsada(ID_I_SS2)) /// alero patio
      M_O[ID_O_L_PATI] = !M_O[ID_O_L_PATI];

   if (M_O[19] == 1) /////////////////// comando temporizado alero patio
   {
      M_O[19] = 0;

      set_timer(ID_I_SS2, 120); // 120 seg// cada medio segundo
      M_O[ID_O_L_PATI] = 1;
   }

   if (timer_on(ID_I_SS2))
      M_O[ID_O_L_PATI] = 0;

   //////////////////////////////////////////////////////////

   static int8 m;
   if (M_I[ID_I_SS1] == 0)
      m = 0;

   if (M_I[ID_I_SS1] == 1 && M_I[ID_I_SS2] == 1 && m == 0)
   {
      set_timer(ID_I_SS2, 2);
      m = 1;
   }

   if ((M_I[ID_I_SS1] == 1 && M_I[ID_I_SS2] == 1 && m == 1 && timer_on(ID_I_SS2)) || M_O[18] == 1) ///////////////////////////////////// comando apagado total
   {
      m = 0;
      M_O[18] = 0;
      for (i = 0; i < _Max_O; ++i)
         M_O[i] = 0;
      for (i = 0; i < _Max_I; ++i)
         C_I[i] = 0;
   }
   */
}
