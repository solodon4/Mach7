type expr = Plus   of expr * expr      (* means a + b *)
          | Minus  of expr * expr      (* means a - b *)
          | Times  of expr * expr      (* means a * b *)
          | Divide of expr * expr      (* means a / b *)
          | Value  of int
          ;;

let rec eval e =
  match e with
    Plus   (left, right) -> (eval left) + (eval right)
  | Minus  (left, right) -> (eval left) - (eval right)
  | Times  (left, right) -> (eval left) * (eval right)
  | Divide (left, right) -> (eval left) / (eval right)
  | Value v              -> v
  ;;

Printf.printf "%d\n" (eval (Plus (Times (Value 1, Value 2), Times (Value 3, Value 4))));;
