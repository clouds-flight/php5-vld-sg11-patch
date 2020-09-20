--- branchinfo.c	2017-10-16 00:05:17.000000000 +0800
+++ branchinfo_modify.c	2020-09-21 01:54:56.000000000 +0800
@@ -69,7 +69,7 @@ void vld_only_leave_first_catch(zend_op_
 	if (opa->opcodes[position].opcode != ZEND_CATCH) {
 		return;
 	}
-
+#if PHP_VERSION_ID >= 50400
 	if (!opa->opcodes[position].result.num) {
 #if PHP_VERSION_ID >= 70100
 		exit_jmp = position + ((signed int) opa->opcodes[position].extended_value / sizeof(zend_op));
@@ -84,6 +84,7 @@ void vld_only_leave_first_catch(zend_op_
 			vld_only_leave_first_catch(opa, branch_info, exit_jmp);
 		}
 	}
+#endif
 
 	vld_set_remove(branch_info->entry_points, position);
 }
