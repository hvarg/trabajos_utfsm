/**********************************************************
* Variables:                                              *
* 	xi	= Tiempo gastado hasta el nodo i.                 *
*	Yj	= Días acelerables de la tarea j.                 *
*   i entre 0 y 11.                                       *
*	j entre a y o.                                        *
* La naturaleza de variables es implícita ya que se le da *
* un valor exacto a x11. Explícitamente serian: xi >= 0   *
* y los Yj >= 0.                                          *
**********************************************************/

/* Minimización sobre las tareas que se pueden acelerar. */
min: 12000 Yc + 11000 Yd + 8000 Yh + 5000 Yi + 4000 Yj + 
	 9000 Yk + 7000 Yl + 8000 Ym + 7000 Yn + 9000 Yo;

/* Cantidad de días máximos acelerables por tarea. */
Ya <= 0;	Yb <= 0;	Yc <= 2;	Yd <= 1;	Yf <= 0;
Yg <= 0;	Yh <= 1;	Yi <= 1;	Yj <= 1;	Yk <= 1;
Yl <= 2;	Ym <= 1;	Yn <= 1;	Yo <= 2;

/* Grafo de tareas */
x0   = 0;
x1  >= x0 + 4 - Ya;
x2  >= x0 + 2 - Yb;
x1  >= x2;
x3  >= x1 + 3 - Yc;
x3  >= x2 + 4 - Yd;
x4  >= x3 + 1 - Yf;
x4  >= x2;
x5  >= x3 + 3 - Yh;
x5  >= x4 + 1 - Yg;
x6  >= x3;
x6  >= x5;
x7  >= x6 + 3 - Yj;
x7  >= x5 + 5 - Yi;
x8  >= x7 + 5 - Yl;
x9  >= x7 + 2 - Yk;
x10 >= x9 + 2 - Ym;
x11 >= x10 + 5 - Yo;
x10 >= x8;
x11 >= x8 + 4 - Yn;

/* Cuanto queremos acelerar la operación. */
x11 - x0 <= 25 - 3;
