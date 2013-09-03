module Bar where

bar :: Int -> Int
bar x = bar $ x + 1

barbar = 0 : map bar [1, 2, 3]

headb :: [a] -> a
headb [] = error "hoge"
headb (x:xs) = x
