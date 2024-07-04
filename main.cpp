#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Book {
public:
  int id;
  std::string title;
  std::string author;
  bool isAvailable;

  Book(int id, const std::string &title, const std::string &author)
      : id(id), title(title), author(author), isAvailable(true) {}
};

class Student {
public:
  int id;
  std::string name;
  std::string className;

  Student(int id, const std::string &name, const std::string &className)
      : id(id), name(name), className(className) {}
};

class IssuedBook {
public:
  int bookId;
  int studentId;
  std::string issueDate;

  IssuedBook(int bookId, int studentId, const std::string &issueDate)
      : bookId(bookId), studentId(studentId), issueDate(issueDate) {}
};

class Library {
private:
  std::vector<Book> books;
  std::vector<Student> students;
  std::vector<IssuedBook> issuedBooks;

  std::string getCurrentDate() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return std::string(buf);
  }

public:
  void addBook() {
    int id;
    std::string title, author;

    std::cout << "Enter Book ID: ";
    std::cin >> id;
    std::cin.ignore();
    std::cout << "Enter Book Title: ";
    std::getline(std::cin, title);
    std::cout << "Enter Book Author: ";
    std::getline(std::cin, author);

    books.emplace_back(id, title, author);
    std::cout << "Book added successfully!" << std::endl;
  }

  void addBook(const Book &book) { books.push_back(book); }

  void deleteBook() {
    int id;
    std::cout << "Enter Book ID to delete: ";
    std::cin >> id;

    auto it = std::find_if(books.begin(), books.end(),
                           [id](const Book &b) { return b.id == id; });

    if (it != books.end()) {
      books.erase(it);
      std::cout << "Book deleted successfully!" << std::endl;
    } else {
      std::cout << "Book not found!" << std::endl;
    }
  }

  void addStudent(const Student &student) { students.push_back(student); }

  void issueBook() {
    int bookId, studentId;

    std::cout << "Enter Book ID: ";
    std::cin >> bookId;
    std::cout << "Enter Student ID: ";
    std::cin >> studentId;

    auto bookIt =
        std::find_if(books.begin(), books.end(),
                     [bookId](const Book &b) { return b.id == bookId; });
    auto studentIt = std::find_if(
        students.begin(), students.end(),
        [studentId](const Student &s) { return s.id == studentId; });

    if (bookIt == books.end()) {
      std::cout << "Book not found!" << std::endl;
      return;
    }

    if (studentIt == students.end()) {
      std::cout << "Student not found!" << std::endl;
      return;
    }

    if (!bookIt->isAvailable) {
      std::cout << "Book is not available for issue!" << std::endl;
      return;
    }

    bookIt->isAvailable = false;
    issuedBooks.emplace_back(bookId, studentId, getCurrentDate());

    std::cout << "Book issued successfully!" << std::endl;
    std::cout << "Issue Details:" << std::endl;
    std::cout << "Book: " << bookIt->title << std::endl;
    std::cout << "Student: " << studentIt->name << std::endl;
    std::cout << "Issue Date: " << issuedBooks.back().issueDate << std::endl;
  }

  void returnBook() {
    int bookId;
    std::cout << "Enter Book ID to return: ";
    std::cin >> bookId;

    auto bookIt =
        std::find_if(books.begin(), books.end(),
                     [bookId](const Book &b) { return b.id == bookId; });

    if (bookIt == books.end()) {
      std::cout << "Book not found!" << std::endl;
      return;
    }

    auto issueIt = std::find_if(
        issuedBooks.begin(), issuedBooks.end(),
        [bookId](const IssuedBook &ib) { return ib.bookId == bookId; });

    if (issueIt == issuedBooks.end()) {
      std::cout << "This book is not issued!" << std::endl;
      return;
    }

    bookIt->isAvailable = true;
    issuedBooks.erase(issueIt);

    std::cout << "Book returned successfully!" << std::endl;
  }

  void displayIssuedBooks() {
    if (issuedBooks.empty()) {
      std::cout << "No books are currently issued." << std::endl;
      return;
    }

    std::cout << std::left << std::setw(10) << "Book ID" << std::setw(20)
              << "Book Title" << std::setw(20) << "Student Name"
              << std::setw(15) << "Issue Date" << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    for (const auto &issue : issuedBooks) {
      auto book =
          std::find_if(books.begin(), books.end(), [&issue](const Book &b) {
            return b.id == issue.bookId;
          });
      auto student = std::find_if(
          students.begin(), students.end(),
          [&issue](const Student &s) { return s.id == issue.studentId; });

      if (book != books.end() && student != students.end()) {
        std::cout << std::left << std::setw(10) << issue.bookId << std::setw(20)
                  << book->title << std::setw(20) << student->name
                  << std::setw(15) << issue.issueDate << std::endl;
      }
    }
  }

  void searchBook() {
    std::string searchTerm;
    std::cout << "Enter book title or author to search: ";
    std::cin.ignore();
    std::getline(std::cin, searchTerm);

    bool found = false;
    std::cout << std::left << std::setw(10) << "Book ID" << std::setw(30)
              << "Title" << std::setw(20) << "Author" << std::setw(15)
              << "Status" << std::endl;
    std::cout << std::string(75, '-') << std::endl;

    for (const auto &book : books) {
      if (book.title.find(searchTerm) != std::string::npos ||
          book.author.find(searchTerm) != std::string::npos) {
        std::cout << std::left << std::setw(10) << book.id << std::setw(30)
                  << book.title << std::setw(20) << book.author << std::setw(15)
                  << (book.isAvailable ? "Available" : "Issued") << std::endl;
        found = true;
      }
    }

    if (!found) {
      std::cout << "No books found matching the search term." << std::endl;
    }
  }

  void listAllBooks() {
    if (books.empty()) {
      std::cout << "No books in the library." << std::endl;
      return;
    }

    std::cout << std::left << std::setw(10) << "Book ID" << std::setw(30)
              << "Title" << std::setw(20) << "Author" << std::setw(15)
              << "Status" << std::endl;
    std::cout << std::string(75, '-') << std::endl;

    for (const auto &book : books) {
      std::cout << std::left << std::setw(10) << book.id << std::setw(30)
                << book.title << std::setw(20) << book.author << std::setw(15)
                << (book.isAvailable ? "Available" : "Issued") << std::endl;
    }
  }
};

