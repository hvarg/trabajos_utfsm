#!/usr/bin/env python
#-*- coding: utf-8 -*-
from PIL import Image
import math
from numpy import *
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d as spline

#### Parte 1: Agrandando imágenes. ####
IMAGE_PATH = './homero.png'

#Diferencias divididas de newton
def ddNewton(x, y):
	#Código modificado de: http://adorio-research.org/wordpress/?p=11165
	#Obteniendo coeficientes:
	c = y[:]
	n = len(x) 
	for i in range(1, n):
		for j in range(n-1, i-1, -1): 
			c[j] = (c[j] - c[j-1])/(x[j]-x[j-i])
	#Creando la función:
	def _lambda(xpoint):
		val = c[0]
		factor = 1.0
		for i in range(1, n):
			factor *= (xpoint - x[i-1])
			val += (c[i] * factor)
		return val
	return _lambda

#Clase encargada de tratar con las imágenes.
class MyImage():
	# Abre la imagen y la separa en colores
	def __init__(self, path, size = None):
		img = Image.open(path)
		data = array(img)
		if size == None: self.X, self.Y = img.size
		else: self.X, self.Y = size
		self.data =	{'R': [], 'G': [], 'B': []}
		for x in range(self.X):
			for y in range(self.Y):
				for key, num in [('R',0), ('G',1), ('B', 2)]:
					self.data[key].append( data[x,y][num] )
	
	# Agranda la imagen, recibe un modo (spline o newton) y aplica dicho
	# algoritmo para interpolar los nuevos valores. Luego guarda la imagen.
	def resize(self, mode, ratio, name):
		if mode == 'spline': f_mode = lambda x,y: spline(x,y, kind = 'cubic')
		elif mode == 'newton': f_mode = ddNewton
		else: raise ValueError('modos permitidos: spline, newton')
		new_x, new_y = self.X * ratio, self.Y * ratio
		## Para filas:
		dic_f = {'R':{}, 'G':{}, 'B':{}}
		p_x = [k*(new_x/(self.X -1.0)) for k in range(self.X)]
		#Buscamos funciones:
		for y in range(self.Y):	
			for c in ['R','G','B']:
				dic_f[c][y] = f_mode(p_x, self.data[c][y*self.X:(y+1)*self.X])
		#Creamos nueva matriz:
		new_data = {'R':[], 'G':[], 'B':[]}
		for y in range(self.Y):
			for x in range(new_x):
				for c in ['R','G','B']:
					new_data[c].append(dic_f[c][y](x))

		#Ahora para las columnas con la imagen extendida.
		dic_f = {'R':{}, 'G':{}, 'B':{}}
		p_y = [k*(new_y/(self.Y -1.0)) for k in range(self.Y)]
		#Buscamos funciones:
		for i in range(new_x):
			for c in ['R','G','B']:
				dic_f[c][i] = f_mode(p_y, [new_data[c][new_x*j + i] 
											for j in range(self.Y)])
		#Evaluamos:
		new_data2 = {'R':[], 'G':[], 'B':[]}
		for x in range(new_x):
			for y in range(new_y):
				for c in ['R','G','B']:
					try: value = dic_f[c][y](x)
					except: value = 0
					if value < 0: value = 0
					elif value>255: value = 255
					new_data2[c].append(int(value))
		# Creamos la imagen y guardamos:
		img = Image.new('RGB', (new_x,new_y))
		img.putdata(zip(new_data2['R'], new_data2['G'], new_data2['B']))
		img.show()
		img.save(name)

img = MyImage(IMAGE_PATH, (100,100))
# Des-comentar las siguientes lineas para crear las imágenes, Nota: puede tomar
# mucho tiempo, en especial Newton.
#img.resize('spline',5,'../Informe/Graficos/5xhomero_spline.png' )
#img.resize('newton',5,'../Informe/Graficos/5xhomero_newton.png' )

################################################################################

#### Parte 2: Puntos de Chebysev ####

#Función para la tarea
f1 = lambda x,y: 1./(y + x**2)

