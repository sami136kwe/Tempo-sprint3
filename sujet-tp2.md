# Travail pratique 2: Extension de l'application `tempo`

Ce document décrit le travail pratique 2 du cours [INF3135 Construction et
maintenance de logiciels](https://etudier.uqam.ca/cours?sigle=INF3135) au
trimestre d'automne 2025. Le cours est enseigné par Alexandre Blondin Massé,
professeur au [département d'informatique](http://info.uqam.ca/),
à l'[Université du Québec à Montréal](https://uqam.ca/).

Le travail doit être accompli de façon **individuelle**. Il doit être remis au
plus tard le **dimanche 23 novembre à 23h59**. À partir de minuit, une pénalité
de **2 points** par heure de retard sera appliquée.

## Objectifs pédagogiques

Les principaux objectifs pédagogiques visés sont les suivants:

* Approfondir votre maîtrise du langage de **programmation C**
* Organiser adéquatement votre développement à l'aide des **branches** d'un
  logiciel de contrôle de versions (Git)
* Vous familiariser avec la compilation d'un programme divisé en plusieurs
  modules à l'aide d'un **Makefile**
* Vous habituer à suivre un **style de programmation imposé**
* Rédiger des **tests fonctionnels** couvrant les modifications que vous avez
  apportées à l'application
* Vous assurer de ne pas introduire de **régression** lorsque vous apportez des
  modifications à un programme
* Mettre à jour la **documentation** pour refléter les modifications apportées
  à l'application

## Description du travail

Lors du [travail pratique 1](sujet-tp1.md), vous avez eu à mettre en place les
bases d'une application nommée `tempo`. Dans ce deuxième travail, vous devez
apporter différentes modifications à cette application, que ce soit pour des
raisons de maintenance, d'ajout de nouvelles fonctionnalités ou de
réorganisation de la base de code. Afin de s'assurer que tout le monde
travaille à partir de la même base et pour faciliter la correction, nous vous
fournissons la solution du premier travail pratique. Vous devrez donc dupliquer
(*fork*) le dépôt courant dans votre espace personnel, le rendre privé (afin
d'éviter le plagiat) et donner accès aux utilisateurs `blondin_al` et
`guite-vinet.julien` en mode *Maintainer*, comme pour le premier travail.

Vous devez accomplir les 4 tâches suivantes:

1. `validate-subcommands`: Valider de façon plus systématique si les
   sous-commandes sont correctement utilisées
2. `dynamic-timeseries`: Rendre `struct Timeseries` dynamique
3. `interpolate-subcommand`: Introduire la sous-commande `interpolate`
4. `interpolate-step-option`: Introduire l'option `-s|--step` à la
   sous-commande `interpolate`

Ces tâches sont détaillées dans les sections suivantes. Noter qu'elles doivent
être réparties sur des branches nommées exactement `validate-subcommands`,
`dynamic-timeseries`, `interpolate-subcommand` et `interpolate-step-option`, en
suivant la syntaxe `kebab-case` pour la nomenclature.

## Tâches préliminaires à accomplir

Dans un premier temps, vous devez accomplir les actions suivantes:

1. Lisez le sujet du travail jusqu'à la fin
2. Créez un clone (*fork*) du [dépôt du
   projet](https://gitlab.info.uqam.ca/inf31351/20253/inf3135-253-tp2)
3. Assurez-vous que le dépôt soit privé, pour éviter le plagiat
4. Donnez accès à votre dépôt aux utilisateurs `blondin_al` et
   `guite-vinet.julien` en mode *Maintainer* (pour la correction)
5. Familiarisez-vous avec le contenu du dépôt, en étudiant chacun des fichiers
   présents (`README.md`, `sujet-tp1.md`, `sujet-tp2.md`, répertoire `bats`,
   `.gitlab-ci.yml`, etc.).
6. Testez les différentes cibles des fichiers Makefile, notamment `make build`
   et `make test`

Vous êtes maintenant prêt ou prête à commencer les tâches demandées!

## Tâche 1: `validate-subcommands`

La première tâche consiste essentiellement à mieux valider l'utilisation des
différentes sous-commandes disponibles (`help`, `describe` et `show`). Plus
spécifiquement, si l'utilisateur fournit des arguments supplémentaires à l'une
de ces sous-commandes, un message d'erreur doit être affiché sur la sortie
d'erreur, en retournant le code d'erreur `1`. Par exemple:

```
$ bin/tempo help a b
error: too many arguments to 'help' subcommand
$ echo $?
1
$ bin/tempo show a < examples/2.ts
error: too many arguments to 'show' subcommand
```

Vous devez également ajouter des tests Bats qui montrent que chacune des
3 sous-commandes est correctement validée. Plus spécifiquement, vous devez
minimalement apporter les modifications suivantes, en plaçant tous vos
*commits* sur la branche `validate-subcommands`:

1. Afficher le message `error: too many arguments to 'SUBCOMMAND' subcommand`
   sur la sortie d'erreur, où `SUBCOMMAND` est la sous-commande qui a été
   fournie en cas d'erreur
2. Retourner le code `1` lorsque l'erreur survient
3. Ajouter des tests Bats qui confirment que la validation est correctement
   effectuée
4. Si vous avez introduit de nouvelles fonctions dans le code, y ajouter les
   *docstrings*
5. Si vous avez modifié des fonctions déjà existantes dans le code, mettre
   à jour les *docstrings*
6. Vous assurer que toutes vos modifications respectent le style de
   programmation existant
7. Répartir vos modifications en *commits* atomiques, en formatant vos messages
   selon le style demandé dans le cadre du travail pratique 1

## Tâche 2: `dynamic-timeseries`

La deuxième tâche consiste à rendre la structure de données `Timeseries` plus
flexible. Considérez la déclaration actuelle:

```c
// A timeseries
struct Timeseries {
  // The start datetime of the timeseries
  struct Datetime start_datetime;
  // The last datetime of the timeseries
  struct Datetime last_datetime;
  // The size of the timeseries
  unsigned int size;
  // The offsets of the observations
  int offsets[MAX_SIZE];
  // The observed values
  int values[MAX_SIZE];
};
```

On remarque que la taille de la série temporelle ne peut pas excéder
`MAX_SIZE`. De plus, on gaspille de l'espace mémoire si on fixe une valeur trop
grande pour `MAX_SIZE`. Pour corriger la situation, on peut utiliser
l'allocation dynamique. Pour cela, il faut supprimer la constante `MAX_SIZE` et
remplacer la déclaration par celle-ci:

```c
// A timeseries
struct Timeseries {
  // The start datetime of the timeseries
  struct Datetime start_datetime;
  // The last datetime of the timeseries
  struct Datetime last_datetime;
  // The size of the timeseries
  unsigned int size;
  // The offsets of the observations
  int* offsets;
  // The observed values
  int* values;
};
```

En plus de rendre la création de séries temporelles plus flexibles, vous devez
gérer le cas hypothétique où on manquerait de mémoire (c'est-à-dire que
l'allocation dynamique échoue). Dans ce cas, vous devez afficher le message
`error: out of memory (timeseries)` et arrêter l'exécution du programme avec le
code d'erreur `3`.

Plus spécifiquement, vous devez minimalement apporter les modifications
suivantes, en plaçant tous vos *commits* sur la branche `dynamic-timeseries`:

1. Réorganiser le module `timeseries` (`timeseries.h`/`timeseries.c`) afin de
   supporter l'allocation dynamique pour une série temporelle
2. Ne pas introduire de problème de mémoire
3. Ne pas introduire de régression dans les tests Bats existants
4. Si vous avez introduit de nouvelles fonctions dans le code, y ajouter les
   *docstrings*
5. Si vous avez modifié des fonctions déjà existantes dans le code, mettre
   à jour les *docstrings*
6. Vous assurer de respecter le style de programmation existant
7. Répartir vos modifications le plus possible en différents *commits*
   atomiques, en formatant vos messages selon le style demandé dans le cadre du
   travail pratique 1

## Tâche 3: `interpolate-subcommand`

Votre troisième tâche consiste à introduire une nouvelle sous-commande
`interpolate`, qui rapporte sur la sortie standard, en plus des observations,
des valeurs interpolées (en prenant la partie entière) d'une série temporelle,
en utilisant un pas de temps de 1 seconde.

Par exemple, supposons que le fichier `examples/3_10.ts` contienne la série
temporelle suivante:

```
2025-09-01T09:00:00
0 10
5 20
10 15
```

Alors on s'attend au comportement suivant:

```
$ bin/tempo interpolate < example/3_10s.ts
2025-09-01T09:00:00 10
2025-09-01T09:00:01 12
2025-09-01T09:00:02 14
2025-09-01T09:00:03 16
2025-09-01T09:00:04 18
2025-09-01T09:00:05 20
2025-09-01T09:00:06 19
2025-09-01T09:00:07 18
2025-09-01T09:00:08 17
2025-09-01T09:00:09 16
2025-09-01T09:00:10 15
```

Autrement dit, on [interpole
linéairement](https://en.wikipedia.org/wiki/Linear_interpolation) les valeurs
entre `09:00:00` et `09:00:05` (en allant de `10` à `20`, par bonds de `2`),
puis ensuite on interpole les valeurs entre `09:00:05` et `09:00:10` (en allant
de `20` à `15`, par bonds de `-1`). Si les bonds ne sont pas entiers, on
effectue d'abord le calcul en valeurs flottantes, puis on tronque ensuite la
partie fractionnaire pour ne conserver que la partie entière de la valeur.

Plus spécifiquement, vous devez minimalement apporter les modifications
suivantes, en plaçant tous vos *commits* sur la branche
`interpolate-subcommand`:

1. Étendre la base de code afin de supporter cette nouvelle sous-commande
2. Mettre à jour les tests fonctionnels (tests Bats) existants s'il y a lieu
3. Ajouter de nouveaux tests fonctionnels reflétant les modifications apportées
4. Mettre à jour la documentation (l'aide `tempo help`, les *docstrings* des
   fonctions impactées, le fichier `README.md`) pour refléter les changements
   apportés
5. Vous assurer de respecter le style de programmation existant
6. Répartir vos modifications le plus possible en différents *commits*
   atomiques, en formatant vos messages selon le style demandé dans le cadre du
   travail pratique 1

## Tâche 4: `interpolate-step-option`

Finalement, la quatrième tâche consiste à étendre la sous-commande
`interpolate` afin de spécifier un pas de temps arbitraire, en supportant
l'option `-s|--step`.

Par exemple, si on reprend l'exemple plus haut:

```
$ bin/tempo interpolate -s 2s < example/3_10s.ts
2025-09-01T09:00:00 10
2025-09-01T09:00:02 14
2025-09-01T09:00:04 18
2025-09-01T09:00:06 19
2025-09-01T09:00:08 17
2025-09-01T09:00:10 15
```

Si l'option `-s|--step` n'apparaît pas, le pas d'interpolation par défaut est
1 seconde (`1s`). Plus généralement, la durée du pas de temps peut être
spécifiée à l'aide du format `N[smh]`, où `N` est un nombre entier strictement
positif, suivi d'un caractère choisi parmi `s` (seconde), `m` (minute) et `h`
(heure). Par exemple:

```sh
# Pour un pas de 5 secondes
$ bin/temp interpolate -s 5s < example/serie.ts
# Pour un pas de 10 minutes
$ bin/temp interpolate -s 10m < example/serie.ts
# Pour un pas de 4 heures
$ bin/temp interpolate -s 4h < example/serie.ts
```

1. Étendre la base de code afin de supporter cette nouvelle option
2. Mettre à jour les tests fonctionnels (tests Bats) existants s'il y a lieu
3. Ajouter de nouveaux tests fonctionnels reflétant les modifications apportées
4. Mettre à jour la documentation (l'aide `tempo help`, les *docstrings* des
   fonctions impactées, le fichier `README.md`) pour refléter les changements
   apportés
5. Vous assurer de respecter le style de programmation existant
6. Répartir vos modifications le plus possible en différents *commits*
   atomiques, en formatant vos messages selon le style demandé dans le cadre du
   travail pratique 1

## Clone et création du dépôt

Comme pour le travail pratique 1, vous devez cloner le dépôt fourni et
l'héberger sur la plateforme [Gitlab du
département](https://gitlab.info.uqam.ca/). Votre dépôt devra se nommer
**exactement** `inf3135-253-tp2` et l'URL devra être **exactement**
`https://gitlab.info.uqam.ca/<utilisateur>/inf3135-253-tp2`, où `<utilisateur>`
doit être remplacé par votre nom d'utilisateur. Il devra être **privé** et
accessible seulement par vous et par les utilisateurs `blondin_al` et
`guite-vinet.julien`.

## Git

Il est important de continuer à utiliser correctement Git pour suivre
l'évolution de votre projet. N'oubliez pas de bien configurer correctement
votre fichier `.gitconfig` qui permet de vous identifier comme auteur de
*commits*, en y indiquant vos **véritables** prénom, nom et courriel. Comme
pour le travail pratique 1, les messages de *commit* doivent suivre [la
convention de Chris Beams](https://chris.beams.io/posts/git-commit/), adaptée
au français:

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

**Attention!**. Une différence notable entre le premier travail pratique et
celui-ci est que vous ne devez pas modifier la branche `master`. Plutôt, vous
devez remettre quatre branches, sans les fusionner sur la branche `master`.

Finalement, afin de bien décomposer le travail en branches, vous devez
respecter les contraintes suivantes:

* Les trois premières branches (`validate-subcommands`, `dynamic-timeseries`,
  et `interpolate-subcommand`) doivent toutes démarrer de la branche `master` du dépôt de référence et être complètement indépendantes l'une de l'autre
* La branche `interpolate-step-option` doit démarrer depuis le dernier *commit*
  de la branche `interpolate-subcommand`, puisqu'elle dépend des modifications apportées sur cette dernière

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
| Tâche 1             |     /10 |
| Tâche 2             |     /20 |
| Tâche 3             |     /30 |
| Tâche 4             |     /20 |
| Utilisation de Git  |     /20 |
| Total               |    /100 |

Plus précisément, les éléments suivants seront pris en compte:

- **Tâche 1 (10 points)**: L'application compile sans erreur et sans
  avertissement. La mémoire est gérée correctement. Elle vérifie si la commande
  fournie a des arguments supplémentaires. En cas d'erreur, elle affiche un
  message d'erreur sur la sortie d'erreur et retourne le code approprié. Des
  tests fonctionnels ont été ajoutés pour refléter les modifications. Les
  *docstrings* du code source ont été mises à jour ou ajoutées s'il y a lieu.
  Le style de programmation existant est respecté.

- **Tâche 2 (20 points)**: L'application compile sans erreur et sans
  avertissement. La mémoire est gérée correctement. Elle gère les séries
  temporelles de façon plus flexible, à l'aide de l'allocation dynamique. Les
  modifications n'introduisent pas de régression. En cas de problème de
  mémoire, un message d'erreur est affiché sur la sortie d'erreur et
  l'application retourne le code approprié. Les *docstrings* du code source ont
  été mises à jour ou ajoutées aux nouvelles fonctions, s'il y a lieu. Le style
  de programmation existant est respecté.

- **Tâche 3 (30 points)**: L'application compile sans erreur et sans
  avertissement. La mémoire est gérée correctement. Elle introduit la nouvelle
  sous-commande `interpolate`. Elle rapporte correctement les valeurs
  interpolées. Les tests fonctionnels existants ont été mis à jour pour
  refléter les modifications, s'il y a lieu. Des nouveaux tests ont été ajoutés
  pour refléter l'ajout de la nouvelle sous-commande. Les *docstrings* du code
  source ont été mises à jour ou ajoutées aux nouvelles fonctions. Le fichier
  `README.md` et l'aide (`kover help`) ont été mis à jour. Le style de
  programmation existant est respecté.

- **Tâche 4 (20 points)**: L'application compile sans erreur et sans
  avertissement. La mémoire est gérée correctement. Elle introduit l'option
  `-s|--step` à la sous-commande `interpolate`. Elle gère correctement des
  durées en secondes, en minutes et en heures. Elle gère les cas de mauvaise
  utilisation de l'option. Les tests fonctionnels existants ont été mis à jour
  pour refléter les modifications. Des nouveaux tests ont été ajoutés pour
  refléter l'ajout de la nouvelle option. Les *docstrings* du code source ont
  été mises à jour ou ajoutées aux nouvelles fonctions. Le fichier `README.md`
  et l'aide (`kover help`) ont été mis à jour. Le style de programmation
  existant est respecté.

- **Utilisation de Git (20 points)**: Les modifications sont réparties en
  *commits* atomiques. Le fichier `.gitignore` est complet. Les messages de
  *commit* sont significatifs, uniformes et suivent la convention suggérée. Les
  *commits* sont répartis sur des branches nommées respectivement
  `validate-subcommands`, `dynamic-timeseries`, `interpolate-subcommand` et
  `interpolate-step-option`. Chaque *commit* est pertinent pour la branche sur
  laquelle il se trouve. Les branches des 3 premières tâches sont
  indépendantes, alors que la branche de la 4e tâche apparaît à la suite de la
  3e tâche. Noter que si vous n'avez pas beaucoup avancé dans certaines des
  tâches demandées, vous ne pourrez pas obtenir beaucoup de points sur ce
  critère.

## Pénalités

Si votre programme ne compile pas pour une tâche donnée, une pénalité pouvant
aller jusqu'à **100%** pourra être appliquée pour la tâche en question.

**Note**: vous n'avez pas besoin d'insérer votre code permanent dans le fichier
`README.md`: nous avons établi l'association entre votre nom d'utilisateur et
votre code permanent dans le premier travail pratique.

En outre, si vous ne respectez pas les critères suivants, une pénalité pouvant
aller jusqu'à **50%** pourra être appliquée:

- Votre dépôt doit se nommer **exactement** `inf3135-253-tp2`
- L'URL de votre dépôt doit être **exactement**
  `https://gitlab.info.uqam.ca/<utilisateur>/inf3135-253-tp2` où
  `<utilisateur>` doit être remplacé par votre identifiant
- Les utilisateurs `blondin_al` et `guite-vinet.julien` doivent avoir accès
  à votre projet en mode *Maintainer*.
- Votre dépôt doit être un *fork* du [gabarit
  fourni](https://gitlab.info.uqam.ca/inf31351/20253/inf3135-253-tp2).
- Votre dépôt doit être **privé**.

## Remise

La remise se fait automatiquement en ajoutant les utilisateurs `blondin_al` et
`guite-vinet.julien` en mode *Maintainer*, vous n'avez rien de plus à faire.
À moins d'indication contraire de votre part, ce sera le dernier (*commit*)
disponible sur chacune de vos branches qui sera considéré pour la correction.
