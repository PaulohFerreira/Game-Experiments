# -*- coding: utf-8 -*-

# Módulo Game - 22/01/2015
# Módulo gerenciador do jogo

# Imports básicos
import pygame as pg, sys, random
from pygame.locals import *
# Imports de classes
from characters import *
from world import *
from camera import *

# Constantes básicas
FPS = 30
FPS_CLOCK = pg.time.Clock()
WIN_WIDTH = 800
WIN_HEIGTH = 600

# Método Main do jogo, essencialmente inicializa os elementos básicos e bota o runGame() pra funcionar
def main():
	global DisplaySurface, BasicFont, player, land, camera, player_counter
	
	# Inicialização dos objetos
	pg.init()
	DisplaySurface = pg.display.set_mode((WIN_WIDTH, WIN_HEIGTH))
	pg.display.set_caption('Generic Survival')
	pg.display.set_icon(pg.image.load('graphics/gameicon.png'))
	BasicFont = pg.font.Font(None, 18)
	camera = Camera()

	# Array de personagens
	player = []
	# Terreno
	land = Land(camera, FPS_CLOCK)

	# Spawna os personagens
	spawnPersonas(player, land, camera, 5)
	# Contador que guarda qual personagem está sendo usado no momento
	player_counter = 0
	# Muda a flag da HUD
	player[player_counter].isSelected = True
	# Seta o foco da câmera no primeiro Persona
	camera.set_focus(player[player_counter])

	# Loop de jogo principal
	while True:
		runGame()
# END main()

# Método onde a lógica do jogo é feita 
def runGame():
	global player_counter

	# Gerenciamento de entradas do usuário
	for event in pg.event.get():
		# Apertou algum botão
		if event.type == KEYDOWN:
			# Movimentação de Personagem
			if event.key == K_UP: player[player_counter].moveUp = True
			if event.key == K_DOWN: player[player_counter].moveDown = True
			if event.key == K_LEFT: player[player_counter].moveLeft = True
			if event.key == K_RIGHT: player[player_counter].moveRight = True
			if event.key == K_RETURN: player[player_counter].enter = True

			# Troca de personagem
			if event.key == K_TAB:
				# isSelected é uma flag de desenho da HUD
				player[player_counter].isSelected = False
				# Reseta flags de movimento
				player[player_counter].moveUp = False
				player[player_counter].moveDown = False
				player[player_counter].moveLeft = False
				player[player_counter].moveRight = False
				player[player_counter].backpackOpened = False
				player[player_counter].craftingOpened = False
				# Incrementa o índice do contador de players
				player_counter = (player_counter + 1)%len(player)
				# flag da HUD do novo Persona
				player[player_counter].isSelected = True
				# Joga a câmera para o próximo Persona
				camera.set_focus(player[player_counter])

			# Ação do player
			# Ação com a mão esquerda
			if event.key == K_z: 
				player[player_counter].actingL = True
				player[player_counter].zPressed = True
			# Ação com a mão direita
			elif event.key == K_x:
				player[player_counter].actingR = True
				player[player_counter].xPressed = True
			# Abrir a mochila
			if event.key == K_d: player[player_counter].backpackOpened =  not player[player_counter].backpackOpened
			# Abrir a janela de crafting
			if event.key == K_c: player[player_counter].craftingOpened = not player[player_counter].craftingOpened
				
		# Soltou algum botão
		elif event.type == KEYUP:
			# Reseta flags
			if event.key  == K_UP: player[player_counter].moveUp = False
			if event.key == K_DOWN: player[player_counter].moveDown = False
			if event.key == K_LEFT: player[player_counter].moveLeft = False
			if event.key == K_RIGHT: player[player_counter].moveRight = False
			if event.key == K_RETURN: player[player_counter].enter = False

			if event.key == K_z: player[player_counter].actingL = False
			if event.key == K_x: player[player_counter].actingR = False

		# Fecha o jogo
		elif event.type == QUIT:
			land.save()
			for p in range(len(player)):
				savePersona(player, p)
			pg.quit()
			sys.exit()

	# Rotinas de Update
	camera.update()
	for p in player:
		p.update()
	land.update()

	# Limpa a tela
	DisplaySurface.fill((0,0,0))
	# Rotinas de Draw
	land.draw(player[player_counter])
	for p in player:
		p.draw()

	# Desenha o ciclo dia - noite
	DisplaySurface.blit(land.imageDusk, land.camera.transform(player[player_counter].x - 420, player[player_counter].y - 320))
	
	# Desenha huds e menus quando necesário
	player[player_counter].drawHUD()

	# Verifica se a mochila foi aberta
	if player[player_counter].backpackOpened == True:
		# Navegador da lista
		if player[player_counter].moveUp == True and player[player_counter].positionChoiceBackpack > 1:
			player[player_counter].positionChoiceBackpack = player[player_counter].positionChoiceBackpack - 1
			player[player_counter].moveUp = False
		elif player[player_counter].moveDown == True and player[player_counter].positionChoiceBackpack < len(player[player_counter].backpack):
			player[player_counter].positionChoiceBackpack = player[player_counter].positionChoiceBackpack + 1
			player[player_counter].moveDown = False
		player[player_counter].showBackpack(player[player_counter].positionChoiceBackpack)

	# Verifica se o crafting foi aberto
	if player[player_counter].craftingOpened == True:
		# Navegador da lista
		if player[player_counter].moveUp == True and player[player_counter].positionChoiceCraft > 1:
			player[player_counter].positionChoiceCraft = player[player_counter].positionChoiceCraft - 1
			player[player_counter].moveUp = False
		elif player[player_counter].moveDown == True and player[player_counter].positionChoiceCraft < len(player[player_counter].recipeList):
			player[player_counter].positionChoiceCraft= player[player_counter].positionChoiceCraft + 1
			player[player_counter].moveDown = False
		player[player_counter].showCrafting(player[player_counter].positionChoiceCraft)

	# Mostra FPS
	DisplaySurface.blit(BasicFont.render('FPS: ' + str('%.0f' % FPS_CLOCK.get_fps()), 1, (0,0,0)), (730,0))
	
	# Atualização do frame
	FPS_CLOCK.tick(FPS)
	pg.display.update()
