Hệ thống cung cấp các chức năng chính như sau:

1. **Tạo mới tài khoản khi đăng ký**: tạo mới bộ dữ liệu liên quan đến tài khoản người dùng  

Hãy phân tích và đề xuất một cấu trúc dữ liệu (lớp) quản lý thông tin tài khoản người dùng.  
-> Nhóm tôi đã sử dụng việc lưu các trường của lớp UserAccount vào trong file user_account.dat (password đã được hash)  

2. **Lưu trữ dữ liệu**:  lưu lại dữ liệu tài khoản người dùng xuống tập tin.  
Hãy đề xuất việc lưu trữ dữ liệu người dùng dưới dạng tập tin riêng cho từng người dùng hay một tập tin cho tất cả người dùng? Nêu lý do lựa chọn giải pháp. 
-> lưu trữ dữ liệu người dùng dưới dạng tập tin chung cho tất cả người dùng. Điều này nhằm mục đích khi truy vấn tất cả người dùng thì chỉ đọc từ 1 file đó. Những thông tin nhạy cảm như mật khẩu thì ta phải mã hóa trước khi lưu vào tập tin  
Với mật khẩu, hãy tìm hiểu cách lưu mật khẩu của người dùng vào CSDL.  
-> Để lưu được mật khẩu ta phải dùng hàm băm, có nhiều thuật toán để băm có thể kể đến như MD5, ...  
Mọi lưu trữ phải có bản sao lưu (backup). Hãy tìm hiểu quy trình sao lưu - phục hồi dữ liệu. Đề xuất giải pháp sao lưu cho dữ liệu người dùng  
-> Để có thể sao lưu và phục hồi dữ liệu -> với mỗi transaction tôi đã lưu thông tin vào 1 file và giữ trong folder "backup" trong dự án. Nếu sai sót, sẽ trace lại file và hoàn lại dữ liệu như ban đầu khi chưa xảy ra lỗi  

4. **Quản lý đăng nhập**:
Hãy đề xuất cách sử dụng OTP để xác nhận thực hiện giao dịch chuyển điểm giữa các ví.  
-> Tôi sử dụng thư viên curl của C++ hỗ trợ việc gửi OTP qua gmail mà người dùng nhập vào.  
Hệ thống ghi nhận lại giao dịch này trong lịch sử giao dịch (transaction log)  
-> Tất cả những giao dịch đều được tôi ghi vào file transactions.dat trong đó gồm những thông tin như: trạng thái giao dịch, ví nguồn, ví đích, số tiền chuyển khoản.

