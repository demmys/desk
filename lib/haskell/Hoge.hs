module Hoge where

hoge :: Int -> Int
hoge a = hoge $ a + 1

fuga :: IO ()
fuga = do let a = hoge 1
          putStrLn $ show a
