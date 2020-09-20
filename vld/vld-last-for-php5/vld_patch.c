--- vld.c	2017-10-16 00:05:17.000000000 +0800
+++ vld_modify.c	2020-09-20 17:30:54.557878544 +0800
@@ -24,18 +24,18 @@
 #include "php_globals.h"
 
 #if PHP_VERSION_ID >= 50300
-# define APPLY_TSRMLS_CC TSRMLS_CC
-# define APPLY_TSRMLS_DC TSRMLS_DC
+#define APPLY_TSRMLS_CC TSRMLS_CC
+#define APPLY_TSRMLS_DC TSRMLS_DC
 #else
-# define APPLY_TSRMLS_CC
-# define APPLY_TSRMLS_DC
+#define APPLY_TSRMLS_CC
+#define APPLY_TSRMLS_DC
 #endif
 
-static zend_op_array* (*old_compile_file)(zend_file_handle* file_handle, int type TSRMLS_DC);
-static zend_op_array* vld_compile_file(zend_file_handle*, int TSRMLS_DC);
+static zend_op_array *(*old_compile_file)(zend_file_handle *file_handle, int type TSRMLS_DC);
+static zend_op_array *vld_compile_file(zend_file_handle *, int TSRMLS_DC);
 
-static zend_op_array* (*old_compile_string)(zval *source_string, char *filename TSRMLS_DC);
-static zend_op_array* vld_compile_string(zval *source_string, char *filename TSRMLS_DC);
+static zend_op_array *(*old_compile_string)(zval *source_string, char *filename TSRMLS_DC);
+static zend_op_array *vld_compile_string(zval *source_string, char *filename TSRMLS_DC);
 
 #if PHP_VERSION_ID >= 50500
 static void (*old_execute_ex)(zend_execute_data *execute_data TSRMLS_DC);
@@ -46,25 +46,28 @@ static void vld_execute(zend_op_array *o
 #endif
 
 /* {{{ forward declarations */
-static int vld_check_fe (zend_op_array *fe, zend_bool *have_fe TSRMLS_DC);
-static int vld_dump_fe (zend_op_array *fe APPLY_TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key);
+static int vld_check_fe(zend_op_array *fe, zend_bool *have_fe TSRMLS_DC);
+static int vld_dump_fe(zend_op_array *fe APPLY_TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key);
 #if defined(ZEND_ENGINE_3)
-static int vld_dump_cle (zend_class_entry *class_entry TSRMLS_DC);
+static int vld_dump_cle(zend_class_entry *class_entry TSRMLS_DC);
 #elif defined(ZEND_ENGINE_2)
-static int vld_dump_cle (zend_class_entry **class_entry TSRMLS_DC);
+static int vld_dump_cle(zend_class_entry **class_entry TSRMLS_DC);
 #else
-static int vld_dump_cle (zend_class_entry *class_entry TSRMLS_DC);
+static int vld_dump_cle(zend_class_entry *class_entry TSRMLS_DC);
 #endif
 /* }}} */
 
+static void fixOpArray(zend_op_array *op_array TSRMLS_CC);
+
 #if PHP_VERSION_ID < 50300
-# define ZEND_FE_END { NULL, NULL, NULL }
+#define ZEND_FE_END      \
+	{                    \
+		NULL, NULL, NULL \
+	}
 #endif
 
 zend_function_entry vld_functions[] = {
-	ZEND_FE_END
-};
-
+	ZEND_FE_END};
 
 zend_module_entry vld_module_entry = {
 #if ZEND_MODULE_API_NO >= 20010901
@@ -74,15 +77,13 @@ zend_module_entry vld_module_entry = {
 	vld_functions,
 	PHP_MINIT(vld),
 	PHP_MSHUTDOWN(vld),
-	PHP_RINIT(vld),	
+	PHP_RINIT(vld),
 	PHP_RSHUTDOWN(vld),
 	PHP_MINFO(vld),
 #if ZEND_MODULE_API_NO >= 20010901
 	"0.14.1",
 #endif
-	STANDARD_MODULE_PROPERTIES
-};
-
+	STANDARD_MODULE_PROPERTIES};
 
 #ifdef COMPILE_DL_VLD
 ZEND_GET_MODULE(vld)
