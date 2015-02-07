import Text.Printf

data Expr = Value Int
          | Plus  Expr Expr
          | Minus Expr Expr
          | Mult  Expr Expr
          | Div   Expr Expr
 
eval :: Expr -> Int
eval (Value n)      = n
eval (Plus  a b) = eval a + eval b
eval (Minus a b) = eval a - eval b
eval (Mult  a b) = eval a * eval b
eval (Div   a b) = eval a `div` eval b

toString :: Expr -> String
toString (Value n)      = show n
toString (Plus  a b) = "(" ++ toString a ++ "+" ++ toString b ++ ")"
toString (Minus a b) = "(" ++ toString a ++ "-" ++ toString b ++ ")"
toString (Mult  a b) = "(" ++ toString a ++ "*" ++ toString b ++ ")"
toString (Div   a b) = "(" ++ toString a ++ "/" ++ toString b ++ ")"

main :: IO ()
main = do
    let e = Div (Plus (Value 17) (Value 25)) (Minus (Value 3) (Value 1))
    printf "Hello, world! %d\n" (eval e)
    printf "%s\n" (toString(e))
