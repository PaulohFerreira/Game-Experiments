# -*- coding: utf-8 -*-

# Módulo World - 25/01/2015
# Módulo que contém o criador e gerenciador de mapas

import pygame as pg, sys, random, os, math
from pygame.locals import *

class Land(object):

	def __init__(self, camera, time):
		# Torna variáveis visíveis as outras funções
		global DisplaySurface

		# Informações da superfície de desenho
		DisplaySurface = pg.display.get_surface()
		self.width, self.height = DisplaySurface.get_size()
		
		# Sistema de dia-noite
		self.dayTimeMin = 10
		self.duskTimeMin = 2
		self.nightTimeMin = 10

		self.timeCounter = 0 
		self.duskCounter1 = 0
		self.duskCounter2 = 0
		self.imageDusk = pg.image.load('graphics/world/dusk.png').convert()

		self.imageLowLight = pg.image.load('graphics/world/lowLight.png').convert().set_alpha(120)
		self.imageMediumLight = pg.image.load('graphics/world/mediumLight.png').convert().set_alpha(120)
		self.imageHighLight = pg.image.load('graphics/world/highLight.png').convert().set_alpha(120)

		# Dicionário com informações dos bloco (Tipo do bloco e seu código na matriz)
		self.terrainType = {
		'void' : 0,
		'ground' : 1,
		'tree' : 2,
		'stone' : 3,
		'water' : 4,
		# Constuções
		'constructionSite' : 50,
		'house' : 51,
		'deposit' : 52,
		'firepit': 99
		};

		# Imagens das texturas
		self.imageGround = pg.image.load('graphics/world/ground.png')
		self.imageTree = pg.image.load('graphics/world/tree.png')
		self.imageStone = pg.image.load('graphics/world/stone.png')
		self.imageWater = pg.image.load('graphics/world/water.png')
		self.imageFirepit = pg.image.load('graphics/world/firepit.png')

		self.imageConstructionSite = pg.image.load('graphics/constructions/construction_site.png')
		self.imageHouse = pg.image.load('graphics/constructions/house.png')
		self.imageDeposit = pg.image.load('graphics/constructions/deposit.png')

		# Lista de objetos construídos
		# Dicionário de objetos instanciados no mapa
		self.listInstanciedResources = {};
		
		# Dimensões do terreno gerado (em blocos)
		# No caso colocamos x + 1 porquê esse 1 bloco a mais não é desenhável, servindo apenas para que o algoritmo de colisão não dê erro ao verificar a última linha e coluna do mapa 
		self.terrainWidth = 1200 + 1
		self.terrainHeight = 1200 + 1
		
		self.imageWidth = 40
		self.imageHeight = 40

		# Informações do jogador
		self.camera = camera

		# Cria a matriz de elementos do mapa e a inicializa
		self.terrain = [[self.terrainType['ground'] for x in range(self.terrainWidth)] for x in range(self.terrainHeight)]

		# Zera a linha e coluna extra da matriz para torna-las não desenháveis
		for x in range(self.terrainWidth):
			self.terrain[self.terrainWidth-1][x] = self.terrainType['void']
			self.terrain[x][self.terrainHeight-1] = self.terrainType['void']
		self.terrain[self.terrainWidth-1][self.terrainHeight-1] = self.terrainType['void']

		# Verifica se já existe um terreno gerado
		if os.path.exists('files/world.land'):
			# Abre o mapa existente
			fileTerrain = open('files/world.land', 'r')
			# Joga o mapa do arquivo pra array de mapa
			for x in range(self.terrainWidth):
				# Ignora as vírgulas na hora da leitura do mapa
				lineSplited = fileTerrain.readline().split(',')
				# Passa os caracteres para as posições do vetor
				self.terrain[x] = list(lineSplited)
				# Tira o \n da lista
				del self.terrain[x][-1]
				# Converte os chars em ints
				self.terrain[x] = list(map(int, self.terrain[x]))

		else:
			# Cria um arquivo para guardar o mapa a ser criado
			fileTerrain = open('files/world.land', 'w')

			# Gerador de florestas
			# 1- Cria pivôs
			for x in range (0,10): self.terrain[random.randrange(0, self.terrainWidth-2)][random.randrange(0, self.terrainHeight-2)] = self.terrainType['tree']

			# 2 - Expande a floresta a partir dos pivôs
			for k in range(3):
				for x in range(1,self.terrainWidth-2):
					for y in range(1,self.terrainHeight-2):
							# Definindo a chance de se gerar árvores baseando-se em seu redor
							chance = 5
							if self.terrain[x-1][y-1] == self.terrainType['tree']: chance = chance + 5
							if self.terrain[x-1][y] == self.terrainType['tree']: chance = chance + 5
							if self.terrain[x-1][y+1] == self.terrainType['tree']: chance = chance + 5
							if self.terrain[x][y-1] == self.terrainType['tree']: chance = chance + 5
							if self.terrain[x][y+1] == self.terrainType['tree']: chance = chance + 5
							if self.terrain[x+1][y-1] == self.terrainType['tree']: chance = chance + 5
							if self.terrain[x+1][y] == self.terrainType['tree']: chance = chance + 5
							if self.terrain[x+1][y+1] == self.terrainType['tree']: chance = chance + 5

							# Define se o bloco será uma árvore
							if random.randrange(0,100) < chance: self.terrain[x][y] = self.terrainType['tree']
			# 3 - Sucesso! \o/

			# Gerador de Corpos de água
			# 1 - Gera pivôs aleaóreos
			for x in range (0, 12*self.terrainWidth):self.terrain[random.randrange(2, self.terrainWidth-2)][random.randrange(2, self.terrainHeight-2)] = self.terrainType['water']

			# 2 - Após encontrar os locias possíveis, marca o redor desses blocos
			for x in range(1,self.terrainWidth-2):
				for y in range(1,self.terrainHeight-2):
						if self.terrain[x][y] == self.terrainType['water']:
								if self.terrain[x-1][y-1] != self.terrainType['water']: self.terrain[x-1][y-1] = 9
								if self.terrain[x-1][y-1] != self.terrainType['water']: self.terrain[x-1][y] = 9
								if self.terrain[x-1][y-1] != self.terrainType['water']: self.terrain[x-1][y+1] = 9
								if self.terrain[x-1][y-1] != self.terrainType['water']: self.terrain[x][y-1] = 9
								if self.terrain[x-1][y-1] != self.terrainType['water']: self.terrain[x][y+1] = 9
								if self.terrain[x-1][y-1] != self.terrainType['water']: self.terrain[x+1][y-1] = 9
								if self.terrain[x-1][y-1] != self.terrainType['water']: self.terrain[x+1][y] = 9
								if self.terrain[x-1][y-1] != self.terrainType['water']: self.terrain[x+1][y+1] = 9

			# 3 - Substitui os 9 por blocos de água de verdade, e liga corpos de água quando possível
			for x in range(1,self.terrainWidth-2):
				for y in range(1,self.terrainHeight-2):
						if self.terrain[x][y] == 9: self.terrain[x][y] = self.terrainType['water']
						if self.terrain[x][y] == self.terrainType['water']:
							if self.terrain[x-2][y-2] == self.terrainType['water']: self.terrain[x-1][y-1] = self.terrainType['water']
							if self.terrain[x-2][y] == self.terrainType['water']: self.terrain[x-1][y] = self.terrainType['water']
							if self.terrain[x-2][y+2] == self.terrainType['water']:  self.terrain[x-1][y+1] = self.terrainType['water']
							if self.terrain[x][y-2] == self.terrainType['water']: self.terrain[x][y-1] = self.terrainType['water']
							if self.terrain[x][y+2] == self.terrainType['water']:  self.terrain[x][y+1] = self.terrainType['water']
							if self.terrain[x+2][y-2] == self.terrainType['water']: self.terrain[x+1][y-1] = self.terrainType['water']
							if self.terrain[x+2][y] == self.terrainType['water']:  self.terrain[x+1][y] == self.terrainType['water']
							if self.terrain[x+2][y+2] == self.terrainType['water']: self.terrain[x+1][y+1] = self.terrainType['water']


			# Teste de sanidade para verificar se algum bloco ficou sem preenchimento
			for x in range(self.terrainWidth):
				if self.terrain[self.terrainWidth-2][x] == 9: self.terrain[self.terrainWidth-2][x] = self.terrainType['water']

			# 4 - Sucesso! \o/


			# Gerador de pedrarias (? - Coltivo de pedra)
			# 1 - Gerar pivôs
			for x in range (0, 30*self.terrainWidth): self.terrain[random.randrange(1, self.terrainWidth-2)][random.randrange(1, self.terrainHeight-2)] = self.terrainType['stone']

			# 2 - Expande as pedras a partir dos pivôs
			for x in range(1,self.terrainWidth-2):
				for y in range(1,self.terrainHeight-2):
						if self.terrain[x][y] == self.terrainType['stone']:
								if random.randrange(0,100) < 20: self.terrain[x-1][y-1] = self.terrainType['stone']
								if random.randrange(0,100) < 20: self.terrain[x-1][y] = self.terrainType['stone']
								if random.randrange(0,100) < 20: self.terrain[x-1][y+1] = self.terrainType['stone']
								if random.randrange(0,100) < 20: self.terrain[x][y-1] = self.terrainType['stone']
								if random.randrange(0,100) < 20: self.terrain[x][y+1] = self.terrainType['stone']
								if random.randrange(0,100) < 20: self.terrain[x+1][y-1] = self.terrainType['stone']
								if random.randrange(0,100) < 20: self.terrain[x+1][y] = self.terrainType['stone']
								if random.randrange(0,100) < 20: self.terrain[x+1][y+1] = self.terrainType['stone']
			
			# 3 - Tapa os buracos deixados pelo for anterior
			for x in range(1,self.terrainWidth-2):
					for y in range(1,self.terrainHeight-2):
						quant = 0
						if self.terrain[x-1][y-1] == self.terrainType['stone']: quant = quant + 1
						if self.terrain[x-1][y] == self.terrainType['stone']: quant = quant + 1
						if self.terrain[x-1][y+1] == self.terrainType['stone']: quant = quant + 1
						if self.terrain[x][y-1] == self.terrainType['stone']: quant = quant + 1
						if self.terrain[x][y+1] == self.terrainType['stone']: quant = quant + 1
						if self.terrain[x+1][y-1] == self.terrainType['stone']: quant = quant + 1
						if self.terrain[x+1][y] == self.terrainType['stone']: quant = quant + 1
						if self.terrain[x+1][y+1] == self.terrainType['stone']: quant = quant + 1

						# Define se o bloco será uma pedra
						if quant >= 4: self.terrain[x][y] = self.terrainType['stone']
			# 4 - Sucesso! \o/

			# No final, escreve a array no arquivo
			for x in range(self.terrainWidth):
				for y in range(self.terrainHeight):
					fileTerrain.write(str(self.terrain[x][y]) + ',')
				fileTerrain.write('\n')

		# E por fim, fecha o arquivo criado
		fileTerrain.close()
	# END init()

	def update(self):
		# Atualiza o ciclo dia-noite
		# Período de dia (10 minutos)
		if self.timeCounter <= self.dayTimeMin*1800:
			self.imageDusk.set_alpha(0)
		# Período de penumbra dia-noite (2 minutos)
		elif self.timeCounter > self.dayTimeMin*1800 and self.timeCounter <= (self.dayTimeMin + self.duskTimeMin)*1800 :
			self.imageDusk.set_alpha(100 + (self.duskCounter1/30))
			self.duskCounter1 = self.duskCounter1 + 1
		# Período de noite (10 minutos)
		elif self.timeCounter > (self.dayTimeMin + self.duskTimeMin)*1800 and self.timeCounter <= (self.dayTimeMin + self.duskTimeMin + self.nightTimeMin)*1800: 
			self.imageDusk.set_alpha(230)
		# Período de penumbra noite-dia (2 minutos)
		elif  self.timeCounter > (self.dayTimeMin + self.duskTimeMin + self.nightTimeMin)*1800 and self.timeCounter <= (self.dayTimeMin + self.duskTimeMin + self.nightTimeMin + self.duskTimeMin )*1800:
			self.imageDusk.set_alpha(240 - (self.duskCounter2/30))
			self.duskCounter2 = self.duskCounter2 + 1
		else:
			self.timeCounter = 0
			self.duskCounter1 = 0
			self.duskCounter2 = 0 
		
		self.timeCounter = self.timeCounter + 1

		auxList = []
		# Atualiza os blocos de recurso de acordo com o tempo
		for key in self.listInstanciedResources:
			if hasattr(self.listInstanciedResources[key], 'life') and self.listInstanciedResources[key].life <= 0:
				self.listInstanciedResources[key].playerActing = False
				self.terrain[self.listInstanciedResources[key].x][self.listInstanciedResources[key].y] = 1
				auxList.append('[' + str(self.listInstanciedResources[key].x)+','+str(self.listInstanciedResources[key].y) + ']')

		for i in auxList: del self.listInstanciedResources[i]
	# END update()

	def draw(self, player):
		# Calcula a distância de renderização, para não desenhar partes desnecessárias da matriz
		rangeXIni = int((player.x - 1.1*self.width)/self.imageWidth)
		rangeXOut = int((player.x + 1.1*self.width)/self.imageWidth)
		rangeYIni = int((player.y - 1.1*self.height)/self.imageHeight)
		rangeYOut = int((player.y + 1.1*self.height)/self.imageHeight)

		# Verifica se um valor inconsistente foi gerado para acessar a matriz
		if rangeXIni <= 0: rangeXIni = 0
		if rangeYIni <= 0: rangeYIni = 0
		if rangeXOut >= self.terrainWidth: rangeXOut = self.terrainWidth
		if rangeYOut >= self.terrainHeight: rangeYOut = self.terrainHeight

		for x in range(rangeXIni, rangeXOut):
			for y in range(rangeYIni, rangeYOut):
					if self.terrain[x][y] == self.terrainType['ground']: DisplaySurface.blit(self.imageGround, self.camera.transform(self.imageWidth*x, self.imageHeight*y))
					elif self.terrain[x][y] == self.terrainType['tree']: DisplaySurface.blit(self.imageTree, self.camera.transform(self.imageWidth*x, self.imageHeight*y))
					elif self.terrain[x][y] == self.terrainType['stone']: DisplaySurface.blit(self.imageStone, self.camera.transform(self.imageWidth*x, self.imageHeight*y))
					elif self.terrain[x][y] == self.terrainType['water']: DisplaySurface.blit(self.imageWater, self.camera.transform(self.imageWidth*x, self.imageHeight*y))
					elif self.terrain[x][y] == self.terrainType['firepit']: DisplaySurface.blit(self.imageFirepit, self.camera.transform(self.imageWidth*x, self.imageHeight*y))
					elif self.terrain[x][y] == self.terrainType['constructionSite']: DisplaySurface.blit(self.imageConstructionSite, self.camera.transform(self.imageWidth*x, self.imageHeight*y))
					elif self.terrain[x][y] == self.terrainType['house']: DisplaySurface.blit(self.imageHouse, self.camera.transform(self.imageWidth*x, self.imageHeight*y))
					elif self.terrain[x][y] == self.terrainType['deposit']: DisplaySurface.blit(self.imageDeposit, self.camera.transform(self.imageWidth*x, self.imageHeight*y))

		# Desenha informações dos blocos instanciados e objetos construídos
		for key in self.listInstanciedResources:
			#if self.listInstanciedResources[key].type in ('Construction Site', 'House', 'Deposit') and self.listInstanciedResources[key].x in range(rangeXIni, rangeXOut) and self.listInstanciedResources[key].y in range(rangeYIni, rangeYOut): DisplaySurface.blit(self.listInstanciedResources[key].image, self.camera.transform(self.listInstanciedResources[key].x*40,self.listInstanciedResources[key].y*40))
			if self.listInstanciedResources[key].type in ('Tree', 'Stone', 'Bush') and self.listInstanciedResources[key].playerActing == True: self.listInstanciedResources[key].drawInfo()
	# END draw()

	def save(self):
		fileTerrain = open("files/world.land", "w")

		for x in range(self.terrainWidth):
			for y in range(self.terrainHeight):
				fileTerrain.write(str(self.terrain[x][y])+',')
			fileTerrain.write('\n')
		
		fileTerrain.close()
	# END save()
