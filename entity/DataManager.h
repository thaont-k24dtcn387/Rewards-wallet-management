#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <functional>
#include <random>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <curl/curl.h>

#include "Wallet.h"
#include "Transaction.h"
#include "User.h"

class DataManager
{
private:
    std::string usersFile = "database/users.dat";
    std::string walletsFile = "database/wallets.dat";
    std::string transactionsFile = "database/transactions.dat";
    std::string backupDir = "backup/";

    std::vector<Wallet> wallets;
    std::vector<Transaction> transactions;

    // Tao ID ngau nhien duy nhat
    std::string generateUniqueId()
    {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_c), "%Y%m%d%H%M%S");

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1000, 9999);

        return ss.str() + std::to_string(dis(gen));
    }

    // Lay thoi gian hien tai duoi dang chuoi
    std::string getCurrentTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

public:
    DataManager()
    {
        loadData();
    }
    std::vector<User> users;

    std::string hashPassword(const std::string &password)
    {
        std::hash<std::string> hasher;
        size_t hash = hasher(password);
        return std::to_string(hash);
    }

    // Doc du lieu tu file
    void loadData()
    {
        loadUsers();
        loadWallets();
        loadTransactions();
    }

    // Doc du lieu nguoi dung tu file
    void loadUsers()
    {
        users.clear();
        std::ifstream file(usersFile);
        std::string line;

        if (file.is_open())
        {
            while (getline(file, line))
            {
                if (!line.empty())
                {
                    users.push_back(User::fromString(line));
                }
            }
            file.close();
        }
    }

    // Doc du lieu vi tu file
    void loadWallets()
    {
        wallets.clear();
        std::ifstream file(walletsFile);
        std::string line;

        if (file.is_open())
        {
            while (getline(file, line))
            {
                if (!line.empty())
                {
                    wallets.push_back(Wallet::fromString(line));
                }
            }
            file.close();
        }
    }

    // Doc du lieu giao dich tu file
    void loadTransactions()
    {
        transactions.clear();
        std::ifstream file(transactionsFile);
        std::string line;

        if (file.is_open())
        {
            while (getline(file, line))
            {
                if (!line.empty())
                {
                    transactions.push_back(Transaction::fromString(line));
                }
            }
            file.close();
        }
    }

    // Luu du lieu vao file
    void saveData()
    {
        saveUsers();
        saveWallets();
        saveTransactions();
        backupData(); // Sao luu sau moi lan luu
    }

    // Luu du lieu nguoi dung vao file
    void saveUsers()
    {
        std::ofstream file(usersFile);
        if (file.is_open())
        {
            for (const auto &user : users)
            {
                file << user.toString() << std::endl;
            }
            file.close();
        }
    }

    void saveWallets()
    {
        std::ofstream file(walletsFile);
        if (file.is_open())
        {
            for (const auto &wallet : wallets)
            {
                file << wallet.toString() << std::endl;
            }
            file.close();
        }
    }

    void saveTransactions()
    {
        std::ofstream file(transactionsFile);
        if (file.is_open())
        {
            for (const auto &transaction : transactions)
            {
                file << transaction.toString() << std::endl;
            }
            file.close();
        }
    }

    void backupData()
    {
        std::string timestamp = getCurrentTimestamp();
        std::replace(timestamp.begin(), timestamp.end(), ' ', '_');
        std::replace(timestamp.begin(), timestamp.end(), ':', '-');

        std::string backupUserFile = backupDir + "users_" + timestamp + ".bak";
        std::string backupWalletFile = backupDir + "wallets_" + timestamp + ".bak";
        std::string backupTransactionFile = backupDir + "transactions_" + timestamp + ".bak";

        // Tao thu muc backup neu chua ton tai
        system(("mkdir -p " + backupDir).c_str());

        // Sao chep cac file du lieu vao thu muc backup
        std::ifstream srcUsers(usersFile, std::ios::binary);
        std::ofstream dstUsers(backupUserFile, std::ios::binary);
        dstUsers << srcUsers.rdbuf();

        std::ifstream srcWallets(walletsFile, std::ios::binary);
        std::ofstream dstWallets(backupWalletFile, std::ios::binary);
        dstWallets << srcWallets.rdbuf();

        std::ifstream srcTransactions(transactionsFile, std::ios::binary);
        std::ofstream dstTransactions(backupTransactionFile, std::ios::binary);
        dstTransactions << srcTransactions.rdbuf();
    }

    bool restoreData(const std::string &timestamp)
    {
        std::string backupUserFile = backupDir + "users_" + timestamp + ".bak";
        std::string backupWalletFile = backupDir + "wallets_" + timestamp + ".bak";
        std::string backupTransactionFile = backupDir + "transactions_" + timestamp + ".bak";

        // Kiem tra xem cac file backup co ton tai khong
        std::ifstream testUsers(backupUserFile);
        std::ifstream testWallets(backupWalletFile);
        std::ifstream testTransactions(backupTransactionFile);

        if (!testUsers || !testWallets || !testTransactions)
        {
            return false; // Khong tim thay file backup
        }

        // Sao chep tu backup vao file chinh
        std::ifstream srcUsers(backupUserFile, std::ios::binary);
        std::ofstream dstUsers(usersFile, std::ios::binary);
        dstUsers << srcUsers.rdbuf();

        std::ifstream srcWallets(backupWalletFile, std::ios::binary);
        std::ofstream dstWallets(walletsFile, std::ios::binary);
        dstWallets << srcWallets.rdbuf();

        std::ifstream srcTransactions(backupTransactionFile, std::ios::binary);
        std::ofstream dstTransactions(transactionsFile, std::ios::binary);
        dstTransactions << srcTransactions.rdbuf();

        loadData();

        return true;
    }

    User createUser(const std::string &username, const std::string &password,
                    const std::string &fullName, const std::string &email,
                    const std::string &phone, bool isAdmin = false, bool isGenPassword = false)
    {
        for (const auto &user : users)
        {
            if (user.getUsername() == username)
            {
                throw std::runtime_error("Ten nguoi dung da ton tai.");
            }
        }

        std::string userId = generateUniqueId();
        std::string passwordHash = hashPassword(password);

        User newUser(userId, username, passwordHash, fullName, email, phone, isAdmin, isGenPassword);
        users.push_back(newUser);

        createWallet(userId);

        saveData();
        return newUser;
    }

    Wallet createWallet(const std::string &userId)
    {
        std::string walletId = "W" + generateUniqueId();
        Wallet newWallet(walletId, userId);
        wallets.push_back(newWallet);

        saveData();
        return newWallet;
    }

    Transaction createTransaction(const std::string &fromWalletId, const std::string &toWalletId,
                                  double amount, const std::string &description)
    {
        std::string transactionId = "T" + generateUniqueId();
        std::string timestamp = getCurrentTimestamp();

        Transaction newTransaction(transactionId, fromWalletId, toWalletId,
                                   amount, timestamp, "pending", description);
        transactions.push_back(newTransaction);

        saveData();
        return newTransaction;
    }

    bool executeTransaction(const std::string &transactionId, const std::string &otp)
    {
        auto transactionIt = std::find_if(transactions.begin(), transactions.end(),
                                          [&transactionId](const Transaction &t)
                                          { return t.getTransactionId() == transactionId; });

        if (transactionIt == transactions.end() || transactionIt->getStatus() != "pending")
        {
            return false; // Khong tim thay giao dich hoac giao dich khong o trang thai pending
        }
        // Tim vi nguon
        auto fromWalletIt = std::find_if(wallets.begin(), wallets.end(),
                                         [&](const Wallet &w)
                                         { return w.getWalletId() == transactionIt->getFromWalletId(); });

        // Tim vi dich
        auto toWalletIt = std::find_if(wallets.begin(), wallets.end(),
                                       [&](const Wallet &w)
                                       { return w.getWalletId() == transactionIt->getToWalletId(); });

        if (fromWalletIt == wallets.end() || toWalletIt == wallets.end())
        {
            transactionIt->setStatus("failed");
            saveData();
            return false; // Khong tim thay vi nguon hoac vi dich
        }

        double amount = transactionIt->getAmount();

        // Kiem tra so du
        if (fromWalletIt->getBalance() < amount)
        {
            transactionIt->setStatus("failed");
            saveData();
            return false; // So du khong du
        }

        // Thuc hien giao dich (atomic)
        try
        {
            fromWalletIt->subtractPoints(amount);
            toWalletIt->addPoints(amount);
            transactionIt->setStatus("success");
            saveData();
            return true;
        }
        catch (const std::exception &e)
        {
            // Neu co loi, huy giao dich
            transactionIt->setStatus("failed");
            saveData();
            return false;
        }
    }

    void moneyTransactionToWallet(const std::string &walletId, double amount)
    {
        auto walletIt = std::find_if(wallets.begin(), wallets.end(),
                                      [&walletId](const Wallet &w)
                                      { return w.getWalletId() == walletId; });

        if (walletIt != wallets.end())
        {
            walletIt->addPoints(amount);
            saveData();
            std::cout << "Da chuyen " << amount << " diem vao vi " << walletId << std::endl;
        }
    }

    // Tao OTP (gia lap)
    std::string generateOTP()
    {
        return "123456"; // OTP mau
    }

    // Sinh mat khau tu dong
    std::string generateRandomPassword()
    {
        const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, chars.size() - 1);

        std::string password;
        for (int i = 0; i < 10; ++i)
        { // Mat khau 10 ky tu
            password += chars[dis(gen)];
        }
        return password;
    }

    User *authenticate(const std::string &username, const std::string &password)
    {
        std::string passwordHash = hashPassword(password);

        for (auto &user : users)
        {
            if (user.getUsername() == username && user.getPasswordHash() == passwordHash)
            {
                return &user;
            }
        }
        return nullptr;
    }

    Wallet *getWalletByUserId(const std::string &userId)
    {
        for (auto &wallet : wallets)
        {
            if (wallet.getUserId() == userId)
            {
                return &wallet;
            }
        }
        return nullptr;
    }

    Wallet *getWalletById(const std::string &walletId)
    {
        for (auto &wallet : wallets)
        {
            if (wallet.getWalletId() == walletId)
            {
                return &wallet;
            }
        }
        return nullptr;
    }

    std::vector<Transaction> getTransactionsByWalletId(const std::string &walletId)
    {
        std::vector<Transaction> result;

        for (const auto &transaction : transactions)
        {
            if (transaction.getFromWalletId() == walletId || transaction.getToWalletId() == walletId)
            {
                result.push_back(transaction);
            }
        }

        return result;
    }

    User *getUserByUsername(const std::string &username)
    {
        for (auto &user : users)
        {
            if (user.getUsername() == username)
            {
                return &user;
            }
        }
        return nullptr;
    }

    User *getUserById(const std::string &userId)
    {
        for (auto &user : users)
        {
            if (user.getUserId() == userId)
            {
                return &user;
            }
        }
        return nullptr;
    }
};


#endif