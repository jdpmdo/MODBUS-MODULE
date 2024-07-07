

int16 hold_regs[_Max_HR] = {};
int1 flag_hold_regs[_Max_HR] = {}; /// se usa para espesificar si cambio algun HR que se debe procesar
int16 input_regs[_Max_IR] = {};
int16 event_count = 0;


void LeerMSGsModbus()

{

   if (modbus_kbhit())

   {

      ////////////////////////////////// actualizar registros
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      int8 i;

      // Inputs Registers 0 corresponden a 16 DI
      for (i = 0; i < _Max_I; i++)
         if (M_I[i])
            bit_set(input_regs[0], i);
         else
            bit_clear(input_regs[0], i);

      hold_regs[0] = input_regs[0];
      // Inputs Registers 1 corresponden a 14 DOs

      for (i = 0; i < 16; i++)
         if (M_O[i])
            bit_set(input_regs[1], i);
         else
            bit_clear(input_regs[1], i);

      // Lo mismo que los Holding registers
      hold_regs[1] = input_regs[1];

      for (i = 0; i < 16; i++)
         if (M_O[i + 16])
            bit_set(hold_regs[2], i);
         else
            bit_clear(hold_regs[2], i);

      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      // delay_us(10);

      // check address against our address, 0 is broadcast
      if ((modbus_rx.address == MODBUS_ADDRESS) || modbus_rx.address == 0)
      {
         switch (modbus_rx.func)
         {
         case FUNC_READ_COILS: // read coils

            if (modbus_rx.data[0] || modbus_rx.data[2] || modbus_rx.data[1] >= _Max_O_T || modbus_rx.data[3] + modbus_rx.data[1] > _Max_O_T)
               modbus_exception_rsp(MODBUS_ADDRESS, modbus_rx.func, ILLEGAL_DATA_ADDRESS);
            else
            {
               int8 data_[(_Max_O_T / 8) + 1];

               int8 ii = 0;
               int8 i;
               int8 j = 0;
               for (i = modbus_rx.data[1]; i < (modbus_rx.data[1] + modbus_rx.data[3]); i++)
               {
                  if (j == 8)
                  {
                     j = 0;
                     ii++;
                  }

                  if (M_O[i])
                     bit_set(data_[ii], j);
                  else
                     bit_clear(data_[ii], j);

                  j++;
               }

               modbus_read_discrete_input_rsp(MODBUS_ADDRESS, (ii + 1), &data_);

               event_count++;
            }
            break;

         case FUNC_READ_DISCRETE_INPUT: // read inputs////////////////////////////////////////////////////////////////////////////////////////////
            if (modbus_rx.data[0] || modbus_rx.data[2] || modbus_rx.data[1] >= _Max_I || modbus_rx.data[3] + modbus_rx.data[1] > _Max_I)
               modbus_exception_rsp(MODBUS_ADDRESS, modbus_rx.func, ILLEGAL_DATA_ADDRESS);
            else
            {
               int8 data_[(_Max_I / 8) + 1];

               int8 ii = 0;
               int8 i;
               int8 j = 0;
               for (i = modbus_rx.data[1]; i < (modbus_rx.data[1] + modbus_rx.data[3]); i++)
               {
                  if (j == 8)
                  {
                     j = 0;
                     ii++;
                  }

                  if (M_I[i])
                     bit_set(data_[ii], j);
                  else
                     bit_clear(data_[ii], j);

                  j++;
               }

               modbus_read_discrete_input_rsp(MODBUS_ADDRESS, (ii + 1), &data_);

               event_count++;
            }
            break;

         case FUNC_READ_HOLDING_REGISTERS: //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            if (modbus_rx.data[0] || modbus_rx.data[2] || modbus_rx.data[1] >= _Max_HR || modbus_rx.data[3] + modbus_rx.data[1] > _Max_HR)
               modbus_exception_rsp(MODBUS_ADDRESS, modbus_rx.func, ILLEGAL_DATA_ADDRESS);
            else
            {

               modbus_read_holding_registers_rsp(MODBUS_ADDRESS, (modbus_rx.data[3] * 2), hold_regs + modbus_rx.data[1]);
               event_count++;
            }

            break;

         case FUNC_READ_INPUT_REGISTERS: //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (modbus_rx.data[0] || modbus_rx.data[2] || modbus_rx.data[1] >= _Max_HR || modbus_rx.data[3] + modbus_rx.data[1] > _Max_IR)
               modbus_exception_rsp(MODBUS_ADDRESS, modbus_rx.func, ILLEGAL_DATA_ADDRESS);
            else
            {
               modbus_read_input_registers_rsp(MODBUS_ADDRESS, (modbus_rx.data[3] * 2), input_regs + modbus_rx.data[1]);
               event_count++;
            }
            break;

         case FUNC_WRITE_SINGLE_COIL: // write coil ////////////////////////////////////////////////////////////////////////////////////////
            if (modbus_rx.data[0] || modbus_rx.data[3] || modbus_rx.data[1] > _Max_O_T)

               modbus_exception_rsp(MODBUS_ADDRESS, modbus_rx.func, ILLEGAL_DATA_ADDRESS);

            else if (modbus_rx.data[2] != 0xFF && modbus_rx.data[2] != 0x00)

               modbus_exception_rsp(MODBUS_ADDRESS, modbus_rx.func, ILLEGAL_DATA_VALUE);

            else
            {
               if (modbus_rx.data[2] == 0xFF)
                  M_O[modbus_rx.data[1]] = 1;
               else
                  M_O[modbus_rx.data[1]] = 0;

               modbus_write_single_coil_rsp(MODBUS_ADDRESS, modbus_rx.data[1], ((int16)(modbus_rx.data[2])) << 8);

               event_count++;
            }
            break;

         case FUNC_WRITE_SINGLE_REGISTER: //////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (modbus_rx.data[0] || modbus_rx.data[1] >= _Max_HR)
               modbus_exception_rsp(MODBUS_ADDRESS, modbus_rx.func, ILLEGAL_DATA_ADDRESS);
            else
            {

               for (i = 0; i < _Max_HR; i++)
                  flag_hold_regs[i] = 0; /// dejo las banderas a 0

               hold_regs[modbus_rx.data[1]] = make16(modbus_rx.data[2], modbus_rx.data[3]);

               flag_hold_regs[modbus_rx.data[1]] = 1;

               modbus_write_single_register_rsp(MODBUS_ADDRESS,
                                                make16(modbus_rx.data[0], modbus_rx.data[1]),
                                                make16(modbus_rx.data[2], modbus_rx.data[3]));
            }
            break;

         case FUNC_WRITE_MULTIPLE_COILS: ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (modbus_rx.data[0] || modbus_rx.data[2] || modbus_rx.data[1] >= _Max_O_T || modbus_rx.data[3] + modbus_rx.data[1] > _Max_O_T)
               modbus_exception_rsp(MODBUS_ADDRESS, modbus_rx.func, ILLEGAL_DATA_ADDRESS);
            else
            {
               int8 i, j, ii;

               ii = 0;
               j = 0;
               for (i = modbus_rx.data[1]; i < (modbus_rx.data[1] + modbus_rx.data[3]); i++)
               {
                  if (j == 8)
                  {
                     j = 0;
                     ii++;
                  }

                  if (bit_test(modbus_rx.data[5 + ii], j))
                     M_O[i] = 1;
                  else
                     M_O[i] = 0;

                  j++;
               }

               modbus_write_multiple_coils_rsp(MODBUS_ADDRESS, make16(modbus_rx.data[0], modbus_rx.data[1]), make16(modbus_rx.data[2], modbus_rx.data[3]));

               event_count++;
            }
            break;

         case FUNC_WRITE_MULTIPLE_REGISTERS: //////////////////////////////////////////////////////////////////////////////////////////////
            if (modbus_rx.data[0] || modbus_rx.data[2] || modbus_rx.data[1] >= 8 || modbus_rx.data[3] + modbus_rx.data[1] > _Max_HR)
               modbus_exception_rsp(MODBUS_ADDRESS, modbus_rx.func, ILLEGAL_DATA_ADDRESS);
            else
            {
               int8 i, j;

               for (i = 0; i < _Max_HR; i++)
                  flag_hold_regs[i] = 0; /// dejo las banderas a 0

               /*
                for(i=0,j=5; i < modbus_rx.data[4]/2; ++i,j+=2)
                   {
                   hold_regs[i] = make16(modbus_rx.data[j],modbus_rx.data[j+1]);

                   flag_hold_regs[i] =1;


                   }
                */

               for (i = 0, j = 5; i < modbus_rx.data[4] / 2; ++i, j += 2) //
               {
                  hold_regs[modbus_rx.data[1] + i] = make16(modbus_rx.data[j], modbus_rx.data[j + 1]);

                  flag_hold_regs[modbus_rx.data[1] + i] = 1;
               }

               modbus_write_multiple_registers_rsp(MODBUS_ADDRESS,
                                                   make16(modbus_rx.data[0], modbus_rx.data[1]),
                                                   make16(modbus_rx.data[2], modbus_rx.data[3]));

               event_count++;
            }
            break;
         default: // We don't support the function, so return exception
            modbus_exception_rsp(MODBUS_ADDRESS, modbus_rx.func, ILLEGAL_FUNCTION);
         }
      }
   }
}


void CargarDOsDesdeModbus()
{

      if (flag_hold_regs[1])
      {

         int8 ii;
         for (ii = 0; ii < _Max_O; ii++)
            M_O[ii] = bit_test(hold_regs[1], ii);
         flag_hold_regs[1] = 0; /// dejo la banderas a 0
      }

      if (flag_hold_regs[2])
      {
         int8 ii;
         for (ii = 0; ii < _Max_OC; ii++)
            M_O[ii + 16] = bit_test(hold_regs[2], ii);
         flag_hold_regs[2] = 0; /// dejo la banderas a 0
      }
}
