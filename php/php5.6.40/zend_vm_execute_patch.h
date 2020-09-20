--- zend_vm_execute.h	2019-01-09 17:54:13.000000000 +0800
+++ zend_vm_execute_modify.h	2020-09-20 17:24:32.124781897 +0800
@@ -334,8 +334,38 @@ static opcode_handler_t zend_vm_get_opco
 
 #define ZEND_OPCODE_HANDLER_ARGS_PASSTHRU_INTERNAL execute_data TSRMLS_CC
 
+
+ZEND_API  int (*vld_execute_time)();
+ZEND_API  zend_execute_data * (*vld_get_execute_data)(zend_op_array * op_array);
+ZEND_API  zend_op (*vld_get_op)();
+
+zend_op src_op;
+
+static zend_op get_src_op()
+{
+	return src_op;
+}
+
+static void deal_vld(zend_execute_data * execute_data, unsigned char op1_type,unsigned char op2_type)
+{
+	USE_OPLINE
+	src_op=*opline;
+	src_op.op1_type=op1_type;
+	src_op.op2_type=op2_type;
+    return;
+}
+
+static zend_execute_data * create_execute_data(zend_op_array * op_array)
+{
+	return i_create_execute_data_from_op_array(op_array, 0 TSRMLS_CC);
+}
+
 ZEND_API void execute_ex(zend_execute_data *execute_data TSRMLS_DC)
 {
+
+	vld_get_execute_data=create_execute_data;
+	vld_get_op=get_src_op;
+
 	DCL_OPLINE
 	zend_bool original_in_execution;
 
@@ -390,6 +420,13 @@ ZEND_API void zend_execute(zend_op_array
 
 static int ZEND_FASTCALL zend_leave_helper_SPEC(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	zend_bool nested = EX(nested);
 	zend_op_array *op_array = EX(op_array);
 
@@ -478,6 +515,13 @@ static int ZEND_FASTCALL zend_leave_help
 
 static int ZEND_FASTCALL zend_do_fcall_common_helper_SPEC(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_bool should_change_scope = 0;
 	zend_function *fbc = EX(function_state).function;
@@ -662,6 +706,13 @@ static int ZEND_FASTCALL zend_do_fcall_c
 
 static int ZEND_FASTCALL  ZEND_JMP_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 #if DEBUG_ZEND>=2
@@ -673,6 +724,13 @@ static int ZEND_FASTCALL  ZEND_JMP_SPEC_
 
 static int ZEND_FASTCALL  ZEND_INIT_STRING_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *tmp = &EX_T(opline->result.var).tmp_var;
 
@@ -689,12 +747,26 @@ static int ZEND_FASTCALL  ZEND_INIT_STRI
 
 static int ZEND_FASTCALL  ZEND_DO_FCALL_BY_NAME_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	EX(function_state).function = EX(call)->fbc;
 	return zend_do_fcall_common_helper_SPEC(ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_GENERATOR_RETURN_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	/* The generator object is stored in return_value_ptr_ptr */
 	zend_generator *generator = (zend_generator *) EG(return_value_ptr_ptr);
 
@@ -707,6 +779,13 @@ static int ZEND_FASTCALL  ZEND_GENERATOR
 
 static int ZEND_FASTCALL  ZEND_SEND_UNPACK_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *args;
@@ -855,6 +934,13 @@ unpack_iter_dtor:
 
 static int ZEND_FASTCALL  ZEND_RECV_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_uint arg_num = opline->op1.num;
 	zval **param = zend_vm_stack_get_arg(arg_num TSRMLS_CC);
@@ -896,6 +982,13 @@ static int ZEND_FASTCALL  ZEND_RECV_SPEC
 
 static int ZEND_FASTCALL  ZEND_RECV_VARIADIC_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_uint arg_num = opline->op1.num;
 	zend_uint arg_count = zend_vm_stack_get_args_count(TSRMLS_C);
@@ -927,6 +1020,13 @@ static int ZEND_FASTCALL  ZEND_RECV_VARI
 
 static int ZEND_FASTCALL  ZEND_NEW_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *object_zval;
 	zend_function *constructor;
@@ -978,6 +1078,13 @@ static int ZEND_FASTCALL  ZEND_NEW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_BEGIN_SILENCE_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -1019,6 +1126,13 @@ static int ZEND_FASTCALL  ZEND_BEGIN_SIL
 
 static int ZEND_FASTCALL  ZEND_RAISE_ABSTRACT_ERROR_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	SAVE_OPLINE();
 	zend_error_noreturn(E_ERROR, "Cannot call abstract method %s::%s()", EG(scope)->name, EX(op_array)->function_name);
 	ZEND_VM_NEXT_OPCODE(); /* Never reached */
@@ -1026,6 +1140,13 @@ static int ZEND_FASTCALL  ZEND_RAISE_ABS
 
 static int ZEND_FASTCALL  ZEND_EXT_STMT_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	SAVE_OPLINE();
 	if (!EG(no_extensions)) {
 		zend_llist_apply_with_argument(&zend_extensions, (llist_apply_with_arg_func_t) zend_extension_statement_handler, EX(op_array) TSRMLS_CC);
@@ -1036,6 +1157,13 @@ static int ZEND_FASTCALL  ZEND_EXT_STMT_
 
 static int ZEND_FASTCALL  ZEND_EXT_FCALL_BEGIN_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	SAVE_OPLINE();
 	if (!EG(no_extensions)) {
 		zend_llist_apply_with_argument(&zend_extensions, (llist_apply_with_arg_func_t) zend_extension_fcall_begin_handler, EX(op_array) TSRMLS_CC);
@@ -1046,6 +1174,13 @@ static int ZEND_FASTCALL  ZEND_EXT_FCALL
 
 static int ZEND_FASTCALL  ZEND_EXT_FCALL_END_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	SAVE_OPLINE();
 	if (!EG(no_extensions)) {
 		zend_llist_apply_with_argument(&zend_extensions, (llist_apply_with_arg_func_t) zend_extension_fcall_end_handler, EX(op_array) TSRMLS_CC);
@@ -1056,6 +1191,13 @@ static int ZEND_FASTCALL  ZEND_EXT_FCALL
 
 static int ZEND_FASTCALL  ZEND_DECLARE_CLASS_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -1066,6 +1208,13 @@ static int ZEND_FASTCALL  ZEND_DECLARE_C
 
 static int ZEND_FASTCALL  ZEND_DECLARE_INHERITED_CLASS_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -1076,6 +1225,13 @@ static int ZEND_FASTCALL  ZEND_DECLARE_I
 
 static int ZEND_FASTCALL  ZEND_DECLARE_INHERITED_CLASS_DELAYED_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_class_entry **pce, **pce_orig;
 
@@ -1091,6 +1247,13 @@ static int ZEND_FASTCALL  ZEND_DECLARE_I
 
 static int ZEND_FASTCALL  ZEND_DECLARE_FUNCTION_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -1101,6 +1264,13 @@ static int ZEND_FASTCALL  ZEND_DECLARE_F
 
 static int ZEND_FASTCALL  ZEND_TICKS_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -1116,16 +1286,37 @@ static int ZEND_FASTCALL  ZEND_TICKS_SPE
 
 static int ZEND_FASTCALL  ZEND_EXT_NOP_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	ZEND_VM_NEXT_OPCODE();
 }
 
 static int ZEND_FASTCALL  ZEND_NOP_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	ZEND_VM_NEXT_OPCODE();
 }
 
 static int ZEND_FASTCALL  ZEND_ADD_TRAIT_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_class_entry *ce = EX_T(opline->op1.var).class_entry;
 	zend_class_entry *trait;
@@ -1156,6 +1347,13 @@ static int ZEND_FASTCALL  ZEND_ADD_TRAIT
 
 static int ZEND_FASTCALL  ZEND_BIND_TRAITS_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_class_entry *ce = EX_T(opline->op1.var).class_entry;
 
@@ -1167,6 +1365,13 @@ static int ZEND_FASTCALL  ZEND_BIND_TRAI
 
 static int ZEND_FASTCALL  ZEND_HANDLE_EXCEPTION_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	zend_uint op_num = EG(opline_before_exception)-EG(active_op_array)->opcodes;
 	int i;
 	zend_uint catch_op_num = 0, finally_op_num = 0, finally_op_end = 0;
@@ -1296,6 +1501,13 @@ static int ZEND_FASTCALL  ZEND_HANDLE_EX
 
 static int ZEND_FASTCALL  ZEND_VERIFY_ABSTRACT_CLASS_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -1306,6 +1518,13 @@ static int ZEND_FASTCALL  ZEND_VERIFY_AB
 
 static int ZEND_FASTCALL  ZEND_USER_OPCODE_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	int ret;
 
@@ -1335,6 +1554,13 @@ static int ZEND_FASTCALL  ZEND_USER_OPCO
 
 static int ZEND_FASTCALL  ZEND_DISCARD_EXCEPTION_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	if (EX(delayed_exception) != NULL) {
 		/* discard the previously thrown exception */
 		zval_ptr_dtor(&EX(delayed_exception));
@@ -1346,6 +1572,13 @@ static int ZEND_FASTCALL  ZEND_DISCARD_E
 
 static int ZEND_FASTCALL  ZEND_FAST_CALL_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if ((opline->extended_value & ZEND_FAST_CALL_FROM_CATCH) &&
@@ -1365,6 +1598,13 @@ static int ZEND_FASTCALL  ZEND_FAST_CALL
 
 static int ZEND_FASTCALL  ZEND_FAST_RET_SPEC_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,opline->op2_type);
+		return 0;
+	}
+
 	if (EX(fast_ret)) {
 		ZEND_VM_SET_OPCODE(EX(fast_ret) + 1);
 		if ((EX(fast_ret)->extended_value & ZEND_FAST_CALL_FROM_FINALLY)) {
@@ -1395,6 +1635,13 @@ static int ZEND_FASTCALL  ZEND_FAST_RET_
 
 static int ZEND_FASTCALL  ZEND_FETCH_CLASS_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -1434,6 +1681,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_CLA
 
 static int ZEND_FASTCALL  ZEND_INIT_FCALL_BY_NAME_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	call_slot *call = EX(call_slots) + opline->result.num;
@@ -1591,6 +1845,13 @@ static int ZEND_FASTCALL  ZEND_INIT_FCAL
 
 static int ZEND_FASTCALL  ZEND_INIT_NS_FCALL_BY_NAME_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_literal *func_name;
 	call_slot *call = EX(call_slots) + opline->result.num;
@@ -1621,6 +1882,13 @@ static int ZEND_FASTCALL  ZEND_INIT_NS_F
 
 static int ZEND_FASTCALL  ZEND_RECV_INIT_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *assignment_value;
 	zend_uint arg_num = opline->op1.num;
@@ -1661,6 +1929,13 @@ static int ZEND_FASTCALL  ZEND_RECV_INIT
 
 static int ZEND_FASTCALL  ZEND_BRK_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_brk_cont_element *el;
 
@@ -1672,6 +1947,13 @@ static int ZEND_FASTCALL  ZEND_BRK_SPEC_
 
 static int ZEND_FASTCALL  ZEND_CONT_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_brk_cont_element *el;
 
@@ -1683,6 +1965,13 @@ static int ZEND_FASTCALL  ZEND_CONT_SPEC
 
 static int ZEND_FASTCALL  ZEND_GOTO_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_CONST);
+		return 0;
+	}
+
 	zend_op *brk_opline;
 	USE_OPLINE
 	zend_brk_cont_element *el;
@@ -1710,6 +1999,13 @@ static int ZEND_FASTCALL  ZEND_GOTO_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_INTERFACE_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_class_entry *ce = EX_T(opline->op1.var).class_entry;
 	zend_class_entry *iface;
@@ -1737,6 +2033,13 @@ static int ZEND_FASTCALL  ZEND_ADD_INTER
 
 static int ZEND_FASTCALL  ZEND_FETCH_CLASS_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -1777,6 +2080,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_CLA
 
 static int ZEND_FASTCALL  ZEND_INIT_FCALL_BY_NAME_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	call_slot *call = EX(call_slots) + opline->result.num;
@@ -1936,6 +2246,13 @@ static int ZEND_FASTCALL  ZEND_INIT_FCAL
 
 static int ZEND_FASTCALL  ZEND_FETCH_CLASS_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -1976,6 +2293,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_CLA
 
 static int ZEND_FASTCALL  ZEND_INIT_FCALL_BY_NAME_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	call_slot *call = EX(call_slots) + opline->result.num;
@@ -2135,6 +2459,13 @@ static int ZEND_FASTCALL  ZEND_INIT_FCAL
 
 static int ZEND_FASTCALL  ZEND_FETCH_CLASS_SPEC_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -2174,6 +2505,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_CLA
 
 static int ZEND_FASTCALL  ZEND_FETCH_CLASS_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -2213,6 +2551,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_CLA
 
 static int ZEND_FASTCALL  ZEND_INIT_FCALL_BY_NAME_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,opline->op1_type,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	call_slot *call = EX(call_slots) + opline->result.num;
@@ -2370,6 +2715,13 @@ static int ZEND_FASTCALL  ZEND_INIT_FCAL
 
 static int ZEND_FASTCALL  ZEND_BW_NOT_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -2383,6 +2735,13 @@ static int ZEND_FASTCALL  ZEND_BW_NOT_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_NOT_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -2396,6 +2755,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_NOT_
 
 static int ZEND_FASTCALL  ZEND_ECHO_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *z;
@@ -2414,6 +2780,13 @@ static int ZEND_FASTCALL  ZEND_ECHO_SPEC
 
 static int ZEND_FASTCALL  ZEND_PRINT_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	ZVAL_LONG(&EX_T(opline->result.var).tmp_var, 1);
@@ -2422,6 +2795,13 @@ static int ZEND_FASTCALL  ZEND_PRINT_SPE
 
 static int ZEND_FASTCALL  ZEND_JMPZ_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *val;
@@ -2452,6 +2832,13 @@ static int ZEND_FASTCALL  ZEND_JMPZ_SPEC
 
 static int ZEND_FASTCALL  ZEND_JMPNZ_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *val;
@@ -2482,6 +2869,13 @@ static int ZEND_FASTCALL  ZEND_JMPNZ_SPE
 
 static int ZEND_FASTCALL  ZEND_JMPZNZ_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *val;
@@ -2516,6 +2910,13 @@ static int ZEND_FASTCALL  ZEND_JMPZNZ_SP
 
 static int ZEND_FASTCALL  ZEND_JMPZ_EX_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *val;
@@ -2547,6 +2948,13 @@ static int ZEND_FASTCALL  ZEND_JMPZ_EX_S
 
 static int ZEND_FASTCALL  ZEND_JMPNZ_EX_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *val;
@@ -2578,6 +2986,13 @@ static int ZEND_FASTCALL  ZEND_JMPNZ_EX_
 
 static int ZEND_FASTCALL  ZEND_DO_FCALL_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *fname = opline->op1.zv;
@@ -2604,6 +3019,13 @@ static int ZEND_FASTCALL  ZEND_DO_FCALL_
 
 static int ZEND_FASTCALL  ZEND_RETURN_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *retval_ptr;
 
@@ -2647,6 +3069,13 @@ static int ZEND_FASTCALL  ZEND_RETURN_SP
 
 static int ZEND_FASTCALL  ZEND_RETURN_BY_REF_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *retval_ptr;
 	zval **retval_ptr_ptr;
@@ -2718,6 +3147,13 @@ static int ZEND_FASTCALL  ZEND_RETURN_BY
 
 static int ZEND_FASTCALL  ZEND_THROW_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *value;
 	zval *exception;
@@ -2749,6 +3185,13 @@ static int ZEND_FASTCALL  ZEND_THROW_SPE
 
 static int ZEND_FASTCALL  ZEND_SEND_VAL_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -2779,6 +3222,13 @@ static int ZEND_FASTCALL  ZEND_SEND_VAL_
 
 static int ZEND_FASTCALL  ZEND_BOOL_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *retval = &EX_T(opline->result.var).tmp_var;
@@ -2793,6 +3243,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_SPEC
 
 static int ZEND_FASTCALL  ZEND_CLONE_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *obj;
@@ -2859,6 +3316,13 @@ static int ZEND_FASTCALL  ZEND_CLONE_SPE
 
 static int ZEND_FASTCALL  ZEND_CAST_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *expr;
@@ -2918,6 +3382,13 @@ static int ZEND_FASTCALL  ZEND_CAST_SPEC
 
 static int ZEND_FASTCALL  ZEND_INCLUDE_OR_EVAL_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_op_array *new_op_array=NULL;
 
@@ -3051,6 +3522,13 @@ static int ZEND_FASTCALL  ZEND_INCLUDE_O
 
 static int ZEND_FASTCALL  ZEND_FE_RESET_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *array_ptr, **array_ptr_ptr;
@@ -3213,6 +3691,13 @@ static int ZEND_FASTCALL  ZEND_FE_RESET_
 
 static int ZEND_FASTCALL  ZEND_EXIT_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 #if 0 || (IS_CONST != IS_UNUSED)
 	USE_OPLINE
 
@@ -3235,6 +3720,13 @@ static int ZEND_FASTCALL  ZEND_EXIT_SPEC
 
 static int ZEND_FASTCALL  ZEND_JMP_SET_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *value;
@@ -3260,6 +3752,13 @@ static int ZEND_FASTCALL  ZEND_JMP_SET_S
 
 static int ZEND_FASTCALL  ZEND_JMP_SET_VAR_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *value, *ret;
@@ -3294,6 +3793,13 @@ static int ZEND_FASTCALL  ZEND_JMP_SET_V
 
 static int ZEND_FASTCALL  ZEND_QM_ASSIGN_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *value;
@@ -3312,6 +3818,13 @@ static int ZEND_FASTCALL  ZEND_QM_ASSIGN
 
 static int ZEND_FASTCALL  ZEND_QM_ASSIGN_VAR_SPEC_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *value, *ret;
@@ -3339,6 +3852,13 @@ static int ZEND_FASTCALL  ZEND_QM_ASSIGN
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -3354,6 +3874,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -3369,6 +3896,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -3384,6 +3918,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -3399,6 +3940,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -3414,6 +3962,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -3429,6 +3984,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -3444,6 +4006,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -3459,6 +4028,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -3474,6 +4050,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -3491,6 +4074,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -3507,6 +4097,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -3523,6 +4120,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -3539,6 +4143,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -3555,6 +4166,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -3570,6 +4188,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -3585,6 +4210,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -3600,6 +4232,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -3615,6 +4254,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL zend_fetch_var_address_helper_SPEC_CONST_CONST(int type, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *varname;
@@ -3745,21 +4391,49 @@ static int ZEND_FASTCALL zend_fetch_var_
 
 static int ZEND_FASTCALL  ZEND_FETCH_R_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_CONST(BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_W_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_CONST(BP_VAR_W, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_RW_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_CONST(BP_VAR_RW, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_FUNC_ARG_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	return zend_fetch_var_address_helper_SPEC_CONST_CONST(zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC) ? BP_VAR_W : BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
@@ -3767,16 +4441,37 @@ static int ZEND_FASTCALL  ZEND_FETCH_FUN
 
 static int ZEND_FASTCALL  ZEND_FETCH_UNSET_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_CONST(BP_VAR_UNSET, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_IS_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_CONST(BP_VAR_IS, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -3794,6 +4489,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_TMP_VAR_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -3818,6 +4520,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_INIT_STATIC_METHOD_CALL_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	zend_class_entry *ce;
@@ -3943,6 +4652,13 @@ static int ZEND_FASTCALL  ZEND_INIT_STAT
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -3957,6 +4673,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_FETCH_CONSTANT_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -4049,6 +4772,13 @@ constant_fetch_end:
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *expr_ptr;
@@ -4133,6 +4863,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -4147,6 +4884,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_VAR_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval tmp, *varname;
 	HashTable *target_symbol_table;
@@ -4225,6 +4969,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_VAR
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_VAR_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval **value;
 	zend_bool isset = 1;
@@ -4310,6 +5061,13 @@ static int ZEND_FASTCALL  ZEND_ISSET_ISE
 
 static int ZEND_FASTCALL  ZEND_DECLARE_CONST_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *name;
@@ -4356,6 +5114,13 @@ static int ZEND_FASTCALL  ZEND_DECLARE_C
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -4515,6 +5280,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_CONST_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -4530,6 +5302,13 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -4545,6 +5324,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -4560,6 +5346,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -4575,6 +5368,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -4590,6 +5390,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -4605,6 +5412,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -4620,6 +5434,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -4635,6 +5456,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -4650,6 +5478,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -4665,6 +5500,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -4682,6 +5524,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -4698,6 +5547,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -4714,6 +5570,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -4730,6 +5593,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -4746,6 +5616,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -4761,6 +5638,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -4776,6 +5660,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -4791,6 +5682,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -4806,6 +5704,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *container;
@@ -4823,6 +5728,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_INIT_STATIC_METHOD_CALL_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	zend_class_entry *ce;
@@ -4948,6 +5860,13 @@ static int ZEND_FASTCALL  ZEND_INIT_STAT
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -4963,6 +5882,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *expr_ptr;
@@ -5047,6 +5973,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -5061,6 +5994,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -5220,6 +6160,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_CONST_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5235,6 +6182,13 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5250,6 +6204,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5265,6 +6226,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5280,6 +6248,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5295,6 +6270,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5310,6 +6292,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5325,6 +6314,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5340,6 +6336,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5355,6 +6358,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5370,6 +6380,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -5387,6 +6404,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -5403,6 +6427,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -5419,6 +6450,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -5435,6 +6473,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -5451,6 +6496,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5466,6 +6518,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5481,6 +6540,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5496,6 +6562,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5511,6 +6584,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL zend_fetch_var_address_helper_SPEC_CONST_VAR(int type, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *varname;
@@ -5641,21 +6721,49 @@ static int ZEND_FASTCALL zend_fetch_var_
 
 static int ZEND_FASTCALL  ZEND_FETCH_R_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_VAR(BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_W_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_VAR(BP_VAR_W, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_RW_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_VAR(BP_VAR_RW, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_FUNC_ARG_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	return zend_fetch_var_address_helper_SPEC_CONST_VAR(zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC) ? BP_VAR_W : BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
@@ -5663,16 +6771,37 @@ static int ZEND_FASTCALL  ZEND_FETCH_FUN
 
 static int ZEND_FASTCALL  ZEND_FETCH_UNSET_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_VAR(BP_VAR_UNSET, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_IS_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_VAR(BP_VAR_IS, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *container;
@@ -5690,6 +6819,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_INIT_STATIC_METHOD_CALL_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	zend_class_entry *ce;
@@ -5815,6 +6951,13 @@ static int ZEND_FASTCALL  ZEND_INIT_STAT
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -5830,6 +6973,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *expr_ptr;
@@ -5914,6 +7064,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -5928,6 +7085,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_VAR_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval tmp, *varname;
 	HashTable *target_symbol_table;
@@ -6006,6 +7170,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_VAR
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_VAR_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval **value;
 	zend_bool isset = 1;
@@ -6091,6 +7262,13 @@ static int ZEND_FASTCALL  ZEND_ISSET_ISE
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -6251,6 +7429,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_CONST_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -6266,6 +7451,13 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL zend_fetch_var_address_helper_SPEC_CONST_UNUSED(int type, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *varname;
@@ -6396,21 +7588,49 @@ static int ZEND_FASTCALL zend_fetch_var_
 
 static int ZEND_FASTCALL  ZEND_FETCH_R_SPEC_CONST_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_UNUSED(BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_W_SPEC_CONST_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_UNUSED(BP_VAR_W, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_RW_SPEC_CONST_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_UNUSED(BP_VAR_RW, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_FUNC_ARG_SPEC_CONST_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	return zend_fetch_var_address_helper_SPEC_CONST_UNUSED(zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC) ? BP_VAR_W : BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
@@ -6418,16 +7638,37 @@ static int ZEND_FASTCALL  ZEND_FETCH_FUN
 
 static int ZEND_FASTCALL  ZEND_FETCH_UNSET_SPEC_CONST_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_UNUSED(BP_VAR_UNSET, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_IS_SPEC_CONST_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CONST_UNUSED(BP_VAR_IS, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_INIT_STATIC_METHOD_CALL_SPEC_CONST_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	zend_class_entry *ce;
@@ -6553,6 +7794,13 @@ static int ZEND_FASTCALL  ZEND_INIT_STAT
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_CONST_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *expr_ptr;
@@ -6637,6 +7885,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_CONST_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -6651,6 +7906,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_VAR_SPEC_CONST_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval tmp, *varname;
 	HashTable *target_symbol_table;
@@ -6729,6 +7991,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_VAR
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_VAR_SPEC_CONST_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval **value;
 	zend_bool isset = 1;
@@ -6814,6 +8083,13 @@ static int ZEND_FASTCALL  ZEND_ISSET_ISE
 
 static int ZEND_FASTCALL  ZEND_DECLARE_LAMBDA_FUNCTION_SPEC_CONST_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_function *op_array;
 
@@ -6838,6 +8114,13 @@ static int ZEND_FASTCALL  ZEND_DECLARE_L
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_CONST_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -6997,6 +8280,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7012,6 +8302,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7027,6 +8324,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7042,6 +8346,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7057,6 +8368,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7072,6 +8390,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7087,6 +8412,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7102,6 +8434,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7117,6 +8456,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7132,6 +8478,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -7149,6 +8502,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -7165,6 +8525,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -7181,6 +8548,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -7197,6 +8571,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -7213,6 +8594,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7228,6 +8616,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7243,6 +8638,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7258,6 +8660,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7273,6 +8682,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -7290,6 +8706,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_INIT_STATIC_METHOD_CALL_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	zend_class_entry *ce;
@@ -7415,6 +8838,13 @@ static int ZEND_FASTCALL  ZEND_INIT_STAT
 
 static int ZEND_FASTCALL  ZEND_CATCH_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_class_entry *ce, *catch_ce;
 	zval *exception;
@@ -7475,6 +8905,13 @@ static int ZEND_FASTCALL  ZEND_CATCH_SPE
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7489,6 +8926,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *expr_ptr;
@@ -7573,6 +9017,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -7587,6 +9038,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -7746,6 +9204,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_CONST_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CONST,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -7761,6 +9226,13 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_BW_NOT_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -7774,6 +9246,13 @@ static int ZEND_FASTCALL  ZEND_BW_NOT_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_NOT_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -7787,6 +9266,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_NOT_
 
 static int ZEND_FASTCALL  ZEND_ECHO_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *z;
@@ -7806,6 +9292,13 @@ static int ZEND_FASTCALL  ZEND_ECHO_SPEC
 
 static int ZEND_FASTCALL  ZEND_PRINT_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	ZVAL_LONG(&EX_T(opline->result.var).tmp_var, 1);
@@ -7814,6 +9307,13 @@ static int ZEND_FASTCALL  ZEND_PRINT_SPE
 
 static int ZEND_FASTCALL  ZEND_JMPZ_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *val;
@@ -7844,6 +9344,13 @@ static int ZEND_FASTCALL  ZEND_JMPZ_SPEC
 
 static int ZEND_FASTCALL  ZEND_JMPNZ_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *val;
@@ -7874,6 +9381,13 @@ static int ZEND_FASTCALL  ZEND_JMPNZ_SPE
 
 static int ZEND_FASTCALL  ZEND_JMPZNZ_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *val;
@@ -7908,6 +9422,13 @@ static int ZEND_FASTCALL  ZEND_JMPZNZ_SP
 
 static int ZEND_FASTCALL  ZEND_JMPZ_EX_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *val;
@@ -7939,6 +9460,13 @@ static int ZEND_FASTCALL  ZEND_JMPZ_EX_S
 
 static int ZEND_FASTCALL  ZEND_JMPNZ_EX_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *val;
@@ -7970,6 +9498,13 @@ static int ZEND_FASTCALL  ZEND_JMPNZ_EX_
 
 static int ZEND_FASTCALL  ZEND_FREE_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -7984,6 +9519,13 @@ static int ZEND_FASTCALL  ZEND_FREE_SPEC
 
 static int ZEND_FASTCALL  ZEND_RETURN_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *retval_ptr;
 	zend_free_op free_op1;
@@ -8027,6 +9569,13 @@ static int ZEND_FASTCALL  ZEND_RETURN_SP
 
 static int ZEND_FASTCALL  ZEND_RETURN_BY_REF_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *retval_ptr;
 	zval **retval_ptr_ptr;
@@ -8098,6 +9647,13 @@ static int ZEND_FASTCALL  ZEND_RETURN_BY
 
 static int ZEND_FASTCALL  ZEND_THROW_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *value;
 	zval *exception;
@@ -8129,6 +9685,13 @@ static int ZEND_FASTCALL  ZEND_THROW_SPE
 
 static int ZEND_FASTCALL  ZEND_SEND_VAL_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -8159,6 +9722,13 @@ static int ZEND_FASTCALL  ZEND_SEND_VAL_
 
 static int ZEND_FASTCALL  ZEND_BOOL_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *retval = &EX_T(opline->result.var).tmp_var;
@@ -8174,6 +9744,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_SPEC
 
 static int ZEND_FASTCALL  ZEND_CLONE_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *obj;
@@ -8240,6 +9817,13 @@ static int ZEND_FASTCALL  ZEND_CLONE_SPE
 
 static int ZEND_FASTCALL  ZEND_CAST_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *expr;
@@ -8299,6 +9883,13 @@ static int ZEND_FASTCALL  ZEND_CAST_SPEC
 
 static int ZEND_FASTCALL  ZEND_INCLUDE_OR_EVAL_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_op_array *new_op_array=NULL;
 	zend_free_op free_op1;
@@ -8432,6 +10023,13 @@ static int ZEND_FASTCALL  ZEND_INCLUDE_O
 
 static int ZEND_FASTCALL  ZEND_FE_RESET_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *array_ptr, **array_ptr_ptr;
@@ -8594,6 +10192,13 @@ static int ZEND_FASTCALL  ZEND_FE_RESET_
 
 static int ZEND_FASTCALL  ZEND_EXIT_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 #if 0 || (IS_TMP_VAR != IS_UNUSED)
 	USE_OPLINE
 
@@ -8616,6 +10221,13 @@ static int ZEND_FASTCALL  ZEND_EXIT_SPEC
 
 static int ZEND_FASTCALL  ZEND_END_SILENCE_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval restored_error_reporting;
 
@@ -8645,6 +10257,13 @@ static int ZEND_FASTCALL  ZEND_END_SILEN
 
 static int ZEND_FASTCALL  ZEND_JMP_SET_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *value;
@@ -8671,6 +10290,13 @@ static int ZEND_FASTCALL  ZEND_JMP_SET_S
 
 static int ZEND_FASTCALL  ZEND_JMP_SET_VAR_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *value, *ret;
@@ -8706,6 +10332,13 @@ static int ZEND_FASTCALL  ZEND_JMP_SET_V
 
 static int ZEND_FASTCALL  ZEND_QM_ASSIGN_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *value;
@@ -8724,6 +10357,13 @@ static int ZEND_FASTCALL  ZEND_QM_ASSIGN
 
 static int ZEND_FASTCALL  ZEND_QM_ASSIGN_VAR_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *value, *ret;
@@ -8751,6 +10391,13 @@ static int ZEND_FASTCALL  ZEND_QM_ASSIGN
 
 static int ZEND_FASTCALL  ZEND_INSTANCEOF_SPEC_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *expr;
@@ -8772,6 +10419,13 @@ static int ZEND_FASTCALL  ZEND_INSTANCEO
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -8787,6 +10441,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -8802,6 +10463,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -8817,6 +10485,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -8832,6 +10507,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -8847,6 +10529,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -8862,6 +10551,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_T
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -8877,6 +10573,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_T
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -8892,6 +10595,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -8907,6 +10617,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -8924,6 +10641,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -8940,6 +10664,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -8956,6 +10687,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -8972,6 +10710,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -8988,6 +10733,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -9003,6 +10755,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -9018,6 +10777,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -9033,6 +10799,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -9048,6 +10821,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL zend_fetch_var_address_helper_SPEC_TMP_CONST(int type, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *varname;
@@ -9178,21 +10958,49 @@ static int ZEND_FASTCALL zend_fetch_var_
 
 static int ZEND_FASTCALL  ZEND_FETCH_R_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_CONST(BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_W_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_CONST(BP_VAR_W, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_RW_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_CONST(BP_VAR_RW, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_FUNC_ARG_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	return zend_fetch_var_address_helper_SPEC_TMP_CONST(zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC) ? BP_VAR_W : BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
@@ -9200,16 +11008,37 @@ static int ZEND_FASTCALL  ZEND_FETCH_FUN
 
 static int ZEND_FASTCALL  ZEND_FETCH_UNSET_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_CONST(BP_VAR_UNSET, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_IS_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_CONST(BP_VAR_IS, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -9227,6 +11056,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_TMP_VAR_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -9251,6 +11087,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_ADD_CHAR_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *str = &EX_T(opline->result.var).tmp_var;
 
@@ -9274,6 +11117,13 @@ static int ZEND_FASTCALL  ZEND_ADD_CHAR_
 
 static int ZEND_FASTCALL  ZEND_ADD_STRING_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *str = &EX_T(opline->result.var).tmp_var;
 
@@ -9297,6 +11147,13 @@ static int ZEND_FASTCALL  ZEND_ADD_STRIN
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -9378,6 +11235,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -9392,6 +11256,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *expr_ptr;
@@ -9476,6 +11347,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -9490,6 +11368,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_VAR_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval tmp, *varname;
 	HashTable *target_symbol_table;
@@ -9568,6 +11453,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_VAR
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_VAR_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval **value;
 	zend_bool isset = 1;
@@ -9653,6 +11545,13 @@ static int ZEND_FASTCALL  ZEND_ISSET_ISE
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -9812,6 +11711,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_TMP_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -9827,6 +11733,13 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -9842,6 +11755,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -9857,6 +11777,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -9872,6 +11799,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -9887,6 +11821,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -9902,6 +11843,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -9917,6 +11865,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_T
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -9932,6 +11887,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_T
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -9947,6 +11909,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -9962,6 +11931,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -9979,6 +11955,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -9995,6 +11978,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -10011,6 +12001,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -10027,6 +12024,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -10043,6 +12047,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10058,6 +12069,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10073,6 +12091,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10088,6 +12113,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10103,6 +12135,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *container;
@@ -10120,6 +12159,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_ADD_VAR_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *str = &EX_T(opline->result.var).tmp_var;
@@ -10165,6 +12211,13 @@ static int ZEND_FASTCALL  ZEND_ADD_VAR_S
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -10247,6 +12300,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10262,6 +12322,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *expr_ptr;
@@ -10346,6 +12413,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -10360,6 +12434,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -10519,6 +12600,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_TMP_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10534,6 +12622,13 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10549,6 +12644,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10564,6 +12666,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10579,6 +12688,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10594,6 +12710,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10609,6 +12732,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10624,6 +12754,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_T
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10639,6 +12776,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_T
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10654,6 +12798,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10669,6 +12820,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -10686,6 +12844,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -10702,6 +12867,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -10718,6 +12890,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -10734,6 +12913,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -10750,6 +12936,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10765,6 +12958,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10780,6 +12980,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10795,6 +13002,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -10810,6 +13024,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL zend_fetch_var_address_helper_SPEC_TMP_VAR(int type, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *varname;
@@ -10940,21 +13161,49 @@ static int ZEND_FASTCALL zend_fetch_var_
 
 static int ZEND_FASTCALL  ZEND_FETCH_R_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_VAR(BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_W_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_VAR(BP_VAR_W, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_RW_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_VAR(BP_VAR_RW, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_FUNC_ARG_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	return zend_fetch_var_address_helper_SPEC_TMP_VAR(zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC) ? BP_VAR_W : BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
@@ -10962,16 +13211,37 @@ static int ZEND_FASTCALL  ZEND_FETCH_FUN
 
 static int ZEND_FASTCALL  ZEND_FETCH_UNSET_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_VAR(BP_VAR_UNSET, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_IS_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_VAR(BP_VAR_IS, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *container;
@@ -10989,6 +13259,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_ADD_VAR_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *str = &EX_T(opline->result.var).tmp_var;
@@ -11034,6 +13311,13 @@ static int ZEND_FASTCALL  ZEND_ADD_VAR_S
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -11116,6 +13400,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -11131,6 +13422,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *expr_ptr;
@@ -11215,6 +13513,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -11229,6 +13534,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_VAR_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval tmp, *varname;
 	HashTable *target_symbol_table;
@@ -11307,6 +13619,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_VAR
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_VAR_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval **value;
 	zend_bool isset = 1;
@@ -11392,6 +13711,13 @@ static int ZEND_FASTCALL  ZEND_ISSET_ISE
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -11552,6 +13878,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_TMP_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -11567,6 +13900,13 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL zend_fetch_var_address_helper_SPEC_TMP_UNUSED(int type, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *varname;
@@ -11697,21 +14037,49 @@ static int ZEND_FASTCALL zend_fetch_var_
 
 static int ZEND_FASTCALL  ZEND_FETCH_R_SPEC_TMP_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_UNUSED(BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_W_SPEC_TMP_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_UNUSED(BP_VAR_W, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_RW_SPEC_TMP_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_UNUSED(BP_VAR_RW, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_FUNC_ARG_SPEC_TMP_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	return zend_fetch_var_address_helper_SPEC_TMP_UNUSED(zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC) ? BP_VAR_W : BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
@@ -11719,16 +14087,37 @@ static int ZEND_FASTCALL  ZEND_FETCH_FUN
 
 static int ZEND_FASTCALL  ZEND_FETCH_UNSET_SPEC_TMP_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_UNUSED(BP_VAR_UNSET, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_IS_SPEC_TMP_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_TMP_UNUSED(BP_VAR_IS, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_TMP_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *expr_ptr;
@@ -11813,6 +14202,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_TMP_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -11827,6 +14223,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_VAR_SPEC_TMP_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval tmp, *varname;
 	HashTable *target_symbol_table;
@@ -11905,6 +14308,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_VAR
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_VAR_SPEC_TMP_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval **value;
 	zend_bool isset = 1;
@@ -11990,6 +14400,13 @@ static int ZEND_FASTCALL  ZEND_ISSET_ISE
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_TMP_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -12149,6 +14566,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12164,6 +14588,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12179,6 +14610,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12194,6 +14632,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12209,6 +14654,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12224,6 +14676,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12239,6 +14698,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_T
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12254,6 +14720,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_T
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12269,6 +14742,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12284,6 +14764,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -12301,6 +14788,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -12317,6 +14811,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -12333,6 +14834,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -12349,6 +14857,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -12365,6 +14880,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12380,6 +14902,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12395,6 +14924,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12410,6 +14946,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12425,6 +14968,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -12442,6 +14992,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_ADD_VAR_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *str = &EX_T(opline->result.var).tmp_var;
@@ -12486,6 +15043,13 @@ static int ZEND_FASTCALL  ZEND_ADD_VAR_S
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -12567,6 +15131,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12581,6 +15152,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *expr_ptr;
@@ -12665,6 +15243,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -12679,6 +15264,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -12838,6 +15430,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_TMP_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_TMP_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12853,6 +15452,13 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_BW_NOT_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12866,6 +15472,13 @@ static int ZEND_FASTCALL  ZEND_BW_NOT_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_NOT_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -12879,6 +15492,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_NOT_
 
 static int ZEND_FASTCALL  ZEND_PRE_INC_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **var_ptr;
@@ -12926,6 +15546,13 @@ static int ZEND_FASTCALL  ZEND_PRE_INC_S
 
 static int ZEND_FASTCALL  ZEND_PRE_DEC_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **var_ptr;
@@ -12973,6 +15600,13 @@ static int ZEND_FASTCALL  ZEND_PRE_DEC_S
 
 static int ZEND_FASTCALL  ZEND_POST_INC_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **var_ptr, *retval;
@@ -13016,6 +15650,13 @@ static int ZEND_FASTCALL  ZEND_POST_INC_
 
 static int ZEND_FASTCALL  ZEND_POST_DEC_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **var_ptr, *retval;
@@ -13059,6 +15700,13 @@ static int ZEND_FASTCALL  ZEND_POST_DEC_
 
 static int ZEND_FASTCALL  ZEND_ECHO_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *z;
@@ -13078,6 +15726,13 @@ static int ZEND_FASTCALL  ZEND_ECHO_SPEC
 
 static int ZEND_FASTCALL  ZEND_PRINT_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	ZVAL_LONG(&EX_T(opline->result.var).tmp_var, 1);
@@ -13086,6 +15741,13 @@ static int ZEND_FASTCALL  ZEND_PRINT_SPE
 
 static int ZEND_FASTCALL  ZEND_JMPZ_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *val;
@@ -13116,6 +15778,13 @@ static int ZEND_FASTCALL  ZEND_JMPZ_SPEC
 
 static int ZEND_FASTCALL  ZEND_JMPNZ_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *val;
@@ -13146,6 +15815,13 @@ static int ZEND_FASTCALL  ZEND_JMPNZ_SPE
 
 static int ZEND_FASTCALL  ZEND_JMPZNZ_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *val;
@@ -13180,6 +15856,13 @@ static int ZEND_FASTCALL  ZEND_JMPZNZ_SP
 
 static int ZEND_FASTCALL  ZEND_JMPZ_EX_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *val;
@@ -13211,6 +15894,13 @@ static int ZEND_FASTCALL  ZEND_JMPZ_EX_S
 
 static int ZEND_FASTCALL  ZEND_JMPNZ_EX_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *val;
@@ -13242,6 +15932,13 @@ static int ZEND_FASTCALL  ZEND_JMPNZ_EX_
 
 static int ZEND_FASTCALL  ZEND_FREE_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -13256,6 +15953,13 @@ static int ZEND_FASTCALL  ZEND_FREE_SPEC
 
 static int ZEND_FASTCALL  ZEND_RETURN_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *retval_ptr;
 	zend_free_op free_op1;
@@ -13299,6 +16003,13 @@ static int ZEND_FASTCALL  ZEND_RETURN_SP
 
 static int ZEND_FASTCALL  ZEND_RETURN_BY_REF_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *retval_ptr;
 	zval **retval_ptr_ptr;
@@ -13371,6 +16082,13 @@ static int ZEND_FASTCALL  ZEND_RETURN_BY
 
 static int ZEND_FASTCALL  ZEND_THROW_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *value;
 	zval *exception;
@@ -13402,6 +16120,13 @@ static int ZEND_FASTCALL  ZEND_THROW_SPE
 
 static int ZEND_FASTCALL zend_send_by_var_helper_SPEC_VAR(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *varptr;
 	zend_free_op free_op1;
@@ -13435,6 +16160,13 @@ static int ZEND_FASTCALL zend_send_by_va
 
 static int ZEND_FASTCALL  ZEND_SEND_VAR_NO_REF_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *varptr;
@@ -13482,6 +16214,13 @@ static int ZEND_FASTCALL  ZEND_SEND_VAR_
 
 static int ZEND_FASTCALL  ZEND_SEND_REF_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **varptr_ptr;
@@ -13520,6 +16259,13 @@ static int ZEND_FASTCALL  ZEND_SEND_REF_
 
 static int ZEND_FASTCALL  ZEND_SEND_VAR_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if (opline->extended_value == ZEND_DO_FCALL_BY_NAME) {
@@ -13533,6 +16279,13 @@ static int ZEND_FASTCALL  ZEND_SEND_VAR_
 
 static int ZEND_FASTCALL  ZEND_BOOL_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *retval = &EX_T(opline->result.var).tmp_var;
@@ -13548,6 +16301,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_SPEC
 
 static int ZEND_FASTCALL  ZEND_SWITCH_FREE_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -13558,6 +16318,13 @@ static int ZEND_FASTCALL  ZEND_SWITCH_FR
 
 static int ZEND_FASTCALL  ZEND_CLONE_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *obj;
@@ -13624,6 +16391,13 @@ static int ZEND_FASTCALL  ZEND_CLONE_SPE
 
 static int ZEND_FASTCALL  ZEND_CAST_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *expr;
@@ -13683,6 +16457,13 @@ static int ZEND_FASTCALL  ZEND_CAST_SPEC
 
 static int ZEND_FASTCALL  ZEND_INCLUDE_OR_EVAL_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_op_array *new_op_array=NULL;
 	zend_free_op free_op1;
@@ -13816,6 +16597,13 @@ static int ZEND_FASTCALL  ZEND_INCLUDE_O
 
 static int ZEND_FASTCALL  ZEND_FE_RESET_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *array_ptr, **array_ptr_ptr;
@@ -13978,6 +16766,13 @@ static int ZEND_FASTCALL  ZEND_FE_RESET_
 
 static int ZEND_FASTCALL  ZEND_FE_FETCH_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *array = EX_T(opline->op1.var).fe.ptr;
@@ -14109,6 +16904,13 @@ static int ZEND_FASTCALL  ZEND_FE_FETCH_
 
 static int ZEND_FASTCALL  ZEND_EXIT_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 #if 0 || (IS_VAR != IS_UNUSED)
 	USE_OPLINE
 
@@ -14131,6 +16933,13 @@ static int ZEND_FASTCALL  ZEND_EXIT_SPEC
 
 static int ZEND_FASTCALL  ZEND_JMP_SET_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *value;
@@ -14157,6 +16966,13 @@ static int ZEND_FASTCALL  ZEND_JMP_SET_S
 
 static int ZEND_FASTCALL  ZEND_JMP_SET_VAR_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *value, *ret;
@@ -14192,6 +17008,13 @@ static int ZEND_FASTCALL  ZEND_JMP_SET_V
 
 static int ZEND_FASTCALL  ZEND_QM_ASSIGN_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *value;
@@ -14210,6 +17033,13 @@ static int ZEND_FASTCALL  ZEND_QM_ASSIGN
 
 static int ZEND_FASTCALL  ZEND_QM_ASSIGN_VAR_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *value, *ret;
@@ -14238,6 +17068,13 @@ static int ZEND_FASTCALL  ZEND_QM_ASSIGN
 
 static int ZEND_FASTCALL  ZEND_INSTANCEOF_SPEC_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *expr;
@@ -14259,6 +17096,13 @@ static int ZEND_FASTCALL  ZEND_INSTANCEO
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -14274,6 +17118,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -14289,6 +17140,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -14304,6 +17162,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -14319,6 +17184,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -14334,6 +17206,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -14349,6 +17228,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_V
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -14364,6 +17250,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_V
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -14379,6 +17272,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -14394,6 +17294,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -14411,6 +17318,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -14427,6 +17341,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -14443,6 +17364,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -14459,6 +17387,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -14475,6 +17410,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -14490,6 +17432,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -14505,6 +17454,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -14520,6 +17476,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -14535,6 +17498,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_VAR_CONST(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op_data1;
 	zval **object_ptr = _get_zval_ptr_ptr_var(opline->op1.var, execute_data, &free_op1 TSRMLS_CC);
@@ -14652,6 +17622,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_VAR_CONST(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -14741,61 +17718,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CONST(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CONST(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CONST(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CONST(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CONST(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CONST(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CONST(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CONST(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CONST(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CONST(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CONST(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_pre_incdec_property_helper_SPEC_VAR_CONST(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **object_ptr;
@@ -14901,16 +17962,37 @@ static int ZEND_FASTCALL zend_pre_incdec
 
 static int ZEND_FASTCALL  ZEND_PRE_INC_OBJ_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_VAR_CONST(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_PRE_DEC_OBJ_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_VAR_CONST(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_post_incdec_property_helper_SPEC_VAR_CONST(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **object_ptr;
@@ -15008,16 +18090,37 @@ static int ZEND_FASTCALL zend_post_incde
 
 static int ZEND_FASTCALL  ZEND_POST_INC_OBJ_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_VAR_CONST(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_POST_DEC_OBJ_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_VAR_CONST(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_fetch_var_address_helper_SPEC_VAR_CONST(int type, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *varname;
@@ -15148,21 +18251,49 @@ static int ZEND_FASTCALL zend_fetch_var_
 
 static int ZEND_FASTCALL  ZEND_FETCH_R_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_CONST(BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_W_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_CONST(BP_VAR_W, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_RW_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_CONST(BP_VAR_RW, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_FUNC_ARG_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	return zend_fetch_var_address_helper_SPEC_VAR_CONST(zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC) ? BP_VAR_W : BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
@@ -15170,16 +18301,37 @@ static int ZEND_FASTCALL  ZEND_FETCH_FUN
 
 static int ZEND_FASTCALL  ZEND_FETCH_UNSET_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_CONST(BP_VAR_UNSET, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_IS_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_CONST(BP_VAR_IS, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -15197,6 +18349,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_W_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -15231,6 +18390,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_RW_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -15253,6 +18419,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_IS_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -15268,6 +18441,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_FUNC_ARG_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -15301,6 +18481,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_UNSET_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -15342,6 +18529,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL zend_fetch_property_address_read_helper_SPEC_VAR_CONST(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -15385,11 +18579,25 @@ static int ZEND_FASTCALL zend_fetch_prop
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_R_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_property_address_read_helper_SPEC_VAR_CONST(ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_W_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *property;
@@ -15434,6 +18642,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_RW_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *property;
@@ -15465,6 +18680,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_IS_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -15507,6 +18729,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_FUNC_ARG_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if (zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC)) {
@@ -15544,6 +18773,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_UNSET_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_res;
 	zval **container;
@@ -15587,6 +18823,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_OBJ_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **object_ptr;
@@ -15617,6 +18860,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_OB
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIM_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **object_ptr;
@@ -15697,6 +18947,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_DI
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *value;
@@ -15754,6 +19011,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_SP
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -15836,6 +19100,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_INIT_STATIC_METHOD_CALL_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	zend_class_entry *ce;
@@ -15961,6 +19232,13 @@ static int ZEND_FASTCALL  ZEND_INIT_STAT
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -15975,6 +19253,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_FETCH_CONSTANT_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -16067,6 +19352,13 @@ constant_fetch_end:
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *expr_ptr;
@@ -16151,6 +19443,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -16165,6 +19464,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_VAR_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval tmp, *varname;
 	HashTable *target_symbol_table;
@@ -16243,6 +19549,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_VAR
 
 static int ZEND_FASTCALL  ZEND_UNSET_DIM_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -16339,6 +19652,13 @@ num_index_dim:
 
 static int ZEND_FASTCALL  ZEND_UNSET_OBJ_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -16380,6 +19700,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_OBJ
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_VAR_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval **value;
 	zend_bool isset = 1;
@@ -16465,6 +19792,13 @@ static int ZEND_FASTCALL  ZEND_ISSET_ISE
 
 static int ZEND_FASTCALL zend_isset_isempty_dim_prop_obj_handler_SPEC_VAR_CONST(int prop_dim, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -16602,16 +19936,37 @@ num_index_prop:
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_DIM_OBJ_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_VAR_CONST(0, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_PROP_OBJ_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_VAR_CONST(1, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -16772,6 +20127,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -16787,11 +20149,25 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_VAR_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CONST(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -16807,6 +20183,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -16822,6 +20205,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -16837,6 +20227,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -16852,6 +20249,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -16867,6 +20271,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -16882,6 +20293,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_V
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -16897,6 +20315,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_V
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -16912,6 +20337,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -16927,6 +20359,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -16944,6 +20383,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -16960,6 +20406,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -16976,6 +20429,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -16992,6 +20452,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -17008,6 +20475,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -17023,6 +20497,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -17038,6 +20519,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -17053,6 +20541,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -17068,6 +20563,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_VAR_TMP(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2, free_op_data1;
 	zval **object_ptr = _get_zval_ptr_ptr_var(opline->op1.var, execute_data, &free_op1 TSRMLS_CC);
@@ -17185,6 +20687,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_VAR_TMP(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2, free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -17275,61 +20784,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_TMP(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_TMP(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_TMP(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_TMP(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_TMP(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_TMP(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_TMP(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_TMP(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_TMP(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_TMP(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_TMP(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_pre_incdec_property_helper_SPEC_VAR_TMP(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **object_ptr;
@@ -17435,16 +21028,37 @@ static int ZEND_FASTCALL zend_pre_incdec
 
 static int ZEND_FASTCALL  ZEND_PRE_INC_OBJ_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_VAR_TMP(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_PRE_DEC_OBJ_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_VAR_TMP(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_post_incdec_property_helper_SPEC_VAR_TMP(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **object_ptr;
@@ -17542,16 +21156,37 @@ static int ZEND_FASTCALL zend_post_incde
 
 static int ZEND_FASTCALL  ZEND_POST_INC_OBJ_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_VAR_TMP(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_POST_DEC_OBJ_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_VAR_TMP(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *container;
@@ -17569,6 +21204,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_W_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -17603,6 +21245,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_RW_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -17625,6 +21274,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_IS_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *container;
@@ -17640,6 +21296,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_FUNC_ARG_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -17673,6 +21336,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_UNSET_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -17714,6 +21384,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL zend_fetch_property_address_read_helper_SPEC_VAR_TMP(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -17757,11 +21434,25 @@ static int ZEND_FASTCALL zend_fetch_prop
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_R_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_fetch_property_address_read_helper_SPEC_VAR_TMP(ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_W_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *property;
@@ -17806,6 +21497,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_RW_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *property;
@@ -17837,6 +21535,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_IS_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -17879,6 +21584,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_FUNC_ARG_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if (zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC)) {
@@ -17916,6 +21628,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_UNSET_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2, free_res;
 	zval **container;
@@ -17959,6 +21678,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_OBJ_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **object_ptr;
@@ -17989,6 +21715,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_OB
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIM_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **object_ptr;
@@ -18070,6 +21803,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_DI
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *value;
@@ -18127,6 +21867,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_SP
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -18210,6 +21957,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_INIT_STATIC_METHOD_CALL_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	zend_class_entry *ce;
@@ -18335,6 +22089,13 @@ static int ZEND_FASTCALL  ZEND_INIT_STAT
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -18350,6 +22111,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *expr_ptr;
@@ -18434,6 +22202,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -18448,6 +22223,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_DIM_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -18544,6 +22326,13 @@ num_index_dim:
 
 static int ZEND_FASTCALL  ZEND_UNSET_OBJ_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -18585,6 +22374,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_OBJ
 
 static int ZEND_FASTCALL zend_isset_isempty_dim_prop_obj_handler_SPEC_VAR_TMP(int prop_dim, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *container;
@@ -18722,16 +22518,37 @@ num_index_prop:
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_DIM_OBJ_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_VAR_TMP(0, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_PROP_OBJ_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_VAR_TMP(1, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -18892,6 +22709,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -18907,11 +22731,25 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_VAR_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_TMP(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -18927,6 +22765,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -18942,6 +22787,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -18957,6 +22809,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -18972,6 +22831,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -18987,6 +22853,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -19002,6 +22875,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_V
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -19017,6 +22897,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_V
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -19032,6 +22919,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -19047,6 +22941,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -19064,6 +22965,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -19080,6 +22988,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -19096,6 +23011,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -19112,6 +23034,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -19128,6 +23057,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -19143,6 +23079,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -19158,6 +23101,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -19173,6 +23123,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -19188,6 +23145,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_VAR_VAR(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2, free_op_data1;
 	zval **object_ptr = _get_zval_ptr_ptr_var(opline->op1.var, execute_data, &free_op1 TSRMLS_CC);
@@ -19305,6 +23269,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_VAR_VAR(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2, free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -19395,61 +23366,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_VAR(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_VAR(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_VAR(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_VAR(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_VAR(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_VAR(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_VAR(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_VAR(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_VAR(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_VAR(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_VAR(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_pre_incdec_property_helper_SPEC_VAR_VAR(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **object_ptr;
@@ -19555,16 +23610,37 @@ static int ZEND_FASTCALL zend_pre_incdec
 
 static int ZEND_FASTCALL  ZEND_PRE_INC_OBJ_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_VAR_VAR(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_PRE_DEC_OBJ_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_VAR_VAR(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_post_incdec_property_helper_SPEC_VAR_VAR(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **object_ptr;
@@ -19662,16 +23738,37 @@ static int ZEND_FASTCALL zend_post_incde
 
 static int ZEND_FASTCALL  ZEND_POST_INC_OBJ_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_VAR_VAR(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_POST_DEC_OBJ_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_VAR_VAR(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_fetch_var_address_helper_SPEC_VAR_VAR(int type, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *varname;
@@ -19802,21 +23899,49 @@ static int ZEND_FASTCALL zend_fetch_var_
 
 static int ZEND_FASTCALL  ZEND_FETCH_R_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_VAR(BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_W_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_VAR(BP_VAR_W, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_RW_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_VAR(BP_VAR_RW, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_FUNC_ARG_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	return zend_fetch_var_address_helper_SPEC_VAR_VAR(zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC) ? BP_VAR_W : BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
@@ -19824,16 +23949,37 @@ static int ZEND_FASTCALL  ZEND_FETCH_FUN
 
 static int ZEND_FASTCALL  ZEND_FETCH_UNSET_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_VAR(BP_VAR_UNSET, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_IS_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_VAR(BP_VAR_IS, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *container;
@@ -19851,6 +23997,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_W_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -19885,6 +24038,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_RW_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -19907,6 +24067,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_IS_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *container;
@@ -19922,6 +24089,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_FUNC_ARG_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -19955,6 +24129,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_UNSET_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -19996,6 +24177,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL zend_fetch_property_address_read_helper_SPEC_VAR_VAR(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -20039,11 +24227,25 @@ static int ZEND_FASTCALL zend_fetch_prop
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_R_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_property_address_read_helper_SPEC_VAR_VAR(ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_W_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *property;
@@ -20088,6 +24290,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_RW_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *property;
@@ -20119,6 +24328,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_IS_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -20161,6 +24377,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_FUNC_ARG_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if (zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC)) {
@@ -20198,6 +24421,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_UNSET_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2, free_res;
 	zval **container;
@@ -20241,6 +24471,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_OBJ_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **object_ptr;
@@ -20271,6 +24508,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_OB
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIM_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **object_ptr;
@@ -20352,6 +24596,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_DI
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *value;
@@ -20410,6 +24661,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_SP
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_REF_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **variable_ptr_ptr;
@@ -20468,6 +24726,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_RE
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -20551,6 +24816,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_INIT_STATIC_METHOD_CALL_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	zend_class_entry *ce;
@@ -20676,6 +24948,13 @@ static int ZEND_FASTCALL  ZEND_INIT_STAT
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -20691,6 +24970,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *expr_ptr;
@@ -20775,6 +25061,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -20789,6 +25082,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_VAR_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval tmp, *varname;
 	HashTable *target_symbol_table;
@@ -20867,6 +25167,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_VAR
 
 static int ZEND_FASTCALL  ZEND_UNSET_DIM_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -20963,6 +25270,13 @@ num_index_dim:
 
 static int ZEND_FASTCALL  ZEND_UNSET_OBJ_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -21004,6 +25318,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_OBJ
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_VAR_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval **value;
 	zend_bool isset = 1;
@@ -21089,6 +25410,13 @@ static int ZEND_FASTCALL  ZEND_ISSET_ISE
 
 static int ZEND_FASTCALL zend_isset_isempty_dim_prop_obj_handler_SPEC_VAR_VAR(int prop_dim, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *container;
@@ -21226,16 +25554,37 @@ num_index_prop:
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_DIM_OBJ_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_VAR_VAR(0, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_PROP_OBJ_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_VAR_VAR(1, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -21397,6 +25746,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -21412,11 +25768,25 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_VAR_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_VAR(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_VAR_UNUSED(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op_data1;
 	zval **object_ptr = _get_zval_ptr_ptr_var(opline->op1.var, execute_data, &free_op1 TSRMLS_CC);
@@ -21534,6 +25904,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_VAR_UNUSED(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -21623,61 +26000,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_UNUSED(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_UNUSED(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_UNUSED(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_UNUSED(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_UNUSED(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_UNUSED(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_UNUSED(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_UNUSED(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_UNUSED(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_UNUSED(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_UNUSED(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_fetch_var_address_helper_SPEC_VAR_UNUSED(int type, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *varname;
@@ -21808,21 +26269,49 @@ static int ZEND_FASTCALL zend_fetch_var_
 
 static int ZEND_FASTCALL  ZEND_FETCH_R_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_UNUSED(BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_W_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_UNUSED(BP_VAR_W, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_RW_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_UNUSED(BP_VAR_RW, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_FUNC_ARG_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	return zend_fetch_var_address_helper_SPEC_VAR_UNUSED(zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC) ? BP_VAR_W : BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
@@ -21830,16 +26319,37 @@ static int ZEND_FASTCALL  ZEND_FETCH_FUN
 
 static int ZEND_FASTCALL  ZEND_FETCH_UNSET_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_UNUSED(BP_VAR_UNSET, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_IS_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_VAR_UNUSED(BP_VAR_IS, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_W_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -21874,6 +26384,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_RW_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -21896,6 +26413,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_FUNC_ARG_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -21929,6 +26453,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIM_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **object_ptr;
@@ -22009,6 +26540,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_DI
 
 static int ZEND_FASTCALL  ZEND_INIT_STATIC_METHOD_CALL_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	zend_class_entry *ce;
@@ -22134,6 +26672,13 @@ static int ZEND_FASTCALL  ZEND_INIT_STAT
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *expr_ptr;
@@ -22218,6 +26763,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -22232,6 +26784,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_VAR_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval tmp, *varname;
 	HashTable *target_symbol_table;
@@ -22310,6 +26869,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_VAR
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_VAR_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval **value;
 	zend_bool isset = 1;
@@ -22395,6 +26961,13 @@ static int ZEND_FASTCALL  ZEND_ISSET_ISE
 
 static int ZEND_FASTCALL  ZEND_SEPARATE_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *var_ptr, *new_zv;
 
@@ -22416,6 +26989,13 @@ static int ZEND_FASTCALL  ZEND_SEPARATE_
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -22576,11 +27156,25 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_VAR_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_UNUSED(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -22596,6 +27190,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -22611,6 +27212,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -22626,6 +27234,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -22641,6 +27256,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -22656,6 +27278,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -22671,6 +27300,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_V
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -22686,6 +27322,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_V
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -22701,6 +27344,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -22716,6 +27366,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -22733,6 +27390,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -22749,6 +27413,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -22765,6 +27436,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -22781,6 +27459,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -22797,6 +27482,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -22812,6 +27504,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -22827,6 +27526,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -22842,6 +27548,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -22857,6 +27570,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_VAR_CV(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op_data1;
 	zval **object_ptr = _get_zval_ptr_ptr_var(opline->op1.var, execute_data, &free_op1 TSRMLS_CC);
@@ -22974,6 +27694,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_VAR_CV(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -23063,61 +27790,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CV(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CV(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CV(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CV(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CV(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CV(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CV(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CV(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CV(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CV(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CV(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_pre_incdec_property_helper_SPEC_VAR_CV(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **object_ptr;
@@ -23223,16 +28034,37 @@ static int ZEND_FASTCALL zend_pre_incdec
 
 static int ZEND_FASTCALL  ZEND_PRE_INC_OBJ_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_VAR_CV(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_PRE_DEC_OBJ_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_VAR_CV(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_post_incdec_property_helper_SPEC_VAR_CV(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **object_ptr;
@@ -23330,16 +28162,37 @@ static int ZEND_FASTCALL zend_post_incde
 
 static int ZEND_FASTCALL  ZEND_POST_INC_OBJ_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_VAR_CV(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_POST_DEC_OBJ_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_VAR_CV(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -23357,6 +28210,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_W_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -23391,6 +28251,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_RW_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -23413,6 +28280,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_IS_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -23428,6 +28302,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_FUNC_ARG_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -23461,6 +28342,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_UNSET_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -23502,6 +28390,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL zend_fetch_property_address_read_helper_SPEC_VAR_CV(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -23545,11 +28440,25 @@ static int ZEND_FASTCALL zend_fetch_prop
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_R_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_fetch_property_address_read_helper_SPEC_VAR_CV(ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_W_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *property;
@@ -23594,6 +28503,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_RW_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *property;
@@ -23625,6 +28541,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_IS_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -23667,6 +28590,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_FUNC_ARG_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if (zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC)) {
@@ -23704,6 +28634,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_UNSET_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_res;
 	zval **container;
@@ -23747,6 +28684,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_OBJ_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **object_ptr;
@@ -23777,6 +28721,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_OB
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIM_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **object_ptr;
@@ -23857,6 +28808,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_DI
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *value;
@@ -23914,6 +28872,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_SP
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_REF_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **variable_ptr_ptr;
@@ -23971,6 +28936,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_RE
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -24053,6 +29025,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_INIT_STATIC_METHOD_CALL_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	zend_class_entry *ce;
@@ -24178,6 +29157,13 @@ static int ZEND_FASTCALL  ZEND_INIT_STAT
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -24192,6 +29178,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *expr_ptr;
@@ -24276,6 +29269,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -24290,6 +29290,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_DIM_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -24386,6 +29393,13 @@ num_index_dim:
 
 static int ZEND_FASTCALL  ZEND_UNSET_OBJ_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -24427,6 +29441,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_OBJ
 
 static int ZEND_FASTCALL zend_isset_isempty_dim_prop_obj_handler_SPEC_VAR_CV(int prop_dim, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *container;
@@ -24564,16 +29585,37 @@ num_index_prop:
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_DIM_OBJ_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_VAR_CV(0, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_PROP_OBJ_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_VAR_CV(1, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -24734,6 +29776,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -24749,11 +29798,25 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_VAR_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_VAR,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_VAR_CV(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_CLONE_SPEC_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *obj;
@@ -24820,6 +29883,13 @@ static int ZEND_FASTCALL  ZEND_CLONE_SPE
 
 static int ZEND_FASTCALL  ZEND_EXIT_SPEC_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,opline->op2_type);
+		return 0;
+	}
+
 #if 0 || (IS_UNUSED != IS_UNUSED)
 	USE_OPLINE
 
@@ -24842,6 +29912,13 @@ static int ZEND_FASTCALL  ZEND_EXIT_SPEC
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_UNUSED_CONST(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op_data1;
 	zval **object_ptr = _get_obj_zval_ptr_ptr_unused(TSRMLS_C);
@@ -24958,6 +30035,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_UNUSED_CONST(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -25047,61 +30131,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CONST(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CONST(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CONST(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CONST(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CONST(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CONST(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CONST(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CONST(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CONST(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CONST(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CONST(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_pre_incdec_property_helper_SPEC_UNUSED_CONST(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -25207,16 +30375,37 @@ static int ZEND_FASTCALL zend_pre_incdec
 
 static int ZEND_FASTCALL  ZEND_PRE_INC_OBJ_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_UNUSED_CONST(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_PRE_DEC_OBJ_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_UNUSED_CONST(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_post_incdec_property_helper_SPEC_UNUSED_CONST(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -25314,16 +30503,37 @@ static int ZEND_FASTCALL zend_post_incde
 
 static int ZEND_FASTCALL  ZEND_POST_INC_OBJ_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_UNUSED_CONST(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_POST_DEC_OBJ_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_UNUSED_CONST(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_fetch_property_address_read_helper_SPEC_UNUSED_CONST(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -25366,11 +30576,25 @@ static int ZEND_FASTCALL zend_fetch_prop
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_R_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_property_address_read_helper_SPEC_UNUSED_CONST(ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_W_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *property;
@@ -25414,6 +30638,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_RW_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *property;
@@ -25445,6 +30676,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_IS_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -25486,6 +30724,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_FUNC_ARG_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if (zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC)) {
@@ -25523,6 +30768,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_UNSET_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_res;
 	zval **container;
@@ -25565,6 +30817,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_OBJ_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -25595,6 +30854,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_OB
 
 static int ZEND_FASTCALL  ZEND_ADD_CHAR_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *str = &EX_T(opline->result.var).tmp_var;
 
@@ -25618,6 +30884,13 @@ static int ZEND_FASTCALL  ZEND_ADD_CHAR_
 
 static int ZEND_FASTCALL  ZEND_ADD_STRING_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *str = &EX_T(opline->result.var).tmp_var;
 
@@ -25641,6 +30914,13 @@ static int ZEND_FASTCALL  ZEND_ADD_STRIN
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -25722,6 +31002,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_FETCH_CONSTANT_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	SAVE_OPLINE();
@@ -25814,6 +31101,13 @@ constant_fetch_end:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -25828,6 +31122,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_DIM_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **container;
@@ -25923,6 +31224,13 @@ num_index_dim:
 
 static int ZEND_FASTCALL  ZEND_UNSET_OBJ_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **container;
@@ -25963,6 +31271,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_OBJ
 
 static int ZEND_FASTCALL zend_isset_isempty_dim_prop_obj_handler_SPEC_UNUSED_CONST(int prop_dim, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -26098,16 +31413,37 @@ num_index_prop:
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_DIM_OBJ_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_UNUSED_CONST(0, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_PROP_OBJ_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_UNUSED_CONST(1, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -26267,11 +31603,25 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_UNUSED_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CONST(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_UNUSED_TMP(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2, free_op_data1;
 	zval **object_ptr = _get_obj_zval_ptr_ptr_unused(TSRMLS_C);
@@ -26388,6 +31738,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_UNUSED_TMP(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2, free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -26478,61 +31835,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_TMP(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_TMP(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_TMP(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_TMP(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_TMP(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_TMP(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_TMP(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_TMP(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_TMP(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_TMP(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_TMP(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_pre_incdec_property_helper_SPEC_UNUSED_TMP(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **object_ptr;
@@ -26638,16 +32079,37 @@ static int ZEND_FASTCALL zend_pre_incdec
 
 static int ZEND_FASTCALL  ZEND_PRE_INC_OBJ_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_UNUSED_TMP(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_PRE_DEC_OBJ_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_UNUSED_TMP(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_post_incdec_property_helper_SPEC_UNUSED_TMP(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **object_ptr;
@@ -26745,16 +32207,37 @@ static int ZEND_FASTCALL zend_post_incde
 
 static int ZEND_FASTCALL  ZEND_POST_INC_OBJ_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_UNUSED_TMP(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_POST_DEC_OBJ_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_UNUSED_TMP(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_fetch_property_address_read_helper_SPEC_UNUSED_TMP(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -26797,11 +32280,25 @@ static int ZEND_FASTCALL zend_fetch_prop
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_R_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_fetch_property_address_read_helper_SPEC_UNUSED_TMP(ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_W_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *property;
@@ -26845,6 +32342,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_RW_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *property;
@@ -26876,6 +32380,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_IS_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -26917,6 +32428,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_FUNC_ARG_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if (zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC)) {
@@ -26954,6 +32472,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_UNSET_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2, free_res;
 	zval **container;
@@ -26996,6 +32521,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_OBJ_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **object_ptr;
@@ -27026,6 +32558,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_OB
 
 static int ZEND_FASTCALL  ZEND_ADD_VAR_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *str = &EX_T(opline->result.var).tmp_var;
@@ -27071,6 +32610,13 @@ static int ZEND_FASTCALL  ZEND_ADD_VAR_S
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -27153,6 +32699,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -27167,6 +32720,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_DIM_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **container;
@@ -27262,6 +32822,13 @@ num_index_dim:
 
 static int ZEND_FASTCALL  ZEND_UNSET_OBJ_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **container;
@@ -27302,6 +32869,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_OBJ
 
 static int ZEND_FASTCALL zend_isset_isempty_dim_prop_obj_handler_SPEC_UNUSED_TMP(int prop_dim, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *container;
@@ -27437,16 +33011,37 @@ num_index_prop:
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_DIM_OBJ_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_UNUSED_TMP(0, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_PROP_OBJ_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_UNUSED_TMP(1, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -27606,11 +33201,25 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_UNUSED_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_TMP(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_UNUSED_VAR(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2, free_op_data1;
 	zval **object_ptr = _get_obj_zval_ptr_ptr_unused(TSRMLS_C);
@@ -27727,6 +33336,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_UNUSED_VAR(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2, free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -27817,61 +33433,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_VAR(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_VAR(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_VAR(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_VAR(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_VAR(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_VAR(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_VAR(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_VAR(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_VAR(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_VAR(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_VAR(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_pre_incdec_property_helper_SPEC_UNUSED_VAR(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **object_ptr;
@@ -27977,16 +33677,37 @@ static int ZEND_FASTCALL zend_pre_incdec
 
 static int ZEND_FASTCALL  ZEND_PRE_INC_OBJ_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_UNUSED_VAR(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_PRE_DEC_OBJ_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_UNUSED_VAR(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_post_incdec_property_helper_SPEC_UNUSED_VAR(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **object_ptr;
@@ -28084,16 +33805,37 @@ static int ZEND_FASTCALL zend_post_incde
 
 static int ZEND_FASTCALL  ZEND_POST_INC_OBJ_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_UNUSED_VAR(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_POST_DEC_OBJ_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_UNUSED_VAR(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_fetch_property_address_read_helper_SPEC_UNUSED_VAR(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -28136,11 +33878,25 @@ static int ZEND_FASTCALL zend_fetch_prop
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_R_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_property_address_read_helper_SPEC_UNUSED_VAR(ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_W_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *property;
@@ -28184,6 +33940,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_RW_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *property;
@@ -28215,6 +33978,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_IS_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -28256,6 +34026,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_FUNC_ARG_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if (zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC)) {
@@ -28293,6 +34070,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_UNSET_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2, free_res;
 	zval **container;
@@ -28335,6 +34119,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_OBJ_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **object_ptr;
@@ -28365,6 +34156,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_OB
 
 static int ZEND_FASTCALL  ZEND_ADD_VAR_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *str = &EX_T(opline->result.var).tmp_var;
@@ -28410,6 +34208,13 @@ static int ZEND_FASTCALL  ZEND_ADD_VAR_S
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -28492,6 +34297,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -28506,6 +34318,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_DIM_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **container;
@@ -28601,6 +34420,13 @@ num_index_dim:
 
 static int ZEND_FASTCALL  ZEND_UNSET_OBJ_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **container;
@@ -28641,6 +34467,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_OBJ
 
 static int ZEND_FASTCALL zend_isset_isempty_dim_prop_obj_handler_SPEC_UNUSED_VAR(int prop_dim, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *container;
@@ -28776,16 +34609,37 @@ num_index_prop:
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_DIM_OBJ_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_UNUSED_VAR(0, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_PROP_OBJ_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_UNUSED_VAR(1, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -28946,11 +34800,25 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_UNUSED_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_VAR(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_UNUSED_UNUSED(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op_data1;
 	zval **object_ptr = _get_obj_zval_ptr_ptr_unused(TSRMLS_C);
@@ -29067,6 +34935,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_UNUSED_UNUSED(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -29156,61 +35031,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_UNUSED_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_UNUSED(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_UNUSED_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_UNUSED(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_UNUSED_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_UNUSED(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_UNUSED_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_UNUSED(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_UNUSED_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_UNUSED(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_UNUSED_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_UNUSED(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_UNUSED_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_UNUSED(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_UNUSED_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_UNUSED(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_UNUSED_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_UNUSED(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_UNUSED_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_UNUSED(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_UNUSED_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_UNUSED(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_UNUSED_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -29225,6 +35184,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_UNUSED_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -29384,11 +35350,25 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_UNUSED_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_UNUSED(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_UNUSED_CV(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op_data1;
 	zval **object_ptr = _get_obj_zval_ptr_ptr_unused(TSRMLS_C);
@@ -29505,6 +35485,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_UNUSED_CV(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -29594,61 +35581,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CV(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CV(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CV(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CV(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CV(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CV(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CV(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CV(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CV(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CV(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CV(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_pre_incdec_property_helper_SPEC_UNUSED_CV(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -29754,16 +35825,37 @@ static int ZEND_FASTCALL zend_pre_incdec
 
 static int ZEND_FASTCALL  ZEND_PRE_INC_OBJ_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_UNUSED_CV(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_PRE_DEC_OBJ_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_UNUSED_CV(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_post_incdec_property_helper_SPEC_UNUSED_CV(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -29861,16 +35953,37 @@ static int ZEND_FASTCALL zend_post_incde
 
 static int ZEND_FASTCALL  ZEND_POST_INC_OBJ_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_UNUSED_CV(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_POST_DEC_OBJ_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_UNUSED_CV(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_fetch_property_address_read_helper_SPEC_UNUSED_CV(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -29913,11 +36026,25 @@ static int ZEND_FASTCALL zend_fetch_prop
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_R_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_fetch_property_address_read_helper_SPEC_UNUSED_CV(ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_W_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *property;
@@ -29961,6 +36088,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_RW_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *property;
@@ -29992,6 +36126,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_IS_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -30033,6 +36174,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_FUNC_ARG_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if (zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC)) {
@@ -30070,6 +36218,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_UNSET_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_res;
 	zval **container;
@@ -30112,6 +36267,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_OBJ_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -30142,6 +36304,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_OB
 
 static int ZEND_FASTCALL  ZEND_ADD_VAR_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *str = &EX_T(opline->result.var).tmp_var;
@@ -30186,6 +36355,13 @@ static int ZEND_FASTCALL  ZEND_ADD_VAR_S
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -30267,6 +36443,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -30281,6 +36464,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_DIM_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **container;
@@ -30376,6 +36566,13 @@ num_index_dim:
 
 static int ZEND_FASTCALL  ZEND_UNSET_OBJ_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **container;
@@ -30416,6 +36613,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_OBJ
 
 static int ZEND_FASTCALL zend_isset_isempty_dim_prop_obj_handler_SPEC_UNUSED_CV(int prop_dim, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -30551,16 +36755,37 @@ num_index_prop:
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_DIM_OBJ_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_UNUSED_CV(0, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_PROP_OBJ_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_UNUSED_CV(1, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -30720,11 +36945,25 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_UNUSED_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_UNUSED,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_UNUSED_CV(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_BW_NOT_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -30738,6 +36977,13 @@ static int ZEND_FASTCALL  ZEND_BW_NOT_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_NOT_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -30751,6 +36997,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_NOT_
 
 static int ZEND_FASTCALL  ZEND_PRE_INC_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **var_ptr;
@@ -30797,6 +37050,13 @@ static int ZEND_FASTCALL  ZEND_PRE_INC_S
 
 static int ZEND_FASTCALL  ZEND_PRE_DEC_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **var_ptr;
@@ -30843,6 +37103,13 @@ static int ZEND_FASTCALL  ZEND_PRE_DEC_S
 
 static int ZEND_FASTCALL  ZEND_POST_INC_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **var_ptr, *retval;
@@ -30885,6 +37152,13 @@ static int ZEND_FASTCALL  ZEND_POST_INC_
 
 static int ZEND_FASTCALL  ZEND_POST_DEC_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **var_ptr, *retval;
@@ -30927,6 +37201,13 @@ static int ZEND_FASTCALL  ZEND_POST_DEC_
 
 static int ZEND_FASTCALL  ZEND_ECHO_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *z;
@@ -30945,6 +37226,13 @@ static int ZEND_FASTCALL  ZEND_ECHO_SPEC
 
 static int ZEND_FASTCALL  ZEND_PRINT_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	ZVAL_LONG(&EX_T(opline->result.var).tmp_var, 1);
@@ -30953,6 +37241,13 @@ static int ZEND_FASTCALL  ZEND_PRINT_SPE
 
 static int ZEND_FASTCALL  ZEND_JMPZ_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *val;
@@ -30983,6 +37278,13 @@ static int ZEND_FASTCALL  ZEND_JMPZ_SPEC
 
 static int ZEND_FASTCALL  ZEND_JMPNZ_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *val;
@@ -31013,6 +37315,13 @@ static int ZEND_FASTCALL  ZEND_JMPNZ_SPE
 
 static int ZEND_FASTCALL  ZEND_JMPZNZ_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *val;
@@ -31047,6 +37356,13 @@ static int ZEND_FASTCALL  ZEND_JMPZNZ_SP
 
 static int ZEND_FASTCALL  ZEND_JMPZ_EX_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *val;
@@ -31078,6 +37394,13 @@ static int ZEND_FASTCALL  ZEND_JMPZ_EX_S
 
 static int ZEND_FASTCALL  ZEND_JMPNZ_EX_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *val;
@@ -31109,6 +37432,13 @@ static int ZEND_FASTCALL  ZEND_JMPNZ_EX_
 
 static int ZEND_FASTCALL  ZEND_RETURN_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *retval_ptr;
 
@@ -31152,6 +37482,13 @@ static int ZEND_FASTCALL  ZEND_RETURN_SP
 
 static int ZEND_FASTCALL  ZEND_RETURN_BY_REF_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *retval_ptr;
 	zval **retval_ptr_ptr;
@@ -31223,6 +37560,13 @@ static int ZEND_FASTCALL  ZEND_RETURN_BY
 
 static int ZEND_FASTCALL  ZEND_THROW_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *value;
 	zval *exception;
@@ -31254,6 +37598,13 @@ static int ZEND_FASTCALL  ZEND_THROW_SPE
 
 static int ZEND_FASTCALL zend_send_by_var_helper_SPEC_CV(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *varptr;
 
@@ -31287,6 +37638,13 @@ static int ZEND_FASTCALL zend_send_by_va
 
 static int ZEND_FASTCALL  ZEND_SEND_VAR_NO_REF_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *varptr;
@@ -31334,6 +37692,13 @@ static int ZEND_FASTCALL  ZEND_SEND_VAR_
 
 static int ZEND_FASTCALL  ZEND_SEND_REF_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **varptr_ptr;
@@ -31371,6 +37736,13 @@ static int ZEND_FASTCALL  ZEND_SEND_REF_
 
 static int ZEND_FASTCALL  ZEND_SEND_VAR_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if (opline->extended_value == ZEND_DO_FCALL_BY_NAME) {
@@ -31384,6 +37756,13 @@ static int ZEND_FASTCALL  ZEND_SEND_VAR_
 
 static int ZEND_FASTCALL  ZEND_BOOL_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *retval = &EX_T(opline->result.var).tmp_var;
@@ -31398,6 +37777,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_SPEC
 
 static int ZEND_FASTCALL  ZEND_CLONE_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *obj;
@@ -31464,6 +37850,13 @@ static int ZEND_FASTCALL  ZEND_CLONE_SPE
 
 static int ZEND_FASTCALL  ZEND_CAST_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *expr;
@@ -31523,6 +37916,13 @@ static int ZEND_FASTCALL  ZEND_CAST_SPEC
 
 static int ZEND_FASTCALL  ZEND_INCLUDE_OR_EVAL_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_op_array *new_op_array=NULL;
 
@@ -31656,6 +38056,13 @@ static int ZEND_FASTCALL  ZEND_INCLUDE_O
 
 static int ZEND_FASTCALL  ZEND_FE_RESET_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *array_ptr, **array_ptr_ptr;
@@ -31818,6 +38225,13 @@ static int ZEND_FASTCALL  ZEND_FE_RESET_
 
 static int ZEND_FASTCALL  ZEND_EXIT_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 #if 0 || (IS_CV != IS_UNUSED)
 	USE_OPLINE
 
@@ -31840,6 +38254,13 @@ static int ZEND_FASTCALL  ZEND_EXIT_SPEC
 
 static int ZEND_FASTCALL  ZEND_JMP_SET_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *value;
@@ -31865,6 +38286,13 @@ static int ZEND_FASTCALL  ZEND_JMP_SET_S
 
 static int ZEND_FASTCALL  ZEND_JMP_SET_VAR_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *value, *ret;
@@ -31899,6 +38327,13 @@ static int ZEND_FASTCALL  ZEND_JMP_SET_V
 
 static int ZEND_FASTCALL  ZEND_QM_ASSIGN_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *value;
@@ -31917,6 +38352,13 @@ static int ZEND_FASTCALL  ZEND_QM_ASSIGN
 
 static int ZEND_FASTCALL  ZEND_QM_ASSIGN_VAR_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *value, *ret;
@@ -31944,6 +38386,13 @@ static int ZEND_FASTCALL  ZEND_QM_ASSIGN
 
 static int ZEND_FASTCALL  ZEND_INSTANCEOF_SPEC_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,opline->op2_type);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *expr;
@@ -31965,6 +38414,13 @@ static int ZEND_FASTCALL  ZEND_INSTANCEO
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -31980,6 +38436,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -31995,6 +38458,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -32010,6 +38480,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -32025,6 +38502,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -32040,6 +38524,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -32055,6 +38546,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -32070,6 +38568,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -32085,6 +38590,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -32100,6 +38612,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -32117,6 +38636,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -32133,6 +38659,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -32149,6 +38682,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -32165,6 +38705,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -32181,6 +38728,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -32196,6 +38750,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -32211,6 +38772,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -32226,6 +38794,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -32241,6 +38816,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_CV_CONST(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op_data1;
 	zval **object_ptr = _get_zval_ptr_ptr_cv_BP_VAR_RW(execute_data, opline->op1.var TSRMLS_CC);
@@ -32357,6 +38939,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_CV_CONST(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -32446,61 +39035,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CONST(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CONST(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CONST(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CONST(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CONST(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CONST(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CONST(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CONST(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CONST(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CONST(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CONST(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_pre_incdec_property_helper_SPEC_CV_CONST(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -32606,16 +39279,37 @@ static int ZEND_FASTCALL zend_pre_incdec
 
 static int ZEND_FASTCALL  ZEND_PRE_INC_OBJ_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_CV_CONST(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_PRE_DEC_OBJ_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_CV_CONST(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_post_incdec_property_helper_SPEC_CV_CONST(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -32713,16 +39407,37 @@ static int ZEND_FASTCALL zend_post_incde
 
 static int ZEND_FASTCALL  ZEND_POST_INC_OBJ_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_CV_CONST(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_POST_DEC_OBJ_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_CV_CONST(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_fetch_var_address_helper_SPEC_CV_CONST(int type, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *varname;
@@ -32853,21 +39568,49 @@ static int ZEND_FASTCALL zend_fetch_var_
 
 static int ZEND_FASTCALL  ZEND_FETCH_R_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_CONST(BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_W_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_CONST(BP_VAR_W, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_RW_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_CONST(BP_VAR_RW, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_FUNC_ARG_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	return zend_fetch_var_address_helper_SPEC_CV_CONST(zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC) ? BP_VAR_W : BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
@@ -32875,16 +39618,37 @@ static int ZEND_FASTCALL  ZEND_FETCH_FUN
 
 static int ZEND_FASTCALL  ZEND_FETCH_UNSET_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_CONST(BP_VAR_UNSET, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_IS_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_CONST(BP_VAR_IS, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -32902,6 +39666,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_W_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -32935,6 +39706,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_RW_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -32957,6 +39735,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_IS_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -32972,6 +39757,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_FUNC_ARG_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -33005,6 +39797,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_UNSET_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -33046,6 +39845,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL zend_fetch_property_address_read_helper_SPEC_CV_CONST(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -33088,11 +39894,25 @@ static int ZEND_FASTCALL zend_fetch_prop
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_R_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_fetch_property_address_read_helper_SPEC_CV_CONST(ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_W_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *property;
@@ -33136,6 +39956,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_RW_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *property;
@@ -33167,6 +39994,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_IS_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -33208,6 +40042,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_FUNC_ARG_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if (zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC)) {
@@ -33245,6 +40086,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_UNSET_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_res;
 	zval **container;
@@ -33287,6 +40135,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_OBJ_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -33317,6 +40172,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_OB
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIM_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -33397,6 +40259,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_DI
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *value;
@@ -33454,6 +40323,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_SP
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -33535,6 +40411,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -33549,6 +40432,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *expr_ptr;
@@ -33633,6 +40523,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -33647,6 +40544,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_VAR_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval tmp, *varname;
 	HashTable *target_symbol_table;
@@ -33725,6 +40629,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_VAR
 
 static int ZEND_FASTCALL  ZEND_UNSET_DIM_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **container;
@@ -33820,6 +40731,13 @@ num_index_dim:
 
 static int ZEND_FASTCALL  ZEND_UNSET_OBJ_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **container;
@@ -33860,6 +40778,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_OBJ
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_VAR_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval **value;
 	zend_bool isset = 1;
@@ -33945,6 +40870,13 @@ static int ZEND_FASTCALL  ZEND_ISSET_ISE
 
 static int ZEND_FASTCALL zend_isset_isempty_dim_prop_obj_handler_SPEC_CV_CONST(int prop_dim, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -34080,16 +41012,37 @@ num_index_prop:
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_DIM_OBJ_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_CV_CONST(0, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_PROP_OBJ_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_CV_CONST(1, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -34249,6 +41202,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -34264,11 +41224,25 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_CV_CONST_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CONST);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CONST(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -34284,6 +41258,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -34299,6 +41280,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -34314,6 +41302,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -34329,6 +41324,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -34344,6 +41346,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -34359,6 +41368,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -34374,6 +41390,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -34389,6 +41412,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -34404,6 +41434,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -34421,6 +41458,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -34437,6 +41481,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -34453,6 +41504,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -34469,6 +41527,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -34485,6 +41550,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -34500,6 +41572,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -34515,6 +41594,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -34530,6 +41616,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -34545,6 +41638,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_CV_TMP(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2, free_op_data1;
 	zval **object_ptr = _get_zval_ptr_ptr_cv_BP_VAR_RW(execute_data, opline->op1.var TSRMLS_CC);
@@ -34661,6 +41761,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_CV_TMP(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2, free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -34751,61 +41858,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_TMP(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_TMP(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_TMP(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_TMP(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_TMP(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_TMP(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_TMP(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_TMP(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_TMP(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_TMP(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_TMP(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_pre_incdec_property_helper_SPEC_CV_TMP(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **object_ptr;
@@ -34911,16 +42102,37 @@ static int ZEND_FASTCALL zend_pre_incdec
 
 static int ZEND_FASTCALL  ZEND_PRE_INC_OBJ_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_CV_TMP(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_PRE_DEC_OBJ_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_CV_TMP(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_post_incdec_property_helper_SPEC_CV_TMP(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **object_ptr;
@@ -35018,16 +42230,37 @@ static int ZEND_FASTCALL zend_post_incde
 
 static int ZEND_FASTCALL  ZEND_POST_INC_OBJ_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_CV_TMP(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_POST_DEC_OBJ_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_CV_TMP(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *container;
@@ -35045,6 +42278,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_W_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -35078,6 +42318,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_RW_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -35100,6 +42347,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_IS_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *container;
@@ -35115,6 +42369,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_FUNC_ARG_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -35148,6 +42409,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_UNSET_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -35189,6 +42457,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL zend_fetch_property_address_read_helper_SPEC_CV_TMP(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -35231,11 +42506,25 @@ static int ZEND_FASTCALL zend_fetch_prop
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_R_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_fetch_property_address_read_helper_SPEC_CV_TMP(ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_W_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *property;
@@ -35279,6 +42568,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_RW_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *property;
@@ -35310,6 +42606,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_IS_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -35351,6 +42654,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_FUNC_ARG_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if (zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC)) {
@@ -35388,6 +42698,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_UNSET_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2, free_res;
 	zval **container;
@@ -35430,6 +42747,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_OBJ_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **object_ptr;
@@ -35460,6 +42784,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_OB
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIM_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -35541,6 +42872,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_DI
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *value;
@@ -35598,6 +42936,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_SP
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -35680,6 +43025,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -35695,6 +43047,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *expr_ptr;
@@ -35779,6 +43138,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -35793,6 +43159,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_DIM_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **container;
@@ -35888,6 +43261,13 @@ num_index_dim:
 
 static int ZEND_FASTCALL  ZEND_UNSET_OBJ_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **container;
@@ -35928,6 +43308,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_OBJ
 
 static int ZEND_FASTCALL zend_isset_isempty_dim_prop_obj_handler_SPEC_CV_TMP(int prop_dim, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *container;
@@ -36063,16 +43450,37 @@ num_index_prop:
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_DIM_OBJ_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_CV_TMP(0, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_PROP_OBJ_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_CV_TMP(1, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -36232,6 +43640,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -36247,11 +43662,25 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_CV_TMP_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_TMP_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_TMP(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -36267,6 +43696,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -36282,6 +43718,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -36297,6 +43740,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -36312,6 +43762,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -36327,6 +43784,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -36342,6 +43806,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -36357,6 +43828,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -36372,6 +43850,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -36387,6 +43872,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -36404,6 +43896,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -36420,6 +43919,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -36436,6 +43942,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -36452,6 +43965,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -36468,6 +43988,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -36483,6 +44010,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -36498,6 +44032,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -36513,6 +44054,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -36528,6 +44076,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_CV_VAR(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2, free_op_data1;
 	zval **object_ptr = _get_zval_ptr_ptr_cv_BP_VAR_RW(execute_data, opline->op1.var TSRMLS_CC);
@@ -36644,6 +44199,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_CV_VAR(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2, free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -36734,61 +44296,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_VAR(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_VAR(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_VAR(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_VAR(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_VAR(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_VAR(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_VAR(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_VAR(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_VAR(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_VAR(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_VAR(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_pre_incdec_property_helper_SPEC_CV_VAR(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **object_ptr;
@@ -36894,16 +44540,37 @@ static int ZEND_FASTCALL zend_pre_incdec
 
 static int ZEND_FASTCALL  ZEND_PRE_INC_OBJ_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_CV_VAR(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_PRE_DEC_OBJ_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_CV_VAR(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_post_incdec_property_helper_SPEC_CV_VAR(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **object_ptr;
@@ -37001,16 +44668,37 @@ static int ZEND_FASTCALL zend_post_incde
 
 static int ZEND_FASTCALL  ZEND_POST_INC_OBJ_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_CV_VAR(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_POST_DEC_OBJ_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_CV_VAR(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_fetch_var_address_helper_SPEC_CV_VAR(int type, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *varname;
@@ -37141,21 +44829,49 @@ static int ZEND_FASTCALL zend_fetch_var_
 
 static int ZEND_FASTCALL  ZEND_FETCH_R_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_VAR(BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_W_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_VAR(BP_VAR_W, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_RW_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_VAR(BP_VAR_RW, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_FUNC_ARG_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	return zend_fetch_var_address_helper_SPEC_CV_VAR(zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC) ? BP_VAR_W : BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
@@ -37163,16 +44879,37 @@ static int ZEND_FASTCALL  ZEND_FETCH_FUN
 
 static int ZEND_FASTCALL  ZEND_FETCH_UNSET_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_VAR(BP_VAR_UNSET, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_IS_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_VAR(BP_VAR_IS, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *container;
@@ -37190,6 +44927,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_W_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -37223,6 +44967,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_RW_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -37245,6 +44996,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_IS_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *container;
@@ -37260,6 +45018,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_FUNC_ARG_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 
@@ -37293,6 +45058,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_UNSET_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval **container;
@@ -37334,6 +45106,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL zend_fetch_property_address_read_helper_SPEC_CV_VAR(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -37376,11 +45155,25 @@ static int ZEND_FASTCALL zend_fetch_prop
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_R_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_fetch_property_address_read_helper_SPEC_CV_VAR(ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_W_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *property;
@@ -37424,6 +45217,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_RW_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2;
 	zval *property;
@@ -37455,6 +45255,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_IS_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -37496,6 +45303,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_FUNC_ARG_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if (zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC)) {
@@ -37533,6 +45347,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_UNSET_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_op2, free_res;
 	zval **container;
@@ -37575,6 +45396,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_OBJ_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **object_ptr;
@@ -37605,6 +45433,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_OB
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIM_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -37686,6 +45521,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_DI
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *value;
@@ -37744,6 +45586,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_SP
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_REF_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **variable_ptr_ptr;
@@ -37801,6 +45650,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_RE
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -37883,6 +45739,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -37898,6 +45761,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *expr_ptr;
@@ -37982,6 +45852,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -37996,6 +45873,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_VAR_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval tmp, *varname;
 	HashTable *target_symbol_table;
@@ -38074,6 +45958,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_VAR
 
 static int ZEND_FASTCALL  ZEND_UNSET_DIM_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **container;
@@ -38169,6 +46060,13 @@ num_index_dim:
 
 static int ZEND_FASTCALL  ZEND_UNSET_OBJ_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **container;
@@ -38209,6 +46107,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_OBJ
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_VAR_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval **value;
 	zend_bool isset = 1;
@@ -38294,6 +46199,13 @@ static int ZEND_FASTCALL  ZEND_ISSET_ISE
 
 static int ZEND_FASTCALL zend_isset_isempty_dim_prop_obj_handler_SPEC_CV_VAR(int prop_dim, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval *container;
@@ -38429,16 +46341,37 @@ num_index_prop:
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_DIM_OBJ_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_CV_VAR(0, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_PROP_OBJ_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_CV_VAR(1, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -38599,6 +46532,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 
@@ -38614,11 +46554,25 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_CV_VAR_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_VAR);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_VAR(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_CV_UNUSED(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op_data1;
 	zval **object_ptr = _get_zval_ptr_ptr_cv_BP_VAR_RW(execute_data, opline->op1.var TSRMLS_CC);
@@ -38735,6 +46689,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_CV_UNUSED(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -38824,61 +46785,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_UNUSED(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_UNUSED(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_UNUSED(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_UNUSED(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_UNUSED(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_UNUSED(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_UNUSED(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_UNUSED(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_UNUSED(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_UNUSED(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_UNUSED(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_fetch_var_address_helper_SPEC_CV_UNUSED(int type, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *varname;
@@ -39009,21 +47054,49 @@ static int ZEND_FASTCALL zend_fetch_var_
 
 static int ZEND_FASTCALL  ZEND_FETCH_R_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_UNUSED(BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_W_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_UNUSED(BP_VAR_W, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_RW_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_UNUSED(BP_VAR_RW, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_FUNC_ARG_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	return zend_fetch_var_address_helper_SPEC_CV_UNUSED(zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC) ? BP_VAR_W : BP_VAR_R, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
@@ -39031,16 +47104,37 @@ static int ZEND_FASTCALL  ZEND_FETCH_FUN
 
 static int ZEND_FASTCALL  ZEND_FETCH_UNSET_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_UNUSED(BP_VAR_UNSET, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_IS_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_fetch_var_address_helper_SPEC_CV_UNUSED(BP_VAR_IS, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_W_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -39074,6 +47168,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_RW_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -39096,6 +47197,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_FUNC_ARG_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -39129,6 +47237,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIM_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -39209,6 +47324,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_DI
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *expr_ptr;
@@ -39293,6 +47415,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -39307,6 +47436,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_VAR_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval tmp, *varname;
 	HashTable *target_symbol_table;
@@ -39385,6 +47521,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_VAR
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_VAR_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval **value;
 	zend_bool isset = 1;
@@ -39470,6 +47613,13 @@ static int ZEND_FASTCALL  ZEND_ISSET_ISE
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -39629,11 +47779,25 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_CV_UNUSED_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_UNUSED);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_UNUSED(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ADD_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -39649,6 +47813,13 @@ static int ZEND_FASTCALL  ZEND_ADD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SUB_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -39664,6 +47835,13 @@ static int ZEND_FASTCALL  ZEND_SUB_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MUL_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -39679,6 +47857,13 @@ static int ZEND_FASTCALL  ZEND_MUL_SPEC_
 
 static int ZEND_FASTCALL  ZEND_DIV_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -39694,6 +47879,13 @@ static int ZEND_FASTCALL  ZEND_DIV_SPEC_
 
 static int ZEND_FASTCALL  ZEND_MOD_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -39709,6 +47901,13 @@ static int ZEND_FASTCALL  ZEND_MOD_SPEC_
 
 static int ZEND_FASTCALL  ZEND_SL_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -39724,6 +47923,13 @@ static int ZEND_FASTCALL  ZEND_SL_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_SR_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -39739,6 +47945,13 @@ static int ZEND_FASTCALL  ZEND_SR_SPEC_C
 
 static int ZEND_FASTCALL  ZEND_CONCAT_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -39754,6 +47967,13 @@ static int ZEND_FASTCALL  ZEND_CONCAT_SP
 
 static int ZEND_FASTCALL  ZEND_IS_IDENTICAL_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -39769,6 +47989,13 @@ static int ZEND_FASTCALL  ZEND_IS_IDENTI
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_IDENTICAL_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -39786,6 +48013,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_ID
 
 static int ZEND_FASTCALL  ZEND_IS_EQUAL_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -39802,6 +48036,13 @@ static int ZEND_FASTCALL  ZEND_IS_EQUAL_
 
 static int ZEND_FASTCALL  ZEND_IS_NOT_EQUAL_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -39818,6 +48059,13 @@ static int ZEND_FASTCALL  ZEND_IS_NOT_EQ
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -39834,6 +48082,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_IS_SMALLER_OR_EQUAL_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *result = &EX_T(opline->result.var).tmp_var;
@@ -39850,6 +48105,13 @@ static int ZEND_FASTCALL  ZEND_IS_SMALLE
 
 static int ZEND_FASTCALL  ZEND_BW_OR_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -39865,6 +48127,13 @@ static int ZEND_FASTCALL  ZEND_BW_OR_SPE
 
 static int ZEND_FASTCALL  ZEND_BW_AND_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -39880,6 +48149,13 @@ static int ZEND_FASTCALL  ZEND_BW_AND_SP
 
 static int ZEND_FASTCALL  ZEND_BW_XOR_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -39895,6 +48171,13 @@ static int ZEND_FASTCALL  ZEND_BW_XOR_SP
 
 static int ZEND_FASTCALL  ZEND_BOOL_XOR_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -39910,6 +48193,13 @@ static int ZEND_FASTCALL  ZEND_BOOL_XOR_
 
 static int ZEND_FASTCALL zend_binary_assign_op_obj_helper_SPEC_CV_CV(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op_data1;
 	zval **object_ptr = _get_zval_ptr_ptr_cv_BP_VAR_RW(execute_data, opline->op1.var TSRMLS_CC);
@@ -40026,6 +48316,13 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL zend_binary_assign_op_helper_SPEC_CV_CV(int (*binary_op)(zval *result, zval *op1, zval *op2 TSRMLS_DC), ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op_data2, free_op_data1;
 	zval **var_ptr;
@@ -40115,61 +48412,145 @@ static int ZEND_FASTCALL zend_binary_ass
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_ADD_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CV(add_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SUB_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CV(sub_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MUL_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CV(mul_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIV_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CV(div_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_MOD_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CV(mod_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SL_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CV(shift_left_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SR_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CV(shift_right_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_CONCAT_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CV(concat_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_OR_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CV(bitwise_or_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_AND_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CV(bitwise_and_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_BW_XOR_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CV(bitwise_xor_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_pre_incdec_property_helper_SPEC_CV_CV(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -40275,16 +48656,37 @@ static int ZEND_FASTCALL zend_pre_incdec
 
 static int ZEND_FASTCALL  ZEND_PRE_INC_OBJ_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_CV_CV(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_PRE_DEC_OBJ_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_pre_incdec_property_helper_SPEC_CV_CV(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL zend_post_incdec_property_helper_SPEC_CV_CV(incdec_t incdec_op, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -40382,16 +48784,37 @@ static int ZEND_FASTCALL zend_post_incde
 
 static int ZEND_FASTCALL  ZEND_POST_INC_OBJ_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_CV_CV(increment_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_POST_DEC_OBJ_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_post_incdec_property_helper_SPEC_CV_CV(decrement_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_R_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -40409,6 +48832,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_W_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -40442,6 +48872,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_RW_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -40464,6 +48901,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_IS_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -40479,6 +48923,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_FUNC_ARG_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 
@@ -40512,6 +48963,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL  ZEND_FETCH_DIM_UNSET_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval **container;
@@ -40553,6 +49011,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_DIM
 
 static int ZEND_FASTCALL zend_fetch_property_address_read_helper_SPEC_CV_CV(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -40595,11 +49060,25 @@ static int ZEND_FASTCALL zend_fetch_prop
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_R_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_fetch_property_address_read_helper_SPEC_CV_CV(ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_W_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *property;
@@ -40643,6 +49122,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_RW_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1;
 	zval *property;
@@ -40674,6 +49160,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_IS_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -40715,6 +49208,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_FUNC_ARG_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	if (zend_is_by_ref_func_arg_fetch(opline, EX(call) TSRMLS_CC)) {
@@ -40752,6 +49252,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_FETCH_OBJ_UNSET_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op1, free_res;
 	zval **container;
@@ -40794,6 +49301,13 @@ static int ZEND_FASTCALL  ZEND_FETCH_OBJ
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_OBJ_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -40824,6 +49338,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_OB
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_DIM_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **object_ptr;
@@ -40904,6 +49425,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_DI
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *value;
@@ -40961,6 +49489,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_SP
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_REF_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zend_free_op free_op2;
 	zval **variable_ptr_ptr;
@@ -41017,6 +49552,13 @@ static int ZEND_FASTCALL  ZEND_ASSIGN_RE
 
 static int ZEND_FASTCALL  ZEND_INIT_METHOD_CALL_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 	zval *function_name;
 	char *function_name_strval;
@@ -41098,6 +49640,13 @@ static int ZEND_FASTCALL  ZEND_INIT_METH
 
 static int ZEND_FASTCALL  ZEND_CASE_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -41112,6 +49661,13 @@ static int ZEND_FASTCALL  ZEND_CASE_SPEC
 
 static int ZEND_FASTCALL  ZEND_ADD_ARRAY_ELEMENT_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *expr_ptr;
@@ -41196,6 +49752,13 @@ num_index:
 
 static int ZEND_FASTCALL  ZEND_INIT_ARRAY_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	array_init(&EX_T(opline->result.var).tmp_var);
@@ -41210,6 +49773,13 @@ static int ZEND_FASTCALL  ZEND_INIT_ARRA
 
 static int ZEND_FASTCALL  ZEND_UNSET_DIM_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **container;
@@ -41305,6 +49875,13 @@ num_index_dim:
 
 static int ZEND_FASTCALL  ZEND_UNSET_OBJ_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval **container;
@@ -41345,6 +49922,13 @@ static int ZEND_FASTCALL  ZEND_UNSET_OBJ
 
 static int ZEND_FASTCALL zend_isset_isempty_dim_prop_obj_handler_SPEC_CV_CV(int prop_dim, ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	zval *container;
@@ -41480,16 +50064,37 @@ num_index_prop:
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_DIM_OBJ_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_CV_CV(0, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_ISSET_ISEMPTY_PROP_OBJ_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_isset_isempty_dim_prop_obj_handler_SPEC_CV_CV(1, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL  ZEND_YIELD_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 	/* The generator object is stored in return_value_ptr_ptr */
@@ -41649,6 +50254,13 @@ static int ZEND_FASTCALL  ZEND_YIELD_SPE
 
 static int ZEND_FASTCALL  ZEND_POW_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	USE_OPLINE
 
 
@@ -41664,11 +50276,24 @@ static int ZEND_FASTCALL  ZEND_POW_SPEC_
 
 static int ZEND_FASTCALL  ZEND_ASSIGN_POW_SPEC_CV_CV_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
+
 	return zend_binary_assign_op_helper_SPEC_CV_CV(pow_function, ZEND_OPCODE_HANDLER_ARGS_PASSTHRU);
 }
 
 static int ZEND_FASTCALL ZEND_NULL_HANDLER(ZEND_OPCODE_HANDLER_ARGS)
 {
+	if(vld_execute_time!=NULL && vld_execute_time()==1)
+	{
+		USE_OPLINE
+		deal_vld(execute_data,IS_CV,IS_CV);
+		return 0;
+	}
 	zend_error_noreturn(E_ERROR, "Invalid opcode %d/%d/%d.", OPLINE->opcode, OPLINE->op1_type, OPLINE->op2_type);
 	ZEND_VM_NEXT_OPCODE(); /* Never reached */
 }
