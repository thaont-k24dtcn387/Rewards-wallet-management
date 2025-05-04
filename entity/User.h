#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <ctime>
#include <random>

// Lop User: Quan ly thong tin nguoi dung
class User
{
private:
    std::string userId;
    std::string username;
    std::string passwordHash; // Mat khau da duoc ma hoa
    std::string fullName;
    std::string email;
    std::string phone;
    bool isAdmin;
    bool isGenPassword;

public:
    User() : isAdmin(false) {}

    User(const std::string &id, const std::string &username, const std::string &passwordHash,
         const std::string &fullName, const std::string &email, const std::string &phone, bool isAdmin = false, bool isGenPassword = false)
        : userId(id), username(username), passwordHash(passwordHash),
          fullName(fullName), email(email), phone(phone), isAdmin(isAdmin), isGenPassword(isGenPassword) {}

    // Getters
    std::string getUserId() const { return userId; }
    std::string getUsername() const { return username; }
    std::string getPasswordHash() const { return passwordHash; }
    std::string getFullName() const { return fullName; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    bool getIsAdmin() const { return isAdmin; }
    bool getIsGenPassword() const { return isGenPassword; }

    // Setters
    void setUserId(const std::string &id) { userId = id; }
    void setUsername(const std::string &uname) { username = uname; }
    void setPasswordHash(const std::string &pwd) { passwordHash = pwd; }
    void setFullName(const std::string &name) { fullName = name; }
    void setEmail(const std::string &mail) { email = mail; }
    void setPhone(const std::string &ph) { phone = ph; }
    void setIsAdmin(bool admin) { isAdmin = admin; }
    void setIsGenPassword(bool genPassword) { isGenPassword = genPassword; }

    // Chuyen doi thanh chuoi de luu vao file
    std::string toString() const
    {
        return userId + "," + username + "," + passwordHash + "," +
               fullName + "," + email + "," + phone + "," + (isAdmin ? "1" : "0") + "," + (isGenPassword ? "1" : "0");
    }

    // Tao doi tuong User tu chuoi
    static User fromString(const std::string &str)
    {
        User user;
        size_t pos = 0;
        std::string token;
        std::string s = str;
        int i = 0;

        while ((pos = s.find(",")) != std::string::npos)
        {
            token = s.substr(0, pos);
            s.erase(0, pos + 1);

            switch (i)
            {
            case 0:
                user.setUserId(token);
                break;
            case 1:
                user.setUsername(token);
                break;
            case 2:
                user.setPasswordHash(token);
                break;
            case 3:
                user.setFullName(token);
                break;
            case 4:
                user.setEmail(token);
                break;
            case 5:
                user.setPhone(token);
                break;
            case 6:
                user.setIsAdmin(token == "1");
                break;
            }
            i++;
        }
        user.setIsGenPassword(s == "1");
        return user;
    }
};

#endif 