// --------------------------------------------------------
// Create twitter stream & set credentials
// --------------------------------------------------------
var TwitterStream; // stream handler
var twitter = require('ntwitter');
var twit = new twitter({
	
	consumer_key: 'ZTioLF2GOz5ZR3xtRyl7g',
	consumer_secret: 'oji9yVuN024t7C67yc18GyHmGnwYxxaNoRP7Z98jEOw',
	access_token_key: '371351910-f4pp7PjaM6ny3frWyImB1JIg2kpfGqtcKl1Glt4n',
	access_token_secret: 'qod1HsYuMhcM7eImbipb1p7bYkcEDracyLx89XVCfik'
	
});


// --------------------------------------------------------
// Set up mongo client
// --------------------------------------------------------
var MongoClient = require('mongodb').MongoClient;
var mongoURL = "mongodb://127.0.0.1/twitterCannesLions";
var tweetsDB, whitelistDB, followingDB;

var whitelist;
var following


// --------------------------------------------------------
// Connect to mongo server
// Start stream upon connecton
// --------------------------------------------------------
MongoClient.connect(mongoURL, function(err, db) {
	if (err) throw err;
	console.log("Connected to Database");

	tweetsDB = db.collection('tweets');
	whitelistDB = db.collection('whitelist');
	followingDB = db.collection('following')

	updateListsAndRestart();
});


// --------------------------------------------------------
// Load up request & dom modules for scraping
var request = require("request");
var cheerio = require("cheerio");
// --------------------------------------------------------


// --------------------------------------------------------
// Set up express server
// --------------------------------------------------------
var express = require('express');
var app = express();
app.listen(9001);

app.get('/update',function(req, res){
	updateListsAndRestart();	
});
app.get('/dump', function(req, res) {
	res.send('Whitelist: ' + whitelist + "\nFollowing: " + following);
});


// --------------------------------------------------------
// Update list of whitelisted & following users
// Start/restart the street once they're updated
// --------------------------------------------------------
function updateListsAndRestart() {

	// Get user handles for whitelist
	whitelistDB.find().toArray(function(err, docs) {
		whitelist = [];
		for (var i = 0; i < docs.length; i++) {
			whitelist.push(docs[i]['user_screen_name']);
		}
		
		// Get user ids for following
		followingDB.find().toArray(function(err, docs) {
			var id_list = [];
			for (var i = 0; i < docs.length; i++) {
				id_list.push(docs[i]['id']);
			}
			following = id_list.join();
			
			// Once both are done, send the result out
			//res.send('Whitelist: ' + whitelist + "\nFollowing: " + following);

			// After updating lists, kill the stream and then restart it
			if (TwitterStream) TwitterStream.destroy();
			TwitterService();
		});
	});	

}

// --------------------------------------------------------
// Takes a URL, searches for an instagram photo, and
// updates a mongo document
// --------------------------------------------------------
function findPhoto(url, document) {
	console.log("Trying to find photo at " + url);

	// Make request, don't follow redirects. We only want the headers & to be as quick as possible
	request({ uri: url, followRedirect: false }, function(error, response, body) {

		if (!error) {
		
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

						tweetsDB.update(document, { $set: { media_url: photo }}, function(err, inserted) {
							if (err) throw err;
						});

						return photo;

					});

				} else {
					return '';
				}
			} else {
				return '';
			}
		}
	});
}

