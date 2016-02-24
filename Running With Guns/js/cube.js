// cube.js
// Client-side

// ================== CLIENT-SIDE EURECA.IO ================== //
var ready = false; // Flag de verificação para conexão
var eurecaServer; // Guarda uma referência ao servidor
var uniqueId; // Guarda a id única de identificação para uso do servidor
var brawlersList = {}; // Guarda a lista de jogadores atuais
var kills = 0; // Guarda a quantidade de kills

var updateCounter = 0; // Guarda a quantidade de vezes que o update foi chamado

// Essa função vai manipular a conexão cliente-servidor
var eurecaClientSetup = function(){
	var eurecaClient = new Eureca.Client(); // Cria uma instância do cliente de eureca.io
	
	// Função que faz a conexão cliente-servidor
	eurecaClient.ready(function (proxy){ eurecaServer = proxy; });

	// Função que seta a id única ao cliente e chama a função create() do phaser
	eurecaClient.exports.setId = function(id, arena){
		uniqueId = id;
		create(arena); // A função create fica aqui para garantir que o create só será chamado se a conexão estiver feita
		eurecaServer.setInfo(player.avatarName, player.gun.name, player.avatarStyle, player.avatar.x); // Dá informações para o servidor
       	eurecaServer.handshake(); // Termina a configuração da conexão
       	ready = true;
	}

	// Função que mata jogadores
	eurecaClient.exports.kill = function(id){
		if (brawlersList[id]){
			if(id == uniqueId){
				brawlersList[id].kill();
				delete brawlersList[id];
			}
			else{
				console.log("Killing ", id, brawlersList[id]);
				brawlersList[id].kill();
				delete brawlersList[id];
				console.log("Done");
			}
		}
	}
	
	// Função que respawna o jogadores depois de mortos
	eurecaClient.exports.respawn = function(id, name, weapon, style, spawnX){
		if(id == uniqueId){
			control = null;
			hud = null;

			player = new Brawler(id, room, name, weapon, style, spawnX, 60); //Cria o player
			hud = new HUD(player);
			control = new Input(player); // Cria os controles

			// Adiciona o player à lista de jogadores local
			brawlersList[id] = player;

			// Faz a câmera seguir o player
			cube.camera.follow(player.avatar);
		}
		else{
			console.log("Spawning");
			var brw = new Brawler(id, room, name, weapon, style, spawnX, 60);
			brawlersList[id] = brw;
		}
	}

	// Função que gera jogadores na tela
	eurecaClient.exports.spawnEnemy = function(id, name, weapon, style, x, y){
		if (id == uniqueId) return; // Não spawna você mesmo no seu jogo
		
		console.log("Spawning");
		var brw = new Brawler(id, room, name, weapon, style, x, y);
		brawlersList[id] = brw;
	}

	// Função que faz a atualização das entradas 
	eurecaClient.exports.updateState = function(id, state){
		if (brawlersList[id]){
			brawlersList[id].inputsFromServer = state;
			brawlersList[id].update();
		}
	}

	// Função que anuncia as mortes
	eurecaClient.exports.announce = function(type, char1, char2){
		if(type == "kill"){
			var announce = cube.add.text(400, 150, char1+" killed "+char2, { font: "36px Courier New", fill: "#FF0000", align: "center"});
			announce.fixedToCamera = true;
			cube.time.events.add(3000, function(){ announce.kill(); }, this);
		}
		else if(type == "win"){
			var announce = cube.add.text(50, 150, "GAME OVER\nWinner: "+char1+"\nPlease restart the page for init a new game.", { font: "36px Courier New", fill: "#0000AA", align: "center"});
			announce.fixedToCamera = true;
			player.kill();			
		}
	}
}
// =========================================================== //

// ========================== PHASER ========================= //
function initGame(){
	// Esconde a form de seleção de char 
	document.getElementById("divCharCreation").style.display = "none";
	document.getElementById("info").style.display = "none";

	// Inicia o jogo
	cube = new Phaser.Game(1024, 768, Phaser.AUTO, "hypercube_online", { preload: preload, create: eurecaClientSetup, update: update, render: render});
}

