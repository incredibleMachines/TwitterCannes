var SocketServer = require('ws').Server;

var wss = new SocketServer({port: 9092});

wss.on('connection', function(ws){
	
	ws.on('message', function(message){
		
		console.log('recieved: %s', message);
		
	});
	
	ws.on('error', function(error){
		
		console.log("Error: "+ error );
		
	});
	
	ws.on('close', function(){
		
		console.log('Client Disconnected');
	});
	
	ws.send('Hello OF from Node.js');
	
});