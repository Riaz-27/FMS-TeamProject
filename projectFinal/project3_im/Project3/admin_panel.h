#ifndef ADMIN_PANEL_H
#define ADMIN_PANEL_H

#include <QWidget>
#include <QStateMachine>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QEventTransition>
#include <QtSql>
#include <QPushButton>
#include <QMessageBox>
#include "db.h"
#include "qcustomplot.h"
#include <QScrollBar>

namespace Ui {
class Admin_Panel;
}

class Admin_Panel : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_Panel(QWidget *parent = nullptr);
    ~Admin_Panel();

private slots:
    void on_pushButton_logout_clicked();

    void on_pushButton_mdash_clicked();

    void on_pushButton_sett_clicked();

    void on_pushButton_manage_clicked();

    void on_pushButton_menu_clicked();

    void on_pushButton_cpic_clicked();

    void on_pushButton_update_clicked();

    void on_scroll_button_name_clicked();

    void on_pushButton_abort_clicked();

    void on_pushButton_assign_clicked();

    void on_tableView_3_clicked(const QModelIndex index);

    void makePlot(int, int, int, int,int, int, int); // for custom plot

    void on_pushButton_message_clicked();

    void on_lineEdit_massage_textChanged(const QString &arg1);

    void keyPressEvent(QKeyEvent *event);

    void on_lineEdit_search_textChanged(const QString &arg1);

    void load_tableview_data(QTableView *table, QString qry);

    void on_pushButton_manage_user_clicked();

    void on_lineEdit_search_2_textChanged(const QString &arg1);

    void on_tableView_user_clicked(const QModelIndex &index);

    void on_pushButton_back_clicked();

    void on_pushButton_user_remove_clicked();

    void on_pushButton_send_message_clicked();

    void showProfileFromMessage_clicked();

    void  moveScrollBarToBottom(int , int);

    void on_pushButton_hide_clicked();

    void on_lineEdit_day_editingFinished();

private:
    Ui::Admin_Panel *ui;

    QString pic_path;
    QString user_username;
    QString status;

    QPropertyAnimation *anim;
    QPropertyAnimation *assign_anim;
    QPropertyAnimation *menu_anim;
    QPropertyAnimation *user_anim;
    db mydb;

    bool menu_clicked;

    //variables for accessing datas
    int admin, dep, free, des, running_project, complete_project, pending_project;
    // for scroll area
    int trackforScroll = 0;
    QPushButton *pushButton;
    QString getDataFromScrollArea, scrollAreaToAssign, getDurationData;
    QSqlQueryModel *modelForAssign;
    QSqlQuery *qryForAssign;
    int indexForAssign;

    QFormLayout *lay = new QFormLayout(this);

    //for message box loading
    bool loaded;
    // for profile loading in message box
    bool animationDoneForProfileView;
    int x, y;
    // for scroll area attached in the bottom
    QScrollBar *scrollbar;
    int RowCount = 0; // for settele the last element of the message box
};


#endif // ADMIN_PANEL_H
