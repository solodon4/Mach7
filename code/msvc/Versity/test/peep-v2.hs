pass :: [Inst] -> [Inst]
pass = peep1 . peep2 . peep3 . removeUnused

peep3 (x1:x2:x3:xs) = case (x1, x2, x3) of
    (InstMove a b, InstMove c d, InstMove _ f)        | (b == c) && (b == f)             -> peep3 $ (InstMove a d):x3:xs
    (InstMove a b, InstMove c d, InstUn _ _ f)        | (b == c) && (b == f)             -> peep3 $ (InstMove a d):x3:xs
    (InstMove a b, InstMove c d, InstBin _ _ _ f)     | (b == c) && (b == f)             -> peep3 $ (InstMove a d):x3:xs

    (InstMove a b, InstUn _ c d, InstMove _ f)        | (b == c) && (b == f)             -> peep3 $ (InstMove a d):x3:xs
    (InstMove a b, InstUn _ c d, InstUn op _ f)       | (b == c) && (b == f)             -> peep3 $ (InstMove a d):x3:xs
    (InstMove a b, InstUn _ c d, InstBin _ _ _ f)     | (b == c) && (b == f)             -> peep3 $ (InstMove a d):x3:xs

    (InstMove a b, InstMove _ _, InstMove e g)        | (b == g) && (b == e)             -> peep3 $ x2:(InstMove a g):x3:xs
    (InstMove a b, InstMove _ _, InstUn op e g)       | (b == g) && (b == e)             -> peep3 $ x2:(InstUn op a g):x3:xs
    (InstMove a b, InstMove _ _, InstBin op e g f)    | (b == g) && (b == e) && (b /= f) -> peep3 $ x2:(InstBin op a f g):xs
    (InstMove a b, InstMove _ _, InstBin op e g f)    | (b == g) && (b /= e) && (b == f) -> peep3 $ x2:(InstBin op e a g):xs
    (InstMove a b, InstMove _ _, InstBin op e g f)    | (b == g) && (b == e) && (b == f) -> peep3 $ x2:(InstBin op a a g):xs

    (InstMove a b, InstBin op c d e, InstMove _ f)    | (b == d) && (b /= c) && (b == f) -> peep3 $ (InstBin op c a e):x3:xs
    (InstMove a b, InstBin op c d e, InstUn _ _ f)    | (b == d) && (b /= c) && (b == f) -> peep3 $ (InstBin op c a e):x3:xs
    (InstMove a b, InstBin op c d e, InstBin _ _ _ f) | (b == d) && (b /= c) && (b == f) -> peep3 $ (InstBin op c a e):x3:xs

    (InstMove a b, InstBin op c d e, InstMove _ f)    | (b /= d) && (b == c) && (b == f) -> peep3 $ (InstBin op c d e):x3:xs
    (InstMove a b, InstBin op c d e, InstUn _ _ f)    | (b /= d) && (b == c) && (b == f) -> peep3 $ (InstBin op c d e):x3:xs
    (InstMove a b, InstBin op c d e, InstBin _ _ _ f) | (b /= d) && (b == c) && (b == f) -> peep3 $ (InstBin op c d e):x3:xs

    (InstMove a b, InstBin op c d e, InstMove _ f)    | (b == d) && (b == c) && (b == f) -> peep3 $ (InstBin op a a e):x3:xs
    (InstMove a b, InstBin op c d e, InstUn _ _ f)    | (b == d) && (b == c) && (b == f) -> peep3 $ (InstBin op a a e):x3:xs
    (InstMove a b, InstBin op c d e, InstBin _ _ _ f) | (b == d) && (b == c) && (b == f) -> peep3 $ (InstBin op a a e):x3:xs

    _ -> x1:(peep3 $ x2:x3:xs)

peep3 (x:xs) = x:(peep3 xs)
peep3 [] = []

peep2 (x1:x2:xs) = case x1 of
    InstMove a b -> case x2 of
                         InstMove c d     | (a == d) && (b == c)               -> peep2 $ x1:xs
                         InstMove c d     | (b == c)                           -> peep2 $ renameRegsMove a c d xs
                         InstUn  op c d   | (b == d) && (b == c)               -> peep2 $ (InstUn  op a b  ):xs
                         InstBin op c d e | (b == e) && ((c == b) && (d /= b)) -> peep2 $ (InstBin op a d e):xs
                         InstBin op c d e | (b == e) && ((c /= b) && (d == b)) -> peep2 $ (InstBin op c a e):xs
                         InstBin op c d e | (b == e) && ((c == b) && (d == b)) -> peep2 $ (InstBin op a a e):xs
                         _ -> x1:(peep2 $ x2:xs)
    InstUn op a b -> case x2 of 
                         InstMove c d     | (b == c)                           -> peep2 $ renameRegsUn op a c d xs
                         _ -> x1:(peep2 $ x2:xs)
    InstBin op a b c -> case x2 of 
                         InstMove d e     | (c == d)                           -> peep2 $ renameRegsBin op a b d e xs
                         _ -> x1:(peep2 $ x2:xs)

peep2 (x:xs) = x:(peep2 xs)
peep2 [] = []

peep1 (x1:xs) = case x1 of
    InstMove a b | a == b -> peep1 xs
                 | otherwise -> if doRename
                                   then x1:(renameRegs src dst xs)
                                   else x1:(peep1 xs)
                                where (doRename, src, dst) = chooseReg a b
                                      rename reg = if reg == src
                                                      then dst
                                                      else reg
    _ -> x1:(peep1 xs)