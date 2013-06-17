Instructions:
-------------

1. SSH into Server
	*ssh im@cannes.incrediblemachin.es -p 30000

2. Go into RemoteServer root folder
	*cd /home/im/node
	
3. Install Node Dependencies
	*npm install

4. Run Stream Server
	*sudo forever start -o log/output.log -e log/error.log -a log/logfile.log stream.js

5. Run Remote Server
	*sudo forever start -o log/expressRemoteOutput.log -e log/expressRemoteError.log remoteServer.js