function preload(){
	cube.stage.backgroundColor = '#000000';
	cube.scale.scaleMode = Phaser.ScaleManager.NO_SCLAE;

	// Centraliza o jogo na tela
	cube.scale.pageAlignHorizontally = true;
	cube.scale.pageAlignVertically = true;

	// Escolhe o sistema de física
	cube.physics.startSystem(Phaser.Physics.ARCADE);

	// Carrega elementos para a RAM
	// Imagens e Spritesheets
	cube.load.image("background", "assets/images/room/background.png");
	cube.load.image("platform", "assets/images/room/platform.png");
	cube.load.image("wall", "assets/images/room/wall.png");
	cube.load.image("jumper", "assets/images/room/jumper.png");
	
	cube.load.image("knife", "assets/images/guns/knife.png");
	cube.load.image("gun_knife", "assets/images/guns/gun_knife.png");
	cube.load.image("machineGun", "assets/images/guns/machineGun.png");
	cube.load.image("gun_machineGun", "assets/images/guns/gun_machineGun.png");
	cube.load.image("rocketLauncher", "assets/images/guns/rocketLauncher.png");
	cube.load.image("gun_rocketLauncher", "assets/images/guns/gun_rocketLauncher.png");

	cube.load.image("hud", "assets/images/hud/hud.png");

	cube.load.spritesheet("brawler_naked", "assets/images/player/naked.png", 21, 46);
	cube.load.spritesheet("brawler_bob_pants", "assets/images/player/bob_pants.png", 21, 46);
	cube.load.spritesheet("brawler_astronaut_jumpsuit", "assets/images/player/astronaut_jumpsuit.png", 21, 46);
	
	// Audios
	// this.load.audio('coin', 'assets/audio/coin.wav');
}
function create(arena){
	this.room = new Room(arena);

	var name = document.getElementById("nameChar").value; 
	var weapon = document.getElementById("weaponChar").value;
	var style = document.getElementById("styleChar").value;	
	this.player = new Brawler(uniqueId, this.room, name, weapon, style, Math.floor((Math.random()*(cube.world.width-80))+40), 60); //Cria o player
	this.hud = new HUD(this.player);

	this.control = new Input(this.player); // Cria os controles

	// Adiciona o player à lista de jogadores local
	brawlersList[uniqueId] = this.player;

	// Faz a câmera seguir o player
	cube.camera.follow(this.player.avatar);
}
function update(){
	if(ready == false) return;// Não faz o update se a conexão não estiver estabelecida

	// Atualiza a HUD
	hud.update();

	// Atualiza o contador de ciclos usado para sincronização de posição do multiplayer
	updateCounter++;
	// Atualizações dos players
	for (var i in brawlersList){
		if (!brawlersList[i]) continue;
		
		var curBrawler = brawlersList[i];
		var curBullets = brawlersList[i].gun.bullets;

		// Colisão das balas com a parede
		cube.physics.arcade.collide(curBullets, room.platforms, function(bullet, wall){ bullet.kill(); }, null, this);

		// Ciclo de updates 
		for (var j in brawlersList){
			if (!brawlersList[j]) continue;
			if(j!=i){
				var enemyBrawler = brawlersList[j];

				// Testa a colisão 
				cube.physics.arcade.collide(curBrawler.avatar, enemyBrawler.avatar);
				cube.physics.arcade.collide(curBullets, enemyBrawler.avatar, function(enemy, bullet){
																				bullet.kill(); // Mata a bala
																				enemyBrawler.life = enemyBrawler.life - curBrawler.gun.shotDamage;
																				if(enemyBrawler.life <= 0){
																					eurecaServer.callAnnouncer("kill", curBrawler.avatarName, enemyBrawler.avatarName); // Faz o announcer falar
																					eurecaServer.informKill(enemyBrawler.id); // Mata o player e o respawna

																					kills++; // Adiciona uma kill ao seu contador
																					// Se você chegou a 25 kills, informa a todos que o jogo acabaou
																					if(kills >= 15) eurecaServer.callAnnouncer("win", curBrawler.avatarName, ""); // Faz o announcer falar se ganhou
																				}
																			}, null, this);
			}
			if(brawlersList[j].alive){ brawlersList[j].update(); }			
		}
    }

    control.update(); // Update do controle. Aqui também é atualizado o pacote de informações a ser enviado para o servidor
}
function render(){}
// ============================================================ //

