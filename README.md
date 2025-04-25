### A. Các chức năng:

Hệ thống cung cấp các chức năng chính như sau:

1. **Tạo mới tài khoản khi đăng ký**: tạo mới bộ dữ liệu liên quan đến tài khoản người dùng

Người dùng nhập dữ liệu để tạo tài khoản. Có thể có nhân viên nhập liệu (người quản lý) để tạo tài khoản hộ người dùng, với điều kiện người dùng cung cấp đầy đủ thông tin cá nhân theo yêu cầu tạo tài khoản.

Hãy phân tích và đề xuất một cấu trúc dữ liệu (lớp) quản lý thông tin tài khoản người dùng.

-> Nhóm tôi đã sử dụng việc lưu các trường của lớp UserAccount vào trong file user_account.dat (password đã được hash)


2. **Lưu trữ dữ liệu**:  lưu lại dữ liệu tài khoản người dùng xuống tập tin.

    Hãy đề xuất việc lưu trữ dữ liệu người dùng dưới dạng tập tin riêng cho từng người dùng hay một tập tin cho tất cả người dùng? Nêu lý do lựa chọn giải pháp.

    Với mật khẩu, hãy tìm hiểu cách lưu mật khẩu của người dùng vào CSDL. Gợi ý: dùng hàm băm (hash function).

    Mọi lưu trữ phải có bản sao lưu (backup). Hãy tìm hiểu quy trình sao lưu - phục hồi dữ liệu. Đề xuất giải pháp sao lưu cho dữ liệu người dùng
    

3. **Quản lý đăng nhập**:
   - **Sinh mật khẩu tự động**: Trong trường hợp người dùng không cung cấp mật khẩu, hệ thống sẽ tự động sinh mật khẩu kèm theo thông tin đăng nhập. Tính năng này thường được sử dụng khi nhân viên quản lý mở tài khoản mới cho người dùng.
   - **Thay đổi mật khẩu**: Người dùng có thể tự thay đổi mật khẩu đăng nhập của mình. Với trường hợp mật khẩu tự sinh, hệ thống sẽ kiểm tra và yêu cầu người dùng thay đổi mật khẩu mới trong lần đăng nhập đầu tiên.
   - **OTP (One Time Password)**: Hệ thống sử dụng OTP để bảo mật hai lớp, xác thực các hoạt động thay đổi thông tin quan trọng của người dùng. OTP cũng được sử dụng để xác thực các giao dịch quan trọng.
   - **Đăng nhập**: Khi người dùng đăng nhập, hệ thống sẽ kiểm tra thông tin đăng nhập trong dữ liệu đã lưu trữ để xác định tính hợp lệ. Nếu thông tin hợp lệ, người dùng sẽ được phép truy cập hệ thống và sử dụng các chức năng tương ứng.

Hệ thống được thiết kế để đảm bảo tính bảo mật, dễ sử dụng và khả năng mở rộng trong tương lai.