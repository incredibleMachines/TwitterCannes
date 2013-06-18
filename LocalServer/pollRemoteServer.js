var http = require('http');
var fs = require('fs');
var request = require('request');

var MongoClient = require('mongodb').MongoClient
	,Server = require('mongodb').Server
	,mongo = new MongoClient(new Server('localhost', 27017))
	,BSON = require('mongodb').BSONPure;

var db, collection;

mongo.open(function(err,mongo){

	db = mongo.db('twitterCannesLions');
	collection = db.collection('tweets');

});


setInterval(checkRemote, 1000*30); //one minute

function checkRemote(){
	var body=''; 
	var json;
	var timestamp;

	collection.find().sort({"approved_at":-1}).limit(1).toArray( function(err,doc){

		if(!doc.length){
			//console.log('Empty');
			timestamp = '2013-06-18+03:00:00';

		}else{
			var newtime = doc[0].approved_at;
			timestamp=newtime.replace(' ','+');
		}
		var options = {

			host: 'cannes.incrediblemachin.es',
			port: 9000,
			path: '/?since='+timestamp+'&limit=5'

		};

		var current_time = (new Date()).toISOString().substring(0, 19).replace('T', ' ');
		console.log("\n");
		console.log('[' +current_time + '] Fetching since: ' + timestamp);

		http.get(options,function(res){

			if (res.hasOwnProperty('statusCode')) {
				console.log('[' +current_time + '] Remote server status code: ' + res.statusCode);
			} else {
				console.log('[' +current_time + '] No status code in result from remote server!');
			}

			res.on('data',function(chunk){

				body+= chunk;

			});
			res.on('end',function(){

				//console.log(body);
				json = JSON.parse(body);

				for(var i = 0; i< json.length; i++){
						insertOrUpdate(json[i]);
				}

			});

		}).on('error',function(e){

			console.log('Get Error: '+e);
		});


	});	

}


function insertOrUpdate(json){


	var oID = {"_id": new BSON.ObjectID(json._id)};
	collection.findOne(oID, function(err,result){

		if(!result){

			var obj = {
				"_id": new BSON.ObjectID(json._id),
				"approved_at": json.approved_at, //
				"created_at": json.created_at,
				"user_id":json.user_id,
				"user_name":json.user_name,
				"user_screen_name":json.user_screen_name,
				"user_image":"", //json.user_image,
				"text":json.text,
				"media_url":"", //json.media_url,
				"lang":json.lang,
				"status":json.status,
				"category":json.category, //
				"starred": json.starred, //
				"shown_count": 0,
				"last_shown": "",
				"id":json.id

			};

			if (json.user_image != "") {
				var user_image_path = "/twitter/user_images/" + obj.user_id + "." + json.user_image.split('.').reverse()[0];
				fetchUserImage(json.user_image, user_image_path, obj.id);
				//obj.user_image = user_image_path;
			}
			if (json.media_url != "") {
				var media_url_path = "/twitter/media_images/" + obj.id + "." + json.media_url.split('.').reverse()[0];
				fetchMediaImage(json.media_url, media_url_path, obj.id);
				//obj.media_url = media_url_path;
			}

			collection.insert(obj, {safe:true},function(err,doc){
				if(err)throw err;

			});

		}else{

			var oID = {"_id": new BSON.ObjectID(json._id) };
			var obj = { $set: { "approved_at": json.approved_at, "category": json.category, "starred": json.starred}};

			collection.update(oID, obj, function(err){
				if(err) throw err;
			});
		}

	});

}

var fetchUserImage = function(uri, outfile, tweet_id) {

	console.log("\n");
	console.log(tweet_id + ": Fetching user image...");
	console.log(tweet_id + ": URI: " + uri);
	console.log(tweet_id + ": Outfile: " + outfile);

	var request = http.get(uri, function(res) {
	    var imageData = '';
	    res.setEncoding('binary');

	    if (res.hasOwnProperty('statusCode')) {
			console.log(tweet_id + ": Status: " + res.statusCode);
			console.log(tweet_id + ": Content-length: " + res.headers['content-length']);
		}

	    res.on('data', function(chunk) {
	        imageData += chunk;
	    })

	    res.on('end', function(){
	        fs.writeFile(outfile, imageData, 'binary', function(err) {
	            if (err) throw err;
	            console.log(tweet_id + ": User file saved.");
		        collection.update({ id: tweet_id }, { $set: { user_image: outfile }}, function(err) { if (err) throw err; })
	        })
	    })

	})


};

var fetchMediaImage = function(uri, outfile, tweet_id) {

	console.log("\n");
	console.log(tweet_id + ": Fetching media image...");
	console.log(tweet_id + ": URI: " + uri);
	console.log(tweet_id + ": Outfile: " + outfile);

	var request = http.get(uri, function(res) {
	    var imageData = '';
	    res.setEncoding('binary');

	    if (res.hasOwnProperty('statusCode')) {
			console.log(tweet_id + ": Status: " + res.statusCode);
			console.log(tweet_id + ": Content-length: " + res.headers['content-length']);
		}

	    res.on('data', function(chunk) {
	        imageData += chunk;
	    })

	    res.on('end', function(){
	        fs.writeFile(outfile, imageData, 'binary', function(err) {
	            if (err) throw err;
	            console.log(tweet_id + ": Media file saved.");
		        collection.update({ id: tweet_id }, { $set: { media_url: outfile }}, function(err) { if (err) throw err; })
	        })
	    })

	})

};
