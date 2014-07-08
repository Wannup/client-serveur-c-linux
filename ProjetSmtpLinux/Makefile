all: smtp_server smtp_client
smtp_server: serveur_smtp_concurrent.c 
	gcc serveur_smtp_concurrent.c -o smtp_server
smtp_client: client_smtp.c  
	gcc client_smtp.c -o client_smtp

