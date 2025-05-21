#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include <string>
#include <ctime>
#include <random>
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

    std::string getTransactionId() const { return transactionId; }
    std::string getFromWalletId() const { return fromWalletId; }
    std::string getToWalletId() const { return toWalletId; }
    double getAmount() const { return amount; }
    std::string getTimestamp() const { return timestamp; }
    std::string getStatus() const { return status; }
    std::string getDescription() const { return description; }

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

#endif // TRANSACTION_H