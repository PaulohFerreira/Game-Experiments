# -*- coding: utf-8 -*-

# Módulo Constructions - 10/02/2015
# Módulo que contém as construções que podem ser feitas pelas Personas

import pygame as pg
from pygame.locals import *

class ConstructionSite(object):
	def __init__(self, x, y, recipe):
		self.type = 'Construction Site'
		self.x = x
		self.y = y
		self.image = pg.image.load('graphics/constructions/construction_site.png')
		self.rect = pg.Rect(self.x, self.y, self.image.get_width(), self.image.get_height())

		self.recipe = recipe
		self.resources = {};
		self.destroyNow = False
	# END init()
# END ConstructionSite()

class House(object):
	def __init__(self, x, y):
		self.type = 'House'
		self.x = x
		self.y = y
		self.image = pg.image.load('graphics/constructions/house.png')
		self.rect = pg.Rect(self.x, self.y, self.image.get_width(), self.image.get_height())		
# END House()

class Deposit(object):
	def __init__(self, x, y):
		self.type = 'Deposit'
		self.x = x
		self.y = y
		self.image = pg.image.load('graphics/constructions/deposit.png')
		self.rect = pg.Rect(self.x, self.y, self.image.get_width(), self.image.get_height())		
# END House()
