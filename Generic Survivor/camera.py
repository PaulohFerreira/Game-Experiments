# -*- coding: utf-8 -*-

# Módulo Camera - 27/01/2015
# Módulo que contém o criador e gerencidor da câmera do jogo

import pygame as pg
from pygame.locals import *

class Camera(object):
	def __init__(self):
		global DisplaySurface

		DisplaySurface = pg.display.get_surface()
		self.sizeX, self.sizeY = DisplaySurface.get_size()

		# Dimensões do terreno em pixels (Quantidade de blocos no eixo * tamanho do bloco em pixels)
		self.dimX = 1200*40
		self.dimY = 1200*40

		self.offsetX = 0
		self.offsetY = 0
	# END init()

	def set_focus(self, player):
		self.player = player

		# Compensação para colocar a câmera no meio da tela
		self.XCompensation = self.sizeX*0.5
		self.YCompensation = self.sizeY*0.5
	# END set_focus()

	def update(self):
		# Caso a Persona esteja próximo ao ponto inicial do eixo X, não move a câmera - isso impede de mostrar a parte não renderizada do mapa
		if self.player.x <= self.XCompensation: self.offsetX = 0
		# Caso a Persona esteja próximo ao ponto final do eixo X, não move a câmera - isso impede de mostrar a parte não renderizada do mapa
		elif self.player.x >= self.dimX - self.XCompensation: self.offsetX = self.dimX - self.sizeX
		# Caso não ocorra nada acima, atualiza a posição da câmera normalmente
		else: self.offsetX = self.player.x - self.XCompensation

		# Caso a Persona esteja próximo ao ponto inicial do eixo Y, não move a câmera - isso impede de mostrar a parte não renderizada do mapa
		if self.player.y <= self.YCompensation: self.offsetY = 0
		# Caso a Persona esteja próximo ao ponto final do eixo Y, não move a câmera - isso impede de mostrar a parte não renderizada do mapa
		elif self.player.y >= self.dimY - self.YCompensation: self.offsetY = self.dimY - self.sizeY
		# Caso não ocorra nada acima, atualiza a posição da câmera normalmente
		else: self.offsetY = self.player.y - self.YCompensation
	# END update()

	def transform(self,x,y):
		self.newPosX = x - self.offsetX
		self.newPosY = y - self.offsetY

		return (self.newPosX, self.newPosY) 
	# END transform()

# END Camera()
