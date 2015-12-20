#!/usr/bin/env python
#-*- coding: utf-8 -*-
#SVD
import numpy
from numpy.linalg import svd
from numpy import matrix, array
#Manejo de imágenes
from PIL import Image
#Gráficos
import matplotlib.pyplot as plt

### Parte 0: Datos ###
e_mach = 10**(-16)
A = matrix([[0, 1, 3],
			[1, 3, 1],
			[2,-1, 3],
			[0, 1,-1]])
#Imágenes a ocupar
imgs = {'fractal': "Dataset/imagenes/fractal.png",
		'logo-di': 'Dataset/imagenes/logo-di.png',
		'paisaje': 'Dataset/imagenes/paisaje.bmp' }
#matrices formato txt
mTxt = {"matriz1":	"Dataset/matrices/matriz1.txt",
		"matriz2":	"Dataset/matrices/matriz2.txt",
		"matriz3":	"Dataset/matrices/matriz3.txt",
		"matriz4":	"Dataset/matrices/matriz4.txt",
		"matriz5":	"Dataset/matrices/matriz5.txt" }

### Parte 1: Factorización SVD ###
#Retorna el SVD, full indica si será completo o reducido.
def getSVD(A, verbose=False, full=True):
	u,z,v = svd(A, full_matrices = 1 if full else 0)
	if verbose: print 'U:', u.shape, ' Z:', z.shape, ' V:', v.shape
	return u,z,v
#Iteración para imprimir.
for name, bol in (('reducida', False), ('completa', True)):
	print 'Factorizacion', name,  'de A:'	
	u,z,v = getSVD(A,True,bol)
	print 'U:\n',u,'\nZ:\n',z,'\nV:\n',v,'\n'

### Parte 2: Compresión de imágenes ###
#imgToMatx toma una imagen Image y retorna una matriz.
def imgToMatx(img):
	x, y = img.size
	array = list(img.getdata())
	return matrix([array[x*i:x*(i+1)] for i in range(y)])

#txtToMatx toma la ruta a un fichero de texto, retorna una matriz con él.
def txtToMatx(fdir):
	f = open(fdir, "r")
	# matrix puede leer desde string si las lineas están separadas por ';'
	data = f.read().replace('\n',';')[:-1]
	f.close()
	return matrix(data)

#imgOpen abre una imagen y la retorna como una matriz.
def imgOpen(fdir, mode='F'):
	img = Image.open(fdir).convert(mode)
	return imgToMatx(img)

#compress toma una matriz m y un valor de compresión y retorna m comprimida.
#Si el parámetro last_value es True retornará además el ultimo valor singular,
#esto es útil solo para los gráficos.
def compress(matx, compress_value, verbose=False, last_value=False):
	if compress_value > 1 or compress_value < 0: 
		raise ValueError('Compresión debe ser un valor entre 0 y 1')
	u,z,v = getSVD(matx,verbose)
	r = z.shape[0]				#Dimensión de matriz con valores singulares.
	result = matrix("0")
	#Rearmando matriz (propiedad 5) descartando un % de valores singulares.
	for k in range(0,r):
		result = result + ((u[:,k]*z[k])*v[k])
		if k > r*compress_value: 
			lvalue = z[k] 
			break
	if last_value: return result, lvalue
	else: return result

#quality determina la calidad entre dos matrices de la misma imagen.
def quality(mtx,mtx2):
	dmtx = svd(mtx - mtx2)
	mtx = svd(mtx)
	fnorm = 0
	for x in mtx[1]:			#Norma de Frobenius para la imagen original.
		fnorm = x**2 + fnorm
	den = fnorm**0.5 + fnorm	#Denominador.
	fnorm = 0
	for x in dmtx[1]:			#Norma de Frobenius para la diferencia.
		fnorm = x**2 + fnorm
	num = fnorm**0.5 			#Numerador.
	return den/num				#Inversa de la división.

## Para la creación de gráficos: ##
#Gráfico 1, recibe una matriz mtx y gráfica sus valores singulares
def graph1(mtx, filename, path="../Informe/Graficos/"):
	u, svalue, v = getSVD(mtx,True)
	size = svalue.size
	plt.plot(range(0,size),svalue)
	plt.yscale('log')
	plt.ylabel('Magnitud de valores singulares')
	plt.xlabel('Posicion de los valores singulares')
	plt.title('Magnitud de valores singulares vs Posicion')
	plt.savefig(path+filename+'-svalue.png', dpi = 60)
	print('Grafico "'+path+filename+'-svalue.png" guardado.')
	plt.hold(False)

#Gráfico 2: Recibe una matriz mtx y crea los gráficos de calidad v/s el valor 
#			singular y calidad v/s tamaño de la imagen.
#Advertencia: Esta función se basa en comprimir varias veces la imagen por lo
#			  que puede ser realmente lenta.
def graph2(mtx, filename, path="../Informe/Graficos/"):
	qlty = [0.8,0.5,0.3,0.1,0.01]
	perc = [80, 50, 30, 10, 1]
	v_quality, v_last, size_compress = [],[],[]
	m,n = mtx.shape
	p = svd(mtx)[1].size
	calcSize = lambda x: 4*x*(m+n+1)
	orig_size = calcSize(p)
	#Bucle con varias compresiones de la imagen.
	for c in qlty:
		mtx2, pC = compress(mtx, c, last_value=True)
		v_quality.append(quality(mtx, mtx2))
		v_last.append(pC)
		size_compress.append(calcSize(p*c)/orig_size)
	#Creando el gráfico de calidad:
	plt.plot(perc, v_quality, 'r', perc, v_last, 'b')
	plt.yscale('log')
	plt.ylabel('Calidad (rojo) - p-esimo valor singular (azul)')
	plt.xlabel('Porcentaje de valores singulares')
	plt.title('Calidad  vs Valores Singulares')
	plt.savefig(path+filename+'-quality.png', dpi = 60)
	print('Grafico "'+path+filename+'-quality.png" guardado.')
	plt.hold(False)
	#Creando el gráfico de tamaño:
	plt.plot(perc, v_quality, 'r', perc, size_compress, 'b')
	plt.yscale('log')
	plt.ylabel('Calidad (rojo) - Razon de compresion (azul)')
	plt.xlabel('Porcentaje de valores singulares')
	plt.title('Calidad y  Compresion vs % valores singulares')
	plt.savefig(path+filename+'-size.png', dpi = 60)
	print('Grafico "'+path+filename+'-size.png" guardado.')
	plt.hold(False)

ENABLE_PLOT = True #True creará los graficos, demorará mucho más.

#Bucle para las imágenes.
for key in imgs:
	print 'Para', key, 'tenemos:'
	m = imgOpen(imgs[key])
	if ENABLE_PLOT:
		graph1(m, key)
		graph2(m, key)
	else: getSVD(m,True)
#Bucle para las matrices.
for key in mTxt:
	print 'Para', key, 'tenemos:'
	m = txtToMatx(mTxt[key])
	if ENABLE_PLOT:
		graph1(m, key)
		graph2(m, key)
	else: getSVD(m,True)