// ========================== BRAWLER ========================= //
Brawler = function(index, arena, name, weapon, style, x, y){
	// Referência ao mapa
	this.arena = arena;

	// Inicia o objeto que guarda o player
	this.avatar = cube.add.sprite(x, y, "brawler_"+style);
	this.avatar.anchor.setTo(0.5, 1); // Centra o sprite para rodar em seu eixo

	// Status do player
	this.id = index;
	this.alive = true;
	this.life = 100;
	this.stamina = 100;
	this.facing = "right";
	this.avatarStyle = style;
	this.staminaStatus = "none";

	// Nome do char
	this.avatarName = name;
	this.refName = cube.add.text(0, 0, this.avatarName, {font: "12px Arial", fill: '#000000', align: 'center'});
	this.refName.anchor.setTo(0.5, 3);
	this.avatar.addChild(this.refName);

	// Definições para arma
	this.gun = new Gun(weapon);
	this.avatar.addChild(this.gun.gunBody); // Linka o sprite da arma ao char

	// Ativa a física no player
	cube.physics.arcade.enable(this.avatar, true);

	//  Propriedades físicas do player
	this.avatar.body.bounce.y = 0;
	this.avatar.body.gravity.y = 1250;
	this.avatar.body.collideWorldBounds = false;

	// Cria duas animações
	this.avatar.animations.add("idle", [0, 0, 1, 1], 5, true);
	this.avatar.animations.add("run", [2, 3, 4, 5, 4], 10, true);

	// Esse objeto guarda a entrada do jogador que será enviada para o servidor.
	this.inputsToServer = {
		left: false,
		right: false,
		jump: false,
		attack: false,

		x: this.avatar.body.x,
		y: this.avatar.body.y
	};

	// Já esse objeto guarda a entrada que vem do servidor e é usada para mover elementos na tela.
	this.inputsFromServer = {
		left: false,
		right: false,
		jump: false,
		attack: false,

		x: this.avatar.body.x,
		y: this.avatar.body.y
	}
}
Brawler.prototype = {
	update: function(){
		// Primeiro, fazemos manipulações para manter o nome de forma correta
		if(this.facing == "right") this.refName.scale.x = -1;
		else this.refName.scale.x = 1;

		// Essa variável verifica se houve uma mudança entre a entrada que o servidor conhece e a do usuário
		var inputChanged = (
			this.inputsToServer.left != this.inputsFromServer.left ||
			this.inputsToServer.right != this.inputsFromServer.right ||
			this.inputsToServer.jump != this.inputsFromServer.jump ||
			this.inputsToServer.attack != this.inputsFromServer.attack
		);

		// Caso haja uma diferença entre as entradas, atualiza-a
		if(inputChanged){
			// Envia ao servidor a entrada do jogador
			if(this.id == uniqueId){ eurecaServer.handleKeys(this.inputsToServer); }
		}
		// Após a função acima, o objeto inputFromServer está com valores atualizados

		// Reseta a velocidade em x do player
	    this.avatar.body.velocity.x = 0;

	    // Move o player de acordo com os inputs coletados
		if(this.inputsFromServer.left){ this.moveLeft();  }
		else if(this.inputsFromServer.right){ this.moveRight(); }
	    else{ 
	    	this.idle();
	    	if(this.inputsFromServer.x != this.avatar.body.x) this.avatar.body.x = this.inputsFromServer.x; // Essa linha faz a correção de movimento em x
	    }

	    if(this.inputsFromServer.jump){
	    	this.jump();
	    	if(this.inputsFromServer.y != this.avatar.body.y) this.avatar.body.y = this.inputsFromServer.y; // Essa linha faz a correção de movimento em y
	    }
	    if(this.inputsFromServer.attack){ this.attack(); }
	    // Faz a correção da posição em y a cada 6 segundos
	    if(updateCounter >= 360000){ // 60 fps*6000 milisegundos = 360000
	    	this.avatar.body.y = this.inputsFromServer.y;
	    	updateCounter = 0;
	    }

		// Colisões
		cube.physics.arcade.collide(this.avatar, this.arena.walls);
		cube.physics.arcade.collide(this.avatar, this.arena.platforms);
		// Colisão com jumper
		cube.physics.arcade.overlap(this.avatar, this.arena.jumpers, function(player, jumper){ player.body.velocity.y = -1500; }, null, this);

		// Atualiza o ganho de stamina
		if(this.stamina < 100){
			if(this.staminaStatus == "rapid") this.stamina = this.stamina + 0.25/3;
			else if(this.staminaStatus == "slow") this.stamina = this.stamina + 0.25/6;

			if(this.stamina > 100) this.stamina = 100;
		}

		// Atualiza as posições que serão passadas ao server em algum momento
		this.inputsToServer.x = this.avatar.body.x;
		this.inputsToServer.y = this.avatar.body.y;
	},
	// Essa função mata o personagem quando ele desconecta do servidor
	kill: function(){
		this.alive = false;
		this.avatar.kill();
	},
	// FUNÇÕES DE MOVIMENTAÇÃO
	moveLeft: function(){
		this.facing = "left";
        this.avatar.body.velocity.x = -150;
        this.avatar.scale.x = 1;
        this.avatar.animations.play("run"); // Liga a animação
        this.staminaStatus = "slow";
	},
	moveRight: function(){
		this.facing = "right";
        this.avatar.body.velocity.x = 150;
        this.avatar.scale.x = -1;
        this.avatar.animations.play("run");
        this.staminaStatus = "slow";
	},
	idle: function(){
		this.avatar.animations.play("idle");
		this.staminaStatus = "rapid";
	},
	jump: function(){
		this.avatar.body.velocity.y = -550;
		this.staminaStatus = "slow";
	},
	attack: function(){
		this.gun.shoot(this);
		this.staminaStatus = "none";
	}
}
// ============================================================ //

