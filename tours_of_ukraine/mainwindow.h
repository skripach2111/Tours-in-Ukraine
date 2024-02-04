#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "databasemodule.h"

#include "clientsfiltermodel.h"
#include "touristsitesfiltermodel.h"
#include "toursfiltermodel.h"
#include "vauchersfiltermodel.h"

#include "addeditclientsdialog.h"
#include "addedittouristsitesdialog.h"
#include "addedittoursdialog.h"
#include "addeditvauchersdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    DatabaseModule dbm;

    ClientsFilterModel* clientsFilterModel;
    TouristSitesFilterModel* touristSitesFilterModel;
    ToursFilterModel* toursFilterModel;
    VauchersFilterModel* vauchersFilterModel;

    enum PAGES {
        HELLO = 0,
        CLIENTS,
        TOURIST_SITES,
        TOURS,
        VAUCHERS,
        STATISTIC
    };



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_clients_triggered();

    void on_action_tours_objects_triggered();

    void on_action_tours_triggered();

    void on_action_vauchers_triggered();

    void on_pushButton_clients_search_clicked();

    void on_pushButton_tourist_sites_search_clicked();

    void on_pushButton_tours_search_clicked();

    void on_pushButton_vauchers_search_clicked();

    void on_pushButton_vauchers_add_clicked();

    void on_pushButton_vauchers_edit_clicked();

    void on_pushButton_vauchers_delete_clicked();

    void on_pushButton_clients_add_clicked();

    void on_pushButton_clients_edit_clicked();

    void on_pushButton_clients_delete_clicked();

    void on_pushButton_tourist_sites_add_clicked();

    void on_pushButton_tourist_sites_edit_clicked();

    void on_pushButton_tourist_sites_delete_clicked();

    void on_pushButton_tours_add_clicked();

    void on_pushButton_tours_edit_clicked();

    void on_pushButton_tours_delete_clicked();

    void on_action_stats_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
