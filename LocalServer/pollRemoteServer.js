var http = require('http');

var MongoClient = require('mongodb').MongoClient
	,Server = require('mongodb').Server
	,mongo = new MongoClient(new Server('localhost', 27017))
	,BSON = require('mongodb').BSONPure;
	
var db, collection;

mongo.open(function(err,mongo){
	
	db = mongo.db('twitterCannesLions');
	collection = db.collection('tweets');
	
});


setInterval(checkRemote, 1000*60); //one minute

function checkRemote(){
	var body=''; 
	var json;
	var timestamp;
	
	collection.find().sort({"approved_at":-1}).limit(1).toArray( function(err,doc){
		
		if(!doc.length){
			//console.log('Empty');
			timestamp = '2013-06-12+00:00:00';
			console.log(timestamp);
			
		}else{
			var newtime = doc[0].approved_at;
			timestamp=newtime.replace(' ','+');
			console.log(timestamp);
		}
		var options = {

			host: 'ec2-23-23-188-178.compute-1.amazonaws.com',
			port: 9000,
			path: '/?since='+timestamp+'&limit=0'
	
		};

		http.get(options,function(res){
			
			console.log(res.statusCode);
			
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
				"user_image":json.user_image,
				"text":json.text,
				"media_url":json.media_url,
				"lang":json.lang,
				"status":json.status,
				"category":json.category, //
				"starred": json.starred, //
				"shown_count": 0,
				"last_shown": "",
				"id":json.id
				
			};
				
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