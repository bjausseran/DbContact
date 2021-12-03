# BdD contact

Tests : 
- Les tests fonctionnaient durant une bonne partie du dev, cependant avec l'ajout des threads, ils ont cassés. 
- Pour avoir des tests fonctionnels, regarder a branche Test avant le merge.

DLL :
- Ajouter des données à la base depuis un csv (1M contact < 10s)
- Supprimer en fonction de la valeur d'un champs ( DatabaseManager::deleteData("company, "Facebook") )
- Modifier des donnéesen fonction de la valeur d'un champ ( DatabaseManager::updateData("conpany","Ynov") )
- Exporter les contacts par société ( DatabaseManager::exportCsvByField("company", "Facebook", "C:/CSVs/Facebook.csv");
- Exporter les contacts par catégorie ( DatabaseManager::exportCsvByField("category", "School", "C:/CSVs/School.csv");

GUI :
- Stats ok
- Liste des contacts à gauches + Selection d'un contact à droite
- Recherche par champs et valeur
- Supprimer/Modifier le contact selectionner
- Exporter le tableau en CSV (on peut choisir le chemin, on a préféré à AppData, la bdd est dans AppData)
- La progress bar est seulement active pour l'export de CSV.

### Utilisation

1. Load CSV :     Taper le chemin du CSV ou du dossier contenant des CSVs dans le text input puis cliquer sur Load Csv pour le charger
2. Display db :   Taper sur refresh pour afficher les contacts de la bdd
3. Filter db :    Pour filtrer taper le nom d'un champ puis la valeurs chercher, enfin cliquer sur refresh
4. Show contact : Double cliquer sur une ligne de la table pour l'afficher dans le formulaire de droite
5. Update :       Cliquer sur Update ou Delete pour effectuer l'action sur le contact selectionner
6. Exporter CSV : Taper le nom du chemin complet du fichier CSV créer puis cliquer sur Exporter Csv