var MongoClient = require('mongodb').MongoClient;
    // , format = require('util').format;

// var url = format("mongodb://%s:%s@%s:%s/%s"
//     , process.env.OPENSHIFT_MONGODB_DB_USERNAME
//     , process.env.OPENSHIFT_MONGODB_DB_PASSWORD
//     , process.env.OPENSHIFT_MONGODB_DB_HOST
//     , parseInt(process.env.OPENSHIFT_MONGODB_DB_PORT)
//     , process.env.OPENSHIFT_APP_NAME)

var url = "mongodb://127.0.0.1/test";

MongoClient.connect(url, function(err, db) {
	if(err) throw err;
	console.log("Connected to Database");

	var collection = db.collection('test');

	// var document = { name:"Matt", title: "Intern" };

	// //insert record
 // 	collection.insert(document, function(err, records) {
 // 		if (err) throw err;
 // 		console.log("Record added as "+records[0]._id);
 // 	});


	collection.find().each(function(err, doc) {
		if (doc != null) {
			console.log(doc);
		} else {
			db.close();
		}
	});
});