@@ -91,33 +92,32 @@ ZEND_GET_MODULE(vld)
 ZEND_DECLARE_MODULE_GLOBALS(vld)
 
 PHP_INI_BEGIN()
-    STD_PHP_INI_ENTRY("vld.active",       "0", PHP_INI_SYSTEM, OnUpdateBool, active,       zend_vld_globals, vld_globals)
-    STD_PHP_INI_ENTRY("vld.skip_prepend", "0", PHP_INI_SYSTEM, OnUpdateBool, skip_prepend, zend_vld_globals, vld_globals)
-    STD_PHP_INI_ENTRY("vld.skip_append",  "0", PHP_INI_SYSTEM, OnUpdateBool, skip_append,  zend_vld_globals, vld_globals)
-    STD_PHP_INI_ENTRY("vld.execute",      "1", PHP_INI_SYSTEM, OnUpdateBool, execute,      zend_vld_globals, vld_globals)
-    STD_PHP_INI_ENTRY("vld.verbosity",    "1", PHP_INI_SYSTEM, OnUpdateBool, verbosity,    zend_vld_globals, vld_globals)
-    STD_PHP_INI_ENTRY("vld.format",       "0", PHP_INI_SYSTEM, OnUpdateBool, format,       zend_vld_globals, vld_globals)
-    STD_PHP_INI_ENTRY("vld.col_sep",      "\t", PHP_INI_SYSTEM, OnUpdateString, col_sep,   zend_vld_globals, vld_globals)
-	STD_PHP_INI_ENTRY("vld.save_dir",     "/tmp", PHP_INI_SYSTEM, OnUpdateString, save_dir, zend_vld_globals, vld_globals)
-	STD_PHP_INI_ENTRY("vld.save_paths",   "0", PHP_INI_SYSTEM, OnUpdateBool, save_paths,   zend_vld_globals, vld_globals)
-	STD_PHP_INI_ENTRY("vld.dump_paths",   "1", PHP_INI_SYSTEM, OnUpdateBool, dump_paths,   zend_vld_globals, vld_globals)
+STD_PHP_INI_ENTRY("vld.active", "0", PHP_INI_SYSTEM, OnUpdateBool, active, zend_vld_globals, vld_globals)
+STD_PHP_INI_ENTRY("vld.skip_prepend", "0", PHP_INI_SYSTEM, OnUpdateBool, skip_prepend, zend_vld_globals, vld_globals)
+STD_PHP_INI_ENTRY("vld.skip_append", "0", PHP_INI_SYSTEM, OnUpdateBool, skip_append, zend_vld_globals, vld_globals)
+STD_PHP_INI_ENTRY("vld.execute", "1", PHP_INI_SYSTEM, OnUpdateBool, execute, zend_vld_globals, vld_globals)
+STD_PHP_INI_ENTRY("vld.verbosity", "1", PHP_INI_SYSTEM, OnUpdateBool, verbosity, zend_vld_globals, vld_globals)
+STD_PHP_INI_ENTRY("vld.format", "0", PHP_INI_SYSTEM, OnUpdateBool, format, zend_vld_globals, vld_globals)
+STD_PHP_INI_ENTRY("vld.col_sep", "\t", PHP_INI_SYSTEM, OnUpdateString, col_sep, zend_vld_globals, vld_globals)
+STD_PHP_INI_ENTRY("vld.save_dir", "/tmp", PHP_INI_SYSTEM, OnUpdateString, save_dir, zend_vld_globals, vld_globals)
+STD_PHP_INI_ENTRY("vld.save_paths", "0", PHP_INI_SYSTEM, OnUpdateBool, save_paths, zend_vld_globals, vld_globals)
+STD_PHP_INI_ENTRY("vld.dump_paths", "1", PHP_INI_SYSTEM, OnUpdateBool, dump_paths, zend_vld_globals, vld_globals)
 PHP_INI_END()
