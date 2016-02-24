// server.js
// Server-side

// Inicializa o servidor http e o app express
var express = require("express");
var app = express(app);
var server = require("http").createServer(app);
var clients = {}; // Os dados dos clientes ficam aqui
var Eureca = require("eureca.io"); // Obtém a classe EurecaServer
var eurecaServer = new Eureca.Server({allow:["setId", "spawnEnemy", "respawn", "kill", "updateState", "getInfo", "announce"]}); // Cria uma instância do servidor Eureca, informando quais funções podem ser chamadas do cliente

// Variáveis relacionadas a arena 
var arena = []; // Guarda a arena

// Variáveis relacionadas ao narrador
var announcer = {}; // Buffer de informações a serem ditas
var score = {}; // Guarda a pontuação de cada jogador

app.use(express.static(__dirname)); // Serve os arquivos do diretório padrão
eurecaServer.attach(server); // Ata o eureca.io ao servidor http

// Seta o server para esperar requisições na porta correta
server.listen(process.env.PORT || 8000);

// ========================= FUNÇÕES ========================== //

// Detecção de conexão de um cliente
eurecaServer.onConnect(function (conn){
    console.log("Connection %s", conn.id, conn.remoteAddress);

	var remote = eurecaServer.getClient(conn.id); // getClient() cria um proxy que nos permite chamar funções do cliente 
	clients[conn.id] = {id: conn.id, remote: remote, name: "", weapon:"", style: "", spawnX: 300}; // Registra o cliente no servidor e guarda informações necessárias
	
	// Caso a arena não tenha sido criado, cria-a
	if(arena.length == 0){
		for(var i=0; i<30; i++){
			var obj = {
				posX: Math.floor((Math.random()*(2000-80))+40),
				posY: Math.floor((Math.random()*(1000-120))+60),
				scaleX: Math.floor((Math.random()*10)+1),
				scaleY: Math.floor((Math.random()*10)+1)
			};
			arena.push(obj);
		}
	}
	remote.setId(conn.id, arena); // Chamamos a função setId que foi definida em Client-side
});

// Detecção de desconexão de um cliente
eurecaServer.onDisconnect(function (conn){
    console.log("Desconnection", conn.id);
	
	delete clients[conn.id];
	for (var c in clients){
		var remote = clients[c].remote;
		// Chamamos o método kill para matar o personagem desconectado em todos os clientes
		remote.kill(conn.id);
	}
	// Se não houver mais usuários no jogo, mata o mapa
	if (Object.getOwnPropertyNames(clients).length === 0){ arena.length = 0; }
});

// Função que dá informações de cada jogador ao servidor
eurecaServer.exports.setInfo = function(name, weapon, style, spawnX){
	var conn = this.connection;
	var client = clients[conn.id];

	client.name = name;
	client.weapon = weapon;
	client.style = style;
	client.spawnX = spawnX;
}

// Função que continua a conexão iniciada no cliente
eurecaServer.exports.handshake = function(){
	for (var c in clients){
		var remote = clients[c].remote;

		// Spawna todos os elementos
		for (var cc in clients){		
			// Define a posição dos inimigos para serem spawnados. Se eles não estiverem em campo, gera um spawn point
			var x = clients[cc].laststate ? clients[cc].laststate.x : clients[cc].spawnX;
			var y = clients[cc].laststate ? clients[cc].laststate.y : 60;

			remote.spawnEnemy(clients[cc].id, clients[cc].name, clients[cc].weapon, clients[cc].style, x, y);		
		}
	}
}

// Função que repassa a entrada de um player para todos os outros
eurecaServer.exports.handleKeys = function (keys){
	var conn = this.connection;
	var updatedClient = clients[conn.id];

	for (var c in clients){
		var remote = clients[c].remote;
		remote.updateState(updatedClient.id, keys);

		// keep last known state so we can send it to new connected clients
		clients[c].laststate = keys;
	}
}

// Quando um player morre, essa informação é passada a todos os outros
eurecaServer.exports.informKill = function(id){
	for (var c in clients){
		var remote = clients[c].remote;
		remote.kill(id);
		remote.respawn(id, clients[id].name, clients[id].weapon, clients[id].style, clients[id].spawnX);
	}
}

// Função que é chamada quando ocorre alguma morte e ela precisa ser notificada
eurecaServer.exports.callAnnouncer = function(type, char1, char2){
	for(c in clients){
		var remote = clients[c].remote;
		remote.announce(type, char1, char2);
	}
}