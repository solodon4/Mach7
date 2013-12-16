(* Similar examples are used in http://ocaml.org/tutorials/data_types_and_matching.html#Pattern_matching__on_datatypes_ *)

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

let rec equal a b =
  match (a,b) with
    (Plus  (l1, r1), Plus  (l2, r2))
  | (Minus (l1, r1), Minus (l2, r2))
  | (Times (l1, r1), Times (l2, r2))
  | (Divide(l1, r1), Divide(l2, r2)) -> (equal l1 l2) && (equal r1 r2)
  | (Value (v1),     Value (v2)    ) -> v1 == v2
  | _                                -> false
  ;;
     	
type card = Card of regular | Joker
and regular = { suit : card_suit; name : card_name; }
and card_suit = Heart | Club | Spade | Diamond
and card_name =  Ace | King | Queen | Jack | Simple of int;;

type entry = { name:string; spouse:string option; };;

Printf.printf "%d\n" (eval (Plus (Times (Value 1, Value 2), Times (Value 3, Value 4))));;
Printf.printf "%b\n" (equal (Plus (Times (Value 1, Value 2), Times (Value 3, Value 4))) 
                            (Plus (Times (Value 1, Value 2), Minus (Value 3, Value 4)))
                     );;