int main() {
  Library library;

  // Adding  students
  library.addStudent(Student(101, "RAM", "Class A"));
  library.addStudent(Student(102, "LAXMAN", "Class B"));
  library.addStudent(Student(103, "BHART", "Class A"));
  library.addStudent(Student(104, "SHTRUGAN", "Class C"));

  // Adding  books
  library.addBook(Book(1, " Ramayan", "Valmiki"));
  library.addBook(Book(2, "Mahabharat", "Ved Vyasa"));
  library.addBook(Book(3, "Aryabhatta", "Aryabhatta"));
  library.addBook(Book(4, "Kamasutra", "Vatsayana"));
  library.addBook(Book(5, "Bhagavad Gita", "Ved Vyasa"));
  library.addBook(Book(6, "Bhagavata Purana", "Ved Vyasa"));
  library.addBook(Book(7, "Mahabhasya", "Patanjli"));
  library.addBook(Book(8, "Bible", "Zeus"));

  int choice;
  do {
    std::cout << "\nLibrary Management System" << std::endl;
    std::cout << "1. Add a Book" << std::endl;
    std::cout << "2. Delete a Book" << std::endl;
    std::cout << "3. Issue a Book" << std::endl;
    std::cout << "4. Return a Book" << std::endl;
    std::cout << "5. Display Issued Books" << std::endl;
    std::cout << "6. Search for a Book" << std::endl;
    std::cout << "7. List All Books" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    switch (choice) {
    case 1:
      library.addBook();
      break;
    case 2:
      library.deleteBook();
      break;
    case 3:
      library.issueBook();
      break;
    case 4:
      library.returnBook();
      break;
    case 5:
      library.displayIssuedBooks();
      break;
    case 6:
      library.searchBook();
      break;
    case 7:
      library.listAllBooks();
      break;
    case 0:
      std::cout << "Exiting..." << std::endl;
      break;
    default:
      std::cout << "Invalid choice. Please try again." << std::endl;
    }
  } while (choice != 0);

  return 0;
}