#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random, sys

l = len(sys.argv)
if l < 7 or l > 9:
    print >> sys.stderr, "Error en los argumentos."
    print "USO:\n ./generate.py numero_buses capacidad_buses numero_estaciones",
    print "numero_puntos_de_encuentro numero_refugios total_personas",
    print "[dist_min] [dist_max]"
    print "NOTA: El total personas sera multiplicado por la capacidad del bus."
    exit(-1)

try:
    nB =  int(sys.argv[1])   #raw_input("Numero de Buses: ")
    cB =  int(sys.argv[2])   #raw_input("Capacidad de Buses: ")
    nE =  int(sys.argv[3])   #raw_input("Estaciones: ")
    nP =  int(sys.argv[4])   #raw_input("Numero de puntos de encuentro: ")
    nR =  int(sys.argv[5])   #raw_input("Numero de refugios: ")
    T  =  int(sys.argv[6])   #raw_input("Total de personas: ") # mult por cbuses
except:
    print >> sys.stderr, "Los argumentos deben ser numeros!"
    exit(-1)

if l >= 8: rmin = int(sys.argv[7])
else: rmin = 1
if l >= 9: rmax = int(sys.argv[8])
else: rmax = 10

sobras = int(T*0.1)

if nE > nB: 
    print >> sys.stderr, "Error: Menos estaciones que buses."
    exit(-1)
if nP > T: 
    print >> sys.stderr, "Error: Menos puntos de encuentro que personas."
    exit(-1)

bus_est = [1] * nE
while sum(bus_est) != nB:
    r = random.randint(1,nE-1)
    bus_est[r] += 1

peps = [1] * nP
while sum(peps) != T:
    r = random.randint(1,nP-1)
    peps[r] += 1

refs = [1] * nR
while sum(refs) < T + sobras:
    r = random.randint(1,nR-1)
    refs[r] += 1

print str(nB) + ':', cB
print str(nE) + ':',
for i in bus_est:
    print i,
print
print str(nP) + ':', str(T*cB) + ':',
for i in peps:
    print i*cB,
print
print str(nR) + ':', str(sum(refs)*cB) + ':',
for i in refs:
    print i*cB,
print '\n'

for i in range(1, nE+1):
    print str(i) + ':',
    for _ in range(nP):
        print random.randint(rmin,rmax),
    print
print

for i in range(1, nP+1):
    print str(i) + ':',
    for _ in range(nR):
        print random.randint(rmin,rmax),
    print
