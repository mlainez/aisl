(* AISL Bytecode Definition *)

type opcode =
  (* Stack operations *)
  | OpPushInt of int64
  | OpPushFloat of float
  | OpPushString of string
  | OpPushBool of bool
  | OpPushDecimal of string
  | OpPushUnit
  | OpPop
  | OpDup
  (* Local variables *)
  | OpLoadLocal of int  (* index *)
  | OpStoreLocal of int
  (* Arithmetic - int *)
  | OpAddInt
  | OpSubInt
  | OpMulInt
  | OpDivInt
  | OpModInt
  | OpNegInt
  (* Arithmetic - float *)
  | OpAddFloat
  | OpSubFloat
  | OpMulFloat
  | OpDivFloat
  | OpNegFloat
  (* Arithmetic - decimal *)
  | OpAddDecimal
  | OpSubDecimal
  | OpMulDecimal
  | OpDivDecimal
  | OpNegDecimal
  (* Comparison - int *)
  | OpEqInt
  | OpNeInt
  | OpLtInt
  | OpGtInt
  | OpLeInt
  | OpGeInt
  (* Comparison - float *)
  | OpEqFloat
  | OpNeFloat
  | OpLtFloat
  | OpGtFloat
  | OpLeFloat
  | OpGeFloat
  (* Comparison - decimal *)
  | OpEqDecimal
  | OpNeDecimal
  | OpLtDecimal
  | OpGtDecimal
  | OpLeDecimal
  | OpGeDecimal
  (* Comparison - string *)
  | OpEqStr
  | OpNeStr
  (* Comparison - bool *)
  | OpEqBool
  | OpNeBool
  (* Logical *)
  | OpAndBool
  | OpOrBool
  | OpNotBool
  (* Type conversions *)
  | OpCastIntFloat
  | OpCastFloatInt
  | OpCastIntDecimal
  | OpCastDecimalInt
  | OpCastFloatDecimal
  | OpCastDecimalFloat
  (* Math functions *)
  | OpSqrtFloat
  | OpPowFloat
  | OpAbsInt
  | OpAbsFloat
  | OpMinInt
  | OpMinFloat
  | OpMaxInt
  | OpMaxFloat
  (* Control flow *)
  | OpJump of int  (* offset *)
  | OpJumpIfFalse of int
  | OpJumpIfTrue of int
  | OpCall of string  (* function name *)
  | OpReturn
  (* I/O - Simple print for now *)
  | OpPrintInt
  | OpPrintFloat
  | OpPrintString
  | OpPrintBool
  | OpPrintNewline
  | OpReadLine
  (* String operations *)
  | OpStrLen
  | OpStrConcat
  | OpStrSlice
  | OpStrFromInt
  | OpStrFromFloat
  (* Array operations *)
  | OpArrayNew
  | OpArrayPush
  | OpArrayGet
  | OpArraySet
  | OpArrayLen
  (* Map operations *)
  | OpMapNew
  | OpMapSet
  | OpMapGet
  | OpMapHas
  | OpMapDelete
  | OpMapLen
  | OpMapKeys
  (* File operations *)
  | OpFileRead
  | OpFileWrite
  | OpFileExists

type instruction = {
  op : opcode;
  line : int;  (* For debugging *)
}

type function_def = {
  name : string;
  arity : int;
  locals_count : int;
  instructions : instruction list;
}

type program = {
  functions : function_def list;
  entry_point : string;  (* Usually "main" *)
}

