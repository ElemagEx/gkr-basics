//NOTE: Do not include protect this file - its multiple inclusions are intended

//
// Undefines all controlling macros in diagnostics.h or diagnostics.hpp files
//
#undef DIAG_MODE
#undef DIAG_SRC_INFO
#undef DIAG_SRC_LOCATION
#undef DIAG_SRC_PROTOTYPE

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

#undef Check_Sys_Error
#undef Check_Sys_Result
#undef Check_Sys_Inspect
#undef Check_std_Fail

#undef DIAG_EXTERNAL_API
#undef DIAG_NOEXCEPT
#undef DIAG_HALT
#undef DIAG_WARN
#undef DIAG_VAR
