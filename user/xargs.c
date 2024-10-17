#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"  

#define is_blank(chr) (chr == ' ' || chr == '\t') 

int
main(int argc, char *argv[])
{
	char buf[2048], ch; // Mảng buf để lưu trữ dữ liệu đầu vào, biến ch để đọc từng ký tự
	char *p = buf; // Trỏ đến vị trí đầu mảng buf --> Quản lý vị trí ghi
	char *v[MAXARG]; // Mảng con trỏ để lưu trữ các chuỗi đối số và dữ liệu từ stdin
	int c; // Đếm số lượng đối số hoặc từ được lưu vào mảng
	int blanks = 0; // Đếm số khoảng trắng giữa các từ
	int offset = 0; // Vị trí hiện tại trong mảng buf khi thêm ký tự vào

	// Nếu số lượng cú pháp truyền vào không đúng thì xuất ra lỗi
	if(argc <= 1){
		fprintf(2, "usage: xargs <command> [argv...]\n");
		exit(1);
	}

	// Ta lấy từng chữ lưu vào mảng v
	for (c = 1; c < argc; c++) {
		v[c-1] = argv[c];
	}
	--c;

	// Vòng lặp để đọc từng ký tự 
	while (read(0, &ch, 1) > 0) {
		// Nếu kí tự đó là khoảng trắng thì tăng biến đếm khoảng trắng
		if (is_blank(ch)) {
			blanks++;
			continue;
		}

		// Nếu gặp khoảng trắng trước đó thì nghĩa là vừa đi qua một từ
		if (blanks) { 
			//Đặt giá trị  \0 vào cuối từ trước đó trong mảng
			buf[offset++] = 0;

			v[c++] = p; // Lưu địa chỉ của từ vừa hoàn thành vào mảng p
			p = buf + offset; // Cập nhật địa chỉ con trỏ p để chuẩn bị cho từ tiếp theo

			// Reset lại = 0 để tiếp tục từ tiếp theo
			blanks = 0;
		}

		// Nếu kí tự không phải dấu xuống dòng, lưu nó vào buff
		if (ch != '\n') {
			buf[offset++] = ch; // Thêm kí tự vào buf và tăng offset
		} else { // Nếu gặp kí tự xuống dòng nghĩa là xong lệnh
			v[c++] = p; // Lưu địa chỉ của từ cuối cùng vào mảng v
			p = buf + offset; // Cập nhật vị trí con trỏ p cho lần đọc tiếp theo

			if (!fork()) {
				exit(exec(v[0], v));
			}
			wait(0);
			
			c = argc - 1;
		}
	}

	exit(0);
}