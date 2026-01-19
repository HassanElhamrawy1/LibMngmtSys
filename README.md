# 📚 LibMngmtSys – Library Management System  
*A Qt C++ Application with SQL Database Integration*

## 📘 Overview
**LibMngmtSys** is a desktop-based Library Management System built using **Qt (C++ Widgets)**, with backend data handling via **QSqlDatabase** and **QSqlQuery**.  
The application provides an intuitive interface for managing library books, tracking their status, and performing essential CRUD operations.

## ✨ Features
- **Add Books** – Insert new book records into the database.  
- **Load Books** – Retrieve and view all stored book data.  
- **Update Books** – Modify existing book details.  
- **Delete Books** – Remove books from the system.  
- **Search Functionality**  
  - Search by **Book ID**  
  - Search by **Title/Text**  
- **Borrow Status Tracking** – Easily view whether a book is available or currently borrowed.
- **User-Friendly GUI** built with Qt Widgets.

## 🛠️ Technologies Used
- **Qt Framework (C++ Widgets)**
- **QSqlDatabase & QSqlQuery**
- **SQLite / MySQL** (Depending on your chosen backend)

## 📂 Project Structure
/LibMngmtSys
│── LibMngmtSys.pro
│── main.cpp
│── mainwindow.cpp
│── mainwindow.h
│── mainwindow.ui
│── Library.cpp
│── User.h
│── Library.h
│── Admin.h
│── Book.h
│── README.md
