# Các thành viên của dự án
- Nguyễn Thu Thảo
- Nguyễn Quang Trung
- Lê Hoàng Nam

# Giới thiệu dự án
- Dự án quản lý ví điểm thưởng giúp cho người dùng có thể dễ dàng chuyển điểm từ ví mình sang ví của người dùng khác một cách dễ dàng.  
- Ngoài ra hệ thống còn sử dụng xác thực OTP mỗi khi chuyển tiền cũng như thay đổi thông tin người dùng nên đem lại sự bảo mật cho người dùng.

# Bản phân tích, đặc tả chức năng đầy đủ:
- Chức năng đăng ký, đăng nhập: Lưu dữ liệu vào file users.dat
- Chức năng chuyển điểm từ ví admin sang ví người dùng
- Chức năng chuyển điểm từ ví người dùng 1 sang người dùng 2
- Chức năng 

# Cách tải và chạy chương trình trên máy tính cá nhân:
- Cài đặt môi trường phát triển C++: VSCode + MinGW
- Cài đặt thư viện libcurl vì dự án có sử dụng
  - Truy cập trang chủ curl: https://curl.se/windows/
  - Tải bản "curl-x.x.x-win64-mingw.zip"
  - Giải nén và thêm vào PATH
- Tải mã nguồn dự án: git clone [địa chỉ repository]
- cd [tên thư mục dự án]
- Đối với MinGW ta sẽ sử dụng câu lệnh biên dịch: g++ -o reward_wallet reward_wallet.cpp entity/*.cpp -I. -lcurl
- Để chạy chương trình ta sử dụng câu lệnh: reward_wallet.exe

# Hệ thống quản lý ví điểm thưởng
## Trả lời các câu hỏi về chức năng chính của hệ thống:
### Hãy phân tích và đề xuất một cấu trúc dữ liệu (lớp) quản lý thông tin tài khoản người dùng.

- Nhóm tôi đã sử dụng việc lưu các trường của lớp `UserAccount` vào trong file `user_account.dat` (password đã được hash).

#### Hãy đề xuất việc lưu trữ dữ liệu người dùng dưới dạng tập tin riêng cho từng người dùng hay một tập tin cho tất cả người dùng? Nêu lý do lựa chọn giải pháp.
- Lưu trữ dữ liệu người dùng dưới dạng **tập tin chung** cho tất cả người dùng.  
  - **Lý do**: Khi truy vấn tất cả người dùng, chỉ cần đọc từ một file duy nhất.  
  - Những thông tin nhạy cảm như mật khẩu cần được mã hóa trước khi lưu vào tập tin.

#### Với mật khẩu, hãy tìm hiểu cách lưu mật khẩu của người dùng vào CSDL. 
- Để lưu mật khẩu, cần sử dụng **hàm băm**.  
  - Có nhiều thuật toán băm phổ biến như: MD5, SHA-256, ...
  - Nhưng trong dự án này, tôi sử dụng hàm băm đã tích hợp sẵn trong thư viện <string> có sẵn của C++ -> dễ sử dụng, thân thiện với C++, và thực hiện cùng mục đích.

#### Mọi lưu trữ phải có bản sao lưu (backup). Hãy tìm hiểu quy trình sao lưu - phục hồi dữ liệu. Đề xuất giải pháp sao lưu cho dữ liệu người dùng
- Nhóm tôi sẽ có sự lựa chọn cho người sử dụng, đó là sao lưu dữ liệu và phục hồi dữ liệu.
- Khi lựa chọn sao lưu dữ liệu -> nó sẽ đọc toàn bộ dữ liệu của 3 enity: users, transactions, wallets -> lưu vào file backup của ngày + giờ hôm đó. 
- Khi lựa chọn phục hồi dữ liệu -> sử dụng std::ifstream và std::ofstream để sao chép nội dung từ tệp backup sang tệp gốc users, transactions, wallets.

#### Với trường hợp mật khẩu tự sinh, hệ thống phải kiểm tra mật khẩu là tự sinh và yêu cầu người dùng thay đổi ngay mật khẩu mới trong lần đăng nhập đầu tiên của mình.
- Trong entity Users có thêm thuộc isGenPassword -> nếu password được gen thì lần đăng nhập đầu tiên thì điều hướng người dùng đến phần thay đổi mật khẩu. Sau khi đổi xong sẽ set isGenPassword là false

#### Sử dụng OTP để xác nhận giao dịch:
- Tôi sử dụng thư viện `curl` của C++ để hỗ trợ việc gửi OTP qua Gmail mà người dùng nhập vào.

#### Ghi nhận lịch sử giao dịch:
- Hệ thống ghi nhận lại giao dịch này trong **lịch sử giao dịch** (transaction log).  
- Tất cả các giao dịch đều được ghi vào file `transactions.dat`, bao gồm các thông tin:
  - Trạng thái giao dịch.
  - Ví nguồn.
  - Ví đích.
  - Số tiền chuyển khoản.