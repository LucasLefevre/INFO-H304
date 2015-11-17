# Projet d'INFO-H304
## INSTALLATION ET CONFIGURATION
- Installer git : sudo apt-get git

- Se créer un compte sur Github et me donner le nom d'utilisateur pour que je vous ajoute avec les permissions de lecture/écriture 


- Pour cloner le répertoire (avoir une version locale, sur votre ordinateur du code)  : 
	* se placer dans le dossier dans lequel on veut placer le répertoire avec le terminal
	* lancer la commande : git clone https://github.com/LucasLefevre/INFO-H304.git

- Configurer git
	* git config --global user.email "you@example.com"
	* git config --global user.name "Your Name"


## COMMANDES UTILES 
- **git status** : voir le status actuel, très très utile
- **git commit** : pour commiter, c-à-d créer une version du code qui pourra être uploadée sur le serveur.  Il ne faut donc jamais commiter un code non fonctionnel.  Il est possible de revenr à n'importe quel commit à tout moment.
	*  écire le message de commit : indiquer ce qui a été modifié/ajouté
	*  ctrl + X
	*  o (Oui)
	*  Entrer nom d'utilisateur/mot de passe, normalement, c'est bon
		
- **git add <fichier>** : à faire quand on crée un nouveau fichier pour l'indexer (ne pas indexer les fichiers exécutables et les .o).  A faire aussi quand on a modifié un fichier pour que la modification soit prise en compte pour le prochain commit.
	
- **git pull** : télécharge la dernière version depuis le serveur (ne peut se faire que si il n'y a plus rien à commiter localement = pas de changements non enregistrés)
	
- **git push** : envoie la dernière version locale (dernier commit) sur le serveur. Tant que ça n'a pas été fait, les autres ne voient pas le nouveau code vu qu'il n'est pas encore sur le serveur.  Si le serveur a une version plus récente que la version locale, il faudra *pull* la version du serveur et fusionner les deux (automatique si il n'y a pas de conflits) avant de *pusher*.
	
		
	* En cas de problèmes, "git status" indique souvent des infos intéressante ;)
