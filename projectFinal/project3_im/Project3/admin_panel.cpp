#include "admin_panel.h"
#include "ui_admin_panel.h"
#include "customshadoweffect.h"
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QTimer>
#include <QGridLayout>
#include <QtGui>

Admin_Panel::Admin_Panel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_Panel)
{
    ui->setupUi(this);

    ui->label_send_message->setVisible(false);
    ui->frame_7->hide();
    loaded = false; // for message bos loading
    x = 0; y = 0; // for animating the profile in the message box
    scrollbar = ui->scrollArea_message->verticalScrollBar(); // for scrollbar set in the bottom

    //setting line validator
    ui->lineEdit_2->setValidator(new QRegExpValidator(QRegExp("^[a-z0-9A-Z._@$]+"), this));
    ui->lineEdit_day->setValidator(new QRegExpValidator(QRegExp("^[0-9]{1,2}"), this));
    ui->lineEdit_year->setValidator(new QRegExpValidator(QRegExp("^[0-9]{1,4}"), this));

    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->label_send_message->setVisible(false);
    ui->frame_assign_bg->setGeometry(1070,30,20,571);
    ui->frame_menu->setGeometry(QRect(1340,65,0,0));


    ui->label_incorr->setVisible(false);
    ui->label_corr->setVisible(false);
    //seting up data
    QString name, pass, sts, picPath,username,join;
    QSqlQuery *qry;
    QSqlQueryModel *model;
    if(mydb.connOpen()){
        qDebug() << "admin hello open....";
        qry = new QSqlQuery;
        model = new QSqlQueryModel;
        if (qry->exec("select * from Retrive")) {
            qDebug() << "1st qry";
            model->setQuery(*qry);

            name = model->record(0).value(1).toString();
            pass = model->record(0).value(2).toString();
            qDebug() << name << sts;
            mydb.connClose();
            qry = nullptr;
            model = nullptr;
            mydb.connClose();
        }
    }
    if(mydb.connOpen()){
        qDebug() << "admin hello open....";
        qry = new QSqlQuery;
        model = new QSqlQueryModel;
        if (qry->exec("select * from AllMemberDetails where User_id = '"+name+"' and Password = '"+pass+"'")) {
            qDebug() << "2nd qry";
            model->setQuery(*qry);
            name = model->record(0).value(0).toString();
            username = model->record(0).value(1).toString();
            sts = model->record(0).value(4).toString();
            picPath = model->record(0).value(9).toString();
            join = model->record(0).value(12).toString();
            qDebug() << name << sts << picPath;
            ui->label_sname->setText(name);
            ui->label_name->setText(name);
            ui->label_username->setText(username);
            ui->label_stats_2->setText(sts);
            ui->label_join->setText(join);
            QPixmap pix(picPath);
            ui->label_pic->setPixmap(pix.scaled(ui->label_pic->width(), ui->label_pic->height(), Qt::KeepAspectRatio,Qt::SmoothTransformation));
            ui->label_pic_2->setPixmap(pix.scaled(ui->label_pic->width(), ui->label_pic_2->height(), Qt::KeepAspectRatio,Qt::SmoothTransformation));
            ui->label_menu->setPixmap(pix.scaled(ui->label_pic->width(), ui->label_menu->height(), Qt::KeepAspectRatio,Qt::SmoothTransformation));
            mydb.connClose();
        }
    }

    // setting up label data
    QSet<QString> sta, deprt;
    QString setAdTxt, setDepTxt, setDesTxt, setFreeTxt;

    admin = 0;
    free = 0;
    int i = 0;
    qDebug() << "label load";

    if (mydb.connOpen()) {
        qry = new QSqlQuery;
        model = new QSqlQueryModel;
        if (qry->exec("select * from AllMemberDetails")) {
            model->setQuery(*qry);

            qDebug() << model->record(i).value(4).toString();

            while (qry->next()) {
                if (model->record(i).value(4).toString() == "Admin") {
                    admin++;
                }
                else if (model->record(i).value(4).toString() == "Freelancer") {
                    free++;
                }
                sta.insert(model->record(i).value(4).toString());
                deprt.insert(model->record(i).value(13).toString());
                i++;
            }

            mydb.connClose();

            qDebug() << "admin " << admin;
            qDebug() << "freelancer " << free;
            des = sta.size();
            dep = deprt.size();
            setAdTxt = "Total (" + QString::number(admin) + ")";
            setFreeTxt = "Total (" + QString::number(free) + ")";
            setDesTxt =  "Total (" + QString::number(des) + ")";
            setDepTxt = "Total (" + QString::number(dep) + ")";

            ui->label_9->setText(setAdTxt);
            ui->label_11->setText(setFreeTxt);
            ui->label_13->setText(setDesTxt);
            ui->label_17->setText(setDepTxt);
        }
    }

    // setting up scroll area data
        QString str, str2;
        QPushButton *buttonName;
        QLabel * label, *label_details;
        QFormLayout * layout = new QFormLayout(this);

        if (mydb.connOpen()) {
            qDebug() << "open for manage project";


            QSqlQuery *qry = new QSqlQuery;
            QSqlQueryModel *modelNameDepAmount = new QSqlQueryModel;

            if (qry->exec("select Name, Department, Assinged_to, Details from Projects")) {
                modelNameDepAmount->setQuery(*qry);
            }
            pending_project = 0;
            for (int i = 0; i < modelNameDepAmount->rowCount(); i++) {
                if (modelNameDepAmount->record(i).value(2).toString() == "") {
                    pending_project++;
                    buttonName = new QPushButton(modelNameDepAmount->record(i).value(0).toString());
                    buttonName->setFocusPolicy(Qt::NoFocus);
                    str = "Work type : " + (modelNameDepAmount->record(i).value(0).toString()) + ",   Department: " + (modelNameDepAmount->record(i).value(1).toString());
                    str2 = modelNameDepAmount->record(i).value(3).toString();
                    label = new QLabel(str);
                    label_details = new QLabel(str2);

                    label_details->setWordWrap(true);

                    QFrame *frame_line = new QFrame();
                    frame_line->setFrameShape(QFrame::HLine);
                    frame_line->setFrameShadow(QFrame::Sunken);
                    frame_line->setStyleSheet("background:white");

                    buttonName->setObjectName("buttonNameObject");
                    buttonName->setStyleSheet("QPushButton#buttonNameObject {background: transparent; Text-align: left; font-family:""Segoe UI Semibold""; font-size: 15px; color: #6fa000; border: none;}QPushButton#buttonNameObject:hover {border:none; font-size: bold; color: #2f8700;}");
                    label->setStyleSheet("background: transparent; Text-align: left; font-family: century gothic; font-size: 15px; color: black; border: none;");
                    label_details->setStyleSheet("background: transparent; Text-align: left; font-family: century gothic; font-size: 15px; color: black; border: none;");


                    layout->addWidget(buttonName);
                    layout->addWidget(label);
                    layout->addWidget(label_details);
                    layout->addWidget(frame_line);
                    ui->scrollAreaContents->setLayout(layout);

                    connect(buttonName, SIGNAL(clicked()), this, SLOT(on_scroll_button_name_clicked()));
            }

        }
        mydb.connClose();
    }

    //setting up table in progress and running
    if(mydb.connOpen()){
        QSqlQuery *qry = new QSqlQuery;
        QSqlQueryModel *model = new QSqlQueryModel;

        if(qry->exec("select Name, Assinged_to, Status, Deadline from Projects where Assinged_to is not null and (Status <> 'Complete' or Status is null)")){
            model->setQuery(*qry);
            ui->tableView->setModel(model);
            mydb.connClose();
            qry = nullptr;
            model = nullptr;
        }
    }
    if(mydb.connOpen()){
        qry = new QSqlQuery;
        model = new QSqlQueryModel;
        if(qry->exec("select Name, Assinged_to, Status, Deadline from Projects where Assinged_to is not null and Status = 'Complete'")){
            model->setQuery(*qry);
            ui->tableView_2->setModel(model);
            mydb.connClose();
        }
    }

    running_project = ui->tableView->model()->rowCount();
    complete_project = ui->tableView_2->model()->rowCount();
    ui->label_ppro->setText("Pending Projects :     \t"+QString::number(pending_project));
    ui->label_rpro->setText("Running Projects :     \t"+QString::number(running_project));
    ui->label_cpro->setText("Completed Projects :   \t"+QString::number(complete_project));


    // for chart
    makePlot(admin, dep, des, free, running_project, complete_project, pending_project);

    for(int i=0;i< ui->listWidget_pro->count(); i++)
        ui->listWidget_pro->item(i)->setSizeHint(QSize(120,50));

    // shawdow for message profile system

        CustomShadowEffect *bodyShadow = new CustomShadowEffect();
        bodyShadow->setBlurRadius(20.0);
        bodyShadow->setDistance(6.0);
        bodyShadow->setColor(QColor(0, 0, 0, 80));
        ui->frame_7->setGraphicsEffect(bodyShadow);

}

