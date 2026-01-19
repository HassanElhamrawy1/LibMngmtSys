#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    User(const std::string& username = "", const std::string& password = "")
        : username(username), password(password) {}

    std::string getUsername() const { return username; }
    bool checkPassword(const std::string& pass) const { return pass == password; }

protected:
    std::string username;
    std::string password;
};

#endif
