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
#include "OTPEmailSender.h"

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

// Lop Transaction: Quan ly giao dich
class Transaction
{
private:
    std::string transactionId;
    std::string fromWalletId;
    std::string toWalletId;
    double amount;
    std::string timestamp;
    std::string status; // "success", "failed", "pending"
    std::string description;

public:
    Transaction() : amount(0.0), status("pending") {}

    Transaction(const std::string &tid, const std::string &from, const std::string &to,
                double amt, const std::string &time, const std::string &stat, const std::string &desc)
        : transactionId(tid), fromWalletId(from), toWalletId(to),
          amount(amt), timestamp(time), status(stat), description(desc) {}

    // Getters
    std::string getTransactionId() const { return transactionId; }
    std::string getFromWalletId() const { return fromWalletId; }
    std::string getToWalletId() const { return toWalletId; }
    double getAmount() const { return amount; }
    std::string getTimestamp() const { return timestamp; }
    std::string getStatus() const { return status; }
    std::string getDescription() const { return description; }

    // Setters
    void setTransactionId(const std::string &id) { transactionId = id; }
    void setFromWalletId(const std::string &id) { fromWalletId = id; }
    void setToWalletId(const std::string &id) { toWalletId = id; }
    void setAmount(double amt) { amount = amt; }
    void setTimestamp(const std::string &time) { timestamp = time; }
    void setStatus(const std::string &stat) { status = stat; }
    void setDescription(const std::string &desc) { description = desc; }

    // Chuyen doi thanh chuoi de luu vao file
    std::string toString() const
    {
        return transactionId + "," + fromWalletId + "," + toWalletId + "," +
               std::to_string(amount) + "," + timestamp + "," + status + "," + description;
    }

    // Tao doi tuong Transaction tu chuoi
    static Transaction fromString(const std::string &str)
    {
        Transaction transaction;
        size_t pos = 0;
        std::string token;
        std::string s = str;
        int i = 0;

        while ((pos = s.find(",")) != std::string::npos && i < 6)
        {
            token = s.substr(0, pos);
            s.erase(0, pos + 1);

            switch (i)
            {
            case 0:
                transaction.setTransactionId(token);
                break;
            case 1:
                transaction.setFromWalletId(token);
                break;
            case 2:
                transaction.setToWalletId(token);
                break;
            case 3:
                transaction.setAmount(std::stod(token));
                break;
            case 4:
                transaction.setTimestamp(token);
                break;
            case 5:
                transaction.setStatus(token);
                break;
            }
            i++;
        }
        transaction.setDescription(s); // Phan con lai la description
        return transaction;
    }
};

// Lop quan ly du lieu
class DataManager
{
private:
    std::string usersFile = "users.dat";
    std::string walletsFile = "wallets.dat";
    std::string transactionsFile = "transactions.dat";
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

    // Luu du lieu vi vao file
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

    // Luu du lieu giao dich vao file
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

    // Sao luu du lieu
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

    // Khoi phuc du lieu tu ban sao luu
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

        // Tai lai du lieu tu file
        loadData();

