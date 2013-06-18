Instructions:
	Install Homebrew
		http://mxcl.github.io/homebrew/
		
		Run: 
		Brew Doctor & Brew Update

	Install Mongodb through Homebrew
		brew install mongodb
 		
	Install Node.js
		http://nodejs.org/

	Create image folder & set permissions
		sudo mkdir /twitter
		sudo mkdir /twitter/user_images
		sudo mkdir /twitter/media_images

		sudo chmod -R 777 /twitter
		sudo chmod -R 777 /twitter/user_images
		sudo chmod -R 777 /twitter/media_images
	
	Go into LocalServer root folder
		cd /path/to/LocalServer
	
	Install Node Dependencies
		npm install

	Run Polling Server
		sudo forever start -o pollRemoteOutput.log -e pollRemoteError.log -l pollRemoteLogfile.log -a pollRemoteServer.js 
		tail -f -n 20 ~/.forever/pollRemoteLogfile.log

	Run Local Server
		node localServer.js
