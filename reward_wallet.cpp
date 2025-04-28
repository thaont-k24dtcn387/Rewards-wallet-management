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

// Lớp User: Quản lý thông tin người dùng
class User
{
private:
    std::string userId;
    std::string username;
    std::string passwordHash; // Mật khẩu đã được mã hóa
    std::string fullName;
    std::string email;
    std::string phone;
    bool isAdmin;

public:
    User() : isAdmin(false) {}

    User(const std::string &id, const std::string &username, const std::string &passwordHash,
         const std::string &fullName, const std::string &email, const std::string &phone, bool isAdmin = false)
        : userId(id), username(username), passwordHash(passwordHash),
          fullName(fullName), email(email), phone(phone), isAdmin(isAdmin) {}

    // Getters
    std::string getUserId() const { return userId; }
    std::string getUsername() const { return username; }
    std::string getPasswordHash() const { return passwordHash; }
    std::string getFullName() const { return fullName; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    bool getIsAdmin() const { return isAdmin; }

    // Setters
    void setUserId(const std::string &id) { userId = id; }
    void setUsername(const std::string &uname) { username = uname; }
    void setPasswordHash(const std::string &pwd) { passwordHash = pwd; }
    void setFullName(const std::string &name) { fullName = name; }
    void setEmail(const std::string &mail) { email = mail; }
    void setPhone(const std::string &ph) { phone = ph; }
    void setIsAdmin(bool admin) { isAdmin = admin; }

    // Chuyển đổi thành chuỗi để lưu vào file
    std::string toString() const
    {
        return userId + "," + username + "," + passwordHash + "," +
               fullName + "," + email + "," + phone + "," + (isAdmin ? "1" : "0");
    }

    // Tạo đối tượng User từ chuỗi
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
            }
            i++;
        }
        user.setIsAdmin(s == "1");
        return user;
    }
};

// Lớp Wallet: Quản lý ví điểm thưởng
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

    // Thêm điểm vào ví
    bool addPoints(double points)
    {
        balance += points;
        return true;
    }

    // Trừ điểm từ ví
    bool subtractPoints(double points)
    {
        if (balance >= points)
        {
            balance -= points;
            return true;
        }
        return false;
    }

    // Chuyển đổi thành chuỗi để lưu vào file
    std::string toString() const
    {
        return walletId + "," + userId + "," + std::to_string(balance);
    }

    // Tạo đối tượng Wallet từ chuỗi
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

// Lớp Transaction: Quản lý giao dịch
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

    // Chuyển đổi thành chuỗi để lưu vào file
    std::string toString() const
    {
        return transactionId + "," + fromWalletId + "," + toWalletId + "," +
               std::to_string(amount) + "," + timestamp + "," + status + "," + description;
    }

    // Tạo đối tượng Transaction từ chuỗi
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
        transaction.setDescription(s); // Phần còn lại là description
        return transaction;
    }
};

// Lớp quản lý dữ liệu
class DataManager
{
private:
    std::string usersFile = "users.dat";
    std::string walletsFile = "wallets.dat";
    std::string transactionsFile = "transactions.dat";
    std::string backupDir = "backup/";

    std::vector<Wallet> wallets;
    std::vector<Transaction> transactions;

   

    // Tạo ID ngẫu nhiên duy nhất
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

    // Lấy thời gian hiện tại dưới dạng chuỗi
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
    // Đọc dữ liệu từ file
    void loadData()
    {
        loadUsers();
        loadWallets();
        loadTransactions();
    }

    // Đọc dữ liệu người dùng từ file
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

    // Đọc dữ liệu ví từ file
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

    // Đọc dữ liệu giao dịch từ file
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

    // Lưu dữ liệu vào file
    void saveData()
    {
        saveUsers();
        saveWallets();
        saveTransactions();
        backupData(); // Sao lưu sau mỗi lần lưu
    }

    // Lưu dữ liệu người dùng vào file
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

    // Lưu dữ liệu ví vào file
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

    // Lưu dữ liệu giao dịch vào file
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

