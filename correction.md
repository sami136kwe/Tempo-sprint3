# Évaluation du travail pratique 3

## Identification

* Nom d’utilisation : geagea.sami
* Prénom et nom: Sami Geagea
* Code Permanent: GEAS72090507

## Total: 90/100

## Vérification de base:

* L'URL du dépôt est correcte? oui
* Le dépôt est privé? oui
* Le dépôt est un *fork*? oui
* Le rôle accordé est *Maintainer*? oui
* Le projet compile sans erreur et sans avertissement avec la commande `make
  build`? oui


## Tâche 1: 20/20

* L'application compile sans erreur et sans avertissement? oui
* La mémoire est gérée correctement? oui
* Un fichier `test_timeseries.c` a été ajouté? oui
* Il contient des test unitaires Libtap couvrant les différentes fonctions
  publiques fournies par le module `timeseries`? oui
* Si des bogues ont été découverts, ils ont été corrigés? oui
* Les *docstrings* du code source ont été mises à jour ou ajoutées s'il
  y a lieu? oui
* Les fichiers Makefiles ont été mis à jour correctement? oui
* Le style de programmation existant est respecté? oui
* Les modifications n'introduisent pas de régression? oui

## Tâche 2: 25/25

* L'application compile sans erreur et sans avertissement? oui
* La mémoire est gérée correctement? oui
* L'option `-J|--from-json` a été introduite pour chacune des sous-commandes?
  oui
* Des test fonctionnels ont été ajoutés afin d'illustrer la nouvelle option?
  oui
* Les *docstrings* du code source ont été mises à jour ou ajoutées aux
  nouvelles fonctions, s'il y a lieu? oui
* Le message d'aide du programme a été mis à jour correctement? oui
* Les fichiers Makefiles, ainsi que le fichier README, ont également été mis
  à jour? oui
* La bibliothèque Jansson a été intégrée correctement dans la base de code,
  dans les fichiers Makefiles, ainsi que dans le fichier `.gitlab-ci.yml`? oui
* Le style de programmation existant est respecté? oui
* Les modifications n'introduisent pas de régression? oui

## Tâche 3: 33/35

* L'application compile sans erreur et sans avertissement? oui
* La mémoire est gérée correctement? oui
* La sous-commande `gnuplot` a été introduite correctement et supporte l'option
  `-J|--from-json`? oui
* Des nouveaux tests ont été ajoutés pour refléter l'ajout de la nouvelle
  sous-commande et de son option? oui
* Les *docstrings* du code source ont été mises à jour ou ajoutées aux
  nouvelles fonctions? oui
* Le fichier `README.md`, l'aide (`tempo help`), les Makefiles et le fichier
  `.gitlab-ci.yml` ont été mis à jour? oui
* Le style de programmation existant est respecté? oui
* Les modifications n'introduisent pas de régression? non
* Une requête d'intégration avec titre pertinent, description détaillée et
  exemple, a été rédigée? oui
* Nombre de tests échouant sur la branche origin/to-gnuplot? 2, -2 pts

## Utilisation de Git: 12/20

* Les modifications sont réparties en *commits* atomiques? oui
* Le fichier `.gitignore` est complet? oui
* Les messages de *commit* sont significatifs, uniformes et suivent la
  convention suggérée? oui
* Les *commits* sont répartis sur des branches nommées respectivement
  `test-timeseries-libtap`, `from-json` et `to-gnuplot`? oui
* Chaque *commit* est pertinent pour la branche sur laquelle il se trouve? oui
* Les branches des 2 premières tâches sont indépendantes, alors que la branche
  de la 3e tâche apparaît à la suite de la 2e tâche? oui
* Nombre de messages qui ne commencent pas par une majuscule? 24. -2pts
* Nombre de messages qui ne commencent pas par un verbe à l'indicatif? >5. -2pts
* Nombre de messages qui dépassent 50 caractères? 10. -1pt
* Nombre de messaggs qui contiennent le caractère `:`. 1. -1pt
* On retrouve des messages de commit qui ne sont pas rédigés en français. -2pts


## Annexe test-timeseries-libtap: Rapport de tests Bats

