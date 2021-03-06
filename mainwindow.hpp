#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <phonon>
#include "dbmanager.hpp"
#include "scandir.hpp"
#include "insertpl.hpp"
#include <iostream>
#include <QtGui>

/** @class Classe de gestion de la MainWindow */

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QWidget *rightDock;
    QWidget *options;
    QWidget *plBlock;
    QWidget *searchBlock;
    QLineEdit *searchField;
    QTreeWidget *biblio;
    QListWidget *srcDirList;
    QTreeWidget *current;
    QTreeWidget *plists;
    QTreeWidget *searchRes;
    QWidget *cover;
    QLabel *coverImg;
    void scanDir(QString);
    void insertSong(QStringList);
    void regenPlaylists();
    QAction *actionPlay;
    QAction *actionStop;
    QAction *actionPrev;
    QAction *actionNext;
    QAction *actionLoop;
    QLabel *timeTotal;
    QLabel *timeCurrent;
    QStatusBar *statusBar;
    QString convertTime(qint64);
    Phonon::MediaObject* media;
    Phonon::AudioOutput* output;
    int loopState;
    QTreeWidgetItem *playing;
    void bold();
    void clear();
    ScanDir* scan;
    InsertPl* insert;
    QMutex * mut;


public:
    MainWindow();
    dbManager *db;

public slots:
    void editTags();
    void showOptions();
    void showBiblio();
    void showPlists();
    void showSearch();
    void addSourceDir();
    void delSourceDir();
    void addPl();
    void delPl();
    void refresh();
    void upTimeTot(qint64);
    void incrTimeCur(qint64);
    void addToCurrent(QTreeWidgetItem*,int);
    void addSearchToCurrent(QTreeWidgetItem*,int);
    void play();
    void stop();
    void prev();
    void next();
    void songEnd();
    void loop();
    void selectedSong(QTreeWidgetItem*,int);
    void contextCurrent(QPoint);
    void contextSearch(QPoint);
    void contextBiblio(QPoint);
    void contextSrc(QPoint);
    void contextPl(QPoint);
    void playSelected();
    void addCurrentToPl();
    void addSearchToPl();
    void addBiblioToPl();
    void insertPl(QString,QTreeWidgetItem *,int,QTreeWidgetItem* wait=NULL);
    void changeDockInfo(QTreeWidgetItem *,int);
    void changeSearch(QString);
    void saveCurrent();
    void loadCurrent();
    void regenBiblio();
    void clearCurrent();
    void clearBiblio();
    void clearRes();
    void currentToPl();
    void changeStatus(QString);
    void scalePixMap(int,int);

signals :
    void changeWikiInfo(QStringList);

};

#endif // MAINWINDOW_HPP
