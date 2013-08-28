public class Test{
    /*
     * plus :: Int -> Int -> Int
     */
    private static class Plus extends Thunk{
        public Plus(Thunk... args){
            super(2, args);
        }

        @Override
            public LazyValue eval(){
                int n1 = args[0].eval().getInt();
                int n2 = args[1].eval().getInt();
                return new LazyValue(n1 + n2);
            }
    }

    /*
     * sum :: [Int] -> Int
     */
    private static class Sum extends Thunk{
        public Sum(Thunk... args){
            super(1, args);
        }

        @Override
            public LazyValue eval(){
                Thunk res = LazyMethod.foldl(new Plus(), new LazyValue(0), (Thunk.Array)args[0]);
                return res.eval();
            }
    }

    /*
     * range :: Int -> Int -> [Int]
     */
    private static class Range extends Thunk{
        public Range(Thunk... args){
            super(2, args);
        }

        @Override
            public LazyValue eval(){
                int n1 = args[0].eval().getInt();
                int n2 = args[1].eval().getInt();
                Thunk.Array res = new Thunk.Array(n2 - n1);
                while(n1 < n2){
                    res.apply(new LazyValue(n1++));
                }
            }
    }

    public static void main(String[] args){
        int arg = Integer.parseInt(args[0]);
        /*
        Thunk.Array array = new Thunk.Array(arg);
        for(int i = 0; i < arg; i++){
            array.apply(new LazyValue(i));
        }
        */
        Thunk sum = new Sum(array);
        int res = sum.eval().getInt();
        System.out.println(res);
    }
}
