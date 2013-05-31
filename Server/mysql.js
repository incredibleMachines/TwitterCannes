var mysql = require('mysql');
var connection = mysql.createConnection({
	host: 'localhost',
	port: 8889,
	user: 'root',
	password: 'root',
	database: 'twitter_canneslions'
});

connection.connect(function(err){

	// connection.query('SELECT * FROM tweets', function(err, rows, fields) {
	//   if (err) throw err;

	//   console.log(rows);
	// });

	packet = {
		id: 340221547811835950,
		created_at: 'Fri May 31 11:21:25 +0000 2013',
		user_id: 96929962,
		user_name: 'Matt Felsen',
		user_screen_name: 'mattfelsen',
		text: 'This is a test!!',
		media_id: false,
		media_url: false,
		status: 0,
		category: 0
	}

	connection.query('INSERT INTO tweets SET ?', packet, function(err, result) {
	  if (err) throw err;

	  console.log(result.insertId);
	});

	connection.end();

});