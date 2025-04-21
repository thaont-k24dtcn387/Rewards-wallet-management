#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include <random>

using namespace std;


// Cau truc giao dich
class Transaction
{
private:
    int customerId;
    int pointsChange;
    std::string description;
    time_t timestamp;

public:
    Transaction(int _customerId, int _pointsChange, std::string _description, time_t _timestamp)
        : customerId(_customerId), pointsChange(_pointsChange), description(_description), timestamp(_timestamp) {}

    int getCustomerId() const { return customerId; }
    int getPointsChange() const { return pointsChange; }
    std::string getDescription() const { return description; }
    time_t getTimestamp() const { return timestamp; }

    std::string getFormattedTime() const
    {
        struct tm *timeinfo;
        timeinfo = localtime(&timestamp);
        char buffer[80];
        strftime(buffer, 80, "%d-%m-%Y %H:%M:%S", timeinfo);
        return std::string(buffer);
    }
};


// Cau truc khach hang
class Customer
{
private:
    int id;
    std::string name;
    std::string phone;
    std::string email;
    double balance;
    int points;
    std::vector<Transaction> transactions;

public:
    Customer(int _id, std::string _name, std::string _phone, std::string _email)
        : id(_id), name(_name), phone(_phone), email(_email), points(0) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getPhone() const { return phone; }
    std::string getEmail() const { return email; }

    int getPoints() const { return points; }

    void addPoints(int value, std::string description)
    {
        points += value;
        transactions.push_back(Transaction(id, value, description, time(nullptr)));
    }

    void usePoints(int value, std::string description)
    {
        if (points >= value)
        {
            points -= value;
            transactions.push_back(Transaction(id, -value, description, time(nullptr)));
        }
        else
        {
            throw std::runtime_error("Khong du diem de thuc hien giao dich!");
        }
    }

    std::vector<Transaction> getTransactionHistory() const
    {
        return transactions;
    }
};


// Cau truc phan thuong
class Reward
{
private:
    int id;
    std::string name;
    std::string description;
    int pointsRequired;

public:
    Reward() : id(0), pointsRequired(0) {}

    Reward(int _id, std::string _name, std::string _description, int _pointsRequired)
        : id(_id), name(_name), description(_description), pointsRequired(_pointsRequired) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    int getPointsRequired() const { return pointsRequired; }
};

// Lop quan ly he thong
class PointsManager
{
private:
    std::vector<Customer> customers;
    std::map<int, Reward> rewards;
    int nextCustomerId;

     // Tạo OTP ngẫu nhiên
     int generateOTP()
     {
         std::random_device rd;
         std::mt19937 gen(rd());
         std::uniform_int_distribution<> dis(100000, 999999);
         return dis(gen);
     }

public:
    PointsManager() : nextCustomerId(1) {}

     // Chuyển điểm giữa các ví
     void transferPoints(int fromCustomerId, int toCustomerId, int points)
     {
         Customer *fromCustomer = findCustomer(fromCustomerId);
         Customer *toCustomer = findCustomer(toCustomerId);
 
         if (fromCustomer == nullptr)
         {
             throw std::runtime_error("Khong tim thay vi nguon!");
         }
         if (toCustomer == nullptr)
         {
             throw std::runtime_error("Khong tim thay vi dich!");
         }
 
         // Kiểm tra số dư
         if (fromCustomer->getPoints() < points)
         {
             throw std::runtime_error("So du khong du de thuc hien giao dich!");
         }
 
         // Tạo OTP và yêu cầu xác nhận
         int otp = generateOTP();
         std::cout << "OTP de xac nhan giao dich: " << otp << std::endl;
         int userOtp;
         std::cout << "Nhap OTP: ";
         std::cin >> userOtp;
 
         if (userOtp != otp)
         {
             throw std::runtime_error("OTP khong hop le!");
         }

         
        // Thực hiện giao dịch (atomic)
        try
        {
            fromCustomer->usePoints(points, "Chuyen diem den vi ID: " + std::to_string(toCustomerId));
            toCustomer->addPoints(points, "Nhan diem tu vi ID: " + std::to_string(fromCustomerId));
            std::cout << "Giao dich thanh cong!" << std::endl;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Giao dich that bai: " + std::string(e.what()));
        }
    }

    int registerCustomer(std::string name, std::string phone, std::string email)
    {
        // Kiem tra trung lap
        for (const auto &customer : customers)
        {
            if (customer.getPhone() == phone || customer.getEmail() == email)
            {
                throw std::runtime_error("So dien thoai hoac email da duoc dang ky!");
            }
        }

        Customer newCustomer(nextCustomerId, name, phone, email);
        customers.push_back(newCustomer);
        return nextCustomerId++;
    }

