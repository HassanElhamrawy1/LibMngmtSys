#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


/* function to check if the book exist in the database or not */
bool bookExists(int id)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM books WHERE id = ?");
    query.addBindValue(id);
    if (query.exec() && query.next())
        return query.value(0).toInt() > 0;

    return false;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Load the dataBase after running */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("library.db");
    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", db.lastError().text());
        return;
    }

    /* Create the table if it is not exist */
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS books ("
               "id INTEGER PRIMARY KEY, "
               "title TEXT, "
               "author TEXT, "
               "borrowed INTEGER)");

    /* print the books directly into the widget */
    updateTable(); /*  */

    /* Connect the window elements (Buttons and Widget) to the API handling the click */
    connect(ui->btnLoadBooks, &QPushButton::clicked, this, &MainWindow::handleLoadBooks);
    connect(ui->btnSaveBooks, &QPushButton::clicked, this, &MainWindow::handleSaveBooks);
    connect(ui->btnUpdate, &QPushButton::clicked, this, &MainWindow::handleUpdateBooks);
    connect(ui->btn_AddBook, &QPushButton::clicked, this, &MainWindow::handleAddBook);
    connect(ui->btn_DeleteBook, &QPushButton::clicked, this, &MainWindow::handleDeleteBook);
    connect(ui->btn_Search, &QPushButton::clicked, this, &MainWindow::handleSearch);
    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &MainWindow::on_tableWidget_cellClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTable()
{
    ui->tableWidget->setRowCount(0);
    QSqlQuery query("SELECT id, title, author, borrowed FROM books");

    /* for each record inside the query */
    while (query.next())
    {
        int row = ui->tableWidget->rowCount();   /* get the row count to use it to insert the new row */
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("title").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("author").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("borrowed").toBool() ? "Borrowed" : "Available"));
    }
}

void MainWindow::handleLoadBooks()
{
    /* create database object */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("library.db");
    /* show error message if the database is already open */
    if (!db.open())
    {
        QMessageBox::critical(this, "Database Error", db.lastError().text());
        return;
    }
    /* select all column from books table */
    QSqlQuery query("SELECT id, title, author, borrowed FROM books", db);

    /* clear the table from the widget */
    ui->tableWidget->setRowCount(0);

    int row = 0;
    /* insert all the table rows on the widget  */
    while (query.next())
    {
        ui->tableWidget->insertRow(row);
        /* add the 4 values id, title, author, borrowed to the row on the widget */
        for (int col = 0; col < 4; ++col)
        {
            QString value = query.value(col).toString();
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(value));
        }

        row++;
    }
    /* close the database */
    db.close();
}

void MainWindow::handleSaveBooks()
{
    /* To be removed */
    library.saveToFile("books.txt");
}

void MainWindow::handleAddBook()
{
    int id = ui->txtId->text().toInt();

    if (bookExists(id))
    {
        QMessageBox::warning(this, "Duplicate", "A book with this ID already exists.");
        return;
    }

    QString title = ui->txtTitle->text();
    QString author = ui->txtAuthor->text();
    bool borrowed = ui->checkBoxStatus->isChecked();

    /* make sure that the id is not exist in the books */
    QSqlQuery query;
    query.prepare("INSERT INTO books (id, title, author, borrowed) VALUES (?, ?, ?, ?)");
    query.addBindValue(id);             /* addBind value will put the data in the place of the ? */
    query.addBindValue(title);
    query.addBindValue(author);
    query.addBindValue(borrowed ? 1 : 0);

    if (!query.exec())             /* Execute the query and show the error message if it is not successful */
    {
        QMessageBox::warning(this, "Error", query.lastError().text());
        return;
    }
    /* Update the table value on the Widget */
    updateTable();
}


void MainWindow::handleDeleteBook()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Delete Book", "Please select a row to delete.");
        return;
    }

    int id = ui->tableWidget->item(row, 0)->text().toInt();

    if (!bookExists(id))
    {
        QMessageBox::warning(this, "Not Found", "This book no longer exists in the database.");
        return;
    }

    /* Confirmation message before deleting */
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Deletion", "Delete selected book?", QMessageBox::Yes | QMessageBox::No
        );
    if (reply != QMessageBox::Yes) return;

    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE id = ?");
    query.addBindValue(id);                            /* addBind value will put the data in the place of the ? */
    query.exec();

    /* Update the table value on the Widget */
    updateTable();
}

void MainWindow::handleUpdateBooks()
{
    int id        = ui->txtId->text().toInt();
    QString title = ui->txtTitle->text();
    QString author= ui->txtAuthor->text();
    bool borrowed = ui->checkBoxStatus->isChecked();

    /* Update the data inside the library */
    QSqlQuery query;
    query.prepare("UPDATE books SET title = ?, author = ?, borrowed = ? WHERE id = ?");
    query.addBindValue(title);
    query.addBindValue(author);
    query.addBindValue(borrowed ? 1 : 0);
    query.addBindValue(id);
    query.exec();

    /* Update the table value on the Widget */
    updateTable();
}

void MainWindow::handleSearch()
{
    QString keyword = ui->txtSearch->text().trimmed();
    QString searchBy = ui->comboBoxSearch->currentText();

    if (keyword.isEmpty()) {
        updateTable(); /* show all books if there is no search */
        return;
    }

    ui->tableWidget->setRowCount(0); /* clear the table */

    for (const Book& book : library.getBooks())
    {
        bool match = false;

        if (searchBy == "ID")
            match = QString::number(book.getId()) == keyword;
        else if (searchBy == "Title")
            match = QString::fromStdString(book.getTitle()).contains(keyword, Qt::CaseInsensitive);
        else if (searchBy == "Author")
            match = QString::fromStdString(book.getAuthor()).contains(keyword, Qt::CaseInsensitive);

        if (match) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(book.getId())));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(book.getTitle())));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(book.getAuthor())));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(book.getIsBorrowed() ? "Borrowed" : "Available"));
        }
    }

    if (ui->tableWidget->rowCount() == 0) {
        QMessageBox::information(this, "Search", "No matching books found.");
    }
}

void MainWindow::on_tableWidget_cellClicked(int row, int /*column*/)
{
    // get the values from the table
    ui->txtId->setText(ui->tableWidget->item(row, 0)->text());
    ui->txtTitle->setText(ui->tableWidget->item(row, 1)->text());
    ui->txtAuthor->setText(ui->tableWidget->item(row, 2)->text());
    bool borrowed = ui->tableWidget->item(row, 3)->text() == "Borrowed";
    ui->checkBoxStatus->setChecked(borrowed);
}