let string_of_opcode = function
  | OpPushInt n -> "PUSH_INT " ^ Int64.to_string n
  | OpPushFloat f -> "PUSH_FLOAT " ^ string_of_float f
  | OpPushString s -> "PUSH_STRING \"" ^ String.escaped s ^ "\""
  | OpPushBool b -> "PUSH_BOOL " ^ string_of_bool b
  | OpPushDecimal s -> "PUSH_DECIMAL " ^ s
  | OpPushUnit -> "PUSH_UNIT"
  | OpPop -> "POP"
  | OpDup -> "DUP"
  | OpLoadLocal i -> "LOAD_LOCAL " ^ string_of_int i
  | OpStoreLocal i -> "STORE_LOCAL " ^ string_of_int i
  | OpAddInt -> "ADD_INT"
  | OpSubInt -> "SUB_INT"
  | OpMulInt -> "MUL_INT"
  | OpDivInt -> "DIV_INT"
  | OpModInt -> "MOD_INT"
  | OpNegInt -> "NEG_INT"
  | OpAddFloat -> "ADD_FLOAT"
  | OpSubFloat -> "SUB_FLOAT"
  | OpMulFloat -> "MUL_FLOAT"
  | OpDivFloat -> "DIV_FLOAT"
  | OpNegFloat -> "NEG_FLOAT"
  | OpAddDecimal -> "ADD_DECIMAL"
  | OpSubDecimal -> "SUB_DECIMAL"
  | OpMulDecimal -> "MUL_DECIMAL"
  | OpDivDecimal -> "DIV_DECIMAL"
  | OpNegDecimal -> "NEG_DECIMAL"
  | OpEqInt -> "EQ_INT"
  | OpNeInt -> "NE_INT"
  | OpLtInt -> "LT_INT"
  | OpGtInt -> "GT_INT"
  | OpLeInt -> "LE_INT"
  | OpGeInt -> "GE_INT"
  | OpEqFloat -> "EQ_FLOAT"
  | OpNeFloat -> "NE_FLOAT"
  | OpLtFloat -> "LT_FLOAT"
  | OpGtFloat -> "GT_FLOAT"
  | OpLeFloat -> "LE_FLOAT"
  | OpGeFloat -> "GE_FLOAT"
  | OpEqDecimal -> "EQ_DECIMAL"
  | OpNeDecimal -> "NE_DECIMAL"
  | OpLtDecimal -> "LT_DECIMAL"
  | OpGtDecimal -> "GT_DECIMAL"
  | OpLeDecimal -> "LE_DECIMAL"
  | OpGeDecimal -> "GE_DECIMAL"
  | OpEqStr -> "EQ_STR"
  | OpNeStr -> "NE_STR"
  | OpEqBool -> "EQ_BOOL"
  | OpNeBool -> "NE_BOOL"
  | OpAndBool -> "AND_BOOL"
  | OpOrBool -> "OR_BOOL"
  | OpNotBool -> "NOT_BOOL"
  | OpCastIntFloat -> "CAST_INT_FLOAT"
  | OpCastFloatInt -> "CAST_FLOAT_INT"
  | OpCastIntDecimal -> "CAST_INT_DECIMAL"
  | OpCastDecimalInt -> "CAST_DECIMAL_INT"
  | OpCastFloatDecimal -> "CAST_FLOAT_DECIMAL"
  | OpCastDecimalFloat -> "CAST_DECIMAL_FLOAT"
  | OpSqrtFloat -> "SQRT_FLOAT"
  | OpPowFloat -> "POW_FLOAT"
  | OpAbsInt -> "ABS_INT"
  | OpAbsFloat -> "ABS_FLOAT"
  | OpMinInt -> "MIN_INT"
  | OpMinFloat -> "MIN_FLOAT"
  | OpMaxInt -> "MAX_INT"
  | OpMaxFloat -> "MAX_FLOAT"
  | OpJump offset -> "JUMP " ^ string_of_int offset
  | OpJumpIfFalse offset -> "JUMP_IF_FALSE " ^ string_of_int offset
  | OpJumpIfTrue offset -> "JUMP_IF_TRUE " ^ string_of_int offset
  | OpCall name -> "CALL " ^ name
  | OpReturn -> "RETURN"
  | OpPrintInt -> "PRINT_INT"
  | OpPrintFloat -> "PRINT_FLOAT"
  | OpPrintString -> "PRINT_STRING"
  | OpPrintBool -> "PRINT_BOOL"
  | OpPrintNewline -> "PRINT_NEWLINE"
  | OpReadLine -> "READ_LINE"
  | OpStrLen -> "STR_LEN"
  | OpStrConcat -> "STR_CONCAT"
  | OpStrSlice -> "STR_SLICE"
  | OpStrFromInt -> "STR_FROM_INT"
  | OpStrFromFloat -> "STR_FROM_FLOAT"
  | OpArrayNew -> "ARRAY_NEW"
  | OpArrayPush -> "ARRAY_PUSH"
  | OpArrayGet -> "ARRAY_GET"
  | OpArraySet -> "ARRAY_SET"
  | OpArrayLen -> "ARRAY_LEN"
  | OpMapNew -> "MAP_NEW"
  | OpMapSet -> "MAP_SET"
  | OpMapGet -> "MAP_GET"
  | OpMapHas -> "MAP_HAS"
  | OpMapDelete -> "MAP_DELETE"
  | OpMapLen -> "MAP_LEN"
  | OpMapKeys -> "MAP_KEYS"
  | OpFileRead -> "FILE_READ"
  | OpFileWrite -> "FILE_WRITE"
  | OpFileExists -> "FILE_EXISTS"

let print_instruction inst =
  Printf.printf "  %04d: %s\n" inst.line (string_of_opcode inst.op)

let print_function func =
  Printf.printf "Function %s (arity=%d, locals=%d):\n"
    func.name func.arity func.locals_count;
  List.iter print_instruction func.instructions;
  Printf.printf "\n"

let print_program prog =
  Printf.printf "AISL Bytecode Program\n";
  Printf.printf "Entry point: %s\n\n" prog.entry_point;
  List.iter print_function prog.functions