    Customer *findCustomer(int id)
    {
        for (auto &customer : customers)
        {
            if (customer.getId() == id)
            {
                return &customer;
            }
        }
        return nullptr;
    }

    Customer *findCustomerByPhone(std::string phone)
    {
        for (auto &customer : customers)
        {
            if (customer.getPhone() == phone)
            {
                return &customer;
            }
        }
        return nullptr;
    }

    void addPoints(int customerId, int points, std::string description)
    {
        Customer *customer = findCustomer(customerId);
        if (customer != nullptr)
        {
            customer->addPoints(points, description);
        }
        else
        {
            throw std::runtime_error("Khong tim thay khach hang!");
        }
    }

    void redeemReward(int customerId, int rewardId)
    {
        Customer *customer = findCustomer(customerId);
        if (customer == nullptr)
        {
            throw std::runtime_error("Khong tim thay khach hang!");
        }

        auto rewardIt = rewards.find(rewardId);
        if (rewardIt == rewards.end())
        {
            throw std::runtime_error("Khong tim thay phan thuong!");
        }

        Reward reward = rewardIt->second;
        if (customer->getPoints() < reward.getPointsRequired())
        {
            throw std::runtime_error("Khong du diem de doi phan thuong nay!");
        }

        customer->usePoints(reward.getPointsRequired(), "Doi thuong: " + reward.getName());
    }

    void addReward(int id, std::string name, std::string description, int pointsRequired)
    {
        rewards[id] = Reward(id, name, description, pointsRequired);
    }

    std::vector<Customer> getAllCustomers() const
    {
        return customers;
    }

    void saveToFile(std::string filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Khong the mo file de ghi!");
        }

        // Ghi du lieu khach hang
        file << customers.size() << std::endl;
        for (const auto &customer : customers)
        {
            file << customer.getId() << "|"
                 << customer.getName() << "|"
                 << customer.getPhone() << "|"
                 << customer.getEmail() << "|"
                 << customer.getPoints() << std::endl;

            // Ghi giao dich
            auto transactions = customer.getTransactionHistory();
            file << transactions.size() << std::endl;
            for (const auto &trans : transactions)
            {
                file << trans.getPointsChange() << "|"
                     << trans.getDescription() << "|"
                     << trans.getTimestamp() << std::endl;
            }
        }

        // Ghi du lieu phan thuong
        file << rewards.size() << std::endl;
        for (const auto &reward : rewards)
        {
            file << reward.second.getId() << "|"
                 << reward.second.getName() << "|"
                 << reward.second.getDescription() << "|"
                 << reward.second.getPointsRequired() << std::endl;
        }

        file.close();
    }

    void loadFromFile(std::string filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Khong the mo file de doc!");
        }

        customers.clear();
        rewards.clear();

        // TODO: Doc du lieu tu file

        file.close();
    }
};


