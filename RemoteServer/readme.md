Instructions:

	#SSH into Server
		ssh im@cannes.incrediblemachin.es -p 30000

	#Go into RemoteServer root folder
		cd /home/im/node
	
	#Install Node Dependencies
		npm install

	#Run Stream Server
		sudo forever start -o log/output.log -e log/error.log -a log/logfile.log stream.js

	#Run Remote Server
		sudo forever start -o log/expressRemoteOutput.log -e log/expressRemoteError.log remoteServer.js
