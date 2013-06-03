logtime("Loading modules...");

var request = require("request");
var jsdom = require("jsdom"); 

var url = "http://t.co//QhT6KyFnTt";

logtime("Modules loaded!");

// Make request, don't follow redirects. We only want the headers & to be as quick as possible
request({ uri: url, followRedirect: false }, function(error, response, body) {
	logtime("Making request");

	// Check for 301, a redirect for permanent move
	if (response.statusCode == 301) {
		logtime("Getting redirected");
		dest = response.headers.location;

		// Check if we're getting redirected to instagram
		// Looks to make sure the url string begins with "http://instagram.com"
		// (No match would return -1)
		if (dest.toLowerCase().indexOf("http://instagram.com") == 0) {
			logtime("Redirected to Instagram")

			// If so, make a new request to the redirected URL, and do follow redirects
			request(url, function(error, response, body) {

				jsdom.env({ html: body, scripts: ['http://code.jquery.com/jquery-1.6.min.js'] }, function (err, window) {
				    var $ = window.jQuery;
				    photo = $('img.photo').attr('src');
				    logtime(photo);
				});

			});

		} else {
			logtime("Redirected elsewhere");
			logtime(dest);
		}
	} else {
		logtime("No redirect");
	}
	

});

function logtime(str) {
	var d = new Date();
	var d_str = d.getSeconds() + ' ' + d.getMilliseconds() + ' ';
	console.log(d_str + str);
}