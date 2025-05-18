#ifndef SYSTEM_H
#define SYSTEM_H
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

#include "DataManager.h"
#include "User.h"
#include "Wallet.h"
#include "Transaction.h"
#include "OTPEmailSender.h"

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
            std::cout << "6. Thay doi thong tin ca nhan\n";
            std::cout << "7. Dang xuat\n";
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
                 changeAccountInfo();   
                break;
            case 7:
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
            std::cout << "5. Chuyen tien den vi\n";
            std::cout << "6. Dang xuat\n";
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
                transfermoney();
                break;
            case 6:
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

        std::cout << std::left << std::setw(25) << "ID GD"
                  << std::setw(25) << "TU VI"
                  << std::setw(25) << "DEN VI"
                  << std::setw(10) << "SO DIEM"
                  << std::setw(25) << "THOI GIAN"
                  << std::setw(12) << "TRANG THAI "
                  << "MO TA\n";

        for (const auto &transaction : transactions)
        {
            std::cout << std::left << std::setw(25) << transaction.getTransactionId() + " "
                      << std::setw(25) << transaction.getFromWalletId() + " "
                      << std::setw(25) << transaction.getToWalletId() + " "
                      << std::setw(10) << transaction.getAmount()
                      << std::setw(25) << transaction.getTimestamp()
                      << std::setw(12) << transaction.getStatus()
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
        std::cout << "Nhap ma OTP nhan: ";
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
    // mai check lai phan nay xem co dung khong?
    void changeAccountInfo()
    {
        std::cin.ignore(); // Xoa bo dem
        std::string fullName, email, phone;

        std::cout << "\n===== THAY DOI THONG TIN CA NHAN =====\n";
        std::cout << "Nhap ho ten moi: ";
        std::getline(std::cin, fullName);

        std::cout << "Nhap email moi: ";
        std::getline(std::cin, email);

        std::cout << "Nhap so dien thoai moi: ";
        std::getline(std::cin, phone);

        currentUser->setFullName(fullName);
        currentUser->setEmail(email);
        currentUser->setPhone(phone);

        dataManager.saveData();

        std::cout << "Thay doi thong tin ca nhan thanh cong.\n";
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
        // gui OTP den email
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
        std::cout << "Nhap email nguoi nhan OTP de thay doi mat khau: ";
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
        std::cout << "Nhap ma OTP nhan: ";
        std::cin >> recipientOTP;
        if (recipientOTP != otp)
        {
            std::cout << "Ma OTP khong dung. Vui long thu lai.\n";
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
        std::cout << std::left << std::setw(20) << "ID"
                  << std::setw(20) << "TEN DANG NHAP"
                  << std::setw(30) << "HO TEN"
                  << std::setw(30) << "EMAIL"
                  << std::setw(15) << "SO DIEN THOAI"
                  << "VAI TRO\n";

        for (const auto &user : users)
        {
            std::cout << std::left << std::setw(20) << user.getUserId() + " "
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
    void transfermoney()
    {
        std::cout << "Nhap ID vi nhan: ";
        std::string receiveWalletId;
        std::cin >> receiveWalletId;
        std::cout << "Nhap so diem can chuyen: ";
        double amount;
        std::cin >> amount;
        dataManager.moneyTransactionToWallet(receiveWalletId, amount);
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
                    std::cout << "Lua chon cua ban khong hop le.\n";
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
#endif