* Nombre de tests réussis pour la branche `test-timeseries-libtap`? 72
```
1..72
ok 1 tempo describe with empty timeseries displays the domain
ok 2 tempo describe with timeseries of size 1 displays the domain
ok 3 tempo describe with chronological timeseries of size 2 displays the domain
ok 4 tempo describe with antichronological timeseries of size 2 displays the domain
ok 5 tempo describe with timeseries with repeated offsets displays the domain
ok 6 tempo describe with empty timeseries displays the codomain
ok 7 tempo describe with timeseries of size 1 displays the codomain
ok 8 tempo describe with chronological timeseries of size 2 displays the codomain
ok 9 tempo describe with antichronological timeseries of size 2 displays the codomain
ok 10 tempo describe with timeseries with repeated offsets displays the codomain
ok 11 tempo describe with empty timeseries displays the size
ok 12 tempo describe with timeseries of size 1 displays the size
ok 13 tempo describe with chronological timeseries of size 2 displays the size
ok 14 tempo describe with antichronological timeseries of size 2 displays the size
ok 15 tempo describe with timeseries with repeated offsets displays the size
ok 16 tempo describe with empty timeseries displays the duration
ok 17 tempo describe with timeseries of size 1 displays the duration
ok 18 tempo describe with chronological timeseries of size 2 displays the duration
ok 19 tempo describe with antichronological timeseries of size 2 displays the duration
ok 20 tempo describe with timeseries with repeated offsets displays the duration
ok 21 tempo describe with empty timeseries displays the amplitude
ok 22 tempo describe with timeseries of size 1 displays the amplitude
ok 23 tempo describe with chronological timeseries of size 2 displays the amplitude
ok 24 tempo describe with antichronological timeseries of size 2 displays the amplitude
ok 25 tempo describe with timeseries with repeated offsets displays the amplitude
ok 26 tempo show with datetime missing the character T returns an error
ok 27 tempo show with wrong character in date returns an error
ok 28 tempo show with wrong character in time returns an error
ok 29 tempo show with wrong day returns an error
ok 30 tempo show with another wrong day returns an error
ok 31 tempo show with wrong month returns an error
ok 32 tempo show with wrong year returns an error
ok 33 tempo show with empty observation line returns an error
ok 34 tempo show with wrong character in observation returns an error
ok 35 tempo show with wrong offset format returns an error
ok 36 tempo show with wrong value format returns an error
ok 37 tempo show with negative offset format returns an error
ok 38 tempo interpolate with empty timeseries works
ok 39 tempo interpolate with timeseries of size 1 works
ok 40 tempo interpolate with timeseries of size 2 works
ok 41 tempo interpolate with timeseries of size 3 works
ok 42 tempo interpolate -s 4s with timeseries of size 3 works
ok 43 tempo interpolate -s 30m with timeseries of size 2 works
ok 44 tempo interpolate -s 4h with timeseries 24h.ts works
ok 45 tempo describe runs without memory problem
ok 46 tempo help runs without memory problem
ok 47 tempo show runs without memory problem on empty timeseries
ok 48 tempo show runs without memory problem on nonempty timeseries
ok 49 tempo show with empty timeseries works
ok 50 tempo show with timeseries of size 1 works
ok 51 tempo show with timeseries of size 1 with spaces works
ok 52 tempo show with chronological timeseries of size 2 works
ok 53 tempo show with antichronological timeseries of size 2 works
ok 54 tempo show with timeseries with repeated offsets works
ok 55 tempo show with timeseries with multiple repeated offsets works
ok 56 tempo show with year-changing timeseries works
ok 57 tempo show with month-changing timeseries works
ok 58 tempo show with day-changing timeseries works
ok 59 tempo show with hour-changing timeseries works
ok 60 tempo show with minute-changing timeseries works
ok 61 tempo help shows the help
ok 62 tempo help describes a valid timeseries text stream
ok 63 tempo help mentions the describe subcommand
ok 64 tempo help mentions the help subcommand
ok 65 tempo help mentions the interpolate subcommand
ok 66 tempo help mentions the -s|--step option
ok 67 tempo help mentions the show subcommand
ok 68 tempo without subcommand returns an error
ok 69 tempo with unrecognized subcommand returns an error
ok 70 tempo describe with additional arguments returns an error
ok 71 tempo help with additional arguments returns an error
ok 72 tempo show with additional arguments returns an error
```