# END runGame()

#================================================#
#	Funções relacionadas a Personas
#================================================#
def createPersona(quant, sex, role, x, y):
	# Cria um arquivo onde serão guardados os dados de cada Persona
	if not os.path.exists('files/personas/'): os.mkdir('files/personas/')

	filePersona = open('files/personas/persona_' + str(quant) + '.prs', 'w')

	# Escreve nesse arquivo as informações dessa persona
	# Escreve o nome da Persona
	filePersona.write('Bala Juquinha')
	filePersona.write('\n')
	# Escreve o sexo da Persona
	filePersona.write(sex)
	filePersona.write('\n')
	# Escreve o role da Persona
	filePersona.write(role)
	filePersona.write('\n')
	# Escreve a posição x da Persona
	filePersona.write(str(x))
	filePersona.write('\n')
	# Escreve a posição y da Persona
	filePersona.write(str(y))
	filePersona.write('\n')

	# Escreve a posição segura em x da Persona (inicialmente a mesma do spawn)
	filePersona.write(str(x))
	filePersona.write('\n')
	# Escreve a posição segura em y da Persona (inicialmente a mesma do spawn)
	filePersona.write(str(y))
	filePersona.write('\n')

	# Escreve o HP da Persona
	filePersona.write('100')
	filePersona.write('\n')
	# Escreve a Stamina da Persona
	filePersona.write('100')
	filePersona.write('\n')
	# Escreve a Hungry da Persona
	filePersona.write('100')
	filePersona.write('\n')
	# Escreve a Hidratation da Persona
	filePersona.write('100')
	filePersona.write('\n')
	filePersona.write('==')
	filePersona.write('\n')

	# Escreve os itens equipados em mãos
	# Mão esquerda (L)
	filePersona.write('Nothing')
	filePersona.write('\n')
	# Mão direita (R)
	filePersona.write('Nothing')
	filePersona.write('\n')

	# Escreve os itens da backpack
	filePersona.write('0')
	filePersona.write('\n')
	filePersona.write('==')
	filePersona.write('\n')

	# Escreve a quantidade de receitas
	filePersona.write('4')
	filePersona.write('\n')
	
	# Escreve as receitas conhecidas
	filePersona.write('Axe')
	filePersona.write('\n')
	filePersona.write('Pickaxe')
	filePersona.write('\n')
	filePersona.write('Rope')
	filePersona.write('\n')
	filePersona.write('House')
	filePersona.write('\n')
	filePersona.write('Deposit')
	filePersona.write('\n')
	filePersona.write('==')
	filePersona.write('\n')

	# Escreve o número de comportamentos
	filePersona.write('0')
	filePersona.write('\n')

	# Escreve os comportamentos conhecidos
	filePersona.write('==')

	filePersona.close()
