### A. Các chức năng:

Hệ thống cung cấp các chức năng chính như sau:

1. **Tạo mới tài khoản khi đăng ký**: tạo mới bộ dữ liệu liên quan đến tài khoản người dùng

Người dùng nhập dữ liệu để tạo tài khoản. Có thể có nhân viên nhập liệu (người quản lý) để tạo tài khoản hộ người dùng, với điều kiện người dùng cung cấp đầy đủ thông tin cá nhân theo yêu cầu tạo tài khoản.

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
   - **Sinh mật khẩu tự động**: Trong trường hợp người dùng không cung cấp mật khẩu, hệ thống sẽ tự động sinh mật khẩu kèm theo thông tin đăng nhập. Tính năng này thường được sử dụng khi nhân viên quản lý mở tài khoản mới cho người dùng.
   - **Thay đổi mật khẩu**: Người dùng có thể tự thay đổi mật khẩu đăng nhập của mình. Với trường hợp mật khẩu tự sinh, hệ thống sẽ kiểm tra và yêu cầu người dùng thay đổi mật khẩu mới trong lần đăng nhập đầu tiên.
   - **OTP (One Time Password)**: Hệ thống sử dụng OTP để bảo mật hai lớp, xác thực các hoạt động thay đổi thông tin quan trọng của người dùng. OTP cũng được sử dụng để xác thực các giao dịch quan trọng.
   - **Đăng nhập**: Khi người dùng đăng nhập, hệ thống sẽ kiểm tra thông tin đăng nhập trong dữ liệu đã lưu trữ để xác định tính hợp lệ. Nếu thông tin hợp lệ, người dùng sẽ được phép truy cập hệ thống và sử dụng các chức năng tương ứng.

Hãy đề xuất cách sử dụng OTP để xác nhận thực hiện giao dịch chuyển điểm giữa các ví. 
Hệ thống ghi nhận lại giao dịch này trong lịch sử giao dịch (transaction log) 
Báo cáo: hệ thống cho phép người dùng theo dõi số dư, lịch sử giao dịch (và trạng thái thực hiện giao dịch) 
