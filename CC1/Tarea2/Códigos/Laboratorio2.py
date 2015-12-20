#!/usr/bin/env python
# -*- coding: utf-8 -*-
import math as m
from sympy import *
import sympy
import scipy.special
import time

# Función de Bessel
Jbessel = scipy.special.sph_jn
Ybessel = scipy.special.sph_yn
# Tolerancia de bisección y newton
TOL = 0.00000001
# Símbolo x
x = symbols('x')

#f es el diccionario de funciones:
f = {	1:	lambda x:	m.sin(x) - x,
		2:	lambda x:	x**3,
		3:	lambda x:	1.0/m.exp(x**2),
		4:	lambda x:	(x - 10),
		5:	lambda x:	((2*x-1)**5)/32,
		6:	lambda x:	tuple(Jbessel(0,x)[0])[0], 
		7:	lambda x: 	tuple(Ybessel(0,x)[0])[0] }

#g es el diccionario de funciones simbólicas.
g = {	1:	1 - (sin(x)/x),
	2:	x**10,
	3:	1/exp(x**2),
	4:	((x-10)**2)**0.5,
	5:	((2*x-1)**5)/32,
	6:	lambda x: Jbessel(0,x),
	7:	lambda x: Ybessel(0,x)}

#x0s es el diccionario de valores iniciales sugeridos.
x0s = { 1: 0.5, 2: 0.5, 3: 0, 4: 8, 5: 0, 6: 1, 7: 2 }

#rangos es el diccionario de rangos para la bisección.
rangos = {1: (-2, 3), 2: (-4, 1), 3: (-5, 3), 
		  4: (5, 12), 5: (-1, 4), 6: (2, 5), 7: (-4,10) }

#timeTest retorna el tiempo de ejecución de una función.
def timeTest(f, args):
	start = time.time()
	if hasattr(args, '__iter__'): out = f(*args)
	else: out = f(args)
	totalTime = time.time() - start
	return out, totalTime

#getSolutionError calcula el error de los datos con respecto a la solución.
def getSolutionError(value, data):
	if not hasattr(data, '__iter__'): return m.fabs(data - value)
	errors = []
	for elem in data:
		errors.append(m.fabs(elem - value))
	return errors

### PARTE 1: MÉTODO DE BISECCIÓN ###
#fcheck evalúa si dos puntos de una función tienen diferentes signos.
def fcheck(func,a,b):
	fa,fb = func(a), func(b)
	if (fa < 0  and fb <0) or (fa > 0 and fb >0): return False
	return True

def bisec(f, a, b):
	if not fcheck(f,a,b): raise ValueError('f(a)*f(b)>0')
	count = 0
	prev_value = []
	while True:
		count +=1
		c = (a+b)/2.0
		prev_value.append(c)
		r = f(c)
		if (b-a)/2 < TOL or r == 0: break
		if fcheck(f,a,c): b = c
		else: a=c
	return c, count, prev_value

#Imprimiendo resultados
verbose = True		#Cambiar a false para que no imprima los errores
for i in range(1,8):
	try:
		a,b = rangos[i]
		[value, iterations, prev_values], t = timeTest(bisec, (f[i],a,b))
		print 'F =',i,'-> Se obtuvo:',repr(value),\
				'en', iterations,'iteraciones.'
		print 'El algoritmo demoro',t,'segundos'
		if verbose:
			print 'Los errores son:\n\titer\tERROR'
			for i, elem in enumerate(getSolutionError(value, prev_values)):
				print '\t',i,'\t',elem
	except ValueError, e:
		print 'F =',i,'-> No pudo ser evaluado por biseccion ya que:',e
	finally:
		print '#'*70
	
### PARTE 2: MÉTODO DE NEWTON ###

#error Cálcula los errores entre iteraciones 
def error(xi,x0,e,ALL):
	ei = ((xi-x0)**2)**0.5
	if e != 0: #No consideramos el error para la 1 iteración
		elin = ei/e
		equa = ei/(e**2)
	else:
		elin = ei
		equa = ei
	if ALL:
		return [ei, elin, equa]
	print "X_i: ", xi, " | e/ei: ",elin," | e/ei^2: ",equa
	return ei

#newton implementa el método de newton
def newton(f, x0, key):
	if key < 6:
		df = diff(f, x) #derivamos la función
	e = 0 #error
	saveErr=[] #data ei for Plot
	#Cálculo de la multiplicidad
	m = testMult(f, x0, key)
	if m==0:
		return
	print "f",key," |initial: ",x0,"|multiple root: ",m
	print "#Iteration#"
	while True:
		if key < 6:
			if limit(f, x, x0)==0:
				break
		fx0 = f.evalf(subs={x:x0}) if key<6 else f(x0)[0][0]
		dfx0 = df.evalf(subs={x:x0}) if key<6 else f(x0)[1][0]
		if dfx0 == 0:
			print "F'(Xi) = 0, Metodo no valido"
			return
		xi = x0 - m*(fx0/dfx0) #Método de Newton
		e = error(xi,x0,e, False) #error
		saveErr.append(xi)
		x0 = xi
		if e < TOL:
			break
	print "Root found: ", x0
	print "Errores: "
	i = 1
	for elem in saveErr:
		print '\t',i,'\t', ((elem-x0)**2)**0.5
		i = i + 1

#Chequea raices múltiples y convergencia
def testMult(f,x0, key):
	if key < 6:
		df = diff(f,x)
	e,err, check1, check2 = 0,[],[],[]
	for i in range(0,5):
		if key < 6:
			if limit(f,x,x0)==0:
				return 1
		fx0 = f.evalf(subs={x:x0}) if key<6 else f(x0)[0][0]
		dfx0 = df.evalf(subs={x:x0}) if key<6  else f(x0)[1][0]
		xi = x0 - (fx0/dfx0) 
		err = error(xi,x0,e,True) #error
		e = err[0]
		check1.append(err[1]) #e/ei
		check2.append(err[0]) #e/ei^2
		x0 = xi
	#Si el error crece, no hay raices
	if check2[4]/check2[3] > 1:
		print "The function not have root"
		return 0
	#Si la tasa de error lineal es constante
	if abs(check1[4] - check1[3]) < 1:
		return round(1/(1-check1[4])) #se resuelve: s=m-1/m -> m=1/(1-s)
	else:
		return 1

for key in x0s:
	trash, t = timeTest(newton, (g[key], x0s[key], key))
	print "DEMORO:",t,"segundos.\n",'#'*70
