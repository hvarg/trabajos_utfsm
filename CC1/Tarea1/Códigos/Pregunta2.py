#!/usr/bin/env python
# -*- coding: utf-8 -*-
import math
MAX_P = 20 #p maximo para el bucle.

#Se define una función que calcule la expresión:
def formula(p, verbose=False):
	x = 10**-p				# x = 10^-p
	a = math.e**x			# a = e^x
	b = math.cos(x)			# b = cos(x)
	c = math.sin(x)			# c = sin(x)
	d = x**3				# d = x^3
	arriba = a + b - c - 2	
	y = arriba/d			
	p = str(p) if p>9 else '0' + str(p)
	print p + ':', str(arriba) + '/' + str(d), '=', repr(y)
	#Con verbose = True se obtienen más detalles.
	if verbose:
		print "\tx         :", repr(x)
		print "\ta = e^x   :", repr(a)
		print "\tb = cos(x):", repr(b)
		print "\tc = sin(x):", repr(c)
		print "\td = x^3   :", repr(d)
		e = a + b
		print "\ta + b     :", repr(e)
		f = b - c
		print "\tb - c     :", repr(f)
		print "\t(a+b) - c :", repr(e-c)
		print "\ta + (b-c) :", repr(a+f)
		print '#'*50

#Se imprime para ver desde donde empieza a ocurrir el error:
print '-p|Value:'
for i in range(1,MAX_P+1):
	formula(i)

#Se ve que el error ocurre antes de la 6ta iteracion, se imprimen más detalles:
print '\nDetallado:'
for i in range(4,7):
	formula(i,True)
