# -*- coding: utf-8 -*-

# Módulo Characters - 22/01/2015
# Módulo que contém as classes para dos objetos personagens

import pygame as pg, sys
from pygame.locals import *
from world import Tree, Stone
from equipments import *
from constructions import *
from recipes import *

# Classe Persona - Personagem básico inicial
class Persona(object):	
	# Método criador
	def __init__(self, name, sex, role, x, y, camera, land):
		# Torna variáveis visíveis as outras funções
		global DisplaySurface, BasicFont, speed

		# Informações da superfície de desenho
		DisplaySurface = pg.display.get_surface()
		BasicFont = pg.font.Font(None, 18)

		# Inicialização do Objeto
		self.name = name
		self.sex = sex
		self.x = x
		self.y = y
		self.safeX = x
		self.safeY = y
		self.role = role
		self.land = land
		self.baseHud = pg.image.load('graphics/characters/baseHud.png')
		self.baseHud_backpack = pg.image.load('graphics/characters/baseHud_backpack.png')
		self.baseHud_crafting = pg.image.load('graphics/characters/baseHud_crafting.png')
		self.listNavigator = pg.image.load('graphics/characters/navigator.png')

		if self.role == 'Peasant':
			if self.sex == 'f': self.image = pg.image.load('graphics/characters/peasantF.png')
			else: self.image = pg.image.load('graphics/characters/peasantM.png')

		# Variáveis de status
		self.hp = 100
		self.stamina = 100
		self.feeding = 100
		self.hidratation = 100
		self.isSelected = False

		# Variáveis para movimentação
		self.speed = 3
		self.moveUp = False
		self.moveDown = False
		self.moveLeft = False
		self.moveRight = False
		self.rect = pg.Rect(self.x, self.y, self.image.get_width(), self.image.get_height())

		# Variáveis de ação
		self.facing = 'down'
		self.actingL = False
		self.actingR = False
		self.craftingOpened = False
		self.backpackOpened = False
		self.enter = False
		self.zPressed = False
		self.xPressed = False
		
		# Variáveis de inventário e equipamento
		self.left_hand = Nothing(self, land, 'l')
		self.right_hand = Nothing(self, land, 'r')
		self.backpack = {};
		self.actualWeight = 0
		self.maxWeight = 25.0
		# PositionChoice - Responsável por guardar a posição do cursor de escolha na janela de crafting
		self.positionChoiceCraft = 1
		self.positionChoiceBackpack = 1

		# Lista de receitas conhecidas por cada Persona
		self.recipeList = {};
		self.recipeList['Rope'] = Recipe('Rope')
		self.recipeList['Axe'] = Recipe('Axe')
		self.recipeList['Pickaxe'] = Recipe('Pickaxe')
		self.recipeList['House'] = Recipe('House')
		self.recipeList['Deposit'] = Recipe('Deposit')

		# Lista de comportamentos conhecidos por cada Persona
		self.behaviours = {};

		# Variáveis da câmera
		self.camera = camera
	# END init()

	# Método atualizador
	def update(self):
		# Atualização do movimento
		if self.moveUp == True and self.craftingOpened == False and self.backpackOpened == False:
			self.facing = 'up'
			self.y = self.y - self.speed
		if self.moveDown == True  and self.craftingOpened == False and self.backpackOpened == False:
			self.facing = 'down'
			self.y = self.y + self.speed
		if self.moveLeft == True  and self.craftingOpened == False and self.backpackOpened == False:
			self.facing = 'left'
			self.x = self.x - self.speed
		if self.moveRight == True  and self.craftingOpened == False and self.backpackOpened == False:
			self.facing = 'right'
			self.x = self.x + self.speed

		# Atualização da posição do retângulo de colisão
		self.rect.x = self.x
		self.rect.y = self.y

		# Tratamento de Colisão
		# Pega a posição do player e converte em coordenadas em relação o grid de terreno
		posPlayerX = int((self.x + (self.rect.width/2))/self.land.imageWidth)
		posPlayerY = int((self.y + (self.rect.height/2))/self.land.imageHeight)

		# Verifica os tipos de terreno que estão ao redor do player
		tileUp = self.land.terrain[posPlayerX][posPlayerY - 1]
		tileDown = self.land.terrain[posPlayerX][posPlayerY + 1]
		tileLeft = self.land.terrain[posPlayerX - 1][posPlayerY]
		tileRight = self.land.terrain[posPlayerX + 1][posPlayerY]

		# Cria os retângulos de colisão ao redor do player
		rectUp = pg.Rect((posPlayerX)*self.land.imageWidth, (posPlayerY - 1)*self.land.imageHeight, self.land.imageWidth, self.land.imageHeight)
		rectDown = pg.Rect((posPlayerX)*self.land.imageWidth, (posPlayerY + 1)*self.land.imageHeight, self.land.imageWidth, self.land.imageHeight)
		rectLeft = pg.Rect((posPlayerX - 1)*self.land.imageWidth, (posPlayerY)*self.land.imageHeight, self.land.imageWidth, self.land.imageHeight)
		rectRight = pg.Rect((posPlayerX + 1)*self.land.imageWidth, (posPlayerY)*self.land.imageHeight, self.land.imageWidth, self.land.imageHeight)

		# Verificação e tratamento da colisão
		if self.rect.colliderect(rectUp) and tileUp != self.land.terrainType['ground'] : self.y = self.y + self.speed
		elif self.rect.colliderect(rectDown) and tileDown != self.land.terrainType['ground'] : self.y = self.y - self.speed
		if self.rect.colliderect(rectLeft) and tileLeft != self.land.terrainType['ground'] : self.x = self.x + self.speed
		elif self.rect.colliderect(rectRight) and tileRight != self.land.terrainType['ground'] : self.x = self.x - self.speed

		# Impede o player de andar para fora da tela
		if self.y <= 0: self.y = self.y + self.speed
		if self.y >= (self.land.imageHeight*self.land.terrainHeight - self.land.imageHeight) - self.rect.height: self.y = self.y - self.speed
		if self.x <= 0: self.x = self.x + self.speed
		if self.x >= (self.land.imageWidth*self.land.terrainWidth - self.land.imageWidth) - self.rect.width: self.x = self.x - self.speed

		# Verifica se o player fez alguma ação
		if self.actingL == True or self.actingR == True: self.action()
	# END update()

	# Método desenhista
	def draw(self):
		# Desenha o personagem
		DisplaySurface.blit(self.image, self.camera.transform(self.x,self.y))
	# END draw()

	#=================================================#
	# Outras rotinas de desenho
	#=================================================#

	def drawHUD(self):
		# Desenha a HUD da Persona atual
		if self.isSelected == True:
			# Desenha a base da HUD
			DisplaySurface.blit(self.baseHud, (0,0))
			# Nome do Persona atual
			DisplaySurface.blit(BasicFont.render(self.name, 1, (139,87,58)), (52,8))
			# Classe
			DisplaySurface.blit(BasicFont.render(self.role, 1, (139,87,58)), (60,22))
			# HP, energia, fome e sede
			DisplaySurface.blit(BasicFont.render(str('%.0f' % self.hp), 1, (210,0,0)), (30,42))
			DisplaySurface.blit(BasicFont.render(str('%.0f' % self.stamina), 1, (255,204,0)), (30,59))
			DisplaySurface.blit(BasicFont.render(str('%.0f' % self.hidratation), 1, (102,153,255)), (82,41))
			DisplaySurface.blit(BasicFont.render(str('%.0f' % self.feeding), 1, (0,102,0)), (82,60))
			
			# Mão direita e Esquerda
			DisplaySurface.blit(BasicFont.render(self.left_hand.name, 1, (139,87,58)), (182,8))
			DisplaySurface.blit(BasicFont.render(self.right_hand.name, 1, (139,87,58)), (182,25))
	# END drawHUD()

	def showCrafting(self, positionChoice):
		# Abre a janela de crafting
		DisplaySurface.blit(self.baseHud_crafting, (300,200))
		# Essas variáveis são variáveis auxiliares para desenhar os textos nas posições corretas
		i = 0
		j = 0
		k = 0

		# Cria um dicionário que enumera as receitas
		# Não rela a mão, peguei da internet. Nem sei como funciona, mas no fim cada chave do nosso dicionário de receitas é associado a um número
		numericIndexes = {};
		for i,x in enumerate(self.recipeList): numericIndexes[i] = x

		# Desenha na tela as receitas
		for item in self.recipeList:
			k = k+1
			DisplaySurface.blit(BasicFont.render(self.recipeList[item].name, 1, (139,87,58)), (310, 235 + k*10))

		# Desenha o navegador da lista
		DisplaySurface.blit(self.listNavigator, (305, 237 + 10*positionChoice))

		# Desenha os itens necessários para a construção da receita
		for item in self.recipeList[numericIndexes[(positionChoice-1)]].requisites:
			j = j+1
			DisplaySurface.blit(BasicFont.render(item + ' x ' +str(self.recipeList[numericIndexes[positionChoice-1]].requisites[item]), 1, (139,87,58)), (400, 235 + j*10))

		# Verifica se alguma receita foi escolhida
		if self.enter == True:
			self.craft(self.recipeList[numericIndexes[positionChoice-1]])
			self.enter = False
	# END showCrafting()

	def showBackpack(self, positionChoice):
		# Backpack
		DisplaySurface.blit(self.baseHud_backpack, (0,400))
		i = 0

		# Cria um dicionário que enumera os itens
		# Não rela a mão, peguei da internet. Nem sei como funciona, mas no fim cada chave do nosso dicionário de itens é associado a um número (como em um vetor)
		numericIndexesBackpack = {};
		for i,x in enumerate(self.backpack): numericIndexesBackpack[i] = x

		# Desenha o nome e a quantidade dos itens
		for item in self.backpack:
			i = i+12
			if isinstance(self.backpack[item], int): DisplaySurface.blit(BasicFont.render(item + ' x ' + str(self.backpack[item]), 1, (139,87,58)), (10,420 + i))
			elif hasattr(self.backpack[item], 'name') and self.backpack[item].name not in ('Nothing', 'Nothing '): DisplaySurface.blit(BasicFont.render(item + ' - ' + str(self.backpack[item].durability) +'%', 1, (139,87,58)), (10, 420 + i))
			# Trata de desenhar os Nothings
			else: DisplaySurface.blit(BasicFont.render(self.backpack[item].name, 1, (139,87,58)), (10,420 + i))

		# Desenha o navegador da lista
		if len(self.backpack) > 0: DisplaySurface.blit(self.listNavigator, (5, 435 + 12*(positionChoice-1)))

		# Verifica se algum equipamento foi escolhido
		if self.zPressed == True:
			# Se equipamento, equipa na mão esquerda (tecla Z)
			if len(self.backpack)>0 and hasattr(self.backpack[numericIndexesBackpack[positionChoice-1]], 'name'):
				equipAntL = self.left_hand
				self.left_hand = self.backpack[numericIndexesBackpack[positionChoice-1]]
				del self.backpack[numericIndexesBackpack[positionChoice-1]]
				self.backpack[equipAntL.name] = equipAntL
			self.zPressed = False

		if self.xPressed == True:
			# Se equipamento, equipa na mão direita (tecla X)
			if len(self.backpack)>0 and hasattr(self.backpack[numericIndexesBackpack[positionChoice-1]], 'name'):
				equipAntR = self.right_hand
				self.right_hand = self.backpack[numericIndexesBackpack[positionChoice-1]]
				del self.backpack[numericIndexesBackpack[positionChoice-1]]
				self.backpack[equipAntR.name] = equipAntR
			self.xPressed = False
	# END showBackpack()

	#=================================================#
	# Ações da Persona
	#=================================================#

	def action(self):
		# Pega a posição do player e converte em coordenadas em relação o grid de terreno
		posPlayerX = int((self.x + (self.rect.width/2))/self.land.imageWidth)
		posPlayerY = int((self.y + (self.rect.height/2))/self.land.imageHeight)

		# Gera a chave a ser buscada no dicionário que possui os elementos modificáveis 
		key = ''

		if self.facing == 'up': key = '[' + str(posPlayerX) +','+str(posPlayerY-1) + ']'
		if self.facing == 'down': key = '[' + str(posPlayerX) +','+str(posPlayerY+1) + ']'
		if self.facing == 'left': key = '[' + str(posPlayerX-1) +','+str(posPlayerY) + ']'
		if self.facing == 'right': key = '[' + str(posPlayerX+1) +','+str(posPlayerY) + ']'

		# Verifica se o objeto requirido já foi instanciado anteriormente
		# No caso, só inicializa objetos do mapa que geram recursos, já que os outros objetos são inicializados por outras funções
		if key not in self.land.listInstanciedResources:
			# Caso o objeto não tenha sido instanciado, cria-o
			if self.facing == 'up':
				if self.land.terrain[posPlayerX][posPlayerY - 1] == self.land.terrainType['tree']: self.land.listInstanciedResources['['+str(posPlayerX)+','+str(posPlayerY-1)+']'] = Tree(posPlayerX, posPlayerY-1, self.camera)
				elif self.land.terrain[posPlayerX][posPlayerY - 1] == self.land.terrainType['stone']: self.land.listInstanciedResources['['+str(posPlayerX)+','+str(posPlayerY-1)+']'] = Stone(posPlayerX, posPlayerY-1, self.camera)
			if self.facing == 'down':
				if self.land.terrain[posPlayerX][posPlayerY + 1] == self.land.terrainType['tree']: self.land.listInstanciedResources['['+str(posPlayerX)+','+str(posPlayerY+1)+']'] = Tree(posPlayerX, posPlayerY+1, self.camera)
				elif self.land.terrain[posPlayerX][posPlayerY + 1] == self.land.terrainType['stone']: self.land.listInstanciedResources['['+str(posPlayerX)+','+str(posPlayerY+1)+']'] = Stone(posPlayerX, posPlayerY+1, self.camera)
			if self.facing == 'left':
				if self.land.terrain[posPlayerX - 1][posPlayerY] == self.land.terrainType['tree']: self.land.listInstanciedResources['['+str(posPlayerX-1)+','+str(posPlayerY)+']'] = Tree(posPlayerX-1, posPlayerY, self.camera)
				elif self.land.terrain[posPlayerX - 1][posPlayerY] == self.land.terrainType['stone']: self.land.listInstanciedResources['['+str(posPlayerX-1)+','+str(posPlayerY)+']'] = Stone(posPlayerX-1, posPlayerY, self.camera)
			if self.facing == 'right':
				if self.land.terrain[posPlayerX + 1][posPlayerY] == self.land.terrainType['tree']: self.land.listInstanciedResources['['+str(posPlayerX+1)+','+str(posPlayerY)+']'] = Tree(posPlayerX+1, posPlayerY, self.camera)
				elif self.land.terrain[posPlayerX + 1][posPlayerY] == self.land.terrainType['stone']: self.land.listInstanciedResources['['+str(posPlayerX+1)+','+str(posPlayerY)+']'] = Stone(posPlayerX+1, posPlayerY, self.camera)

		# Age sobre o objeto instanciado
		if key in self.land.listInstanciedResources and self.actingL == True and key != '': self.left_hand.act(self.land.listInstanciedResources[key])
		if key in self.land.listInstanciedResources and self.actingR == True and key != '': self.right_hand.act(self.land.listInstanciedResources[key])
	# END action()

	def craft(self, recipe):
		# Verifica o tipo de receita a ser criada
		if recipe.typeRecipe == 'item' or recipe.typeRecipe == 'equipment':
			# Primeiramente cria uma flag para verificar se temos todos os recursos necessários
			proceedWithCraft = False
			# Verifica se todos os componentes estão presentes
			for item in recipe.requisites:
				if (item in self.backpack) and (self.backpack[item] - recipe.requisites[item]) >= 0: proceedWithCraft = True
				else: 
					proceedWithCraft = False
					break
			# Caso todos os componentes estejam presentes nas quantidades certas, tira os recursos e cria o item
			if proceedWithCraft == True:
				for item in recipe.requisites:
					# Tira os recursos da backpack
					self.backpack[item] = self.backpack[item] - recipe.requisites[item]
				if recipe.typeRecipe == 'item':
					if recipe.name not in self.backpack: self.backpack[recipe.name] = 0
					self.backpack[recipe.name] = self.backpack[recipe.name] + recipe.giveItem(self)
				else:
					if recipe.name not in self.backpack: self.backpack[recipe.name] = Nothing(self, self.land, 'l')
					self.backpack[recipe.name] = recipe.giveItem(self, self.land)
		
		elif recipe.typeRecipe == 'construction':
			# Pega a posição do player e converte em coordenadas em relação o grid de terreno
			posPlayerX = int((self.x + (self.rect.width/2))/self.land.imageWidth)
			posPlayerY = int((self.y + (self.rect.height/2))/self.land.imageHeight)

			key = ''

			if self.facing == 'up' and self.land.terrain[posPlayerX][posPlayerY - 1] == self.land.terrainType['ground']: key = '['+str(posPlayerX)+','+str(posPlayerY-1)+']'
			if self.facing == 'down' and self.land.terrain[posPlayerX][posPlayerY + 1] == self.land.terrainType['ground']: key = '['+str(posPlayerX)+','+str(posPlayerY+1)+']'
			if self.facing == 'left' and self.land.terrain[posPlayerX - 1][posPlayerY] == self.land.terrainType['ground']: key = '['+str(posPlayerX-1)+','+str(posPlayerY)+']'
			if self.facing == 'right' and self.land.terrain[posPlayerX + 1][posPlayerY] == self.land.terrainType['ground']: key = '['+str(posPlayerX+1)+','+str(posPlayerY)+']'

			# Verifica se o objeto requirido já foi instanciado anteriormente
			if key != '' and key not in self.land.listInstanciedResources:
				# Caso o objeto não tenha sido instanciado, cria-o
				if self.facing == 'up':
					self.land.listInstanciedResources[key] = ConstructionSite(posPlayerX, posPlayerY-1, recipe) 
					self.land.terrain[posPlayerX][posPlayerY-1] = self.land.terrainType['constructionSite']
				if self.facing == 'down':
					self.land.listInstanciedResources[key] = ConstructionSite(posPlayerX, posPlayerY+1, recipe)
					self.land.terrain[posPlayerX][posPlayerY+1] = self.land.terrainType['constructionSite']
				if self.facing == 'left':
					self.land.listInstanciedResources[key] = ConstructionSite(posPlayerX-1, posPlayerY, recipe)
					self.land.terrain[posPlayerX-1][posPlayerY] = self.land.terrainType['constructionSite']
				if self.facing == 'right':
					self.land.listInstanciedResources[key] = ConstructionSite(posPlayerX+1, posPlayerY, recipe)
					self.land.terrain[posPlayerX+1][posPlayerY] = self.land.terrainType['constructionSite']
			self.craftingOpened = False
	# END craft()
# END Persona()
