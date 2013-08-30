public class LazyOperator{
    /*
     * add :: Int -> Int -> Int
     */
    public static class Add extends Thunk{
        public Add(Thunk... args){
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
     * sub :: Int -> Int -> Int
     */
    public static class Sub extends Thunk{
        public Sub(Thunk... args){
            super(2, args);
        }

        @Override
            public LazyValue eval(){
                int n1 = args[0].eval().getInt();
                int n2 = args[1].eval().getInt();
                return new LazyValue(n1 - n2);
            }
    }

    /*
     * mul :: Int -> Int -> Int
     */
    public static class Mul extends Thunk{
        public Mul(Thunk... args){
            super(2, args);
        }

        @Override
            public LazyValue eval(){
                int n1 = args[0].eval().getInt();
                int n2 = args[1].eval().getInt();
                return new LazyValue(n1 * n2);
            }
    }

    /*
     * div :: Int -> Int -> Int
     */
    public static class Div extends Thunk{
        public Div(Thunk... args){
            super(2, args);
        }

        @Override
            public LazyValue eval(){
                int n1 = args[0].eval().getInt();
                int n2 = args[1].eval().getInt();
                return new LazyValue(n1 / n2);
            }
    }

    /*
     * mod :: Int -> Int -> Int
     */
    public static class Mod extends Thunk{
        public Mod(Thunk... args){
            super(2, args);
        }

        @Override
            public LazyValue eval(){
                int n1 = args[0].eval().getInt();
                int n2 = args[1].eval().getInt();
                return new LazyValue(n1 % n2);
            }
    }
}
