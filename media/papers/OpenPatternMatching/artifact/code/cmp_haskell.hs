import Text.Printf
import Data.List
import System.CPUTime.Rdtsc

data Shape = Shape00 | Shape01 | Shape02 | Shape03 | Shape04 | Shape05 | Shape06 | Shape07 | Shape08 | Shape09
           | Shape10 | Shape11 | Shape12 | Shape13 | Shape14 | Shape15 | Shape16 | Shape17 | Shape18 | Shape19
           | Shape20 | Shape21 | Shape22 | Shape23 | Shape24 | Shape25 | Shape26 | Shape27 | Shape28 | Shape29
           | Shape30 | Shape31 | Shape32 | Shape33 | Shape34 | Shape35 | Shape36 | Shape37 | Shape38 | Shape39
           | Shape40 | Shape41 | Shape42 | Shape43 | Shape44 | Shape45 | Shape46 | Shape47 | Shape48 | Shape49
           | Shape50 | Shape51 | Shape52 | Shape53 | Shape54 | Shape55 | Shape56 | Shape57 | Shape58 | Shape59
           | Shape60 | Shape61 | Shape62 | Shape63 | Shape64 | Shape65 | Shape66 | Shape67 | Shape68 | Shape69
           | Shape70 | Shape71 | Shape72 | Shape73 | Shape74 | Shape75 | Shape76 | Shape77 | Shape78 | Shape79
           | Shape80 | Shape81 | Shape82 | Shape83 | Shape84 | Shape85 | Shape86 | Shape87 | Shape88 | Shape89
           | Shape90 | Shape91 | Shape92 | Shape93 | Shape94 | Shape95 | Shape96 | Shape97 | Shape98 | Shape99

make_shape n = case n of
                    0  -> Shape00 
                    1  -> Shape01
                    2  -> Shape02
                    3  -> Shape03
                    4  -> Shape04
                    5  -> Shape05
                    6  -> Shape06
                    7  -> Shape07
                    8  -> Shape08
                    9  -> Shape09
                    10 -> Shape10
                    11 -> Shape11
                    12 -> Shape12
                    13 -> Shape13
                    14 -> Shape14
                    15 -> Shape15
                    16 -> Shape16
                    17 -> Shape17
                    18 -> Shape18
                    19 -> Shape19
                    20 -> Shape20
                    21 -> Shape21
                    22 -> Shape22
                    23 -> Shape23
                    24 -> Shape24
                    25 -> Shape25
                    26 -> Shape26
                    27 -> Shape27
                    28 -> Shape28
                    29 -> Shape29
                    30 -> Shape30
                    31 -> Shape31
                    32 -> Shape32
                    33 -> Shape33
                    34 -> Shape34
                    35 -> Shape35
                    36 -> Shape36
                    37 -> Shape37
                    38 -> Shape38
                    39 -> Shape39
                    40 -> Shape40
                    41 -> Shape41
                    42 -> Shape42
                    43 -> Shape43
                    44 -> Shape44
                    45 -> Shape45
                    46 -> Shape46
                    47 -> Shape47
                    48 -> Shape48
                    49 -> Shape49
                    50 -> Shape50
                    51 -> Shape51
                    52 -> Shape52
                    53 -> Shape53
                    54 -> Shape54
                    55 -> Shape55
                    56 -> Shape56
                    57 -> Shape57
                    58 -> Shape58
                    59 -> Shape59
                    60 -> Shape60
                    61 -> Shape61
                    62 -> Shape62
                    63 -> Shape63
                    64 -> Shape64
                    65 -> Shape65
                    66 -> Shape66
                    67 -> Shape67
                    68 -> Shape68
                    69 -> Shape69
                    70 -> Shape70
                    71 -> Shape71
                    72 -> Shape72
                    73 -> Shape73
                    74 -> Shape74
                    75 -> Shape75
                    76 -> Shape76
                    77 -> Shape77
                    78 -> Shape78
                    79 -> Shape79
                    80 -> Shape80
                    81 -> Shape81
                    82 -> Shape82
                    83 -> Shape83
                    84 -> Shape84
                    85 -> Shape85
                    86 -> Shape86
                    87 -> Shape87
                    88 -> Shape88
                    89 -> Shape89
                    90 -> Shape90
                    91 -> Shape91
                    92 -> Shape92
                    93 -> Shape93
                    94 -> Shape94
                    95 -> Shape95
                    96 -> Shape96
                    97 -> Shape97
                    98 -> Shape98
                    99 -> Shape99