# END createPersonas()

def savePersona(playerArray, playerIdentificator):

	filePersona = open('files/personas/persona_' + str(playerIdentificator) + '.prs', 'w')

	# Escreve nesse arquivo as informações dessa persona
	# Escreve o nome da Persona
	filePersona.write(playerArray[playerIdentificator].name)
	filePersona.write('\n')
	# Escreve o sexo da Persona
	filePersona.write(playerArray[playerIdentificator].sex)
	filePersona.write('\n')
	# Escreve o role da Persona
	filePersona.write(playerArray[playerIdentificator].role)
	filePersona.write('\n')
	# Escreve a posição x da Persona
	filePersona.write(str(playerArray[playerIdentificator].x))
	filePersona.write('\n')
	# Escreve a posição y da Persona
	filePersona.write(str(playerArray[playerIdentificator].y))
	filePersona.write('\n')

	# Escreve a posição segura em x da Persona (inicialmente a mesma do spawn)
	filePersona.write(str(playerArray[playerIdentificator].safeX))
	filePersona.write('\n')
	# Escreve a posição segura em y da Persona (inicialmente a mesma do spawn)
	filePersona.write(str(playerArray[playerIdentificator].safeY))
	filePersona.write('\n')

	# Escreve o HP da Persona
	filePersona.write(str(playerArray[playerIdentificator].hp))
	filePersona.write('\n')
	# Escreve a Stamina da Persona
	filePersona.write(str(playerArray[playerIdentificator].stamina))
	filePersona.write('\n')
	# Escreve a Hungry da Persona
	filePersona.write(str(playerArray[playerIdentificator].feeding))
	filePersona.write('\n')
	# Escreve a Hidratation da Persona
	filePersona.write(str(playerArray[playerIdentificator].hidratation))
	filePersona.write('\n')
	filePersona.write('==')
	filePersona.write('\n')

	# Escreve os itens equipados em mãos
	# Mão esquerda (L)
	if hasattr(playerArray[playerIdentificator].left_hand, 'name'):
			if playerArray[playerIdentificator].left_hand.name not in ('Nothing', 'Nothing '):
				filePersona.write(playerArray[playerIdentificator].left_hand.name +','+str(playerArray[playerIdentificator].left_hand.durability))
				filePersona.write('\n')
			else:
				filePersona.write(playerArray[playerIdentificator].left_hand.name)
				filePersona.write('\n')

	# Mão direita (R)
	if hasattr(playerArray[playerIdentificator].right_hand, 'name'):
			if playerArray[playerIdentificator].right_hand.name not in ('Nothing', 'Nothing '):
				filePersona.write(playerArray[playerIdentificator].right_hand.name +','+str(playerArray[playerIdentificator].right_hand.durability))
				filePersona.write('\n')
			else:
				filePersona.write(playerArray[playerIdentificator].right_hand.name)
				filePersona.write('\n')

	# Escreve os itens da backpack
	# Primeiro escreve o número de elementos na backpack
	filePersona.write(str(len(playerArray[playerIdentificator].backpack)))
	filePersona.write('\n')

	# Escreve os itens da backpack
	for element in playerArray[playerIdentificator].backpack:
		if hasattr(playerArray[playerIdentificator].backpack[element], 'name'):
			if element not in ('Nothing', 'Nothing '):
				filePersona.write(element +','+str(playerArray[playerIdentificator].backpack[element].durability))
				filePersona.write('\n')
			else:
				filePersona.write(element)
				filePersona.write('\n')
		else:	
			filePersona.write(element +','+ str(playerArray[playerIdentificator].backpack[element]))
			filePersona.write('\n')

	filePersona.write('==')
	filePersona.write('\n')

	# Escreve o número de receitas
	filePersona.write(str(len(playerArray[playerIdentificator].recipeList)))
	filePersona.write('\n')
	
	# Escreve as receitas conhecidas
	for recipe in playerArray[playerIdentificator].recipeList:
		filePersona.write(recipe)
		filePersona.write('\n')

	filePersona.write('==')
	filePersona.write('\n')

	# Escreve o número de receitas
	filePersona.write(str(len(playerArray[playerIdentificator].behaviours)))
	filePersona.write('\n')
	# Escreve os comportamentos conhecidos
	filePersona.write('==')

	filePersona.close()
# END savePersona()

