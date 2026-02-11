(* AISL Compiler - Main Entry Point *)

open Lexer
open Parser
open Ast

let compile_file filename =
  try
    (* Read the file *)
    let ic = open_in filename in
    let content = really_input_string ic (in_channel_length ic) in
    close_in ic;
    
    (* Lex *)
    Printf.printf "Lexing %s...\n" filename;
    let tokens = tokenize content in
    
    (* Parse *)
    Printf.printf "Parsing...\n" ;
    let module_def = parse tokens in
    
    Printf.printf "Successfully compiled module: %s\n" module_def.module_name;
    Printf.printf "Functions: %d\n" (List.length module_def.module_functions);
    Printf.printf "Imports: %d\n" (List.length module_def.module_imports);
    
    (* Print AST for now *)
    List.iter (fun func ->
      Printf.printf "\nFunction: %s\n" func.func_name;
      Printf.printf "  Parameters: %d\n" (List.length func.func_params);
      Printf.printf "  Return type: %s\n" (Types.string_of_type func.func_return_type);
      Printf.printf "  Body statements: %d\n" (List.length func.func_body);
    ) module_def.module_functions;
    
    0
  with
  | Sys_error msg ->
      Printf.eprintf "Error reading file: %s\n" msg;
      1
  | LexError msg ->
      Printf.eprintf "Lexer error: %s\n" msg;
      1
  | ParseError msg ->
      Printf.eprintf "Parse error: %s\n" msg;
      1
  | e ->
      Printf.eprintf "Unexpected error: %s\n" (Printexc.to_string e);
      1

let () =
  if Array.length Sys.argv < 2 then begin
    Printf.eprintf "Usage: %s <file.aisl>\n" Sys.argv.(0);
    exit 1
  end;
  
  let filename = Sys.argv.(1) in
  let exit_code = compile_file filename in
  exit exit_code
