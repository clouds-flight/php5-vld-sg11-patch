--- srm_oparray.c	2017-10-16 00:05:17.000000000 +0800
+++ srm_oparray_modify.c	2020-09-20 16:34:28.757353528 +0800
@@ -693,9 +693,9 @@ void vld_dump_op(int nr, zend_op * op_pt
 	unsigned int flags, op1_type, op2_type, res_type;
 	const zend_op op = op_ptr[nr];
 	
-	if (op.lineno == 0) {
-		return;
-	}
+	// if (op.lineno == 0) {
+	// 	return;
+	// }
 
 	if (op.opcode >= NUM_KNOWN_OPCODES) {
 		flags = ALL_USED;