// ============================ GUN =========================== //
Gun = function(gunType){
	this.name = gunType;
	this.shotTimer = 0;
	this.bullets = cube.add.group();
	this.cadency = 0;
	this.shotCost = 0;
	this.shotDamage = 0;
	this.shotVelocity = 0;
	this.gunBody = cube.add.sprite(0,0, "gun_" + this.name);

	cube.physics.enable(this.bullets, Phaser.Physics.ARCADE);

	// Informações específicas de cada 
	if(this.name == "machineGun"){
		this.gunBody.anchor.setTo(0.8, 3.9);
		this.cadency = 75;
		this.shotCost = 5;
		this.shotDamage = 7;
		this.shotVelocity = 600;
	}
	else if(this.name == "knife"){
		this.gunBody.anchor.setTo(0.5, 2);
		this.cadency = 500;
		this.shotCost = 15;
		this.shotDamage = 25;
		this.shotVelocity = 400;
	}
	else if(this.name == "rocketLauncher"){
		this.gunBody.anchor.setTo(0.7, 2.3);
		this.cadency = 1200;
		this.shotCost = 65;
		this.shotDamage = 100;
		this.shotVelocity = 300;
	}
}
Gun.prototype = {
	shoot: function(owner){
		if(this.shotTimer < cube.time.now && owner.stamina >= this.shotCost){
			this.shotTimer = cube.time.now + this.cadency;

			var bullet;

			if(owner.facing == "right"){ bullet = this.bullets.create((owner.avatar.body.x + owner.avatar.body.width/2 + 10), (owner.avatar.body.y + owner.avatar.body.height/2 + 4), this.name); }
			else{ bullet = this.bullets.create((owner.avatar.body.x + owner.avatar.body.width/2 - 10), (owner.avatar.body.y + owner.avatar.body.height/2 + 4), this.name); }

			cube.physics.enable(bullet, Phaser.Physics.ARCADE);

			bullet.outOfBoundsKill = true;
			bullet.anchor.setTo(0.5, 0.5);
			bullet.body.velocity.y = 0;
			bullet.owner = owner;

			if(owner.facing == "right"){
				bullet.scale.x = -1;
				bullet.body.velocity.x = this.shotVelocity;
			}
			else{ bullet.body.velocity.x = -this.shotVelocity; }
			// Remove a stamina do jogador
			owner.stamina = owner.stamina - this.shotCost;
		}
	}
}
// ============================================================ //

