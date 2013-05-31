var twitter = require('ntwitter');

var twit = new twitter({
	
	consumer_key: 'ZTioLF2GOz5ZR3xtRyl7g',
	consumer_secret: 'oji9yVuN024t7C67yc18GyHmGnwYxxaNoRP7Z98jEOw',
	access_token_key: '371351910-f4pp7PjaM6ny3frWyImB1JIg2kpfGqtcKl1Glt4n',
	access_token_secret: 'qod1HsYuMhcM7eImbipb1p7bYkcEDracyLx89XVCfik'
	
});


twit.stream('statuses/filter', {track: 'nba'}, function(stream){
	
	stream.on('data', function(data){
		
		console.log(data);
		
	});
	
	stream.on('error', function(error, code){
		
		console.log('ERROR: '+error+': \n'+code);
		
	});
	
});