        return true;
    }

    // Tao va them nguoi dung moi
    User createUser(const std::string &username, const std::string &password,
                    const std::string &fullName, const std::string &email,
                    const std::string &phone, bool isAdmin = false, bool isGenPassword = false)
    {
        // Kiem tra xem username da ton tai chua
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

        // Tao vi cho nguoi dung moi
        createWallet(userId);

        saveData();
        return newUser;
    }

    // Tao va them vi moi
    Wallet createWallet(const std::string &userId)
    {
        std::string walletId = "W" + generateUniqueId();
        Wallet newWallet(walletId, userId);
        wallets.push_back(newWallet);

        saveData();
        return newWallet;
    }

    // Tao va them giao dich moi
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

    // Thuc hien giao dich chuyen diem
    bool executeTransaction(const std::string &transactionId, const std::string &otp)
    {
        // Tim giao dich theo ID
        auto transactionIt = std::find_if(transactions.begin(), transactions.end(),
                                          [&transactionId](const Transaction &t)
                                          { return t.getTransactionId() == transactionId; });

        if (transactionIt == transactions.end() || transactionIt->getStatus() != "pending")
        {
            return false; // Khong tim thay giao dich hoac giao dich khong o trang thai pending
        }

        // // Xac thuc OTP (day la vi du don gian, trong thuc te can mot he thong OTP an toan hon)
        // if (otp != "123456")
        // { // OTP mau
        //     transactionIt->setStatus("failed");
        //     saveData();
        //     return false;
        // }

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

    // Tao OTP (gia lap)
    std::string generateOTP()
    {
        // Trong thuc te, OTP se duoc gui qua SMS hoac email
        // Day chi la vi du don gian
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

    // Xac thuc dang nhap
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

    // Lay vi cua nguoi dung
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

    // Lay vi theo ID
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

    // Lay danh sach giao dich cua vi
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

    // Lay nguoi dung theo username
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

    // Lay nguoi dung theo ID
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

class System
{
private:
    DataManager dataManager;
    User *currentUser;

    // Hien thi menu cho nguoi dung
    void showUserMenu()
    {
        while (true)
        {
            std::cout << "\n===== MENU NGUOI DUNG =====\n";
            std::cout << "1. Xem thong tin ca nhan\n";
            std::cout << "2. Xem so du vi\n";
            std::cout << "3. Xem lich su giao dich\n";
            std::cout << "4. Chuyen diem\n";
            std::cout << "5. Doi mat khau\n";
            std::cout << "6. Dang xuat\n";
            std::cout << "Lua chon cua ban: ";

            int choice;
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                showUserInfo();
                break;
            case 2:
                showWalletBalance();
                break;
            case 3:
                showTransactionHistory();
                break;
            case 4:
                transferPoints();
                break;
            case 5:
                changePassword();
                break;
            case 6:
                logout();
                return;
            default:
                std::cout << "Lua chon khong hop le.\n";
            }
        }
    }

    // Hien thi menu cho admin
    void showAdminMenu()
    {
        while (true)
        {
            std::cout << "\n===== MENU QUAN TRI VIEN =====\n";
            std::cout << "1. Tao tai khoan nguoi dung moi\n";
            std::cout << "2. Xem danh sach nguoi dung\n";
            std::cout << "3. Sao luu du lieu\n";
            std::cout << "4. Khoi phuc du lieu\n";
            std::cout << "5. Dang xuat\n";
            std::cout << "Lua chon cua ban: ";

            int choice;
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                createNewUser();
                break;
            case 2:
                showAllUsers();
                break;
            case 3:
                backupData();
                break;
            case 4:
                restoreData();
                break;
            case 5:
                logout();
                return;
            default:
                std::cout << "Lua chon khong hop le.\n";
            }
        }
    }

    // Hien thi thong tin nguoi dung
    void showUserInfo()
    {
        std::cout << "\n===== THONG TIN CA NHAN =====\n";
        std::cout << "ID: " << currentUser->getUserId() << std::endl;
        std::cout << "Ten dang nhap: " << currentUser->getUsername() << std::endl;
        std::cout << "Ho ten: " << currentUser->getFullName() << std::endl;
        std::cout << "Email: " << currentUser->getEmail() << std::endl;
        std::cout << "So dien thoai: " << currentUser->getPhone() << std::endl;
        std::cout << "Loai tai khoan: " << (currentUser->getIsAdmin() ? "Quan tri vien" : "Nguoi dung") << std::endl;
    }

    // Hien thi so du vi
    void showWalletBalance()
    {
        Wallet *wallet = dataManager.getWalletByUserId(currentUser->getUserId());

        if (wallet)
        {
            std::cout << "\n===== SO DU VI =====\n";
            std::cout << "ID vi: " << wallet->getWalletId() << std::endl;
            std::cout << "So du: " << wallet->getBalance() << " diem" << std::endl;
        }
        else
        {
            std::cout << "Khong tim thay thong tin vi.\n";
        }
    }

    // Hien thi lich su giao dich
    void showTransactionHistory()
    {
        Wallet *wallet = dataManager.getWalletByUserId(currentUser->getUserId());

        if (!wallet)
        {
            std::cout << "Khong tim thay thong tin vi.\n";
            return;
        }

        std::vector<Transaction> transactions = dataManager.getTransactionsByWalletId(wallet->getWalletId());

        std::cout << "\n===== LICH SU GIAO DICH =====\n";

        if (transactions.empty())
        {
            std::cout << "Chua co giao dich nao.\n";
            return;
        }

        std::cout << std::left << std::setw(12) << "ID GD"
                  << std::setw(12) << "TU VI"
                  << std::setw(12) << "DEN VI"
                  << std::setw(10) << "SO DIEM"
                  << std::setw(20) << "THOI GIAN"
                  << std::setw(10) << "TRANG THAI"
                  << "MO TA\n";

        for (const auto &transaction : transactions)
        {
            std::cout << std::left << std::setw(12) << transaction.getTransactionId()
                      << std::setw(12) << transaction.getFromWalletId()
                      << std::setw(12) << transaction.getToWalletId()
                      << std::setw(10) << transaction.getAmount()
                      << std::setw(20) << transaction.getTimestamp()
                      << std::setw(10) << transaction.getStatus()
                      << transaction.getDescription() << std::endl;
        }
    }

    // Chuyen diem tu vi cua nguoi dung hien tai sang vi khac
    void transferPoints()
    {
        std::cin.ignore(); // Xoa bo dem

        Wallet *fromWallet = dataManager.getWalletByUserId(currentUser->getUserId());

        if (!fromWallet)
        {
            std::cout << "Khong tim thay thong tin vi.\n";
            return;
        }

        std::string toWalletId;
        double amount;
        std::string description;

        std::cout << "\n===== CHUYEN DIEM =====\n";
        std::cout << "Nhap ID vi nhan: ";
        std::getline(std::cin, toWalletId);

        // Kiem tra vi dich
        Wallet *toWallet = dataManager.getWalletById(toWalletId);
        if (!toWallet)
        {
            std::cout << "Khong tim thay vi dich.\n";
            return;
        }

        // Khong cho phep chuyen diem cho chinh minh
        if (fromWallet->getWalletId() == toWalletId)
        {
            std::cout << "Khong the chuyen diem cho chinh minh.\n";
            return;
        }

        std::cout << "Nhap so diem can chuyen: ";
        std::cin >> amount;

        // Kiem tra so diem can chuyen
        if (amount <= 0)
        {
            std::cout << "So diem phai lon hon 0.\n";
            return;
        }

        if (fromWallet->getBalance() < amount)
        {
            std::cout << "So du khong du.\n";
            return;
        }

        std::cin.ignore(); // Xoa bo dem
        std::cout << "Nhap mo ta giao dich: ";
        std::getline(std::cin, description);

        // Tao giao dich
        Transaction transaction = dataManager.createTransaction(fromWallet->getWalletId(), toWalletId, amount, description);

        CURLcode curlInitResult = curl_global_init(CURL_GLOBAL_DEFAULT);
        if (curlInitResult != CURLE_OK)
        {
            std::cerr << "Khong the khoi tao thu vien curl: " << curl_easy_strerror(curlInitResult) << std::endl;
            std::cout << "Nhan Enter de thoat...";
            std::cin.get();
            return;
        }

        // Tạo đối tượng email sender
        OTPEmailSender emailSender(
            "smtp.gmail.com:587",
            "hieunm.hrt@gmail.com", // Thay bằng email của bạn
            "fsyl ymhq iswj hhwe",  // Thay bằng mật khẩu ứng dụng (không phải mật khẩu Gmail)
            6,                      // Độ dài OTP
            300                     // Thời gian hết hạn (giây)
        );

        // Email người nhận
        std::string recipientEmail;
        std::cout << "Nhap email nguoi nhan: ";
        std::cin >> recipientEmail;

        // Gửi OTP
        std::string otp;
        if (emailSender.sendOTP(recipientEmail, otp))
        {
            std::cout << "Da gui ma OTP thanh cong den " << recipientEmail << std::endl;
        }
        else
        {
            std::cout << "Khong the gui ma OTP!" << std::endl;
        }

        // Dọn dẹp thư viện curl
        curl_global_cleanup();
        std::string recipientOTP;
        std::cout << "Nhap OTPA nhan: ";
        std::cin >> recipientOTP;
        if (recipientOTP != otp)
        {
            std::cout << "Ma OTP khong dung. Vui long thu lai.\n";
            return;
        }
        // Thuc hien giao dich
        bool success = dataManager.executeTransaction(transaction.getTransactionId(), otp);

        if (success)
        {
            std::cout << "Chuyen diem thanh cong.\n";
        }
        else
        {
            std::cout << "Chuyen diem that bai. Vui long kiem tra lai OTP hoac thu lai sau.\n";
        }
    }

    // Doi mat khau
    void changePassword()
    {
        std::cin.ignore(); // Xoa bo dem

        std::string currentPassword, newPassword, confirmPassword;

        std::cout << "\n===== DOI MAT KHAU =====\n";
        std::cout << "Nhap mat khau hien tai: ";
        std::getline(std::cin, currentPassword);

        // Xac thuc mat khau hien tai
        if (dataManager.hashPassword(currentPassword) != currentUser->getPasswordHash())
        {
            std::cout << "Mat khau hien tai khong dung.\n";
            return;
        }

        std::cout << "Nhap mat khau moi: ";
        std::getline(std::cin, newPassword);

        std::cout << "Xac nhan mat khau moi: ";
        std::getline(std::cin, confirmPassword);

        if (newPassword != confirmPassword)
        {
            std::cout << "Mat khau xac nhan khong khop.\n";
            return;
        }

        // Cap nhat mat khau
        currentUser->setPasswordHash(dataManager.hashPassword(newPassword));
        dataManager.saveData();

        std::cout << "Doi mat khau thanh cong.\n";
    }

    // Dang xuat
    void logout()
    {
        currentUser = nullptr;
        std::cout << "Da dang xuat thanh cong.\n";
    }

    // Tao nguoi dung moi (chi danh cho admin)
    void createNewUser()
    {
        std::cin.ignore(); // Xoa bo dem

        std::string username, password, fullName, email, phone;
        bool isAdmin;

        std::cout << "\n===== TAO NGUOI DUNG MOI =====\n";
        std::cout << "Nhap ten dang nhap: ";
        std::getline(std::cin, username);

        // Kiem tra xem username da ton tai chua
        if (dataManager.getUserByUsername(username))
        {
            std::cout << "Ten dang nhap da ton tai.\n";
            return;
        }

        std::cout << "Tao mat khau tu dong? (1-Co, 0-Khong): ";
        int autoGen;
        std::cin >> autoGen;

        std::cin.ignore(); // Xoa bo dem

        if (autoGen == 1)
        {
            password = dataManager.generateRandomPassword();
            std::cout << "Mat khau duoc tao tu dong: " << password << std::endl;
        }
        else
        {
            std::cout << "Nhap mat khau: ";
            std::getline(std::cin, password);
        }

        std::cout << "Nhap ho ten: ";
        std::getline(std::cin, fullName);

        std::cout << "Nhap email: ";
        std::getline(std::cin, email);

        std::cout << "Nhap so dien thoai: ";
        std::getline(std::cin, phone);

        std::cout << "La quan tri vien? (1-Co, 0-Khong): ";
        std::cin >> isAdmin;

        try
        {
            User newUser = dataManager.createUser(username, password, fullName, email, phone, isAdmin, autoGen);
            std::cout << "Tao tai khoan thanh cong.\n";
            std::cout << "ID nguoi dung: " << newUser.getUserId() << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "Loi: " << e.what() << std::endl;
        }
    }

    // Hien thi danh sach nguoi dung (chi danh cho admin)
    void showAllUsers()
    {
        std::vector<User> &users = dataManager.users;

        std::cout << "\n===== DANH SACH NGUOI DUNG =====\n";
        std::cout << std::left << std::setw(12) << "ID"
                  << std::setw(20) << "TEN DANG NHAP"
                  << std::setw(30) << "HO TEN"
                  << std::setw(30) << "EMAIL"
                  << std::setw(15) << "SO DIEN THOAI"
                  << "LOAI\n";

        for (const auto &user : users)
        {
            std::cout << std::left << std::setw(12) << user.getUserId()
                      << std::setw(20) << user.getUsername()
                      << std::setw(30) << user.getFullName()
                      << std::setw(30) << user.getEmail()
                      << std::setw(15) << user.getPhone()
                      << (user.getIsAdmin() ? "Quan tri vien" : "Nguoi dung") << std::endl;
        }
    }

    // Sao luu du lieu
    void backupData()
    {
        dataManager.backupData();
        std::cout << "Da sao luu du lieu thanh cong.\n";
    }

    // Khoi phuc du lieu
    void restoreData()
    {
        std::cin.ignore(); // Xoa bo dem

        std::string timestamp;
        std::cout << "\n===== KHOI PHUC DU LIEU =====\n";
        std::cout << "Nhap thoi gian sao luu (dinh dang YYYY-MM-DD_HH-MM-SS): ";
        std::getline(std::cin, timestamp);

        bool success = dataManager.restoreData(timestamp);

        if (success)
        {
            std::cout << "Khoi phuc du lieu thanh cong.\n";
        }
        else
        {
            std::cout << "Khong tim thay ban sao luu tai thoi diem nay.\n";
        }
    }

public:
    System() : currentUser(nullptr)
    {
        // Kiem tra xem da co tai khoan admin mac dinh chua
        User *admin = dataManager.getUserByUsername("admin");
        if (!admin)
        {
            try
            {
                dataManager.createUser("admin", "admin123", "Administrator", "admin@example.com", "0123456789", true, false);
                std::cout << "Da tao tai khoan quan tri mac dinh.\n";
                std::cout << "Ten dang nhap: admin\n";
                std::cout << "Mat khau: admin123\n";
            }
            catch (const std::exception &e)
            {
                std::cout << "Loi khi tao tai khoan quan tri: " << e.what() << std::endl;
            }
        }
    }

    // Hien thi menu chinh
    void showMainMenu()
    {
        while (true)
        {
            if (currentUser)
            {
                if (currentUser->getIsAdmin())
                {
                    showAdminMenu();
                }
                else
                {
                    showUserMenu();
                }
            }
            else
            {
                std::cout << "\n===== HE THONG DANG NHAP VA VI DIEM THUONG =====\n";
                std::cout << "1. Dang nhap\n";
                std::cout << "2. Dang ky\n";
                std::cout << "3. Thoat\n";
                std::cout << "Lua chon cua ban: ";

                int choice;
                std::cin >> choice;

                switch (choice)
                {
                case 1:
                    login();
                    break;
                case 2:
                    registerUser();
                    break;
                case 3:
                    std::cout << "Cam on ban da su dung he thong. Tam biet!\n";
                    return;
                default:
                    std::cout << "Lua chon khong hop le.\n";
                }
            }
        }
    }

    // Dang nhap
    void login()
    {
        std::cin.ignore(); // Xoa bo dem

        std::string username, password;

        std::cout << "\n===== DANG NHAP =====\n";
        std::cout << "Ten dang nhap: ";
        std::getline(std::cin, username);

        std::cout << "Mat khau: ";
        std::getline(std::cin, password);

        currentUser = dataManager.authenticate(username, password);

        if (currentUser)
        {
            std::cout << "Dang nhap thanh cong. Xin chao, " << currentUser->getFullName() << "!\n";
        }
        else
        {
            std::cout << "Dang nhap that bai. Ten dang nhap hoac mat khau khong dung.\n";
        }
        std::cout << currentUser->getUsername() << std::endl;
        std::cout << currentUser->getIsAdmin() << std::endl;
        if (currentUser && currentUser->getIsGenPassword())
        {
            std::cout << "Mat khau da duoc tao tu dong. Vui long doi mat khau.\n";
            changePassword();
            currentUser->setIsGenPassword(false);
            dataManager.saveData();
        }
    }

    // Dang ky nguoi dung moi
    void registerUser()
    {
        std::cin.ignore(); // Xoa bo dem

        std::string username, password, confirmPassword, fullName, email, phone;

        std::cout << "\n===== DANG KY =====\n";
        std::cout << "Nhap ten dang nhap: ";
        std::getline(std::cin, username);

        // Kiem tra xem username da ton tai chua
        if (dataManager.getUserByUsername(username))
        {
            std::cout << "Ten dang nhap da ton tai.\n";
            return;
        }

        std::cout << "Nhap mat khau: ";
        std::getline(std::cin, password);

        std::cout << "Xac nhan mat khau: ";
        std::getline(std::cin, confirmPassword);

        if (password != confirmPassword)
        {
            std::cout << "Mat khau xac nhan khong khop.\n";
            return;
        }

        std::cout << "Nhap ho ten: ";
        std::getline(std::cin, fullName);

        std::cout << "Nhap email: ";
        std::getline(std::cin, email);

        std::cout << "Nhap so dien thoai: ";
        std::getline(std::cin, phone);

        try
        {
            User newUser = dataManager.createUser(username, password, fullName, email, phone);
            std::cout << "Dang ky thanh cong.\n";
            currentUser = dataManager.getUserByUsername(username);
        }
        catch (const std::exception &e)
        {
            std::cout << "Loi: " << e.what() << std::endl;
        }
    }
};

int main()
{
    System system;
    system.showMainMenu();
    return 0;
}