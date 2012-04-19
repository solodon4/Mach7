(*#load "unix.cma";;*)

Random.init(int_of_float(Unix.time()));;

(*-----------------------------*)

type shape = Shape00 | Shape01 | Shape02 | Shape03 | Shape04 | Shape05 | Shape06 | Shape07 | Shape08 | Shape09
           | Shape10 | Shape11 | Shape12 | Shape13 | Shape14 | Shape15 | Shape16 | Shape17 | Shape18 | Shape19
           | Shape20 | Shape21 | Shape22 | Shape23 | Shape24 | Shape25 | Shape26 | Shape27 | Shape28 | Shape29
           | Shape30 | Shape31 | Shape32 | Shape33 | Shape34 | Shape35 | Shape36 | Shape37 | Shape38 | Shape39
           | Shape40 | Shape41 | Shape42 | Shape43 | Shape44 | Shape45 | Shape46 | Shape47 | Shape48 | Shape49
           | Shape50 | Shape51 | Shape52 | Shape53 | Shape54 | Shape55 | Shape56 | Shape57 | Shape58 | Shape59
           | Shape60 | Shape61 | Shape62 | Shape63 | Shape64 | Shape65 | Shape66 | Shape67 | Shape68 | Shape69
           | Shape70 | Shape71 | Shape72 | Shape73 | Shape74 | Shape75 | Shape76 | Shape77 | Shape78 | Shape79
           | Shape80 | Shape81 | Shape82 | Shape83 | Shape84 | Shape85 | Shape86 | Shape87 | Shape88 | Shape89
           | Shape90 | Shape91 | Shape92 | Shape93 | Shape94 | Shape95 | Shape96 | Shape97 | Shape98 | Shape99
           ;;

let make_shape n =
	match n with
              0 -> Shape00 |  1 -> Shape01 |  2 -> Shape02 |  3 -> Shape03 |  4 -> Shape04 |  5 -> Shape05 |  6 -> Shape06 |  7 -> Shape07 |  8 -> Shape08 |  9 -> Shape09
           | 10 -> Shape10 | 11 -> Shape11 | 12 -> Shape12 | 13 -> Shape13 | 14 -> Shape14 | 15 -> Shape15 | 16 -> Shape16 | 17 -> Shape17 | 18 -> Shape18 | 19 -> Shape19
           | 20 -> Shape20 | 21 -> Shape21 | 22 -> Shape22 | 23 -> Shape23 | 24 -> Shape24 | 25 -> Shape25 | 26 -> Shape26 | 27 -> Shape27 | 28 -> Shape28 | 29 -> Shape29
           | 30 -> Shape30 | 31 -> Shape31 | 32 -> Shape32 | 33 -> Shape33 | 34 -> Shape34 | 35 -> Shape35 | 36 -> Shape36 | 37 -> Shape37 | 38 -> Shape38 | 39 -> Shape39
           | 40 -> Shape40 | 41 -> Shape41 | 42 -> Shape42 | 43 -> Shape43 | 44 -> Shape44 | 45 -> Shape45 | 46 -> Shape46 | 47 -> Shape47 | 48 -> Shape48 | 49 -> Shape49
           | 50 -> Shape50 | 51 -> Shape51 | 52 -> Shape52 | 53 -> Shape53 | 54 -> Shape54 | 55 -> Shape55 | 56 -> Shape56 | 57 -> Shape57 | 58 -> Shape58 | 59 -> Shape59
           | 60 -> Shape60 | 61 -> Shape61 | 62 -> Shape62 | 63 -> Shape63 | 64 -> Shape64 | 65 -> Shape65 | 66 -> Shape66 | 67 -> Shape67 | 68 -> Shape68 | 69 -> Shape69
           | 70 -> Shape70 | 71 -> Shape71 | 72 -> Shape72 | 73 -> Shape73 | 74 -> Shape74 | 75 -> Shape75 | 76 -> Shape76 | 77 -> Shape77 | 78 -> Shape78 | 79 -> Shape79
           | 80 -> Shape80 | 81 -> Shape81 | 82 -> Shape82 | 83 -> Shape83 | 84 -> Shape84 | 85 -> Shape85 | 86 -> Shape86 | 87 -> Shape87 | 88 -> Shape88 | 89 -> Shape89
           | 90 -> Shape90 | 91 -> Shape91 | 92 -> Shape92 | 93 -> Shape93 | 94 -> Shape94 | 95 -> Shape95 | 96 -> Shape96 | 97 -> Shape97 | 98 -> Shape98 | 99 -> Shape99
           | _ -> Shape00
           ;;

let do_match s =
	match s with
             Shape00 ->  0 | Shape01 ->  1 | Shape02 ->  2 | Shape03 ->  3 | Shape04 ->  4 | Shape05 ->  5 | Shape06 ->  6 | Shape07 ->  7 | Shape08 ->  8 | Shape09 ->  9
           | Shape10 -> 10 | Shape11 -> 11 | Shape12 -> 12 | Shape13 -> 13 | Shape14 -> 14 | Shape15 -> 15 | Shape16 -> 16 | Shape17 -> 17 | Shape18 -> 18 | Shape19 -> 19
           | Shape20 -> 20 | Shape21 -> 21 | Shape22 -> 22 | Shape23 -> 23 | Shape24 -> 24 | Shape25 -> 25 | Shape26 -> 26 | Shape27 -> 27 | Shape28 -> 28 | Shape29 -> 29
           | Shape30 -> 30 | Shape31 -> 31 | Shape32 -> 32 | Shape33 -> 33 | Shape34 -> 34 | Shape35 -> 35 | Shape36 -> 36 | Shape37 -> 37 | Shape38 -> 38 | Shape39 -> 39
           | Shape40 -> 40 | Shape41 -> 41 | Shape42 -> 42 | Shape43 -> 43 | Shape44 -> 44 | Shape45 -> 45 | Shape46 -> 46 | Shape47 -> 47 | Shape48 -> 48 | Shape49 -> 49
           | Shape50 -> 50 | Shape51 -> 51 | Shape52 -> 52 | Shape53 -> 53 | Shape54 -> 54 | Shape55 -> 55 | Shape56 -> 56 | Shape57 -> 57 | Shape58 -> 58 | Shape59 -> 59
           | Shape60 -> 60 | Shape61 -> 61 | Shape62 -> 62 | Shape63 -> 63 | Shape64 -> 64 | Shape65 -> 65 | Shape66 -> 66 | Shape67 -> 67 | Shape68 -> 68 | Shape69 -> 69
           | Shape70 -> 70 | Shape71 -> 71 | Shape72 -> 72 | Shape73 -> 73 | Shape74 -> 74 | Shape75 -> 75 | Shape76 -> 76 | Shape77 -> 77 | Shape78 -> 78 | Shape79 -> 79
           | Shape80 -> 80 | Shape81 -> 81 | Shape82 -> 82 | Shape83 -> 83 | Shape84 -> 84 | Shape85 -> 85 | Shape86 -> 86 | Shape87 -> 87 | Shape88 -> 88 | Shape89 -> 89
           | Shape90 -> 90 | Shape91 -> 91 | Shape92 -> 92 | Shape93 -> 93 | Shape94 -> 94 | Shape95 -> 95 | Shape96 -> 96 | Shape97 -> 97 | Shape98 -> 98 | Shape99 -> 99
           ;;

let n = 1000000 in (* The amount of times visitor and matching procedure is invoked in one time measuring *)
let m = 101     in (* The amount of times time measuring is done *)
let k = 100     in (* Maximum number of shapes that can be created *)
let z = ref 0 in
let total_time = ref 0. in
(*let array = Array.init n (fun _ -> make_shape(Random.int(k))) in*)
let array = Array.init n (fun i -> make_shape(i mod k)) in
(*let s = make_shape(42) in*)

for i = 1 to m do
  let before = Unix.gettimeofday() in

  for j = 0 to n-1 do
    z := !z + do_match(Array.get array j);
    (*z := !z + do_match(s);*)
  done;

  let time = Unix.gettimeofday() -. before in
  total_time := !total_time +. time;
done;

Printf.printf "\nAverage time for %d runs takes %.5f seconds: %d\n" n (!total_time /. float m) !z ;;
