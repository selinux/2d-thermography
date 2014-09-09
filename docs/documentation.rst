.. include:: /home/seba/documents/hepia/cours/.conf/labo.rst

.. |cours_name| replace:: ITI - Univérsité d'été
.. |auteur| replace:: Sebastien Chassot, sebastien.chassot@etu.hesge.ch ( GnuPG 0x3F432F8E )
.. |sujet| replace:: Thermographe
.. |creation_date| replace:: 26.08.2014
.. |revision| replace:: 1.0

Thermographe
=============


Introduction
-------------

Le but de ce travail était de générer un thermogramme à l’aide d’un thermomètre infrarouge.

Deux moteurs pas-à-pas permettent d’orienter un capteur IR en 2 dimensions. Ce mécanisme permet de scanner
une surface (X-Y). Pour chaque coordonnée, une mesure de température est effectuée. Au final, les données
permettent de générer une cartographie thermique de la surface sous forme d’image (ou thermogramme).

Matériel
---------

 * Arduino (Uno)
 * thermomètre MLX90614ESF-DCI (version médicale focal 5°)
 * Dual step motor shield
 * 2 switch
 * 2 moteurs PàP de CDRom (tête de lecture)
 * 4 résistances pullup (pour SMBus et les switch)
 * 1 capacité de découplage (recommandé pour SMBus)

Montage
---------

Le montage a été relativement facile. J'ai perdu du temps avec le câblage du capteur MLX90614 en interprétant mal la datasheet (vue de dessus, vue de dessous).

Une des difficultés qui m'a fait perdre le plus de temps a été un problème avec le moteur de l'axe Y. Le signal de la sortie (Y1B) était étrange et le moteur ne faisait que des oscillations. Nous avons changé le driver moteur (A3967) ce qui n'a pas résolu le problème. J'ai finalement découvert qu'une pin touchait le blindage du connecteur USB de l'arduino - un problème fréquent.

Temps de lecture
-----------------

Au début, j'ai essayé d'obtenir la vitesse de déplacement la plus rapide afin de réduire le temps d'acquisition. Les premiers résultats avaient une succession de valeurs identiques (5,6 pixels). Le problème vient du temps de réaction du capteur (la datasheet n'en parle apparemment pas). Pour résoudre le problème, j'ai dû augmenter le temps entre les mesures à 90ms. Ce delay rallonge considérablement le temps de mesure.

Deformation de l'image
-------------------------

L'image est déformée pour plusieurs raisons. Tout d'abord, la distance entre deux points n'est pas la même à 1m qu'à 2m ( x ~= distance*tg(a) ce qui implique que x proportionnel à la distance). De plus si l'on est perpendiculaire à une surface la distance n'est pas la même au centre (leger effet oeil-de-boeuf). Si l'on mesure plusieurs sujets sur des plans diférents, leur résultion respective ne sera pas la même.

Problème d'entrelacement

Calibration
------------

Pour calibrer, j'ai utilisé le laser et des lignes sur le murs. Il y a une petite dérive (perte de pas) que j'ai corrigé "en dure" dans le sketch arduino.

Test de la communication
-------------------------

Pour tester la communication, une constante (10) est envoyée à l'aller et une autre (20). En testant à 115200 bauds, il y avait des erreurs de transmission (réception de 0.1 au lieu de 10)
