// Create websocket server
var SocketServer = require('ws').Server;
var wss = new SocketServer({port: 9093});

// Set up mongo client
var MongoClient = require('mongodb').MongoClient;
var mongoURL = "mongodb://127.0.0.1/test";
var mongodb;

// Connect to mongo server
MongoClient.connect(mongoURL, function(err, db) {
	if (err) throw err;
	console.log("Connected to Database");

	mongodb = db.collection('test');
});


// Create twitter stream & set credentials
var twitter = require('ntwitter');
var twit = new twitter({
	
	consumer_key: 'ZTioLF2GOz5ZR3xtRyl7g',
	consumer_secret: 'oji9yVuN024t7C67yc18GyHmGnwYxxaNoRP7Z98jEOw',
	access_token_key: '371351910-f4pp7PjaM6ny3frWyImB1JIg2kpfGqtcKl1Glt4n',
	access_token_secret: 'qod1HsYuMhcM7eImbipb1p7bYkcEDracyLx89XVCfik'
	
});

// Load up request & dom modules for scraping
var request = require("request");
var cheerio = require("cheerio");


// Set flag for whether socket connection was closed
var bTerminated = false;

// Once we have a connection to our socket
wss.on('connection', function(ws){

	bTerminated = false;
	ws.send('Hello OF from Node.js');

	// Log any received messages
	ws.on('message', function(message){
		console.log('received: %s', message);
	});
	
	// And errors
	ws.on('error', function(error){
		console.log("Error: "+ error );
	});
	
	ws.on('close', function(){
		console.log('Client Disconnected Terminating Stream');
		bTerminated = true;
	});
	
	// Set up our twitter stream
	twit.stream('statuses/filter', {track: 'morning'}, function(stream){
	
			stream.on('data', function(data){

				// console.log(data['user']['screen_name']);
				console.log(data);

				// Check for an attached photo and set values appropriately
				if (data['entities'].hasOwnProperty('media')) {
					media_url = data['entities']['media'][0]['media_url'];
				} else {
					media_url = false;
				}

				// Set up mongo document for insertion, keep only what we're interested in
				document = {
					//id: data['id'],
					created_at: (new Date(data['created_at'])).toISOString().substring(0, 19).replace('T', ' '),
					user_id: data['user']['id'],
					user_name: data['user']['name'],
					user_screen_name: data['user']['screen_name'],
					text: data['text'],
					media_url: media_url,
					status: 0,
					category: 0
				}


				// insert record into mongo
			 	mongodb.insert(document, function(err, records) {
			 		if (err) throw err;
			 		// console.log("Record added as "+records[0]._id);

			 		// If there was no attached photo that Twitter gave us, search through
			 		// all the words and see if any of them are URLs. If so, try and find a photo
			 		// at that URL
			 		if (!media_url) {
						var text = data['text'];
						var split = text.split(' ');

						for (i = 0; i < split.length; i++) {
							// Remove all double quotes
							var s = split[i].replace(/"/g, '');
							
							if (s.substring(0, 4) == 'http') {
								media_url = findPhoto(s, document);
							}
						}
					}
			 	});

				// console.log(data);
				ws.send(JSON.stringify(document));
			
			});
			
			stream.on('error', function(error, code){
			
				var error = 'ERROR: '+error+': \n'+code;
				ws.send(error);
				console.log(error);
			
			});
			
			if(bTerminated) stream.destroy();
	
	});
	
});


function findPhoto(url, document) {
	console.log("Trying to find photo at " + url);

	// Make request, don't follow redirects. We only want the headers & to be as quick as possible
	request({ uri: url, followRedirect: false }, function(error, response, body) {
		
		// Check for 301, a redirect for permanent move
		if (response.statusCode == 301) {
			dest = response.headers.location;

			// Check if we're getting redirected to instagram
			// Look to make sure the url string begins with "http://instagram.com"
			// (No match would return -1)
			if (dest.toLowerCase().indexOf("http://instagram.com") == 0) {

				// If so, make a new request to the redirected URL, and do follow redirects
				request(url, function(error, response, body) {

					$ = cheerio.load(body);
					var photo = $('img.photo').attr('src');
					console.log("Here's what I got: " + photo)

					mongodb.update(document, { $set: { media_url: photo }}, function(err, inserted) {
						if (err) throw err;
					});

					return photo;

				});

			} else {
				return false;
			}
		} else {
			return false;
		}

	});
}