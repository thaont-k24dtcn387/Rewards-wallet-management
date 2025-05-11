# Hệ thống quản lý ví điểm thưởng

## Các chức năng chính của hệ thống:

### 1. **Tạo mới tài khoản khi đăng ký**
- Tạo mới bộ dữ liệu liên quan đến tài khoản người dùng.

#### Phân tích và đề xuất:
- Nhóm tôi đã sử dụng việc lưu các trường của lớp `UserAccount` vào trong file `user_account.dat` (password đã được hash).

---

### 2. **Lưu trữ dữ liệu**
- Lưu lại dữ liệu tài khoản người dùng xuống tập tin.

#### Đề xuất:
- Lưu trữ dữ liệu người dùng dưới dạng **tập tin chung** cho tất cả người dùng.  
  - **Lý do**: Khi truy vấn tất cả người dùng, chỉ cần đọc từ một file duy nhất.  
  - Những thông tin nhạy cảm như mật khẩu cần được mã hóa trước khi lưu vào tập tin.

#### Lưu mật khẩu:
- Để lưu mật khẩu, cần sử dụng **hàm băm**.  
  - Có nhiều thuật toán băm phổ biến như: MD5, SHA-256, ...
  - Nhưng trong dự án này, tôi sử dụng hàm băm đã tích hợp sẵn trong thư viện <string> có sẵn của C++ -> dễ sử dụng, thân thiện với C++, và thực hiện cùng mục đích.

#### Sao lưu và phục hồi dữ liệu:
- Để sao lưu và phục hồi dữ liệu:
  - Với mỗi transaction, tôi đã lưu thông tin vào một file và giữ trong thư mục `backup` trong dự án.
  - Nếu xảy ra sai sót, có thể trace lại file và hoàn lại dữ liệu như ban đầu khi chưa xảy ra lỗi.

---

### 3. **Quản lý đăng nhập**
#### Sử dụng OTP để xác nhận giao dịch:
- Tôi sử dụng thư viện `curl` của C++ để hỗ trợ việc gửi OTP qua Gmail mà người dùng nhập vào.

#### Ghi nhận lịch sử giao dịch:
- Hệ thống ghi nhận lại giao dịch này trong **lịch sử giao dịch** (transaction log).  
- Tất cả các giao dịch đều được ghi vào file `transactions.dat`, bao gồm các thông tin:
  - Trạng thái giao dịch.
  - Ví nguồn.
  - Ví đích.
  - Số tiền chuyển khoản.