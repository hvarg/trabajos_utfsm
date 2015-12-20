#!/usr/bin/env python
# -*- coding: utf-8 -*-

#Nota: Se utiliza 'repr()' en vez de la conversión implícita 'str()'
#Referencia:
# http://drj11.wordpress.com/2007/07/03/python-poor-printing-of-floating-point/

### a)
print 'a) 5 - 2^-53:'
#Primero guardamos los números a operar para ver si pueden ser representados:
a = 5
b = 2**-53
print a, '-', b
#Se ve por pantalla: "5 - 1.11022302463e-16" lo que muestra que los números
#pueden ser representados en punto flotante. Ahora calculamos la resta:
c = a - b
print 'Resultado:', repr(c)
#La salida es: "5.0", lo que nos muestra que el resultado se aproximó, debido a
#que el resultado 'real' está entre 5 y su sucesor representable.


### b)
print '\nb) (5 - 3*2^-53) + 5:'
#Primero volvemos a guardar los números:
a = 5
b = 3*2**-53
print '(', a, '-', b, ') +', (a)
#La salida es: "5 y 3.33066907388e-16" es decir los números fueron guardados
#correctamente. Ahora la operación, primero el paréntesis:
c = (a - b)
print 'Paréntesis:', repr(c)
#Imprime solamente: "5.0", de lo cual deducimos que ya ocurrió el 'error' visto
#anteriormente, imprimimos el resultado que se espera:
d = c + a
print 'Resultado:', repr(d)
#La salida es la esperada.

#Probamos que se puede almacenar números como 5 - 2^-50:
a = 5 - 2**-50
print '\n5 - 2^-50 =', repr(a)
#El resultado es: "5 - 2^-50 = 4.999999999999999"