Admin_Panel::~Admin_Panel()
{
    delete ui;
}

void Admin_Panel::on_pushButton_logout_clicked()
{
    QApplication::exit(-123);
}

void Admin_Panel::on_pushButton_mdash_clicked()
{
    if(menu_clicked){
        on_pushButton_menu_clicked();
        menu_clicked = false;
    }
    ui->stackedWidget->setCurrentIndex(0);
}

void Admin_Panel::on_pushButton_sett_clicked()
{
    if(menu_clicked){
        on_pushButton_menu_clicked();
        menu_clicked = false;
    }
    ui->stackedWidget->setCurrentIndex(1);
    QString username, name, pass, dept, address, email, dob, mbl, status;
    QSqlQuery *qry;
    QSqlQueryModel *model = new QSqlQueryModel;
    username = ui->label_username->text();
    status = ui->label_stats_2->text();

    if (status == "Admin") {
        ui->lineEdit_5->setFocusPolicy(Qt::NoFocus);
    }

    qDebug() << username;
    if(mydb.connOpen()){
        qry = new QSqlQuery;
        if(qry->exec("select * from AllMemberDetails where User_id = '"+username+"'")){
            model->setQuery(*qry);
            name = model->record(0).value(0).toString();
            pass = model->record(0).value(3).toString();
            dept = model->record(0).value(13).toString();
            address = model->record(0).value(14).toString();
            email = model->record(0).value(7).toString();
            dob = model->record(0).value(11).toString();
            mbl = model->record(0).value(15).toString();
            qDebug() << address;

            ui->lineEdit_1->setText(name);
            ui->lineEdit_2->setText(email);
            ui->lineEdit_3->setText(username);
            ui->lineEdit_4->setText(pass);
            ui->lineEdit_6->setText(pass);
            ui->lineEdit_5->setText(dept);
            ui->lineEdit_7->setText(address);
            ui->lineEdit_9->setText(mbl);
            mydb.connClose();

            QString month = dob.split(',').first().split(' ').last();
            ui->lineEdit_day->setText(dob.left(2));
            ui->lineEdit_year->setText(dob.right(4));
            ui->comboBox_month->setCurrentIndex(ui->comboBox_month->findText(month));
        }
    }

}