# END Land()

#==================================================#
# Objetos do mapa
#==================================================#

class Tree(object):
	def __init__(self, x, y, camera):
		self.type = 'Tree'
		self.x = x
		self.y = y
		self.camera = camera

		self.playerActing = False
		# Imagens da hud dos blocos
		self.hudBase = pg.image.load('graphics/world/hudBase.png')
		self.hudDot = pg.image.load('graphics/world/hudDot.png')

		self.life = 20
		self.maxLife = 20

		# Estados: 'full' = Com galhos e cipós; 'depleted' = Sem galhos e cipós; 'on_ground' = Recursos no chão
		self.state = 'full'
	# END init()

	def drawInfo(self):
		DisplaySurface.blit(self.hudBase, self.camera.transform((self.x*40) + 2,(self.y*40 - 6)))
		# Desenha a barra de vida
		for i in range(6):
			if self.life >= (6-i)*(self.maxLife/6):
				# Posição de desenho = (posição base da hud + posição do dot na hud) + correções se necessárias
				DisplaySurface.blit(self.hudDot, self.camera.transform((self.x*40  + 2 + 6*(6-i)) + (6-i) - 6,(self.y*40 - 6)+1))
	# END drawInfo()
# END Tree()

class Stone(object):
	def __init__(self, x, y, camera):
		self.type = 'Stone'
		self.x = x
		self.y = y
		self.camera = camera

		self.playerActing = False
		# Imagens da hud dos blocos
		self.hudBase = pg.image.load('graphics/world/hudBase.png')
		self.hudDot = pg.image.load('graphics/world/hudDot.png')

		# Estados: 'full' = Com pedregulho; 'depleted' = Sem nada
		self.state = 'full'

		self.life = 35
		self.maxLife = 35
	# END init()

	def drawInfo(self):
		DisplaySurface.blit(self.hudBase, self.camera.transform((self.x*40) + 2,(self.y*40 - 6)))
		# Desenha a barra de vida
		for i in range(6):
			if self.life >= (6-i)*(self.maxLife/6):
				# Posição de desenho = (posição base da hud + posição do dot na hud) + correções se necessárias
				DisplaySurface.blit(self.hudDot, self.camera.transform((self.x*40  + 2 + 6*(6-i)) + (6-i) - 6,(self.y*40 - 6)+1))
	# END drawInfo()
# END Stone()
		
class Bush(object):
	def __init__(self, x, y, camera):
		self.type = 'Bush'
		self.x = x
		self.y = y
		self.camera = camera

		self.playerActing = False
		# Imagens da hud dos blocos
		self.hudBase = pg.image.load('graphics/world/hudBase.png')
		self.hudDot = pg.image.load('graphics/world/hudDot.png')
		
		self.life = 15
		self.maxLife = 15

		# Estados: 'full' = Com ervas e/ou frutas e/ou sementes; 'depleted' = Sem nada
		self.state = 'full'
	# END init()

	def drawInfo(self):
		DisplaySurface.blit(self.hudBase, self.camera.transform((self.x*40) + 2,(self.y*40 - 6)))
		# Desenha a barra de vida
		for i in range(6):
			if self.life >= (6-i)*(self.maxLife/6):
				# Posição de desenho = (posição base da hud + posição do dot na hud) + correções se necessárias
				DisplaySurface.blit(self.hudDot, self.camera.transform((self.x*40  + 2 + 6*(6-i)) + (6-i) - 6,(self.y*40 - 6)+1))
	# END drawInfo()
# END Bush()

#=================================================#
# Outros objetos (vivos)
#=================================================#
