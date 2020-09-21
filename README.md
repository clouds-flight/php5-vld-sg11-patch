# php5-vld-sg11-patch
php vld 扩展 显示sg11解密后opcode补丁

sg11 php5 版的加密后直接获取的opcode其jmp等跳转 opcode 的地址和部分opcode 的 op1_type 、op2_type 是错的，sg11解密扩展覆盖了了opcode执行的handle，在其内部还原真实的opcode后再调用原始opcode handle执行，这里在所有opcode handle 函数内添加了代码，获取正确的opcode

1.克隆php目录下对应版本的php源码，使用zend_vm_execute.h替换php源码Zend目录下的zend_vm_execute.h

2.克隆vld扩展 last-for-php5版本，将vld_patch.c 和 srm_oparray_patch.c （如果是php5.3.29，则需拷贝vld-last-for-php5/vld_patch.c 、vld-last-for-php5/php5.3.29下的 branchinfo_patch.c和 srm_oparray_patch.c）

3.在vld扩展目录执行

  patch -p0 < vld_patch.c
  
  patch -p0 < srm_oparray_patch.c
  
  patch -p0 < branchinfo_patch.c （使用php5.3需执行）

4.编译安装php和vld扩展

5.需将php.ini vld扩展参数vld.execute设置为0时才有效
