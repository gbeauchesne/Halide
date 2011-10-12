open Analysis
open Ir
open Ir_printer

let _ =
  let x = Var (i32, "x") and y = Var (i32, "y") and z = Var (i32, "z") in
  let test_set = [(IntImm 17) +~ x; 
                  x +~ (IntImm 17);
                  Select (y >~ (IntImm 14), FloatImm 14.0, FloatImm 15.0);
                  Select (y >~ (IntImm 14), FloatImm 14.0, FloatImm 14.0);
                  FloatImm(14.0);
                  IntImm(0);
                  x -~ x;
                  x *~ (y +~ z);
                  (x *~ y) +~ (x *~ z);
                  (Load ((Float 32), "im0", x))
                 ] in
  List.iter (fun e -> 
    let (a, b, c, d) = hash_expr e in
    Printf.printf "Expr: %s\nHash: %d %d %d %d\n\n%!" (string_of_expr e) a b c d)
  test_set
