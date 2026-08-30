g++ X.cpp -Wall -Wshadow -Wextra -Wconversion -O2 \
  -fsanitize=undefined -fsanitize=address
# -fsanitize=undefined: 检测未定义行为
# -fsanitize=address: 检测内存溢出
