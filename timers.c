/// Funciones simples para implementar un timer

struct timer___
{
   int1 on;
   int8 set;
   int8 t;
   int1 verifico;
} Timer_I[_Max_T];

void TimersIni(void)
{
   int8 i;
   for (i = 0; i < _Max_T; i++)
   {
      Timer_I[i].on = 0;
      Timer_I[i].t = 0;
      Timer_I[i].set = 0;
      Timer_I[i].verifico = 0;
   }
}

void UpdateTimers(void)
{
   int8 i;
   for (i = 0; i < _Max_T; i++)
   {
      if (!Timer_I[i].on) ++Timer_I[i].t;
      if (Timer_I[i].t >= Timer_I[i].set)
         Timer_I[i].on = 1;
      else
         Timer_I[i].on = 0;
   }
}

void SetTimer(int8 id, int8 set)
{
   Timer_I[id].set = set;
   Timer_I[id].on = 0;
   Timer_I[id].verifico = 0;
   Timer_I[id].t = 0;
}

int1 TimerOn(int8 id)
{
   if (Timer_I[id].on && !Timer_I[id].verifico)
   {
      Timer_I[id].verifico = 1;
      return 1;
   }
   else
      return 0;
}
