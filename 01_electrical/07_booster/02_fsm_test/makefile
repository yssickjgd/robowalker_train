# 生成所有项目
all: demo_fsm

# 生成有限状态机项目
demo_fsm: ./alg_fsm.cpp ./drv_math.cpp ./main.cpp
	g++ -o ./main.exe ./alg_fsm.cpp ./drv_math.cpp ./main.cpp

# 删除编译中间文件
clean_o:
	del /s *.o

# 删除所有编译生成文件
clean:
	del /s *.o
	del /s *.exe