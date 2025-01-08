# Mallette Pédagogique du SCRIME

### Dispositif pédagogique d'éveil musical par l'exploration sonore

Cette version est basée sur pure data : un simple module de manipulation sonore pilotable par un ensemble prédéterminé de contrôleurs hid (joysticks, gamepads) constitue la brique de base de la mallette, cette brique est instanciée autant de fois qu'il y a de "joueurs".

### NOTES

Les deux machines actuellement en service dans les ateliers pédagogiques sont sous Ubuntu Studio.

Instructions pour l'installation de Pure Data et des dépendances nécessaires à l'utilisation de la mallette :

* mettre à jour les ppa (packages personnels) :
  * `sudo add-apt-repository ppa:pure-data/pure-data`
  * `sudo apt update`
* installer Pure Data (dans `/usr/bin`) :
  * `sudo apt install puredata`
  * copier le raccourci `/usr/bin/pd` sur le bureau
* installer les dépendances :
  * démarrer Pure Data
  * spécifier les chemins à utiliser :
    * créer un dossier `Pd` dans `/home/<username>/Documents`, puis un dossier `externals` dans `Pd`
    * aller dans Fichier > Préférences > Éditer les préférences
    * dans le premier onglet `chemins`, spécifier `/home/<username>/Documents/Pd/externals` comme dossier d'installation des externals, et comme chemin pour "la recherche d'objets, aides, sons, textes et autres fichiers" (l'ajouter grâce au bouton "Nouveau")
  * installer les dépendances :
    * aller dans Aide > Installer des objets supplémentaires
    * rechercher et installer les librairies suivantes : `jl`, `hcs`, `hidraw` et `freeverb~`

Lancer le serveur jack et sélectionner jack comme driver audio depuis pure data : le patch devrait à présent fonctionner !

### Sous Linux :

compiler hidraw :
* `apt install libudev-dev`
* `make`

autoriser pd à accéder aux périphériques hidraw :
* create `/etc/udev/rules.d/71-pd-hidraw.rules`
* add the following line : `KERNEL=="hidraw*", SUBSYSTEM=="hidraw", MODE="0664", GROUP="plugdev"`
* update permissions without rebooting (or reboot to update them automatically) :
  * `sudo udevadm control --reload-rules`
  * `sudo service udev restart`
  * `sudo udevadm trigger`
NB : the current user must belong to the plugdev group for this to work