    // Sao lưu dữ liệu
    void backupData()
    {
        std::string timestamp = getCurrentTimestamp();
        std::replace(timestamp.begin(), timestamp.end(), ' ', '_');
        std::replace(timestamp.begin(), timestamp.end(), ':', '-');

        std::string backupUserFile = backupDir + "users_" + timestamp + ".bak";
        std::string backupWalletFile = backupDir + "wallets_" + timestamp + ".bak";
        std::string backupTransactionFile = backupDir + "transactions_" + timestamp + ".bak";

        // Tạo thư mục backup nếu chưa tồn tại
        system(("mkdir -p " + backupDir).c_str());

        // Sao chép các file dữ liệu vào thư mục backup
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

    // Khôi phục dữ liệu từ bản sao lưu
    bool restoreData(const std::string &timestamp)
    {
        std::string backupUserFile = backupDir + "users_" + timestamp + ".bak";
        std::string backupWalletFile = backupDir + "wallets_" + timestamp + ".bak";
        std::string backupTransactionFile = backupDir + "transactions_" + timestamp + ".bak";

        // Kiểm tra xem các file backup có tồn tại không
        std::ifstream testUsers(backupUserFile);
        std::ifstream testWallets(backupWalletFile);
        std::ifstream testTransactions(backupTransactionFile);

        if (!testUsers || !testWallets || !testTransactions)
        {
            return false; // Không tìm thấy file backup
        }

        // Sao chép từ backup vào file chính
        std::ifstream srcUsers(backupUserFile, std::ios::binary);
        std::ofstream dstUsers(usersFile, std::ios::binary);
        dstUsers << srcUsers.rdbuf();

        std::ifstream srcWallets(backupWalletFile, std::ios::binary);
        std::ofstream dstWallets(walletsFile, std::ios::binary);
        dstWallets << srcWallets.rdbuf();

        std::ifstream srcTransactions(backupTransactionFile, std::ios::binary);
        std::ofstream dstTransactions(transactionsFile, std::ios::binary);
        dstTransactions << srcTransactions.rdbuf();

        // Tải lại dữ liệu từ file
        loadData();

        return true;
    }

    // Tạo và thêm người dùng mới
    User createUser(const std::string &username, const std::string &password,
                    const std::string &fullName, const std::string &email,
                    const std::string &phone, bool isAdmin = false)
    {
        // Kiểm tra xem username đã tồn tại chưa
        for (const auto &user : users)
        {
            if (user.getUsername() == username)
            {
                throw std::runtime_error("Tên người dùng đã tồn tại.");
            }
        }

        std::string userId = generateUniqueId();
        std::string passwordHash = hashPassword(password);

        User newUser(userId, username, passwordHash, fullName, email, phone, isAdmin);
        users.push_back(newUser);

        // Tạo ví cho người dùng mới
        createWallet(userId);

        saveData();
        return newUser;
    }

    // Tạo và thêm ví mới
    Wallet createWallet(const std::string &userId)
    {
        std::string walletId = "W" + generateUniqueId();
        Wallet newWallet(walletId, userId);
        wallets.push_back(newWallet);

        saveData();
        return newWallet;
    }

    // Tạo và thêm giao dịch mới
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

    // Thực hiện giao dịch chuyển điểm
    bool executeTransaction(const std::string &transactionId, const std::string &otp)
    {
        // Tìm giao dịch theo ID
        auto transactionIt = std::find_if(transactions.begin(), transactions.end(),
                                          [&transactionId](const Transaction &t)
                                          { return t.getTransactionId() == transactionId; });

        if (transactionIt == transactions.end() || transactionIt->getStatus() != "pending")
        {
            return false; // Không tìm thấy giao dịch hoặc giao dịch không ở trạng thái pending
        }

        // Xác thực OTP (đây là ví dụ đơn giản, trong thực tế cần một hệ thống OTP an toàn hơn)
        if (otp != "123456")
        { // OTP mẫu
            transactionIt->setStatus("failed");
            saveData();
            return false;
        }

        // Tìm ví nguồn
        auto fromWalletIt = std::find_if(wallets.begin(), wallets.end(),
                                         [&](const Wallet &w)
                                         { return w.getWalletId() == transactionIt->getFromWalletId(); });

        // Tìm ví đích
        auto toWalletIt = std::find_if(wallets.begin(), wallets.end(),
                                       [&](const Wallet &w)
                                       { return w.getWalletId() == transactionIt->getToWalletId(); });

        if (fromWalletIt == wallets.end() || toWalletIt == wallets.end())
        {
            transactionIt->setStatus("failed");
            saveData();
            return false; // Không tìm thấy ví nguồn hoặc ví đích
        }

        double amount = transactionIt->getAmount();

        // Kiểm tra số dư
        if (fromWalletIt->getBalance() < amount)
        {
            transactionIt->setStatus("failed");
            saveData();
            return false; // Số dư không đủ
        }

        // Thực hiện giao dịch (atomic)
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
            // Nếu có lỗi, hủy giao dịch
            transactionIt->setStatus("failed");
            saveData();
            return false;
        }
    }

