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
    int UserAccountId;
    int pointsChange;
    std::string description;
    time_t timestamp;

public:
    Transaction(int _UserAccountId, int _pointsChange, std::string _description, time_t _timestamp)
        : UserAccountId(_UserAccountId), pointsChange(_pointsChange), description(_description), timestamp(_timestamp) {}

    int getUserAccountId() const { return UserAccountId; }
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

class UserAccount
{
private:
    int id;                                // ID duy nhất của tài khoản
    std::string name;                      // Tên người dùng
    std::string phone;                     // Số điện thoại
    std::string email;                     // Email
    std::string hashedPassword;            // Mật khẩu đã băm
    double balance;                        // Số dư ví
    int points;                            // Điểm thưởng
    std::vector<Transaction> transactions; // Lịch sử giao dịch

public:
    UserAccount() : id(0), balance(0), points(0) {}
    UserAccount(int _id, std::string _name, std::string _phone, std::string _email)
        : id(_id), name(_name), phone(_phone), email(_email), hashedPassword("12345678"), balance(0), points(0) {}
    UserAccount(int _id, std::string _name, std::string _phone, std::string _email, std::string _hashedPassword)
        : id(_id), name(_name), phone(_phone), email(_email), hashedPassword(_hashedPassword), balance(0), points(0) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getPhone() const { return phone; }
    std::string getEmail() const { return email; }
    double getBalance() const { return balance; }
    int getPoints() const { return points; }
    std::string getHashedPassword() const { return hashedPassword; }

    void addBalance(double amount) { balance += amount; }
    void subtractBalance(double amount)
    {
        if (balance >= amount)
            balance -= amount;
        else
            throw std::runtime_error("So du khong du!");
    }

    void addPoints(int value, std::string description)
    {
        points += value;
        transactions.push_back(Transaction(id, value, description, time(nullptr)));
        std::vector<UserAccount> users = loadUsersFromFile("user_account.dat");
        for (auto &user : users)
        {
            cout<< "user: " << &user << endl;
            // if (user.getId() == id)
            // {
            //     user.points = points;             // Cập nhật điểm
            //     user.transactions = transactions; // Cập nhật lịch sử giao dịch
            //     break;
            // }
        }
        // saveUsersToFile(users, "user_account.dat");
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

    void addTransaction(const Transaction &transaction) { transactions.push_back(transaction); }
    std::vector<Transaction> getTransactionHistory() const { return transactions; }
    void saveUsersToFile(const std::vector<UserAccount> &users, const std::string &filename)
    {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("Khong the mo file de ghi!");

        for (const auto &user : users)
        {
            file << user.getId() << "|"
                 << user.getName() << "|"
                 << user.getPhone() << "|"
                 << user.getEmail() << "|"
                 << user.getHashedPassword() << "|"
                 << user.getBalance() << std::endl;
        }
        file.close();
    }
    std::vector<UserAccount> loadUsersFromFile(const std::string &filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("Khong the mo file de doc!");

        std::vector<UserAccount> users;
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            int id, points;
            std::string name, phone, email, hashedPassword;
            double balance;

            // Đọc các trường từ dòng
            iss >> id;
            iss.ignore(); // Bỏ qua ký tự phân cách '|'
            std::getline(iss, name, '|');
            std::getline(iss, phone, '|');
            std::getline(iss, email, '|');
            std::getline(iss, hashedPassword, '|');
            iss >> balance;
            iss.ignore(); // Bỏ qua ký tự phân cách '|'
            iss >> points;

            // Thêm UserAccount vào danh sách
            users.emplace_back(id, name, phone, email, hashedPassword);
            users.back().addBalance(balance);                   // Gán số dư
            users.back().addPoints(points, "Khoi tao tu file"); // Gán điểm
        }
        file.close();
        return users;
    }
    std::vector<UserAccount> getAllUserAccounts() const
    {
        // lay tu user_account.dat ra

        std::vector<UserAccount> userAccounts;
        userAccounts.push_back(*this);
        return userAccounts;
    }
};

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
    std::vector<UserAccount> userAccounts;
    std::map<int, Reward> rewards;
    int nextUserAccountId;

    // Tạo OTP ngẫu nhiên
    int generateOTP()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(100000, 999999);
        return dis(gen);
    }