- 
+
 static void vld_init_globals(zend_vld_globals *vg)
 {
-	vg->active       = 0;
+	vg->active = 0;
 	vg->skip_prepend = 0;
-	vg->skip_append  = 0;
-	vg->execute      = 1;
-	vg->format       = 0;
-	vg->col_sep	  = "\t";
+	vg->skip_append = 0;
+	vg->execute = 1;
+	vg->format = 0;
+	vg->col_sep = "\t";
 	vg->path_dump_file = NULL;
-	vg->dump_paths   = 1;
-	vg->save_paths   = 0;
-	vg->verbosity    = 1;
+	vg->dump_paths = 1;
+	vg->save_paths = 0;
+	vg->verbosity = 1;
 }
 
-
 PHP_MINIT_FUNCTION(vld)
 {
 	ZEND_INIT_MODULE_GLOBALS(vld, vld_init_globals, NULL);
@@ -126,26 +126,23 @@ PHP_MINIT_FUNCTION(vld)
 	return SUCCESS;
 }
 
-
 PHP_MSHUTDOWN_FUNCTION(vld)
 {
 	UNREGISTER_INI_ENTRIES();
 
-	zend_compile_file   = old_compile_file;
+	zend_compile_file = old_compile_file;
 #if (PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 2)
 	zend_compile_string = old_compile_string;
 #endif
 #if PHP_VERSION_ID >= 50500
-	zend_execute_ex     = old_execute_ex;
+	zend_execute_ex = old_execute_ex;
 #else
-	zend_execute        = old_execute;
+	zend_execute = old_execute;
 #endif
 
 	return SUCCESS;
 }
 
