/****************************************************
* Variables:										*
*	i_ed = vehículos Smart Fortwo ED importados.	*
*	i_up = vehículos Volkswagen e-up! importados.	*
*	v_ed = vehículos Smart Fortwo ED vendidos.		*
*	v_up = vehículos Volkswagen e-up! vendidos.		*
*	t	 = trabajadores contratados.				*
****************************************************/

/* Se restó el 10% directamente */
max: 19800000 v_ed + 27000000 v_up - 18000000 i_ed - 23000000 i_up - 1200000 t;

/* Restricciones */
i_ed >= 20;					// Al menos importar 20 ED.
i_ed - v_ed >= 3;			// Dejar al menos 3 a fin de mes.
i_up - v_up >= 3;
v_ed - 0.6 i_ed >= 0;		// Vender al menos el 60%.
v_up - 0.6 i_up >= 0;
t = 0.2 i_ed + 0.2 i_up;	// 1 trabajador cada 5 vehículos.
t <= 40;					// a lo más 40 trabajadores.

/* Naturaleza de las variables */
v_ed >= 0;
v_up >= 0;
i_ed >= 0;
i_up >= 0;
t	 >= 0;
