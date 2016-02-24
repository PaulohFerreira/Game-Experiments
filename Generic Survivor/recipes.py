# -*- coding: utf-8 -*-

# Módulo Recipes - 21/02/2015
# Módulo que contém as receitas para crafting

from equipments import *

class Recipe(object):
	def __init__(self, name):
		self.name = name
		# Tipos de receita: 'item' - Itens; 'equipment' - ferramentas; 'construction' - construções
		self.requisites = {};

		#self.requisites[''] =
		# ============= Recursos secundários ================= #
		if self.name == 'Rope':
			self.typeRecipe = 'item'
			self.requisites['Fiber'] = 3

		# ============= Ferramentas ================= # 
		elif self.name == 'Axe':
			self.typeRecipe = 'equipment'
			self.requisites['Stick'] = 1
			#self.requisites['Fiber'] = 2
			#self.requisites['Rock'] = 1

		elif self.name == 'Pickaxe':
			self.typeRecipe = 'equipment'
			self.requisites['Stick'] = 1
			#self.requisites['Fiber'] = 2
			#self.requisites['Rock'] = 2


		# ============= Construções ================= #
		elif self.name == 'House':
			self.typeRecipe = 'construction'
			self.requisites['Wood'] = 1#10
			#self.requisites['Rope'] = 5
			#self.requisites['Stone'] = 5

		elif self.name == 'Deposit':
			self.typeRecipe = 'construction'
			self.requisites['Wood'] = 1#10
			#self.requisites['Rope'] = 5
			#self.requisites['Stone'] = 5
	# END init()

	def giveItem(self, target, land):
		if self.name == 'Rope': return 1
		elif self.name == 'Axe': return Axe(target, land, 100)
		elif self.name == 'Pickaxe': return Pickaxe(target, land, 100)
	# END giveItem()
# END Recipes
