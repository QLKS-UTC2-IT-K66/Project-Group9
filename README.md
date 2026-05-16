# 🏨 Hệ Thống Quản Lý Khách Sạn — MMLK TechHotel
**Môn học:** Kỹ Thuật Lập Trình (IT1.103.3)  
**GVHD:** ThS. Trần Thị Dung  
**Trường:** ĐH Giao Thông Vận Tải – Phân Hiệu Tại TP.HCM
---
## 👥 Thành viên nhóm 9
| STT |          Họ và tên           |   Vai trò   | Tiến độ |
|:---:|:----------------------------:|:-----------:|:-------:|
|  1  |    **Huỳnh Việt Minh**      | Trưởng nhóm | ✅ 100% |
|  2  |      **Đoàn Văn Lộc**       |  Phó nhóm   | ✅ 100% |
|  3  |    **Trần Minh Khôi**       | Thành viên  | ✅ 100% |
|  4  |  **Trần Trung Tuấn Minh**   | Thành viên  | ✅ 100% |
---
## 📌 Mô tả bài toán
Nhiều khách sạn vừa và nhỏ vẫn đang quản lý thủ công bằng sổ sách hoặc Excel — tiềm ẩn rủi ro mất dữ liệu và khó mở rộng. Dự án xây dựng một phần mềm quản lý khách sạn chạy trên **giao diện dòng lệnh (CLI)** bằng ngôn ngữ C, giải quyết đầy đủ các nghiệp vụ thực tế:
- Quản lý phòng (Room Management)
- Quản lý đặt phòng (Booking Management)
- Quản lý dịch vụ (Service Management)
- Quản lý thành viên (Membership)
- Lưu trữ dữ liệu bền vững
---
## ✨ Danh sách chức năng
| STT |           Module            |                        Chức năng chi tiết                         |
|:---:|:---------------------------:|:-----------------------------------------------------------------:|
|  1  |     🛏️ Quản lý phòng      |     Thêm, sửa, xóa, tìm kiếm, danh sách phòng trống, thống kê   |
|  2  |  🔑 Check-in / Check-out   |        Nhận khách, trả phòng, tạo và chốt hóa đơn tự động        |
|  3  |   🍽️ Quản lý dịch vụ     |   Hiển thị menu, thêm dịch vụ vào hóa đơn, xem dịch vụ đã dùng  |
|  4  |  🎫 Giảm giá & Thành viên  |    Hạng thẻ Bronze / Silver / Gold, mã khuyến mãi `WELCOME2026`  |
|  5  |  💳 Thanh toán & Hóa đơn   |      Xem hóa đơn theo phòng, xuất file, báo cáo doanh thu        |
|  6  |    💾 Lưu / Tải dữ liệu   |       Đọc/ghi file `.dat` tự động khi khởi động và thoát         |
|  7  |        📊 Thống kê         |         Biểu đồ phân bố loại phòng, tỉ lệ trống/có khách         |
---
## 🗂️ Cấu trúc thư mục
```
MMLK-TechHotel/
├── main.c                       # menu chính, điều hướng chương trình
├── model.h                      # khai báo struct & hằng số dùng chung
├── ui.h                         # macro màu ANSI giao diện
├── room.c / room.h              # module quản lý phòng
├── booking.c / booking.h        # module check-in / check-out
├── billing.c / billing.h        # module hóa đơn & thanh toán
├── service.c / service.h        # module dịch vụ
├── membership.c / membership.h  # module thành viên & giảm giá
├── file.c / file.h              # module đọc/ghi file
├── Data/
│   ├── phong.dat                # dữ liệu phòng (binary)
│   └── hoadon.dat               # dữ liệu hóa đơn (binary)
└── Makefile
```
Dự án tổ chức theo mô hình **Modular Programming** — mỗi chức năng tách thành cặp `.c` / `.h` riêng biệt, các thành viên làm việc song song mà không xung đột.
---
## ⚙️ Kỹ thuật áp dụng
Dự án vận dụng kiến thức từ đủ 5 chương của môn học:
**Chương 2 — Hàm & module**  
Chương trình chia thành 7 module độc lập. Mỗi module có file `.h` khai báo hàm và file `.c` cài đặt logic. Sử dụng hàm trả về giá trị, hàm tham biến con trỏ và hàm đệ quy.
**Chương 2–3 — Con trỏ & bộ nhớ động**  
Danh sách phòng dùng linked list cấp phát động bằng `malloc()`. Toàn bộ node được giải phóng bằng `freeRoomList()` khi thoát — không rò rỉ bộ nhớ.
```c
RoomNode *p = (RoomNode*)malloc(sizeof(RoomNode));
if (!p) return NULL;
p->data = r;
p->next = NULL;
```
**Chương 3 — Danh sách liên kết đơn**  
Mỗi phòng là một `RoomNode` trong danh sách liên kết. Hỗ trợ đầy đủ: thêm (`addRoom`), xóa (`deleteRoom`), sửa (`editRoom`), tìm kiếm (`findRoom`).
```c
typedef struct RoomNode {
    Room data;
    struct RoomNode *next;
} RoomNode;
```
**Chương 4 — Thao tác tệp tin**  
Dùng `fwrite()` / `fread()` ghi thẳng struct ra file nhị phân. Kiểm tra `fopen()` trước khi thao tác, đóng file sau khi dùng. Dữ liệu tự động tải khi khởi động và lưu khi thoát.
```c
fwrite(&cur->data, sizeof(Room), 1, fp);  
fread(&r, sizeof(Room), 1, fp);           
```
**Chương 5 — Xử lý lỗi & tiền xử lý**  
Toàn bộ đầu vào người dùng được validate qua `scanf()`. Macro ANSI định nghĩa tập trung trong `ui.h`. Tất cả file header có `#ifndef` guard chống include trùng.
```c
#define SUCCESS_STYLE  "\033[32m"    
#define ERROR_STYLE    "\033[31m"    
#define MONEY_STYLE    "\033[33;1m"  
```
---
## 🚀 Hướng dẫn cài đặt & chạy
**Yêu cầu môi trường:**
- Hệ điều hành: Windows 10/11
- Trình biên dịch: GCC MinGW-w64 (chuẩn C99)
- Terminal: hỗ trợ UTF-8 (Windows Terminal hoặc PowerShell)
```bash
# 1. Clone repository
git clone https://github.com/<your-username>/MMLK-TechHotel.git
cd MMLK-TechHotel
# 2. Biên dịch
mingw32-make run
# 3. Chạy chương trình
./qlks.exe
```
> ⚠️ Khi thoát, **bắt buộc nhấn `0`** tại menu chính để chương trình lưu dữ liệu và giải phóng bộ nhớ đúng cách.
---
## 🧪 Kiểm thử
13/13 test case đều pass:
|            Tình huống             |                  Kết quả mong đợi                   | Trạng thái |
|:---------------------------------:|:---------------------------------------------------:|:----------:|
|      Nhập `abc` vào ô số          | Bắt lỗi `scanf`, xả buffer, không loop vô hạn       |  ✅ Pass   |
| Chọn menu ngoài phạm vi (`9`,`-1`)| Hiện cảnh báo, yêu cầu nhập lại                     |  ✅ Pass   |
| Check-in phòng đang có khách      | Từ chối thao tác, hiển thị cảnh báo                 |  ✅ Pass   |
| Check-out phòng đang trống        | Thông báo lỗi, hủy giao dịch                        |  ✅ Pass   |
| Gọi dịch vụ cho phòng trống       | Kiểm tra `STATUS_OCCUPIED`, hủy thao tác            |  ✅ Pass   |
| Thoát qua phím `0`                | Lưu file + gọi `freeRoomList()`, thoát an toàn      |  ✅ Pass   |
---
## 🔗 Phụ lục
- 📁 **GitHub repository:** https://github.com/QLKS-UTC2-IT-K66/Project-Group9
- 🎬 **Video demo:** https://www.tiktok.com/@toshi1475/video/7639976554700852487?is_from_webapp=1&web_id=7557697204464453137
- 📚 **Thư viện C sử dụng:** `stdio.h`, `stdlib.h`, `string.h`, `time.h`, `windows.h`
---
*Nhóm 9 · Kỹ Thuật Lập Trình (IT1.103.3) · ĐH GTVT TP.HCM · 2025*