public:
    PointsManager() : nextUserAccountId(1) {}

    // Chuyển điểm giữa các ví
    void transferPoints(int fromUserAccountId, int toUserAccountId, int points)
    {
        UserAccount *fromUserAccount = findUserAccount(fromUserAccountId);
        UserAccount *toUserAccount = findUserAccount(toUserAccountId);

        if (fromUserAccount == nullptr)
        {
            throw std::runtime_error("Khong tim thay vi nguon!");
        }
        if (toUserAccount == nullptr)
        {
            throw std::runtime_error("Khong tim thay vi dich!");
        }

        // Kiểm tra số dư
        if (fromUserAccount->getPoints() < points)
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
            fromUserAccount->usePoints(points, "Chuyen diem den vi ID: " + std::to_string(toUserAccountId));
            toUserAccount->addPoints(points, "Nhan diem tu vi ID: " + std::to_string(fromUserAccountId));
            std::cout << "Giao dich thanh cong!" << std::endl;
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Giao dich that bai: " + std::string(e.what()));
        }
    }

    int registerUserAccount(std::string name, std::string phone, std::string email)
    {
        nextUserAccountId = getIdMaxOfFile("user_account.dat");
        // Kiem tra trung lap
        for (const auto &UserAccount : userAccounts)
        {
            if (UserAccount.getPhone() == phone || UserAccount.getEmail() == email)
            {
                throw std::runtime_error("So dien thoai hoac email da duoc dang ky!");
            }
        }

        UserAccount newUserAccount(nextUserAccountId, name, phone, email);
        userAccounts.push_back(newUserAccount);
        return nextUserAccountId;
    }

    UserAccount *findUserAccount(int id)
    {
        cout << "ID: " << id << endl;
        UserAccount newUser;
        userAccounts = newUser.loadUsersFromFile("user_account.dat");
        for (auto &UserAccount : userAccounts)
        {
            if (UserAccount.getId() == id)
            {
                cout << "user: " << &UserAccount << endl;
                return &UserAccount;
            }
        }
        return nullptr;
    }

    UserAccount *findUserAccountByPhone(std::string phone)
    {
        for (auto &UserAccount : userAccounts)
        {
            if (UserAccount.getPhone() == phone)
            {
                return &UserAccount;
            }
        }
        return nullptr;
    }

    void addPoints(int UserAccountId, int points, std::string description)
    {
        UserAccount *UserAccount = findUserAccount(UserAccountId);
        if (UserAccount != nullptr)
        {
            UserAccount->addPoints(points, description);
        }
        else
        {
            throw std::runtime_error("Khong tim thay khach hang!");
        }
    }

    void redeemReward(int UserAccountId, int rewardId)
    {
        UserAccount *UserAccount = findUserAccount(UserAccountId);
        if (UserAccount == nullptr)
        {
            throw std::runtime_error("Khong tim thay khach hang!");
        }

        auto rewardIt = rewards.find(rewardId);
        if (rewardIt == rewards.end())
        {
            throw std::runtime_error("Khong tim thay phan thuong!");
        }

        Reward reward = rewardIt->second;
        if (UserAccount->getPoints() < reward.getPointsRequired())
        {
            throw std::runtime_error("Khong du diem de doi phan thuong nay!");
        }

        UserAccount->usePoints(reward.getPointsRequired(), "Doi thuong: " + reward.getName());
    }

    void addReward(int id, std::string name, std::string description, int pointsRequired)
    {
        rewards[id] = Reward(id, name, description, pointsRequired);
    }

    std::vector<UserAccount> getAllUserAccounts() const
    {
        return userAccounts;
    }

    void saveToFile(std::string filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Khong the mo file de ghi!");
        }

        // Ghi du lieu khach hang
        file << userAccounts.size() << std::endl;
        for (const auto &UserAccount : userAccounts)
        {
            file << UserAccount.getId() << "|"
                 << UserAccount.getName() << "|"
                 << UserAccount.getPhone() << "|"
                 << UserAccount.getEmail() << "|"
                 << UserAccount.getPoints() << std::endl;

            // Ghi giao dich
            auto transactions = UserAccount.getTransactionHistory();
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
    int getIdMaxOfFile(std::string filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Khong the mo file de doc!");
        }

        if (filename == "user_account.dat")
        {
            int maxId = 0; // Biến lưu ID lớn nhất
            std::string line;

            while (std::getline(file, line))
            {
                std::istringstream iss(line);
                int id;
                char delimiter;

                // Đọc ID (phần tử đầu tiên trước dấu '|')
                iss >> id >> delimiter;

                // Cập nhật maxId nếu ID lớn hơn
                if (id > maxId)
                {
                    maxId = id;
                }
            }

            file.close();

            // Gán nextUserAccountId bằng maxId + 1
            nextUserAccountId = maxId + 1;
        }
        file.close();
        return nextUserAccountId;
    }
    void loadFromFile(std::string filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Khong the mo file de doc!");
        }

        userAccounts.clear();
        rewards.clear();
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
    UserAccount newUser = UserAccount(1, "Nguyen Van A", "0123456789", "abc@gmail.com", "123");

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
                int id = manager.registerUserAccount(name, phone, email);
                std::vector<UserAccount> users = newUser.loadUsersFromFile("user_account.dat");
                newUser = UserAccount(id, name, phone, email);
                users.push_back(newUser);
                newUser.saveUsersToFile(users, "user_account.dat");
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

            UserAccount *UserAccount = manager.findUserAccountByPhone(phone);
            if (UserAccount != nullptr)
            {
                std::cout << "ID: " << UserAccount->getId() << std::endl;
                std::cout << "Ten: " << UserAccount->getName() << std::endl;
                std::cout << "Dien thoai: " << UserAccount->getPhone() << std::endl;
                std::cout << "Email: " << UserAccount->getEmail() << std::endl;
                std::cout << "Diem hien tai: " << UserAccount->getPoints() << std::endl;
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
            int UserAccountId, rewardId;
            std::cout << "Nhap ID khach hang: ";
            std::cin >> UserAccountId;
            std::cout << "Nhap ID phan thuong: ";
            std::cin >> rewardId;

            try
            {
                manager.redeemReward(UserAccountId, rewardId);
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

            UserAccount *UserAccount = manager.findUserAccount(id);
            if (UserAccount != nullptr)
            {
                auto transactions = UserAccount->getTransactionHistory();
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
            auto UserAccounts = newUser.loadUsersFromFile("user_account.dat");
            std::cout << "DANH SACH KHACH HANG:\n";
            std::cout << "-----------------------------------------\n";
            std::cout << std::left << std::setw(5) << "ID"
                      << std::setw(20) << "Ten"
                      << std::setw(15) << "Dien thoai"
                      << std::setw(25) << "Email"
                      << "Diem\n";
            std::cout << "-----------------------------------------\n";
            for (const auto &UserAccount : UserAccounts)
            {
                std::cout << std::left << std::setw(5) << UserAccount.getId()
                          << std::setw(20) << UserAccount.getName()
                          << std::setw(15) << UserAccount.getPhone()
                          << std::setw(25) << UserAccount.getEmail()
                          << UserAccount.getBalance() << std::endl;
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