// --------------------------------------------------------
// Twitter stream processing
// --------------------------------------------------------
var profanity = [
	'2g1c','2 girls 1 cup','acrotomophilia','anal','anilingus','anus','arsehole','ass','asshole','assmunch','auto erotic','autoerotic','babeland','baby batter','ball gag','ball gravy','ball kicking','ball licking','ball sack','ball sucking','bangbros','bareback','barely legal','barenaked','bastardo','bastinado','bbw','bdsm','beaver cleaver','beaver lips','bestiality','bi curious','big black','big breasts','big knockers','big tits','bimbos','birdlock','bitch','black cock','blonde action','blonde on blonde action','blow j','blow your l','blue waffle','blumpkin','bollocks','bondage','boner','boob','boobs','booty call','brown showers','brunette action','bukkake','bulldyke','bullet vibe','bung hole','bunghole','busty','butt','buttcheeks','butthole','camel toe','camgirl','camslut','camwhore','carpet muncher','carpetmuncher','chocolate rosebuds','circlejerk','cleveland steamer','clit','clitoris','clover clamps','clusterfuck','cock','cocks','coprolagnia','coprophilia','cornhole','cum','cumming','cunnilingus','cunt','darkie','date rape','daterape','deep throat','deepthroat','dick','dildo','dirty pillows','dirty sanchez','dog style','doggie style','doggiestyle','doggy style','doggystyle','dolcett','domination','dominatrix','dommes','donkey punch','double dong','double penetration','dp action','eat my ass','ecchi','ejaculation','erotic','erotism','escort','ethical slut','eunuch','faggot','fecal','felch','fellatio','feltch','female squirting','femdom','figging','fingering','fisting','foot fetish','footjob','frotting','fuck','fuck buttons','fudge packer','fudgepacker','futanari','g-spot','gang bang','gay sex','genitals','giant cock','girl on','girl on top','girls gone wild','goatcx','goatse','gokkun','golden shower','goo girl','goodpoop','goregasm','grope','group sex','guro','hand job','handjob','hard core','hardcore','hentai','homoerotic','honkey','hooker','hot chick','how to kill','how to murder','huge fat','humping','incest','intercourse','jack off','jail bait','jailbait','jerk off','jigaboo','jiggaboo','jiggerboo','jizz','juggs','kike','kinbaku','kinkster','kinky','knobbing','leather restraint','leather straight jacket','lemon party','lolita','lovemaking','make me come','male squirting','masturbate','menage a trois','milf','missionary position','motherfucker','mound of venus','mr hands','muff diver','muffdiving','nambla','nawashi','negro','neonazi','nig nog','nigga','nigger','nimphomania','nipple','nipples','nsfw images','nude','nudity','nympho','nymphomania','octopussy','omorashi','one cup two girls','one guy one jar','orgasm','orgy','paedophile','panties','panty','pedobear','pedophile','pegging','penis','phone sex','piece of shit','piss pig','pissing','pisspig','playboy','pleasure chest','pole smoker','ponyplay','poof','poop chute','poopchute','porn','porno','pornography','prince albert piercing','pthc','pubes','pussy','queaf','raghead','raging boner','rape','raping','rapist','rectum','reverse cowgirl','rimjob','rimming','rosy palm','rosy palm and her 5 sisters','rusty trombone','s&m','sadism','scat','schlong','scissoring','semen','sex','sexo','sexy','shaved beaver','shaved pussy','shemale','shibari','shit','shota','shrimping','slanteye','slut','smut','snatch','snowballing','sodomize','sodomy','spic','spooge','spread legs','strap on','strapon','strappado','strip club','style doggy','suck','sucks','suicide girls','sultry women','swastika','swinger','tainted love','taste my','tea bagging','threesome','throating','tied up','tight white','tit','tits','titties','titty','tongue in a','topless','tosser','towelhead','tranny','tribadism','tub girl','tubgirl','tushy','twat','twink','twinkie','two girls one cup','undressing','upskirt','urethra play','urophilia','vagina','venus mound','vibrator','violet blue','violet wand','vorarephilia','voyeur','vulva','wank','wet dream','wetback','white power','women rapping','wrapping men','wrinkled starfish','xx','xxx','yaoi','yellow showers','yiffy','zoophilia'
	];