// Ham menu chinh
int main()
{
    try
    {
        std::locale::global(std::locale("")); // Su dung locale mac dinh cua he thong
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Khong the thiet lap locale: " << e.what() << std::endl;
    }
    PointsManager manager;

    try
    {
        manager.loadFromFile("points_system.dat");
    }
    catch (const std::exception &e)
    {
        std::cout << "Khoi tao he thong moi" << std::endl;
    }

    // Them mot so phan thuong mau
    manager.addReward(1, "Voucher 10%", "Giam gia 10% cho lan mua hang tiep theo", 100);
    manager.addReward(2, "Voucher 20%", "Giam gia 20% cho lan mua hang tiep theo", 200);
    manager.addReward(3, "Mien phi van chuyen", "Mien phi van chuyen cho don hang", 150);

    int choice;
    do
    {
        std::cout << "\n==== HE THONG QUAN LY VI DIEM THUONG ====\n";
        std::cout << "1. Dang ky khach hang moi\n";
        std::cout << "2. Tim kiem khach hang\n";
        std::cout << "3. Cong diem cho khach hang\n";
        std::cout << "4. Doi phan thuong\n";
        std::cout << "5. Xem lich su giao dich\n";
        std::cout << "6. Danh sach khach hang\n";
        std::cout << "7. Luu du lieu\n";
        std::cout << "8. Chuyen diem giua cac vi\n";
        std::cout << "0. Thoat\n";
        std::cout << "Lua chon cua ban: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            std::string name, phone, email;
            std::cout << "Nhap ten khach hang: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            std::cout << "Nhap so dien thoai: ";
            std::cin >> phone;
            std::cout << "Nhap email: ";
            std::cin >> email;

            try
            {
                int id = manager.registerCustomer(name, phone, email);
                std::cout << "Dang ky thanh cong! ID khach hang: " << id << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "Loi: " << e.what() << std::endl;
            }
            break;
        }
        case 2:
        {
            std::string phone;
            std::cout << "Nhap so dien thoai khach hang: ";
            std::cin >> phone;

            Customer *customer = manager.findCustomerByPhone(phone);
            if (customer != nullptr)
            {
                std::cout << "ID: " << customer->getId() << std::endl;
                std::cout << "Ten: " << customer->getName() << std::endl;
                std::cout << "Dien thoai: " << customer->getPhone() << std::endl;
                std::cout << "Email: " << customer->getEmail() << std::endl;
                std::cout << "Diem hien tai: " << customer->getPoints() << std::endl;
            }
            else
            {
                std::cout << "Khong tim thay khach hang!" << std::endl;
            }
            break;
        }
        case 3:
        {
            int id, points;
            std::string description;
            std::cout << "Nhap ID khach hang: ";
            std::cin >> id;
            std::cout << "Nhap so diem: ";
            std::cin >> points;
            std::cout << "Nhap mo ta: ";
            std::cin.ignore();
            std::getline(std::cin, description);

            try
            {
                manager.addPoints(id, points, description);
                std::cout << "Da cong " << points << " diem cho khach hang!" << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "Loi: " << e.what() << std::endl;
            }
            break;
        }
        case 4:
        {
            int customerId, rewardId;
            std::cout << "Nhap ID khach hang: ";
            std::cin >> customerId;
            std::cout << "Nhap ID phan thuong: ";
            std::cin >> rewardId;

            try
            {
                manager.redeemReward(customerId, rewardId);
                std::cout << "Doi phan thuong thanh cong!" << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "Loi: " << e.what() << std::endl;
            }
            break;
        }
        case 5:
        {
            int id;
            std::cout << "Nhap ID khach hang: ";
            std::cin >> id;

            Customer *customer = manager.findCustomer(id);
            if (customer != nullptr)
            {
                auto transactions = customer->getTransactionHistory();
                if (transactions.empty())
                {
                    std::cout << "Khach hang chua co giao dich nao!" << std::endl;
                }
                else
                {
                    std::cout << "LICH SU GIAO DICH:\n";
                    std::cout << "-----------------------------------------\n";
                    for (const auto &trans : transactions)
                    {
                        std::cout << trans.getFormattedTime() << " | ";
                        if (trans.getPointsChange() > 0)
                        {
                            std::cout << "+" << trans.getPointsChange();
                        }
                        else
                        {
                            std::cout << trans.getPointsChange();
                        }
                        std::cout << " diem | " << trans.getDescription() << std::endl;
                    }
                    std::cout << "-----------------------------------------\n";
                }
            }
            else
            {
                std::cout << "Khong tim thay khach hang!" << std::endl;
            }
            break;
        }
        case 6:
        {
            auto customers = manager.getAllCustomers();
            std::cout << "DANH SACH KHACH HANG:\n";
            std::cout << "-----------------------------------------\n";
            std::cout << std::left << std::setw(5) << "ID"
                      << std::setw(20) << "Ten"
                      << std::setw(15) << "Dien thoai"
                      << std::setw(25) << "Email"
                      << "Diem\n";
            std::cout << "-----------------------------------------\n";

            for (const auto &customer : customers)
            {
                std::cout << std::left << std::setw(5) << customer.getId()
                          << std::setw(20) << customer.getName()
                          << std::setw(15) << customer.getPhone()
                          << std::setw(25) << customer.getEmail()
                          << customer.getPoints() << std::endl;
            }
            break;
        }
        case 7:
        {
            try
            {
                manager.saveToFile("points_system.dat");
                std::cout << "Luu du lieu thanh cong!" << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "Loi: " << e.what() << std::endl;
            }
            break;
        }
        case 8:
        {
            int fromId, toId, points;
            std::cout << "Nhap ID vi nguon: ";
            std::cin >> fromId;
            std::cout << "Nhap ID vi dich: ";
            std::cin >> toId;
            std::cout << "Nhap so diem can chuyen: ";
            std::cin >> points;

            try
            {
                manager.transferPoints(fromId, toId, points);
            }
            catch (const std::exception &e)
            {
                std::cout << "Loi: " << e.what() << std::endl;
            }
            break;
        }
        case 0:
            std::cout << "Cam on ban da su dung he thong!" << std::endl;
            break;
        default:
            std::cout << "Lua chon khong hop le!" << std::endl;
        }
    } while (choice != 0);

    return 0;
}