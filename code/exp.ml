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

     	
type card = Card of regular | Joker
and regular = { suit : card_suit; name : card_name; }
and card_suit = Heart | Club | Spade | Diamond
and card_name =  Ace | King | Queen | Jack | Simple of int;;

type entry = { name:string; spouse:string option; };;

datatype DT = C1 of {L11:int, L1m:int} 
            | Cr of {Lr1:int, Lrn:int}

Printf.printf "%d\n" (eval (Plus (Times (Value 1, Value 2), Times (Value 3, Value 4))));;