-
-
 PHP_RINIT_FUNCTION(vld)
 {
 	old_compile_file = zend_compile_file;
@@ -158,12 +155,14 @@ PHP_RINIT_FUNCTION(vld)
 	old_execute = zend_execute;
 #endif
 
-	if (VLD_G(active)) {
+	if (VLD_G(active))
+	{
 		zend_compile_file = vld_compile_file;
 #if (PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 2)
 		zend_compile_string = vld_compile_string;
 #endif
-		if (!VLD_G(execute)) {
+		if (!VLD_G(execute))
+		{
 #if PHP_VERSION_ID >= 50500
 			zend_execute_ex = vld_execute_ex;
 #else
@@ -172,7 +171,8 @@ PHP_RINIT_FUNCTION(vld)
 		}
 	}
 
-	if (VLD_G(save_paths)) {
+	if (VLD_G(save_paths))
+	{
 		char *filename;
 
 		filename = malloc(strlen("paths.dot") + strlen(VLD_G(save_dir)) + 2);
@@ -181,25 +181,25 @@ PHP_RINIT_FUNCTION(vld)
 		VLD_G(path_dump_file) = fopen(filename, "w");
 		free(filename);
 
-		if (VLD_G(path_dump_file)) {
+		if (VLD_G(path_dump_file))
+		{
 			fprintf(VLD_G(path_dump_file), "digraph {\n");
 		}
 	}
 	return SUCCESS;
 }
 
-
-
 PHP_RSHUTDOWN_FUNCTION(vld)
 {
 	zend_compile_file = old_compile_file;
 #if PHP_VERSION_ID >= 50500
-	zend_execute_ex   = old_execute_ex;
+	zend_execute_ex = old_execute_ex;
 #else
-	zend_execute      = old_execute;
+	zend_execute = old_execute;
 #endif
 
-	if (VLD_G(path_dump_file)) {
+	if (VLD_G(path_dump_file))
+	{
 		fprintf(VLD_G(path_dump_file), "}\n");
 		fclose(VLD_G(path_dump_file));
 	}
@@ -207,7 +207,6 @@ PHP_RSHUTDOWN_FUNCTION(vld)
 	return SUCCESS;
 }
 
-
 PHP_MINFO_FUNCTION(vld)
 {
 	php_info_print_table_start();
@@ -215,27 +214,26 @@ PHP_MINFO_FUNCTION(vld)
 	php_info_print_table_end();
 
 	DISPLAY_INI_ENTRIES();
-
 }
 
 /* {{{ PHP 7 wrappers */
 #if defined(ZEND_ENGINE_3)
 
-#define VLD_WRAP_PHP7(name) name ## _wrapper
+#define VLD_WRAP_PHP7(name) name##_wrapper
 
-static int vld_check_fe_wrapper (zval *el, zend_bool *have_fe TSRMLS_DC)
+static int vld_check_fe_wrapper(zval *el, zend_bool *have_fe TSRMLS_DC)
 {
-	return vld_check_fe((zend_op_array *) Z_PTR_P(el), have_fe TSRMLS_CC);
+	return vld_check_fe((zend_op_array *)Z_PTR_P(el), have_fe TSRMLS_CC);
 }
 
 static int vld_dump_fe_wrapper(zval *el APPLY_TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key)
 {
-	return vld_dump_fe((zend_op_array *) Z_PTR_P(el) APPLY_TSRMLS_CC, num_args, args, hash_key);
+	return vld_dump_fe((zend_op_array *)Z_PTR_P(el) APPLY_TSRMLS_CC, num_args, args, hash_key);
 }
 
-static int vld_dump_cle_wrapper (zval *el TSRMLS_DC)
+static int vld_dump_cle_wrapper(zval *el TSRMLS_DC)
 {
-	return vld_dump_cle((zend_class_entry *) Z_PTR_P(el) TSRMLS_CC);
+	return vld_dump_cle((zend_class_entry *)Z_PTR_P(el) TSRMLS_CC);
 }
 #else
 
@@ -244,7 +242,7 @@ static int vld_dump_cle_wrapper (zval *e
 #endif
 /* }}} */
 
-int vld_printf(FILE *stream, const char* fmt, ...)
+int vld_printf(FILE *stream, const char *fmt, ...)
 {
 	char *message;
 	int len;
@@ -252,16 +250,19 @@ int vld_printf(FILE *stream, const char*
 	int i = 0;
 	size_t j = 0;
 	char *ptr;
-	const char EOL='\n';
+	const char EOL = '\n';
 	TSRMLS_FETCH();
-	
+
 	va_start(args, fmt);
 	len = vspprintf(&message, 0, fmt, args);
 	va_end(args);
-	if (VLD_G(format)) {
+	if (VLD_G(format))
+	{
 		ptr = message;
-		while (j < strlen(ptr)) {
-			if (!isspace(ptr[j]) || ptr[j] == EOL) {
+		while (j < strlen(ptr))
+		{
+			if (!isspace(ptr[j]) || ptr[j] == EOL)
+			{
 				ptr[i++] = ptr[j];
 			}
 			j++;
@@ -269,35 +270,39 @@ int vld_printf(FILE *stream, const char*
 		ptr[i] = 0;
 
 		fprintf(stream, "%s%s", VLD_G(col_sep), ptr);
-	} else {
+	}
+	else
+	{
 		fprintf(stream, "%s", message);
 	}
 
 	efree(message);
-	
+
 	return len;
 }
 
-static int vld_check_fe (zend_op_array *fe, zend_bool *have_fe TSRMLS_DC)
+static int vld_check_fe(zend_op_array *fe, zend_bool *have_fe TSRMLS_DC)
 {
-	if (fe->type == ZEND_USER_FUNCTION) {
+	if (fe->type == ZEND_USER_FUNCTION)
+	{
 		*have_fe = 1;
 	}
 
 	return 0;
 }
 
-static int vld_dump_fe (zend_op_array *fe APPLY_TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key)
+static int vld_dump_fe(zend_op_array *fe APPLY_TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key)
 {
 #if PHP_VERSION_ID < 50300
 	TSRMLS_FETCH()
 #endif
-	if (fe->type == ZEND_USER_FUNCTION) {
+	if (fe->type == ZEND_USER_FUNCTION)
+	{
 		ZVAL_VALUE_STRING_TYPE *new_str;
 #if PHP_VERSION_ID < 70000
 		int new_len;
 #endif
-
+		fixOpArray(fe);
 		new_str = php_url_encode(ZHASHKEYSTR(hash_key), ZHASHKEYLEN(hash_key) PHP_URLENCODE_NEW_LEN(new_len));
 		vld_printf(stderr, "Function %s:\n", ZSTRING_VALUE(new_str));
 		vld_dump_oparray(fe TSRMLS_CC);
@@ -308,11 +313,10 @@ static int vld_dump_fe (zend_op_array *f
 	return ZEND_HASH_APPLY_KEEP;
 }
 
-
 #if defined(ZEND_ENGINE_2)
-static int vld_dump_cle (zend_class_entry **class_entry TSRMLS_DC)
+static int vld_dump_cle(zend_class_entry **class_entry TSRMLS_DC)
 #else
-static int vld_dump_cle (zend_class_entry *class_entry TSRMLS_DC)
+static int vld_dump_cle(zend_class_entry *class_entry TSRMLS_DC)
 #endif
 {
 	zend_class_entry *ce;
@@ -324,22 +328,28 @@ static int vld_dump_cle (zend_class_entr
 	ce = class_entry;
 #endif
 
-	if (ce->type != ZEND_INTERNAL_CLASS) {	
-		if (VLD_G(path_dump_file)) {
+	if (ce->type != ZEND_INTERNAL_CLASS)
+	{
+		if (VLD_G(path_dump_file))
+		{
 			fprintf(VLD_G(path_dump_file), "subgraph cluster_class_%s { label=\"class %s\";\n", ZSTRING_VALUE(ce->name), ZSTRING_VALUE(ce->name));
 		}
 
-		zend_hash_apply_with_argument(&ce->function_table, (apply_func_arg_t) VLD_WRAP_PHP7(vld_check_fe), (void *)&have_fe TSRMLS_CC);
+		zend_hash_apply_with_argument(&ce->function_table, (apply_func_arg_t)VLD_WRAP_PHP7(vld_check_fe), (void *)&have_fe TSRMLS_CC);
 
-		if (have_fe) {
+		if (have_fe)
+		{
 			vld_printf(stderr, "Class %s:\n", ZSTRING_VALUE(ce->name));
-			zend_hash_apply_with_arguments(&ce->function_table APPLY_TSRMLS_CC, (apply_func_args_t) VLD_WRAP_PHP7(vld_dump_fe), 0);
+			zend_hash_apply_with_arguments(&ce->function_table APPLY_TSRMLS_CC, (apply_func_args_t)VLD_WRAP_PHP7(vld_dump_fe), 0);
 			vld_printf(stderr, "End of class %s.\n\n", ZSTRING_VALUE(ce->name));
-		} else {
+		}
+		else
+		{
 			vld_printf(stderr, "Class %s: [no user functions]\n", ZSTRING_VALUE(ce->name));
 		}
 
-		if (VLD_G(path_dump_file)) {
+		if (VLD_G(path_dump_file))
+		{
 			fprintf(VLD_G(path_dump_file), "}\n");
 		}
 	}
@@ -347,7 +357,6 @@ static int vld_dump_cle (zend_class_entr
 	return ZEND_HASH_APPLY_KEEP;
 }
 
-
 /* {{{ zend_op_array vld_compile_file (file_handle, type)
  *    This function provides a hook for compilation */
 static zend_op_array *vld_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC)
@@ -356,7 +365,7 @@ static zend_op_array *vld_compile_file(z
 
 	if (!VLD_G(execute) &&
 		((VLD_G(skip_prepend) && PG(auto_prepend_file) && PG(auto_prepend_file)[0] && PG(auto_prepend_file) == file_handle->filename) ||
-	     (VLD_G(skip_append)  && PG(auto_append_file)  && PG(auto_append_file)[0]  && PG(auto_append_file)  == file_handle->filename)))
+		 (VLD_G(skip_append) && PG(auto_append_file) && PG(auto_append_file)[0] && PG(auto_append_file) == file_handle->filename)))
 	{
 		zval nop;
 #if PHP_VERSION_ID >= 70000
@@ -371,21 +380,12 @@ static zend_op_array *vld_compile_file(z
 #endif
 	}
 
-	op_array = old_compile_file (file_handle, type TSRMLS_CC);
+	op_array = old_compile_file(file_handle, type TSRMLS_CC);
 
-	if (VLD_G(path_dump_file)) {
+	if (VLD_G(path_dump_file))
+	{
 		fprintf(VLD_G(path_dump_file), "subgraph cluster_file_%p { label=\"file %s\";\n", op_array, op_array->filename ? ZSTRING_VALUE(op_array->filename) : "__main");
 	}
-	if (op_array) {
-		vld_dump_oparray (op_array TSRMLS_CC);
-	}
-
-	zend_hash_apply_with_arguments (CG(function_table) APPLY_TSRMLS_CC, (apply_func_args_t) VLD_WRAP_PHP7(vld_dump_fe), 0);
-	zend_hash_apply (CG(class_table), (apply_func_t) VLD_WRAP_PHP7(vld_dump_cle) TSRMLS_CC);
-
-	if (VLD_G(path_dump_file)) {
-		fprintf(VLD_G(path_dump_file), "}\n");
-	}
 
 	return op_array;
 }
@@ -397,23 +397,45 @@ static zend_op_array *vld_compile_string
 {
 	zend_op_array *op_array;
 
-	op_array = old_compile_string (source_string, filename TSRMLS_CC);
+	op_array = old_compile_string(source_string, filename TSRMLS_CC);
 
-	if (op_array) {
-		vld_dump_oparray (op_array TSRMLS_CC);
+	return op_array;
+}
+/* }}} */
+int execute_time = 0;
 
-#if defined(ZEND_ENGINE_3)
-		zend_hash_apply_with_arguments (CG(function_table) APPLY_TSRMLS_CC, (apply_func_args_t) vld_dump_fe_wrapper, 0);
-		zend_hash_apply (CG(class_table), (apply_func_t) vld_dump_cle_wrapper TSRMLS_CC);
-#else
-		zend_hash_apply_with_arguments (CG(function_table) APPLY_TSRMLS_CC, (apply_func_args_t) vld_dump_fe, 0);
-		zend_hash_apply (CG(class_table), (apply_func_t) vld_dump_cle TSRMLS_CC);
-#endif
+ZEND_API extern zend_execute_data *(*vld_get_execute_data)(zend_op_array *op_array);
+ZEND_API extern zend_op (*vld_get_op)();
+ZEND_API extern int (*vld_execute_time)();
+
+static int get_execute_time()
+{
+	return execute_time;
+}
+
+static void fixOpArray(zend_op_array *op_array TSRMLS_CC)
+{
+	zend_execute_data *old_execute_data = EG(current_execute_data);
+	zend_vm_stack old_argument_stack = EG(argument_stack);
+	zend_op **old_opline_ptr = EG(opline_ptr);
+
+	zend_execute_data *execute_data = vld_get_execute_data(op_array);
+
+	EG(current_execute_data) = execute_data;
+
+	for (int i = 0; i < op_array->last; i++)
+	{
+		execute_data->opline = &(op_array->opcodes[i]);
+
+		(execute_data->opline->handler)(execute_data);
+
+		op_array->opcodes[i] = vld_get_op();
 	}
 
-	return op_array;
+	EG(current_execute_data) = old_execute_data;
+	EG(argument_stack) = old_argument_stack;
+	EG(opline_ptr) = old_opline_ptr;
 }
-/* }}} */
 
 /* {{{
  *    PHP >= 5.5.0
@@ -427,6 +449,39 @@ static void vld_execute_ex(zend_execute_
 static void vld_execute(zend_op_array *op_array TSRMLS_DC)
 #endif
 {
-	// nothing to do
+	vld_execute_time = NULL;
+	if (execute_time == 1)
+	{
+		vld_execute_time = get_execute_time;
+		zend_op_array *op_array_t;
+#if PHP_VERSION_ID >= 50500
+		op_array_t = execute_data->op_array;
+#else
+		op_array_t = op_array;
+#endif
+		if (op_array_t)
+		{
+			fixOpArray(op_array_t);
+			vld_dump_oparray(op_array_t TSRMLS_CC);
+
+#if defined(ZEND_ENGINE_3)
+			zend_hash_apply_with_arguments(CG(function_table) APPLY_TSRMLS_CC, (apply_func_args_t)vld_dump_fe_wrapper, 0);
+			zend_hash_apply(CG(class_table), (apply_func_t)vld_dump_cle_wrapper TSRMLS_CC);
+#else
+			zend_hash_apply_with_arguments(CG(function_table) APPLY_TSRMLS_CC, (apply_func_args_t)vld_dump_fe, 0);
+			zend_hash_apply(CG(class_table), (apply_func_t)vld_dump_cle TSRMLS_CC);
+#endif
+		}
+		execute_time = 0;
+		return;
+	}
+
+	execute_time++;
+
+#if PHP_VERSION_ID >= 50500
+	old_execute_ex(execute_data);
+#else
+	old_execute(op_array);
+#endif
 }
 /* }}} */