    // Tạo OTP (giả lập)
    std::string generateOTP()
    {
        // Trong thực tế, OTP sẽ được gửi qua SMS hoặc email
        // Đây chỉ là ví dụ đơn giản
        return "123456"; // OTP mẫu
    }

    // Sinh mật khẩu tự động
    std::string generateRandomPassword()
    {
        const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, chars.size() - 1);

        std::string password;
        for (int i = 0; i < 10; ++i)
        { // Mật khẩu 10 ký tự
            password += chars[dis(gen)];
        }
        return password;
    }

    // Xác thực đăng nhập
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

    // Lấy ví của người dùng
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

    // Lấy ví theo ID
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

    // Lấy danh sách giao dịch của ví
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

    // Lấy người dùng theo username
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

    // Lấy người dùng theo ID
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

// Lớp quản lý hệ thống
class System
{
private:
    DataManager dataManager;
    User *currentUser;

    // Hiển thị menu cho người dùng
    void showUserMenu()
    {
        while (true)
        {
            std::cout << "\n===== MENU NGƯỜI DÙNG =====\n";
            std::cout << "1. Xem thông tin cá nhân\n";
            std::cout << "2. Xem số dư ví\n";
            std::cout << "3. Xem lịch sử giao dịch\n";
            std::cout << "4. Chuyển điểm\n";
            std::cout << "5. Đổi mật khẩu\n";
            std::cout << "6. Đăng xuất\n";
            std::cout << "Lựa chọn của bạn: ";

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
                std::cout << "Lựa chọn không hợp lệ.\n";
            }
        }
    }

    // Hiển thị menu cho admin
    void showAdminMenu()
    {
        while (true)
        {
            std::cout << "\n===== MENU QUẢN TRỊ VIÊN =====\n";
            std::cout << "1. Tạo tài khoản người dùng mới\n";
            std::cout << "2. Xem danh sách người dùng\n";
            std::cout << "3. Sao lưu dữ liệu\n";
            std::cout << "4. Khôi phục dữ liệu\n";
            std::cout << "5. Đăng xuất\n";
            std::cout << "Lựa chọn của bạn: ";

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
                std::cout << "Lựa chọn không hợp lệ.\n";
            }
        }
    }

    // Hiển thị thông tin người dùng
    void showUserInfo()
    {
        std::cout << "\n===== THÔNG TIN CÁ NHÂN =====\n";
        std::cout << "ID: " << currentUser->getUserId() << std::endl;
        std::cout << "Tên đăng nhập: " << currentUser->getUsername() << std::endl;
        std::cout << "Họ tên: " << currentUser->getFullName() << std::endl;
        std::cout << "Email: " << currentUser->getEmail() << std::endl;
        std::cout << "Số điện thoại: " << currentUser->getPhone() << std::endl;
        std::cout << "Loại tài khoản: " << (currentUser->getIsAdmin() ? "Quản trị viên" : "Người dùng") << std::endl;
    }

    // Hiển thị số dư ví
    void showWalletBalance()
    {
        Wallet *wallet = dataManager.getWalletByUserId(currentUser->getUserId());

        if (wallet)
        {
            std::cout << "\n===== SỐ DƯ VÍ =====\n";
            std::cout
                << "ID ví: " << wallet->getWalletId() << std::endl;
            std::cout << "Số dư: " << wallet->getBalance() << " điểm" << std::endl;
        }
        else
        {
            std::cout << "Không tìm thấy thông tin ví.\n";
        }
    }

    // Hiển thị lịch sử giao dịch
    void showTransactionHistory()
    {
        Wallet *wallet = dataManager.getWalletByUserId(currentUser->getUserId());

        if (!wallet)
        {
            std::cout << "Không tìm thấy thông tin ví.\n";
            return;
        }

        std::vector<Transaction> transactions = dataManager.getTransactionsByWalletId(wallet->getWalletId());

        std::cout << "\n===== LỊCH SỬ GIAO DỊCH =====\n";

        if (transactions.empty())
        {
            std::cout << "Chưa có giao dịch nào.\n";
            return;
        }

        std::cout << std::left << std::setw(12) << "ID GD"
                  << std::setw(12) << "TỪ VÍ"
                  << std::setw(12) << "ĐẾN VÍ"
                  << std::setw(10) << "SỐ ĐIỂM"
                  << std::setw(20) << "THỜI GIAN"
                  << std::setw(10) << "TRẠNG THÁI"
                  << "MÔ TẢ\n";

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

    // Chuyển điểm từ ví của người dùng hiện tại sang ví khác
    void transferPoints()
    {
        std::cin.ignore(); // Xóa bộ đệm

        Wallet *fromWallet = dataManager.getWalletByUserId(currentUser->getUserId());

        if (!fromWallet)
        {
            std::cout << "Không tìm thấy thông tin ví.\n";
            return;
        }

        std::string toWalletId;
        double amount;
        std::string description;

        std::cout << "\n===== CHUYỂN ĐIỂM =====\n";
        std::cout << "Nhập ID ví nhận: ";
        std::getline(std::cin, toWalletId);

        // Kiểm tra ví đích
        Wallet *toWallet = dataManager.getWalletById(toWalletId);
        if (!toWallet)
        {
            std::cout << "Không tìm thấy ví đích.\n";
            return;
        }

        // Không cho phép chuyển điểm cho chính mình
        if (fromWallet->getWalletId() == toWalletId)
        {
            std::cout << "Không thể chuyển điểm cho chính mình.\n";
            return;
        }

        std::cout << "Nhập số điểm cần chuyển: ";
        std::cin >> amount;

        // Kiểm tra số điểm cần chuyển
        if (amount <= 0)
        {
            std::cout << "Số điểm phải lớn hơn 0.\n";
            return;
        }

        if (fromWallet->getBalance() < amount)
        {
            std::cout << "Số dư không đủ.\n";
            return;
        }

        std::cin.ignore(); // Xóa bộ đệm
        std::cout << "Nhập mô tả giao dịch: ";
        std::getline(std::cin, description);

        // Tạo giao dịch
        Transaction transaction = dataManager.createTransaction(fromWallet->getWalletId(), toWalletId, amount, description);

        // Sinh OTP
        std::string otp = dataManager.generateOTP();
        std::cout << "Mã OTP của bạn là: " << otp << std::endl;

        // Xác nhận OTP
        std::string inputOTP;
        std::cout << "Nhập mã OTP để xác nhận giao dịch: ";
        std::cin >> inputOTP;

        // Thực hiện giao dịch
        bool success = dataManager.executeTransaction(transaction.getTransactionId(), inputOTP);

        if (success)
        {
            std::cout << "Chuyển điểm thành công.\n";
        }
        else
        {
            std::cout << "Chuyển điểm thất bại. Vui lòng kiểm tra lại OTP hoặc thử lại sau.\n";
        }
    }

    // Đổi mật khẩu
    void changePassword()
    {
        std::cin.ignore(); // Xóa bộ đệm

        std::string currentPassword, newPassword, confirmPassword;

        std::cout << "\n===== ĐỔI MẬT KHẨU =====\n";
        std::cout << "Nhập mật khẩu hiện tại: ";
        std::getline(std::cin, currentPassword);

        // Xác thực mật khẩu hiện tại
        if (dataManager.hashPassword(currentPassword) != currentUser->getPasswordHash())
        {
            std::cout << "Mật khẩu hiện tại không đúng.\n";
            return;
        }

        std::cout << "Nhập mật khẩu mới: ";
        std::getline(std::cin, newPassword);

        std::cout << "Xác nhận mật khẩu mới: ";
        std::getline(std::cin, confirmPassword);

        if (newPassword != confirmPassword)
        {
            std::cout << "Mật khẩu xác nhận không khớp.\n";
            return;
        }

        // Cập nhật mật khẩu
        currentUser->setPasswordHash(dataManager.hashPassword(newPassword));
        dataManager.saveData();

        std::cout << "Đổi mật khẩu thành công.\n";
    }

    // Đăng xuất
    void logout()
    {
        currentUser = nullptr;
        std::cout << "Đã đăng xuất thành công.\n";
    }

    // Tạo người dùng mới (chỉ dành cho admin)
    void createNewUser()
    {
        std::cin.ignore(); // Xóa bộ đệm

        std::string username, password, fullName, email, phone;
        bool isAdmin;

        std::cout << "\n===== TẠO NGƯỜI DÙNG MỚI =====\n";
        std::cout << "Nhập tên đăng nhập: ";
        std::getline(std::cin, username);

        // Kiểm tra xem username đã tồn tại chưa
        if (dataManager.getUserByUsername(username))
        {
            std::cout << "Tên đăng nhập đã tồn tại.\n";
            return;
        }

        std::cout << "Tạo mật khẩu tự động? (1-Có, 0-Không): ";
        int autoGen;
        std::cin >> autoGen;

        std::cin.ignore(); // Xóa bộ đệm

        if (autoGen == 1)
        {
            password = dataManager.generateRandomPassword();
            std::cout << "Mật khẩu được tạo tự động: " << password << std::endl;
        }
        else
        {
            std::cout << "Nhập mật khẩu: ";
            std::getline(std::cin, password);
        }

        std::cout << "Nhập họ tên: ";
        std::getline(std::cin, fullName);

        std::cout << "Nhập email: ";
        std::getline(std::cin, email);

        std::cout << "Nhập số điện thoại: ";
        std::getline(std::cin, phone);

        std::cout << "Là quản trị viên? (1-Có, 0-Không): ";
        std::cin >> isAdmin;

        try
        {
            User newUser = dataManager.createUser(username, password, fullName, email, phone, isAdmin);
            std::cout << "Tạo tài khoản thành công.\n";
            std::cout << "ID người dùng: " << newUser.getUserId() << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "Lỗi: " << e.what() << std::endl;
        }
    }

    // Hiển thị danh sách người dùng (chỉ dành cho admin)
    void showAllUsers()
    {
        std::vector<User> &users = dataManager.users;

        std::cout << "\n===== DANH SÁCH NGƯỜI DÙNG =====\n";
        std::cout << std::left << std::setw(12) << "ID"
                  << std::setw(20) << "TÊN ĐĂNG NHẬP"
                  << std::setw(30) << "HỌ TÊN"
                  << std::setw(30) << "EMAIL"
                  << std::setw(15) << "SỐ ĐIỆN THOẠI"
                  << "LOẠI\n";

        for (const auto &user : users)
        {
            std::cout << std::left << std::setw(12) << user.getUserId()
                      << std::setw(20) << user.getUsername()
                      << std::setw(30) << user.getFullName()
                      << std::setw(30) << user.getEmail()
                      << std::setw(15) << user.getPhone()
                      << (user.getIsAdmin() ? "Quản trị viên" : "Người dùng") << std::endl;
        }
    }

    // Sao lưu dữ liệu
    void backupData()
    {
        dataManager.backupData();
        std::cout << "Đã sao lưu dữ liệu thành công.\n";
    }

    // Khôi phục dữ liệu
    void restoreData()
    {
        std::cin.ignore(); // Xóa bộ đệm

        std::string timestamp;
        std::cout << "\n===== KHÔI PHỤC DỮ LIỆU =====\n";
        std::cout << "Nhập thời gian sao lưu (định dạng YYYY-MM-DD_HH-MM-SS): ";
        std::getline(std::cin, timestamp);

        bool success = dataManager.restoreData(timestamp);

        if (success)
        {
            std::cout << "Khôi phục dữ liệu thành công.\n";
        }
        else
        {
            std::cout << "Không tìm thấy bản sao lưu tại thời điểm này.\n";
        }
    }

public:
    System() : currentUser(nullptr)
    {
        // Kiểm tra xem đã có tài khoản admin mặc định chưa
        User *admin = dataManager.getUserByUsername("admin");
        if (!admin)
        {
            try
            {
                dataManager.createUser("admin", "admin123", "Administrator", "admin@example.com", "0123456789", true);
                std::cout << "Đã tạo tài khoản quản trị mặc định.\n";
                std::cout << "Tên đăng nhập: admin\n";
                std::cout << "Mật khẩu: admin123\n";
            }
            catch (const std::exception &e)
            {
                std::cout << "Lỗi khi tạo tài khoản quản trị: " << e.what() << std::endl;
            }
        }
    }

    // Hiển thị menu chính
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
                std::cout << "\n===== HỆ THỐNG ĐĂNG NHẬP VÀ VÍ ĐIỂM THƯỞNG =====\n";
                std::cout << "1. Đăng nhập\n";
                std::cout << "2. Đăng ký\n";
                std::cout << "3. Thoát\n";
                std::cout << "Lựa chọn của bạn: ";

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
                    std::cout << "Cảm ơn bạn đã sử dụng hệ thống. Tạm biệt!\n";
                    return;
                default:
                    std::cout << "Lựa chọn không hợp lệ.\n";
                }
            }
        }
    }

    // Đăng nhập
    void login()
    {
        std::cin.ignore(); // Xóa bộ đệm

        std::string username, password;

        std::cout << "\n===== ĐĂNG NHẬP =====\n";
        std::cout << "Tên đăng nhập: ";
        std::getline(std::cin, username);

        std::cout << "Mật khẩu: ";
        std::getline(std::cin, password);

        currentUser = dataManager.authenticate(username, password);

        if (currentUser)
        {
            std::cout << "Đăng nhập thành công. Xin chào, " << currentUser->getFullName() << "!\n";
        }
        else
        {
            std::cout << "Đăng nhập thất bại. Tên đăng nhập hoặc mật khẩu không đúng.\n";
        }
    }

    // Đăng ký người dùng mới
    void registerUser()
    {
        std::cin.ignore(); // Xóa bộ đệm

        std::string username, password, confirmPassword, fullName, email, phone;

        std::cout << "\n===== ĐĂNG KÝ =====\n";
        std::cout << "Nhập tên đăng nhập: ";
        std::getline(std::cin, username);

        // Kiểm tra xem username đã tồn tại chưa
        if (dataManager.getUserByUsername(username))
        {
            std::cout << "Tên đăng nhập đã tồn tại.\n";
            return;
        }

        std::cout << "Nhập mật khẩu: ";
        std::getline(std::cin, password);

        std::cout << "Xác nhận mật khẩu: ";
        std::getline(std::cin, confirmPassword);

        if (password != confirmPassword)
        {
            std::cout << "Mật khẩu xác nhận không khớp.\n";
            return;
        }

        std::cout << "Nhập họ tên: ";
        std::getline(std::cin, fullName);

        std::cout << "Nhập email: ";
        std::getline(std::cin, email);

        std::cout << "Nhập số điện thoại: ";
        std::getline(std::cin, phone);

        try
        {
            User newUser = dataManager.createUser(username, password, fullName, email, phone);
            std::cout << "Đăng ký thành công.\n";
            currentUser = dataManager.getUserByUsername(username);
        }
        catch (const std::exception &e)
        {
            std::cout << "Lỗi: " << e.what() << std::endl;
        }
    }
};

int main()
{
    System system;
    system.showMainMenu();
    return 0;
}