# Projet d'INFO-H304
- installer git : sudo apt-get git

- pour cloner le répertoire (avoir une version locale)  : 
	* se placer dans le dossier dans lequel on veut placer le répertoire avec le terminal
	* lancer la commande : git clone https://github.com/LucasLefevre/INFO-H304.git

- configurer git
	* git config --global user.email "you@example.com"
	* git config --global user.name "Your Name"


- commandes utiles : 
- 
	* git status : voir le status actuel, très très utile

	* git commit : pour commiter, c-à-d créer une version du code qui pourra être uploadée sur le serveur.  Il ne faut donc jamais      commiter un code non fonctionnel.  Il est possible de revenr à n'importe quel commit à tout moment.
		-  écire le message de commit : indiquer ce qui a été modifié/ajouté
		-  ctrl + X
		-  o (Oui)
		-  entrer nom d'utilisateur/mot de passe
		normalement, c'est bon
		
	* git add <fichier> : à faire quand on crée un nouveau fichier pour l'indexer (ne pas indexer les fichiers exécutables et les .o)
	
	* git pull : télécharge la dernière version depuis le serveur (ne peut se faire que si il n'y a plus rien à commiter localement = pas de changements non enregistrés)
	
	* git push : envoie la dernière version locale (dernier commit) sur le serveur. Tant que ça n'a pas été fait, les autres ne voient pas le nouveau code vu qu'il n'est pas encore sur le serveur.  Si le serveur a une version plus récente que la version locale, il faudra "pull" la version du serveur et fusionner les deux (automatique si il n'y a pas de conflits) avant de pusher.
	
		- En cas de problèmes, "git status" indique souvent des infos intéressante ;)
