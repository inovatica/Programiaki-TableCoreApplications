const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 8886 });

wss.on('connection', function connection(ws) {
  ws.on('message', function incoming(message) {
	//console.log(message)
	wss.clients.forEach(function each(client) {
		if(client.readyState === WebSocket.OPEN){
		  client.send(message)
		}
	})
  });
});
