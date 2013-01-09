#include "dbmanager.hpp"

dbManager::dbManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("PotatoBase");

    query = QSqlQuery(db);

    if(!db.open())
    {
        std::cerr << "Unable to create/open database" << std::endl;
        exit(1);
    }

    //Création des tables si nécessaire
    query = db.exec("CREATE TABLE IF NOT EXISTS song(file string primary key, artist string, title string, album string, genre string, nb_played integer, rating interger)");
    query = db.exec("CREATE TABLE IF NOT EXISTS playlist(name string primary key)");
    query = db.exec("CREATE TABLE IF NOT EXISTS asso_song_pl(pl string, sg string)");
    query = db.exec("CREATE TABLE IF NOT EXISTS srcDir(dir string)");
}

dbManager* dbManager::getInstance(){
    if (singleton == NULL)
         {
           std::cout << "creating singleton." << std::endl;
           singleton =  new dbManager;
         }
       else
         {
           std::cout << "singleton already created!" << std::endl;
         }

       return singleton;
}

QStringList dbManager::addSong(QString file)
{
    QStringList song;
    QString artiste,titre,album,genre;
    query.prepare("SELECT artist,title,album,genre,nb_played,rating FROM song WHERE file=?");
        query.bindValue(1,file);
    query.exec();
    if(!query.isValid())
    {
        TagLib::FileRef f(file.toStdString().c_str());
        TagLib::Tag *tag = f.tag();

        artiste = QString::fromUtf8(tag->artist().toCString(true));
        titre = QString::fromUtf8(tag->title().toCString(true));
        album = QString::fromUtf8(tag->album().toCString(true));
        genre = QString::fromUtf8(tag->genre().toCString(true));

        query.prepare("INSERT INTO song (file, artist, title, album, genre, nb_played, rating) VALUES (?, ?, ?, ?, ?, 0, 0)");
             query.bindValue("1", file);
             query.bindValue("2", artiste);
             query.bindValue("3", titre);
             query.bindValue("4", album);
             query.bindValue("5", genre);
        query.exec();

    }
    else
    {
        query.first();
        artiste = query.value(0).toString();
        titre = query.value(1).toString();
        album = query.value(2).toString();
    }


    song << titre << album << artiste;
    return song;
}


QStringList dbManager::getSrcDirs()
{
    QStringList sources;
    query.prepare("SELECT dir FROM srcDir");
    query.exec();
    while (query.next()) {
        sources << query.value(0).toString();
    }
    return sources;
}

void dbManager::addSrc(QString dir)
{
    query.prepare("INSERT INTO srcDir (dir) VALUES (?)");
         query.bindValue("1", dir);
    query.exec();
}

void dbManager::delSrc(QString dir)
{
    query.prepare("DELETE FROM srcDir WHERE dir=?");
         query.bindValue("1", dir);
    query.exec();
}


QList<QStringList> *dbManager::getBiblio()
{
    QString artiste;
    QString album;
    QString titre;
    QList<QStringList> *songs = new QList<QStringList>;
    query.prepare("SELECT artist,title,album FROM song");
    query.exec();
    while(query.next())
    {
        if((titre = query.value(1).toString()) == "")
            titre = tr("Inconnu");
        if((artiste = query.value(0).toString()) == "")
            artiste = tr("Inconnu");
        if((album = query.value(2).toString()) == "")
            album = tr("Inconnu");
        QStringList song(titre);
        song << artiste << album;
        songs->append(song);
    }

    return songs;
}

QStringList *dbManager::getPlaylists()
{
    QStringList *pl = new QStringList;
    query.prepare("SELECT name FROM playlist");
    query.exec();
    while(query.next())
    {
        pl->append(query.value(0).toString());
    }

    return pl;
}

QStringList *dbManager::getAssos(QString pl)
{
    QStringList *songs = new QStringList;
    query.prepare("SELECT sg FROM asso_song_pl WHERE pl=?");
        query.bindValue("1",pl);
    query.exec();
    while(query.next())
    {
        songs->append(query.value(0).toString());
    }
    return songs;
}

QStringList dbManager::getSong(QString path)
{
    query.prepare("SELECT title,artist,album,genre,nb_played,rating,file FROM song WHERE file=?");
        query.bindValue("1",path);
    query.exec();
    query.first();

    return QStringList(query.value(0).toString()) << query.value(1).toString() << query.value(2).toString() << query.value(3).toString() << query.value(4).toString() << query.value(5).toString() << query.value(6).toString();
}

QStringList dbManager::getSong(QString titre,QString album,QString artist)
{
    query.prepare("SELECT title,artist,album,genre,nb_played,rating,file FROM song WHERE title=? AND album=? AND artist=?");
        query.bindValue("1",titre);
        query.bindValue("2",album);
        query.bindValue("3",artist);
    query.exec();
    query.first();

    return QStringList(query.value(0).toString()) << query.value(1).toString() << query.value(2).toString() << query.value(3).toString() << query.value(4).toString() << query.value(5).toString() << query.value(6).toString();
}

void dbManager::addSgToPl(QString sg, QString pl)
{
    query.prepare("INSERT INTO asso_song_pl (sg,pl) VALUES (?,?)");
         query.bindValue("1", sg);
         query.bindValue("2", pl);
    query.exec();
}

bool dbManager::addPl(QString nom)
{
    query.prepare("SELECT *FROM playlist WHERE name=?");
        query.bindValue("1", nom);
    query.exec();
    if(query.first())
        return false;

    query.prepare("INSERT INTO playlist (name) VALUES (?)");
         query.bindValue("1", nom);
    query.exec();
    return true;
}

int dbManager::incrNb_played(QString file, int oldNumber)
{
    ++oldNumber;

    query.prepare("UPDATE song SET nb_played=? WHERE file=?");
        query.bindValue("1", oldNumber);
        query.bindValue("2",file);
    query.exec();
    return oldNumber;
}

void dbManager::delPl(QString name)
{
    query.prepare("DELETE FROM playlist WHERE name=?");
         query.bindValue("1", name);
    query.exec();
    query.prepare("DELETE FROM asso_song_pl WHERE pl=?");
         query.bindValue("1", name);
    query.exec();
}

QString dbManager::getTitleFromPath(QString path)
{
    query.prepare("SELECT title FROM song WHERE file=?");
        query.bindValue("1",path);
    query.exec();
    query.first();

    return query.value(0).toString();
}

dbManager *dbManager::singleton = NULL;
