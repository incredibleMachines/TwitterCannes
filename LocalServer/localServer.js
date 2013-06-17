// Set up mongo client
var MongoClient = require('mongodb').MongoClient;
var mongoURL = "mongodb://127.0.0.1/twitterCannesLions";

var tweets;
var port = 9000;

// Connect to mongo server
MongoClient.connect(mongoURL, function(err, db) {
	if (err) throw err;
	console.log("Connected to Database");

	tweets = db.collection('tweets');
});

// Set up express server
var express = require('express');
var app = express();

app.get('/',function(req, res){

	var current_time = (new Date()).toISOString().substring(0, 19).replace('T', ' ');

	// Set up find parameters
	var find = {}

	// Only approved tweets
	find.status = 'Approved';

	// Since the given timestamp
	if (req.query.since)
		find.approved_at = { $gt: req.query.since };

	// Set a category if passed
	if (req.query.category)
		find.category = req.query.category;

	// Set starred if passed
	if (req.query.starred)
		find.starred = req.query.starred == "true" ? true : false;

	if(req.query.media_url)
		find.media_url = {$ne: ""};

	// Set up options
	var options = {}

	// Set limit if passed, otherwise use a default
	if (req.query.limit) {
		var limit = parseInt(req.query.limit);
		if (limit > 0)
			options.limit = limit;
	} else {
		options.limit = 10;
	}
			
	// Set sort field
	options.sort = { shown_count: 1, approved_at: 1 };
	
	console.log('['+current_time+'] Received request: ' + req.url);
	console.log('['+current_time+'] Searching with...');
	console.log('['+current_time+'] find: ' + JSON.stringify(find));
	console.log('['+current_time+'] options: ' + JSON.stringify(options));

	tweets.find(find, options).toArray(function(err, docs) {
		res.send(docs);

		if (req.query.increase_shown_count) {
			if (req.query.increase_shown_count != 'false') {
				for (var i = 0; i < docs.length; i++) {
					tweets.update(docs[i], { $inc: { shown_count: 1 } }, function(){});
				}
			} else {
				console.log('['+current_time+'] Not increasing shown count for this request!')
			}
		}
	});

});

app.listen(port);
console.log('Listening on port ' + port);