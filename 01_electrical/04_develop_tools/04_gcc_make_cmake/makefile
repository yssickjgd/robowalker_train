# 生成所有项目
all: demo_1 demo_2 demo_3 demo_4

# 2.1对应的项目
demo_1: ./1_test_c/test.c
	gcc -o ./1_test_c/test.exe ./1_test_c/test.c

# 2.2对应的项目
demo_2: ./2_test_cpp/test.cpp
	g++ -o ./2_test_cpp/test.exe ./2_test_cpp/test.cpp

# 2.3.1对应的项目
demo_1_step: ./1_test_c/test.c
	gcc -o test.o -c test.c
	gcc -o test.exe test.o

# 2.3.2对应的项目
demo_3: ./3_test_multi_c/test.c ./3_test_multi_c/function.c
	gcc -o ./3_test_multi_c/test.exe ./3_test_multi_c/test.c ./3_test_multi_c/function.c

# 2.3.3对应的项目
demo_4: ./4_test_multi_c_cpp/test.c ./4_test_multi_c_cpp/function.cpp
	g++ -o ./4_test_multi_c_cpp/test.exe ./4_test_multi_c_cpp/test.c ./4_test_multi_c_cpp/function.cpp

# 删除编译中间文件
clean_o:
	del /s *.o

# 删除所有编译生成文件
clean:
	del /s *.o
	del /s *.exe