## Annexe from-json: Rapport de tests Bats

* Nombre de tests réussis pour la branche `from-json`? 83
```
1..83
ok 1 tempo describe with empty timeseries displays the domain
ok 2 tempo describe with timeseries of size 1 displays the domain
ok 3 tempo describe with chronological timeseries of size 2 displays the domain
ok 4 tempo describe with antichronological timeseries of size 2 displays the domain
ok 5 tempo describe with timeseries with repeated offsets displays the domain
ok 6 tempo describe with empty timeseries displays the codomain
ok 7 tempo describe with timeseries of size 1 displays the codomain
ok 8 tempo describe with chronological timeseries of size 2 displays the codomain
ok 9 tempo describe with antichronological timeseries of size 2 displays the codomain
ok 10 tempo describe with timeseries with repeated offsets displays the codomain
ok 11 tempo describe with empty timeseries displays the size
ok 12 tempo describe with timeseries of size 1 displays the size
ok 13 tempo describe with chronological timeseries of size 2 displays the size
ok 14 tempo describe with antichronological timeseries of size 2 displays the size
ok 15 tempo describe with timeseries with repeated offsets displays the size
ok 16 tempo describe with empty timeseries displays the duration
ok 17 tempo describe with timeseries of size 1 displays the duration
ok 18 tempo describe with chronological timeseries of size 2 displays the duration
ok 19 tempo describe with antichronological timeseries of size 2 displays the duration
ok 20 tempo describe with timeseries with repeated offsets displays the duration
ok 21 tempo describe with empty timeseries displays the amplitude
ok 22 tempo describe with timeseries of size 1 displays the amplitude
ok 23 tempo describe with chronological timeseries of size 2 displays the amplitude
ok 24 tempo describe with antichronological timeseries of size 2 displays the amplitude
ok 25 tempo describe with timeseries with repeated offsets displays the amplitude
ok 26 tempo show with datetime missing the character T returns an error
ok 27 tempo show with wrong character in date returns an error
ok 28 tempo show with wrong character in time returns an error
ok 29 tempo show with wrong day returns an error
ok 30 tempo show with another wrong day returns an error
ok 31 tempo show with wrong month returns an error
ok 32 tempo show with wrong year returns an error
ok 33 tempo show with empty observation line returns an error
ok 34 tempo show with wrong character in observation returns an error
ok 35 tempo show with wrong offset format returns an error
ok 36 tempo show with wrong value format returns an error
ok 37 tempo show with negative offset format returns an error
ok 38 tempo interpolate with empty timeseries works
ok 39 tempo interpolate with timeseries of size 1 works
ok 40 tempo interpolate with timeseries of size 2 works
ok 41 tempo interpolate with timeseries of size 3 works
ok 42 tempo interpolate -s 4s with timeseries of size 3 works
ok 43 tempo interpolate -s 30m with timeseries of size 2 works
ok 44 tempo interpolate -s 4h with timeseries 24h.ts works
ok 45 tempo describe -J with basic timeseries works
ok 46 tempo describe --from-json handles repeated offsets
ok 47 tempo show -J with basic timeseries works
ok 48 tempo show --from-json handles antichronological order
ok 49 tempo interpolate -J with basic timeseries works
ok 50 tempo interpolate --from-json with step options works
ok 51 tempo -J validates JSON structure
ok 52 tempo -J validates observation structure
ok 53 tempo -J validates CLI arguments
ok 54 tempo help documents JSON options
ok 55 tempo -J handles special values
ok 56 tempo describe runs without memory problem
ok 57 tempo help runs without memory problem
ok 58 tempo show runs without memory problem on empty timeseries
ok 59 tempo show runs without memory problem on nonempty timeseries
ok 60 tempo show with empty timeseries works
ok 61 tempo show with timeseries of size 1 works
ok 62 tempo show with timeseries of size 1 with spaces works
ok 63 tempo show with chronological timeseries of size 2 works
ok 64 tempo show with antichronological timeseries of size 2 works
ok 65 tempo show with timeseries with repeated offsets works
ok 66 tempo show with timeseries with multiple repeated offsets works
ok 67 tempo show with year-changing timeseries works
ok 68 tempo show with month-changing timeseries works
ok 69 tempo show with day-changing timeseries works
ok 70 tempo show with hour-changing timeseries works
ok 71 tempo show with minute-changing timeseries works
ok 72 tempo help shows the help
ok 73 tempo help describes a valid timeseries text stream
ok 74 tempo help mentions the describe subcommand
ok 75 tempo help mentions the help subcommand
ok 76 tempo help mentions the interpolate subcommand
ok 77 tempo help mentions the -s|--step option
ok 78 tempo help mentions the show subcommand
ok 79 tempo without subcommand returns an error
ok 80 tempo with unrecognized subcommand returns an error
ok 81 tempo describe with additional arguments returns an error
ok 82 tempo help with additional arguments returns an error
ok 83 tempo show with additional arguments returns an error
```