void Admin_Panel::on_pushButton_manage_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->stackedWidget_2->setCurrentIndex(0);
}

void Admin_Panel::on_pushButton_menu_clicked()
{
    QRect close = QRect(1340,65,0,0);
    QRect open = QRect(1200,65,171,171);

    menu_anim = new QPropertyAnimation(ui->frame_menu, "geometry");
    menu_clicked = true;
    if(ui->frame_menu->geometry() == close){
        menu_anim->stop();
        menu_anim->setStartValue(close);
        menu_anim->setEndValue(open);
        menu_anim->setDuration(300);
        menu_anim->setEasingCurve(QEasingCurve::OutQuint);
        menu_anim->start();
    }else if (ui->frame_menu->geometry() == open){
        menu_anim->stop();
        menu_anim->setStartValue(open);
        menu_anim->setEndValue(close);
        menu_anim->setDuration(150);
        menu_anim->setEasingCurve(QEasingCurve::OutQuint);
        menu_anim->start();
    }
}

void Admin_Panel::on_pushButton_cpic_clicked()
{
    pic_path = QFileDialog::getOpenFileName(this, tr("choose"), "", tr("Images (*jpg *png *jpeg)"));
    QPixmap pix(pic_path);
    ui->label_pic_2->setPixmap(pix.scaled(ui->label_pic_2->width(), ui->label_pic_2->height(), Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

void Admin_Panel::on_pushButton_update_clicked()
{
    QString name, email, picPath, user_id, pass, c_pass, dob, mob;


    if (ui->lineEdit_1->text() == "" || ui->lineEdit_2->text() == "" || ui->lineEdit_3->text() == "" || ui->lineEdit_4->text() == "" || ui->lineEdit_6->text() == "" || ui->lineEdit_7->text() == "" || ui->lineEdit_9->text() == ""|| ui->lineEdit_day->text() == "" || ui->lineEdit_year->text() == "" ) {
        ui->label_incorr->setVisible(true);
    } else {
        ui->label_incorr->setVisible(false);
        QTimer::singleShot(5,ui->label_corr, SLOT(show()));
        QTimer::singleShot(2500,ui->label_corr, SLOT(hide()));
        QString username, name, pass, c_pass, dept, address, email, dob, mbl;
        QSqlQuery *qry;

        username = ui->label_username->text();

        if(mydb.connOpen()){
            qry = new QSqlQuery;
            name = ui->lineEdit_1->text();
            pass = ui->lineEdit_4->text();
            c_pass = ui->lineEdit_6->text();
            dept = ui->lineEdit_5->text();
            address = ui->lineEdit_7->text();
            email = ui->lineEdit_2->text();
            dob = ui->lineEdit_day->text() + " " + ui->comboBox_month->currentText() + ", " + ui->lineEdit_year->text();
            qDebug() << dob;
            mbl = ui->lineEdit_9->text();

            qDebug() << address;

            if (pass == c_pass) {
                if(qry->exec("update AllMemberDetails set Name = '"+name+"', Password = '"+pass+"', Department = '"+dept+"', Address = '"+address+"', Email = '"+email+"',DOB = '"+dob+"', Mobile = '"+mbl+"' where User_id = '"+username+"'")){
                    mydb.connClose();
                    if(pic_path != ""){
                        if(mydb.connOpen()){
                            qry = nullptr;
                            qry = new QSqlQuery;
                            if(qry->exec("update AllMemberDetails set Picture = '"+pic_path+"' where User_id = '"+username+"'")){
                                mydb.connClose();
                                QPixmap pix(pic_path);
                                ui->label_menu->setPixmap(pix.scaled(ui->label_menu->width(), ui->label_pic_2->height(), Qt::KeepAspectRatio,Qt::SmoothTransformation));
                                ui->label_pic->setPixmap(pix.scaled(ui->label_pic->width(), ui->label_pic_2->height(), Qt::KeepAspectRatio,Qt::SmoothTransformation));
                            }
                        }
                    }
                }
            } else ui->label_incorr->setVisible(true);
        }
    }
}

void Admin_Panel::on_scroll_button_name_clicked()
{
   ui->frame_8->setVisible(false);

   if (trackforScroll > 0) {
         pushButton->setStyleSheet("QPushButton#buttonNameObject {background: transparent; Text-align: left; font-family:""Segoe UI Semibold""; font-size: 15px; color: #6fa000; border: none;}QPushButton#buttonNameObject:hover {border:none; font-size: bold; color: #2f8700;}");
   }

    pushButton = new QPushButton;
    pushButton = qobject_cast<QPushButton*>(sender());

    pushButton->setStyleSheet("QPushButton#buttonNameObject {background: transparent; Text-align: left; color:#2f8700; border: none; font-size:15px;font-family:""Segoe UI Semibold"";}");
    if (trackforScroll == 0) trackforScroll++;


    QString str = pushButton->text();
    QString Set_lbl;

    if(mydb.connOpen()) {
        qDebug() << "connection open for scroll button";
        QSqlQuery *qry = new QSqlQuery;
        if (qry->exec("select Name, Department, Amount, Duration from Projects where Name = '"+str+"'")) {
            qDebug() << "hello from scroll";
            while (qry->next()) {
                Set_lbl = "Work type : " + (qry->value(0).toString()) + "\n\n" + "Department: " + (qry->value(1).toString()) + "\n\n" + "Amount : " + (qry->value(2).toString()) + "\n\n" + "Duration : " + (qry->value(3).toString());
                getDataFromScrollArea  = qry->value(1).toString(); // department
                scrollAreaToAssign = qry->value(0).toString(); // work type
                getDurationData = qry->value(3).toString();
                ui->label_28->setText(Set_lbl);

                assign_anim = new QPropertyAnimation(ui->frame_assign_bg, "geometry");
                assign_anim->setStartValue(ui->frame_assign_bg->geometry());
                assign_anim->setEndValue(QRect(20,30,1031,571));
                assign_anim->setDuration(500);
                assign_anim->setEasingCurve(QEasingCurve::OutQuint);
                assign_anim->start();
            }
        }
        mydb.connClose();

        // for table view_3

        if (mydb.connOpen())
        {

             QSqlQuery *qry = new QSqlQuery;
             modelForAssign = new QSqlQueryModel;

            if (qry->exec("select Name, Department, User_id from AllMemberDetails where Department = '"+getDataFromScrollArea+"'")) {

                qDebug() << getDataFromScrollArea;
                modelForAssign ->setQuery(*qry);
            }

            ui->tableView_3->setModel(modelForAssign);
        }
        mydb.connClose();
    }
}

void Admin_Panel::on_pushButton_abort_clicked()
{
    assign_anim->stop();
    assign_anim->setStartValue(ui->frame_assign_bg->geometry());
    assign_anim->setEndValue(QRect(1070,30,20,571));
    assign_anim->setDuration(300);
    assign_anim->setEasingCurve(QEasingCurve::OutQuint);
    assign_anim->start();
}

void Admin_Panel::on_pushButton_assign_clicked()
{
    QString dataFromTable = modelForAssign->record(indexForAssign).value(2).toString();
    QString name, status, deadline, user_id;
    name = modelForAssign->record(indexForAssign).value(0).toString();
    user_id = modelForAssign->record(indexForAssign).value(2).toString();
    qDebug() << scrollAreaToAssign;
    qDebug() << getDataFromScrollArea;
    if (mydb.connOpen()) {
        QSqlQuery *qry = new QSqlQuery;
        if (qry->exec("update Projects set Assinged_to = '"+name+"', Status = 'Running', Deadline = '19 : 9 : 2019', User_id = '"+user_id+"' where Name = '"+scrollAreaToAssign+"'"))
            qDebug() << "updated projects";
    }
    mydb.connClose();
}

void Admin_Panel::on_tableView_3_clicked(const QModelIndex index)
{
    ui->frame_8->setVisible(true);
    QString dataFromTable = modelForAssign->record(index.row()).value(2).toString();
    indexForAssign = index.row(); //...............
    QString SetData;
    if (mydb.connOpen())
    {
        QSqlQuery *qry = new QSqlQuery;
        if (qry->exec("select Picture, Availablity, Project_done, Specialist, Name from AllMemberDetails where User_id = '"+dataFromTable+"'")) {

            while (qry->next()) {
                QPixmap pix(qry->value(0).toString());
                ui->label_27->setPixmap(pix.scaled(ui->label_27->width(), ui->label_27->height(), Qt::KeepAspectRatio,Qt::SmoothTransformation));

                SetData = "Work Type : " + (scrollAreaToAssign) + "\n" + "Available : " + (qry->value(1).toString()) + "\n" + "Project done : " + (qry->value(2).toString() + "\n" + "Duration : " + getDurationData + " month");
                ui->label_28->setStyleSheet("color: black; font-size: 15px bold;");
                ui->label_28->setText(SetData);
                ui->label_specialist->setText(qry->value(3).toString());
                ui->label_30->setText(qry->value(4).toString() + ".");
            }
        }
    }
    mydb.connClose();
}

void Admin_Panel::makePlot(int a, int d, int de, int free, int rp, int cp, int pp)
{
    // create empty bar chart objects:
       QCPBars *total_admin = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
       QCPBars *total_freelancer = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
       QCPBars *total_designation = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
       QCPBars *total_department = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
       QCPBars *pending_pro = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
       QCPBars *running_pro = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
       QCPBars *complete_pro = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);

       total_admin->setAntialiased(false); // gives more crisp, pixel aligned bar borders
       total_freelancer->setAntialiased(false);
       total_designation->setAntialiased(false);
       total_department->setAntialiased(false);
       pending_pro->setAntialiased(false);
       running_pro->setAntialiased(false);
       complete_pro->setAntialiased(false);

   // set names and colors:
       total_designation->setName("Designations");
       total_designation->setPen(QPen(QColor(80, 144, 255).lighter(0)));
       total_designation->setBrush(QColor(80, 144, 255, 255));

       total_freelancer->setName("Freelancers");
       total_freelancer->setPen(QPen(QColor(110, 110, 255).lighter(130)));
       total_freelancer->setBrush(QColor(110, 110, 255, 250));

       total_admin->setName("Admins");
       total_admin->setPen(QPen(QColor(248, 94, 0).lighter(130)));
       total_admin->setBrush(QColor(248, 94, 0, 250));

       total_department->setName("Departments");
       total_department->setPen(QPen(QColor(0, 179, 170).lighter(130)));
       total_department->setBrush(QColor(0, 179, 170, 250));

       pending_pro->setName("Pending Projects");
       pending_pro->setPen(QPen(QColor(171, 120, 221).lighter(130)));
       pending_pro->setBrush(QColor(171, 120, 221, 250));

       running_pro->setName("Running Projects");
       running_pro->setPen(QPen(QColor(254, 200, 27).lighter(130)));
       running_pro->setBrush(QColor(254, 200, 27, 250));

       complete_pro->setName("Complete Projects");
       complete_pro->setPen(QPen(QColor(53, 167, 115).lighter(130)));
       complete_pro->setBrush(QColor(53, 167, 115, 250));

   // prepare x axis with country labels:
       QVector<double> ticks;
       QVector<QString> labels;
       ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7;
       //labels << "Admin" << "Freelancer" << "Departmrnt" << "Designation" << "Pending Project" << "Running Project" << "Completed Project" ;

       QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
       textTicker->addTicks(ticks, labels);
       ui->customPlot->xAxis->setTicker(textTicker);
       ui->customPlot->xAxis->setTickLabelRotation(60);
       ui->customPlot->xAxis->setSubTicks(false);
       ui->customPlot->xAxis->setTickLength(0, 6);
       ui->customPlot->xAxis->setRange(0, 9);
       ui->customPlot->xAxis->setBasePen(QPen(Qt::black));
       ui->customPlot->xAxis->setTickPen(QPen(Qt::black));
       ui->customPlot->xAxis->grid()->setVisible(false);
       //ui->customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
       ui->customPlot->xAxis->setTickLabelColor(Qt::black);
       ui->customPlot->xAxis->setLabelColor(Qt::black);

       // prepare y axis:
       ui->customPlot->yAxis->setRange(0, free+1);
       ui->customPlot->yAxis->setPadding(5); // a bit more space to the left border
       ui->customPlot->yAxis->setLabel("Number of Members of this app");
       //ui->customPlot->yAxis->setBasePen(QPen(Qt::black));
       ui->customPlot->yAxis->setTickPen(QPen(Qt::black));
       //ui->customPlot->yAxis->setSubTickPen(QPen(Qt::black));
       //ui->customPlot->yAxis->grid()->setSubGridVisible(true);
       ui->customPlot->yAxis->setTickLabelColor(Qt::black);
       ui->customPlot->yAxis->setLabelColor(Qt::black);
       ui->customPlot->yAxis->grid()->setPen(QPen(QColor(231, 236, 241), 0, Qt::SolidLine));
       //ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

       // Add data:
       QVector<double> total_designationData, total_freelancerData, total_adminData, total_departmentData, Pending_project, Completed_project, Running_project;
       total_designationData  << 0 << 0 << 0 << de << 0 << 0 << 0;
       total_freelancerData << 0 << free << 0 << 0 << 0 << 0 << 0;
       total_adminData   << a << 0 << 0 << 0 << 0 << 0 << 0;
       total_departmentData << 0 << 0 << d << 0 << 0 << 0 << 0;
       Pending_project << 0 << 0 << 0 << 0 << pp << 0 << 0;
       Running_project << 0 << 0 << 0 << 0 << 0 << rp << 0;
       Completed_project << 0 << 0 << 0 << 0 << 0 << 0 << cp;

       total_designation->setData(ticks, total_designationData);
       total_freelancer->setData(ticks, total_freelancerData);
       total_admin->setData(ticks, total_adminData);
       total_department->setData(ticks, total_departmentData);
       pending_pro->setData(ticks, Pending_project);
       running_pro->setData(ticks, Running_project);
       complete_pro->setData(ticks, Completed_project);

       // setup legend:
       ui->customPlot->legend->setVisible(true);
       ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
       ui->customPlot->legend->setBrush(QColor(255, 255, 255, 100));
       ui->customPlot->legend->setBorderPen(Qt::NoPen);
       QFont legendFont = font();
       legendFont.setPointSize(10);
       ui->customPlot->legend->setFont(legendFont);
}

void Admin_Panel::on_lineEdit_massage_textChanged(const QString &arg1)
{
    if(arg1.size() > 0){
        ui->label_send_message->setVisible(true);
        ui->pushButton_send_message->setIcon(QIcon(":/imgs/send_active.png"));
    }
    else{
        ui->pushButton_send_message->setIcon(QIcon(":/imgs/send.png"));
        ui->label_send_message->setVisible(false);
    }
}

void Admin_Panel::keyPressEvent(QKeyEvent *event)
{
    qDebug() << event->key();
    if((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)){
        if(ui->stackedWidget->currentIndex() == 3 && ui->lineEdit_massage->text().size() > 0)
            on_pushButton_send_message_clicked();
    }
}

void Admin_Panel::on_pushButton_message_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

    if(menu_clicked){
        on_pushButton_menu_clicked();
        menu_clicked = false;
    }

    QPushButton *button;
    QLabel * label_timeDate, *label_mess;
    QString mess, time, date, status;

    if (loaded == false) {
        loaded = true;
        if (mydb.connOpen()) {
            QSqlQuery * qry = new QSqlQuery;

            if (qry->exec("select Name, Message, Time, Date, Status from Message")) {
                qDebug() << "selected message";

                while (qry->next()) {
                    button = new QPushButton(qry->value(0).toString());
                    mess = qry->value(1).toString();
                    time = qry->value(2).toString();
                    date = qry->value(3).toString();
                    status = qry->value(4).toString();

                    label_timeDate= new QLabel;
                    label_mess = new QLabel;

                    label_timeDate->setText("   " + date + ", " + "at " + time);
                    label_timeDate->setStyleSheet("background: transparent; color: #bbbbbb; border: none; font-family: century gothic; font-size: 12px;");

                    label_mess->setText(mess);
                    label_mess->setStyleSheet("background: transparent; Text-align: left; font-family: century gothic; font-size: 15px; color: black; border: none;");


                    button->setObjectName("buttonNameObject");
                    button->setStyleSheet("QPushButton#buttonNameObject {background: transparent; Text-align: left; font-family:""Segoe UI Semibold""; font-size: 15px; color: #6fa000; border: none;}QPushButton#buttonNameObject:hover {border:none; font-size: bold; color: #2f8700;}");

                    QFrame *frame_line = new QFrame();
                    frame_line->setFrameShape(QFrame::HLine);
                    frame_line->setFrameShadow(QFrame::Sunken);
                    frame_line->setStyleSheet("background:gray");

                    lay->addRow(button, label_timeDate);
                    lay->addItem(new QSpacerItem(0, 7, QSizePolicy::Expanding, QSizePolicy::Expanding));
                    lay->addRow(label_mess);
                    //lay->addItem(new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
                    lay->addRow(frame_line);
                    lay->addItem(new QSpacerItem(0,10, QSizePolicy::Expanding, QSizePolicy::Expanding));

                    ui->scrollAreaMessage->setLayout(lay);

                    connect(button, SIGNAL(clicked()), this, SLOT(showProfileFromMessage_clicked()));
                    connect(scrollbar, SIGNAL(rangeChanged(int , int)), this, SLOT(moveScrollBarToBottom(int , int)));
                    RowCount++;
                }
            }
        }
    }
}

void Admin_Panel::on_lineEdit_search_textChanged(const QString &search_txt)
{
    if(mydb.connOpen()){
        QSqlQuery *qry = new QSqlQuery;
        modelForAssign = new QSqlQueryModel;

        if (qry->exec("select Name, Department, User_id from AllMemberDetails where Department = '"+getDataFromScrollArea+"' and (Name like '%"+search_txt+"%' or User_id like '%"+search_txt+"%')")) {
            modelForAssign ->setQuery(*qry);
        }
        ui->tableView_3->setModel(modelForAssign);
    }
    mydb.connClose();
}

void Admin_Panel::load_tableview_data(QTableView *table, QString query)
{
    if(mydb.connOpen()){
        QSqlQuery *qry = new QSqlQuery;
        QSqlQueryModel *model = new QSqlQueryModel;

        if(qry->exec(query))
            model->setQuery(*qry);
        table->setModel(model);
    }
    mydb.connClose();
}

void Admin_Panel::on_pushButton_manage_user_clicked()
{
    if(menu_clicked){
        on_pushButton_menu_clicked();
        menu_clicked = false;
    }
    ui->stackedWidget->setCurrentIndex(4);
    load_tableview_data(ui->tableView_user, "select Name, User_id, Email, Department from AllMemberDetails");

    ui->frame_user_details->setGeometry(1051,0,0,681);
}

void Admin_Panel::on_lineEdit_search_2_textChanged(const QString &search_txt)
{
    load_tableview_data(ui->tableView_user, "select Name, User_id, Email, Department from AllMemberDetails where Name like '%"+search_txt+"%' or User_id like '%"+search_txt+"%' or Email like '%"+search_txt+"%';");
}

void Admin_Panel::on_tableView_user_clicked(const QModelIndex &index)
{
    if(mydb.connOpen()){
        QSqlQuery *qry = new QSqlQuery;
        QSqlQueryModel *model = new QSqlQueryModel;

        if(qry->exec("select Name, User_id, Email, Department from AllMemberDetails")){
            model->setQuery(*qry);
        }

        user_username = model->record(index.row()).value(1).toString();

        qry = new QSqlQuery;

        if(qry->exec("select Name, Specialist, Email, Department, Picture, Project_done, Status from AllMemberDetails where User_id = '"+user_username+"'")){
            while(qry->next()){
                QPixmap pix(qry->value(4).toString());
                ui->label_user_pic->setPixmap(pix.scaled(ui->label_user_pic->width(), ui->label_user_pic->height(), Qt::KeepAspectRatio,Qt::SmoothTransformation));
                qDebug() << qry->value(4).toString();
                ui->label_user_name->setText(qry->value(0).toString());
                ui->label_user_username->setText(user_username);
                ui->label_user_specialist->setText(qry->value(1).toString());
                ui->label_user_email->setText(qry->value(2).toString());
                ui->label_user_dep->setText(qry->value(3).toString());
                qDebug() << qry->value(3).toString();
                if(qry->value(5).toString().size() != 0){
                    ui->label_user_project_done_2->setText(qry->value(5).toString());
                    ui->label_user_project_done_2->setStyleSheet("border:none; color:#6fa000;");
                }
                else{
                    ui->label_user_project_done_2->setText("Null");
                    ui->label_user_project_done_2->setStyleSheet("border:none; color:grey;");
                }

                status = qry->value(6).toString();
            }

            qry = new QSqlQuery;
            if(qry->exec("select Name from Projects where User_id = '"+user_username+"' and Status = 'Running'")){
                if(qry->next()){
                    ui->label_user_woking_project_2->setText(qry->value(0).toString());
                    ui->label_user_woking_project_2->setStyleSheet("border:none; color:#6fa000;");
                }
                else{
                    ui->label_user_woking_project_2->setText("Null");
                    ui->label_user_woking_project_2->setStyleSheet("border:none; color:grey;");
                }
            }
        }


        mydb.connClose();
    }


    //ui->frame_user_details->setGeometry(0,0,1071,681);
    user_anim = new QPropertyAnimation(ui->frame_user_details, "geometry");
    user_anim->setStartValue(ui->frame_user_details->geometry());
    user_anim->setEndValue(QRect(0,0,1071,681));
    user_anim->setDuration(300);
    user_anim->setEasingCurve(QEasingCurve::OutQuint);
    user_anim->start();
}

void Admin_Panel::on_pushButton_back_clicked()
{
    //ui->frame_user_details->setGeometry(1051,0,0,681);
    user_anim->stop();
    user_anim->setStartValue(ui->frame_user_details->geometry());
    user_anim->setEndValue(QRect(1051,0,0,681));
    user_anim->setDuration(300);
    user_anim->setEasingCurve(QEasingCurve::OutQuint);
    user_anim->start();
}

void Admin_Panel::on_pushButton_user_remove_clicked()
{
    if(status != "Admin"){
        QMessageBox::StandardButton ans = QMessageBox::question(this, "Confirm", "Are you sure to delete this user from the database ? ", QMessageBox::Yes | QMessageBox::No);
        if(ans == QMessageBox::Yes){
            if(mydb.connOpen()){
                QSqlQuery *qry = new QSqlQuery;
                if(qry->exec("delete from AllMemberDetails where User_id = '"+user_username+"'")){
                    on_pushButton_manage_user_clicked();
                }
                mydb.connClose();
            }
        }
    } else
        QMessageBox::critical(this, "Failed", "You can't delete an Admin.");

}

void Admin_Panel::on_pushButton_send_message_clicked()
{
    int timeModify;

    QDate *date = new QDate;
    QTime *time = new QTime;
    QString message, name, status, timeStr, dateStr, user_id;

    timeStr = time->currentTime().toString();
    timeStr = timeStr.left(5);
    dateStr = date->currentDate().toString();
    message = ui->lineEdit_massage->text();
    name = ui->label_sname->text();
    status = ui->label_stats_2->text();
    user_id = ui->label_username->text();

    timeModify = timeStr.left(2).toInt();
    qDebug() << timeModify;

    if (timeModify >= 12 || timeModify >= 24) {
            if (timeModify > 12) timeModify =  timeModify - 12;
            else if (timeModify >= 24) timeModify = timeModify - 24;
            timeStr = QString::number(timeModify) + timeStr.right(3) + " PM";
        } else timeStr = timeStr + " AM";

    qDebug() << timeStr;

    QPushButton *button = new QPushButton(name);
    QLabel * label_timeDate = new QLabel, *label_mess = new QLabel;

    label_timeDate->setText("   " + dateStr + ", " + "at " + timeStr);
    label_timeDate->setStyleSheet("background: transparent; color: #bbbbbb; border: none; font-family: century gothic; font-size: 12px;");

    label_mess->setText(message);
    label_mess->setStyleSheet("background: transparent; Text-align: left; font-family: century gothic; font-size: 15px; color: black; border: none;");

    button->setObjectName("buttonNameObject");
    button->setStyleSheet("QPushButton#buttonNameObject {background: transparent; Text-align: left; font-family:""Segoe UI Semibold""; font-size: 15px; color: #6fa000; border: none;}QPushButton#buttonNameObject:hover {border:none; font-size: bold; color: #2f8700;}");

    QFrame *frame_line = new QFrame();
    frame_line->setFrameShape(QFrame::HLine);
    frame_line->setFrameShadow(QFrame::Sunken);
    frame_line->setStyleSheet("background:gray");


    lay->addRow(button, label_timeDate);
    lay->addItem(new QSpacerItem(0, 7, QSizePolicy::Expanding, QSizePolicy::Expanding));
    lay->addRow(label_mess);
    //lay->addItem(new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
    lay->addRow(frame_line);
    lay->addItem(new QSpacerItem(0,10, QSizePolicy::Expanding, QSizePolicy::Expanding));
    connect(button, SIGNAL(clicked()), this, SLOT(showProfileFromMessage_clicked()));
    connect(scrollbar, SIGNAL(rangeChanged(int , int)), this, SLOT(moveScrollBarToBottom(int , int)));
    RowCount++;

    if (mydb.connOpen()) {
        QSqlQuery *qry = new QSqlQuery;
        if (qry->exec("insert into Message (Name, Message, Time, Date, Status, User_id) values ('"+name+"', '"+message+"', '"+timeStr+"', '"+dateStr+"', '"+status+"', '"+user_id+"')")){
            mydb.connClose();
        }
       ui->lineEdit_massage->clear();
    }
}

void Admin_Panel::showProfileFromMessage_clicked()
{
    QPixmap pix;
    pushButton = new QPushButton;
    pushButton = qobject_cast<QPushButton*>(sender());
    QString name = pushButton->text();
    int xx = pushButton->x();
    int yy = pushButton->y();
    QString row,  user_id, picPath, specialist_details;
    if (yy/71 == 0) row = QString::number(1);
    else row = QString::number((yy-9)/71 + 1);

    qDebug() << yy << " -" << row;

    pushButton->setStyleSheet("QPushButton#buttonNameObject {background: transparent; Text-align: left; color:#2f8700; border: none;font-size:15px;font-family:""Segoe UI Semibold"";}");
    // retriving the data
    if (mydb.connOpen()) {
        QSqlQuery *qry = new QSqlQuery;

        if (qry->exec("select User_id from Message where ROWID is '"+row+"'")) {
            qDebug() << "searching  for message";

            while(qry->next()) {
                user_id = qry->value(0).toString();
            }

            if (qry->exec("select Picture, Specialist from AllMemberDetails where User_id = '"+user_id+"'")) {
                while(qry->next()) {
                    picPath = qry->value(0).toString();
                    specialist_details = qry->value(1).toString();
                }
            }
        }
        mydb.connClose();
    }
    if (y != yy and (QString::number(RowCount) == row)) {
        ui->frame_7->show();
        ui->frame_7->activateWindow();
        ui->frame_7->raise();
        ui->label_proName->setText(name);
        ui->label_proDetails->setText(specialist_details);
        pix.load(picPath);
        ui->label_profile_pic->setPixmap(pix);

        QPropertyAnimation *animation = new QPropertyAnimation(ui->frame_7, "geometry");
        animation->setDuration(200);
        animation->setEasingCurve(QEasingCurve::OutQuint);
        animation->setStartValue(QRect(xx+200, yy-40, 0, 0));
        animation->setEndValue(QRect(xx+300, yy-40, 421, 111));
        animation->start();
        x = xx;
        y = yy;
    }
    else if (y != yy) {
        ui->frame_7->show();
        ui->frame_7->activateWindow();
        ui->frame_7->raise();
        ui->label_proName->setText(name);
        ui->label_proDetails->setText(specialist_details);
        pix.load(picPath);
        ui->label_profile_pic->setPixmap(pix);

        QPropertyAnimation *animation = new QPropertyAnimation(ui->frame_7, "geometry");
        animation->setDuration(200);
        animation->setEasingCurve(QEasingCurve::OutQuint);
        animation->setStartValue(QRect(xx+200, yy, 0, 0));
        animation->setEndValue(QRect(xx+300, yy, 421, 111));
        animation->start();
        x = xx;
        y = yy;
    }
    else if (y == yy) {
        QPropertyAnimation *animation = new QPropertyAnimation(ui->frame_7, "geometry");
        pushButton = new QPushButton;
        pushButton = qobject_cast<QPushButton*>(sender());
        QString name = pushButton->text();
        x = ui->frame_7->x();
        y = ui->frame_7->y();
        animation->setDuration(200);
        animation->setEasingCurve(QEasingCurve::OutQuint);
        animation->setStartValue(QRect(ui->frame_7->geometry()));
        animation->setEndValue(QRect(x+400, y-10, 0, 0));
        animation->start();
        x = 0;
        y = 0;
    }

}

void Admin_Panel::moveScrollBarToBottom(int min, int max)
{
    Q_UNUSED(min)
    ui->scrollArea_message->verticalScrollBar()->setValue(max);
}

void Admin_Panel::on_pushButton_hide_clicked()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frame_7, "geometry");
    pushButton = new QPushButton;
    pushButton = qobject_cast<QPushButton*>(sender());
    QString name = pushButton->text();
    x = ui->frame_7->x();
    y = ui->frame_7->y();
    animation->setDuration(200);
    animation->setEasingCurve(QEasingCurve::OutQuint);
    animation->setStartValue(QRect(ui->frame_7->geometry()));
    animation->setEndValue(QRect(x+400, y-10, 0, 0));
    animation->start();
    x = 0;
    y = 0;
}

void Admin_Panel::on_lineEdit_day_editingFinished()
{
    if(ui->lineEdit_day->text().toInt() == 0)
        ui->lineEdit_day->setText("01");
    else if((ui->lineEdit_day->text().toInt() < 10) && (ui->lineEdit_day->text().size() < 2))
        ui->lineEdit_day->setText("0" + ui->lineEdit_day->text());
    else if(ui->lineEdit_day->text().toInt() > 31){
        ui->lineEdit_day->setStyleSheet("*{border-radius:5px; border:1px solid red; color:red;}:hover{border: 1px solid red;}:focus{border: 1px solid red;}");
    } else
        ui->lineEdit_day->setStyleSheet("*{border-radius:5px;}:hover{border: 1px solid #34a671;}:focus{border: 1px solid #34a671;}");
}
