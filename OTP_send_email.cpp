#include <iostream>
#include <string>
#include <ctime>
#include <random>
#include <curl/curl.h>
#include "OTPEmailSender.h"

int main() {
    std::cout << "Chuong trinh gui OTP qua Gmail" << std::endl;
    std::cout << "------------------------------" << std::endl;
    
    // Khoi tao thu vien curl
    CURLcode curlInitResult = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (curlInitResult != CURLE_OK) {
        std::cerr << "Khong the khoi tao thu vien curl: " << curl_easy_strerror(curlInitResult) << std::endl;
        std::cout << "Nhan Enter de thoat...";
        std::cin.get();
        return 1;
    }
    
    std::string senderEmail, appPassword;
    
    // Nhap thong tin dang nhap Gmail
    std::cout << "Nhap email Gmail cua ban: ";
    std::getline(std::cin, senderEmail);
    
    std::cout << "Nhap mat khau ung dung (App Password) cua Gmail: ";
    std::getline(std::cin, appPassword);
    
    // Tao doi tuong email sender
    OTPEmailSender emailSender(
        "smtp.gmail.com:587",
        senderEmail,
        appPassword,
        6,                          // Do dai OTP
        300                         // Thoi gian het han (giay)
    );
    
    // Email nguoi nhan
    std::string recipientEmail;
    std::cout << "Nhap email nguoi nhan: ";
    std::getline(std::cin, recipientEmail);
    
    // Gui OTP
    std::string otp;
    if (emailSender.sendOTP(recipientEmail, otp)) {
        std::cout << "Da gui ma OTP " << otp << " thanh cong den " << recipientEmail << std::endl;
    } else {
        std::cout << "Khong the gui ma OTP!" << std::endl;
    }
    
    // Don dep thu vien curl
    curl_global_cleanup();
    
    // Cho nguoi dung nhan phim truoc khi thoat
    std::cout << "\nNhan Enter de thoat...";
    std::cin.get();
    
    return 0;
}