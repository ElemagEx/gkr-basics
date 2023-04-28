//NOTE: Do not include protect this file - its multiple inclusions are intended

//
// Undefines all macros defined in diag/defines.h file (with exception of DIAG_PROTOTYPES and DIAG_UNIT_TESTING)
//
#undef DIAG_MODE
#undef DIAG_MODE_DISABLED
#undef DIAG_MODE_SILENT
#undef DIAG_MODE_STEADY
#undef DIAG_MODE_NOISY
#undef DIAG_MODE_INTRUSIVE

#undef DIAG_SRC_INFO
#undef DIAG_SRC_INFO_DISABLED
#undef DIAG_SRC_INFO_PREPROCESSOR
#undef DIAG_SRC_INFO_SOURCE_LOCATION
#undef DIAG_SRC_INFO_STACKTRACE

#undef DIAG_SRC_LOCATION
#undef DIAG_SRC_PROTOTYPE

#undef DIAG_ID_ASSERT_NULL_PTR
#undef DIAG_ID_ASSERT_CHECK
#undef DIAG_ID_ASSERT_COND_MSG
#undef DIAG_ID_ASSERT_FAILURE
#undef DIAG_ID_ASSERT_FAIL_MSG

#undef DIAG_ID_CHECK_NULL_PTR
#undef DIAG_ID_CHECK_STATE
#undef DIAG_ID_CHECK_FAIL_MSG
#undef DIAG_ID_CHECK_FAILURE
#undef DIAG_ID_CHECK_RECOVERY
#undef DIAG_ID_ARG_NOT_VALID
#undef DIAG_ID_ARG_NOT_NULL
#undef DIAG_ID_ARG_INVALID
#undef DIAG_ID_ARG_BAD_ARRAY
#undef DIAG_ID_COUNT

#undef Assert_NotNullPtr
#undef Assert_Check
#undef Assert_CheckMsg
#undef Assert_Failure
#undef Assert_FailureMsg

#undef Check_NotNullPtr
#undef Check_ValidState
#undef Check_FailureMsg
#undef Check_Failure
#undef Check_Recovery

#undef Check_Arg_IsValid
#undef Check_Arg_NotNull
#undef Check_Arg_Invalid
#undef Check_Arg_Array

#undef DIAG_EXTERNAL_API
#undef DIAG_NOEXCEPT
#undef DIAG_HALT
#undef DIAG_STOP
#undef DIAG_WARN
