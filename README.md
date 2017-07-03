# Degradation3D
Dégradation d'objet 3D afin de les rendre conforme à l'époque (1720)

# Installation

Installer cgal:
sudo apt-get install libcgal-dev
sudo apt-get install libcgal-qt5-dev

cmake .

# Lancer le projet
./launch objAImporter objAExporter sensCoupure placementCoupure variation

# Explication
  Pour les paramètres de lancement du projet :
    - sensCoupure 0,1,2 pour couper des trois façons (X,Y,Z)
    - placementCoupure est l'emplacement de la coupure, compris entre 0 et 1 (0.5 au centre).
    - variation est pour l'écart de la coupure (0.01 à 0.1).
