#include <QMainWindow>
#include "Library.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void handleLoadBooks();
        void handleSaveBooks();
        void handleUpdateBooks();
        void handleAddBook();
        void handleDeleteBook();
        void handleSearch();
        void on_tableWidget_cellClicked(int row, int column);
        void on_btnUpdate_clicked();

    private:
        Ui::MainWindow *ui;
        Library library;
        void updateTable();
};