# Retorna los puntos óptimos de Chebyshev
def chebPoints(n):
	xi = []
	for i in range(1,n+1):
		a = (2*i-1)*math.pi
		b = 2*n
		xi.append(math.cos(a/b))
	return asarray(xi)	

#coNewton: Coeficientes de Newton para D.D.Newton
#Recibe: los puntos de Chebyshev
#Retorna: Coeficientes de D.D. de Newton (f[x1 .... xn])
def coNewton(x, a):
	y = f1(x,a)
	num, den, coef, n = [],[],[y[0]],len(x)  
	for j in range(1, n):
		for i in range(1, len(y)):
			#por la 1 vez
			i2 = (2*i-1) if j > 1 else i
			dif = (y[i] - y[i-1])/(x[i2] - x[i2-1])
			num.append(dif)
			den.extend([x[i2],x[i2-1]])
		if len(den)>0:
			den.pop(0)
			den.pop(-1)
		if len(num)>0: coef.append(num[0])
		y, x = num, den
		num, den = [], []
	return coef

#polinter: Polinomio interpolador D.D.Newton dado los puntos de Chebyshev.
#Recibe: x a evaluar, los coeficientes de Newton y los puntos de Chebyshev.
#Retorna: El polinomio interpolador evaluado en x.
def polinter(x, coef, pnts):
	Px = coef[0]
	largo, mult = len(pnts), 1
	for i in range(1,largo):
		for j in range(0,i):
			mult = mult*(x-pnts[j])
		Px = Px + coef[i]*mult
		mult = 1
	return Px

#plot1: gráfica 2 funciones en la recta de valores x. k es el valor
#que representa el k-ésimo polinomio de Chebyshev
def plot1(f1, f2, xi, k):
	plt.plot(xi, f1,'r', xi, f2, 'b')
	plt.ylabel('f(x,1) (rojo), P(x) (azul)')
	plt.xlabel('x')
	plt.axis([-1,1,0,1.5])
	plt.title('f(x,1) vs P'+str(k)+'(x)')
	plt.savefig('../Informe/Graficos/pol'+str(k)+'.png', dpi=60)
	plt.hold(False)

#plot2: gráfica el Error entre la función "real" y la interpoladora versus
#el número de puntos de Chebyshev
def plot2(error, n, name):
	plt.plot(n, error)
	plt.ylabel('log_10(En(1))')
	plt.xlabel('n')
	plt.yscale('log')
	plt.title('log10(En(1)) vs '+name+' puntos de Chebyshev)')
	plt.savefig('../Informe/Graficos/Error-vs-n3.png')
	plt.hold(False)

def plot3(pend, alpha):
	plt.plot(alpha, pend)
	plt.ylabel('Pendiente')
	plt.axis([0,270,-5,0])
	plt.xlabel('Alpha')
	plt.title("Pendiente vs Alpha")
	plt.savefig('../Informe/Graficos/SvsA.png')
	plt.hold(False)


### Actividad ###
def err(a,b):
	return abs(a-b)
def logscale(x):
	return log10(x)

## Datos ##
# Puntos x-espaciados
xi = arange(-1,1,0.0001)
error, pend = [], []
deb = True # Debugg
# Alpha de la función
alpha = arange(-3,3,0.6)
alpha = [10**(i) for i in alpha]
# Alpha = range(1,2)	# Comentar para variar el alpha
# Puntos de Chebyshev
n = range(3,8)
# Nombre plot2 para b) o d)
name = '2:100'

# Cambiar los parámetros de este bucle y des-comentar para generar los gráficos.
if deb:
	for a in alpha:
		for k in n:
			cP = chebPoints(k)				# Puntos de Chebyshev
			coef = coNewton(cP,a)			# Coeficientes D.D.Newton
			Px = polinter(xi, coef, cP)		# Función interpoladora en xi
			fx = f1(xi,a)					# Función dada en xi
			#plot1(fx, Px, xi,k)
			error.append(max(err(fx,Px)))	# Máx valor error
		#plot2(error, n, name)
		pend.append(polyfit(logscale(error), n, 1)[0])	# Pendiente
		error = []
#	plot3(pend,alpha)
