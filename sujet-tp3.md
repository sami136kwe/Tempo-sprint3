# Travail pratique 3: Extension de l'application `tempo`

Ce document décrit le travail pratique 3 du cours [INF3135 Construction et
maintenance de logiciels](https://etudier.uqam.ca/cours?sigle=INF3135) au
trimestre d'automne 2025. Le cours est enseigné par Alexandre Blondin Massé,
professeur au [département d'informatique](http://info.uqam.ca/),
à l'[Université du Québec à Montréal](https://uqam.ca/).

Le travail doit être accompli de façon **individuelle**. Il doit être remis au
plus tard le **dimanche 28 décembre à 23h59**. À partir de minuit, une pénalité
de **2 points** par heure de retard sera appliquée.

## Objectifs pédagogiques

Les principaux objectifs pédagogiques visés sont les suivants:

* Intégrer des **bibliothèques** C à un projet existant
* Organiser adéquatement votre développement à l'aide des **branches** d'un
  logiciel de contrôle de versions (Git)
* Assurer la maintenance d'un programme divisé en plusieurs modules à l'aide
  d'un **Makefile**
* Ajouter de **nouvelles fonctionnalités** à un programme existant
* Suivre un **style de programmation imposé**
* Rédiger des **tests fonctionnels** couvrant les modifications que vous avez
  apportées à l'application
* Vous assurer de ne pas introduire de **régression** lorsque vous apportez des
  modifications à un programme
* Mettre à jour l'**intégration continue** en fonction des nouvelles
  dépendances introduites
* Mettre à jour la **documentation** pour refléter les modifications apportées
  à l'application

## Description du travail

Lors deux premiers travaux pratiques (voir [sujet du TP1](sujet-tp1.md) et
[sujet du TP2](sujet-tp2.md)), vous avez travaillé au développement d'une
application nommée `tempo`. Dans ce troisième travail, vous devez continuer
à contribuer à cette application de façon un peu moins dirigée, en ajoutant de
nouvelles fonctionnalités, en réorganisant la base de code, en ajoutant des
tests unitaires et des tests fonctionnels, en mettant à jour la documentation
ou encore en étendant l'intégration continue. Afin de s'assurer que tout le
monde travaille à partir de la même base et pour faciliter la correction, nous
vous fournissons la solution des deux premiers travaux pratiques. Vous devrez
donc dupliquer (*fork*) le dépôt courant dans votre espace personnel, le rendre
privé (afin d'éviter le plagiat) et donner accès aux utilisateurs `blondin_al`
et `guite-vinet.julien` en mode *Maintainer*, comme pour les deux premiers
travaux.

Vous devez accomplir les 3 tâches suivantes:

1. `test-timeseries-libtap`: Ajouter des tests unitaires pour le module
   `timeseries` à l'aide de la bibliothèque Libtap
2. `from-json`: Ajouter l'option `-J|--from-json` aux sous-commandes
   `describe`, `interpolate` et `show` permettant de lire des séries
   temporelles en entrée à l'aide de la bibliothèque
   [Jansson](https://github.com/akheron/jansson?tab=readme-ov-file)
3. `to-gnuplot`: Introduire la sous-commande `gnuplot` permettant de générer un
   script Gnuplot qui dessine la série temporelle lue sur l'entrée standard

Ces tâches sont détaillées dans les sections suivantes. Noter qu'elles doivent
être réparties sur des branches nommées exactement `test-timeseries-libtap`,
`from-json` et `to-gnuplot`, en minuscules, en suivant la syntaxe `kebab-case`
pour la nomenclature.

## Tâches préliminaires à accomplir

Dans un premier temps, vous devez accomplir les actions suivantes:

1. Lisez le sujet du travail jusqu'à la fin
2. Créez un clone (*fork*) du [dépôt du
   projet](https://gitlab.info.uqam.ca/inf31351/20253/inf3135-253-tp3)
3. Assurez-vous que le dépôt soit privé, pour éviter le plagiat
4. Donnez accès à votre dépôt aux utilisateurs `blondin_al` et
   `guite-vinet.julien` en mode *Maintainer* (pour la correction)
5. Familiarisez-vous avec le contenu du dépôt, en étudiant chacun des fichiers
   présents (`README.md`, `sujet-tp1.md`, `sujet-tp2.md`, `sujet-tp3.md`,
   répertoire `bats`, `.gitlab-ci.yml`, etc.).
6. Testez les différentes cibles des fichiers Makefile, notamment `make build`,
   `make test-bats`, `make test-libtap` et `make test`

Vous êtes maintenant prêt ou prête à commencer les tâches demandées!

## Tâche 1: `test-timeseries-libtap`

La première tâche consiste à ajouter des tests unitaires pour le module
`timeseries` à l'aide de la bibliothèque
[Libtap](https://github.com/zorgnax/libtap), en vous inspirant des tests déjà
existants pour le module `datetime`.

Les tests ajoutés devraient minimalement couvrir toutes les fonctions publiques
introduites par le module `timeseries`. Il faut également mettre à jour les
fichiers Makefiles correspondants afin que les tests soient exécutés lorsqu'on
entre les commandes `make test-libtap` et `make test`.

Plus spécifiquement, vous devez minimalement apporter les modifications
suivantes, en plaçant tous vos *commits* sur la branche
`test-timeseries-libtap`:

1. Un fichier `test_timeseries.c` devrait être ajouté avec les différents tests
   unitaires
2. Les différents embranchements de chaque fonction publique du module
   `timeseries` devraient être testés au maximum, en minimisant la redondance
3. Si des bogues sont découverts lors des tests unitaires, ceux-ci doivent être
   corrigés
4. Si de nouvelles fonctions sont introduites, des *docstrings* doivent
   accompagner leur déclaration
5. Si vous avez modifié des fonctions déjà existantes dans le code, mettre
   à jour les *docstrings*
6. Les différents fichiers Makefiles présents dans le projet doivent être mis
   à jour pour inclure les nouveaux tests ajoutés
7. Vous assurer que toutes vos modifications respectent le style de
   programmation existant
8. Répartir vos modifications en *commits* atomiques, en formatant vos messages
   selon le style demandé
9. Vos modifications ne doivent pas introduire de régression, notamment au
   niveau de la gestion de la mémoire (incluant dans les tests)

## Tâche 2: `from-json`

La deuxième tâche consiste à supporter le format JSON pour représenter des
séries temporelles lors de la lecture. Par exemple, la série temporelle

```
2025-09-01T00:00:00
0 10
28800 40
57600 15
86400 35
115200 50
144000 25
```

peut être représentée au format JSON par

```
{
  "origin": "2025-09-01T00:00:00",
  "observations": [
    {"offset": 0, "value": 10},
    {"offset": 28800, "value": 40},
    {"offset": 57600, "value": 15},
    {"offset": 86400, "value": 35},
    {"offset": 115200, "value": 50},
    {"offset": 144000, "value": 25}
  ]
}
```

Vous devez minimalement apporter les modifications suivantes, en plaçant tous
vos *commits* sur la branche `from-json`:

1. Introduire l'option `-J|--from-json` aux sous-commandes `describe`,
   `interpolate` et `show`
2. Ajouter des tests fonctionnels (Bats) illustrant la nouvelle option et ne
   pas introduire de régression dans les tests Bats existants
3. Mettre à jour le message d'aide du programme, de même que le fichier
   `README` pour refléter les modifications apportées
4. Ajouter de nouvelles fonctions avec leur *docstring* et mettre à jour la
   *docstring* de toute fonction modifiée
5. Introduire la bibliothèque Jansson au projet, en mettant à jour les
   Makefiles, le fichier `.gitlab-ci.yml` et le fichier `README`
6. Vous assurer que toutes vos modifications respectent le style de
   programmation existant
7. Répartir vos modifications en *commits* atomiques, en formatant vos messages
   selon le style demandé
8. Vos modifications ne doivent pas introduire de régression, notamment au
   niveau de la gestion de la mémoire (incluant dans les tests)

Noter qu'il n'est pas obligatoire d'ajouter des tests unitaires Libtap dans
cette tâche, mais pouvez le faire si vous le souhaitez.

## Tâche 3: `to-gnuplot`

Votre troisième tâche consiste à introduire une nouvelle sous-commande
`gnuplot`, qui affiche sur la sortie standard un script Gnuplot valide
permettant de tracer un graphe de la série temporelle fournie en entrée. Vous
pouvez vous inspirer du script disponible dans le répertoire
[doc](doc/plot-timeseries). Notez que le programme `tempo` ne doit pas appeler
Gnuplot, mais seulement produire le script sur la sortie standard.

On s'attend donc au comportement suivant:

```
# On utilise tempo pour sauvegarder le script dans plot-timeseries.gp
$ tempo gnuplot < examples/6.ts > plot-timeseries.gp
# On appelle le script
$ gnuplot -e "set output 'timeseries.png'" plot-timeseries.gp
# On ouvre timeseries.png pour voir le graphique
```

Également, vous devez, pour cette tâche, inclure une requête d'intégration
(*merge request*) illustrant la nouvelle fonctionnalité introduite.

Plus spécifiquement, vous devez minimalement apporter les modifications
suivantes, en plaçant tous vos *commits* sur la branche
`to-gnuplot`:

1. Introduire la sous-commande `gnuplot`, avec l'option `-J|--from-json`, de
   sorte qu'on puisse lire une série temporelle au format JSON
2. Ajouter des tests fonctionnels (Bats) illustrant que la nouvelle
   sous-commande produit un script Gnuplot valide, c'est-à-dire que Gnuplot
   exécute le script sans erreur
3. Mettre à jour le message d'aide du programme, de même que le fichier
   `README` pour refléter les modifications apportées
4. Ajouter de nouvelles fonctions avec leur *docstring* et mettre à jour la
   *docstring* de toute fonction modifiée
5. Mettre à jour le fichier `.gitlab-ci.yml` pour tenir compte de la dépendance
   à Gnuplot
6. Vous assurer que toutes vos modifications respectent le style de
   programmation existant
7. Répartir vos modifications en *commits* atomiques, en formatant vos messages
   selon le style demandé
8. Vos modifications ne doivent pas introduire de régression, notamment au
   niveau de la gestion de la mémoire (incluant dans les tests)
9. Rédiger une requête d'intégration avec un titre pertinent, une description
   détaillée illustrant le comportement de l'application avant et après les
   modifications, avec au moins un exemple

## Clone et création du dépôt

Comme pour les travaux pratiques précédents, vous devez cloner le dépôt fourni
et l'héberger sur la plateforme [Gitlab du
département](https://gitlab.info.uqam.ca/). Votre dépôt devra se nommer
**exactement** `inf3135-253-tp3` et l'URL devra être **exactement**
`https://gitlab.info.uqam.ca/<utilisateur>/inf3135-253-tp3`, où `<utilisateur>`
doit être remplacé par votre nom d'utilisateur. Il devra être **privé** et
accessible seulement par vous et par les utilisateurs `blondin_al` et
`guite-vinet.julien`.

## Git

Il est important de continuer à utiliser correctement Git pour suivre
l'évolution de votre projet. N'oubliez pas de bien configurer correctement
votre fichier `.gitconfig` qui permet de vous identifier comme auteur de
*commits*, en y indiquant vos **véritables** prénom, nom et courriel. Comme
pour les travaux pratiques précédents, les messages de *commit* doivent suivre
[la convention de Chris Beams](https://chris.beams.io/posts/git-commit/),
adaptée au français:

1. Séparer le sujet (la première ligne) du corps (le reste du message) par une
   ligne vide
2. Limiter le sujet à 50 caractères
3. Commencer le message par une lettre majuscule
4. Ne pas terminer le sujet avec un point
5. Utiliser un verbe à l'indicatif présent comme premier mot du sujet
6. Limiter les lignes du corps du message à 72 caractères
7. Utiliser le corps pour expliquer *ce que* (*what*) la modification apporte
   et *pourquoi* (*why*) elle apporte cette modification, plutôt que *comment*
   (*how*) elle apporte une modification

Vous pouvez vous inspirer des messages de *commit* déjà existants dans
l'historique. Aussi, n'oubliez pas de mettre les fichiers `.gitignore` du dépôt
si nécessaire en fonction de votre environnement de développement. Assurez-vous
de ne pas versionner de fichiers inutiles (les fichiers binaires, entre autres,
mais pas seulement).

**Attention!**. Comme pour le travail pratique 2, vous ne devez pas modifier la
branche `master`. Plutôt, vous devez remettre trois branches, sans les
fusionner sur la branche `master`.

Finalement, afin de bien décomposer le travail en branches, vous devez
respecter les contraintes suivantes:

* Les deux premières branches (`test-timeseries-libtap` et `from-json`) doivent
  toutes démarrer de la branche `master` du dépôt de référence et être
  complètement indépendantes l'une de l'autre
* La branche `to-gnuplot` doit démarrer depuis le dernier *commit* de la
  branche `from-json`, puisqu'elle dépend des modifications apportées sur cette
  dernière

## Langue

Comme pour le premier travail pratique, le projet est développé dans deux
langues, soit le français et l'anglais. On rappelle les règles suivantes:

* Tous les messages de *commit* doivent être rédigés en français
* Le fichier `README.md` doit être rédigé en français
* Les commentaires et la documentation du code source (fichier `.c`, fichier
  `.bats`, etc.) doivent être rédigés en anglais

Cette convention n'est pas universelle, mais elle est souvent employée dans les
milieux bilingues.

## Barème

Les points sont répartis selon la grille suivante:

| Élément             |  Points |
|:--------------------|--------:|
| Tâche 1             |     /20 |
| Tâche 2             |     /25 |
| Tâche 3             |     /35 |
| Utilisation de Git  |     /20 |
| Total               |    /100 |

Plus précisément, les éléments suivants seront pris en compte:

- **Tâche 1 (20 points)**: L'application compile sans erreur et sans
  avertissement. La mémoire est gérée correctement. Un fichier
  `test_timeseries.c` a été ajouté. Il contient des tests unitaires Libtap
  couvrant les différentes fonctions publiques fournies par le module
  `timeseries`. Si des bogues ont été découverts, ils ont été corrigés. Les
  *docstrings* du code source ont été mises à jour ou ajoutées s'il y a lieu.
  Les fichiers Makefiles ont été mis à jour correctement. Le style de
  programmation existant est respecté. Les modifications n'introduisent pas de
  régression.

- **Tâche 2 (25 points)**: L'application compile sans erreur et sans
  avertissement. La mémoire est gérée correctement. L'option `-J|--from-json`
  a été introduite pour chacune des sous-commandes. Des tests fonctionnels ont
  été ajoutés afin d'illustrer la nouvelle option. Les *docstrings* du code
  source ont été mises à jour ou ajoutées aux nouvelles fonctions, s'il
  y a lieu. Le message d'aide du programme a été mis à jour correctement. Les
  fichiers Makefiles, ainsi que le fichier README, ont également été mis
  à jour. La bibliothèque Jansson a été intégrée correctement dans la base de
  code, dans les fichiers Makefiles ainsi que dans le fichier `.gitlab-ci.yml`.
  Le style de programmation existant est respecté. Les modifications
  n'introduisent pas de régression.

- **Tâche 3 (35 points)**: L'application compile sans erreur et sans
  avertissement. La mémoire est gérée correctement. La sous-commande `gnuplot`
  a été introduite correctement et elle supporte l'option `-J|--from-json`.
  Des nouveaux tests ont été ajoutés pour refléter l'ajout de la nouvelle
  sous-commande et de son option. Les *docstrings* du code source ont été mises
  à jour ou ajoutées aux nouvelles fonctions. Le fichier `README.md`, l'aide
  (`tempo help`), les Makefiles et le fichier `.gitlab-ci.yml` ont été mis
  à jour, s'il y a lieu. Le style de programmation existant est respecté. Les
  modifications n'introduisent pas de régression. Une requête d'intégration
  avec titre pertinent, description détaillée et exemple, a été rédigée.

- **Utilisation de Git (20 points)**: Les modifications sont réparties en
  *commits* atomiques. Le fichier `.gitignore` est complet. Les messages de
  *commit* sont significatifs, uniformes et suivent la convention suggérée. Les
  *commits* sont répartis sur des branches nommées respectivement
  `test-timeseries-libtap`, `from-json` et `to-gnuplot`. Chaque *commit* est
  pertinent pour la branche sur laquelle il se trouve. Les branches des
  2 premières tâches sont indépendantes, alors que la branche de la 3e tâche
  apparaît à la suite de la 2e tâche. Noter que si vous n'avez pas beaucoup
  avancé dans certaines des tâches demandées, vous ne pourrez pas obtenir
  beaucoup de points sur ce critère.

## Pénalités

Si votre programme ne compile pas pour une tâche donnée, une pénalité pouvant
aller jusqu'à **100%** pourra être appliquée pour la tâche en question.

**Note**: vous n'avez pas besoin d'insérer votre code permanent dans le fichier
`README.md`: nous avons établi l'association entre votre nom d'utilisateur et
votre code permanent dans le premier travail pratique.

En outre, si vous ne respectez pas les critères suivants, une pénalité pouvant
aller jusqu'à **50%** pourra être appliquée:

- Votre dépôt doit se nommer **exactement** `inf3135-253-tp3`
- L'URL de votre dépôt doit être **exactement**
  `https://gitlab.info.uqam.ca/<utilisateur>/inf3135-253-tp3` où
  `<utilisateur>` doit être remplacé par votre identifiant
- Les utilisateurs `blondin_al` et `guite-vinet.julien` doivent avoir accès
  à votre projet en mode *Maintainer*.
- Votre dépôt doit être un *fork* du [gabarit
  fourni](https://gitlab.info.uqam.ca/inf31351/20253/inf3135-253-tp3).
- Votre dépôt doit être **privé**.

## Remise

La remise se fait automatiquement en ajoutant les utilisateurs `blondin_al` et
`guite-vinet.julien` en mode *Maintainer*, vous n'avez rien de plus à faire.
À moins d'indication contraire de votre part, ce sera le dernier (*commit*)
disponible sur chacune de vos branches qui sera considéré pour la correction.
