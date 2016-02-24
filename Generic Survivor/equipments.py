# -*- coding: utf-8 -*-

# Módulo Equipments - 31/01/2015
# Módulo que contém os itens usáveis por Personas no jogo

import pygame as pg, random
from pygame.locals import *
from constructions import *

# Mãos vazias - Coleta itens
class Nothing(object):
	def __init__(self, owner, land, hand):
		if hand == 'l': self.name = 'Nothing'
		elif hand == 'r': self.name = 'Nothing '
		self.owner = owner
		self.land = land
	# END init()

	def act(self, target):
		if target.type in ('Tree', 'Stone', 'Bush') and target.state == 'full':
			if target.type == 'Tree':
				# Stick - Peso 0.75
				# Fiber - Peso 0.5
				quantSticks =  random.randrange(0,3)
				quantFibers =  random.randrange(0,3)
				if quantSticks > 0 :
					if 'Stick' not in self.owner.backpack: self.owner.backpack['Stick'] = 0
					self.owner.backpack['Stick'] = self.owner.backpack['Stick'] + quantSticks

				if quantFibers > 0 :
					if 'Fiber' not in self.owner.backpack: self.owner.backpack['Fiber'] = 0
					self.owner.backpack['Fiber'] = self.owner.backpack['Fiber'] + quantFibers

			elif target.type == 'Stone':
				# Rock - Peso 1.5
				quantRock =  random.randrange(0,3)
				if quantRock > 0 :
					if 'Rock' not in self.owner.backpack: self.owner.backpack['Rock'] = 0
					self.owner.backpack['Rock'] = self.owner.backpack['Rock'] + quantRock
			target.state = 'depleted'

		# Interagir com constructions sites
		if target.type == 'Construction Site':
			for item in target.recipe.requisites:
				# Inicializa os elementos da receita no dicionario de recursos do objeto
				if item not in target.resources: target.resources[item] = 0
				# Verifica a backpack da Persona para ver se possui item necessário
				if item in self.owner.backpack and self.owner.backpack[item] > 0:
					ammountToRemove = 0
					if self.owner.backpack[item] >= target.recipe.requisites[item]: ammountToRemove = (target.recipe.requisites[item] - target.resources[item])
					else: ammountToRemove = self.owner.backpack[item]
					self.owner.backpack[item] = self.owner.backpack[item] - ammountToRemove
					target.resources[item] = target.resources[item] + ammountToRemove
			
			# Verifica se o objeto tem todos os recursos pedidos
			for item in target.resources:
				if target.resources[item] == target.recipe.requisites[item]: target.destroyNow = True
				else:
					target.destroyNow = False
					break
			# Verifica se o objeto pode ser destruído para dar lugar a construção original
			if target.destroyNow == True:
				coordCSx = target.x
				coordCSy = target.y
				finalConstruction = target.recipe.name
				del target
				self.owner.land.terrain[coordCSx][coordCSy] = self.owner.land.terrainType['ground']
				if finalConstruction == 'House':
					self.owner.land.terrain[coordCSx][coordCSy] = self.owner.land.terrainType['house']
					self.land.listInstanciedResources['['+ str(coordCSx) + ',' + str(coordCSy) +']'] = House(coordCSx, coordCSy)
				elif finalConstruction == 'Deposit':
					self.owner.land.terrain[coordCSx][coordCSy] = self.owner.land.terrainType['deposit']
					self.land.listInstanciedResources['['+ str(coordCSx) + ',' + str(coordCSy) +']'] = Deposit(coordCSx, coordCSy)
	# END act()
# END Nothing()

# Machado - Derruba árvores
class Axe(object):
	def __init__(self, owner, land, dur):
		self.name = 'Axe'
		self.owner = owner
		self.land = land
		self.durability = dur
	# END init()

	def act(self, target):
		if target.type == 'Tree':
			target.playerActing = True
			target.life = target.life - 0.15 # 0.15*30 = 4.5 de dano na árvore por segundo
			if target.life <= 0:
				# Wood - Peso 7
				if 'Wood' not in self.owner.backpack: self.owner.backpack['Wood'] = 0
				self.owner.backpack['Wood'] = self.owner.backpack['Wood'] + 1
		
	# END act()
# END Axe()

# Picareta - Minera pedras
class Pickaxe(object):
	def __init__(self, owner, land, dur):
		self.name = 'Pickaxe'
		self.owner = owner
		self.land = land
		self.durability = dur
	# END init()

	def act(self, target):
		if target.type == 'Stone':
			target.playerActing = True
			target.life = target.life - 0.15 # 0.15*30 = 4.5 de dano na pedra por segundo
			if target.life <= 0:
				# Wood - Peso 9
				if 'Stone' not in self.owner.backpack: self.owner.backpack['Stone'] = 0
				self.owner.backpack['Stone'] = self.owner.backpack['Stone'] + 1
	# END act()
# END Pickaxe()