function TwitterService() {

	// Set up our twitter stream
	twit.stream('statuses/filter', { track: '#CannesLions', follow: following, stall_warnings: true }, function(stream) {
		
		TwitterStream = stream;
		console.log('STREAM CONNECTED');
		console.log('Whitelist: ' + whitelist);
		console.log('Following: ' + following);

		stream.on('data', function(data){

			//console.log(data['user']['screen_name']);
			console.log('RAW:');
			console.log(data);


			// Check for a warning
			if (data.hasOwnProperty('warning')) {
				console.log('STREAM WARNING');
				console.log('code: ' + data['warning']['code']);
				console.log('message: ' + data['warning']['message']);
				console.log('percent full: ' + data['warning']['percent_full']);
			}

			// Set up data, check that the properties exist first
			var created_at = data.hasOwnProperty('created_at') ? data['created_at'] : (new Date());
			created_at = (new Date(created_at)).toISOString().substring(0, 19).replace('T', ' ')

			var tweet_id = data.hasOwnProperty('id_str') ? data['id_str'] : '';
			var lang = data.hasOwnProperty('lang') ? data['lang'] : '';

			if (data.hasOwnProperty('user')) {
				var user_id = data['user'].hasOwnProperty('id_str') ? data['user']['id_str'] : '';
				var user_name = data['user'].hasOwnProperty('name') ? data['user']['name'] : '';
				var user_screen_name = data['user'].hasOwnProperty('screen_name') ? data['user']['screen_name'] : '';

				// Process user image...
				var user_image = data['user'].hasOwnProperty('profile_image_url') ? data['user']['profile_image_url'] : '';

				// Alter URL to get the original, highest-res version
				user_image = user_image.replace('_normal', '');

				// Reset to http if it's https
				if (user_image.substring(0, 5) == 'https') {
					user_image = 'http' + user_image.substring(5);
				}
			}

			// Check for an attached photo and set values appropriately
			if (data.hasOwnProperty('entities')) {
				if (data['entities'].hasOwnProperty('media')) {
					var media_url = data['entities']['media'][0]['media_url'];

					// Reset to http if it's https
					if (media_url.substring(0, 5) == 'https') {
						media_url = 'http' + media_url.substring(5);
					}
				} else {
					var media_url = '';
				}
			}

			// Check if this is a retweet
			// If so, grab the original text if it's from a whitelisted/following user
			// If not, return prematurelt
			if (data.hasOwnProperty('retweeted_status')) {

				// We want to ignore retweets UNLESS they're from users that are whitelisted or we're following them
				if (whitelist.indexOf(user_screen_name) > -1 || following.split(',').indexOf(user_id) > -1) {
					if (data['retweeted_status'].hasOwnProperty('text')) {
						var tweet_text = data['retweeted_status']['text'];
					} else {
						return
					}
				} else {
					return;
				}

			} else if (data.hasOwnProperty('text')) {
				var tweet_text = data['text'];
			} else {
				return;
			}

			// Now that tweet text is set up (either original or retweet)
			// make sure it includes the hashtag if it's not a whitelisted user
			if (whitelist.indexOf(user_screen_name) == -1 && following.split(',').indexOf(user_id) == -1 && tweet_text.toLowerCase().indexOf("#canneslions") == -1) {
				return;
			}

			// Check for profanity
			for (i = 0; i < profanity.length; i++) {
				// Use whole word match regex for things like 'analysis' not testing positive for 'anal'
				if (new RegExp("\\b" + profanity[i] + "\\b", "g").test(tweet_text)) {
					return;
				}
			}

			// Check if the user is whitelisted
			// If so, check if tweet contains hashtag, and approve their status and set approved timestamp to now
			// If not, then set status to pending
			if (whitelist.indexOf(user_screen_name) > -1) {
				// console.log('User is in whitelist');
				if (tweet_text.toLowerCase().indexOf("#canneslions") > -1) {
					// console.log('hashtag found in tweet');
					var status = 'Approved';
					var approved_at = created_at;
				} else {
					// console.log('hashtag NOT found in tweet');
					var status = 'Pending';
					var approved_at = '';
				}
			} else {
				// console.log('User NOT in whitelist');
				var status = 'Pending';
				var approved_at = '';
			}

			// Loop through text and look for URLs. Remove them!
			split = tweet_text.split(' ');
			for (i = 0; i < split.length; i++) {
				var s = split[i].replace(/"/g, '');
				if (s.substring(0, 4) == 'http') {
					split.splice(i, 1);
				}
			}
			var tweet_text_insert = split.join(' ');


			// Set up mongo document for insertion, keep only what we're interested in
			document = {
				//id: data['id'],
				id: tweet_id,
				approved_at: approved_at,
				created_at: created_at,
				user_id: user_id,
				user_name: user_name,
				user_screen_name: user_screen_name,
				user_image: user_image,
				text: tweet_text_insert,
				media_url: media_url,
				lang: lang,
				status: status,
				category: 'None',
				starred: false
			}
			console.log('PROCESSED:');
			console.log(document);


			// insert record into mongo
		 	tweetsDB.insert(document, function(err, records) {
		 		if (err) throw err;
		 		// console.log("Record added as "+records[0]._id);

		 		// If there was no attached photo that Twitter gave us, search through
		 		// all the words and see if any of them are URLs. If so, try and find a photo
		 		// at that URL
		 		if (!media_url) {
					var text = tweet_text;
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
		
		});
		
		stream.on('error', function(error, code){
		
			var error = error+': \n'+code;
			
			console.log('STREAM ERROR');
			console.log(error);

			throw error;
		
		});

		stream.on('end', function (response) {
			// Handle a disconnection
			console.log('STREAM END');
			console.log(response);
		});
		stream.on('destroy', function (response) {
			// Handle a 'silent' disconnection from Twitter, no end/error event fired
			console.log('STREAM DESTROY');
			console.log(response);
		});
	
	});

}