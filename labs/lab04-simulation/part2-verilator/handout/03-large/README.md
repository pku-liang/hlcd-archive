# clone项目并打patch

```bash
git clone https://github.com/chipsalliance/Cores-SweRV.git
cd Cores-SweRV
git apply ../fix-makefile.patch
```

# 文件结构分析

```bash
Cores-SweRV
├── configs
├── design              # design 文件
├── testbench
│   ├── flist           # file list
│   ├── tb_top.sv       # verilog testbench 文件
│   └── test_tb_top.cpp # cpp testbench 文件
└── tools
```

file list 文件存了 design 的所有源文件，将作为 verilator 的输入：

```bash
$ head testbench/flist
+incdir+$RV_ROOT/testbench
$RV_ROOT/design/swerv_wrapper.sv
$RV_ROOT/design/mem.sv
$RV_ROOT/design/pic_ctrl.sv
$RV_ROOT/design/swerv.sv
$RV_ROOT/design/dma_ctrl.sv
$RV_ROOT/design/ifu/ifu_aln_ctl.sv
$RV_ROOT/design/ifu/ifu_compress_ctl.sv
$RV_ROOT/design/ifu/ifu_ifc_ctl.sv
$RV_ROOT/design/ifu/ifu_bp_ctl.sv
```

检查一下源代码有多长，估计项目的规模

```bash
apt install cloc
cloc ./design/
```

# 运行仿真

接下来，我们分别测试多线程仿真、gcc/clang、不同优化命令下仿真速度的区别，包括
* `-threads 1`, `-threads 4`
* `CXX=g++`, `CXX=clang++`
* `CFLAGS=-O2`, `CFLAGS=-Ofast` 

按照项目默认的配置运行：

```bash
export RV_ROOT=`pwd`
cd tools
make verilator-build
ln -sf ../testbench/hex/cmark.hex program.hex
time make verilator
```

修改C编译器：
```bash
make verilator-build CXX=clang++
```

修改threads：
```bash
make verilator-build VERILATOR="verilator -threads=4"
```

修改CFLAGS:
```bash
make verilator-build CFLAGS=-Ofast
```