// =========================== ROOM =========================== //
Room = function(arena){
	// Informa o tamanho da sala para o uso da câmera
    cube.world.setBounds(0, 0, 2000, 1000);

    // Cria o background
	var bg = cube.add.sprite(0, 0, "background");
	bg.scale.setTo((cube.world.width/bg.width) , (cube.world.height/bg.height));

	this.walls = cube.add.group(); // As paredes da sala ficam aqui
	this.platforms = cube.add.group(); // As plataformas vão ficar aqui dentro
	this.jumpers = cube.add.group();

	// Ativa a física para as paredes e as plataformas
	this.walls.enableBody = true;
    this.platforms.enableBody = true;
    this.jumpers.enableBody = true;

    // Cria as paredes e o chão, escalando para o tamanho certo
    // Parede lateral esquerda
    var wall = this.walls.create(0, 0, "wall");
    wall.scale.setTo(2,(cube.world.height/wall.height));
    wall.body.immovable = true;// Faz a parede não se mover com o toque do jogador

    // Parede lateral direita
    wall = this.walls.create(cube.world.width - 40, 0, "wall");
    wall.scale.setTo(2,(cube.world.height/wall.height));
    wall.body.immovable = true;// Faz a parede não se mover com o toque do jogador
    
    // Chão
    wall = this.walls.create(0, cube.world.height - 40, "wall");
    wall.scale.setTo((cube.world.width/wall.width), 2);
    wall.body.immovable = true;// Faz a parede não se mover com o toque do jogador

    // Teto
   	wall = this.walls.create(0, 0, "wall");
    wall.scale.setTo((cube.world.width/wall.width),2);
    wall.body.immovable = true;// Faz a parede não se mover com o toque do jogador

    // Cria os jumpers
    var jump;
    for(var i=0; i<Math.floor((cube.world.width)/200); i++){
    	jump = this.jumpers.create(200*i + 40, cube.world.height-45, "jumper");
    	jump.scale.setTo(2,2);
    	jump.body.immovable = true;
    }

    // Cria os blocos internos
    var plat;
    for(var i=0; i<arena.length; i++){
    	plat = this.platforms.create(arena[i].posX, arena[i].posY, "platform");
    	plat.scale.setTo(arena[i].scaleX, arena[i].scaleY);

    	plat.body.immovable = true;
    }
}
// ============================================================ //

// ========================== INPUT =========================== //
Input = function(controlled){
	this.controlled = controlled; // Referência ao player

	this.controlCursors = cube.input.keyboard.createCursorKeys();
	this.controlButtons = {
		jump: cube.input.keyboard.addKey(Phaser.Keyboard.Z),
		attack: cube.input.keyboard.addKey(Phaser.Keyboard.X)
	}
}
Input.prototype = {
	update: function(){
		// Só capta movimentos se o player estiver vivo
		if(!this.controlled || !this.controlled.alive) return;

	    // Movimento horizontal
	    if (this.controlCursors.left.isDown){ this.controlled.inputsToServer.left = true; }
	    else{ this.controlled.inputsToServer.left = false; }
	    if (this.controlCursors.right.isDown){ this.controlled.inputsToServer.right = true; }
	    else{ this.controlled.inputsToServer.right = false; }

	    // Pulo
	    if (this.controlButtons.jump.isDown && this.controlled.avatar.body.touching.down){ this.controlled.inputsToServer.jump = true; }
	    else { this.controlled.inputsToServer.jump = false; }

	    // Ataque
	    if(this.controlButtons.attack.isDown){ this.controlled.inputsToServer.attack = true; }
	    else { this.controlled.inputsToServer.attack = false; }
	}
}
// ============================================================ //

// ========================== HUD ============================= //
HUD = function(player){
	this.baseHUD = cube.add.sprite(0, 0, "hud");
	this.hpCounter = cube.add.text(40, 10, player.life.toFixed(0)+" / 100", {font: "20px Arial", fill: "red", align: "center"});
	this.staminaCounter = cube.add.text(40, 40, player.stamina.toFixed(0)+" / 100", {font: "20px Arial", fill: "blue", align: "center"});
	this.killCounter = cube.add.text(160, 35, kills, {font: "20px Arial", fill: "black", align: "center"});

	this.baseHUD.fixedToCamera = true;
	this.hpCounter.fixedToCamera = true;
	this.staminaCounter.fixedToCamera = true;
	this.killCounter.fixedToCamera = true;
}
HUD.prototype = {
	update: function(){
		this.hpCounter.setText(player.life.toFixed(0)+" / 100");
		this.staminaCounter.setText(player.stamina.toFixed(0)+" / 100");
		this.killCounter.setText(kills);
	}
}
// ============================================================ //