def loadPersona(playerArray, playerFile, land, camera):
	# Salva todos os dados do arquivo
	name = playerFile.readline().rstrip('\n')
	sex = playerFile.readline().rstrip('\n')
	role = playerFile.readline().rstrip('\n')
	x = int(playerFile.readline().rstrip('\n'))
	y = int(playerFile.readline().rstrip('\n'))

	safeX = int(playerFile.readline())
	safeY = int(playerFile.readline())

	persona = Persona(name, sex, role, x, y, camera, land)

	persona.hp = int(playerFile.readline().rstrip('\n'))
	persona.stamina = int(playerFile.readline().rstrip('\n'))
	persona.feeding = int(playerFile.readline().rstrip('\n'))
	persona.hidratation = int(playerFile.readline().rstrip('\n'))

	# Pula a linha '=='
	playerFile.readline()

	lhName = playerFile.readline().rstrip('\n').split(',')
	# lhName[0] = nome do item
	# lhName[1] = durabilidade do item (quando existir)

	if len(lhName) == 2:
		if lhName[0] == 'Axe': persona.left_hand = Axe(persona, persona.land, lhName[1])
		elif lhName[0] == 'Pickaxe': persona.left_hand = Pickaxe(persona, persona.land, lhName[1])
	else:
		if lhName == 'Nothing': persona.left_hand = Nothing(persona, persona.land, 'l')
		elif lhName == 'Nothing ': persona.left_hand = Nothing(persona, persona.land, 'r')

	rhName = playerFile.readline().rstrip('\n').split(',')
	# rhName[0] = nome do item
	# rhName[1] = durabilidade do item (quando existir)

	if len(rhName) == 2:
		if rhName[0] == 'Axe': persona.right_hand = Axe(persona, persona.land, rhName[1])
		elif rhName[0] == 'Pickaxe': persona.right_hand = Pickaxe(persona, persona.land, rhName[1])
	else:
		if rhName == 'Nothing': persona.right_hand = Nothing(persona, persona.land, 'l')
		elif rhName == 'Nothing ': persona.right_hand = Nothing(persona, persona.land, 'r')

	# Lê a quantidade de itens
	qItens = playerFile.readline().rstrip('\n')

	# Lê os itens da backpack
	for i in range(int(qItens)):
		item = playerFile.readline().rstrip('\n').split(',')
		
		if len(item) == 2:
			if item[0] in ('Stick','Fiber','Rock', 'Rope', 'Wood', 'Stone'): persona.backpack[item[0]] = int(item[1])
			elif item[0] == 'Axe': persona.backpack[item[0]] = Axe(persona, persona.land, item[1])
			elif item[0] == 'Pickaxe': persona.backpack[item[0]] = Pickaxe(persona, persona.land, item[1])

		else:
			if item[0] == 'Nothing': persona.backpack[item[0]] = Nothing(persona, persona.land, 'l')
			elif item[0] == 'Nothing ': persona.backpack[item[0]] = Nothing(persona, persona.land, 'r')

	# Pula a linha '=='
	playerFile.readline()

	playerArray.append(persona)
# END loadPersonas()

def spawnPersonas(playerArray, land, camera, quantPlayers):
	x = 0
	y = 0

	# Verifica se já existem personas geradas
	if not os.path.exists('files/personas/'):
		# Escolhe um lugar aleatório do mapa para criar o spawner
		spawnPointFinded = False
		spawnPointOk = False

		while spawnPointFinded == False:
			x = random.randrange(200,1000)
			y = random.randrange(200,1000)

			for i in range(x-3,x+3):
				for j in range(y-3,y+3):
					if land.terrain[i][j] == land.terrainType['ground']: spawnPointOk = True
					else:
						spawnPointOk = False
						break
				if spawnPointOk == False: break

			if spawnPointOk == True: spawnPointFinded = True
			else: spawnPointFinded = False

		# Após encontrar o ponto, cria a fogueira e os players
		land.terrain[int(x)][int(y)] = land.terrainType['firepit']

		for q in range(quantPlayers): createPersona(q, 'm', 'Peasant', (int(x) - quantPlayers + 2 + q%3)*40, (int(y) + q%2)*40)

	# Carrega personas
	for x in range(quantPlayers):
		filePlayer = open('files/personas/persona_'+ str(x) +'.prs', 'r')
		loadPersona(playerArray, filePlayer, land, camera)
# END spawnPersonas()

# Faz com que o método main() rode ao iniciar o programa
if __name__ == '__main__': main()
