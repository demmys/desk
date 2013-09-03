module Fuga where

data Fuga = F | Fuga Int
          deriving(Show)

fuga :: Int -> Fuga
fuga x = Fuga x
