Instructions:
-------------

	1. SSH into Server
		ssh im@cannes.incrediblemachin.es -p 30000

	2. Go into RemoteServer root folder
		cd /home/im/node
	
	3. Install Node Dependencies
		npm install

	4. Run Stream Server
		sudo forever start -o log/output.log -e log/error.log -l log/logfile.log -a stream.js

	5. Run Remote Server
		sudo forever start -o log/expressRemoteOutput.log -e log/expressRemoteError.log -l log/expressRemoteLogfile.log -a remoteServer.js