## Annexe to-gnuplot: Rapport de tests Bats

* Nombre de tests réussis pour la branche `to-gnuplot`? 87
```
1..89
ok 1 tempo describe with empty timeseries displays the domain
ok 2 tempo describe with timeseries of size 1 displays the domain
ok 3 tempo describe with chronological timeseries of size 2 displays the domain
ok 4 tempo describe with antichronological timeseries of size 2 displays the domain
ok 5 tempo describe with timeseries with repeated offsets displays the domain
ok 6 tempo describe with empty timeseries displays the codomain
ok 7 tempo describe with timeseries of size 1 displays the codomain
ok 8 tempo describe with chronological timeseries of size 2 displays the codomain
ok 9 tempo describe with antichronological timeseries of size 2 displays the codomain
ok 10 tempo describe with timeseries with repeated offsets displays the codomain
ok 11 tempo describe with empty timeseries displays the size
ok 12 tempo describe with timeseries of size 1 displays the size
ok 13 tempo describe with chronological timeseries of size 2 displays the size
ok 14 tempo describe with antichronological timeseries of size 2 displays the size
ok 15 tempo describe with timeseries with repeated offsets displays the size
ok 16 tempo describe with empty timeseries displays the duration
ok 17 tempo describe with timeseries of size 1 displays the duration
ok 18 tempo describe with chronological timeseries of size 2 displays the duration
ok 19 tempo describe with antichronological timeseries of size 2 displays the duration
ok 20 tempo describe with timeseries with repeated offsets displays the duration
ok 21 tempo describe with empty timeseries displays the amplitude
ok 22 tempo describe with timeseries of size 1 displays the amplitude
ok 23 tempo describe with chronological timeseries of size 2 displays the amplitude
ok 24 tempo describe with antichronological timeseries of size 2 displays the amplitude
ok 25 tempo describe with timeseries with repeated offsets displays the amplitude
not ok 26 tempo gnuplot with timeseries generates valid script
# (from function `assert_line' in file bats/bats-assert/src/assert_line.bash, line 232,
#  in test file bats/test_gnuplot.bats, line 18)
#   `assert_line --partial "plot '-'"' failed
#
# -- no output line contains substring --
# substring : plot '-'
# output (16 lines):
#   #!/usr/bin/gnuplot
#   # Data
#   $data << EOD
#   2025-09-01T09:00:00 10
#   2025-09-01T10:00:00 20
#   EOD
#   # General settings
#   set terminal png
#   set xlabel "Time" offset 0,-1
#   set xdata time
#   set timefmt "%Y-%m-%dT%H:%M:%S"
#   set ylabel "Value"
#   set nokey
#   set output "timeseries.png"
#   # Plot times
#   plot $data using 1:2 ps 2.0 pt 7 lc "orange" title "0.9" with points
# --
#
ok 27 tempo gnuplot script executes without error
not ok 28 tempo gnuplot -J with timeseries generates valid script
# (from function `assert_line' in file bats/bats-assert/src/assert_line.bash, line 232,
#  in test file bats/test_gnuplot.bats, line 35)
#   `assert_line --partial "plot '-'"' failed
#
# -- no output line contains substring --
# substring : plot '-'
# output (16 lines):
#   #!/usr/bin/gnuplot
#   # Data
#   $data << EOD
#   2025-09-01T09:00:00 10
#   2025-09-01T10:00:00 20
#   EOD
#   # General settings
#   set terminal png
#   set xlabel "Time" offset 0,-1
#   set xdata time
#   set timefmt "%Y-%m-%dT%H:%M:%S"
#   set ylabel "Value"
#   set nokey
#   set output "timeseries.png"
#   # Plot times
#   plot $data using 1:2 ps 2.0 pt 7 lc "orange" title "0.9" with points
# --
#
ok 29 tempo gnuplot --from-json script executes without error
ok 30 tempo gnuplot rejects too many arguments
ok 31 tempo gnuplot -J rejects too many arguments
ok 32 tempo show with datetime missing the character T returns an error
ok 33 tempo show with wrong character in date returns an error
ok 34 tempo show with wrong character in time returns an error
ok 35 tempo show with wrong day returns an error
ok 36 tempo show with another wrong day returns an error
ok 37 tempo show with wrong month returns an error
ok 38 tempo show with wrong year returns an error
ok 39 tempo show with empty observation line returns an error
ok 40 tempo show with wrong character in observation returns an error
ok 41 tempo show with wrong offset format returns an error
ok 42 tempo show with wrong value format returns an error
ok 43 tempo show with negative offset format returns an error
ok 44 tempo interpolate with empty timeseries works
ok 45 tempo interpolate with timeseries of size 1 works
ok 46 tempo interpolate with timeseries of size 2 works
ok 47 tempo interpolate with timeseries of size 3 works
ok 48 tempo interpolate -s 4s with timeseries of size 3 works
ok 49 tempo interpolate -s 30m with timeseries of size 2 works
ok 50 tempo interpolate -s 4h with timeseries 24h.ts works
ok 51 tempo describe -J with basic timeseries works
ok 52 tempo describe --from-json handles repeated offsets
ok 53 tempo show -J with basic timeseries works
ok 54 tempo show --from-json handles antichronological order
ok 55 tempo interpolate -J with basic timeseries works
ok 56 tempo interpolate --from-json with step options works
ok 57 tempo -J validates JSON structure
ok 58 tempo -J validates observation structure
ok 59 tempo -J validates CLI arguments
ok 60 tempo help documents JSON options
ok 61 tempo -J handles special values
ok 62 tempo describe runs without memory problem
ok 63 tempo help runs without memory problem
ok 64 tempo show runs without memory problem on empty timeseries
ok 65 tempo show runs without memory problem on nonempty timeseries
ok 66 tempo show with empty timeseries works
ok 67 tempo show with timeseries of size 1 works
ok 68 tempo show with timeseries of size 1 with spaces works
ok 69 tempo show with chronological timeseries of size 2 works
ok 70 tempo show with antichronological timeseries of size 2 works
ok 71 tempo show with timeseries with repeated offsets works
ok 72 tempo show with timeseries with multiple repeated offsets works
ok 73 tempo show with year-changing timeseries works
ok 74 tempo show with month-changing timeseries works
ok 75 tempo show with day-changing timeseries works
ok 76 tempo show with hour-changing timeseries works
ok 77 tempo show with minute-changing timeseries works
ok 78 tempo help shows the help
ok 79 tempo help describes a valid timeseries text stream
ok 80 tempo help mentions the describe subcommand
ok 81 tempo help mentions the help subcommand
ok 82 tempo help mentions the interpolate subcommand
ok 83 tempo help mentions the -s|--step option
ok 84 tempo help mentions the show subcommand
ok 85 tempo without subcommand returns an error
ok 86 tempo with unrecognized subcommand returns an error
ok 87 tempo describe with additional arguments returns an error
ok 88 tempo help with additional arguments returns an error
ok 89 tempo show with additional arguments returns an error
```
