#ifndef OTP_EMAIL_SENDER_H
#define OTP_EMAIL_SENDER_H

#include <iostream>
#include <string>
#include <ctime>
#include <random>
#include <curl/curl.h>

class OTPEmailSender
{
private:
    std::string m_smtpServer;
    std::string m_senderEmail;
    std::string m_senderPassword;
    int m_otpLength;
    int m_otpExpiryTime; // thoi gian het han tinh bang giay

public:
    OTPEmailSender(
        const std::string &smtpServer = "smtp.gmail.com:587",
        const std::string &senderEmail = "hieunm.hrt@gmail.com",
        const std::string &senderPassword = "fsyl ymhq iswj hhwe",
        int otpLength = 6,
        int otpExpiryTime = 300) // 5 phut
        : m_smtpServer(smtpServer),
          m_senderEmail(senderEmail),
          m_senderPassword(senderPassword),
          m_otpLength(otpLength),
          m_otpExpiryTime(otpExpiryTime)
    {
    }

    // Tao ma OTP ngau nhien
    std::string generateOTP()
    {
        const char charset[] = "0123456789";
        std::string otp;

        // Su dung random engine an toan hon
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution(0, sizeof(charset) - 2);

        for (int i = 0; i < m_otpLength; ++i)
        {
            otp += charset[distribution(generator)];
        }

        return otp;
    }

    // Callback function cho CURL de xu ly phan hoi
    static size_t readCallback(void *ptr, size_t size, size_t nmemb, void *userdata)
    {
        std::string *readBuffer = static_cast<std::string *>(userdata);

        if (readBuffer->size() > 0)
        {
            size_t copyLength = std::min(readBuffer->size(), size * nmemb);
            memcpy(ptr, readBuffer->c_str(), copyLength);
            readBuffer->erase(0, copyLength);
            return copyLength;
        }

        return 0;
    }

    // Ham gui OTP qua email
    bool sendOTP(const std::string &recipientEmail, std::string &otp)
    {
        // Tao ma OTP
        otp = generateOTP();

        // Tao noi dung email
        std::string emailBody = "Subject: Your OTP Code\r\n"
                                "To: " +
                                recipientEmail + "\r\n"
                                                 "From: " +
                                m_senderEmail + "\r\n"
                                                "Content-Type: text/plain; charset=utf-8\r\n"
                                                "\r\n"
                                                "Ma OTP cua ban la: " +
                                otp + "\r\n"
                                      "Ma nay se het han sau " +
                                std::to_string(m_otpExpiryTime / 60) + " phut.\r\n";

        // Khoi tao CURL
        CURL *curl = curl_easy_init();
        if (!curl)
        {
            std::cerr << "Khong the khoi tao curl!" << std::endl;
            return false;
        }

        CURLcode res = CURLE_OK;

        // Thiet lap cac tuy chon CURL
        curl_easy_setopt(curl, CURLOPT_URL, m_smtpServer.c_str());
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_USERNAME, m_senderEmail.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, m_senderPassword.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, m_senderEmail.c_str());
        curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\cacert.pem");

        // Thiet lap nguoi nhan
        struct curl_slist *recipients = NULL;
        recipients = curl_slist_append(recipients, recipientEmail.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        // Thiet lap ham callback va du lieu email
        std::string readBuffer = emailBody;
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, readCallback);
        curl_easy_setopt(curl, CURLOPT_READDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        // Tuy chon bo sung cho debug
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // Them cac tuy chon bao mat cho Gmail
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        // Them timeout de tranh treo chuong trinh
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);

        std::cout << "Dang gui email den " << recipientEmail << "..." << std::endl;

        // Thuc hien gui email
        res = curl_easy_perform(curl);

        // Kiem tra ket qua
        bool success = (res == CURLE_OK);
        if (!success)
        {
            std::cerr << "Loi khi gui email: " << curl_easy_strerror(res) << std::endl;
        }
        else
        {
            std::cout << "Gui email thanh cong!" << std::endl;
        }

        // Don dep
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);

        return success;
    }
};
#endif 
