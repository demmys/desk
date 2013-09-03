module MyInt(inc, dec) where

data MyInt a = O | Succ (MyInt a) | Prev (MyInt a)
             deriving(Show)

inc :: MyInt a -> MyInt a
inc (Prev x) = x
inc x = Succ x

dec :: MyInt a -> MyInt a
dec (Succ x) = x
dec x = Prev x
