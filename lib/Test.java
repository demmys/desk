public class Test{
    /*
     * sum :: [Int] -> Int
     */
    private static class Sum extends Thunk{
        public Sum(Thunk... args){
            super(1, args);
        }

        @Override
            public LazyValue eval(){
                Thunk res = LazyMethod.foldl(new LazyOperator.Add(), new LazyValue(0), (Thunk.Array)args[0]);
                return res.eval();
            }
    }

    /*
     * fibonacci :: Int -> Int
     */
    private static class Fibonacci extends Thunk{
        public Fibonacci(Thunk... args){
            super(1, args);
        }

        @Override
            public LazyValue eval(){
                int n = args[0].eval().getInt();
                if(n < 3)
                    return new LazyValue(1);
                int a1 = new Fibonacci(new LazyValue(n - 2)).eval().getInt();
                int a2 = new Fibonacci(new LazyValue(n - 1)).eval().getInt();
                return new LazyValue(a1 + a2);
            }
    }

    public static void main(String[] args){
        int arg = Integer.parseInt(args[0]);
        Thunk.Array a = LazyMethod.range(new LazyValue(1), new LazyValue(arg));
        //Thunk sum = new Sum(a);
        //int res = sum.eval().getInt();
        //System.out.println(res);
        Thunk.Array fib = LazyMethod.map(new Fibonacci(), a);
        Thunk[] fibs = fib.get();
        for(int i = 0; i < fibs.length; i++){
            System.out.println(fibs[i].eval().getInt());
        }
    }
}
