# Projet Scrabble

## Introduction

Le but de ce projet est la programmation d'un joueur automatique au scrabble,
qui déterminera le coup rapportant le plus de points étant donné un plateau de
jeu, l'ensemble des lettres du joueur, et un dictionnaire. Vous pourrez
retrouver les règles du jeu et des tutoriels [facilement sur le
web](https://duckduckgo.com/?q=scrabble+how+to+play&t=h_&ia=web).

Ce projet s'appuie sur la structure de
[GADDAG](https://en.wikipedia.org/wiki/GADDAG) qui permet de déterminer
efficacement étant donné un point de départ sur le plateau les mots qui peuvent
être ajoutés en passant par ce point. Votre rôle consistera à implémenter une
version simplifiée de cette structure, puis de la mettre en œuvre pour chercher
le meilleur coup.

![solver](data/solver.gif)

## Dépôt de base

Ce dépôt constitue une base pour votre projet. Vous y trouverez le nécessaire
pour créer un plateau de jeu vierge, en charger un depuis un fichier, le remplir
et l'afficher. **Vous n'êtes pas obligés d'utiliser ce dépôt**. Libre à vous de
vous créer votre propre code de zéro si vous le préférez, ou de modifier le code
fourni autant que vous le souhaitez pour correspondre à vos besoins. L'important
reste néanmoins que vous rendiez un travail qui soit compilable et lisible par
votre encadrant.

La compilation se fait à l'aide du `Makefile` fourni dans le dossier src, ou via
`cmake` selon vos préférences.

## Gestion du joueur

Votre première tâche consiste à mettre en place le nécessaire pour qu'un joueur
puisse jouer. Vous devez pour celà créer des structures de données pour
matérialiser le sac des lettres et les lettre possédées par les joueurs. Vous
fournirez aussi également des fonctions pour obtenir rapidement le nombre de
points correspondant à une lettre.

En français, le sac de lettres contient initialement les lettres avec les points
suivants :

|      | A | B | C | D | E | F | G | H | I | J | K | L | M |
|------|---|---|---|---|---|---|---|---|---|---|---|---|---|
|Nombre| 9 | 2 | 2 | 3 | 15| 2 | 2 | 2 | 8 | 1 | 1 | 5 | 3 |
|Points| 1 | 3 | 3 | 2 | 1 | 4 | 2 | 4 | 1 | 8 | 10| 1 | 2 |

|      | N | O | P | Q | R | S | T | U | V | W | X | Y | Z |
|------|---|---|---|---|---|---|---|---|---|---|---|---|---|
|Nombre| 6 | 6 | 2 | 1 | 6 | 6 | 6 | 6 | 2 | 1 | 1 | 1 | 1 |
|Points| 1 | 1 | 3 | 8 | 1 | 1 | 1 | 1 | 4 | 10| 10| 10| 10|

**Notez que pour l'instant, on ne traite pas les jokers qui pourront être
ajoutés en bonus.**

### Sac de lettres

Créez une structure de données matérialisant le sac de lettres. Cette structures
sera initialisée avec les lettres dans les quantités données ci-dessus. Cette
structure devra permettre de pouvoir facilement piocher une lettre au hasard
parmi les lettres restantes, c'est à dire que chaque lettre a une probabilité
de tirage proportionnelle au nombre d'exemplaires restant dans le sac. 

En terme de complexité, une structure bien réalisée devra permettre de piocher
une lettre en temps constant.

### Lettres du joueur

Créez une structure de données pour matérialiser les lettre d'un joueur. Sauf
quand il n'y en a plus dans le sac pour recharger, un joueur a toujours 7 lettres
à sa disposition. Votre structure devra être adaptée à l'exploration que vous
ferez par la suite des mots qui sont jouable sur un plateau. Prévoyez également
de pouvoir recharger les lettres d'un joueur depuis le sac de lettres.

### Points des lettres

Fournissez une fonction qui étant donné une lettre fournit le nombre de points
associés.

## GADDAG

Le Gaddag simplifié est une structure de données pour encoder le dictionnaire,
et déterminer rapidement si des mots sont dedans ou non. Nous simplifions ici
cette structure de données pour faciliter son implémentation, en retirant la
minimisation du nombre de nœuds. Votre Gaddag sera donc plus simplement un
arbre.

### Arbre dictionnaire

#### Principe

Un arbre dictionnaire est un arbre dans lequel les arêtes sont étiquetées par
des lettres, et les nœuds sont dotés d'un marqueur indiquant s'ils sont
terminaux ou non. Étant donné un mot, on débute la recherche à la racine. Pour
chaque lettre du mot, on avance le long de l'arête portant cette lettre vers un
enfant. La recherche se termine dès qu'il n'y a pas d'arête étiquetée avec la
lettre souhaitée, ou lorsqu'il n'y a plus de lettres dans le mot, on peut alors
vérifier :

* s'il n'y avait pas d'arête pour une lettre, le mot n'est pas dans le
  dictionnaire ;
* le mot est dans le dictionnaire si et seulement si le nœud sur lequel le
  parcours a abouti est terminal.

Par exemple, sur l'arbre suivant, sur lequel les nœuds terminaux sont colorés en
noir :

![exemple arbre](data/exemple_arbre.png)

Le mot `AMI` est dans le dictionnaire car en partant de la racine, puis en suivant
les arêtes `A` puis `M` puis `I` on arrive sur un nœud terminal. Plus
généralement, les mots reconnus par cet arbre sont `A`, `ARC`, `ART`, `AMI`,
`LE`, `LES`, `LA`, `ET`, `EN`, `ERG`

Le mot `LAS` n'est pas dans le dictionnaire, car après avoir suivi les arêtes
correspondant aux premières lettres `LA` , on aboutit sur un nœud depuis lequel
aucune arête ne part correspondant à la lettre `S`.

Le mot `AM` n'est pas dans le dictionnaire car après avoir suivi les arêtes
correspondant à toutes ses lettres, on aboutit à un nœud qui n'est pas terminal.

#### Insertion

Étant donné un mot à ajouter, le principe consiste à itérer sur les lettres du
mot dans l'ordre de gauche à droite, et à suivre le chemin correspondant dans
l'arbre. Si aucune arête n'existe pour correspondre à une lettre, de nouvelles
arêtes vers de nouveaux nœuds sont créées. Le dernier nœud créé est marqué comme
terminal. Si toutes les arêtes existent, le nœud sur lequel on arrive est marqué
comme terminal.

![vidéo expliquant l'insertion dans un GADDAG](https://myvideo.univ-lyon1.fr/downloads/file/v12664831ff5fwariy1u/?url=media_720_8aPuJULbD7.mp4)



#### Recherche

La recherche consiste à suivre le chemin dans l'arbre, et à vérifier au fur et à
mesure qu'on peut continuer, puis que le nœud final sur lequel on est arrivé est
bien terminal.

#### Liste de mots

Vous trouverez la liste des mots actuellement autorisés au Scrabble dans le
fichier [data/dico.txt](data/dico.txt).

### Rechercher des mots par le milieu, le Gaddag

Les arbres dictionnaires ne sont pas totalement adaptés pour le Scrabble, car
lorsqu'on cherche à ajouter un mot, il faut l'ajouter à partir des lettres déjà
présentes sur le plateau. On identifie donc une case à partir de laquelle
construire notre mot de part et d'autre, verticalement ou horizontalement. Dans
l'exemple suivant, le mot `BATEAU` est placé en travers du mot `BLATTE` :

```
        B
        L
        A
    B A T E A U
        T
        E
```

Pour permettre de chercher des mots *en partant du milieu*, nous allons rajouter
une nouvelle lettre `+`, et pour chaque mot du dictionnaire nous allons créer un
ensemble de mots selon le modèle suivant : pour le mot initial `BATEAU`, nous
allons créer l'ensemble :

```
B+ATEAU
AB+TEAU
TAB+EAU
ETAB+AU
AETAB+U
UAETAB+
```

Avant le `+`, les premières lettres du mot sont renversées, et après le `+` on
retrouve la fin normale du mot.

À partir de la lettre `T` du mot `BLATTE`, on peut chercher dans notre
dictionnaire les mots pouvant se mettre en travers, en suivant un chemin
commençant par `T`. On trouve `TAB+EAU`, ce qui signifie qu'en partant de `T` et
en allant vers la gauche, on peut rajouter `A` puis `B`, ensuite le `+` nous
indique de revenir au `T` initial et de partir vers la droite, pour rajouter les
lettres `E` puis `A`, puis `U`.

Modifiez donc votre arbre dictionnaire pour faire en sorte que pour chaque mot
du dictionnaire, l'ensemble de mots créé avec le `+` selon le modèle précédent
soit ajouté.

## Exploitation du Gaddag

### Les cases par lesquelles passent les mots

Lorsqu'un nouveau mot est joué au scrabble, il doit nécessairement réutiliser
au moins une lettre déjà présente sur le plateau. Le plus simple consiste à
croiser un mot existant comme dans l'exemple précédent :

```
        B
        L
        A
    B A T E A U
        T
        E
```

Ici le mot `BATEAU` est ajouté en réutilisant l'un des T du mot `BLATTE` déjà
présent sur le plateau. Il est également possible de rallonger les mots
existants :

```
        B
        L
        A
    B A T E A U X
        T
        E
```

Ici la lettre `X` a été ajoutée pour étendre le mot `BATEAU` en `BATEAUX`. Il
est enfin possible de rajouter créer un mot adjacent et parallèle à un mot
existant, en s'assurant que tous les mots créés sont valides :

```
        B
        L
        A   C L E S
    B A T E A U X
        T
        E
```

Ici, nous avons joué `CLES` tout en prenant garde  à ce que les mots `CA`, `LU`
et `EX` existent.

Au bilan, dans la mesure où il faut nécessairement réutiliser une lettre
présente, et que pour l'utiliser, il faut placer une nouvelle lettre à côté, on
peut en déduire que **tout nouveau mot passera par une case vide à côté d'une
case occupée**. Lorsque vous cherchez à lister les mots pouvant être ajoutés,
vous pouvez initialiser vos recherches sur ces cases, en essayant
horizontalement et verticalement. Par la suite, vous vous poserez la question
d'accélérer la recherche en choisissant plus judicieusement les cases de départ.

## Lister les coups compatibles

La recherche d'un mot consiste à partir d'une case comme expliqué juste au
dessus, et à choisir une direction, verticale ou horizontale. Ensuite, faites
avancer simultanément deux curseurs, un sur la *case courante* sur le plateau,
et un sur le *nœud courant* dans le Gaddag, initialement sur la racine. Lorsque
le curseur du plateau est sur une case vide, **à la manière d'un parcours en
profondeur**, pour chaque arête dans le Gaddag partant du nœud courant, vous
pouvez ajouter la lettre correspondant à l'arête sur la case du plateau. Lorsque
la case est occupée, vous n'avez pas le choix, il faut utiliser la lettre
présente sur la case. Pour mettre à jour les curseurs :

* le curseur dans le Gaddag avance en suivant l'arête choisie ou forcée
* le curseur sur le plateau avance sur la prochaine case à tester :
  - si nous n'avons pas utilisé de `+` dans le Gaddag, vers l'arrière
  - si nous avons déjà utilisé le `+` dans le Gaddag, vers l'avant
  - si nous utilisons le `+` dans le Gaddag, sur la case voisins de la case de
    départ vers l'avant

Si vous cherchez un mot horizontal, l'avant est à droite, et l'arrière à gauche.
Verticalement, l'avant est vers le bas, et l'arrière vers le haut.

Au fur et à mesure du parcours, à chaque lettre que vous tentez d'ajouter, il
faut également faire attention à vérifier qu'elle ne contribue pas à deux mots à
la fois verticalement et horizontalement. Si c'est le cas, il faut également
tester l'existence du mot perpendiculaire créé. Dans l'exemple précédent, au
moment où le mot `CLES` a été joué horizontalement, il a fallu détecter que la
lettre `C` créait également le mot `CA` verticalement et que ce mot existait.

Lorsque le curseur du plateau est sur une case vide, et que le curseur du Gaddag
est sur un nœud terminal, vous avez trouvé un mot valide.

![vidéo expliquant la recherche de meilleur coup au scrabble](https://myvideo.univ-lyon1.fr/downloads/file/v12664831ffb7w0kqkki/?url=media_720_5SlDw5utsc.mp4)

## Trouver le meilleur coup

Pour trouver le meilleur coup, il faut calculer le score correspondant à chaque
mot possible en respectant les règles du Scrabble :

* pour chaque mot créé ou modifié par le coup
  - pour chaque lettre du mot
    * si la lettre est nouvelle, ajouter sa valeur, éventuellement modifiée par
      le bonus de lettre en dessous
    * sinon ne comptabiliser que la valeur de la lettre, sans le bonus en
      dessous
  - pour chaque bonus de mot sous une nouvelle lettre, multiplier la valeur du
    mot par le bonus.

## Jeu de test

Pour tester votre algorithme, un serveur de test a été mis en place. Pour le
solliciter, contactez l'url suivante :

```
https://liris.cnrs.fr/vincent.nivoliers/scrabble.php?board=<plateau>&rack=<lettres>
```

En remplaçant `<plateau>` par le plateau au format produit par la méthode `save`
de la classe `Board`, et `<lettres>` par les lettres jouables. Vous obtiendrez
alors un `json` indiquant le plateau résultat ainsi que le score correspondant.
Par exemple pour les lettres `AQEIESZ` et le plateau

```
CAIEU......H...
..N........A...
..N....BEAGLE..
.BOF.......O...
..MARGOTERAI...
.JE....O...R...
..E.C..U.......
...PUNTS.......
....M..E.......
.V..U..L.......
.I..L..L.......
.E.....E.......
.TRENDYS.......
.S.............
...............
```

[suivez ce lien](https://liris.cnrs.fr/vincent.nivoliers/scrabble.php?board=CAIEU......H.....N........A.....N....BEAGLE...BOF.......O.....MARGOTERAI....JE....O...R.....E.C..U..........PUNTS...........M..E........V..U..L........I..L..L........E.....E........TRENDYS........S............................&rack=AQEIESZ) qui vous proposera un score de 48 points avec le résultat suivant :

```
CAIEU......HIEZ
..N........A...
..N....BEAGLE..
.BOF.......O...
..MARGOTERAI...
.JE....O...R...
..E.C..U.......
...PUNTS.......
....M..E.......
.V..U..L.......
.I..L..L.......
.E.....E.......
.TRENDYS.......
.S.............
...............
```

## Bonus

### Implémenter les jokers 

Un joker peut remplacer n'importe quelle lettre. Une fois le joker posé, on
identifie la lettre qu'il remplace, et il ne peut plus être utilisé par les
nouveaux mots placés que comme cette lettre.

### Implémenter un joueur pour une partie classique

Il s'agit ici d'une question ouverte pour laquelle il n'y a pas de bonne
méthode. Il faut être inventif. Le joueur qui joue le meilleur coup à chaque
tour est un joueur glouton. En jouant de cette façon, il ne fait pas attention
aux opportunités qu'il laisse à l'adversaire, et lui permet potentiellement de
mettre beaucoup de points. Le but ici est de faire un joueur plus intéressant
qui serait capable de ne pas jouer le meilleur coup s'il estime qu'un autre coup
moins intéressant améliore ses chances pour la victoire finale. Une piste pour
ce joueur consiste à utiliser une stratégie classique en IA : la [recherche
arborescente
Monte-Carlo](https://fr.wikipedia.org/wiki/Recherche_arborescente_Monte-Carlo).
Pour simplifier, étant donné les lettres dans la main du joueur et celles sur le
plateau, il est possible de lister les lettres restant à jouer. Ainsi, on peut
simuler une fin de partie en considérant les deux joueurs comme des gloutons qui
utilisent la stratégie du meilleur coup, et estimer pour chaque coup la
probabilité de mener à une victoire. On se donne ainsi un budget de temps, et
tant qu'on en a, on lance des parties aléatoires pour estimer la probabilité de
succès d'un coup. 
