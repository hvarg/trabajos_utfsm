/**********************************************************
* Variables:                                              *
* 	xi	= Existencia de antena en región i.               *
*	yi	= Antena ampliada en región i..                   *
*   Di	= Demanda de la región i.                         *
*   Ci	= Costo de construcción en la región i.           *
*	i entre 0 y 15, con 0 como la región metropolitana.   *
**********************************************************/

/* Función objetivo, minimiza costos. */
min: 4  x15 + 2 y15 + 3  x1  + 2 y1  + 2 x2  + 2 y2  + 5 x3  + 2 y3  +
	 7  x4  + 2 y4  + 6  x5  + 2 y5  + 8 x0  + 2 y0  + 7 x6  + 2 y6  +
	 8  x7  + 2 y7  + 7  x8  + 2 y8  + 9 x9  + 2 y9  + 9 x10 + 2 y10 +
	 10 x11 + 2 y11 + 11 x12 + 2 y12 + 4 x13 + 2 y13 + 7 x14 + 2 y14;

/* Demandas por región. */
D15 = 650;		D1  = 650;		D2  = 700;		D3  = 800;
D4  = 400;		D5  = 950;		D0  = 1200;		D6  = 600;
D7  = 400;		D8  = 700;		D9  = 450;		D10 = 400;
D11 = 350;		D12 = 350;		D13 = 300;		D14 = 250;		

/* Solo puede construir la ampliación si ya hay antena. */
y15 <= x15;		y1  <= x1;		y2  <= x2;		y3  <= x3;
y4  <= x4;		y5  <= x5;		y0  <= x0;		y6  <= x6;
y7  <= x7;		y8  <= x8;		y9  <= x9;		y10 <= x10;
y11 <= x11;		y12 <= x12;		y13 <= x13;		y14 <= x14;		

/* Restricción de Santiago */
x0 + x5 <= 1;	x0 + x6 <= 1;

/* Restricción de demanda. */
D15 <= 1800 x15 + 400 y15 + 1800 x1  + 400 y1  - D1;
D1  <= 1800 x1  + 400 y1  + 1800 x2  + 400 y2  - D2  + 1800 x15 + 400 y15 - D15;
D2  <= 1800 x2  + 400 y2  + 1800 x3  + 400 y3  - D3  + 1800 x1  + 400 y1  - D1;
D3  <= 1800 x3  + 400 y3  + 1800 x4  + 400 y4  - D4  + 1800 x2  + 400 y2  - D2;
D4  <= 1800 x4  + 400 y4  + 1800 x5  + 400 y5  - D5  + 1800 x3  + 400 y3  - D3;
D5  <= 1800 x5  + 400 y5  + 1800 x0  + 400 y0  - D0  + 1800 x4  + 400 y4  - D4;
D0  <= 1800 x0  + 400 y0  + 1800 x6  + 400 y6  - D6  + 1800 x5  + 400 y5  - D5;
D6  <= 1800 x6  + 400 y6  + 1800 x7  + 400 y7  - D7  + 1800 x0  + 400 y0  - D0;
D7  <= 1800 x7  + 400 y7  + 1800 x8  + 400 y8  - D8  + 1800 x6  + 400 y6  - D6;
D8  <= 1800 x8  + 400 y8  + 1800 x9  + 400 y9  - D9  + 1800 x7  + 400 y7  - D7;
D9  <= 1800 x9  + 400 y9  + 1800 x10 + 400 y10 - D10 + 1800 x8  + 400 y8  - D8;
D10 <= 1800 x10 + 400 y10 + 1800 x11 + 400 y11 - D11 + 1800 x9  + 400 y9  - D9;
D11 <= 1800 x11 + 400 y11 + 1800 x12 + 400 y12 - D12 + 1800 x10 + 400 y10 - D10;
D12 <= 1800 x12 + 400 y12 + 1800 x13 + 400 y13 - D13 + 1800 x11 + 400 y11 - D11;
D13 <= 1800 x13 + 400 y13 + 1800 x14 + 400 y14 - D14 + 1800 x12 + 400 y12 - D12;
D14 <= 1800 x14 + 400 y14 + 1800 x13 + 400 y13 - D13;

int x15, x1, x2, x3, x4, x5, x0, x6, x7, x8, x9, x10, x11, x12, x13, x14,
	y15, y1, y2, y3, y4, y5, y0, y6, y7, y8, y9, y10, y11, y12, y13, y14;
