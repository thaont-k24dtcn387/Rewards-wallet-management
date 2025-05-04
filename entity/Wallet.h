
#ifndef WALLET_H
#define WALLET_H
#include <iostream>
#include <string>
#include <vector>


// Lop Wallet: Quan ly vi diem thuong
class Wallet
{
private:
    std::string walletId;
    std::string userId;
    double balance;

public:
    Wallet() : balance(0.0) {}

    Wallet(const std::string &wid, const std::string &uid, double bal = 0.0)
        : walletId(wid), userId(uid), balance(bal) {}

    // Getters
    std::string getWalletId() const { return walletId; }
    std::string getUserId() const { return userId; }
    double getBalance() const { return balance; }

    // Setters
    void setWalletId(const std::string &id) { walletId = id; }
    void setUserId(const std::string &id) { userId = id; }
    void setBalance(double bal) { balance = bal; }

    // Them diem vao vi
    bool addPoints(double points)
    {
        balance += points;
        return true;
    }

    // Tru diem tu vi
    bool subtractPoints(double points)
    {
        if (balance >= points)
        {
            balance -= points;
            return true;
        }
        return false;
    }

    // Chuyen doi thanh chuoi de luu vao file
    std::string toString() const
    {
        return walletId + "," + userId + "," + std::to_string(balance);
    }

    // Tao doi tuong Wallet tu chuoi
    static Wallet fromString(const std::string &str)
    {
        Wallet wallet;
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
                wallet.setWalletId(token);
                break;
            case 1:
                wallet.setUserId(token);
                break;
            }
            i++;
        }
        wallet.setBalance(std::stod(s));
        return wallet;
    }
};
#endif // WALLET_H