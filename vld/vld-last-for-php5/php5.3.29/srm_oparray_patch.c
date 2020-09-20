--- srm_oparray.c	2017-10-16 00:05:17.000000000 +0800
+++ srm_oparray_modify.c	2020-09-21 01:53:20.000000000 +0800
@@ -670,6 +670,7 @@ static unsigned int vld_get_special_flag
 #if PHP_VERSION_ID >= 50000
 		case ZEND_CATCH:
 			flags = ALL_USED;
+#if PHP_VERSION_ID >= 50400
 			if (!op->result.num) {
 #if PHP_VERSION_ID >= 70100
 				flags |= EXT_VAL_JMP_REL;
@@ -679,6 +680,7 @@ static unsigned int vld_get_special_flag
 			}
 			break;
 #endif
+#endif
 	}
 	return flags;
 }
@@ -693,9 +695,9 @@ void vld_dump_op(int nr, zend_op * op_pt
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