do_match Shape00 =  0
do_match Shape01 =  1
do_match Shape02 =  2
do_match Shape03 =  3
do_match Shape04 =  4
do_match Shape05 =  5
do_match Shape06 =  6
do_match Shape07 =  7
do_match Shape08 =  8
do_match Shape09 =  9
do_match Shape10 = 10
do_match Shape11 = 11
do_match Shape12 = 12
do_match Shape13 = 13
do_match Shape14 = 14
do_match Shape15 = 15
do_match Shape16 = 16
do_match Shape17 = 17
do_match Shape18 = 18
do_match Shape19 = 19
do_match Shape20 = 20
do_match Shape21 = 21
do_match Shape22 = 22
do_match Shape23 = 23
do_match Shape24 = 24
do_match Shape25 = 25
do_match Shape26 = 26
do_match Shape27 = 27
do_match Shape28 = 28
do_match Shape29 = 29
do_match Shape30 = 30
do_match Shape31 = 31
do_match Shape32 = 32
do_match Shape33 = 33
do_match Shape34 = 34
do_match Shape35 = 35
do_match Shape36 = 36
do_match Shape37 = 37
do_match Shape38 = 38
do_match Shape39 = 39
do_match Shape40 = 40
do_match Shape41 = 41
do_match Shape42 = 42
do_match Shape43 = 43
do_match Shape44 = 44
do_match Shape45 = 45
do_match Shape46 = 46
do_match Shape47 = 47
do_match Shape48 = 48
do_match Shape49 = 49
do_match Shape50 = 50
do_match Shape51 = 51
do_match Shape52 = 52
do_match Shape53 = 53
do_match Shape54 = 54
do_match Shape55 = 55
do_match Shape56 = 56
do_match Shape57 = 57
do_match Shape58 = 58
do_match Shape59 = 59
do_match Shape60 = 60
do_match Shape61 = 61
do_match Shape62 = 62
do_match Shape63 = 63
do_match Shape64 = 64
do_match Shape65 = 65
do_match Shape66 = 66
do_match Shape67 = 67
do_match Shape68 = 68
do_match Shape69 = 69
do_match Shape70 = 70
do_match Shape71 = 71
do_match Shape72 = 72
do_match Shape73 = 73
do_match Shape74 = 74
do_match Shape75 = 75
do_match Shape76 = 76
do_match Shape77 = 77
do_match Shape78 = 78
do_match Shape79 = 79
do_match Shape80 = 80
do_match Shape81 = 81
do_match Shape82 = 82
do_match Shape83 = 83
do_match Shape84 = 84
do_match Shape85 = 85
do_match Shape86 = 86
do_match Shape87 = 87
do_match Shape88 = 88
do_match Shape89 = 89
do_match Shape90 = 90
do_match Shape91 = 91
do_match Shape92 = 92
do_match Shape93 = 93
do_match Shape94 = 94
do_match Shape95 = 95
do_match Shape96 = 96
do_match Shape97 = 97
do_match Shape98 = 98
do_match Shape99 = 99

--test1 :: Int -> [Shape] -> Int
--test1 a []     = a
--test1 a (s:xs) = let a1 = (a + (do_match s)) 
--                 in test1 a1 xs

dummy :: a -> a
dummy a = a

test1 :: [Shape] -> Int -> Int
test1 []     a = a
test1 (s:xs) a = test1 xs $! ((+) (do_match s) $! a)

--test2 :: Int -> [Shape] -> Int
--test2 a xs =
--  test1 xs a
--  where test1 []     a = a
--        test1 (s:xs) a = test1 xs $! ((+) (do_match s) $! a)
--        --test1 (s:xs) a = let b = (do_match $! s) + a in test1 xs $! b


test :: Int -> Int -> [Shape] -> Int
test 0 a _      = a
test k a shapes = test (k-1) (test1 shapes a) shapes

main :: IO ()
main = do
    let n = 1000000 -- The amount of times visitor and matching procedure is invoked in one time measuring
    let m = 101     -- The amount of times time measuring is done
    let k = 100     -- Maximum number of shapes that can be created
    let shapes  = [make_shape (i `mod` k) | i <- [1..n]]
    return $! length shapes

    t1 <- rdtsc
    let matches = map do_match shapes
    return $! matches
    t2 <- rdtsc

    t3 <- rdtsc
    let t = test m 0 shapes
    return $! t
    t4 <- rdtsc
    let secs = (fromIntegral(t4 - t3)::Double) / ((fromIntegral(m)::Double)*2530000000)
    printf "\nAverage time for %d runs takes %.5f seconds: %d\n" (n::Int) secs (t::Int)
